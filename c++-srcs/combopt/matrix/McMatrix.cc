
/// @file McMatrix.cc
/// @brief McMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "mincov/McMatrix.h"
#include "mincov/McColComp.h"
#include "ym/Range.h"

//#define VERIFY_MCMATRIX 1


BEGIN_NAMESPACE_YM_MINCOV

int mcmatrix_debug = 0;

//////////////////////////////////////////////////////////////////////
// クラス McColComp
//////////////////////////////////////////////////////////////////////

// @brief col1 の代わりに col2 を使っても全体のコストが上がらない時に true を返す．
bool
McColComp::operator()(
  SizeType col1,
  SizeType col2
) const
{
  // デフォルトでは常に true を返す．
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス McMatrix
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
McMatrix::McMatrix(
  const MinCov& mincov
) : mRowSize{mincov.row_size()},
    mRowHeadArray(mRowSize),
    mRowArray(mRowSize, nullptr),
    mColSize{mincov.col_size()},
    mColHeadArray(mColSize),
    mColArray(mColSize, nullptr),
    mCostArray(mColSize, 1)
{
  // 行の初期化
  for ( auto row_pos: Range(mRowSize) ) {
    mRowHeadArray[row_pos].init(row_pos, false);
    mRowArray[row_pos] = alloc_cell(row_pos, -1);
  }

  // 列の初期化
  auto& cost_dict = mincov.col_cost_dict();
  for ( auto col_pos: Range(mColSize) ) {
    mColHeadArray[col_pos].init(col_pos, true);
    mColArray[col_pos] = alloc_cell(-1, col_pos);
    if ( cost_dict.count(col_pos) > 0 ) {
      mCostArray[col_pos] = cost_dict.at(col_pos);
    }
  }

  // スタックの容量を設定しておく．
  mDelStack.reserve(mRowSize + mColSize);

  // 挿入を効率良く行うため要素を整列させる．
  auto tmp_list = mincov.elem_list();
  std::sort(tmp_list.begin(), tmp_list.end(),
	    [](const ElemType& a, const ElemType& b) {
	      if ( a.row_pos < b.row_pos ) {
		return true;
	      }
	      if ( a.row_pos > b.row_pos ) {
		return false;
	      }
	      return a.col_pos < b.col_pos;
	    });
  // 要素を設定する．
  for ( auto& elem: tmp_list ) {
    insert_elem(elem);
  }
}

// @brief デストラクタ
McMatrix::~McMatrix()
{
}

// @brief 必須列の列番号のリストを返す．
std::vector<SizeType>
McMatrix::essential_cols() const
{
  std::vector<SizeType> col_list;
  std::vector<bool> mark(mColSize, false);
  for ( auto row_pos1: row_head_list() ) {
    if ( row_elem_num(row_pos1) == 1 ) {
      auto col_pos = row_list(row_pos1).front();
      if ( !mark[col_pos] ) {
	mark[col_pos] = true;
	col_list.push_back(col_pos);
      }
    }
  }
  return col_list;
}

// @brief 要素を追加する．
void
McMatrix::insert_elem(
  const ElemType& elem
)
{
  auto row_pos = elem.row_pos;
  auto col_pos = elem.col_pos;

  // 行方向の挿入位置を求める．
  auto row_head = &mRowHeadArray[row_pos];
  auto row_dummy = mRowArray[row_pos];
  McCell* pcell;
  McCell* ncell;
  if ( row_head->num() == 0 || row_dummy->row_prev()->col_pos() < col_pos ) {
    // 末尾への追加
    ncell = row_dummy;
    pcell = ncell->mLeftLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->col_pos() >= col_pos が成り立っている．
    for ( pcell = row_dummy; ; pcell = ncell ) {
      ncell = pcell->mRightLink;
      if ( ncell->col_pos() == col_pos ) {
	// 列番号が重複しているので無視する．
	return;
      }
      if ( ncell->col_pos() > col_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      if ( ncell == row_dummy ) {
	throw std::logic_error{"something goes wrong"};
      }
    }
  }

  auto cell = alloc_cell(row_pos, col_pos);
  cell->mLeftLink = pcell;
  pcell->mRightLink = cell;
  cell->mRightLink = ncell;
  ncell->mLeftLink = cell;
  row_head->inc_num();
  if ( row_head->num() == 1 ) {
    mRowHeadList.insert(row_head);
  }

  // 列方向の挿入位置を求める．
  auto col_head = &mColHeadArray[col_pos];
  auto col_dummy = mColArray[col_pos];
  if ( col_head->num() == 0 || col_dummy->col_prev()->row_pos() < row_pos ) {
    // 末尾への追加
    ncell = col_dummy;
    pcell = ncell->mUpLink;
  }
  else {
    // 追加位置を探索
    // この時点で back->row_pos() >= row_pos が成り立っている．
    for ( pcell = col_dummy; ; pcell = ncell ) {
      ncell = pcell->mDownLink;
      if ( ncell->row_pos() == row_pos ) {
	// 列番号が重複しているので無視する．
	return;
      }
      if ( ncell->row_pos() > row_pos ) {
	// pcell と ncell の間に cell を挿入する．
	break;
      }
      if ( ncell == col_dummy ) {
	throw std::logic_error{"something goes wrong"};
      }
    }
  }
  cell->mUpLink = pcell;
  pcell->mDownLink = cell;
  cell->mDownLink = ncell;
  ncell->mUpLink = cell;
  col_head->inc_num();
  if ( col_head->num() == 1 ) {
    mColHeadList.insert(col_head);
  }
}

// @brief 列 col_pos によって被覆される行を削除し，列も削除する．
void
McMatrix::select_col(
  SizeType col_pos
)
{
  for ( auto row_pos: col_list(col_pos) ) {
    delete_row(row_pos);
  }
  if ( col_elem_num(col_pos) > 0 ) {
    throw std::logic_error{"something goes wrong"};
  }
  delete_col(col_pos);
}

// @brief 行列を縮約する．
bool
McMatrix::reduce(
  std::vector<SizeType>& selected_cols,
  std::vector<SizeType>& deleted_cols,
  const McColComp& col_comp
)
{
  if ( mcmatrix_debug > 0 ) {
    std::cout << "McMatrix::reduce() start: "
	      << active_row_num() << " x " << active_col_num()
	      << std::endl;
    print(std::cout);
  }

  bool reduced = false;

  // 列支配を探し，列の削除を行う．
  if ( col_dominance(deleted_cols, col_comp) ) {
    reduced = true;
    if ( mcmatrix_debug > 0 ) {
      std::cout << " after col_dominance: "
		<< active_row_num() << " x " << active_col_num()
		<< std::endl;
      print(std::cout);
    }
  }

  // 必須列を探し，列の選択を行う．
  bool has_ec = false;
  for ( auto col_pos: essential_cols() ) {
    select_col(col_pos);
    selected_cols.push_back(col_pos);
    has_ec = true;
  }
  if ( has_ec ) {
    reduced = true;
    if ( mcmatrix_debug > 0 ) {
      std::cout << " after essential_col: "
		<< active_row_num() << " x " << active_col_num()
		<< std::endl;
      print(std::cout);
    }
  }

  // 行支配を探し，行の削除を行う．
  if ( row_dominance() ) {
    reduced = true;
  }

  return reduced;
}

// @brief 変化がなくなるまで reduce() を呼ぶ．
void
McMatrix::reduce_loop(
  std::vector<SizeType>& selected_cols,
  std::vector<SizeType>& deleted_cols,
  const McColComp& col_comp
)
{
  for ( ; ; ) {
    if ( !reduce(selected_cols, deleted_cols, col_comp) ) {
      break;
    }
  }
}

// @brief 変化がなくなるまで reduce() を呼ぶ．(deleted_cols がないバージョン)
void
McMatrix::reduce_loop(
  std::vector<SizeType>& selected_cols,
  const McColComp& col_comp
)
{
  std::vector<SizeType> dummy_cols;
  reduce_loop(selected_cols, dummy_cols, col_comp);
}

// @brief 行支配による縮約を行う．
bool
McMatrix::row_dominance()
{
  bool change = false;

  // 削除する行番号のマーク
  std::vector<bool> mark(mRowSize, false);
  // 削除する行番号のリスト
  std::vector<SizeType> del_list;
  del_list.reserve(active_row_num());
  for ( auto row_pos1: row_head_list() ) {
    if ( mark[row_pos1] ) {
      // すでに削除の印がついていたらスキップ
      continue;
    }

    // row_pos1 の dirty フラグ
    bool dirty1 = mRowHeadArray[row_pos1].is_dirty();

    // row1 の行に要素を持つ列で要素数が最小のものを求める．
    SizeType min_num = row_size() + 1;
    SizeType min_col = 0;
    for ( auto col_pos: row_list(row_pos1) ) {
      auto col_num = col_elem_num(col_pos);
      if ( min_num > col_num ) {
	min_num = col_num;
	min_col = col_pos;
      }
    }
    // min_col に要素を持つ行のうち row1 に支配されている行を求める．
    for ( auto row_pos2: col_list(min_col) ) {
      if ( row_pos2 == row_pos1 ) {
	// 自分自身は比較しない．
	continue;
      }

      if ( mark[row_pos2] ) {
	// 削除された行も比較しない.
	continue;
      }
      if ( row_elem_num(row_pos2) < row_elem_num(row_pos1) ) {
	// 要素数が少ない行も比較しない．
	continue;
      }
      // どちらかが dirty でなければチェックする必要はない．
      if ( !dirty1 && !mRowHeadArray[row_pos2].is_dirty() ) {
	continue;
      }

      // row1 に含まれる要素をすべて row2 が含んでいる場合
      // row1 が row2 を支配している．
      if ( check_containment(row_list(row_pos2), row_list(row_pos1)) ) {
	mark[row_pos2] = true;
	del_list.push_back(row_pos2);
	if ( mcmatrix_debug > 1 ) {
	  std::cout << "Row#" << row_pos2 << " is dominated by Row#"
		    << row_pos1
		    << std::endl;
	}
      }
    }
  }
  // 全ての行の dirty フラグを降ろす．
  for ( auto row_pos1: row_head_list() ) {
    mRowHeadArray[row_pos1].set_dirty(false);
  }

  // 実際に削除する．
  for ( auto row: del_list ) {
    delete_row(row);
  }

  return !del_list.empty();
}

// @brief 列支配による縮約を行う．
bool
McMatrix::col_dominance(
  std::vector<SizeType>& deleted_cols,
  const McColComp& col_comp
)
{
  // 削除する列番号のマーク
  std::vector<bool> mark(mColSize, false);
  // 削除する列番号のリスト
  std::vector<SizeType> del_list;
  del_list.reserve(active_col_num());
  for ( auto col_pos1: col_head_list() ) {
    if ( mark[col_pos1] ) {
      // 削除済みはスキップ
      continue;
    }

    if ( col_elem_num(col_pos1) == 0 ) {
      // 要素を持たない列は無条件で削除する．
      del_list.push_back(col_pos1);
      if ( mcmatrix_debug > 1 ) {
	std::cout << "Col#" << col_pos1 << " has no elements" << std::endl;
      }
      continue;
    }

    // col_pos1 の dirty フラグ
    bool dirty1 = mColHeadArray[col_pos1].is_dirty();

    // col1 の列に要素を持つ行で要素数が最小のものを求める．
    SizeType min_num = col_size() + 1;
    SizeType min_row = 0;
    for ( auto row_pos: col_list(col_pos1) ) {
      auto row_num = row_elem_num(row_pos);
      if ( min_num > row_num ) {
	min_num = row_num;
	min_row = row_pos;
      }
    }

    // min_row の行に要素を持つ列を対象にして支配関係のチェックを行う．
    for ( auto col_pos2: row_list(min_row) ) {
      if ( col_pos2 == col_pos1 ) {
	// 自分自身は比較しない．
	continue;
      }
      if ( mark[col_pos2] ) {
	// 削除済みならスキップ
	continue;
      }
      if ( col_elem_num(col_pos2) > col_elem_num(col_pos1) ) {
	// ただし col1 よりも要素数の多い列は調べる必要はない．
	continue;
      }
      // どちらかが dirty でなければチェックする必要はない．
      if ( !dirty1 && !mColHeadArray[col_pos2].is_dirty() ) {
	continue;
      }

      // col2 に含まれる要素を col1 がすべて含んでいる場合
      // col1 は col2 を支配している．
      if ( check_containment(col_list(col_pos1), col_list(col_pos2)) ) {
	if ( col_comp(col_pos2, col_pos1) ) {
	  // col2 を col1 を置き換えてコストが上がらない場合には col2 を削除できる．
	  mark[col_pos2] = true;
	  del_list.push_back(col_pos2);
	  if ( mcmatrix_debug > 1 ) {
	    std::cout << "Col#" << col_pos2 << " is dominated by Col#"
		      << col_pos1
		      << std::endl;
	  }
	  break;
	}
      }
    }
  }
  // すべての列の dirty フラグを降ろしておく．p
  for ( auto col_pos1: col_head_list() ) {
    mColHeadArray[col_pos1].set_dirty(false);
  }

  // 実際に削除する．
  deleted_cols.reserve(deleted_cols.size() + del_list.size());
  for ( auto col: del_list ) {
    delete_col(col);
    deleted_cols.push_back(col);
  }

  return !del_list.empty();
}

// @brief 行を削除する．
void
McMatrix::delete_row(
  SizeType row_pos
)
{
  // ヘッダを削除する．
  auto row_head = &mRowHeadArray[row_pos];
  mRowHeadList.exclude(row_head);
  push(row_head);

  auto dummy = mRowArray[row_pos];
  for ( auto cell = dummy->row_next();
	cell != dummy; cell = cell->row_next() ) {
    // cell を列方向のリンクから切り離す．
    auto prev = cell->col_prev();
    auto next = cell->col_next();
    if ( prev->col_next() != cell ||
	 next->col_prev() != cell ) {
      throw std::logic_error{"something goes wrong"};
    }
    prev->mDownLink = next;
    next->mUpLink = prev;
    // cell の列の要素数を1つ減らす．
    mColHeadArray[cell->col_pos()].dec_num();
  }
}

// @brief 行を復元する．
void
McMatrix::restore_row(
  McHead* row_head
)
{
  // ヘッダを復元する．
  mRowHeadList.restore(row_head);

  // 行の要素を復元する．
  auto row_pos = row_head->pos();
  auto dummy = mRowArray[row_pos];
  for ( auto cell = dummy->row_next();
	cell != dummy; cell = cell->row_next() ) {
    // cell を列方向のリンクに戻す．
    auto prev = cell->col_prev();
    auto next = cell->col_next();
    if ( prev->col_next() != next ||
	 next->col_prev() != prev ) {
      throw std::logic_error{"something goes wrong"};
    }
    prev->mDownLink = cell;
    next->mUpLink = cell;
    // cell の列の要素数を1つ増やす．
    mColHeadArray[cell->col_pos()].inc_num();
  }
}

// @brief 列を削除する．
void
McMatrix::delete_col(
  SizeType col_pos
)
{
  // ヘッダを削除する．
  auto col_head = &mColHeadArray[col_pos];
  mColHeadList.exclude(col_head);
  push(col_head);

  auto dummy = mColArray[col_pos];
  for ( auto cell = dummy->col_next();
	cell != dummy;
	cell = cell->col_next() ) {
    // cell を行方向のリンクから切り離す．
    auto prev = cell->row_prev();
    auto next = cell->row_next();
    if ( prev->row_next() != cell ||
	 next->row_prev() != cell ) {
      throw std::logic_error{"something goes wrong"};
    }
    prev->mRightLink = next;
    next->mLeftLink = prev;
    // cell の行の要素数を1つ減らす．
    mRowHeadArray[cell->row_pos()].dec_num();
  }
}

// @brief 列を復元する．
void
McMatrix::restore_col(
  McHead* col_head
)
{
  // ヘッダを復元する．
  mColHeadList.restore(col_head);

  // この列の要素を復元する．
  auto col_pos = col_head->pos();
  auto dummy = mColArray[col_pos];
  for ( auto cell = dummy->col_next();
	cell != dummy;
	cell = cell->col_next() ) {
    // cell を行方向のリンクに戻す．
    auto prev = cell->row_prev();
    auto next = cell->row_next();
    if ( prev->row_next() != next ||
	 next->row_prev() != prev ) {
      throw std::logic_error{"something goes wrong"};
    }
    prev->mRightLink = cell;
    next->mLeftLink = cell;
    // cell の行の要素数を1つ増やす．
    mRowHeadArray[cell->row_pos()].inc_num();
  }
}

// @brief 削除スタックにマーカーを書き込む．
void
McMatrix::save()
{
  push(nullptr);
}

// @brief 直前のマーカーまで処理を戻す．
void
McMatrix::restore()
{
  while ( !stack_empty() ) {
    auto head = pop();
    if ( head == nullptr ) {
      break;
    }
    if ( head->is_row() ) {
      restore_row(head);
    }
    else {
      restore_col(head);
    }
  }
}

// @brief 内容をコピーする．
void
McMatrix::copy(
  const McMatrix& src
)
{
  if ( row_size() != src.row_size() ||
       col_size() != src.col_size() ) {
    throw std::invalid_argument{"size mismatch"};
  }

  for ( auto row_pos: Range_<SizeType>(row_size()) ) {
    for ( auto col_pos: row_list(row_pos) ) {
      insert_elem(ElemType{row_pos, col_pos});
    }
  }

  for ( auto col_pos: Range(mColSize) ) {
    mCostArray[col_pos] = src.mCostArray[col_pos];
  }
}

// @brief 列集合のコストを返す．
SizeType
McMatrix::cost(
  const std::vector<SizeType>& col_list
) const
{
  SizeType cur_cost = 0;
  for ( auto col: col_list ) {
    cur_cost += col_cost(col);
  }
  return cur_cost;
}

// @brief 列集合がカバーになっているか検証する．
bool
McMatrix::verify(
  const std::vector<SizeType>& colpos_list
) const
{
  // カバーされた行の印
  auto row_mark = std::vector<bool>(row_size(), false);

  // col_list の列でカバーされた行に印をつける．
  for ( auto col_pos: colpos_list ) {
    for ( auto row_pos: col_list(col_pos) ) {
      row_mark[row_pos] = true;
    }
  }

  // 印の付いていない行があったらエラー
  for ( auto row_pos: Range(row_size()) ) {
    if ( !row_mark[row_pos] ) {
      return false;
    }
  }
  return true;
}

// @brief セルの生成
McCell*
McMatrix::alloc_cell(
  SizeType row_pos,
  SizeType col_pos
)
{
  auto cell = new McCell(row_pos,  col_pos);
  mCellList.push_back(std::unique_ptr<McCell>{cell});
  return cell;
}

// @brief 内容を出力する．
void
McMatrix::print(
  std::ostream& s
) const
{
  s << "Cols:";
  for ( auto col_pos: Range(col_size()) ) {
    if ( col_deleted(col_pos) ) {
      continue;
    }
    s << " #" << col_pos;
    if ( col_cost(col_pos) != 1 ) {
      s << ": " << col_cost(col_pos);
    }
  }
  s << std::endl;
  s << "Rows:";
  for ( auto row_pos: Range(row_size()) ) {
    if ( row_deleted(row_pos) ) {
      continue;
    }
    s << " #" << row_pos << ":";
  }
  s << std::endl;
  for ( auto row_pos: row_head_list() ) {
    s << "Row#" << row_pos << ": ";
    for ( auto col_pos: row_list(row_pos) ) {
      s << " " << col_pos;
    }
    s << std::endl;
  }
}

END_NAMESPACE_YM_MINCOV
