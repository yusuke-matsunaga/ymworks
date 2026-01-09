
/// @file and2_test.cc
/// @brief and2_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SatSolver.h"
#include "ym/SatCondBlock.h"
#include "ym/SatLiteral.h"
#include "ym/SatOrderedSet.h"


BEGIN_NAMESPACE_YM

struct Op {
  SatLiteral tval01;
  SatLiteral tval10;
  SatLiteral tval11;
  SatOrderedSet sel0;
  SatOrderedSet sel1;
  SatOrderedSet level;

  Op(
    SatSolver& solver,
    SizeType cand_size
  )
  {
    tval01 = solver.new_variable(true);
    tval10 = solver.new_variable(true);
    tval11 = solver.new_variable(true);
    sel0 = solver.new_ordered_set(0, cand_size - 1);
    sel1 = solver.new_ordered_set(0, cand_size - 1);
    level = solver.new_ordered_set(1, cand_size);

    solver.add_clause( tval01,  tval10,  tval11);
    solver.add_clause( tval01, ~tval10, ~tval11);
    solver.add_clause(~tval01,  tval10, ~tval11);

    solver.add_lt(sel0, sel1);
  }

};

struct Output {
  SatOneHotVar sel;
  SatOrderedSet level;

  Output(
    SatSolver& solver,
    SizeType cand_size
  )
  {
    sel = solver.new_onehot_var(cand_size);
    level = solver.new_ordered_set(0, cand_size);
  }

};


// @brief 演算ノードの入力変数に関する制約を追加する．
SatLiteral
add_op_ivar_cond(
  SatSolver& solver,
  SatOrderedSet& sel,
  const std::vector<SatLiteral>& opvar_list
)
{
  auto ivar = solver.new_variable(true);
  // 他の演算ノードを入力とする場合の条件
  for ( SizeType i = 0; i < opvar_list.size(); ++ i ) {
    auto svar = sel.var(i);
    auto ovar = opvar_list[i];
    solver.add_clause(~svar, ~ovar,  ivar);
    solver.add_clause(~svar,  ovar, ~ivar);
  }
  return ivar;
}

std::vector<SatLiteral>
add_op_cond(
  SatSolver& solver,
  Op& op,
  const std::vector<SatLiteral>& opvar_list
)
{
  // 演算ノードの入力の値を表す変数
  auto ivar0 = add_op_ivar_cond(solver, op.sel0, opvar_list);
  auto ivar1 = add_op_ivar_cond(solver, op.sel1, opvar_list);
  // 演算ノードの出力の値を表す変数
  auto ovar = solver.new_variable(true);
  // 演算ノードの入力と出力に関する条件
  // ivals = 00 -> ovar = 0
  solver.add_clause( ivar0,  ivar1,             ~ovar);
  // ivals = 01 -> ovar = tval01
  solver.add_clause( ivar0, ~ivar1, ~op.tval01,  ovar);
  solver.add_clause( ivar0, ~ivar1,  op.tval01, ~ovar);
  // ivals = 10 -> ovar = tval10
  solver.add_clause(~ivar0,  ivar1, ~op.tval10,  ovar);
  solver.add_clause(~ivar0,  ivar1,  op.tval10, ~ovar);
  // ivals = 11 -> ovar = tval11
  solver.add_clause(~ivar0, ~ivar1, ~op.tval11,  ovar);
  solver.add_clause(~ivar0, ~ivar1,  op.tval11, ~ovar);
  return std::vector<SatLiteral>{ovar, ivar0, ivar1};
}

std::vector<SatLiteral>
add_cond(
  SatSolver& solver,
  Op& op0,
  Output& output,
  const std::vector<bool>& input_vals,
  bool oval
)
{
  std::vector<SatLiteral> opvar_list;
  opvar_list.reserve(3);
  for ( SizeType i = 0; i < 2; ++ i ) {
    auto ivar = solver.new_variable(false);
    if ( input_vals[i] ) {
      solver.add_clause( ivar);
    }
    else {
      solver.add_clause(~ivar);
    }
    opvar_list.push_back(ivar);
  }

  auto vars0 = add_op_cond(solver, op0, opvar_list);
  auto ovar0 = vars0[0];
  auto ivar0_0 = vars0[1];
  auto ivar0_1 = vars0[2];
  opvar_list.push_back(ovar0);

  for ( SizeType i = 0; i < 2; ++ i ) {
    auto svar = output.sel.var(i);
    if ( input_vals[i] != oval ) {
      solver.add_clause(~svar);
    }
  }
  {
    auto svar = output.sel.var(2);
    auto ovar = opvar_list[2];
    if ( oval ) {
      solver.add_clause(~svar,  ovar);
    }
    else {
      solver.add_clause(~svar, ~ovar);
    }
  }
  return vars0;
}

void
print_vars(
  const char* label,
  const std::vector<SatLiteral>& vars,
  const SatModel& model
)
{
  auto ovar0 = vars[0];
  auto ivar0_0 = vars[1];
  auto ivar0_1 = vars[2];
  std::cout << label
	    << ": ovar0 = " << model[ovar0]
	    << ", ivar0_0 = " << model[ivar0_0]
	    << ", ivar0_1 = " << model[ivar0_1]
	    << std::endl;
}

void
and2_test()
{
  SatSolver solver;

  Op op0(solver, 2);
  Output output(solver, 3);
  {
    auto svar0 = output.sel.var(2);
    auto _ = solver.condblock(svar0);
    solver.add_le(op0.level, output.level);
  }

  auto vars01 = add_cond(solver, op0, output,
			 std::vector<bool>{false, true }, false);
  auto vars10 = add_cond(solver, op0, output,
			 std::vector<bool>{true,  false}, false);
  auto vars11 = add_cond(solver, op0, output,
			 std::vector<bool>{true,  true }, true);

  for ( SizeType level = 0; level < 3; ++ level ) {
    std::cout << "level = " << level << std::endl;
    std::vector<SatLiteral> assumptions(1);
    assumptions[0] = output.level.var(level);
    auto res = solver.solve(assumptions);
    if ( res == SatBool3::True ) {
      std::cout << "SAT" << std::endl;
      auto& model = solver.model();
      int tpat0 = 0;
      if ( model[op0.tval01] == SatBool3::True ) {
	tpat0 |= 1U;
      }
      if ( model[op0.tval10] == SatBool3::True ) {
	tpat0 |= 2U;
      }
      if ( model[op0.tval11] == SatBool3::True ) {
	tpat0 |= 4U;
      }
      std::cout << "tpat0 = " << tpat0 << std::endl;
      auto src0_0 = op0.sel0.val(model);
      auto src0_1 = op0.sel1.val(model);
      std::cout << "src0 = ("
		<< src0_0 << ", "
		<< src0_1 << ")"
		<< std::endl
		<< "level = "
		<< op0.level.val(model)
		<< std::endl;
      break;
    }
  }
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  nsYm::and2_test();
  return 0;
}
