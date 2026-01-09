#ifndef YM_SATMSGHANDLERS_H
#define YM_SATMSGHANDLERS_H

/// @file ym/SatMsgHandlerS.h
/// @brief SatMsgHandlerS のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SatMsgHandler.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatMsgHandlerS SatMsgHandlerS.h "ym/SatMsgHandlerS.h"
/// @brief SatMsgHandler の実装例
//////////////////////////////////////////////////////////////////////
class SatMsgHandlerS :
  public SatMsgHandler
{
public:

  /// @brief コンストラクタ
  SatMsgHandlerS(
    std::ostream& s ///< [in] 出力先のストリーム
  );

  /// @brief デストラクタ
  ~SatMsgHandlerS();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダの出力
  void
  print_header() override;

  /// @brief メッセージ出力関数
  void
  print_message(
    const SatStats& stats ///< [in] SAT ソルバの内部状態
  ) override;

  /// @brief フッタの出力
  void
  print_footer(
    const SatStats& stats ///< [in] SAT ソルバの内部状態
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  std::ostream& mS;

};

END_NAMESPACE_YM_SAT

#endif // YM_SATMSGHANDLERS_H
