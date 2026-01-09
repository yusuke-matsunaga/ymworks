#ifndef MODEVALUEHANDLER_H
#define MODEVALUEHANDLER_H

/// @file ModeValueHandler.h
/// @brief ModeValueHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class ModeValHandler ModeValHandler.h "ModeValHandler.h"
/// @brief 'mode_value' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダ名はモード名を表す．
/// * Simple Attributes
///   - when : "Boolean expression" ;
///   - sdf_cond : sdf_expression_(string) ;
//////////////////////////////////////////////////////////////////////
class ModeValHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  ModeValHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~ModeValHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif 'mode_value' Group Statement の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstModeValue*>& dst_list);


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

  // sdf_cond
  const AstString* mSdfCond;

};

END_NAMESPACE_YM_DOTLIB

#endif // MODEVALUEHANDLER_H
