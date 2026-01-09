#ifndef PATHANDLE_H
#define PATHANDLE_H

/// @file PatHandle.h
/// @brief PatHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

class PatNode;

//////////////////////////////////////////////////////////////////////
/// @class PatHandle PatHandle.h "PatHandle.h"
/// @brief PatNode と極性を表すクラス
//////////////////////////////////////////////////////////////////////
class PatHandle
{
public:

  /// @brief コンストラクタ
  explicit
  PatHandle(
    PatNode* node = nullptr, ///< [in] ノード
    bool inv = false         ///< [in] 反転属性
  )
  {
    set(node, inv);
  }

  /// @brief デストラクタ
  ~PatHandle() = default;


public:

  /// @brief ノードを取り出す．
  const PatNode&
  node() const
  {
    return *reinterpret_cast<PatNode*>(mData & ~1UL);
  }

  /// @brief 反転属性を取り出す．
  bool
  inv() const
  {
    return static_cast<bool>(mData & 1UL);
  }

  /// @brief 値を設定する．
  void
  set(
    PatNode* node, ///< [in] ノード
    bool inv         ///< [in] 反転属性
  )
  {
    // bool に対する & 1UL は不必要だが念のため．
    mData = reinterpret_cast<PtrIntType>(node) | (static_cast<PtrIntType>(inv) & 1UL);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタと反転属性を合わせたもの
  PtrIntType mData;

};

END_NAMESPACE_YM_CLIB

#endif // PATHANDLE_H
