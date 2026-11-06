#ifndef EIDECLITEMPRIMARY_H
#define EIDECLITEMPRIMARY_H

/// @file EiDeclItemPrimary.h
/// @brief EiDeclItemPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclPrimary EiDeclItemPrimary.h "EiDeclItemPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclItemPrimary :
  public EiPrimaryBase
{
public:

  /// @brief コンストラクタ
  EiDeclItemPrimary(
    const AstDeclItem& ast_item, ///< [in] パース木の定義要素
    const VlDecl* obj            ///< [in] 本体のオブジェクト
  ) : mAstObj{ast_item},
      mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiDeclItemPrimary() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDecl*
  decl_obj() const override;

  /// @brief Verilog-HDL の文字列を得る．
  std::string
  decompile() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の定義要素
  AstDeclItem mAstObj;

  // 対象の宣言要素
  const VlDecl* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLITEMPRIMARY_H
