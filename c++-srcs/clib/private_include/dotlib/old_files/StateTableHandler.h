#ifndef STATETABLEHANDLER_H
#define STATETABLEHANDLER_H

/// @file StateTableHandler.h
/// @brief StateTableHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class StateTableHandler StateTableHandler.h "StateTableHandler.h"
/// @brief 'statetable' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダの２つの文字列は input node names と internal node names を表す．
/// * Simple Attributes
///   - table: string ; // テーブル記述
//////////////////////////////////////////////////////////////////////
class StateTableHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  StateTableHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~StateTableHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif statetable Group Statement の記述をパースする．
  /// @param[in] dst 読み込んだ値を格納する変数
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(const AstStateTable*& dst);


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

  // table
  const AstString* mTable;

};

END_NAMESPACE_YM_DOTLIB

#endif // STATETABLEHANDLER_H
