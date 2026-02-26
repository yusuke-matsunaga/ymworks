#ifndef EXLOCK_H
#define EXLOCK_H

/// @file ExLock.h
/// @brief ExLock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <mutex>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ExLock ExLock.h "ExLock.h"
/// @brief 複数スレッド間で排他的に処理を行うための機構の基底クラス
//////////////////////////////////////////////////////////////////////
class ExLock
{
public:

  /// @brief コンストラクタ
  ExLock() = default;

  /// @brief デストラクタ
  ~ExLock() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理を行う．
  void
  run(
    std::function<void()> op_func ///< [in] 処理の本体
  )
  {
    std::unique_lock lck{mMtx};
    op_func();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ロック用のミューテックス
  std::mutex mMtx;

};

END_NAMESPACE_YM

#endif // EXLOCK_H
