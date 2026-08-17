#ifndef PTLIST_H
#define PTLIST_H

/// @file PtList.h
/// @brief PtList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/Ast.h"
#include "parser/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

template <typename T1,
	  typename T2>
class PtList;

//////////////////////////////////////////////////////////////////////
/// @class PtListCell PtList.h "PtList.h"
/// @brief PtList 用のセル
//////////////////////////////////////////////////////////////////////
template <typename T>
struct PtListCell
{
  // 本体を指すポインタ
  T* mPtr;

  // リンクポインタ
  PtListCell* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class PtListIterator PtList.h "PtList.h"
/// @brief PtList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtListIterator
{
  using Cell = PtListCell<T>;

public:

  /// @brief コンストラクタ
  PtListIterator() = default;

  /// @brief コピーコンストラクタ
  PtListIterator(
    const PtListIterator& src ///< [in] ソース
  ) = default;

  /// @brief PtList が用いるコンストラクタ
  explicit
  PtListIterator(
    Cell* cell ///< [in] 要素
  ) : mCell{cell}
  {
  }

  /// @brief デストラクタ
  ~PtListIterator() = default;


public:

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
  PtListIterator&
  operator++()
  {
    if ( mCell ) {
      mCell = mCell->mLink;
    }
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const PtListIterator& right ///< [in] 相手のオペランド
  ) const
  {
    return mCell == right.mCell;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const PtListIterator<T>& right
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
/// @class PtList PtList.h "PtList.h"
/// @brief リスト構造のテンプレートクラス
/// @note T2 は T1 の親クラス
///
/// 単純なリンクトリストの実装
//////////////////////////////////////////////////////////////////////
template <typename T1,
	  typename T2 = T1>
class PtList
{
public:

  using const_iterator = PtListIterator<T1>;
  using Cell = PtListCell<T1>;

public:

  /// @brief コンストラクタ
  PtList() = default;

  /// @brief デストラクタ
  ~PtList() = default;


public:

  /// @brief リストのクリア
  void
  clear()
  {
    mTop = nullptr;
    mEnd = nullptr;
    mNum = 0;
  }

  /// @brief 要素を先頭に追加
  void
  push_front(
    Alloc& alloc, ///< [in] アロケータ
    T1* elem      ///< [in] 追加する要素
  )
  {
    auto cell = new_cell(alloc, elem, mTop);
    mTop = cell;
    if ( mEnd == nullptr ) {
      mEnd = cell;
    }
    ++ mNum;
  }

  /// @brief 要素を末尾に追加
  void
  push_back(
    Alloc& alloc, ///< [in] アロケータ
    T1* elem      ///< [in] 追加する要素
  )
  {
    auto cell = new_cell(alloc, elem, nullptr);
    if ( mEnd == nullptr ) {
      mTop = cell;
    }
    else {
      mEnd->mLink = cell;
    }
    mEnd = cell;
    ++ mNum;
  }

  /// @brief 要素数の取得
  /// @return 要素数
  SizeType
  size() const
  {
    return mNum;
  }

  /// @brief 空の時に true を返す．
  bool
  empty() const
  {
    return mNum == 0;
  }

  /// @brief 先頭の反復子を返す．
  const_iterator
  begin() const
  {
    return PtListIterator<T1>(mTop);
  }

  /// @brief 末尾の反復子を返す．
  const_iterator
  end() const
  {
    return PtListIterator<T1>(nullptr);
  }

  /// @brief 先頭の要素を返す．
  T1*
  front() const
  {
    if ( mTop ) {
      return mTop->mPtr;
    }
    return nullptr;
  }

  /// @brief 末尾の要素を返す．
  T1*
  back() const
  {
    if ( mEnd ) {
      return mEnd->mPtr;
    }
    return nullptr;
  }


public:
  //////////////////////////////////////////////////////////////////////
  /// 特別な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を PtArray<> にコピーする．
  ///
  /// この処理の後ではリストは空になる．
  PtArray<T2>
  to_array(
    Alloc& alloc ///< [in] アロケータ
  )
  {
    PtArray<T2> vec(alloc, mNum);
    SizeType pos = 0;
    for ( auto elem: *this ) {
      vec.set(pos, elem);
      ++ pos;
    }
    clear();
    return vec;
  }

  /// @brief 内容を std::vector<T2> にコピーする．
  ///
  /// この処理の後ではリストは空になる．
  std::vector<T2*>
  to_vector()
  {
    std::vector<T2*> vec;
    vec.reserve(mNum);
    for ( auto elem: *this ) {
      vec.push_back(elem);
    }
    clear();
    return vec;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを確保する．
  Cell*
  new_cell(
    Alloc& alloc, ///< [in] アロケータ
    T1* ptr,      ///< [in] 本体の値
    Cell* link    ///< [in] 次の要素
  )
  {
    void* p = alloc.get_memory(sizeof(Cell));
    return new (p) Cell{ptr, link};
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  Cell* mTop{nullptr};

  // 末尾の要素
  Cell* mEnd{nullptr};

  // 要素数
  SizeType mNum{0};

};

class PtAttrInst;
class PtAttrSpec;
class PtCaseItem;
class PtConnection;
class PtExpr;
class PtGenCaseItem;
class PtNameBranch;
class PtRange;
class PtStmt;

using PtAttrInstList    = PtList<const PtAttrInst, const AstAttrInst>;
using PtAttrSpecList    = PtList<const PtAttrSpec, const AstAttrSpec>;
using PtCaseItemList    = PtList<const PtCaseItem, const AstCaseItem>;
using PtConnectionList  = PtList<const PtConnection, const AstConnection>;
using PtExprList        = PtList<const PtExpr, const AstExpr>;
using PtGenCaseItemList = PtList<const PtGenCaseItem, const AstGenCaseItem>;
using PtNameBranchList  = PtList<const AstNameBranch>;
using PtRangeList       = PtList<const PtRange, const AstRange>;
using PtStmtList        = PtList<const PtStmt, const AstStmt>;

END_NAMESPACE_YM_VERILOG

#endif // PTLIST_H
