
/// @file AstAttrInst.cc
/// @brief AstAttrInst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstAttrInst.h"
#include "ym/vl/AstAttrSpec.h"
#include "parser/PtAttrInst.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstAttrInst
//////////////////////////////////////////////////////////////////////

// @brief 要素のリストの取得
AstAttrSpecList
AstAttrInst::attrspec_list() const
{
  _check_ptr();
  return AstAttrSpecList(AstAttrSpec(mPtr->attrspec_top()));
}

// @brief 適切な値を持っている時 true を返す．
bool
AstAttrInst::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstAttrInst::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstAttrInst::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstAttrInst
AstAttrInst::next() const
{
  _check_ptr();
  return AstAttrInst(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstAttrInst::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  jobj.add("attrspec_list", attrspec_list().json_obj());
}

END_NAMESPACE_YM_VERILOG
