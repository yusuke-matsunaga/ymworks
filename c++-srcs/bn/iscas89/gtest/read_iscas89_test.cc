
/// @file read_iscas89_test.cc
/// @brief read_iscas89_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"


BEGIN_NAMESPACE_YM

TEST( BnModelTest, read_iscas1 )
{
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filepath = path / "b10.bench";
  auto model = BnModel::read_iscas89(filepath);
  int ni = 11;
  int no = 6;
  int nd = 17;
  int ng = 172;
  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  // 出力結果の回帰テスト
  std::ostringstream s1;
  model.write(s1);

  auto ref_path = path / "b10.bn";
  auto s2 = std::ifstream{ref_path};
  ASSERT_TRUE( s2 );
  std::string ref_contents;
  std::string buff;
  while ( getline(s2, buff) ) {
    ref_contents += buff + '\n';
  }

  EXPECT_EQ( ref_contents, s1.str() );
}

TEST( BnModelTest, read_iscas2 )
{
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filepath = path / "b10.bench";
  auto model = BnModel::read(filepath, "iscas89");
  int ni = 11;
  int no = 6;
  int nd = 17;
  int ng = 172;
  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  // 出力結果の回帰テスト
  std::ostringstream s1;
  model.write(s1);

  auto ref_path = path / "b10.bn";
  auto s2 = std::ifstream{ref_path};
  ASSERT_TRUE( s2 );
  std::string ref_contents;
  std::string buff;
  while ( getline(s2, buff) ) {
    ref_contents += buff + '\n';
  }

  EXPECT_EQ( ref_contents, s1.str() );
}

TEST( BnModelTest, read_iscas4 )
{
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filepath = path / "b10.bench";
  auto model = BnModel::read(filepath);
  int ni = 11;
  int no = 6;
  int nd = 17;
  int ng = 172;
  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  // 出力結果の回帰テスト
  std::ostringstream s1;
  model.write(s1);

  auto ref_path = path / "b10.bn";
  auto s2 = std::ifstream{ref_path};
  ASSERT_TRUE( s2 );
  std::string ref_contents;
  std::string buff;
  while ( getline(s2, buff) ) {
    ref_contents += buff + '\n';
  }

  EXPECT_EQ( ref_contents, s1.str() );
}

END_NAMESPACE_YM
