#ifndef INTRINSICRESISTANCEHANDLER_H
#define INTRINSICRESISTANCEHANDLER_H

/// @file IntrinsicResistanceHandler.h
/// @brief IntrinsicResistanceHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Str1Group.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class IntrinsicResistanceHandler IntrinsicResistanceHandler.h "IntrinsicResistanceHandler.h"
/// @brief 'intrinsic_resistance' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダ名はピン名のリストを表す．
/// * Simple Attributes
///   - related_out : string ;
///   - value : float ;
//////////////////////////////////////////////////////////////////////
class IntrinsicResistanceHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  IntrinsicResistanceHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~IntrinsicResistanceHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif intrinsic_resistance の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstIntrinsicResistance*>& dst_list);


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

  // related_output
  const AstString* mRelatedOutput;

  // value
  const AstFloat* mValue;

};

END_NAMESPACE_YM_DOTLIB

#endif // INTRINSICRESISTANCEHANDLER_H
