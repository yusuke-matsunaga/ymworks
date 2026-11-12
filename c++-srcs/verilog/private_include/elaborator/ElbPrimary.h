#ifndef ELBPRIMARY_H
#define ELBPRIMARY_H

/// @file ElbPrimary.h
/// @brief ElbPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlValueType.h"
#include "ym/vl/AstExpr.h"
#include "elaborator/ElbDeclRef.h"
#include "elaborator/ElbSelecter.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbPrimary ElbPrimary.h "ElbPrimary.h"
/// @brief プライマリの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbPrimary
{
public:

  /// @brief コンストラクタ
  ElbPrimary(
    const AstExpr& ast_expr,    ///< [in] パース木の構文要素
    const ElbDeclRef& decl_ref, ///< [in] 宣言用への参照
    const ElbSelecter& sel = {} ///< [in] ビット/部分選択の情報
  ) : mAstExpr{ast_expr},
      mDeclRef{decl_ref},
      mSelecter{sel}
  {
  }

  /// @brief デストラクタ
  ~ElbPrimary() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木の構文要素を返す．
  const AstExpr&
  ast_expr() const
  {
    return mAstExpr;
  }

  /// @brief 宣言要素への参照を得る．
  const ElbDeclRef&
  decl_ref() const
  {
    return mDeclRef;
  }

  /// @brief ビット/部分選択の情報を得る．
  const ElbSelecter&
  selecter() const
  {
    return mSelecter;
  }

  /// @brief ビット幅を返す．
  SizeType
  bit_width() const
  {
    if ( mSelecter.type() == VlSelectType::NONE ) {
      return mDeclRef.declbase()->value_type().size();
    }
    return mSelecter.select_width();
  }

  /// @brief ビット/部分選択した結果を返す．
  ElbPrimary
  slice(
    SizeType offset, ///< [in] 開始位置のオフセット
    SizeType width   ///< [in] 選択する幅
  ) const
  {
    auto new_selecter = mSelecter.slice(offset, width);
    return ElbPrimary(mAstExpr, mDeclRef, new_selecter);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の構文要素を返す．
  AstExpr mAstExpr;

  // 宣言要素への参照
  ElbDeclRef mDeclRef;

  // ビット/部分選択の情報
  ElbSelecter mSelecter;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBPRIMARY_H
