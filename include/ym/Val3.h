#ifndef VAL3_H
#define VAL3_H

/// @file Val3.h
/// @brief 3値を表す型の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief 3値を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class Val3 : std::uint8_t {
  X    = 0, ///< 未定
  ZERO = 1, ///< 0
  ONE  = 2  ///< 1
};

/// @relates Val3
/// @brief 否定演算子
///
/// Val3::X の場合には変化しない
inline
Val3
operator~(
  Val3 val ///< [in] 値
)
{
  // val | ~val
  // ----+-----
  //   0 |    0
  //   1 |    2
  //   2 |    1
  // でビットパタンとしては 01 10 00 = 0x18 となる．

  std::uint8_t d = static_cast<std::uint8_t>(val) * 2;
  return static_cast<Val3>((0x18 >> d) & 0x3);
}

/// @relates Val3
/// @brief AND 演算子
inline
Val3
operator&(
  Val3 val1, ///< [in] 値1
  Val3 val2  ///< [in] 値2
)
{
  //      |        val1
  // val2 |   0      1      2
  // -----+------------------
  //    0 |   0      1      0
  //    1 |   1      1      1
  //    2 |   0      1      2
  // でビットパタンとしては 10 01 00 01 01 01 00 01 00 = 0x24544 となる．

  std::uint8_t d1 = static_cast<std::uint8_t>(val1);
  std::uint8_t d2 = static_cast<std::uint8_t>(val2);
  std::uint8_t idx = (d1 * 3 + d2) * 2;
  return static_cast<Val3>((0x24544 >> idx) & 0x3);
}

/// @relates Val3
/// @brief OR 演算子
inline
Val3
operator|(
  Val3 val1, ///< [in] 値1
  Val3 val2  ///< [in] 値2
)
{
  //      |        val1
  // val2 |   0      1      2
  // -----+------------------
  //    0 |   0      0      2
  //    1 |   0      1      2
  //    2 |   2      2      2
  // でビットパタンとしては 10 10 10 10 01 00 10 00 00 = 0x2A920 となる．

  std::uint8_t d1 = static_cast<std::uint8_t>(val1);
  std::uint8_t d2 = static_cast<std::uint8_t>(val2);
  std::uint8_t idx = (d1 * 3 + d2) * 2;
  return static_cast<Val3>((0x2A920 >> idx) & 0x3);
}

/// @relates Val3
/// @brief XOR 演算子
inline
Val3
operator^(
  Val3 val1, ///< [in] 値1
  Val3 val2  ///< [in] 値2
)
{
  //      |        val1
  // val2 |   0      1      2
  // -----+------------------
  //    0 |   0      0      0
  //    1 |   0      1      2
  //    2 |   0      2      1
  // でビットパタンとしては 01 10 00 10 01 00 00 00 00 = 0x18900 となる．

  std::uint8_t d1 = static_cast<std::uint8_t>(val1);
  std::uint8_t d2 = static_cast<std::uint8_t>(val2);
  std::uint8_t idx = (d1 * 3 + d2) * 2;
  return static_cast<Val3>((0x18900 >> idx) & 0x3);
}

/// @relates Val3
/// @brief 値を表す文字を返す．
inline
char
val3_name(
  Val3 val ///< [in] 値
)
{
  switch ( val ) {
  case Val3::X:    return 'X';
  case Val3::ZERO: return '0';
  case Val3::ONE:  return '1';
  default:         return '-';
  }
}

/// @relates Val3
/// @brief ストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力先のストリーム
  Val3 val         ///< [in] 値
)
{
  return s << val3_name(val);
}

END_NAMESPACE_YM

#endif // VAL3_H
