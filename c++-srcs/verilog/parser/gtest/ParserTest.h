#ifndef PARSERTEST_H
#define PARSERTEST_H

/// @file ParserTest.h
/// @brief ParserTest のヘッダファイル
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
/// @class ParserTest ParserTest.h "ParserTest.h"
/// @brief Parser のテスト環境
//////////////////////////////////////////////////////////////////////
class ParserTest :
  public ::testing::Test
{
public:

  // コンストラクタ
  ParserTest(
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
    const char* name = nullptr
  )
  {
    check_Base(obj, fr);
    EXPECT_STREQ( name, obj->name() );
  }

  /// @brief AstHierNamedBase のテスト
  void
  check_HierNamedBase(
    const AstHierNamedBase* obj,
    const FileRegion& fr,
    const char* name = nullptr,
    const std::vector<const AstNameBranch*>& nb_vec = {}
  )
  {
    check_NamedBase(obj, fr, name);
    EXPECT_EQ( nb_vec, obj->namebranch_list().to_vector() );
    std::string exp_name;
    if ( obj->name() != nullptr ) {
      for ( auto nb: obj->namebranch_list() ) {
	exp_name += nb->decompile();
	exp_name += ".";
      }
      exp_name += obj->name();
    }
    EXPECT_EQ( exp_name, obj->decompile_name() );
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

#endif // PARSERTEST_H
