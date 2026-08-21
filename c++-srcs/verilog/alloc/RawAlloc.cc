
/// @file RawAlloc.cc
/// @brief RawAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "RawAlloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス RawAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RawAlloc::RawAlloc()
{
}

// デストラクタ
RawAlloc::~RawAlloc()
{
}

// @brief n バイトの領域を確保する．
void*
RawAlloc::_get_memory(
  SizeType n
)
{
  auto p = new char[n];
  return static_cast<void*>(p);
}

// @brief n バイトの領域を開放する．
void
RawAlloc::_put_memory(
  SizeType n,
  void* block
)
{
  // このクラスでは領域の再利用はしない．
  return;
}

// @brief 今までに確保した全ての領域を破棄する．
void
RawAlloc::_destroy()
{
}

END_NAMESPACE_YM_VERILOG
