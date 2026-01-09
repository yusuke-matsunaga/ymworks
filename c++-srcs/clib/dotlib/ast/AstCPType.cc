
/// @file AstCPType.cc
/// @brief AstCPType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/AstCPType.h"
#include "dotlib/AstMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief clear_preset_var の値を表す AstNode を生成する．
const AstCPType*
AstMgr::new_cptype(
  const FileRegion& loc,
  AstCPType::Type value
)
{
  auto node = new AstCPType(loc, value);
  mNodeList.push_back(node);
  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス AstCPType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル上の位置
// @param[in] value 値
AstCPType::AstCPType(
  const FileRegion& loc,
  Type value
) : AstNode(loc),
    mValue(value)
{
}

// @brief デストラクタ
AstCPType::~AstCPType()
{
}

// @brief 内容をストリーム出力する．
// @param[in] s 出力先のストリーム
// @param[in] indent インデント量
void
AstCPType::dump(
  std::ostream& s,
  int indent
) const
{
  const char* tmp = nullptr;
  switch ( value() ) {
  case L: tmp = "L"; break;
  case H: tmp = "H"; break;
  case N: tmp = "N"; break;
  case T: tmp = "T"; break;
  case X: tmp = "X"; break;
  }
  dump_string(s, tmp);
}

END_NAMESPACE_YM_DOTLIB
