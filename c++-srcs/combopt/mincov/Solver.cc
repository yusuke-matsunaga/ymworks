
/// @file Solver.cc
/// @brief Solver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Solver.h"
#include "Exact.h"
#include "Greedy.h"
#include "mincov/McMatrix.h"
#include "mincov/LbCalc.h"
#include "mincov/Selector.h"
#include "ym/JsonValue.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

BEGIN_NONAMESPACE

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス Solver
//////////////////////////////////////////////////////////////////////

// @brief オプションを解析してインスタンスを生成する関数
std::unique_ptr<Solver>
Solver::new_obj(
  McMatrix& matrix,
  const JsonValue& opt_obj
)
{
  auto algorithm = get_string(opt_obj, "algorithm", "greedy");
  if ( algorithm == "exact" ) {
    return std::unique_ptr<Solver>{new Exact{matrix, opt_obj}};
  }
  if ( algorithm == "greedy" ) {
    return std::unique_ptr<Solver>{new Greedy{matrix, opt_obj}};
  }
  {
    std::ostringstream buf;
    buf << algorithm << ": unknown algorithm";
    throw std::invalid_argument{buf.str()};
  }
  return nullptr;
}

// @brief コンストラクタ
Solver::Solver(
  McMatrix& matrix,
  const JsonValue& opt_obj
) : mMatrix{matrix},
    mDebug{get_int(opt_obj, "debug", 0)},
    mDebugDepth{get_int(opt_obj, "debug_depth", 2)}
{
}

// @brief デストラクタ
Solver::~Solver()
{
}

std::unique_ptr<Selector>
Solver::new_Selector(
  const JsonValue& opt_obj
)
{
  JsonValue sel_opt;
  if ( opt_obj.is_object() && opt_obj.has_key("selector") ) {
    sel_opt = opt_obj.at("selector");
    if ( !sel_opt.is_object() ) {
      throw std::invalid_argument{"selector should be a JSON-object"};
    }
  }
  return Selector::new_obj(sel_opt);
}

std::unique_ptr<LbCalc>
Solver::new_LbCalc(
  const JsonValue& opt_obj
)
{
  JsonValue lb_opt;
  if ( opt_obj.is_object() && opt_obj.has_key("lower_bound") ) {
    lb_opt = opt_obj.at("lower_bound");
  }
  return LbCalc::new_obj(lb_opt);
}

// @brief 行列の内容を出力する．
void
Solver::print_matrix(
  std::ostream& s
) const
{
  if ( debug() > 1 ) {
    matrix().print(s);
  }
  else if ( debug() > 0 ) {
    matrix().print_stats(s);
  }

}

END_NAMESPACE_YM_MINCOV
