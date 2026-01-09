#ifndef FUNCIONALYIELDMETRICHANDLER_H
#define FUNCIONALYIELDMETRICHANDLER_H

/// @file FuncionalYieldMetricHandler.h
/// @brief FuncionalYieldMetricHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/EmptyGroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class FunctionalYieldMetricHandler FunctionalYieldMetricHandler.h "FunctionalYieldMetricHandler.h"
/// @brief 'functional_yield_metric' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * Group Statements
///   - average_number_of_faults ( name_(faults_lut_template) ) { ... }
//////////////////////////////////////////////////////////////////////
class FunctionalYieldMetricHandler :
  public EmptyGroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  FunctionalYieldMetricHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~FunctionalYieldMetricHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif functional_yield_metric Group Statement の記述をパースする．
  /// @param[in] dst 読み込んだ値を格納する変数
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(const AstFYM*& dst);


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

  // average_number_of_faults
  vector<const AstAnof*> mAnofList;

};

END_NAMESPACE_YM_DOTLIB

#endif // FUNCIONALYIELDMETRICHANDLER_H
