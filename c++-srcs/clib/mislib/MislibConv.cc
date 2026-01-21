
/// @file MislibConv.cc
/// @brief MislibConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "MislibConv.h"
#include "MislibParser.h"
#include "MislibNode.h"
#include "MislibGate.h"
#include "MislibExpr.h"
#include "ci/CiCellLibrary.h"
#include "ci/CiCell.h"
#include "ym/Range.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// 論理式に現れる端子名を求めるクラス
//////////////////////////////////////////////////////////////////////
class ExprDfs
{
public:

  /// @brief コンストラクタ
  ExprDfs() = default;

  /// @brief デストラクタ
  ~ExprDfs() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式をスキャンする．
  void
  scan(
    const MislibExpr* expr,           ///< [in] 論理式を表すノード
    std::vector<ShString>& name_list, ///< [out] 端子名のリスト
    MislibNameMap& name_map           ///< [out] 端子名の辞書
  )
  {
    mNameList = &name_list;
    mNameMap = &name_map;
    scan_sub(expr);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式をスキャンする．
  void
  scan_sub(
    const MislibExpr* expr ///< [in] 論理式を表すノード
  );



private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前のリスト
  std::vector<ShString>* mNameList{nullptr};

  // 名前をキーにして mNameList 中の位置を表す辞書
  MislibNameMap* mNameMap{nullptr};

};


// @brief 論理式をスキャンする．
void
ExprDfs::scan_sub(
  const MislibExpr* expr
)
{
  switch ( expr->type() ) {
  case MislibExpr::Const0:
  case MislibExpr::Const1:
    return;

  case MislibExpr::VarName:
    {
      auto name = expr->varname();
      if ( mNameMap->count(name) == 0 ) {
	// 登録する．
	SizeType id = mNameList->size();
	mNameMap->emplace(name, id);
	mNameList->push_back(name);
      }
    }
    break;

  case MislibExpr::Not:
    scan_sub(expr->opr1());
    break;

  case MislibExpr::And:
  case MislibExpr::Or:
  case MislibExpr::Xor:
    scan_sub(expr->opr1());
    scan_sub(expr->opr2());
    break;

  default:
    ASSERT_NOT_REACHED;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MislibConv
//////////////////////////////////////////////////////////////////////

// @brief mislib 形式のファイルを読み込んでセルライブラリを作る．
std::unique_ptr<CiCellLibrary>
MislibConv::convert(
  const std::string& filename
)
{
  MislibParser parser;
  auto gate_list = parser.parse(filename);

  mLibrary = std::unique_ptr<CiCellLibrary>{new CiCellLibrary{}};

  // ファイル名をライブラリ名として登録する．
  auto name = filename.substr(filename.find_last_of('/') + 1);
  mLibrary->set_name(name);

  SizeType nerrs = 0;

  // セルを作る．
  for ( auto& gate: gate_list ) {
    try {
      new_gate(gate.get());
    }
    catch ( std::invalid_argument ) {
      ++ nerrs;
    }
  }

  if ( nerrs > 0 ) {
    std::ostringstream buf;
    buf << "Error in reading '" << filename << "'";
    throw std::invalid_argument{buf.str()};
  }

  mLibrary->compile();
  mLibrary->wrap_up();

  return std::move(mLibrary);
}

// @brief セルを作る．
void
MislibConv::new_gate(
  const MislibGate* gate
)
{
  auto name = gate->name()->str();
  auto area = ClibArea{gate->area()->num()};

  auto opin_name = gate->opin_name()->str();
  auto opin_expr = gate->opin_expr();

  auto npin = gate->ipin_num();
  const MislibPin* ipin_top{nullptr};
  std::vector<const MislibPin*> ipin_array;

  // 機能情報の取得
  std::vector<ShString> ipin_name_list;
  bool wildcard_pin = false;
  MislibNameMap ipin_name_map;
  if ( npin > 0 ) {
    ipin_top = gate->ipin(0);
    if ( ipin_top->name() != nullptr ) {
      // 通常の入力ピン定義がある場合
      // ipin_list の順に入力ピンを作る．
      for ( SizeType i: Range(npin) ) {
	auto pin = gate->ipin(i);
	auto name = pin->name()->str();
	ASSERT_COND( ipin_name_map.count(name) == 0 );
	ipin_name_map[name] = ipin_array.size();
	ipin_array.push_back(pin);
	ipin_name_list.push_back(name);
      }
    }
    else {
      // ワイルドカードの場合
      // 論理式に現れる順に入力ピンを作る．
      wildcard_pin = true;
      ExprDfs dfs;
      dfs.scan(opin_expr, ipin_name_list, ipin_name_map);
      for ( auto i: Range(ipin_name_list.size()) ) {
	ipin_array.push_back(ipin_top);
      }
    }
  }

  // セルを作る．
  auto cell = mLibrary->add_logic_cell(name, area);

  // 入力ピンのリストを作る．
  auto ni = ipin_name_list.size();
  for ( auto i = 0; i < ni; ++ i ) {
    // 入力ピンの設定
    auto name = ipin_name_list[i];
    auto pin = ipin_array[i];
    auto load = ClibCapacitance{pin->input_load()->num()};
    cell->add_input(name, load, load, load);
  }

  // 出力の論理式
  auto oexpr = opin_expr->to_expr(ipin_name_map);

  // 出力ピンを作る．
  cell->add_output(opin_name,
		   ClibCapacitance::infinity(),
		   ClibCapacitance{0.0},
		   ClibCapacitance::infinity(),
		   ClibCapacitance{0.0},
		   ClibTime::infinity(),
		   ClibTime{0.0},
		   oexpr,
		   Expr::invalid());

  // タイミング情報の生成
  cell->init_timing_map();
  auto tv_function = oexpr.tvfunc(ni);
  if ( wildcard_pin ) {
    // すべてのピンが同一のパラメータを持つ．
    auto pt_pin = ipin_top;
    auto timing = add_timing(cell, pt_pin);
    for ( SizeType i: Range(ni) ) {
      set_timing(pt_pin, tv_function, i, 0, cell, timing);
    }
  }
  else {
    // ピンごとに個別のパラメータを持つ．
    for ( SizeType i: Range(ni) ) {
      auto pt_pin = ipin_array[i];
      auto timing = add_timing(cell, pt_pin);
      set_timing(pt_pin, tv_function, i, 0, cell, timing);
    }
  }
}

// @brief タイミング情報を作る．
const CiTiming*
MislibConv::add_timing(
  CiCell* cell,
  const MislibPin* pin
)
{
  auto r_i = ClibTime{pin->rise_block_delay()->num()};
  auto r_r = ClibResistance{pin->rise_fanout_delay()->num()};
  auto f_i = ClibTime{pin->fall_block_delay()->num()};
  auto f_r = ClibResistance{pin->fall_fanout_delay()->num()};
  auto timing = cell->add_timing_generic(
    ClibTimingType::combinational,
    Expr::one(),
    r_i, f_i,
    ClibTime{0.0}, ClibTime{0.0},
    r_r, f_r
  );
  return timing;
}

// タイミング情報をセットする．
void
MislibConv::set_timing(
  const MislibPin* pt_pin,
  const TvFunc& tv_function,
  SizeType ipos,
  SizeType opos,
  CiCell* cell,
  const CiTiming* timing
)
{
  auto p_func = tv_function.cofactor(ipos, false);
  auto n_func = tv_function.cofactor(ipos, true);
  auto sense_real = ClibTimingSense::none;
  if ( ~p_func && n_func ) {
    if ( ~n_func && p_func ) {
      sense_real = ClibTimingSense::non_unate;
    }
    else {
      sense_real = ClibTimingSense::negative_unate;
    }
  }
  else if ( ~n_func && p_func ) {
    sense_real = ClibTimingSense::positive_unate;
  }
  else {
    // つまり p_func == n_func ということ．
    // つまりこの変数は出力に影響しない．
    std::ostringstream buf;
    buf << "The output function does not depend on the input pin, "
	<< pt_pin->name()->str() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_pin->loc(),
		    MsgType::Warning,
		    "MISLIB_PARSER",
		    buf.str());
    // タイミング情報は設定しない．
    return;
  }

  // 実際の極性情報と記述が合っているか確かめる．
  auto sense = ClibTimingSense::none;
  switch ( pt_pin->phase()->type() ) {
  case MislibPhase::Noninv:  sense = ClibTimingSense::positive_unate; break;
  case MislibPhase::Inv:     sense = ClibTimingSense::negative_unate; break;
  case MislibPhase::Unknown: sense = ClibTimingSense::non_unate; break;
  default: ASSERT_NOT_REACHED; break;
  }
  if ( sense != sense_real ) {
    std::ostringstream buf;
    buf << "Phase description does not match the logic expression. "
	<< "Ignored.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_pin->phase()->loc(),
		    MsgType::Warning,
		    "MISLIB_PARSER",
		    buf.str());
    sense = sense_real;
  }

  std::vector<const CiTiming*> timing_list{timing};
  if ( sense == ClibTimingSense::non_unate ) {
    cell->set_timing(ipos, opos, ClibTimingSense::positive_unate, timing_list);
    cell->set_timing(ipos, opos, ClibTimingSense::negative_unate, timing_list);
  }
  else {
    cell->set_timing(ipos, opos, sense, timing_list);
  }
}

END_NAMESPACE_YM_MISLIB

BEGIN_NAMESPACE_YM_CLIB

// @brief mislib 形式のファイルを読み込む．
CiCellLibrary*
CiCellLibrary::read_mislib(
  const std::string& filename
)
{
  nsMislib::MislibConv conv;
  auto lib_ptr = conv.convert(filename);
  auto lib = lib_ptr.get();
  lib_ptr.release();
  return lib;
}

END_NAMESPACE_YM_CLIB
