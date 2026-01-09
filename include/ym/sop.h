#ifndef YM_SOP_H
#define YM_SOP_H

/// @file ym/sop.h
/// @brief Sop サブモジュールの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup LogicGroup 論理式と論理関数を扱うクラス
///
/// - SopCover, SopCube: 積和形論理式とキューブ

#include "ym_config.h"


/// @brief SOP 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_SOP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSop)

/// @brief SOP 用の名前空間の終了
#define END_NAMESPACE_YM_SOP \
END_NAMESPACE(nsSop) \
END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_SOP

class SopCube;
class SopCover;

END_NAMESPACE_YM_SOP

BEGIN_NAMESPACE_YM

using nsSop::SopCube;
using nsSop::SopCover;

/// @brief SopCover/SopCube 中のパタンを表す列挙型
enum class SopPat : std::uint8_t {
  __ = 0, ///< 未使用
  _1 = 1, ///< 正極性
  _0 = 2, ///< 負極性
  _X = 3  ///< なし
};

/// @relates SopPat
/// @brief SopPat のストリーム出力
/// @return s を返す．
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] ストリーム
  SopPat pat      ///< [in] パタン
)
{
  switch ( pat ) {
  case SopPat::__: s << '_'; break;
  case SopPat::_1: s << '1'; break;
  case SopPat::_0: s << '0'; break;
  case SopPat::_X: s << '-'; break;
  }
  return s;
}

END_NAMESPACE_YM

#endif // YM_SOP_H
