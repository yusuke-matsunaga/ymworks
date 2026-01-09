
/// @file FraigMgr.cc
/// @brief FraigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/FraigMgr.h"
#include "FraigMgrImpl.h"


BEGIN_NAMESPACE_YM_FRAIG

//////////////////////////////////////////////////////////////////////
// クラス FraigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FraigMgr::FraigMgr(
  SizeType sig_size,
  const SatInitParam& init_param
) : mPtr{new FraigMgrImpl(sig_size, init_param)}
{
}

// @brief デストラクタ
FraigMgr::~FraigMgr()
{
}

// @brief 外部入力を作る．
FraigLiteral
FraigMgr::new_input()
{
  return mPtr->new_input();
}

// @brief 2つのノードの AND を作る．
FraigLiteral
FraigMgr::new_and(
  FraigLiteral edge1,
  FraigLiteral edge2
)
{
  return mPtr->new_and(edge1, edge2);
}

// @brief 複数ノードの AND を作る．
FraigLiteral
FraigMgr::new_and(
  const std::vector<FraigLiteral>& edge_list
)
{
  return _and_sub(edge_list, 0, edge_list.size(), false);
}

// @brief ２つのノードの OR を作る．
FraigLiteral
FraigMgr::new_or(
  FraigLiteral edge1,
  FraigLiteral edge2
)
{
  return ~new_and(~edge1, ~edge2);
}

// @brief 複数ノードの OR を作る．
FraigLiteral
FraigMgr::new_or(
  const std::vector<FraigLiteral>& edge_list
)
{
  return ~_and_sub(edge_list, 0, edge_list.size(), true);
}

// @brief ２つのノードの XOR を作る．
FraigLiteral
FraigMgr::new_xor(
  FraigLiteral edge1,
  FraigLiteral edge2
)
{
  auto e01 = new_and(~edge1,  edge2);
  auto e10 = new_and( edge1, ~edge2);
  return new_or(e01, e10);
}

// @brief 複数ノードの XOR を作る．
FraigLiteral
FraigMgr::new_xor(
  const std::vector<FraigLiteral>& edge_list
)
{
  return _xor_sub(edge_list, 0, edge_list.size());
}

// @brief ITE演算
FraigLiteral
FraigMgr::ite(
  FraigLiteral if_edge,
  FraigLiteral then_edge,
  FraigLiteral else_edge
)
{
  auto tmp0 = new_and(if_edge, then_edge);
  auto tmp1 = new_and(~if_edge, else_edge);
  auto ans = new_or(tmp0, tmp1);
  return ans;
}

// @brief コファクター演算
FraigLiteral
FraigMgr::cofactor(
  FraigLiteral edge,
  SizeType input_id,
  bool val
)
{
  return mPtr->cofactor(edge, input_id, val);
}

// @brief new_and() の下請け関数
FraigLiteral
FraigMgr::_and_sub(
  const std::vector<FraigLiteral>& edge_list,
  SizeType start_pos,
  SizeType end_pos,
  bool iinv
)
{
  ASSERT_COND( start_pos < end_pos );

  auto n = end_pos - start_pos;
  if ( n == 1 ) {
    auto h = edge_list[start_pos];
    if ( iinv ) {
      return ~h;
    }
    else {
      return h;
    }
  }
  // n >= 2
  auto mid_pos = start_pos + (n + 1) / 2;
  auto h0 = _and_sub(edge_list, start_pos, mid_pos, iinv);
  auto h1 = _and_sub(edge_list, mid_pos, end_pos, iinv);
  return new_and(h0, h1);
}

// @brief new_xor() の下請け関数
FraigLiteral
FraigMgr::_xor_sub(
  const std::vector<FraigLiteral>& edge_list,
  SizeType start_pos,
  SizeType end_pos
)
{
  ASSERT_COND( start_pos < end_pos );

  auto n = end_pos - start_pos;
  if ( n == 1 ) {
    auto h = edge_list[start_pos];
    return h;
  }
  // n >= 2
  auto mid_pos = start_pos + (n + 1) / 2;
  auto h0 = _xor_sub(edge_list, start_pos, mid_pos);
  auto h1 = _xor_sub(edge_list, mid_pos, end_pos);
  return new_xor(h0, h1);
}

// @brief 2つのハンドルが等価かどうか調べる．
SatBool3
FraigMgr::check_equiv(
  FraigLiteral aig1,
  FraigLiteral aig2
)
{
  return mPtr->check_equiv(aig1, aig2);
}

// @brief ログレベルを設定する．
void
FraigMgr::set_loglevel(
  SizeType level
)
{
  mPtr->set_loglevel(level);
}

// @brief ログ出力用ストリームを設定する．
void
FraigMgr::set_logstream(
  std::ostream* out
)
{
  mPtr->set_logstream(out);
}

// @brief 内部の統計情報を出力する．
void
FraigMgr::dump_stats(
  std::ostream& s
)
{
  mPtr->dump_stats(s);
}

END_NAMESPACE_YM_FRAIG
