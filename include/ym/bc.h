#ifndef BC_H
#define BC_H

/// @file bc.h
/// @brief bc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


//////////////////////////////////////////////////////////////////////
// bc 用の名前空間の定義用マクロ
//////////////////////////////////////////////////////////////////////

/// @brief BcGraph 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBc)

/// @brief BcGraph 用の名前空間の終了
#define END_NAMESPACE_YM_BC \
END_NAMESPACE(nsBc) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_BC

/// @brief 演算の種類を表す列挙型
/// @ingroup BcGroup
///
/// 正規化された Boolean Chain の場合，2入力関数であれば
/// AND, OR, XOR のみで表現可能となる．
///
/// 3入力以上の場合にはより複雑な関数が必要となるので
/// 真理値表を用いる．
enum class BcOpType : std::uint8_t {
  NONE = 0, ///< 不正値
  AND  = 1, ///< AND(論理積)
  OR   = 2, ///< OR(論理和)
  XOR  = 3, ///< XOR(排他的論理和)
  TV   = 4  ///< 真理値表(3入力以上の場合のみ)
};

/// @brief ストリーム出力
/// @relates BcOpType
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  BcOpType type    ///< [in] 演算の種類
)
{
  switch ( type ) {
  case BcOpType::NONE: s << "---"; break;
  case BcOpType::AND:  s << "AND"; break;
  case BcOpType::OR:   s << "OR"; break;
  case BcOpType::XOR:  s << "XOR"; break;
  case BcOpType::TV:   s << "TV"; break;
  }
  return s;
}

/// @brief シミュレーション用のビットベクタを表す型
/// @relates BcOpType
using BcBvType = std::uint64_t;

/// @relates BcOpType
const BcBvType BcBv_ALL0 = 0x0000000000000000UL;
/// @relates BcOpType
const BcBvType BcBv_ALL1 = 0xFFFFFFFFFFFFFFFFUL;

class BcGraph;
class BcOp;
class BcEdge;

END_NAMESPACE_YM_BC

BEGIN_NAMESPACE_YM

using nsBc::BcOpType;
using nsBc::BcBvType;
using nsBc::BcGraph;
using nsBc::BcOp;
using nsBc::BcEdge;
using nsBc::BcBvType;
using nsBc::BcBv_ALL0;
using nsBc::BcBv_ALL1;

END_NAMESPACE_YM

#endif // BC_H
