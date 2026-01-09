#ifndef LEAKAGECURRENT_H
#define LEAKAGECURRENT_H

/// @file LeakageCurrent.h
/// @brief LeakageCurrent のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LeakageCurrent LeakageCurrent.h "LeakageCurrent.h"
/// @brief 'leakage_current' Grouop Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * Simple Attributes
///   - when : "Bollean expression" ;
///   - value : float ;
/// * Group Statements
///   - pg_current ( <pg_pin_name> ) { ... }
//////////////////////////////////////////////////////////////////////
class LeakageCurrent :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  LeakageCurrent(DotlibParser& parser);

  /// @brief デストラクタ
  ~LeakageCurrent();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif leakage_current Group Statement の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstLeakageCurrent*>& dst_list);


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

  // when
  const AstExpr* mWhen;

  // value
  const AstFloat* mValue;

  // pg_current
  vector<const AstPGCurrent*> mPgCurrentList;

};

END_NAMESPACE_YM_DOTLIB

#endif // LEAKAGECURRENT_H
