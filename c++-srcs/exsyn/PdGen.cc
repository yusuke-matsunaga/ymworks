
/// @file PdGen.cc
/// @brief PdGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdGen.h"
#include "ym/CombiGen.h"
#include "ym/MultiCombiGen.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief Partial DAG を生成する．
std::vector<PD>
PdGen::operator()(
  const Fence& fence,
  SizeType operand_num
)
{
  mFence = fence;
  mOperandNum = operand_num;
  mPdList.clear();
  recur_sub(OprList(), 0);
  return mPdList;
}

BEGIN_NONAMESPACE

std::ostream&
operator<<(
  std::ostream& s,
  const PdGen::OprList& opr_list
)
{
  s << "(";
  const char* sep0 = "";
  for ( auto& opr: opr_list ) {
    s << sep0 << "(";
    sep0 = ", ";
    const char* sep1 = "";
    for ( auto x: opr ) {
      s << sep1 << x;
      sep1 = ", ";
    }
    s << ")";
  }
  s << ")";
  return s;
}

// オペランドの番号の比較を行う．
bool
check_le(
  const PdGen::Opr& op1,
  const PdGen::Opr& op2
)
{
  auto n1 = op1.size();
  auto n2 = op2.size();
  auto n = std::min(n1, n2);
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( op1[i] < op2[i] ) {
      return true;
    }
    if ( op1[i] > op2[i] ) {
      return false;
    }
  }
  if ( n1 > n ) {
    return false;
  }
  return true;
}

END_NONAMESPACE

// @brief 下請け関数
void
PdGen::recur_sub(
  const OprList& cur_pd,
  SizeType node_id
)
{
  if ( node_id == mFence.total_num() ) {
    if ( check_pd(cur_pd) ) {
      mPdList.push_back(PD(cur_pd));
    }
    return;
  }
  auto l = mFence.level(node_id);
  if ( l == 0 ) {
    // レベル0のノードのオペランドはすべて-1
    for ( SizeType ni = 2; ni <= mOperandNum; ++ ni ) {
      Opr opr(ni, -1);
      auto tmp_pd = cur_pd;
      tmp_pd.push_back(opr);
      recur_sub(tmp_pd, node_id + 1);
    }
    return;
  }
  auto top = mFence.top(l);
  auto top0 = mFence.top(l - 1);
  for ( SizeType ni = 2; ni <= mOperandNum; ++ ni ) {
    // -1 〜 top - 1 の中から ni 個の要素を取り出す．
    // ただし -1 は複数回選ぶことが可能
    for ( SizeType c = 0; c < ni; ++ c ) {
      if ( top < ni - c ) {
	continue;
      }
      Opr opr(ni);
      for ( SizeType i = 0; i < c; ++ i ) {
	opr[i] = -1;
      }
      for ( CombiGen cg(top, ni - c); !cg.is_end(); ++ cg ) {
	for ( SizeType i = c; i < ni; ++ i ) {
	  opr[i] = cg(i - c);
	}
	if ( opr.back() < top0 ) {
	  continue;
	}
	auto tmp_pd = cur_pd;
	tmp_pd.push_back(opr);
	recur_sub(tmp_pd, node_id + 1);
      }
    }
  }
}

// 適切な Partial DAG が調べる．
bool
PdGen::check_pd(
  const PdGen::OprList& pd
) const
{
  if ( !check_ord(pd) ) {
    return false;
  }
  if ( !check_noreapplication(pd) ) {
    return false;
  }

  if ( use_all_steps() ) {
    if ( !check_all_use(pd) ) {
      return false;
    }
  }
  return true;
}

// オペランド間の順序制約を調べる．
bool
PdGen::check_ord(
  const OprList& pd
) const
{
  auto l = pd.size();
  for ( SizeType i = 1; i < l; ++ i ) {
    auto& op1 = pd[i - 1];
    auto& op2 = pd[i];
    if ( !check_le(op1, op2) ) {
      return false;
    }
  }
  return true;
}

// reapplication のチェック
bool
PdGen::check_noreapplication(
  const OprList& pd
) const
{
  auto l = pd.size();
  for ( SizeType i = 0; i < l - 1; ++ i ) {
    auto& op1 = pd[i];
    for ( SizeType j = i + 1; j < l; ++ j ) {
      auto& op2 = pd[j];
      if ( op2.size() > 2 ) {
	continue;
      }
      auto y1 = op2[0];
      auto y2 = op2[1];
      if ( y2 != i ) {
	continue;
      }
      for ( auto x: op1 ) {
	if ( x != -1 && x == y1 ) {
	  return false;
	}
      }
    }
  }
  return true;
}

// 全てのノードが他の演算のオペランドとして用いられているか調べる．
bool
PdGen::check_all_use(
  const OprList& pd
) const
{
  auto l = pd.size();
  std::vector<bool> marks(l, false);
  for ( SizeType i = 0; i < l; ++ i ) {
    auto& op = pd[i];
    for ( auto x: op ) {
      if ( x >= 0 ) {
	marks[x] = true;
      }
    }
  }
  // 最後のノードは出力として用いられる．
  marks[l - 1] = true;
  for ( auto m: marks ) {
    if ( !m ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_EXSYN
