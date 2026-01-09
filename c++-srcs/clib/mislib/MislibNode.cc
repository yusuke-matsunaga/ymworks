
/// @file MislibNode.cc
/// @brief MislibNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MislibNode.h"
#include "MislibExpr.h"
#include "MislibGate.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibNum
//////////////////////////////////////////////////////////////////////

// 内容を出力する．
void
MislibNum::dump(
  std::ostream& s
) const
{
  s << "<NUM>" << std::endl;
  dump_loc(s);
  s << num() << std::endl
    << "</NUM>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibStr
//////////////////////////////////////////////////////////////////////

// 内容を出力する．
void
MislibStr::dump(
  std::ostream& s
) const
{
  s << "<STR>" << std::endl;
  dump_loc(s);
  s << str() << std::endl
    << "</STR>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibNoinv
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibPhase::Type
MislibNoninv::type() const
{
  return Noninv;
}

// 内容を出力する．
// デバッグ用
void
MislibNoninv::dump(
  std::ostream& s
) const
{
  s << "<NONINV>" << std::endl;
  dump_loc(s);
  s << "</NONINV>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibInv
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibPhase::Type
MislibInv::type() const
{
  return Inv;
}

// 内容を出力する．
// デバッグ用
void
MislibInv::dump(
  std::ostream& s
) const
{
  s << "<INV>" << std::endl;
  dump_loc(s);
  s << "</INV>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibUnknown
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibPhase::Type
MislibUnknown::type() const
{
  return Unknown;
}

// 内容を出力する．
// デバッグ用
void
MislibUnknown::dump(
  std::ostream& s
) const
{
  s << "<UNKNOWN>" << std::endl;
  dump_loc(s);
  s << "</UNKNOWN>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibExpr
//////////////////////////////////////////////////////////////////////

// @brief 変数名を取り出す
//
// デフォルトの実装ではエラー(アボート)となる．
ShString
MislibExpr::varname() const
{
  ASSERT_NOT_REACHED;
  return ShString();
}

// 1番目のオペランドを取り出す．
// デフォルトの実装ではエラー(アボート)となる．
const MislibExpr*
MislibExpr::opr1() const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}

// 2番目のオペランドを取り出す．
// デフォルトの実装ではエラー(アボート)となる．
const MislibExpr*
MislibExpr::opr2() const
{
  ASSERT_NOT_REACHED;
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibNot
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibExpr::Type
MislibNot::type() const
{
  return Not;
}

// 1番目のオペランドを取り出す．
const MislibExpr*
MislibNot::opr1() const
{
  return mOpr1.get();
}

// @brief 対応する論理式を生成する．
Expr
MislibNot::to_expr(
  const MislibNameMap& name_map
) const
{
  return ~opr1()->to_expr(name_map);
}

// 内容を出力する．
void
MislibNot::dump(
  std::ostream& s
) const
{
  s << "<NOT>" << std::endl;
  dump_loc(s);

  s << "<CHILD1>" << std::endl;
  opr1()->dump(s);
  s << "</CHILD1>" << std::endl;

  s << "</NOT>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibBop
//////////////////////////////////////////////////////////////////////

// 1番目のオペランドを取り出す．
const MislibExpr*
MislibBop::opr1() const
{
  return mOpr1.get();
}

// 2番目のオペランドを取り出す．
const MislibExpr*
MislibBop::opr2() const
{
  return mOpr2.get();
}


//////////////////////////////////////////////////////////////////////
// クラス MislibAnd
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibExpr::Type
MislibAnd::type() const
{
  return And;
}

// @brief 対応する論理式を生成する．
Expr
MislibAnd::to_expr(
  const MislibNameMap& name_map
) const
{
  return opr1()->to_expr(name_map) & opr2()->to_expr(name_map);
}

// 内容を出力する．
void
MislibAnd::dump(
  std::ostream& s
) const
{
  s << "<AND>" << std::endl;
  dump_loc(s);

  s << "<CHILD1>" << std::endl;
  opr1()->dump(s);
  s << "</CHILD1>" << std::endl;

  s << "<CHILD2>" << std::endl;
  opr2()->dump(s);
  s << "</CHILD2>" << std::endl;

  s << "</AND>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibOr
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibExpr::Type
MislibOr::type() const
{
  return Or;
}

// @brief 対応する論理式を生成する．
Expr
MislibOr::to_expr(
  const MislibNameMap& name_map
) const
{
  return opr1()->to_expr(name_map) | opr2()->to_expr(name_map);
}

// 内容を出力する．
void
MislibOr::dump(
  std::ostream& s
) const
{
  s << "<OR>" << std::endl;
  dump_loc(s);

  s << "<CHILD1>" << std::endl;
  opr1()->dump(s);
  s << "</CHILD1>" << std::endl;

  s << "<CHILD2>" << std::endl;
  opr2()->dump(s);
  s << "</CHILD2>" << std::endl;

  s << "</OR>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibXor
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibExpr::Type
MislibXor::type() const
{
  return Xor;
}

// @brief 対応する論理式を生成する．
Expr
MislibXor::to_expr(
  const MislibNameMap& name_map
) const
{
  return opr1()->to_expr(name_map) ^ opr2()->to_expr(name_map);
}

// 内容を出力する．
void
MislibXor::dump(
  std::ostream& s
) const
{
  s << "<XOR>" << std::endl;
  dump_loc(s);

  s << "<CHILD1>" << std::endl;
  opr1()->dump(s);
  s << "</CHILD1>" << std::endl;

  s << "<CHILD2>" << std::endl;
  opr2()->dump(s);
  s << "</CHILD2>" << std::endl;

  s << "</XOR>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibVarName
//////////////////////////////////////////////////////////////////////

// @brief 種類を取り出す．
MislibExpr::Type
MislibVarName::type() const
{
  return VarName;
}

// @brief 対応する論理式を生成する．
Expr
MislibVarName::to_expr(
  const MislibNameMap& name_map
) const
{
  int c = name_map.count(varname());
  ASSERT_COND( c > 0 );

  SizeType var = name_map.at(varname());
  return Expr::positive_literal(var);
}

// @brief 変数名を取り出す
ShString
MislibVarName::varname() const
{
  return mVarName;
}

// 内容を出力する．
void
MislibVarName::dump(
  std::ostream& s
) const
{
  s << "<VARNAME>" << std::endl;
  dump_loc(s);
  s << varname() << std::endl
    << "</VARNAME>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibConst0
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibExpr::Type
MislibConst0::type() const
{
  return Const0;
}

// @brief 対応する論理式を生成する．
Expr
MislibConst0::to_expr(
  const MislibNameMap& name_map
) const
{
  return Expr::zero();
}

// 内容を出力する．
void
MislibConst0::dump(
  std::ostream& s
) const
{
  s << "<CONST0>" << std::endl;
  dump_loc(s);
  s << "</CONST0>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibConst1
//////////////////////////////////////////////////////////////////////

// 種類を取り出す．
MislibExpr::Type
MislibConst1::type() const
{
  return Const1;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
Expr
MislibConst1::to_expr(
  const MislibNameMap& name_map
) const
{
  return Expr::one();
}

// 内容を出力する．
void
MislibConst1::dump(
  std::ostream& s
) const
{
  s << "<CONST1>" << std::endl;
  dump_loc(s);
  s << "</CONST1>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibPin
//////////////////////////////////////////////////////////////////////

// 内容を出力する．
// デバッグ用
void
MislibPin::dump(
  std::ostream& s
) const
{
  s << "<PIN>" << std::endl;
  dump_loc(s);

  s << "<NAME>" << std::endl;
  if ( name() ) {
    name()->dump(s);
  }
  else {
    s << "*" << std::endl;
  }
  s << "</NAME>" << std::endl;

  s << "<PHASE>" << std::endl;
  phase()->dump(s);
  s << "</PHASE>" << std::endl;

  s << "<INPUT_LOAD>" << std::endl;
  input_load()->dump(s);
  s << "</INPUT_LOAD>" << std::endl;

  s << "<MAX_LOAD>" << std::endl;
  max_load()->dump(s);
  s << "</MAX_LOAD>" << std::endl;

  s << "<RISE_BLOCK_DELAY>" << std::endl;
  rise_block_delay()->dump(s);
  s << "</RISE_BLOCK_DELAY>" << std::endl;

  s << "<RISE_FANOUT_DELAY>" << std::endl;
  rise_fanout_delay()->dump(s);
  s << "</RISE_FANOUT_DELAY>" << std::endl;

  s << "<FALL_BLOCK_DELAY>" << std::endl;
  fall_block_delay()->dump(s);
  s << "</FALL_BLOCK_DELAY>" << std::endl;

  s << "<FALLE_FANOUT_DELAY>" << std::endl;
  fall_fanout_delay()->dump(s);
  s << "</FALL_FANOUT_DELAY>" << std::endl;

  s << "</PIN>" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibGate
//////////////////////////////////////////////////////////////////////

// @brief 内容を出力する．
void
MislibGate::dump(
  std::ostream& s
) const
{
  s << "<GATE>" << std::endl;
  dump_loc(s);

  s << "<NAME>" << std::endl;
  name()->dump(s);
  s << "</NAME>" << std::endl;

  s << "<AREA>" << std::endl;
  area()->dump(s);
  s << "</AREA>" << std::endl;

  s << "<OPIN_NAME>" << std::endl;
  opin_name()->dump(s);
  s << "</OPIN_NAME>" << std::endl;

  s << "<OPIN_EXPR>" << std::endl;
  opin_expr()->dump(s);
  s << "</OPIN_EXPR>" << std::endl;

  s << "<IPIN_LIST>" << std::endl;
  int n = ipin_num();
  for ( int i = 0; i < n; ++ i ) {
    auto pin = ipin(i);
    pin->dump(s);
  }
  s << "</IPIN_LIST>" << std::endl;

  s << "</GATE>" << std::endl;
}

END_NAMESPACE_YM_MISLIB
