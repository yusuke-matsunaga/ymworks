#ifndef INTRINSICPARASITICHANDLER_H
#define INTRINSICPARASITICHANDLER_H

/// @file IntrinsicParasiticHandler.h
/// @brief IntrinsicParasiticHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Str1Group.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class IntrinsicParasiticHandler IntrinsicParasiticHandler.h "IntrinsicParasiticHandler.h"
/// @brief 'intrinsic_parasitic' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダ名は名前を表す．
/// * Simple Attributes
///   - when : "Boolean expression" ;
/// * Group Statements
///   - intrinsic_capacitance ( <pg_pin_name> ) { ... }
///   - intrinsic_resistance ( <pg_pin_name> ) { ... }
///   - total_capacitance ( <pg_pin_name> ) { ... }
//////////////////////////////////////////////////////////////////////
class IntrinsicParasiticHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  IntrinsicParasiticHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~IntrinsicParasiticHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif intrinsic_parasitic の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstIntrinsicParasitic*>& dst_list);


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

  // intrinsic_capacitance
  vector<const AstIntrinsicCapacitance*> mCapList;

  // intrinsic_resistance
  vector<const AstIntrinsicResistance*> mResiList;

  // total_capacitance
  vector<const AstIntrinsicCapacitance*> mCapList;

};

END_NAMESPACE_YM_DOTLIB

#endif // INTRINSICPARASITICHANDLER_H
