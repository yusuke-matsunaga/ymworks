
/// @file read_truth_test.cc
/// @brief read_truth_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"


BEGIN_NAMESPACE_YM

TEST( BnModelTest, read_truth1 )
{
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filepath = path / "ex61.truth";
  auto model = BnModel::read_truth(filepath);
  int ni = 12;
  int no = 11;
  int nd = 0;
  int ng = 11;
  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  // 出力結果の回帰テスト
  std::ostringstream s1;
  model.write(s1);

  auto ref_path = path / "ex61.bn";
  auto s2 = std::ifstream{ref_path};
  ASSERT_TRUE( s2 );
  std::string ref_contents;
  std::string buff;
  while ( getline(s2, buff) ) {
    ref_contents += buff + '\n';
  }

  EXPECT_EQ( ref_contents, s1.str() );
}

TEST( BnModelTest, read_truth2 )
{
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filepath = path / "ex61.truth";
  auto model = BnModel::read(filepath, "truth");
  int ni = 12;
  int no = 11;
  int nd = 0;
  int ng = 11;
  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  // 出力結果の回帰テスト
  std::ostringstream s1;
  model.write(s1);

  auto ref_path = path / "ex61.bn";
  auto s2 = std::ifstream{ref_path};
  ASSERT_TRUE( s2 );
  std::string ref_contents;
  std::string buff;
  while ( getline(s2, buff) ) {
    ref_contents += buff + '\n';
  }

  EXPECT_EQ( ref_contents, s1.str() );
}

TEST( BnModelTest, read_truth3 )
{
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filepath = path / "ex61.truth";
  auto model = BnModel::read(filepath);
  int ni = 12;
  int no = 11;
  int nd = 0;
  int ng = 11;
  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  // 出力結果の回帰テスト
  std::ostringstream s1;
  model.write(s1);

  auto ref_path = path / "ex61.bn";
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
