#ifndef INTERNALPOWERHANDLER_H
#define INTERNALPOWERHANDLER_H

/// @file InternalPowerHandler.h
/// @brief InternalPowerHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class InternalPowerHandler InternalPowerHandler.h "InternalPowerHandler.h"
/// @brief internal power グループ用のハンドラ
///
/// '(' ')' '{'
///    equal_or_opposite_output :
///    falling_together_group :
///    power_level :
///    related_pin :
///    rising_together_group :
///    switching_interval :
///    switching_together_group :
///    when :
///    power
///    fall_power
///    rise_power
///    domain
/// '}'
//////////////////////////////////////////////////////////////////////
class InternalPowerHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  InternalPowerHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~InternalPowerHandler();


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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_DOTLIB

#endif // INTERNALPOWERHANDLER_H
