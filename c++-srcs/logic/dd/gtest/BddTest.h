
/// @file BddTest.h
/// @brief Bdd のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddVarSet.h"
#include "ym/BddCube.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

class BddTest :
public ::testing::Test
{
public:

  /// BddMgr を返す．
  BddMgr&
  mgr()
  {
    return mMgr;
  }

  /// @brief 不正値を返す．
  Bdd
  invalid()
  {
    return Bdd::invalid();
  }

  /// @brief 変数を返す．
  BddVar
  variable(
    SizeType varid
  )
  {
    return mMgr.variable(varid);
  }

  /// @brief 変数を表すBDDを返す．
  Bdd
  variable_bdd(
    SizeType varid
  )
  {
    return mMgr.variable_bdd(varid);
  }

  /// @brief リテラルを返す．
  BddLit
  literal(
    SizeType varid ///< [in] 変数番号
  )
  {
    return mMgr.literal(varid);
  }

  /// @brief リテラルを表すBDDを返す．
  Bdd
  literal_bdd(
    SizeType varid ///< [in] 変数番号
  )
  {
    return variable_bdd(varid);
  }

  /// @brief 真理値表形式の文字列からBDDを作る．
  Bdd
  from_truth(
    const char* str
  )
  {
    return mMgr.from_truth(str);
  }

  /// @brief BDDの内容を調べる．
  void
  check(
    const Bdd& bdd,     ///< [in] 対象のBDD
    const char* exp_str ///< [in] 真理値表の期待値
  );

  /// @brief check_sym() のテストを行う．
  void
  test_check_sym(
    const Bdd& bdd, ///< [in] 対象のBDD
    SizeType var1,  ///< [in] 変数1
    SizeType var2   ///< [in] 変数2
  );

  /// @brief check_sup() のテストを行う．
  void
  test_check_sup(
    const Bdd& bdd, ///< [in] 対象のBDD
    SizeType var    ///< [in] 変数1
  );

  /// @brief サポートのテストを行う．
  void
  test_support(
    const Bdd& bdd,       ///< [in] 対象のBDD
    const BddVarSet& sup, ///< [in] そのサポート
    SizeType max_v        ///< [in] 変数番号の最大値 + 1
  );

  /// @brief onepathのテストを行う．
  void
  test_onepath(
    const Bdd& bdd,     ///< [in] 対象のBDD
    const BddCube& path ///< [in] 1となる経路
  );

  /// @brief zeropathのテストを行う．
  void
  test_zeropath(
    const Bdd& bdd,     ///< [in] 対象のBDD
    const BddCube& path ///< [in] 0となる経路
  );


protected:

  void
  SetUp() override;

  void
  TearDown() override;


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgr mMgr;

};

END_NAMESPACE_YM
