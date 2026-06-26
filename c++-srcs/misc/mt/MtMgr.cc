
/// @file MtMgr.cc
/// @brief MtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MtMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MtMgr
//////////////////////////////////////////////////////////////////////

// @brief 実際のスレッド数を返す．
SizeType
MtMgr::actual_thread_num(
  SizeType thread_num
)
{
  if ( thread_num == 0 ) {
    return std::thread::hardware_concurrency();
  }
  return thread_num;
}

// @brief 均一なマルチスレッド実行を行う．
void
MtMgr::run(
  std::function<void()> th_func,
  SizeType thread_num
)
{
  // スレッド数を決める．
  thread_num = actual_thread_num(thread_num);
  // スレッドのリスト
  std::vector<std::thread> thr_list(thread_num);
  // スレッドを生成する．
  for ( SizeType i = 0; i < thread_num; ++ i ) {
    thr_list[i] = std::thread{th_func};
  }
  // 子スレッドが終了するまで待つ．
  for ( auto& thr: thr_list ) {
    thr.join();
  }
}

// @brief スレッド番号を引数にした関数のマルチスレッド実行を行う．
void
MtMgr::run(
  std::function<void(SizeType)> th_func,
  SizeType thread_num
)
{
  // スレッド数を決める．
  thread_num = actual_thread_num(thread_num);
  // スレッドのリスト
  std::vector<std::thread> thr_list(thread_num);
  // スレッドを生成する．
  for ( SizeType i = 0; i < thread_num; ++ i ) {
    thr_list[i] = std::thread{th_func, i};
  }
  // 子スレッドが終了するまで待つ．
  for ( auto& thr: thr_list ) {
    thr.join();
  }
}

END_NAMESPACE_YM
