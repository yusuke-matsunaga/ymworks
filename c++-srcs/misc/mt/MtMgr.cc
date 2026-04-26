
/// @file MtMgr.cc
/// @brief MtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MtMgr.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// スレッド数を補正する．
// - 具体的には 0 の時には可能な最大数にする．
inline
void
correct_thread_num(
  SizeType& thread_num
)
{
  if ( thread_num == 0 ) {
    thread_num = std::thread::hardware_concurrency();
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス MtMgr
//////////////////////////////////////////////////////////////////////

// @brief 均一なマルチスレッド実行を行う．
void
MtMgr::run(
  std::function<void()> th_func,
  SizeType thread_num
)
{
  // スレッド数を決める．
  correct_thread_num(thread_num);
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
  correct_thread_num(thread_num);
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
