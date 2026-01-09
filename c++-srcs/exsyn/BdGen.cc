
/// @file BdGen.cc
/// @brief BdGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BdGen.h"
#include "ym/CombiGen.h"
#include "ym/MultiCombiGen.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief Bound DAG を生成する．
std::vector<BD>
BdGen::operator()()
{
  mBdList.clear();
  recur_sub(OprList(), 0);
  return mBdList;
}

BEGIN_NONAMESPACE

std::ostream&
operator<<(
  std::ostream& s,
  const BdGen::OprList& opr_list
)
{
  s << "(";
  const char* sep0 = "";
  for ( auto& opr: opr_list ) {
    s << sep0 << "(";
    sep0 = ", ";
    const char* sep1 = "";
    for ( auto x: opr ) {
      s << sep1;
      sep1 = ", ";
      if ( x >= 0 ) {
	s << "OP#" << x;
      }
      else {
	s << "I#" << (-x + 1);
      }
    }
    s << ")";
  }
  s << ")";
  return s;
}

END_NONAMESPACE

// @brief 下請け関数
void
BdGen::recur_sub(
  const OprList& cur_bd,
  SizeType node_id
)
{
  if ( node_id == mPD.op_num() ) {
    if ( check_bd(cur_bd) ) {
      mBdList.push_back(BD(mInputNum, cur_bd));
    }
    return;
  }
  auto& pd_opr = mPD.opr(node_id);
  // -1 の数を数える．
  SizeType r = 0;
  for ( auto op: pd_opr ) {
    if ( op == -1 ) {
      ++ r;
    }
  }
  if ( r == 0 ) {
    std::vector<int> bd_opr;
    bd_opr.reserve(pd_opr.size());
    for ( auto op: pd_opr ) {
      bd_opr.push_back(op + mInputNum);
    }
    auto new_bd = cur_bd; // コピー
    new_bd.push_back(bd_opr);
    recur_sub(new_bd, node_id + 1);
  }
  else {
    for ( CombiGen cg(mInputNum, r); !cg.is_end(); ++ cg ) {
      std::vector<int> bd_opr;
      bd_opr.reserve(pd_opr.size());
      SizeType pos = 0;
      for ( auto op: pd_opr ) {
	if ( op == -1 ) {
	  auto i = cg(pos);
	  ++ pos;
	  bd_opr.push_back(i);
	}
	else {
	  bd_opr.push_back(op + mInputNum);
	}
      }
      auto new_bd = cur_bd; // コピー
      new_bd.push_back(bd_opr);
      recur_sub(new_bd, node_id + 1);
    }
  }
}

// 適切な Bound DAG が調べる．
bool
BdGen::check_bd(
  const BdGen::OprList& bd
) const
{
  if ( !check_ord(bd) ) {
    return false;
  }
  if ( !check_noreapplication(bd) ) {
    return false;
  }
  if ( !check_all_inputs(bd) ) {
    return false;
  }
  return true;
}

// オペランド間の順序制約を調べる．
bool
BdGen::check_ord(
  const OprList& bd
) const
{
  auto l = bd.size();
  for ( SizeType i = 1; i < l; ++ i ) {
    auto& op1 = bd[i - 1];
    auto& op2 = bd[i];
    if ( !check_le(op1, op2) ) {
      return false;
    }
  }
  return true;
}

// オペランドの番号の比較を行う．
bool
BdGen::check_le(
  const BdGen::Opr& op1,
  const BdGen::Opr& op2
) const
{
  // 入力数を数える．
  SizeType ni1 = 0;
  for ( auto op: op1 ) {
    if ( op < mInputNum ) {
      ++ ni1;
    }
  }
  SizeType ni2 = 0;
  for ( auto op: op2 ) {
    if ( op < mInputNum ) {
      ++ ni2;
    }
  }
  if ( ni1 > ni2 ) {
    return true;
  }
  if ( ni1 < ni2 ) {
    return false;
  }
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

// reapplication のチェック
bool
BdGen::check_noreapplication(
  const OprList& bd
) const
{
  auto l = bd.size();
  for ( SizeType i = 0; i < l - 1; ++ i ) {
    auto& op1 = bd[i];
    for ( SizeType j = i + 1; j < l; ++ j ) {
      auto& op2 = bd[j];
      if ( op2.size() > 2 ) {
	continue;
      }
      auto y1 = op2[0];
      auto y2 = op2[1];
      if ( y2 != i + mInputNum ) {
	continue;
      }
      for ( auto x: op1 ) {
	if ( x == y1 ) {
	  return false;
	}
      }
    }
  }
  return true;
}

// 全ての入力が使われているか調べる．
bool
BdGen::check_all_inputs(
  const OprList& bd
) const
{
  std::vector<bool> marks(mInputNum, false);
  for ( auto& op: bd ) {
    for ( auto x: op ) {
      if ( x < mInputNum ) {
	marks[x] = true;
      }
    }
  }
  for ( auto m: marks ) {
    if ( !m ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_EXSYN
