
/// @file read_blif_test.cc
/// @brief read_blif_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"


BEGIN_NAMESPACE_YM

TEST( BnModelTest, read_blif1)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filename = "s5378.blif";
  auto filepath = path / filename;

  auto model = BnModel::read_blif(filepath);

  const SizeType ni = 35;
  const SizeType no = 49;
  const SizeType nd = 179;
  const SizeType ng = 2779;

  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  EXPECT_THROW( model.input(ni),
		std::out_of_range );
  EXPECT_THROW( model.input_name(ni),
		std::out_of_range );
  EXPECT_THROW( model.output(no),
		std::out_of_range );
  EXPECT_THROW( model.output_name(no),
		std::out_of_range );
  EXPECT_THROW( model.dff(nd),
		std::out_of_range );
  EXPECT_THROW( model.dff_name(nd),
		std::out_of_range );
  EXPECT_THROW( model.logic(ng),
		std::out_of_range );

  std::ostringstream s;
  model.write(s);

  auto exp_filename = "s5378.bn";
  auto exp_path = path / exp_filename;
  auto is1 = std::ifstream{exp_path};
  ASSERT_TRUE( is1 );

  auto is2 = std::istringstream{s.str()};
  std::string buff1;
  std::string buff2;
  while ( getline(is1, buff1) ) {
    getline(is2, buff2);
    EXPECT_TRUE( is2 );
    EXPECT_EQ( buff1, buff2 );
  }
  getline(is2, buff2);
  EXPECT_FALSE( is2 );
}

TEST( BnModelTest, read_blif2)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filename = "s5378.blif";
  auto filepath = path / filename;

  auto model = BnModel::read(filepath, "blif");

  const SizeType ni = 35;
  const SizeType no = 49;
  const SizeType nd = 179;
  const SizeType ng = 2779;

  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  std::ostringstream s;
  model.write(s);

  auto exp_filename = "s5378.bn";
  auto exp_path = path / exp_filename;
  auto is1 = std::ifstream{exp_path};
  ASSERT_TRUE( is1 );

  auto is2 = std::istringstream{s.str()};
  std::string buff1;
  std::string buff2;
  while ( getline(is1, buff1) ) {
    getline(is2, buff2);
    EXPECT_TRUE( is2 );
    EXPECT_EQ( buff1, buff2 );
  }
  getline(is2, buff2);
  EXPECT_FALSE( is2 );
}

TEST( BnModelTest, read_blif3)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filename = "s5378.blif";
  auto filepath = path / filename;

  auto model = BnModel::read(filepath);

  const SizeType ni = 35;
  const SizeType no = 49;
  const SizeType nd = 179;
  const SizeType ng = 2779;

  EXPECT_EQ( ni, model.input_num() );
  EXPECT_EQ( no, model.output_num() );
  EXPECT_EQ( nd, model.dff_num() );
  EXPECT_EQ( ng, model.logic_num() );

  std::ostringstream s;
  model.write(s);

  auto exp_filename = "s5378.bn";
  auto exp_path = path / exp_filename;
  auto is1 = std::ifstream{exp_path};
  ASSERT_TRUE( is1 );

  auto is2 = std::istringstream{s.str()};
  std::string buff1;
  std::string buff2;
  while ( getline(is1, buff1) ) {
    getline(is2, buff2);
    EXPECT_TRUE( is2 );
    EXPECT_EQ( buff1, buff2 );
  }
  getline(is2, buff2);
  EXPECT_FALSE( is2 );
}

TEST( BnModelTest, read_blif_bad1)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filename = "s5378.blif";
  auto filepath = path / filename;

  EXPECT_THROW( BnModel::read(filepath, "iscas89"),
		std::invalid_argument );
}

TEST( BnModelTest, read_blif_bad2)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filename = "s5378.blif";
  auto filepath = path / filename;

  EXPECT_THROW( BnModel::read(filepath, "truth"),
		std::invalid_argument );
}

TEST( BnModelTest, read_blif_bad3)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "bn";
  auto filename = "s5378.blif";
  auto filepath = path / filename;

  EXPECT_THROW( BnModel::read(filepath, "abc"),
		std::invalid_argument );
}

TEST( BnModelTest, read_blif_file_not_found)
{
  // 存在しないファイルの場合の例外送出テスト
  EXPECT_THROW( BnModel::read_blif("not_exist_file"),
		std::invalid_argument );
}

TEST( BnModelTest, read_blif_wrong_data)
{
  // 誤った内容のファイルの場合の例外送出テスト
  auto filename = "broken.blif";
  auto path = std::filesystem::path{DATAPATH};
  path.append("bn");
  path.append(filename);
  EXPECT_THROW( BnModel::read_blif(path),
		std::invalid_argument );
}

END_NAMESPACE_YM
