#ifndef FLINFO_H
#define FLINFO_H

/// @file FLInfo.h
/// @brief FLInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/ElemInfo.h"
#include "ym/ShString.h"
#include "ym/clib.h"


BEGIN_NAMESPACE_YM_DOTLIB

class AstExpr;

//////////////////////////////////////////////////////////////////////
/// @class FLInfo FLInfo.h "FLInfo.h"
/// @brief FF/ラッチに共通なパース情報を持つクラス
//////////////////////////////////////////////////////////////////////
class FLInfo :
  public ElemInfo
{
public:

  /// @brief コンストラクタ
  FLInfo(
    LibraryInfo& library_info
  ) : ElemInfo{library_info}
  {
  }

  /// @brief デストラクタ
  ~FLInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 情報をセットする．
  void
  set_common(
    const AstValue* val ///< [in] パース木の情報
  );

  /// @brief 変数名1を返す．
  const ShString&
  var1() const
  {
    return mVar1;
  }

  /// @brief 変数名2を返す．
  const ShString&
  var2() const
  {
    return mVar2;
  }

  /// @brief clear の値を返す．
  const AstExpr*
  clear() const
  {
    return mClear;
  }

  /// @brief preset の値を返す．
  const AstExpr*
  preset() const
  {
    return mPreset;
  }

  /// @brief clear_preset_var1 の値を返す．
  ClibCPV
  clear_preset_var1() const
  {
    return mCpv1;
  }

  /// @brief clear_preset_var1 の値を返す．
  ClibCPV
  clear_preset_var2() const
  {
    return mCpv2;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態変数名1
  ShString mVar1;

  // 状態変数名2
  ShString mVar2;

  // clear
  const AstExpr* mClear;

  // preset
  const AstExpr* mPreset;

  // clear_preset_var1
  ClibCPV mCpv1;

  // clear_preset_var2
  ClibCPV mCpv2;

};

END_NAMESPACE_YM_DOTLIB

#endif // FLINFO_H
