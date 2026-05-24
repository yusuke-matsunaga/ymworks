#ifndef INTERVALTIMER_H
#define INTERVALTIMER_H

/// @file IntervalTimer.h
/// @brief IntervalTimer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <chrono>
#include <functional>
#include <thread>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class IntervalTimer IntervalTimer.h "IntervalTimer.h"
/// @brief 時間経過で割り込み処理を行うクラス
///
/// 別スレッドで指定された時間後に割り込み関数を呼び出す．
/// 本スレッドがそれ以前に終了する場合は stop() 関数で終了を通知する．
/// stop() はこのオブジェクトのデストラクタで呼び出されるので
/// スコープを適切に設定しているなら stop() を明示的に呼び出す必要はない．
//////////////////////////////////////////////////////////////////////
class IntervalTimer
{
public:

  /// @brief コンストラクタ
  ///
  /// interval が 0 の時はなにもしない．
  IntervalTimer(
    SizeType interval,              ///< [in] 間隔(秒)
    std::function<void()> callback, ///< [in] コールバック関数
    bool periodic = false           ///< [in] 周期的なイベントの時 true にするフラグ
  ) : mInterval{interval}
  {
    if ( mInterval == 0 ) {
      return;
    }

    mAlive = true;
    if ( periodic ) {
      // 周期的なイベント
      mThread = std::thread([&] {
	for ( ; ; ) {
	  std::unique_lock lck{mMtx};
	  if ( !mAlive ) {
	    // 本スレッドが終了していたら終わる．
	    break;
	  }
	  // mInterval 秒待つ
	  mCV.wait_for(lck, std::chrono::seconds(mInterval));
	  if ( mAlive ) {
	    // コールバック関数を呼び出す．
	    callback();
	  }
	}
      });
    }
    else {
      // one-shot イベント
      mThread = std::thread([&] {
	std::unique_lock lck{mMtx};
	if ( !mAlive ) {
	  // 本スレッドが終了していたら終わる．
	  return;
	}
	// mInterval 秒待つ
	mCV.wait_for(lck, std::chrono::seconds(mInterval));
	if ( mAlive ) {
	  // コールバック関数を呼び出す．
	  callback();
	}
      });
    }
  }

  /// @brief デストラクタ
  ~IntervalTimer()
  {
    stop();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief タイマーを終了する．
  void
  stop()
  {
    if ( mThread.joinable() ) {
      if ( mAlive ) {
	{
	  std::lock_guard lck{mMtx};
	  mAlive = false;
	}
	mCV.notify_all();
      }
      mThread.join();
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インターバル(秒)
  SizeType mInterval;

  // 動作中を表すフラグ
  bool mAlive{false};

  // mAlive 用のミューテックス
  std::mutex mMtx;

  // 通知用の条件変数
  std::condition_variable mCV;

  // タイマー用のスレッド
  std::thread mThread;

};

END_NAMESPACE_YM

#endif // INTERVALTIMER_H
