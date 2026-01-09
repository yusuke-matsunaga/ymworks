#ifndef BUSHANDLER_H
#define BUSHANDLER_H

/// @file BusHandler.h
/// @brief BusHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Str1GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class BusHandler BusHandler.h "BusHandler.h"
/// @brief 'bus' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダの文字列はバスを表す．
/// * Simple Attributes
///   - bus_type : name ;
///   - capacitance : float ;
///   - direction : input | output | inout | internal ;
/// * Group Statements
///   - pin () { ... }
//////////////////////////////////////////////////////////////////////
class BusHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  BusHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~BusHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif bus Group Statement の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstBus*>& dst_list);


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

  // bus_type
  const AstString* mBusType;

  // capacitance
  const AstFloat* mCapacitance;

  // direction
  const AstPinDirection* mDirection;

  // pin
  vector<const AstPin*> mPinList;

};

END_NAMESPACE_YM_DOTLIB

#endif // BUSHANDLER_H
