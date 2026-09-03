#ifndef PTTEST_H
#define PTTEST_H

/// @file PtTest.h
/// @brief PtTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "parser/Parser.h"
#include "parser/PtNameBranch.h"
#include "parser/PtPart.h"
#include "parser/PtRange.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtTest PtTest.h "PtTest.h"
/// @brief PtXXX のテスト環境
//////////////////////////////////////////////////////////////////////
class PtTest :
  public ::testing::Test
{
public:

  /// @brief NameBranch の仕様を表す構造体
  struct NameBranchSpec {
    const char* name;
    int index; // 最下位ビットが1の時 インデックスを持つ.

    std::string
    decompile() const
    {
      std::ostringstream buf;
      buf << name;
      if ( index & 1 ) {
	buf << "[" << (index / 2) << "]";
      }
      return buf.str();
    }
  };


public:

  // コンストラクタ
  PtTest(
  ) : parser(astmgr),
      factory(parser.factory()),
      file_info("filename1")
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  // FileRegion を作る．
  FileRegion
  make_file_region(
    SizeType start_line,
    SizeType start_column,
    SizeType end_line,
    SizeType end_column
  )
  {
    return FileRegion(file_info,
		      start_line, start_column,
		      end_line, end_column);
  }

  // Range を作る．
  PtRange*
  new_range(
    const FileRegion& left_fr,
    SizeType left_val,
    const FileRegion& right_fr,
    SizeType right_val
  )
  {
    auto left = factory.new_IntConst(left_fr, left_val);
    auto right = factory.new_IntConst(right_fr, right_val);
    return factory.new_Range(FileRegion(left_fr, right_fr), left, right);
  }

  // Part を作る．
  const PtPart*
  new_part(
    const FileRegion& left_fr,
    SizeType left_val,
    const FileRegion& right_fr,
    SizeType right_val,
    VpiRangeMode mode
  )
  {
    auto left = factory.new_IntConst(left_fr, left_val);
    auto right = factory.new_IntConst(right_fr, right_val);
    switch ( mode ) {
    case VpiRangeMode::Const:
      return factory.new_PartConst(FileRegion(left_fr, right_fr),
					    left, right);
    case VpiRangeMode::Plus:
      return factory.new_PartPlus(FileRegion(left_fr, right_fr),
					   left, right);
    case VpiRangeMode::Minus:
      return factory.new_PartMinus(FileRegion(left_fr, right_fr),
					    left, right);
    default:
      break;
    }
    throw std::logic_error{"never be reached"};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 個々の型のテスト
  //////////////////////////////////////////////////////////////////////

  /// @brief PtBase のテスト
  void
  check_Base(
    const PtBase* obj,
    const FileRegion& fr
  )
  {
    ASSERT_TRUE( obj != nullptr );
    EXPECT_EQ( fr, obj->file_region() );
  }

  /// @brief PtNameBranch のリストのテスト
  void
  check_namebranch_list(
    const PtNameBranch* nb_top,
    const std::vector<NameBranchSpec>& nbspec_vec
  )
  {
    SizeType index = 0;
    for ( auto nb = nb_top; nb != nullptr; nb = nb->link() ) {
      auto& spec = nbspec_vec[index ++];
      EXPECT_EQ( spec.name, nb->name() );
      if ( spec.index & 1 ) {
	EXPECT_TRUE( nb->has_index() );
	EXPECT_EQ( spec.index / 2, nb->index() );
      }
      else {
	EXPECT_FALSE( nb->has_index() );
	EXPECT_THROW( nb->index(),
		      std::logic_error );
      }
    }
  }

  /// @brief リンクトリストを std::vector<> に変換する．
  template <typename T>
  std::vector<const T*>
  to_vector(
    const T* top
  )
  {
    std::vector<const T*> vec;
    for ( auto x = top; x != nullptr; x = x->link() ) {
      vec.push_back(x);
    }
    return vec;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  AstMgr astmgr;

  Parser parser;

  PtFactory& factory;

  FileInfo file_info;

};

END_NAMESPACE_YM_VERILOG

#endif // PTTEST_H
