
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

BEGIN_NONAMESPACE

// @brief 範囲チェックを行う
inline
void
_check_range(
  SizeType index,
  SizeType range,
  const char* index_name
)
{
  if ( index < 0 || range <= index ) {
    std::ostringstream buf;
    buf << "'" << index_name << "'(" << index << ") is out-of-range";
    throw std::out_of_range{buf.str()};
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief 入力番号のチェックを行う．
void
BnModel::_check_input_id(
  SizeType input_id
) const
{
  _check_range(input_id, input_num(), "input_id");
}

// @brief 出力番号のチェックを行う．
void
BnModel::_check_output_id(
  SizeType output_id
) const
{
  _check_range(output_id, output_num(), "output_id");
}

// @brief 論理ノード番号のチェックを行う．
void
BnModel::_check_logic_id(
  SizeType logic_id
) const
{
  _check_range(logic_id, logic_num(), "logic_id");
}

// @brief SDFF番号のチェックを行う．
void
BnModel::_check_dff_id(
  SizeType dff_id
) const
{
  _check_range(dff_id, dff_num(), "dff_id");
}

END_NAMESPACE_YM_BN
