#ifndef YM_PRIMTYPE_H
#define YM_PRIMTYPE_H

/// @file ym/PrimType.h
/// @brief PrimType の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @ingroup LogicGroup
/// @brief 組み込み型の論理ゲートタイプ
///
/// 一般的な論理セル(論理ゲート)に用いられる論理関数のタイプを表す列挙型
//////////////////////////////////////////////////////////////////////
enum class PrimType : std::uint8_t {
  None, ///< 不正値
  C0,   ///< 定数0
  C1,   ///< 定数1
  Buff, ///< バッファ
  Not,  ///< NOT
  And,  ///< AND
  Nand, ///< NAND
  Or,   ///< OR
  Nor,  ///< NOR
  Xor,  ///< XOR
  Xnor  ///< XNOR
};

/// @relates PrimType
/// @brief ストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  PrimType type    ///< [in] 組み込み型
)
{
  switch ( type ) {
  case PrimType::None: s << "None"; break;
  case PrimType::C0:   s << "C0"; break;
  case PrimType::C1:   s << "C1"; break;
  case PrimType::Buff: s << "Buff"; break;
  case PrimType::Not:  s << "Not"; break;
  case PrimType::And:  s << "And"; break;
  case PrimType::Nand: s << "Nand"; break;
  case PrimType::Or:   s << "Or"; break;
  case PrimType::Nor:  s << "Nor"; break;
  case PrimType::Xor:  s << "Xor"; break;
  case PrimType::Xnor: s << "Xnor"; break;
  }
  return s;
}

END_NAMESPACE_YM

#endif // YM_PRIMTYPE_H
