
/// @file AigMgr_op.cc
/// @brief AigMgr::and_sub, or_sub, xor_sub の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief 複数の入力の AND を作る．
AigLiteral
AigMgr::new_and(
  const std::vector<AigLiteral>& fanin_list
)
{
  auto n = fanin_list.size();
  auto edge_list = literal2edge_list(fanin_list);
  auto edge = and_sub(edge_list, 0, n);
  return edge2literal(edge);
}

// @brief and_op() の下請け関数
AigEdge
AigMgr::and_sub(
  const std::vector<AigEdge>& fanin_list,
  SizeType begin,
  SizeType end
)
{
  if ( begin >= end ) {
    throw std::logic_error{"begin >= end"};
  }
  auto n = end - begin;
  if ( n == 1 ) {
    return fanin_list[begin];
  }
  AigEdge e0, e1;
  if ( n == 2 ) {
    e0 = fanin_list[begin + 0];
    e1 = fanin_list[begin + 1];
  }
  else {
    auto nh = (begin + end) / 2;
    e0 = and_sub(fanin_list, begin, nh);
    e1 = and_sub(fanin_list, nh, end);
  }
  return get()->new_and(e0, e1);
}

// @brief 複数の入力の OR を作る．
AigLiteral
AigMgr::new_or(
  const std::vector<AigLiteral>& fanin_list
)
{
  auto n = fanin_list.size();
  auto edge_list = literal2edge_list(fanin_list);
  auto edge = or_sub(edge_list, 0, n);
  return edge2literal(edge);
}

// @brief or_op() の下請け関数
AigEdge
AigMgr::or_sub(
  const std::vector<AigEdge>& fanin_list,
  SizeType begin,
  SizeType end
)
{
  if ( begin >= end ) {
    throw std::logic_error{"begin >= end"};
  }
  auto n = end - begin;
  if ( n == 1 ) {
    return fanin_list[begin];
  }
  AigEdge e0, e1;
  if ( n == 2 ) {
    e0 = fanin_list[begin + 0];
    e1 = fanin_list[begin + 1];
  }
  else {
    auto nh = (begin + end) / 2;
    e0 = or_sub(fanin_list, begin, nh);
    e1 = or_sub(fanin_list, nh, end);
  }
  return get()->new_or(e0, e1);
}

// @brief 複数の入力の XOR を作る．
AigLiteral
AigMgr::new_xor(
  const std::vector<AigLiteral>& fanin_list ///< [in] ファンインの枝のリスト
)
{
  auto n = fanin_list.size();
  auto edge_list = literal2edge_list(fanin_list);
  auto edge = xor_sub(edge_list, 0, n);
  return edge2literal(edge);
}

// @brief xor_op() の下請け関数
AigEdge
AigMgr::xor_sub(
  const std::vector<AigEdge>& fanin_list,
  SizeType begin,
  SizeType end
)
{
  if ( begin >= end ) {
    throw std::logic_error{"begin >= end"};
  }
  auto n = end - begin;
  if ( n == 1 ) {
    return fanin_list[begin];
  }
  AigEdge e0, e1;
  if ( n == 2 ) {
    e0 = fanin_list[begin + 0];
    e1 = fanin_list[begin + 1];
  }
  else {
    auto nh = (begin + end) / 2;
    e0 = xor_sub(fanin_list, begin, nh);
    e1 = xor_sub(fanin_list, nh, end);
  }
  return get()->new_xor(e0, e1);
}

// @brief PrimType の演算を行う．
AigLiteral
AigMgr::new_primitive(
  PrimType type,
  const std::vector<AigLiteral>& fanin_list
)
{
  switch ( type ) {
  case PrimType::None: break;
  case PrimType::C0:   return AigLiteral::zero();
  case PrimType::C1:   return AigLiteral::one();
  case PrimType::Buff: return fanin_list[0];
  case PrimType::Not:  return ~fanin_list[0];
  case PrimType::And:  return new_and(fanin_list);
  case PrimType::Nand: return ~new_and(fanin_list);
  case PrimType::Or:   return new_or(fanin_list);
  case PrimType::Nor:  return ~new_or(fanin_list);
  case PrimType::Xor:  return new_xor(fanin_list);
  case PrimType::Xnor: return ~new_xor(fanin_list);
  }
  std::ostringstream buf;
  buf << type << ": Wrong primitive type";
  throw std::invalid_argument{buf.str()};
  return AigLiteral::zero();
}

END_NAMESPACE_YM_AIG
