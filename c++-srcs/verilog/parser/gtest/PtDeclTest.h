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

  /// @brief AstIOH のテスト
  void
  check_IOHead(
    const AstIOHead* iohead,
    const FileRegion& fr,
    VpiDir dir,
    VpiAuxType aux_type,
    VpiNetType net_type,
    VpiVarType var_type,
    bool is_signed,
    const AstRange* range,
    const std::vector<const AstIOItem*>& item_vec
  )
  {
    check_Base(iohead, fr);
    EXPECT_EQ( dir, iohead->direction() );
    EXPECT_EQ( aux_type, iohead->aux_type() );
    EXPECT_EQ( net_type, iohead->net_type() );
    EXPECT_EQ( var_type, iohead->var_type() );
    EXPECT_EQ( is_signed, iohead->is_signed() );
    EXPECT_EQ( range, iohead->range() );
    EXPECT_EQ( item_vec, iohead->item_list().to_vector() );
  }

  /// @brief AstIOItem のテスト
  void
  check_IOItem(
    const AstIOItem* ioitem,
    const FileRegion& fr,
    const char* name,
    const AstExpr* init_value
  )
  {
    check_NamedBase(ioitem, fr, name);
    EXPECT_EQ( init_value, ioitem->init_value() );
  }

  /// @brief AstDeclHead のテスト
  void
  check_DeclHead(
    const AstDeclHead* declhead,
    const FileRegion& fr,
    AstDeclHead::Type type,
    bool is_signed,
    const AstRange* range,
    VpiVarType data_type,
    VpiNetType net_type,
    VpiVsType vs_type,
    const AstStrength* strength,
    const AstDelay* delay,
    const std::vector<const AstDeclItem*>& item_vec
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
    EXPECT_EQ( item_vec, declhead->item_list().to_vector() );
  }

  /// @brief AstDeclItem のテスト
  void
  check_DeclItem(
    const AstDeclItem* declitem,
    const FileRegion& fr,
    const char* name,
    const std::vector<const AstRange*>& range_vec,
    const AstExpr* init_value
  )
  {
    check_NamedBase(declitem, fr, name);
    EXPECT_EQ( range_vec, declitem->range_list().to_vector() );
    EXPECT_EQ( init_value, declitem->init_value() );
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PTDECLTEST_H
