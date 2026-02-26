#ifndef MTMGR_H
#define MTMGR_H

/// @file MtMgr.h
/// @brief MtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <thread>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MtMgr MtMgr.h "ym/MtMgr.h"
/// @brief マルチスレッド実行のためのヘルパークラス
///
/// 実際には複数の関数をひとまとめにする名前空間でしかない．
//////////////////////////////////////////////////////////////////////
class MtMgr
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 均一なマルチスレッド実行を行う．
  ///
  /// - 全てのスレッドの終了を待つ．
  /// - thread_num が 0 の時は可能な最大数を用いる．
  static
  void
  run(
    std::function<void()> th_func, ///< [in] 各スレッドが実行する関数
    SizeType thread_num = 0        ///< [in] スレッド数
  );

  /// @brief スレッド番号を引数にした関数のマルチスレッド実行を行う．
  ///
  /// - 全てのスレッドの終了を待つ．
  /// - thread_num が 0 の時は可能な最大数を用いる．
  static
  void
  run(
    std::function<void(SizeType)> th_func, ///< [in] 各スレッドが実行する関数
    SizeType thread_num = 0                ///< [in] スレッド数
  );

};

END_NAMESPACE_YM

#endif // MTMGR_H
