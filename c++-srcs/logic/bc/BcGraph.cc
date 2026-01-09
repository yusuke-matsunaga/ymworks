
/// @file BcGraph.cc
/// @brief BcGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BcGraph.h"
#include "BcSim.h"
#include "BcSim2.h"


BEGIN_NAMESPACE_YM_BC

//////////////////////////////////////////////////////////////////////
// クラス BcGraph
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
BcGraph::BcGraph()
{
}

// @brief 入力サイズだけ指定したコンストラクタ
BcGraph::BcGraph(
  SizeType input_num
) : mInputNum{input_num}
{
}

// @brief デストラクタ
BcGraph::~BcGraph()
{
}

// @brief 枝のレベルを返す．
SizeType
BcGraph::edge_level(
  const BcEdge& edge
) const
{
  if ( edge.is_input() ) {
    return 0;
  }
  if ( edge.is_zero() ) {
    return 0;
  }
  if ( edge.is_one() ) {
    return 0;
  }
  // is_op() == true のはず
  auto op_id = edge.op_id();
  auto& op = this->op(op_id);
  return op.level();
}

// @brief シミュレーションを行う(bool値版)．
std::vector<bool>
BcGraph::simulate(
  const std::vector<bool>& input_vals
) const
{
  if ( input_vals.size() != input_num() ) {
    throw std::invalid_argument{"input_vals.size() != input_num()"};
  }
  BcSim sim(*this, input_vals);
  return sim.simulate();
}

// @brief シミュレーションを行う．
std::vector<BcBvType>
BcGraph::simulate(
  const std::vector<BcBvType>& input_vals
) const
{
  if ( input_vals.size() != input_num() ) {
    throw std::invalid_argument{"input_vals.size() != input_num()"};
  }
  BcSim2 sim(*this, input_vals);
  return sim.simulate();
}

// @brief add_op(), add_tvop() の下請け関数
SizeType
BcGraph::add_op_sub(
  BcOpType type,
  const std::vector<BcBvType>& tv,
  const std::vector<BcEdge>& opr_list
)
{
  SizeType op_id = mOpList.size();
  SizeType max_lvl = 0;
  for ( auto& opr: opr_list ) {
    auto lvl = edge_level(opr);
    max_lvl = std::max(max_lvl, lvl);
  }
  ++ max_lvl;
  mOpList.push_back(BcOp(op_id, type, tv, opr_list, max_lvl));
  return op_id;
}

// @brief 出力を追加する．
SizeType
BcGraph::add_output(
  BcEdge src
)
{
  SizeType output_id = mOutputList.size();
  mOutputList.push_back(src);
  auto lvl = edge_level(src);
  mOutputLevelList.push_back(lvl);
  mMaxLevel = std::max(mMaxLevel, lvl);
  return output_id;
}

// @brief 定数0の出力を追加する．
SizeType
BcGraph::add_zero_output()
{
  return add_output(BcEdge::zero());
}

// @brief 定数1の出力を追加する．
SizeType
BcGraph::add_one_output()
{
  return add_output(BcEdge::one());
}

