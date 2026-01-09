
/// @file TruthTable_test.cc
/// @brief TruthTable_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/TruthTable.h"


BEGIN_NAMESPACE_YM

TEST(TruthTableTest, empty_construct)
{
  TruthTable tt;

  EXPECT_EQ( 0, tt.input_num() );
  EXPECT_EQ( 0, tt.output_num() );
}

TEST(TruthTableTest, construct1)
{
  SizeType ni = 10;
  SizeType no = 4;
  TruthTable tt(ni, no);

  EXPECT_EQ( ni, tt.input_num() );
  EXPECT_EQ( no, tt.output_num() );

  SizeType ni_exp = 1 << ni;
  for ( SizeType o = 0; o < no; ++ o ) {
    for ( SizeType i = 0; i < ni_exp; ++ i ) {
      auto v = tt.get_value(i, o);
      EXPECT_EQ( Val3::X, v );
    }
  }
}

TEST(TruthTableTest, encode_index)
{
  SizeType ni = 4;
  SizeType no = 1;
  TruthTable tt(ni, no);

  EXPECT_EQ(  0, tt.encode_index(std::vector<bool>{false, false, false, false}) );
  EXPECT_EQ(  1, tt.encode_index(std::vector<bool>{false, false, false, true }) );
  EXPECT_EQ(  2, tt.encode_index(std::vector<bool>{false, false, true,  false}) );
  EXPECT_EQ(  3, tt.encode_index(std::vector<bool>{false, false, true,  true }) );
  EXPECT_EQ(  4, tt.encode_index(std::vector<bool>{false, true,  false, false}) );
  EXPECT_EQ(  5, tt.encode_index(std::vector<bool>{false, true,  false, true }) );
  EXPECT_EQ(  6, tt.encode_index(std::vector<bool>{false, true,  true,  false}) );
  EXPECT_EQ(  7, tt.encode_index(std::vector<bool>{false, true,  true,  true }) );
  EXPECT_EQ(  8, tt.encode_index(std::vector<bool>{true,  false, false, false}) );
  EXPECT_EQ(  9, tt.encode_index(std::vector<bool>{true,  false, false, true }) );
  EXPECT_EQ( 10, tt.encode_index(std::vector<bool>{true,  false, true,  false}) );
  EXPECT_EQ( 11, tt.encode_index(std::vector<bool>{true,  false, true,  true }) );
  EXPECT_EQ( 12, tt.encode_index(std::vector<bool>{true,  true,  false, false}) );
  EXPECT_EQ( 13, tt.encode_index(std::vector<bool>{true,  true,  false, true }) );
  EXPECT_EQ( 14, tt.encode_index(std::vector<bool>{true,  true,  true,  false}) );
  EXPECT_EQ( 15, tt.encode_index(std::vector<bool>{true,  true,  true,  true }) );
}

TEST(TruthTableTest, decode_index)
{
  SizeType ni = 10;
  SizeType no = 1;
  TruthTable tt(ni, no);

  SizeType ni_exp = 1 << ni;
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    auto input_vals = tt.decode_index(b);
    auto index = tt.encode_index(input_vals);
    EXPECT_EQ( b, index );
  }
}

