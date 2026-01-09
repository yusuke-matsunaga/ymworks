#ifndef YM_CLIBCELLPTR_H
#define YM_CLIBCELLPTR_H

/// @file ym/ClibCellPtr.h
/// @brief ClibCellPtr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga (松永 裕介)
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCell;

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibCellPtr ClibCellPtr.h "ym/ClibCellPtr.h"
/// @brief CiCell のスマートポインタ
//////////////////////////////////////////////////////////////////////
class ClibCellPtr
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正値となる．
  ClibCellPtr() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibCellPtr(
    const CiCell* impl ///< [in] 本体
  );

  /// @brief コピーコンストラクタ
  ClibCellPtr(
    const ClibCellPtr& src ///< [in] コピー元
  );

  /// @brief デストラクタ
  ~ClibCellPtr();


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
    const ClibCellPtr& right
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    const ClibCellPtr& right
  ) const
  {
    return !operator==(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // メンバに直接アクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体を取り出す．
  const CiCell*
  _impl() const
  {
    return mImpl;
  }

  /// @brief 適正な値を持っているかチェックする．
  void
  _check_valid() const
  {
    if ( !is_valid() ) {
      throw std::invalid_argument{"not having a valid data"};
    }
  }

  /// @brief ハッシュ用のキー
  SizeType
  key() const
  {
    return reinterpret_cast<SizeType>(mImpl);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装を表すオブジェクトへのポインタ
  const CiCell* mImpl{nullptr};

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCELLPTR_H
