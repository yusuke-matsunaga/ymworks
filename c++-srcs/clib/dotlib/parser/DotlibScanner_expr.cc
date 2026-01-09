
/// @file Scanner.cc
/// @brief Scanner の実装ファイル(expr読み込み)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/DotlibScanner.h"
#include "dotlib/AstExpr.h"
#include "ym/MsgMgr.h"
#include "FuncParser.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief 式を表す AstValue を生成する．
AstValuePtr
DotlibScanner::read_expr()
{
  auto expr = _read_expr(TokenType::SEMI);
  return AstValue::new_expr(std::move(expr));
}

// @brief "式" を表す AstValue を生成する．
AstValuePtr
DotlibScanner::read_function()
{
  auto token = read_token();
  auto tmp_str = token.str_value();

  FuncParser read(tmp_str, token.loc());
  return AstValuePtr{read()};
}

// @brief expression を読み込む．
AstExprPtr
DotlibScanner::_read_expr(
  TokenType end_marker
)
{
  auto opr1 = _read_product();
  for ( ; ; ) {
    auto token = read_token();
    if ( token.type() == end_marker ) {
      return opr1;
    }
    if ( token.type() == TokenType::PLUS || token.type() == TokenType::MINUS ) {
      auto opr2 = _read_product();
      if ( token.type() == TokenType::PLUS ) {
	opr1 = AstExpr::new_plus(std::move(opr1), std::move(opr2), token.loc());
      }
      else {
	opr1 = AstExpr::new_minus(std::move(opr1), std::move(opr2), token.loc());
      }
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      token.loc(),
		      MsgType::Error,
		      "DOTLIB_PARSER",
		      "Syntax error.");
      throw std::invalid_argument{"Syntax error."};
    }
  }
}

// @brief primary を読み込む．
AstExprPtr
DotlibScanner::_read_primary()
{
  auto token = read_token();
  switch ( token.type() ) {
  case TokenType::LP:
    return _read_expr(TokenType::RP);

  case TokenType::SYMBOL:
    {
      auto name = token.str_value();
      if ( name == "VDD" ) {
	return AstExpr::new_vdd(token.loc());
      }
      if ( name == "VSS" ) {
	return AstExpr::new_vss(token.loc());
      }
      if ( name == "VCC" ) {
	return AstExpr::new_vcc(token.loc());
      }
      MsgMgr::put_msg(__FILE__, __LINE__,
		      token.loc(),
		      MsgType::Error,
		      "DOTLIB_PARSER",
		      "Syntax error. "
		      "Only 'VDD', 'VSS', and 'VCC' are allowed.");
      throw std::invalid_argument{"Syntax error."};
    }
    // ここには来ない．
    break;

  default:
    break;
  }

  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_PARSER",
		  "Syntax error. number is expected.");
  throw std::invalid_argument{"Syntax error."};
}

// @brief prudct を読み込む．
AstExprPtr
DotlibScanner::_read_product()
{
  auto opr1 = _read_primary();
  for ( ; ; ) {
    auto token = peek_token();
    if ( token.type() == TokenType::MULT || token.type() == TokenType::DIV ) {
      accept_token();
      auto opr2 = _read_primary();
      if ( token.type() == TokenType::MULT ) {
	opr1 = AstExpr::new_mult(std::move(opr1), std::move(opr2), token.loc());
      }
      else {
	opr1 = AstExpr::new_div(std::move(opr1), std::move(opr2), token.loc());
      }
    }
    else {
      return opr1;
    }
  }
}

END_NAMESPACE_YM_DOTLIB
