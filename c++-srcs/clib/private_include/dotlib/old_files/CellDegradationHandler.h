#ifndef CELLDEGRADATIONHANDLER_H
#define CELLDEGRADATIONHANDLER_H

/// @file CellDegradationHandler.h
/// @brief CellDegradationHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class CellDegradationHandler CellDegradationHandler.h "CellDegradationHandler.h"
/// @brief 'cell_degradation' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 3
///           "pin Group Description and Syntax"
///
/// * ヘッダの文字列はテンプレート名を表す．
/// * Complex Attributes
///   - coefs ( "float, float, ... " ) ;
///   - index_1 ( ) ;
///   - orders ( "integer, integer, ... " ) ;
///   - values ( ) ;
///   - variable_n_range ( float, float ) ;
/// * Group Attributes
///   - domain ( string ) { ... }
//////////////////////////////////////////////////////////////////////
class CellDegradationHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  CellDegradationHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~CellDegradationHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif 'cell_degradation' Group Statement の記述をパースする．
  /// @param[in] dst 読み込んだ値を格納する変数
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(const AstCellDegradation*& dst);


public:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ記述の始まり
  void
  begin_group() override;

  /// @brief グループ記述の終わり
  /// @retval true 正常にパーズした．
  /// @retval false パーズ中にエラーが起こった．
  bool
  end_group() override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // calc_mode
  const AstString* mCalcMode;

  // index_1
  const AstFloatVector* mIndex1;

  // values
  const AstFloatVector* mValues;

  // coefs
  const AstFloatVector* mCoefs;

  // orders
  const AstIntVector* mOrders;

  // マニュアル読んでも variable_n_range なのか variable_1_range なのかわからない．
  // variable_1_range
  const AstVariableRange* mVar1Range;

  // domain
  const AstDomain* mDomain;

};

END_NAMESPACE_YM_DOTLIB

#endif // CELLDEGRADATIONHANDLER_H
