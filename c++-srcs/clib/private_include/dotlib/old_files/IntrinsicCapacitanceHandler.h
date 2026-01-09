#ifndef INTRINSICCAPACITANCEHANDLER_H
#define INTRINSICCAPACITANCEHANDLER_H

/// @file IntrinsicCapacitanceHandler.h
/// @brief IntrinsicCapacitanceHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Str1Group.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class IntrinsicCapacitanceHandler IntrinsicCapacitanceHandler.h "IntrinsicCapacitanceHandler.h"
/// @brief 'intrinsic_capacitance' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダ名はピン名のリストを表す．
/// * Simple Attributes
///   - value : float ;
//////////////////////////////////////////////////////////////////////
class IntrinsicCapacitanceHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  IntrinsicCapacitanceHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~IntrinsicCapacitanceHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif intrinsic_capacitance の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstIntrinsicCapacitance*>& dst_list);


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

  // value
  const AstFloat* mValue;

};

END_NAMESPACE_YM_DOTLIB

#endif // INTRINSICCAPACITANCEHANDLER_H
