#ifndef ASTDOMAIN_H
#define ASTDOMAIN_H

/// @file AstDomain.h
/// @brief AstDomain のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib_nsdef.h"
#include "AstNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstDomain AstDomain.h "AstDomain.h"
/// @brief domain group statement を表すクラス
//////////////////////////////////////////////////////////////////////
class AstDomain :
  public AstNode
{
public:

  /// @brief コンストラクタ
  AstDomain(const FileRegion& attr_loc,  ///< [in] 属性のファイル上の位置
	    const StrHandler& header,    ///< [in] ヘッダを読み込んだハンドラ
	    const DomainHandler& group); ///< [in] グループ本体を読み込んだハンドラ


  /// @brief デストラクタ
  ~AstDomain();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const AstString*
  name() const;

  /// @brief calc_mode 属性を返す．
  const AstString*
  calc_mode() const;

  /// @brief coefs 属性を返す．
  const AstFloatVector*
  coefs() const;

  /// @brief orders 属性を返す．
  const AstIntVector*
  orders() const;

  /// @brief variable_1_range 属性を返す．
  const AstFloat2*
  variable_1_range() const;

  /// @brief variable_2_range 属性を返す．
  const AstFloat2*
  variable_2_range() const;

  /// @brief variable_3_range 属性を返す．
  const AstFloat2*
  variable_3_range() const;

  /// @brief 内容をストリーム出力する．
  void
  dump(ostream& s,                     ///< [in] 出力先のストリーム
       int indent = 0) const override; ///< [in] インデント量


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const AstString* mName;

  // calc_mode 属性
  const AstString* mCalcMode;

  // coefs 属性
  const AstFloatVector* mCoefs;

  // orders 属性
  const AstIntVector* mOrders;

  // variable_1_range 属性
  const AstFloat2* mVar1Range;

  // variable_2_range 属性
  const AstFloat2* mVar2Range;

  // variable_3_range 属性
  const AstFloat2* mVar3Range;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前を返す．
inline
const AstString*
AstDomain::name() const
{
  return mName;
}

// @brief calc_mode 属性を返す．
inline
const AstString*
AstDomain::calc_mode() const
{
  return mCalcMode;
}

// @brief coefs 属性を返す．
inline
const AstFloatVector*
AstDomain::coefs() const
{
  return mCoefs;
}

// @brief orders 属性を返す．
inline
const AstIntVector*
AstDomain::orders() const
{
  return mOrders;
}

// @brief variable_1_range 属性を返す．
inline
const AstFloat2*
AstDomain::variable_1_range() const
{
  return mVar1Range;
}

// @brief variable_2_range 属性を返す．
inline
const AstFloat2*
AstDomain::variable_2_range() const
{
  return mVar2Range;
}

// @brief variable_3_range 属性を返す．
inline
const AstFloat2*
AstDomain::variable_3_range() const
{
  return mVar3Range;
}

END_NAMESPACE_YM_DOTLIB

#endif // ASTDOMAIN_H
