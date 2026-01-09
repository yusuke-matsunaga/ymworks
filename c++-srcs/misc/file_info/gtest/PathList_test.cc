
/// @file PathList_test.cc
/// @brief PathList_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/PathList.h"


BEGIN_NAMESPACE_YM

TEST(PahtListTest, empty_construct)
{
  PathList path_list;

  EXPECT_EQ( "", path_list.to_string() );
}

TEST(PathListTest, construct1)
{
  auto data_dir = Path(DATA_DIR);
  auto path1 = data_dir / Path("clib");
  auto path2 = data_dir / Path("bn");
  PathList path_list({path1, path2});

  std::ostringstream buf;
  buf << DATA_DIR << "/clib"
      << ":"
      << DATA_DIR << "/bn";
  EXPECT_EQ( buf.str(), path_list.to_string() );
}

TEST(PathListTest, construct2)
{
  PathList path_list("a:b");

  EXPECT_EQ( "a:b", path_list.to_string() );
}

TEST(PathListTest, set_from_string)
{
  const char* str = "/a/b/c:/x/y/z";
  PathList path_list;

  path_list.set_from_string(str);

  EXPECT_EQ( str, path_list.to_string() );
}

TEST(PathListTest, search1)
{
  // 相対パスの結果を固定するためにカレントディレクトリを設定する．
  std::filesystem::current_path(Path(DATA_DIR));

  // 空のサーチパスで存在しないファイルを探す．
  PathList path_list;

  // 絶対パスの例
  auto path1 = path_list.search(Path("/a/b/c/"));
  EXPECT_TRUE( path1.empty() );

  // 相対パスの例
  auto path2 = path_list.search(Path("xyz"));
  EXPECT_TRUE( path2.empty() );
}

TEST(PathListTest, search2)
{
  // 相対パスの結果を固定するためにカレントディレクトリを設定する．
  std::filesystem::current_path(Path(DATA_DIR));

  // 空のサーチパスで存在するファイルを探す．
  PathList path_list;

  // 絶対パスの例
  auto path1 = path_list.search(Path(DATA_DIR) / Path("aig") / Path("test1.aig"));
  EXPECT_FALSE( path1.empty() );
  auto status1 = std::filesystem::status(path1);
  EXPECT_TRUE( status1.type() == std::filesystem::file_type::regular );

  // 相対パスの例
  auto path2 = path_list.search(Path("aig") / Path("test1.aig"));
  EXPECT_FALSE( path2.empty() );
  auto status2 = std::filesystem::status(path2);
  EXPECT_TRUE( status2.type() == std::filesystem::file_type::regular );
}

TEST(PathListTest, search3)
{
  // 相対パスの結果を固定するためにカレントディレクトリを設定する．
  std::filesystem::current_path(Path(DATA_DIR));

  auto dir1 = Path(DATA_DIR) / Path("aig");
  auto dir2 = Path(DATA_DIR) / Path("bn");
  PathList path_list({dir1, dir2});

  // "aig" の下にあるファイル
  auto path1 = path_list.search(Path("test1.aig"));
  EXPECT_FALSE( path1.empty() );
  auto status1 = std::filesystem::status(path1);
  EXPECT_TRUE( status1.type() == std::filesystem::file_type::regular );

  // "bn" の下にあるファイル
  auto path2 = path_list.search(Path("s5378.blif"));
  EXPECT_FALSE( path2.empty() );
  auto status2 = std::filesystem::status(path2);
  EXPECT_TRUE( status2.type() == std::filesystem::file_type::regular );

  // 存在しないファイル
  auto path3 = path_list.search(Path("abc"));
  EXPECT_TRUE( path3.empty() );
}

END_NAMESPACE_YM
