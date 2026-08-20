#ifndef YM_VL_ASTLIST_H
#define YM_VL_ASTLIST_H

/// @file AstList.h
/// @brief AstList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#error "obsolete"

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstListCell AstListCell.h "ym/vl/AstList.h"
/// @brief AstList のセルを表すクラス
//////////////////////////////////////////////////////////////////////
template <typename T>
struct AstListCell
{
  T* mPtr; ///< 要素を表すポインタ
  AstListCell* mLink; ///< リンクポインタ
};


//////////////////////////////////////////////////////////////////////
/// @class AstListIterator AstListIterator.h "ym/vl/AstList.h"
/// @brief AstList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstListIterator
{
  using Cell = AstListCell<T>;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  AstListIterator() = default;

  /// @brief AstList が用いるコンストラクタ
  AstListIterator(
    Cell* cell
  ) : mCell{cell}
  {
  }

  /// @brief デストラクタ
  ~AstListIterator() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を取り出す演算子
  T*
  operator*() const
  {
    if ( mCell ) {
      return mCell->mPtr;
    }
    return nullptr;
  }

  /// @brief 次の要素を指す．
  AstListIterator&
  operator++()
  {
    if ( mCell ) {
      mCell = mCell->mLink;
    }
    return *this;
  };

  /// @brief 等価比較演算子
  bool
  operator==(
    const AstListIterator& right
  ) const
  {
    return mCell == right.mCell;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AstListIterator& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル
  Cell* mCell{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class AstList AstList.h "ym/vl/AstList.h"
/// @brief Ast系のリストを表すクラス
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstList
{
public:

  using const_iterator = AstListIterator<T>;
  using Cell = AstListCell<T>;

public:

  /// @brief コンストラクタ
  AstList() = default;

  /// @brief デストラクタ
  ~AstList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数の取得
  SizeType
  size() const
  {
    return mSize;
  }

  /// @brief 空の時に true を返す．
  bool
  empty() const
  {
    return mSize == 0;
  }

  /// @brief 先頭の反復子を返す．
  const_iterator
  begin() const
  {
    return AstListIterator<T>(mTop);
  }

  /// @brief 末尾の反復子を返す．
  const_iterator
  end() const
  {
    return AstListIterator<T>(nullptr);
  }

  /// @brief 先頭の要素を返す．
  T*
  front() const
  {
    if ( mTop) {
      return mTop->mPtr;
    }
    return nullptr;
  }

  /// @brief ベクタに変換する．
  std::vector<T*>
  to_vector() const
  {
    std::vector<T*> vec;
    vec.reserve(size());
    for ( auto elem: *this ) {
      vec.push_back(elem);
    }
    return vec;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  Cell* mTop{nullptr};

  // 末尾の要素
  Cell* mEnd{nullptr};

  // 要素数
  SizeType mSize{0};

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTLIST_H
