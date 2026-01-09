
/// @file PdNode4_4_SSV.cc
/// @brief PdNode4_4_SSV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "PdNode4_4_SSV.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
// クラス PdNode4_4_SSV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PdNode4_4_SSV::PdNode4_4_SSV(
  SatSolver& solver,
  SizeType input_num,
  SizeType op_id
) : PdNode4(solver, input_num, op_id),
    mLitArray(input_num * input_num * input_num * input_num)
{
  auto n = input_num * (input_num - 1) * (input_num - 2) * (input_num - 3) / 24;
  mLitList.reserve(n);
  SizeType input_num2 = input_num * input_num;
  SizeType input_num3 = input_num2 * input_num;
  for ( SizeType sel0 = 0; sel0 < input_num - 3; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num - 2; ++ sel1 ) {
      for ( SizeType sel2 = sel1 + 1; sel2 < input_num - 1; ++ sel2 ) {
	for ( SizeType sel3 = sel2 + 1; sel3 < input_num; ++ sel3 ) {
	  auto lit = solver.new_variable(true);
	  mLitArray[sel0 * input_num3 +
		    sel1 * input_num2 +
		    sel2 * input_num +
		    sel3] = lit;
	  mLitList.push_back(lit);
	}
      }
    }
  }

  // mLitList の中で1つのリテラルのみが True になるという制約
  solver.add_exact_one(mLitList);
}

// @brief 出力値に関する制約を追加する．
SatLiteral
PdNode4_4_SSV::add_ovar_cond(
  SatSolver& solver,
  const std::vector<bool>& ival_list,
  const std::vector<SatLiteral>& opvar_list
) const
{
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(false);
  for ( SizeType sel0 = 0; sel0 < input_num() - 3; ++ sel0 ) {
    for ( SizeType sel1 = sel0 + 1; sel1 < input_num() - 2; ++ sel1 ) {
      for ( SizeType sel2 = sel1 + 1; sel2 < input_num() - 1; ++ sel2 ) {
	for ( SizeType sel3 = sel2 + 1; sel3 < input_num(); ++ sel3 ) {
	  auto lit = sel_lit(sel0, sel1, sel2, sel3);
	  auto _ = solver.condblock(lit);
	  // 演算ノードの入力と出力に関する条件
	  add_op_cond(solver, ival_list[sel0], ival_list[sel1],
		      ival_list[sel2], ival_list[sel3], ovar);
	}
      }
    }
  }
  return ovar;
}

// @brief 直前の演算ノードに対する制約を追加する．
void
PdNode4_4_SSV::add_op_order_cond(
  SatSolver& solver,
  const OpNode* op
) const
{
  auto op_osv = reinterpret_cast<const PdNode4_4_SSV*>(op);
  SizeType N = input_num();
  for ( SizeType op2_i1 = 0; op2_i1 < N - 2; ++ op2_i1 ) {
    for ( SizeType op2_i2 = op2_i1 + 1; op2_i2 < N - 1; ++ op2_i2 ) {
      for ( SizeType op2_i3 = op2_i2 + 1; op2_i3 < N; ++ op2_i3 ) {
	for ( SizeType op2_i4 = op2_i3 + 1; op2_i4 < N; ++ op2_i4 ) {
	  auto lit2 = sel_lit(op2_i1, op2_i2, op2_i3, op2_i4);
	  for ( SizeType op1_i1 = 0; op1_i1 < N - 2; ++ op1_i1 ) {
	    for ( SizeType op1_i2 = op1_i1 + 1; op1_i2 < N - 1; ++ op1_i2 ) {
	      for ( SizeType op1_i3 = op1_i2 + 1; op1_i3 < N; ++ op1_i3 ) {
		for ( SizeType op1_i4 = op1_i3 + 1; op1_i4 < N; ++ op1_i4 ) {
		  if ( op2_i1 > op1_i1 ) {
		    continue;
		  }
		  if ( op2_i1 == op1_i2 ) {
		    if ( op2_i2 > op1_i2 ) {
		      continue;
		    }
		    if ( op2_i2 == op1_i2 ) {
		      if ( op2_i3 > op1_i3 ) {
			continue;
		      }
		      if ( op2_i3 == op1_i3 ) {
			if ( op2_i4 >= op1_i4 ) {
			  continue;
			}
		      }
		    }
		  }
		  // ここの残った場合は opr2 < opr1
		  auto lit1 = op_osv->sel_lit(op1_i1, op1_i2, op1_i3, op1_i4);
		  solver.add_clause(~lit2, ~lit1);
		}
	      }
	    }
	  }
	}
      }
    }
  }
}

// @brief SATモデルから入力の選択結果を得る．
std::vector<SizeType>
PdNode4_4_SSV::get_opr(
  const SatModel& model
) const
{
  for ( SizeType opr0 = 0; opr0 < input_num() - 3; ++ opr0 ) {
    for ( SizeType opr1 = opr0 + 1; opr1 < input_num() - 2; ++ opr1 ) {
      for ( SizeType opr2 = opr1 + 1; opr2 < input_num() - 1; ++ opr2 ) {
	for ( SizeType opr3 = opr2 + 1; opr3 < input_num(); ++ opr3 ) {
	  auto lit = sel_lit(opr0, opr1, opr2, opr3);
	  if ( model[lit] == SatBool3::True ) {
	    return std::vector<SizeType>{opr0, opr1, opr2, opr3};
	  }
	}
      }
    }
  }
  throw std::logic_error{"one-hot constraint is not satisfied"};
}

END_NAMESPACE_YM_EXSYN
