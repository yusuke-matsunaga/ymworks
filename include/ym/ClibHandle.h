#ifndef CLIBHANDLE_H
#define CLIBHANDLE_H

/// @file ClibHandle.h
/// @brief ClibHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class ClibHandle ClibHandle.h "ClibHandle.h"
/// @brief Clib の要素を表す基底クラス
///
/// - 実体を現す実装クラスのスマートポインタ
/// - 実装クラスは inc_ref(), dec_ref() を実装しているものと仮定している．
//////////////////////////////////////////////////////////////////////
template<class T>
class ClibHandle
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正値となる．
  ClibHandle() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibHandle(
    const T* impl ///< [in] 実体
  ) : mImpl{impl}
  {
    if ( mImpl != nullptr ) {
      mImpl->inc_ref();
    }
  }

  /// @brief デストラクタ
  ~ClibHandle()
  {
    if ( mImpl != nullptr ) {
      mImpl->dec_ref();
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mImpl != nullptr;
  }

  /// @brief 不正値の時 true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 等価比較
  bool
  operator==(
    ClibHandle right
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    ClibHandle right
  )
  {
    return !operator==(right);
  }

  /// @brief 適正な値を持っているかチェックする．
  void
  _check_valid() const
  {
    if ( !is_valid() ) {
      throw std::invalid_argument{"not having a valid data"};
    }
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体
  const T* mImpl;

};

END_NAMESPACE_YM_CLIB

#endif // CLIBHANDLE_H
