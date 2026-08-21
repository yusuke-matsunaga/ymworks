
/// @file Alloc.cc
/// @brief Alloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.


#include "alloc/Alloc.h"
#include "SimpleAlloc.h"
#include "RawAlloc.h"

BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス Alloc
//////////////////////////////////////////////////////////////////////

// @brief 実際の継承クラスを生成するクラスメソッド
Alloc*
Alloc::new_obj(
  const std::string name
)
{
  if ( name == "simple" || name == "" ) {
    return new SimpleAlloc;
  }
  if ( name == "raw" ) {
    return new RawAlloc;
  }
  std::ostringstream buf;
  buf << "Alloc::new_obj(" << name << "): "
      << "invalid argument";
  throw std::invalid_argument{buf.str()};
}

// @brief n バイトの領域を確保する．
void*
Alloc::get_memory(
  SizeType n
)
{
  if ( n == 0 ) {
    return nullptr;
  }

  mUsedSize += n;
  if ( mMaxUsedSize < mUsedSize ) {
    mMaxUsedSize = mUsedSize;
  }

  return _get_memory(n);
}

// @brief n バイトの領域を開放する．
void
Alloc::put_memory(
  SizeType n,
  void* block
)
{
  mUsedSize -= n;

  _put_memory(n, block);
}

// @brief 今までに確保した全ての領域を破棄する．
void
Alloc::destroy()
{
  mUsedSize = 0;
  _destroy();
}

// @brief 内部状態を出力する．
void
Alloc::print_stats(
  std::ostream& s
) const
{
  s << "maximum used size: " << max_used_size() << std::endl
    << "current used size: " << used_size() << std::endl
    << "allocated size:    " << allocated_size() << std::endl
    << "allocated count:   " << allocated_count() << std::endl
    << std::endl;
}

// @brief 真のアロケート関数
void*
Alloc::alloc(
  SizeType n
)
{
  if ( mMemLimit > 0 && mAllocSize + n >= mMemLimit ) {
    // 総量が制限値を越えた．
    return 0;
  }
  mAllocSize += n;
  ++ mAllocCount;
  return ::operator new(n);
}

// @brief 新のフリー関数
void
Alloc::free(
  SizeType n,
  void* blk
)
{
  mAllocSize -= n;
  ::operator delete(blk);
}

END_NAMESPACE_YM_VERILOG