TEST(TruthTableTest, read_file)
{
  auto data_dir = std::filesystem::path(DATAPATH) /
    std::filesystem::path("logic") /
    std::filesystem::path("tt");
  auto filename = data_dir / std::filesystem::path("1_0_0.truth");
  auto tt = TruthTable::read_file(filename);

  std::ostringstream buf;
  tt.print(buf);

  const char* exp_str = "6 18\n"
    "000011001000100100\n"
    "000001001111000000\n"
    "000010000101011010\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "111111000110101001\n"
    "000011100100010010\n"
    "111100010111011100\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "000000100111001001\n"
    "111001101000000010\n"
    "000001000011011010\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "111100111101001101\n"
    "111010100011111001\n"
    "111111010011100101\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "000000011000010101\n"
    "111010111101100110\n"
    "111001111111011000\n"

    "XXXXXXXXXXXXXXXXXX\n"
    "000010100110100110\n"
    "001001001000111000\n"
    "111111010101110100\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "000100000100001111\n"
    "000010011010111101\n"
    "111101001111110101\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "000011101001000100\n"
    "000011100001100110\n"
    "111101001001100100\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "000001100110011010\n"
    "000001010010001000\n"
    "111101000011110010\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "111100111000100010\n"
    "111110001001100101\n"
    "111100000110111000\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "111110100101000100\n"
    "000010001000001001\n"
    "111111101110101101\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "000100110010110010\n"
    "111011111001010010\n"
    "000000100001010101\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "111101101011000100\n"
    "111101011110010110\n"
    "111110110011100011\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "111011100000111010\n"
    "000010001001000011\n"
    "111100010010110000\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "111101001011100010\n"
    "111000101000100110\n"
    "000000111010110111\n"
    "XXXXXXXXXXXXXXXXXX\n"
    "000000000101111000\n"
    "000110001001110111\n"
    "000001011111010010\n"
    "XXXXXXXXXXXXXXXXXX\n";
  EXPECT_EQ( exp_str, buf.str() );
}

TEST(TruthTableTest, from_bin1)
{
  const char* src_str = "3:2:01d11*0010xX0101";
  auto tt = TruthTable::from_bin(src_str);
  EXPECT_EQ( 3, tt.input_num() );
  EXPECT_EQ( 2, tt.output_num() );
  EXPECT_EQ( "3:2:01-11-0010--0101", tt.bin_str() );
}

TEST(TruthTableTest, from_bin_bad1)
{
  const char* src_str = "01-11*0010--0101";
  EXPECT_THROW( TruthTable::from_bin(src_str),
		std::invalid_argument );
}

TEST(TruthTableTest, from_bin_bad2)
{
  const char* src_str = "3:2:1-11*0010--0101";
  EXPECT_THROW( TruthTable::from_bin(src_str),
		std::invalid_argument );
}

TEST(TruthTableTest, from_bin_bad3)
{
  const char* src_str = "3:2:1A11*0010--0101D";
  EXPECT_THROW( TruthTable::from_bin(src_str),
		std::invalid_argument );
}

TEST(TruthTableTest, from_hex1)
{
  const char* src_str = "3:2:28CA";
  auto tt = TruthTable::from_hex(src_str);
  EXPECT_EQ( 3, tt.input_num() );
  EXPECT_EQ( 2, tt.output_num() );
  EXPECT_EQ( "3:2:0010100011001010", tt.bin_str() );
}

TEST(TruthTableTest, from_hex_bad1)
{
  const char* src_str = "abcdefg";
  EXPECT_THROW( TruthTable::from_hex(src_str),
		std::invalid_argument );
}

TEST(TruthTableTest, from_hex_bad2)
{
  const char* src_str = "2:1:abcdefg";
  EXPECT_THROW( TruthTable::from_hex(src_str),
		std::invalid_argument );
}

TEST(TruthTableTest, from_hex_bad3)
{
  const char* src_str = "2:1:0X";
  EXPECT_THROW( TruthTable::from_hex(src_str),
		std::invalid_argument );
}

TEST(TruthTableTest, support1)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ONE);

  auto sup = tt.get_support(0);

  ASSERT_EQ( 2, sup.size() );
  EXPECT_EQ( true, sup[0] );
  EXPECT_EQ( true, sup[1] );
}

TEST(TruthTableTest, support2)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ONE);
  tt.set_value(3, 0, Val3::ONE);

  auto sup = tt.get_support(0);

  ASSERT_EQ( 2, sup.size() );
  EXPECT_EQ( true, sup[0] );
  EXPECT_EQ( false, sup[1] );
}

TEST(TruthTableTest, support3)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ONE);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ONE);

  auto sup = tt.get_support(0);

  ASSERT_EQ( 2, sup.size() );
  EXPECT_EQ( false, sup[0] );
  EXPECT_EQ( true, sup[1] );
}

TEST(TruthTableTest, support4)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ZERO);

  auto sup = tt.get_support(0);

  ASSERT_EQ( 2, sup.size() );
  EXPECT_EQ( false, sup[0] );
  EXPECT_EQ( false, sup[1] );
}

END_NAMESPACE_YM
