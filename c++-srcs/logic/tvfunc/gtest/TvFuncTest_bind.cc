
/// @file TvFuncTest_bind.cc
/// @brief TvFuncTest_bind の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <random>
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TvFuncTest_bind TvFuncTest_bind.h "TvFuncTest_bind.h"
/// @brief TvFunc::bind() 用のテスト
//////////////////////////////////////////////////////////////////////
class TvFuncTest_bind:
  public ::testing::Test
{
public:

  /// @brief コンストラクタ
  TvFuncTest_bind() = default;

  /// @brief デストラクタ
  ~TvFuncTest_bind() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  void
  do_test(
    const TvFunc& func
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  check_func(
    const TvFunc& func,
    SizeType var,
    bool inv
  );

};

void
TvFuncTest_bind::do_test(
  const TvFunc& func
)
{
  auto ni = func.input_num();
  for ( SizeType i = 0; i < ni; ++ i ) {
    check_func(func, i, true);
    check_func(func, i, false);
  }
}

void
TvFuncTest_bind::check_func(
  const TvFunc& func,
  SizeType var,
  bool inv
)
{
  auto bfunc = func.bind(var, inv);
  auto ni = func.input_num() - 1;
  ASSERT_EQ( ni, bfunc.input_num() );
  SizeType ni_exp = 1 << ni;
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    auto val = bfunc.value(b);
    SizeType index = 0;
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( b & (1 << i) ) {
	if ( i < var ) {
	  index |= (1 << i);
	}
	else {
	  index |= (1 << (i + 1));
	}
      }
    }
    if ( !inv ) {
      index |= (1 << var);
    }
    auto exp_val = func.value(index);
    std::ostringstream buf;
    buf << func.bin_str() << ": var = " << var
	<< ": inv = " << inv
	<< ": bfunc = " << bfunc.bin_str();
    EXPECT_EQ( exp_val, val ) << buf.str();
  }
}

TEST_F(TvFuncTest_bind, bind1)
{
  SizeType ni = 1;
  SizeType ni_exp = 1 << ni;
  SizeType ni_exp_exp = 1 << ni_exp;
  for ( SizeType pat = 0; pat < ni_exp_exp; ++ pat ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( pat & (1 << b) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    do_test(TvFunc(ni, val_list));
  }
}

TEST_F(TvFuncTest_bind, bind2)
{
  SizeType ni = 2;
  SizeType ni_exp = 1 << ni;
  SizeType ni_exp_exp = 1 << ni_exp;
  for ( SizeType pat = 0; pat < ni_exp_exp; ++ pat ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( pat & (1 << b) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    do_test(TvFunc(ni, val_list));
  }
}

TEST_F(TvFuncTest_bind, bind3)
{
  SizeType ni = 3;
  SizeType ni_exp = 1 << ni;
  SizeType ni_exp_exp = 1 << ni_exp;
  for ( SizeType pat = 0; pat < ni_exp_exp; ++ pat ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( pat & (1 << b) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    do_test(TvFunc(ni, val_list));
  }
}

TEST_F(TvFuncTest_bind, bind4)
{
  SizeType ni = 4;
  SizeType ni_exp = 1 << ni;
  SizeType nsample = 1000;
  std::mt19937 rand_gen;
  std::uniform_int_distribution<int> rand_dist(0, 1);
  for ( SizeType _ = 0; _ < nsample; ++ _ ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( rand_dist(rand_gen) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    do_test(TvFunc(ni, val_list));
  }
}

TEST_F(TvFuncTest_bind, bind5)
{
  SizeType ni = 5;
  SizeType ni_exp = 1 << ni;
  SizeType nsample = 1000;
  std::mt19937 rand_gen;
  std::uniform_int_distribution<int> rand_dist(0, 1);
  for ( SizeType _ = 0; _ < nsample; ++ _ ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( rand_dist(rand_gen) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    do_test(TvFunc(ni, val_list));
  }
}

TEST_F(TvFuncTest_bind, bind6)
{
  SizeType ni = 6;
  SizeType ni_exp = 1 << ni;
  SizeType nsample = 1000;
  std::mt19937 rand_gen;
  std::uniform_int_distribution<int> rand_dist(0, 1);
  for ( SizeType _ = 0; _ < nsample; ++ _ ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( rand_dist(rand_gen) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    do_test(TvFunc(ni, val_list));
  }
}

TEST_F(TvFuncTest_bind, bind7)
{
  SizeType ni = 5;
  SizeType ni_exp = 1 << ni;
  SizeType nsample = 1000;
  std::mt19937 rand_gen;
  std::uniform_int_distribution<int> rand_dist(0, 1);
  for ( SizeType _ = 0; _ < nsample; ++ _ ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( rand_dist(rand_gen) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    do_test(TvFunc(ni, val_list));
  }
}

END_NAMESPACE_YM
