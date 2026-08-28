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
#include "ym/vl/BitVector.h"
#include "parser/PtModule.h"
#include "parser/PtUdp.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"
#include "parser/PtMisc.h"
#include "parser/PtStmt.h"
#include "parser/PtHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtTest PtTest.h "PtTest.h"
/// @brief Parser のテスト環境
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

  /// @brief AstBase のテスト
  void
  check_Base(
    const AstBase* obj,
    const FileRegion& fr
  )
  {
    ASSERT_TRUE( obj != nullptr );
    EXPECT_EQ( fr, obj->file_region() );
  }

  /// @brief AstNamedBase のテスト
  void
  check_NamedBase(
    const AstNamedBase* obj,
    const FileRegion& fr,
    const char* name
  )
  {
    check_Base(obj, fr);
    EXPECT_STREQ( name, obj->name() );
  }

  /// @brief AstNamedBase のテスト
  void
  check_NamedBase(
    const AstNamedBase* obj,
    const FileRegion& fr
  )
  {
    check_Base(obj, fr);
    EXPECT_THROW( obj->name(),
		  std::logic_error );
  }

  /// @brief AstHierNamedBase のテスト
  void
  check_HierNamedBase(
    const AstHierNamedBase* obj,
    const FileRegion& fr,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_vec
  )
  {
    check_NamedBase(obj, fr, name);
    SizeType index = 0;
    for ( auto nb: obj->namebranch_list() ) {
      auto& spec = nbspec_vec[index];
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
      ++ index;
    }
    std::string exp_name;
    if ( name != nullptr ) {
      for ( auto nbspec: nbspec_vec ) {
	exp_name += nbspec.decompile();
	exp_name += ".";
      }
      exp_name += name;
    }
    EXPECT_EQ( exp_name, obj->decompile_name() );
  }

  /// @brief AstHierNamedBase のテスト
  void
  check_HierNamedBase(
    const AstHierNamedBase* obj,
    const FileRegion& fr,
    const char* name
  )
  {
    check_NamedBase(obj, fr, name);
    EXPECT_THROW( obj->namebranch_list(),
		  std::logic_error );
  }

  /// @brief AstHierNamedBase のテスト
  void
  check_HierNamedBase(
    const AstHierNamedBase* obj,
    const FileRegion& fr
  )
  {
    check_NamedBase(obj, fr);
    EXPECT_THROW( obj->namebranch_list(),
		  std::logic_error );
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
