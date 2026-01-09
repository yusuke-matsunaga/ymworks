#ifndef LEAKAGEPOWER_H
#define LEAKAGEPOWER_H

/// @file LeakagePower.h
/// @brief LeakagePower のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/EmptyGroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LeakagePower LeakagePower.h "LeakagePower.h"
/// @brief 'leakage_power' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * Simple Attributes
///   - power_level : string ;
///   - related_pg_pin : string ;
///   - when : "Boolean expression" ;
///   - value : float ;
//////////////////////////////////////////////////////////////////////
class LeakagePower
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  LeakagePower(DotlibParser& parser);

  /// @brief デストラクタ
  ~LeakagePower();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif leakage_power Group Statement の記述をパースする．
  /// @param[in] dst 読み込んだ値を格納する変数
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(const AstLekagePower*& dst);


protected:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ記述の始まり
  void
  begin_group() override;

  /// @brief グループ記述の終わり
  /// @param[in] group_loc グループ全体のファイル上の位置
  /// @retval true 正常にパーズした．
  /// @retval false パーズ中にエラーが起こった．
  bool
  end_group(const FileRegion& group_loc) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // power_level
  const AstString* mPowerLevel;

  // related_pin
  const AstString* mRelatedPin;

  // when
  const AstExpr* mWhen;

  // value
  const AstFloat* mFloat;

};

END_NAMESPACE_YM_DOTLIB

#endif // LEAKAGEPOWER_H
