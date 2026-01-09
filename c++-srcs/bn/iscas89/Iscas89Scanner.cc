
/// @file Iscas89Scanner.cc
/// @brief Iscas89Scanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Iscas89Scanner.h"


BEGIN_NONAMESPACE

// read_token() の動作をデバッグするときに true にする．
const bool debug_read_token = false;

END_NONAMESPACE

BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// iscas89 用の字句解析器
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Iscas89Scanner::Iscas89Scanner(
  std::istream& s,
  const FileInfo& file_info
) : Scanner{s, file_info}
{
  // 予約語辞書を作る．
  mRsvDict.emplace("INPUT", RsvInfo{Iscas89Token::INPUT, PrimType::None});
  mRsvDict.emplace("input", RsvInfo{Iscas89Token::INPUT, PrimType::None});
  mRsvDict.emplace("OUTPUT", RsvInfo{Iscas89Token::OUTPUT, PrimType::None});
  mRsvDict.emplace("output", RsvInfo{Iscas89Token::OUTPUT, PrimType::None});
  mRsvDict.emplace("BUFF", RsvInfo{Iscas89Token::GATE, PrimType::Buff});
  mRsvDict.emplace("BUF", RsvInfo{Iscas89Token::GATE, PrimType::Buff});
  mRsvDict.emplace("buff", RsvInfo{Iscas89Token::GATE, PrimType::Buff});
  mRsvDict.emplace("buf", RsvInfo{Iscas89Token::GATE, PrimType::Buff});
  mRsvDict.emplace("NOT", RsvInfo{Iscas89Token::GATE, PrimType::Not});
  mRsvDict.emplace("not", RsvInfo{Iscas89Token::GATE, PrimType::Not});
  mRsvDict.emplace("INV", RsvInfo{Iscas89Token::GATE, PrimType::Not});
  mRsvDict.emplace("inv", RsvInfo{Iscas89Token::GATE, PrimType::Not});
  mRsvDict.emplace("AND", RsvInfo{Iscas89Token::GATE, PrimType::And});
  mRsvDict.emplace("and", RsvInfo{Iscas89Token::GATE, PrimType::And});
  mRsvDict.emplace("NAND", RsvInfo{Iscas89Token::GATE, PrimType::Nand});
  mRsvDict.emplace("nand", RsvInfo{Iscas89Token::GATE, PrimType::Nand});
  mRsvDict.emplace("OR", RsvInfo{Iscas89Token::GATE, PrimType::Or});
  mRsvDict.emplace("or", RsvInfo{Iscas89Token::GATE, PrimType::Or});
  mRsvDict.emplace("NOR", RsvInfo{Iscas89Token::GATE, PrimType::Nor});
  mRsvDict.emplace("nor", RsvInfo{Iscas89Token::GATE, PrimType::Nor});
  mRsvDict.emplace("XOR", RsvInfo{Iscas89Token::GATE, PrimType::Xor});
  mRsvDict.emplace("xor", RsvInfo{Iscas89Token::GATE, PrimType::Xor});
  mRsvDict.emplace("XNOR", RsvInfo{Iscas89Token::GATE, PrimType::Xnor});
  mRsvDict.emplace("xnor", RsvInfo{Iscas89Token::GATE, PrimType::Xnor});
  mRsvDict.emplace("DFF", RsvInfo{Iscas89Token::DFF, PrimType::None});
  mRsvDict.emplace("dff", RsvInfo{Iscas89Token::DFF, PrimType::None});
}

#if 0
// @brief 拡張型を登録する．
void
Iscas89Scanner::reg_extype(
  const std::string& keyword,
  SizeType ex_id
)
{
  mRsvDict.emplace(keyword, RsvInfo{Iscas89Token::EXGATE, PrimType::None, ex_id});
}
#endif

