#ifndef RAWALLOC_H
#define RAWALLOC_H

/// @file RawAlloc.h
/// @brief RawAlloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "alloc/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class RawAlloc RawAlloc.h "parser/RawAlloc.h"
/// @brief 単純なメモリ管理
//////////////////////////////////////////////////////////////////////
class RawAlloc :
  public Alloc
{
public:

  /// @brief コンストラクタ
  RawAlloc();

  /// @brief デストラクタ
  ~RawAlloc();


private:
  //////////////////////////////////////////////////////////////////////
  // AllocBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  void*
  _get_memory(
    SizeType n ///< [in] 確保するメモリ量(単位はバイト)
  ) override;

  /// @brief n バイトの領域を開放する．
  void
  _put_memory(
    SizeType n, ///< [in] 確保したメモリ量(単位はバイト)
    void* blk   ///< [in] 開放するメモリ領域の先頭番地
  ) override;

  /// @brief 今までに確保した全ての領域を破棄する．
  ///
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  void
  _destroy() override;

};

END_NAMESPACE_YM_VERILOG

#endif // RAWALLOC_H
