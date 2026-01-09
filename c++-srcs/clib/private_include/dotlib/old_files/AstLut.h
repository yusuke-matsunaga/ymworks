#ifndef ASTLUT_H
#define ASTLUT_H

/// @file AstLut.h
/// @brief AstLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib_nsdef.h"
#include "AstNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstLut AstLut.h "AstLut.h"
/// @brief lut 情報を表すクラス
//////////////////////////////////////////////////////////////////////
class AstLut :
  public AstNode
{
public:

  /// @brief コンストラクタ
  AstLut(const FileRegion& attr_loc, ///< [in] 属性のファイル上の位置
	 const StrHandler& header,   ///< [in] ヘッダのハンドラ
	 const TableHandler& group); ///< [in] グループ本体のハンドラ

  /// @brief デストラクタ
  ~AstLut();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレート名を返す．
  const AstString*
  name() const
  {
    return mName;
  }

  /// @brief 'index_1' のノードを返す．
  ///
  /// 未定義なら nullptr を返す．
  const AstFloatVector*
  index_1() const
  {
    return mIndex1;
  }

  /// @brief 'index_2' のノードを返す．
  ///
  /// 未定義なら nullptr を返す．
  const AstFloatVector*
  index_2() const
  {
    return mIndex2;
  }

  /// @brief 'index_3' のノードを返す．
  ///
  /// 未定義なら nullptr を返す．
  const AstFloatVector*
  index_3() const
  {
    return mIndex3;
  }

  /// @brief 値のリストを返す．
  const AstFloatVector*
  value_list() const
  {
    return mValueList;
  }

  /// @brief 'coefs' のノードを返す．
  const AstFloatVector*
  coefs() const
  {
    return mCoefs;
  }

  /// @brief 'orders' のノードを返す．
  const AstIntVector*
  orders() const
  {
    return mOrders;
  }

  /// @brief 'variable_1_range' のノードを返す．
  const AstFloat2*
  variable_1_range() const
  {
    return mVar1Range;
  }

  /// @brief 'variable_2_range' のノードを返す．
  const AstFloat2*
  variable_2_range() const
  {
    return mVar2Range;
  }

  /// @brief 'variable_3_range' のノードを返す．
  const AstFloat2*
  variable_3_range() const
  {
    return mVar3Range;
  }

  /// @brief 'domain' のノードを返す．
  ///
  /// 未定義なら nullptr を返す．
  const AstDomain*
  domain() const
  {
    return mDomain;
  }

  /// @brief 内容をストリーム出力する．
  void
  dump(ostream& s,     ///< [in] 出力先のストリーム
       int indent = 0) ///< [in] インデント量
    const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート名
  const AstString* mName;

  // index_1
  const AstFloatVector* mIndex1;

  // index_2
  const AstFloatVector* mIndex2;

  // index_3
  const AstFloatVector* mIndex3;

  // 値のリスト
  const AstFloatVector* mValueList;

  // coefs
  const AstFloatVector* mCoefs;

  // orders
  const AstIntVector* mOrders;

  // variable_1_range
  const AstFloat2* mVar1Range;

  // variable_2_range
  const AstFloat2* mVar2Range;

  // variable_3_range
  const AstFloat2* mVar3Range;

  // domain
  const AstDomain* mDomain;

};

END_NAMESPACE_YM_DOTLIB

#endif // ASTLUT_H