// @brief トークンを一つ読み出す．
Iscas89Token
Iscas89Scanner::read_token()
{
  auto type = scan();
  auto loc = cur_region();
  Iscas89Token token;
  if ( type == Iscas89Token::NAME ) {
    auto name = cur_string();
    if ( mRsvDict.count(name) > 0 ) {
      auto info = mRsvDict.at(name);
      token = Iscas89Token{info.type, loc, info.gate_type, {}, info.ex_id};
    }
    else {
      token = Iscas89Token{Iscas89Token::NAME, loc, PrimType::None, name};
    }
  }
  else {
    token = Iscas89Token{type, loc};
  }

  if ( debug_read_token ) {
    std::cerr << "read_token()" << " --> "
	      << token.loc() << ": ";
    switch ( token.type() ) {
    case Iscas89Token::LPAR:   std::cerr << "("; break;
    case Iscas89Token::RPAR:   std::cerr << ")"; break;
    case Iscas89Token::EQ:     std::cerr << "="; break;
    case Iscas89Token::COMMA:  std::cerr << ","; break;
    case Iscas89Token::INPUT:  std::cerr << "INPUT"; break;
    case Iscas89Token::OUTPUT: std::cerr << "OUTPUT"; break;
    case Iscas89Token::GATE:
      switch ( token.gate_type() ) {
      case PrimType::Buff:   std::cerr << "BUFF"; break;
      case PrimType::Not:    std::cerr << "NOT"; break;
      case PrimType::And:    std::cerr << "AND"; break;
      case PrimType::Nand:   std::cerr << "NAND"; break;
      case PrimType::Or:     std::cerr << "OR"; break;
      case PrimType::Nor:    std::cerr << "NOR"; break;
      case PrimType::Xor:    std::cerr << "XOR"; break;
      case PrimType::Xnor:   std::cerr << "XNOR"; break;
      default: ASSERT_NOT_REACHED; break;
      }
      break;
    case Iscas89Token::EXGATE: std::cerr << "EXGATE(" << token.ex_id() << ")"; break;
    case Iscas89Token::DFF:    std::cerr << "DFF"; break;
    case Iscas89Token::NAME:   std::cerr << "NAME(" << token.name() << ")"; break;
    case Iscas89Token::_EOF:   std::cerr << "EOF"; break;
    default:                   std::cerr << static_cast<char>(token.type()); break;
    }
    std::cerr << std::endl;
  }

  return token;
}

// @brief read_token() の下請け関数
Iscas89Token::Type
Iscas89Scanner::scan()
{
  int c;

  mCurString = "";

  // 状態遷移を goto 文で表現したもの
  // 効率はよい．

 ST_INIT:
  c = get();
  set_first_loc();
  switch ( c ) {
  case EOF:
    return Iscas89Token::_EOF;

  case ' ':
  case '\t':
  case '\n':
    // ホワイトスペースは読み飛ばす．
    goto ST_INIT;

  case '#':
    goto ST_SHARP;

  case '=':
    return Iscas89Token::EQ;

  case '(':
    return Iscas89Token::LPAR;

  case ')':
    return Iscas89Token::RPAR;

  case ',':
    return Iscas89Token::COMMA;

  default:
    mCurString += static_cast<char>(c);
    goto ST_STR;
  }

 ST_SHARP: // 1行コメントの始まり
  c = get();
  if ( c == '\n' ) {
    goto ST_INIT;
  }
  if ( c == EOF ) {
    return Iscas89Token::_EOF;
  }
  // 改行までは読み飛ばす．
  goto ST_SHARP;

 ST_STR:
  c = peek();
  switch ( c ) {
  case ' ':
  case '\t':
  case '\n':
  case '#':
  case '=':
  case '(':
  case ')':
  case ',':
  case EOF:
    // 文字列の終わり
    return Iscas89Token::NAME;

  default:
    accept();
    mCurString += static_cast<char>(c);
    goto ST_STR;
  }
}

END_NAMESPACE_YM_BN
