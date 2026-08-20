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
#include "ym/MsgMgr.h"
#include "parser/PtArray.h"
#include "alloc/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtListCell PtList.h "parser/PtList.h"
/// @brief PtList のセルを表すクラス
//////////////////////////////////////////////////////////////////////
template <typename T>
struct PtListCell
{
  T mVal;            ///< 要素
  PtListCell* mLink; ///< リンクポインタ
};


//////////////////////////////////////////////////////////////////////
/// @class PtListIterator PList.h "parser/PtList.h"
/// @brief PtList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class PtListIterator
{
  using Cell = PtListCell<T>;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  PtListIterator() = default;

  /// @brief AstList が用いるコンストラクタ
  PtListIterator(
    Cell* cell
  ) : mCell{cell}
  {
  }

  /// @brief デストラクタ
  ~PtListIterator() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を取り出す演算子
  T
  operator*() const
  {
    if ( mCell == nullptr ) {
      throw std::logic_error{"illegal position"};
    }
    return mCell->mVal;
  }

  /// @brief 次の要素を指す．
  PtListIterator&
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
    const PtListIterator& right
  ) const
  {
    return mCell == right.mCell;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const PtListIterator& right
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
/// @class PtList PtList.h "parser/PtList.h"
/// @brief リスト構造のテンプレートクラス
///
/// 単純なリンクトリストの実装
///
/// T2 は T1 の継承クラスであると仮定している．
//////////////////////////////////////////////////////////////////////
template <typename T1, typename T2>
class PtList
{
public:

  using iterator = PtListIterator<T1>;
  using Cell = PtListCell<T1>;

public:

  /// @brief コンストラクタ
  PtList() = default;

  /// @brief std::vector<T1> からのコンストラクタ
  PtList(
    Alloc& alloc,
    const std::vector<T1>& vec
  )
  {
    for ( auto elem: vec ) {
      push_back(alloc, elem);
    }
  }

  /// @brief std::vector<T2> からのコンストラクタ
  PtList(
    Alloc& alloc,
    const std::vector<T2>& vec
  )
  {
    for ( auto elem: vec ) {
      push_back(alloc, elem);
    }
  }

  /// @brief デストラクタ
  ~PtList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を読み出す関数
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
  iterator
  begin() const
  {
    return iterator(mTop);
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return iterator(nullptr);
  }

  /// @brief 先頭の要素を返す．
  T1
  front() const
  {
    if ( mTop == nullptr ) {
      throw std::logic_error{"list is empty"};
    }
    return mTop->mVal;
  }

  /// @brief 内容を PtArray<T1> に変換する．
  ///
  /// この処理の後ではリストは空になる．
  PtArray<T1, T2>
  to_array(
    Alloc& alloc ///< [in] アロケータ
  )
  {
    PtArray<T1, T2> vec(alloc, mSize);
    SizeType pos = 0;
    for ( auto elem: *this ) {
      vec.set(pos, elem);
      ++ pos;
    }
    clear();
    return vec;
  }

  /// @brief std::vector<T1> に変換する．
  std::vector<T1>
  to_vector() const
  {
    std::vector<T1> vec;
    vec.reserve(mSize);
    for ( auto elem: *this ) {
      vec.push_back(elem);
    }
    return vec;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief リストのクリア
  void
  clear()
  {
    mTop = nullptr;
    mEnd = nullptr;
    mSize = 0;
  }

  /// @brief 要素を先頭に追加
  void
  push_front(
    Alloc& alloc, ///< [in] アロケータ
    T1 elem       ///< [in] 追加する要素
  )
  {
    auto cell = new_cell(alloc, elem, mTop);
    mTop = cell;
    if ( mEnd == nullptr ) {
      mEnd = cell;
    }
    ++ mSize;
  }

  /// @brief 要素を末尾に追加
  void
  push_back(
    Alloc& alloc, ///< [in] アロケータ
    T1 elem       ///< [in] 追加する要素
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
    ++ mSize;
  }

  /// @brief 要素を末尾に追加
  void
  push_back(
    Alloc& alloc, ///< [in] アロケータ
    T2 elem       ///< [in] 追加する要素
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
    ++ mSize;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを確保する．
  Cell*
  new_cell(
    Alloc& alloc, ///< [in] アロケータ
    T1 ptr,       ///< [in] 本体の値
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

  // 要素数
  SizeType mSize{0};

  // 先頭の要素
  Cell* mTop{nullptr};

  // 末尾の要素
  Cell* mEnd{nullptr};

};

class PtAttrInst;
class PtAttrSpec;
class PtCaseItem;
class PtConnection;
class PtDeclHead;
class PtExpr;
class PtGenCaseItem;
class PtNameBranch;
class PtPort;
class PtRange;
class PtStmt;

using PtAttrInstList    = PtList<const AstAttrInst*,    PtAttrInst*>;
using PtAttrSpecList    = PtList<const AstAttrSpec*,    PtAttrSpec*>;
using PtCaseItemList    = PtList<const AstCaseItem*,    PtCaseItem*>;
using PtConnectionList  = PtList<const AstConnection*,  PtConnection*>;
using PtDeclHeadList    = PtList<const AstDeclHead*,    PtDeclHead*>;
using PtExprList        = PtList<const AstExpr*,        PtExpr*>;
using PtGenCaseItemList = PtList<const AstGenCaseItem*, PtGenCaseItem*>;
using PtIOHeadList      = PtList<const AstIOHead*,      PtIOHead*>;
using PtNameBranchList  = PtList<const AstNameBranch*,  PtNameBranch*>;
using PtPortList        = PtList<const AstPort*,        PtPort*>;
using PtRangeList       = PtList<const AstRange*,       PtRange*>;
using PtStmtList        = PtList<const AstStmt*,        PtStmt*>;

END_NAMESPACE_YM_VERILOG

#endif // PTLIST_H
