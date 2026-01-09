#ifndef BUNDLEHANDLER_H
#define BUNDLEHANDLER_H

/// @file BundleHandler.h
/// @brief BundleHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class BundleHandler BundleHandler.h "BundleHandler.h"
/// @brief 'bundle' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダ名はバンドル名を表す．
/// * 先頭に members 属性がなければならない．
/// * Simple Attributes
///   - capacitance : float_(value) ;
///   - direction : input | output | inout | internal ;
///   - function : "Boolean expression" ;
/// * Complex Attributes
///   - members ( string, string, ... ) ;
/// * Group Statements
///   - pin ( ) { ... }
//////////////////////////////////////////////////////////////////////
class BundleHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  BundleHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~BundleHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif bundle Group Statement の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstBundle*>& dst_list);


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

  // capacitance
  const AstFloat* mCapacitance;

  // direction
  const AstPinDirection* mDirection;

  // function
  const AstExpr* mFunction;

  // members
  const AstStringVector* mMembers;

  // pin
  vector<const AstPin*> mPinList;

};

END_NAMESPACE_YM_DOTLIB

#endif // BUNDLEHANDLER_H
