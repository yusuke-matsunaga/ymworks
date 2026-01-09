#ifndef YM_CLIBCELLELEM_H
#define YM_CLIBCELLELEM_H

/// @file ym/ClibCellElem.h
/// @brief ClibCellELem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellPtr.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibCellElem ClibCellElem.h "ym/ClibCellElem.h"
/// @brief セルの要素を表すクラス
///
/// 参照回数の管理のために ClibCellPtr を持つ．
//////////////////////////////////////////////////////////////////////
template<class T>
class ClibCellElem :
  private ClibCellPtr
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正値となる．
  ClibCellElem() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibCellElem(
    const CiCell* cell, ///< [in] 親のセル
    const T* impl       ///< [in] 本体
  ) : ClibCellPtr{cell},
      mImpl{impl}
  {
  }

  /// @brief デストラクタ
  ~ClibCellElem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return ClibCellPtr::is_valid() && mImpl != nullptr;
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
    const ClibCellElem& right
  ) const
  {
    // mImpl が等しいときは ClibCellPtr も等しいはず
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    const ClibCellElem& right
  ) const
  {
    return !operator==(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // メンバにアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを取り出す．
  const CiCell*
  _cell() const
  {
    return ClibCellPtr::_impl();
  }

  /// @brief 本体を取り出す．
  const T*
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const T* mImpl{nullptr};

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCELLELEM_H
