#ifndef CLIBLIBRARYPTR_H
#define CLIBLIBRARYPTR_H

/// @file ClibLibraryPtr.h
/// @brief ClibLibraryPtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellLibrary;

//////////////////////////////////////////////////////////////////////
/// @class ClibLibraryPtr ClibLibraryPtr.h "ClibLibraryPtr.h"
/// @brief CiCellLibrary 専用の shared_ptr クラス
//////////////////////////////////////////////////////////////////////
class ClibLibraryPtr
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正値となる．
  ClibLibraryPtr() = default;

  /// @brief 値を指定したコンストラクタ
  ClibLibraryPtr(
    const CiCellLibrary* library ///< [in] ライブラリ
  );

  /// @brief コピーコンストラクタ
  ClibLibraryPtr(
    const ClibLibraryPtr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief 代入演算子
  ClibLibraryPtr&
  operator=(
    const ClibLibraryPtr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~ClibLibraryPtr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  const CiCellLibrary*
  operator->() const { return mPtr; }

  /// @brief ->() の別名
  const CiCellLibrary*
  ptr() const { return mPtr; }

  /// @brief 適正な値を持っている時 true を返す．
  bool
  is_valid() const { return mPtr != nullptr; }

  /// @brief 不正値の時 true を返す．
  bool
  is_invalid() const { return !is_valid(); }

  /// @brief is_valid() の別名
  operator bool() const { return is_valid(); }

  /// @brief 等価比較演算子
  bool
  operator==(
    const ClibLibraryPtr& right ///< [in] オペランド
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ClibLibraryPtr& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタ
  const CiCellLibrary* mPtr{nullptr};

};

END_NAMESPACE_YM_CLIB

#endif // CLIBLIBRARYPTR_H