// @brief 内容を出力する．
void
BcGraph::print(
  std::ostream& s
) const
{
  s << input_num() << " " << output_num() << " " << op_num()
    << std::endl;
  SizeType op_id = 0;
  for ( auto& op: mOpList ) {
    s << "OP#" << op_id << ": "
      << op.to_string()
      << std::endl;
    ++ op_id;
  }
  SizeType output_id = 0;
  for ( auto& edge: mOutputList ) {
    s << "O#" << output_id << ": "
      << edge.to_string()
      << std::endl;
    ++ output_id;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス BcOp
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す文字列を返す．
std::string
BcOp::to_string() const
{
  std::ostringstream buf;
  buf << mType;
  if ( mType == BcOpType::TV ) {
    SizeType vlen = 1 << operand_num();
    buf << "[";
    for ( SizeType i = 0; i < vlen; ++ i ) {
      buf << tv_bit(vlen - i - 1);
    }
    buf << "]";
  }
  buf << "(";
  const char* sep = "";
  for ( auto& opr: mOprList ) {
    buf << sep << opr.to_string();
    sep = ", ";
  }
  buf << ")";
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス BcEdge
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す文字列を返す．
std::string
BcEdge::to_string() const
{
  if ( is_zero() ) {
    return "0";
  }
  if ( is_one() ) {
    return "1";
  }
  std::ostringstream buf;
  if ( inv() ) {
    buf << "~";
  }
  if ( is_input() ) {
    buf << "I#" << input_id();
  }
  else if ( is_op() ) {
    buf << "OP#" << op_id();
  }
  else {
    throw std::logic_error{"never happen"};
  }
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス BcSim
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BcSim::BcSim(
  const BcGraph& g,
  const std::vector<bool>& input_vals
) : mG{g},
    mInputVals{input_vals},
    mOpVals(g.op_num())
{
}

// @brief シミュレーションを行う．
std::vector<bool>
BcSim::simulate()
{
  for ( SizeType op_id = 0; op_id < mG.op_num(); ++ op_id ) {
    auto& op = mG.op(op_id);
    SizeType ni = op.operand_num();
    std::vector<bool> ival_list(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto opr = op.operand(i);
      ival_list[i] = value(opr);
    }
    auto val = calc_op(op, ival_list);
    mOpVals[op.id()] = val;
  }
  SizeType no = mG.output_num();
  std::vector<bool> output_vals(no);
  for ( SizeType i = 0; i < no; ++ i ) {
    auto edge = mG.output(i);
    auto val = value(edge);
    output_vals[i] = val;
  }
  return output_vals;
}

BEGIN_NONAMESPACE

SizeType
encode_index(
  const std::vector<bool>& ival_list
)
{
  auto ni = ival_list.size();
  SizeType index = 0;
  for ( SizeType i = 0; i < ni; ++ i ) {
    if ( ival_list[i] ) {
      index += (1 << (ni - i - 1));
    }
  }
  return index;
}

END_NONAMESPACE

// @brief 演算ノードの出力の値を計算する．
bool
BcSim::calc_op(
  const BcOp& op,
  const std::vector<bool>& ival_list
)
{
  switch ( op.type() ) {
  case BcOpType::AND:
    for ( auto val: ival_list ) {
      if ( !val ) {
	return false;
      }
    }
    return true;

  case BcOpType::OR:
    for ( auto val: ival_list ) {
      if ( val ) {
	return true;
      }
    }
    return false;

  case BcOpType::XOR:
  {
    bool val = false;
    for ( auto val1: ival_list ) {
      if ( val1 ) {
	val = !val;
      }
    }
    return val;
  }

  case BcOpType::TV:
  {
    auto index = encode_index(ival_list);
    return op.tv_bit(index);
  }

  default:
    throw std::runtime_error{"op.type() is NONE"};
  }
}

// @brief 枝に対する値を求める．
bool
BcSim::value(
  const BcEdge& edge
) const
{
  bool val;
  if ( edge.is_input() ) {
    val = mInputVals[edge.input_id()];
  }
  else if ( edge.is_op() ) {
    val = mOpVals[edge.op_id()];
  }
  else if ( edge.is_const() ) {
    val = false;
  }
  else {
    throw std::runtime_error{"edge.type() is wrong"};
  }
  if ( edge.inv() ) {
    val = !val;
  }
  return val;
}


//////////////////////////////////////////////////////////////////////
// クラス BcSim2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BcSim2::BcSim2(
  const BcGraph& g,
  const std::vector<BcBvType>& input_vals
) : mG{g},
    mInputVals{input_vals},
    mOpVals(g.op_num())
{
}

// @brief シミュレーションを行う．
std::vector<BcBvType>
BcSim2::simulate()
{
  for ( SizeType op_id = 0; op_id < mG.op_num(); ++ op_id ) {
    auto& op = mG.op(op_id);
    SizeType ni = op.operand_num();
    std::vector<BcBvType> ival_list(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto opr = op.operand(i);
      ival_list[i] = value(opr);
    }
    auto val = calc_op(op, ival_list);
    mOpVals[op.id()] = val;
  }
  SizeType no = mG.output_num();
  std::vector<BcBvType> output_vals(no);
  for ( SizeType i = 0; i < no; ++ i ) {
    auto edge = mG.output(i);
    auto val = value(edge);
    output_vals[i] = val;
  }
  return output_vals;
}

BEGIN_NONAMESPACE

SizeType
encode_index(
  const std::vector<BcBvType>& ival_list,
  SizeType bitpos
)
{
  auto ni = ival_list.size();
  SizeType index = 0;
  for ( SizeType i = 0; i < ni; ++ i ) {
    if ( ((ival_list[i] >> bitpos) & 1) == 1 ) {
      index += (1 << (ni - i - 1));
    }
  }
  return index;
}

END_NONAMESPACE

// @brief 演算ノードの出力の値を計算する．
BcBvType
BcSim2::calc_op(
  const BcOp& op,
  const std::vector<BcBvType>& ival_list
)
{
  BcBvType val;
  switch ( op.type() ) {
  case BcOpType::AND:
    val = BcBv_ALL1;
    for ( auto val1: ival_list ) {
      val &= val1;
    }
    return val;

  case BcOpType::OR:
    val = BcBv_ALL0;
    for ( auto val1: ival_list ) {
      val |= val1;
    }
    return val;

  case BcOpType::XOR:
    val = BcBv_ALL0;
    for ( auto val1: ival_list ) {
      val ^= val1;
    }
    return val;

  case BcOpType::TV:
  { // 効率的なやり方はない．
    val = BcBv_ALL0;
    for ( SizeType i = 0; i < 64; ++ i ) {
      auto index = encode_index(ival_list, i);
      val |= static_cast<BcBvType>(op.tv_bit(index)) << i;
    }
    return val;
  }

  default:
    throw std::runtime_error{"op.type() is NONE"};
  }
}

// @brief 枝に対する値を求める．
BcBvType
BcSim2::value(
  const BcEdge& edge
) const
{
  BcBvType val;
  if ( edge.is_input() ) {
    val = mInputVals[edge.input_id()];
  }
  else if ( edge.is_op() ) {
    val = mOpVals[edge.op_id()];
  }
  else if ( edge.is_const() ) {
    val = BcBv_ALL0;
  }
  else {
    throw std::runtime_error{"edge.type() is wrong"};
  }
  if ( edge.inv() ) {
    val = ~val;
  }
  return val;
}

END_NAMESPACE_YM_BC
