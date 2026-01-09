#ifndef FPGACONDITIONHANDLER_H
#define FPGACONDITIONHANDLER_H

/// @file FpgaConditionHandler.h
/// @brief FpgaConditionHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class FpgaConditionHandler FpgaConditionHandler.h "FpgaConditionHandler.h"
/// @brief 'fpga_condition' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダ名は fpga_condition group 名を表す．
/// * Group Statements
///   - fpga_condition_value ( name ) { ... }
//////////////////////////////////////////////////////////////////////
class FpgaConditionHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  FpgaConditionHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~FpgaConditionHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif fpga_condition Group Statement の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstFpgaCondition*>& dst_list);


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

  // fpga_condition_value
  vector<const AstFpgaConditionValue*> mFpgaConditionValueList;

};

END_NAMESPACE_YM_DOTLIB

#endif // FPGACONDITIONHANDLER_H
