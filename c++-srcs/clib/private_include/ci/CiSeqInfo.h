#ifndef CISEQINFO_H
#define CISEQINFO_H

/// @file CiSeqInfo.h
/// @brief CiSeqInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class CiSeqInfo CiSeqInfo.h "CiSeqInfo.h"
/// @brief シンプルなFF/Latch の種類を表すクラス
///
/// 以下の属性を持つ．
/// - slave clock を持つか否か
/// - xq 出力を持つか否か
/// - clear 端子を持つか否か
/// - preset 端子を持つか否か
/// - clear, preset を持つ場合には以下の属性も持つ．
/// - clear/preset の両方がアサートされた時の q/xq の出力値
//////////////////////////////////////////////////////////////////////
class CiSeqInfo
{
public:

  /// @brief コンストラクタ
  CiSeqInfo(
    bool slave_clock,     ///< [in] slave clock を持つ時 true
    bool xq,              ///< [in] xq 出力を持つ時 true
    ClibSeqType seq_type  ///< [in] clear端子，preset端子の有無
  ) : mCpv1{ClibCPV::X},
      mCpv2{ClibCPV::X}
  {
    if ( slave_clock ) {
      mFlags.set(0);
    }
    if ( xq ) {
      mFlags.set(1);
    }
    switch ( seq_type ) {
    case ClibSeqType::none:
      break;
    case ClibSeqType::clear:
      mFlags.set(2);
      break;
    case ClibSeqType::preset:
      mFlags.set(3);
      break;
    case ClibSeqType::clear_preset:
      throw std::invalid_argument{"ClibSeqType::clear_preset is not allowed hear"};
      break;
    }
  }

  /// @brief コンストラクタ
  CiSeqInfo(
    bool slave_clock,           ///< [in] slave clock を持つ時 true
    bool xq,		        ///< [in] xq 出力を持つ時 true
    ClibCPV clear_preset_var1,  ///< [in] clear と preset の両方がアサートされた時の値1
    ClibCPV clear_preset_var2   ///< [in] clear と preset の両方がアサートされた時の値2
  ) : mCpv1{clear_preset_var1},
      mCpv2{clear_preset_var2}
  {
    if ( slave_clock ) {
      mFlags.set(0);
    }
    if ( xq ) {
      mFlags.set(1);
    }
    mFlags.set(2);
    mFlags.set(3);
  }

  /// @brief デストラクタ
  ~CiSeqInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief slave clock を持つ時 true を返す．
  bool
  has_slave_clock() const
  {
    return mFlags[0];
  }

  /// @brief xq 端子を持つ時 true を返す．
  bool
  has_xq() const
  {
    return mFlags[1];
  }

  /// @brief clear 端子を持つ時 true を返す．
  bool
  has_clear() const
  {
    return mFlags[2];
  }

  /// @brief preset 端子を持つ時 true を返す．
  bool
  has_preset() const
  {
    return mFlags[3];
  }

  /// @brief clear 端子と preset 端子の有無を表す型
  ClibSeqType
  clear_preset_type() const
  {
    return seq_type_encode(has_clear(), has_preset());
  }

  /// @brief clear/preset が同時に与えられたときの出力の値1
  ClibCPV
  clear_preset_var1() const
  {
    return mCpv1;
  }

  /// @brief clear/preset が同時に与えられたときの出力の値2
  ClibCPV
  clear_preset_var2() const
  {
    return mCpv2;
  }

  /// @brief エンコードしたインデックス値
  SizeType
  encode_val() const
  {
    SizeType index = 0;
    if ( has_slave_clock() ) {
      index += 1;
    }
    if ( has_xq() ) {
      index += 2;
    }
    if ( has_clear() ) {
      index += 4;
    }
    if ( has_preset() ) {
      index += 8;
    }
    if ( index >= 12 ) {
      // clear/preset ありの場合
      index += encode_cpv(mCpv1) * 4;
      index += encode_cpv(mCpv2) * 20;
    }
    return index;
  }

  /// @brief インデックス値から値を作る．
  static
  CiSeqInfo
  decode_index(
    SizeType index
  )
  {
    bool slave_clock = static_cast<bool>((index >> 0) & 1);
    bool xq = static_cast<bool>((index >> 1) & 1);
    if ( index < 12 ) {
      ClibSeqType type;
      switch ( (index >> 2) & 3 ) {
      case 0: type = ClibSeqType::none; break;
      case 1: type = ClibSeqType::clear; break;
      case 2: type = ClibSeqType::preset; break;
      }
      return CiSeqInfo{slave_clock, xq, type};
    }
    else {
      index -= 12;
      index /= 4;
      SizeType index1 = index % 5;
      auto cpv1 = decode_cpv(index1);
      SizeType index2 = index / 5;
      auto cpv2 = decode_cpv(index2);
      return CiSeqInfo{slave_clock, xq, cpv1, cpv2};
    }
  }

  /// @brief インデックス値の総数を返す．
  static
  SizeType
  max_index()
  {
    return 112;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ClibCPV をエンコードする．
  static
  SizeType
  encode_cpv(
    ClibCPV cpv
  )
  {
    switch ( cpv ) {
    case ClibCPV::L: return 0;
    case ClibCPV::H: return 1;
    case ClibCPV::N: return 2;
    case ClibCPV::T: return 3;
    case ClibCPV::X: return 4;
    }
    ASSERT_NOT_REACHED;
    return 0;
  }

  /// @brief 整数値から ClibCPv に変換する．
  static
  ClibCPV
  decode_cpv(
    SizeType index
  )
  {
    switch ( index ) {
    case 0: return ClibCPV::L;
    case 1: return ClibCPV::H;
    case 2: return ClibCPV::N;
    case 3: return ClibCPV::T;
    case 4: return ClibCPV::X;
    }
    ASSERT_NOT_REACHED;
    return ClibCPV::X;
  }

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // x1/slave clock/clear/preset のフラグ
  std::bitset<4> mFlags{0U};

  // cpv1
  ClibCPV mCpv1;

  // pcv2
  ClibCPV mCpv2;

};

END_NAMESPACE_YM_CLIB

#endif // CISEQINFO_H
