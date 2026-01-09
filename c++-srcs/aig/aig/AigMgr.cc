
/// @file AigMgr.cc
/// @brief AigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "AigMgrImpl.h"
#include "AigTerminal.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgr::AigMgr(
) : mPtr{new AigMgrImpl}
{
}

// @brief 内容を指定したコンストラクタ
AigMgr::AigMgr(
  AigMgrImpl* impl
) : mPtr{impl}
{
}

// @brief デストラクタ
AigMgr::~AigMgr()
{
}

// @brief '深い'コピーを作る．
AigMgr
AigMgr::copy() const
{
  auto new_mgr = new AigMgrImpl;
  new_mgr->copy(get());
  return AigMgr(new_mgr);
}

// @brief ノード数を返す．
SizeType
AigMgr::node_num() const
{
  return get()->node_num();
}

// @brief 入力ノード数を返す．
SizeType
AigMgr::input_num() const
{
  return get()->input_num();
}

// @brief 出力ノード数を返す．
SizeType
AigMgr::output_num() const
{
  return get()->output_num();
}

// @brief ANDノード数を返す．
SizeType
AigMgr::and_num() const
{
  return get()->and_num();
}

// @brief DFF数を返す．
SizeType
AigMgr::dff_num() const
{
  return get()->dff_num();
}

// @brief 外部入力を表す枝を返す．
AigLiteral
AigMgr::input(
  SizeType input_id
) const
{
  auto node = get()->input_node(input_id);
  auto edge = AigEdge(node, false);
  return edge2literal(edge);
}

// @brief 入力名を返す．
std::string
AigMgr::input_name(
  SizeType input_id
) const
{
  return get()->input_name(input_id);
}

// @brief 外部出力に対応する枝返す．
AigLiteral
AigMgr::output(
  SizeType output_id
) const
{
  auto edge = get()->output(output_id)->edge();
  return edge2literal(edge);
}

// @brief 出力名を返す．
std::string
AigMgr::output_name(
  SizeType output_id
) const
{
  return get()->output_name(output_id);
}

// @brief ラッチ(DFF)の出力を返す．
AigLiteral
AigMgr::dff_output(
  SizeType dff_id
) const
{
  auto node = get()->dff_output(dff_id);
  auto edge = AigEdge(node, false);
  return edge2literal(edge);
}

// @brief ラッチ(DFF)の入力を返す．
AigLiteral
AigMgr::dff_input(
  SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
) const
{
  auto edge = get()->dff_input(dff_id)->edge();
  return edge2literal(edge);
}

// @brief ラッチ(DFF)名を返す．
std::string
AigMgr::dff_name(
  SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
) const
{
  return get()->dff_name(dff_id);
}

// @brief コメントのリストを返す．
const std::vector<std::string>&
AigMgr::comment_list() const
{
  return get()->comment_list();
}

// @brief 入力ノードの時 true を返す．
bool
AigMgr::is_input(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  return node->is_input();
}

// @brief 外部入力ノードの時 true を返す．
bool
AigMgr::is_primary_input(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  return node->is_primary_input();
}

// @brief 外部入力ノードの時 入力番号を返す．
SizeType
AigMgr::input_id(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  return node->input_id();
}

// @brief DFF出力の時 true を返す．
bool
AigMgr::is_dff_output(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  return node->is_dff_output();
}

// @brief DFF出力の時 DFF番号を返す．
SizeType
AigMgr::dff_id(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  return node->dff_id();
}

// @brief ANDノードの時 true を返す．
bool
AigMgr::is_and(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  return node->is_and();
}

// @brief ANDノードの時 ファンインを返す．
AigLiteral
AigMgr::fanin(
  SizeType node_id,
  SizeType pos
) const
{
  auto node = get()->node(node_id);
  auto e = node->fanin(pos);
  return edge2literal(e);
}

// @brief ANDノードの時 ファンイン0を返す．
AigLiteral
AigMgr::fanin0(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  auto e = node->fanin0();
  return edge2literal(e);
}

// @brief ANDノードの時 ファンイン1を返す．
AigLiteral
AigMgr::fanin1(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  auto e = node->fanin1();
  return edge2literal(e);
}

// @brief ANDグループのファンインのリストを返す．
std::vector<AigLiteral>
AigMgr::ex_fanin_list(
  SizeType node_id
) const
{
  auto node = get()->node(node_id);
  auto edge = AigEdge(node, false);
  return edge2literal_list(edge.ex_fanin_list());
}

// @brief 入力を作る．
AigLiteral
AigMgr::new_input(
  const std::string& name
)
{
  auto edge = get()->new_input(name);
  return edge2literal(edge);
}

// @brief 出力を作る．
SizeType
AigMgr::new_output(
  AigLiteral lit,
  const std::string& name
)
{
  auto edge = literal2edge(lit);
  auto output = get()->new_output(edge, name);
  return output->output_id();
}

// @brief DFFを作る．
SizeType
AigMgr::new_dff(
  const std::string& name
)
{
  return get()->new_dff(name);
}

// @brief DFFのデータ入力をセットする．
void
AigMgr::set_dff_input(
  SizeType dff_id,
  AigLiteral lit
)
{
  auto edge = literal2edge(lit);
  get()->set_dff_input(dff_id, edge);
}

// @brief local rewriting を行う．
void
AigMgr::rewrite()
{
  get()->rewrite();
}

// @brief シミュレーションを行う．
std::vector<AigBitVect>
AigMgr::simulate(
  const std::vector<AigBitVect>& input_vals
) const
{
  return get()->simulate(input_vals);
}

// @brief 内容を出力する．
void
AigMgr::print(
  std::ostream& s
) const
{
  get()->print(s);
}

// @brief 0 - (n - 1) 番目の入力のリストを返す．
std::vector<AigLiteral>
AigMgr::_input_list(
  SizeType n
)
{
  while ( input_num() < n ) {
    new_input();
  }
  std::vector<AigLiteral> input_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    input_list[i] = input(i);
  }
  return input_list;
}

// @brief AigLiteral を AigEdge に変換する
AigEdge
AigMgr::literal2edge(
  AigLiteral lit
) const
{
  if ( lit.is_zero() ) {
    return AigEdge::zero();
  }
  if ( lit.is_one() ) {
    return AigEdge::one();
  }
  auto node = get()->node(lit.node_id());
  return AigEdge(node, lit.inv());
}

// @brief AigLiteral のりストを AigEdge のりストに変換する
std::vector<AigEdge>
AigMgr::literal2edge_list(
  const std::vector<AigLiteral>& lit_list
) const
{
  std::vector<AigEdge> edge_list;
  edge_list.reserve(lit_list.size());
  for ( auto lit: lit_list ) {
    auto edge = literal2edge(lit);
    edge_list.push_back(edge);
  }
  return edge_list;
}

// @brief AigEdge を AigLiteral に変換する
AigLiteral
AigMgr::edge2literal(
  AigEdge edge
)
{
  if ( edge.is_zero() ) {
    return AigLiteral::zero();
  }
  if ( edge.is_one() ) {
    return AigLiteral::one();
  }
  auto node = edge.node();
  return AigLiteral(node->id(), edge.inv());
}

// @brief AigEdge のりストを AigLiteral のりストに変換する
std::vector<AigLiteral>
AigMgr::edge2literal_list(
  const std::vector<AigEdge>& edge_list
)
{
  std::vector<AigLiteral> lit_list;
  lit_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto lit = edge2literal(edge);
    lit_list.push_back(lit);
  }
  return lit_list;
}

END_NAMESPACE_YM_AIG
