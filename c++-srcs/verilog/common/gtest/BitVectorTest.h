#ifndef BITVECTORTEST_H
#define BITVECTORTEST_H

/// @file BitVectorTest.h
/// @brief BitVectorTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BitVectorTest BitVectorTest.h "BitVectorTest.h"
/// @brief BitVector のテスト
//////////////////////////////////////////////////////////////////////
class BitVectorTest :
public ::testing::Test
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BitVector のチェックを行う．
  void
  check(
    const BitVector& bv,
    bool is_signed,
    bool is_sized,
    const std::vector<VlScalarVal>& val_list,
    int base
  )
  {
    auto size = val_list.size();
    EXPECT_EQ( VlValueType(is_signed, is_sized, size), bv.value_type() );
    EXPECT_EQ( size, bv.size() );
    EXPECT_EQ( is_sized, bv.is_sized() );
    EXPECT_EQ( is_signed, bv.is_signed() );
    EXPECT_EQ( base, bv.base() );
    bool has_x = false;
    bool has_z = false;
    for ( SizeType i = 0; i < size; ++ i ) {
      auto val = val_list[i];
      EXPECT_EQ( val, bv.value(i) ) << "bv_value(" << i << ")";
      if ( val == VlScalarVal::x() ) {
	has_x = true;
      }
      if ( val == VlScalarVal::z() ) {
	has_z = true;
      }
    }
    EXPECT_EQ( has_x, bv.has_x() );
    EXPECT_EQ( has_z, bv.has_z() );
    EXPECT_EQ( has_x | has_z, bv.has_xz() );
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM

#endif // BITVECTORTEST_H
