
/// @file CiFLCell.cc
/// @brief CiLFCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "CiFLCell.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiFLCell
//////////////////////////////////////////////////////////////////////

// @brief 内部変数1の名前を返す．
std::string
CiFLCell::qvar1() const
{
  return mVar1;
}

// @brief 内部変数2の名前を返す．
std::string
CiFLCell::qvar2() const
{
  return mVar2;
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
Expr
CiFLCell::clear_expr() const
{
  return mClear;
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
Expr
CiFLCell::preset_expr() const
{
  return mPreset;
}

// @brief 順序セルの属性を返す．
ClibSeqAttr
CiFLCell::seq_attr() const
{
  return mSeqAttr;
}

// @brief 内容をバイナリダンプする．
void
CiFLCell::dump_FL(
  Serializer& s
) const
{
  dump_common(s);
  s.dump(mVar1);
  s.dump(mVar2);
  s.dump(mClear);
  s.dump(mPreset);
  s.dump(mSeqAttr);
}

// @brief 内容を読み込む．
void
CiFLCell::restore_FL(
  Deserializer& s
)
{
  CiCell::restore_common(s);
  s.restore(mVar1);
  s.restore(mVar2);
  s.restore(mClear);
  s.restore(mPreset);
  s.restore(mSeqAttr);
}

END_NAMESPACE_YM_CLIB
