#ifndef AVERAGENUMBEROFFAULTSHANDLER_H
#define AVERAGENUMBEROFFAULTSHANDLER_H

/// @file AverageNumberOfFaultsHandler.h
/// @brief AverageNumberOfFaultsHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Str1GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AverageNumberOfFaultsHandlers AverageNumberOfFaultsHandlers.h "AverageNumberOfFaultsHandlers.h"
/// @brief 'average_number_of_faults' Group Statement 用のハンドラ
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダの文字列は faults_lut_template 名を表す．
/// * Complex Attributes
///   - values ( "float, ..., float" );
//////////////////////////////////////////////////////////////////////
class AverageNumberOfFaultsHandlers
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  AverageNumberOfFaultsHandlers(DotlibParser& parser);

  /// @brief デストラクタ
  ~AverageNumberOfFaultsHandlers();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif average_number_of_faults Group Statement の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstAnof*>& dst_list);


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

  // values
  const AstFloatVector* mValues;

};

END_NAMESPACE_YM_DOTLIB

#endif // AVERAGENUMBEROFFAULTSHANDLER_H
