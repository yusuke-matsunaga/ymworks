#ifndef ASTCELLDEGRADATION_H
#define ASTCELLDEGRADATION_H

/// @file AstLut.h
/// @brief AstLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"
#include "AstNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstCellDegradation AstCellDegradation.h "AstCellDegradation.h"
/// @brief lut 情報を表すクラス
//////////////////////////////////////////////////////////////////////
class AstCellDegradation :
  public AstNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル上の位置
  /// @param[in] name 名前
  /// @param[in] calc_mode 'calc_mode' 属性
  /// @param[in] index_1 インデックスのベクタ
  /// @param[in] value_list 値のリスト
  /// @param[in] coefs 'coefs' 属性
  /// @param[in] orders 'orders' 属性
  /// @param[in] variable_1_range 'variable_1_range' 属性
  /// @param[in] domain 'domain'
  AstCellDegradation(const FileRegion& loc,
		     const AstString* name,
		     const AstString* calc_mode,
		     const AstFloatVector* index_1,
		     const AstFloatVector* value_list,
		     const AstFloatVector* coefs,
		     const AstIntVector* orders,
		     const AstVariableRange* variable_1_range,
		     const AstDomain* domain);

  /// @brief デストラクタ
  ~AstCellDegradation();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレート名を返す．
  const AstString*
  name() const;

  /// @brief 'calc_mode' のノードを返す．
  const AstString*
  calc_mode() const;

  /// @brief 'index_1' のノードを返す．
  ///
  /// 未定義なら nullptr を返す．
  const AstFloatVector*
  index_1() const;

  /// @brief 値のリストを返す．
  const AstFloatVector*
  value_list() const;

  /// @brief 'coefs' のノードを返す．
  const AstFloatVector*
  coefs() const;

  /// @brief 'orders' のノードを返す．
  const AstIntVector*
  orders() const;

  /// @brief 'variable_1_range' のノードを返す．
  const AstVariableRange*
  variable_1_range() const;

  /// @brief 'domain' のノードを返す．
  ///
  /// 未定義なら nullptr を返す．
  const AstDomain*
  domain() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  void
  dump(ostream& s,
       int indent = 0) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート名
  const AstString* mName;

  // calc_mode
  const AstString* mCalcMode;

  // index_1
  const AstFloatVector* mIndex1;

  // 値のリスト
  const AstFloatVector* mValueList;

  // coefs
  const AstFloatVector* mCoefs;

  // orders
  const AstIntVector* mOrders;

  // variable_1_range
  const AstVariableRange* mVar1Range;

  // domain
  const AstDomain* mDomain;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief テンプレート名を返す．
inline
const AstString*
AstCellDegradation::name() const
{
  return mName;
}

// @brief 'calc_mode' のノードを返す．
inline
const AstString*
AstCellDegradation::calc_mode() const
{
  return mCalcMode;
}

// @brief 'index_1' のノードを返す．
inline
const AstFloatVector*
AstCellDegradation::index_1() const
{
  return mIndex1;
}

// @brief 値のリストを返す．
inline
const AstFloatVector*
AstCellDegradation::value_list() const
{
  return mValueList;
}

// @brief 'coefs' のノードを返す．
inline
const AstFloatVector*
AstCellDegradation::coefs() const
{
  return mCoefs;
}

// @brief 'orders' のノードを返す．
inline
const AstIntVector*
AstCellDegradation::orders() const
{
  return mOrders;
}

// @brief 'variable_1_range' のノードを返す．
inline
const AstVariableRange*
AstCellDegradation::variable_1_range() const
{
  return mVar1Range;
}

// @brief 'domain' のノードを返す．
//
// 未定義なら nullptr を返す．
inline
const AstDomain*
AstCellDegradation::domain() const
{
  return mDomain;
}

END_NAMESPACE_YM_DOTLIB

#endif // ASTCELLDEGRADATION_H
