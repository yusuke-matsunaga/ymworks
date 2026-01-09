
/// @file ClibCellLibraryTest.cc
/// @brief ClibCellLibraryTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/ClibCellLibrary.h"
#include "ym/StreamMsgHandler.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_CLIB

TEST(ClibCellLibraryTest, empty)
{
  ClibCellLibrary library;
}

TEST(ClibCellLibraryTest, read_mislib)
{
  try {
    auto data_dir = std::filesystem::path{DATA_DIR} / "clib";
    auto filename = data_dir / "lib2.genlib";
    auto library = ClibCellLibrary::read_mislib(filename);

    EXPECT_EQ( 29, library.cell_num() );

    std::ostringstream s;
    library.display(s);

    auto exp_filename = data_dir / "lib2.genlib.display";
    std::ifstream is1{exp_filename};
    ASSERT_TRUE( is1 );

    std::istringstream is2{s.str()};
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
  catch ( AssertError obj ) {
    std::cout << obj << std::endl;
    EXPECT_TRUE( false );
  }
}

TEST(ClibCellLibraryTest, read_liberty)
{
  try {
    auto data_dir = std::filesystem::path{DATA_DIR} / "clib";
    StreamMsgHandler handler(std::cout);
    MsgMgr::attach_handler(&handler);
    auto filename = data_dir / "HIT018.typ.snp";
    auto library = ClibCellLibrary::read_liberty(filename);

    EXPECT_EQ( 310, library.cell_num() );

    std::ostringstream s;
    library.display(s);

    auto exp_filename = data_dir / "HIT018.typ.snp.display";
    std::ifstream is1{exp_filename};
    ASSERT_TRUE( is1 );

    std::istringstream is2{s.str()};
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
  catch ( AssertError obj ) {
    std::cout << obj << std::endl;
    EXPECT_TRUE( false );
  }
}

TEST(ClibCellLibraryTest, dump_restore)
{
  try {
    auto data_dir = std::filesystem::path{DATA_DIR} / "clib";
    auto filename = data_dir / "HIT018.typ.snp";
    auto library = ClibCellLibrary::read_liberty(filename);

    EXPECT_EQ( 310, library.cell_num() );

    std::string dump_buff;
    {
      std::ostringstream s;
      library.dump(s);
      dump_buff = s.str();
    }

    ClibCellLibrary library2;
    {
      std::istringstream s{dump_buff};
      library2 = ClibCellLibrary::restore(s);
    }

    EXPECT_EQ( library.cell_num(), library2.cell_num() );
    EXPECT_EQ( library.to_string(), library2.to_string() );
  }
  catch ( AssertError obj ) {
    std::cerr << obj << std::endl;
    EXPECT_TRUE( false );
  }
}

END_NAMESPACE_YM_CLIB
