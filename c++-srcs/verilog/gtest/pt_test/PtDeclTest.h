#ifndef PTDECLTEST_H
#define PTDECLTEST_H

/// @file PtDeclTest.h
/// @brief PtDeclTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtTest.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtDeclTest PtDeclTest.h "PtDeclTest.h"
/// @brief PtDecl 用テスト環境
//////////////////////////////////////////////////////////////////////
class PtDeclTest :
  public PtTest
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief PtIOH のテスト
  void
  check_IOHead(
    const PtIOHead* iohead,
    const FileRegion& fr,
    VpiDir dir,
    VpiAuxType aux_type,
    VpiNetType net_type,
    VpiVarType var_type,
    bool is_signed,
    const PtRange* range,
    const std::vector<const PtIOItem*>& item_vec
  )
  {
    check_Base(iohead, fr);
    EXPECT_EQ( dir, iohead->direction() );
    EXPECT_EQ( aux_type, iohead->aux_type() );
    EXPECT_EQ( net_type, iohead->net_type() );
    EXPECT_EQ( var_type, iohead->var_type() );
    EXPECT_EQ( is_signed, iohead->is_signed() );
    EXPECT_EQ( range, iohead->range() );
    EXPECT_EQ( item_vec, to_vector(iohead->item_top()) );
  }

  /// @brief PtIOItem のテスト
  void
  check_IOItem(
    const PtIOItem* ioitem,
    const FileRegion& fr,
    const char* name,
    const PtExpr* init_value
  )
  {
    check_Base(ioitem, fr);
    EXPECT_STREQ( name, ioitem->name() );
    EXPECT_EQ( init_value, ioitem->init_value() );
  }

  /// @brief PtDeclHead のテスト
  void
  check_DeclHead(
    const PtDeclHead* declhead,
    const FileRegion& fr,
    AstDeclHead::Type type,
    bool is_signed,
    const PtRange* range,
    VpiVarType data_type,
    VpiNetType net_type,
    VpiVsType vs_type,
    const PtStrength* strength,
    const PtDelay* delay,
    const std::vector<const PtDeclItem*>& item_vec
  )
  {
    check_Base(declhead, fr);
    EXPECT_EQ( type, declhead->type() );
    EXPECT_EQ( is_signed, declhead->is_signed() );
    EXPECT_EQ( range, declhead->range() );
    EXPECT_EQ( data_type, declhead->data_type() );
    EXPECT_EQ( net_type, declhead->net_type() );
    EXPECT_EQ( vs_type, declhead->vs_type() );
    EXPECT_EQ( strength, declhead->strength() );
    EXPECT_EQ( delay, declhead->delay() );
    EXPECT_EQ( item_vec, to_vector(declhead->item_top()) );
  }

  /// @brief PtDeclItem のテスト
  void
  check_DeclItem(
    const PtDeclItem* declitem,
    const FileRegion& fr,
    const char* name,
    const std::vector<const PtRange*>& range_vec,
    const PtExpr* init_value
  )
  {
    check_Base(declitem, fr);
    EXPECT_STREQ( name, declitem->name() );
    EXPECT_EQ( range_vec, to_vector(declitem->range_top()) );
    EXPECT_EQ( init_value, declitem->init_value() );
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PTDECLTEST_H
