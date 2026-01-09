
/// @file FuncParser.cc
/// @brief FuncParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "FuncParser.h"
#include "dotlib/AstExpr.h"
#include "dotlib/AstValue.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス FuncParser
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncParser::FuncParser(
  const std::string& str,
  const FileRegion& loc
) : mScanner{str, loc}
{
}

// @brief 論理式を読み込む．
AstValuePtr
FuncParser::operator()()
{
  auto expr{read_expr(TokenType::END)};
  return AstValue::new_expr(std::move(expr));
}

// @brief primary を読み込む．
AstExprPtr
FuncParser::read_primary()
{
  auto token = mScanner.read_token();
  switch ( token.type() ) {
  case TokenType::LP:
    return read_expr(TokenType::RP);

  case TokenType::SYMBOL:
    {
      ShString name{mScanner.cur_string()};
      if ( name == "0" ) {
	return AstExpr::new_bool(false, token.loc());
      }
      if ( name == "1" ) {
	return AstExpr::new_bool(true, token.loc());
      }
      return AstExpr::new_string(name, token.loc());
    }

  default:
    break;
  }

  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_PARSER",
		  "Syntax error. number is expected.");
  throw std::invalid_argument{"syntax error"};
}

// @brief プライム付きの primary を読み込む．
AstExprPtr
FuncParser::read_primary2()
{
  Token token = mScanner.peek_token();
  if ( token.type() == TokenType::NOT ) {
    mScanner.accept_token();
    auto opr = read_primary();
    return AstExpr::new_not(std::move(opr), opr->loc());
  }

  auto expr = read_primary();
  token = mScanner.peek_token();
  if ( token.type() == TokenType::PRIME ) {
    mScanner.accept_token();
    return AstExpr::new_not(std::move(expr), token.loc());
  }

  return expr;
}

// @brief product を読み込む．
AstExprPtr
FuncParser::read_product()
{
  auto opr1 = read_primary2();
  for ( ; ; ) {
    auto token = mScanner.peek_token();
    switch ( token.type() ) {
    case TokenType::AND:
      {
	mScanner.accept_token();
	auto opr2 = read_primary2();
	if ( opr2 == nullptr ) {
	  return {};
	}
	opr1 = AstExpr::new_and(std::move(opr1), std::move(opr2), token.loc());
      }
      break;

    case TokenType::NOT:
    case TokenType::LP:
    case TokenType::SYMBOL:
      {
	auto opr2 = read_primary2();
	opr1 = AstExpr::new_and(std::move(opr1), std::move(opr2), token.loc());
      }
      break;

    default:
      return opr1;
    }
  }
}

// @brief expression を読み込む．
AstExprPtr
FuncParser::read_expr(
  TokenType end_marker
)
{
  auto opr1 = read_product();
  for ( ; ; ) {
    auto token = mScanner.read_token();
    auto type = token.type();
    auto loc = token.loc();
    if ( type == end_marker ) {
      return opr1;
    }
    if ( type == TokenType::OR || type == TokenType::XOR ) {
      auto opr2 = read_product();
      if ( type == TokenType::XOR ) {
	opr1 = AstExpr::new_xor(std::move(opr1), std::move(opr2), loc);
      }
      else {
	opr1 = AstExpr::new_or(std::move(opr1), std::move(opr2), loc);
      }
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      MsgType::Error,
		      "DOTLIB_PARSER",
		      "Syntax error.");
      throw std::invalid_argument{"Syntax error."};
    }
  }
}

END_NAMESPACE_YM_DOTLIB
