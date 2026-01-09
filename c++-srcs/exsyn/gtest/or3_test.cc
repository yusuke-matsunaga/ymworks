
/// @file test1.cc
/// @brief test1 の実装ファイル
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
  Op& op1,
  Output& output,
  const std::vector<bool>& input_vals,
  bool oval
)
{
  std::vector<SatLiteral> opvar_list;
  opvar_list.reserve(5);
  for ( SizeType i = 0; i < 3; ++ i ) {
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

  auto vars1 = add_op_cond(solver, op1, opvar_list);
  auto ovar1 = vars1[0];
  auto ivar1_0 = vars1[1];
  auto ivar1_1 = vars1[2];
  opvar_list.push_back(ovar1);

  for ( SizeType i = 0; i < 3; ++ i ) {
    auto svar = output.sel.var(i);
    if ( input_vals[i] != oval ) {
      solver.add_clause(~svar);
    }
  }
  {
    auto svar = output.sel.var(3);
    auto ovar = opvar_list[3];
    if ( oval ) {
      solver.add_clause(~svar,  ovar);
    }
    else {
      solver.add_clause(~svar, ~ovar);
    }
  }
  {
    auto svar = output.sel.var(4);
    auto ovar = opvar_list[4];
    if ( oval ) {
      solver.add_clause(~svar,  ovar);
    }
    else {
      solver.add_clause(~svar, ~ovar);
    }
  }
  vars0.insert(vars0.end(), vars1.begin(), vars1.end());
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
  auto ovar1 = vars[3];
  auto ivar1_0 = vars[4];
  auto ivar1_1 = vars[5];
  std::cout << label
	    << ": ovar0 = " << model[ovar0]
	    << ", ivar0_0 = " << model[ivar0_0]
	    << ", ivar0_1 = " << model[ivar0_1]
	    << ", ovar1 = " << model[ovar1]
	    << ", ivar1_0 = " << model[ivar1_0]
	    << ", ivar1_1 = " << model[ivar1_1]
	    << std::endl;
}

void
or3_test()
{
  SatSolver solver;

  Op op0(solver, 3);
  Op op1(solver, 4);
  {
    auto svar0 = op1.sel0.var(3);
    auto _ = solver.condblock(svar0);
    solver.add_lt(op0.level, op1.level);
  }
  {
    auto svar1 = op1.sel1.var(3);
    auto _ = solver.condblock(svar1);
    solver.add_lt(op0.level, op1.level);
  }
  Output output(solver, 5);
  {
    auto svar0 = output.sel.var(3);
    auto _ = solver.condblock(svar0);
    solver.add_le(op0.level, output.level);
  }
  {
    auto svar1 = output.sel.var(4);
    auto _ = solver.condblock(svar1);
    solver.add_le(op1.level, output.level);
  }

  auto vars001 = add_cond(solver, op0, op1, output,
			  std::vector<bool>{false, false, true }, true);
  auto vars010 = add_cond(solver, op0, op1, output,
			  std::vector<bool>{false, true,  false}, true);
  auto vars011 = add_cond(solver, op0, op1, output,
			  std::vector<bool>{false, true,  true }, true);
  auto vars100 = add_cond(solver, op0, op1, output,
			  std::vector<bool>{true,  false, false}, true);
  auto vars101 = add_cond(solver, op0, op1, output,
			  std::vector<bool>{true,  false, true }, true);
  auto vars110 = add_cond(solver, op0, op1, output,
			  std::vector<bool>{true,  true,  false}, true);
  auto vars111 = add_cond(solver, op0, op1, output,
			  std::vector<bool>{true,  true,  true }, true);

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
      int tpat1 = 0;
      if ( model[op1.tval01] == SatBool3::True ) {
	tpat1 |= 1U;
      }
      if ( model[op1.tval10] == SatBool3::True ) {
	tpat1 |= 2U;
      }
      if ( model[op1.tval11] == SatBool3::True ) {
	tpat1 |= 4U;
      }
      std::cout << "tpat1 = " << tpat1 << std::endl;
      auto src1_0 = op1.sel0.val(model);
      auto src1_1 = op1.sel1.val(model);
      std::cout << "src1 = ("
		<< src1_0 << ", "
		<< src1_1 << ")"
		<< std::endl
		<< "level = "
		<< op1.level.val(model)
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
  nsYm::or3_test();
  return 0;
}
