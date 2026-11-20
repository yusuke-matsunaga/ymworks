#ifndef ELABORATORTEST_H
#define ELABORATORTEST_H

/// @file ElaboratorTest.h
/// @brief ElaboratorTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/vl/VlMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElaboratorTest ElaboratorTest.h "ElaboratorTest.h"
/// @brief VlMgr::elaborate() のテスト環境
//////////////////////////////////////////////////////////////////////
class ElaboratorTest :
  public ::testing::Test
{
public:

  /// @brief コンストラクタ
  ElaboratorTest() = default;

  /// @brief デストラクタ
  ~ElaboratorTest() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  void
  check(
    const std::string& src_filename, ///< [in] Verilog-HDL のファイル名
    const std::string& ref_filename  ///< [in] VlDumper の結果の参照ファイル
  );


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  VlMgr vlmgr;

};

END_NAMESPACE_YM_VERILOG

#endif // ELABORATORTEST_H
