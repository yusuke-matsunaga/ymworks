
/// @file ElaboratorTest.cc
/// @brief ElaboratorTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ElaboratorTest.h"
#include "ym/vl/VlDumper.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// ElaboratorTest
//////////////////////////////////////////////////////////////////////

void
ElaboratorTest::check(
  const std::string& src_filename,
  const std::string& ref_filename
)
{
  // ファイルを読み込む．
  auto r = vlmgr.read_file(src_filename);
  ASSERT_TRUE( r );

  // 実体化する．
  auto nerr = vlmgr.elaborate();
  ASSERT_TRUE( nerr == 0 );

  // 内容をダンプする．
  std::ostringstream buf;
  VlDumper dumper(buf);
  dumper.disable_nullptr_suppress_mode();
  dumper(vlmgr);

  // ダンプ結果が期待値と等しいか調べる．
  auto data = buf.str();
  std::ifstream ref_fs(ref_filename);
  ASSERT_TRUE( ref_fs );
  auto ref_data = std::string((std::istreambuf_iterator<char>(ref_fs)),
			      std::istreambuf_iterator<char>());
  EXPECT_EQ( ref_data, data );
}

END_NAMESPACE_YM_VERILOG
