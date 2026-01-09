
/// @file AigWriter.cc
/// @brief AigWriter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "AigMgrImpl.h"
#include "AigWriter.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief 内容を aag ファイル形式で出力する．
void
AigMgr::write_aag(
  const std::string& filename
) const
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": Could not create";
    throw std::invalid_argument{buf.str()};
  }
  write_aag(s);
}

// @brief 内容を aag ファイル形式で出力する．
void
AigMgr::write_aag(
  std::ostream& s
) const
{
  AigWriter w(get());
  w.write_aag(s);
}

// @brief 内容を aig ファイル形式で出力する．
void
AigMgr::write_aig(
  const std::string& filename
) const
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": Could not create";
    throw std::invalid_argument{buf.str()};
  }
  write_aig(s);
}

// @brief 内容を aig ファイル形式で出力する．
void
AigMgr::write_aig(
  std::ostream& s
) const
{
  AigWriter w(get());
  w.write_aig(s);
}

// @brief 内容を出力する．
void
AigMgr::write(
  const std::string& filename,
  const std::string& format
) const
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": Could not create";
    throw std::invalid_argument{buf.str()};
  }
  write(s, format);
}

// @brief 内容を出力する．
void
AigMgr::write(
  std::ostream& s,
  const std::string& format
) const
{
  if ( format == "aag" ) {
    write_aag(s);
  }
  else if ( format == "aig" ) {
    write_aig(s);
  }
  else {
    std::ostringstream buf;
    buf << format << ": Unknown format for AigMgr::write()";
    throw std::invalid_argument{buf.str()};
  }
}


//////////////////////////////////////////////////////////////////////
// クラス AigWriter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigWriter::AigWriter(
  AigMgrImpl* mgr
) : mMgr{mgr}
{
}

// @brief aag 形式で出力する．
void
AigWriter::write_aag(
  std::ostream& s
)
{
  auto M = mMgr->node_num();
  auto I = mMgr->input_num();
  auto L = mMgr->dff_num();
  auto O = mMgr->output_num();
  auto A = mMgr->and_num();

  // mIdMap は恒等変換にしておく．
  mIdMap.clear();
  for ( SizeType id = 0; id < M; ++ id ) {
    mIdMap.emplace(id, id);
  }

  // ヘッダ行の出力
  s << "aag " << M << " " << I << " " << L
    << " " << O << " " << A
    << std::endl;

  // 入力行の出力
  for ( SizeType input_id = 0; input_id < I; ++ input_id ) {
    auto node = mMgr->input_node(input_id);
    s << id2lit(node->id())
      << std::endl;
  }

  // ラッチ(DFF)行の出力
  for ( SizeType dff_id = 0; dff_id < L; ++ dff_id ) {
    auto output = mMgr->dff_output(dff_id);
    auto input = mMgr->dff_input(dff_id);
    s << id2lit(output->id())
      << " "
      << edge2lit(input->edge())
      << std::endl;
  }

  // 出力行の出力
  for ( SizeType output_id = 0; output_id < O; ++ output_id ) {
    auto oedge = mMgr->output_edge(output_id);
    s << edge2lit(oedge)
      << std::endl;
  }

  // AND行の出力
  for ( SizeType id = 0; id < mMgr->node_num(); ++ id ) {
    auto node = mMgr->node(id);
    if ( !node->is_and() ) {
      continue;
    }
    auto fanin0 = node->fanin0();
    auto fanin1 = node->fanin1();
    s << id2lit(node->id())
      << " "
      << edge2lit(fanin0)
      << " "
      << edge2lit(fanin1)
      << std::endl;
  }

  // シンボルとコメントの出力
  write_symbols(s);
}

// @brief aig 形式で出力する．
void
AigWriter::write_aig(
  std::ostream& s
)
{
  // mAndList を作る．
  mAndList.reserve(mMgr->node_num());

  // DFFの入力からDFSを行い印を付ける，
  for ( SizeType i = 0; i < mMgr->dff_num(); ++ i ) {
    auto edge = mMgr->dff_input(i)->edge();
    mark_dfs(edge);
  }
  // 外部出力からDFSを行い印を付ける．
  for ( SizeType i = 0; i < mMgr->output_num(); ++ i ) {
    auto edge = mMgr->output(i)->edge();
    mark_dfs(edge);
  }

  auto I = mMgr->input_num();
  auto L = mMgr->dff_num();
  auto O = mMgr->output_num();
  auto A = mAndList.size();
  auto M = I + L + A;

  // aig 形式はノード番号に制約があるので実際のノード番号から
  // 見かけのノード番号への変換を行う必要がある．
  mIdMap.clear();

  // まず入力が最初
  for ( SizeType i = 0; i < I; ++ i ) {
    auto node = mMgr->input_node(i);
    auto id = node->id();
    mIdMap.emplace(id, i);
  }

  // 次が DFF の出力
  for ( SizeType i = 0; i < L; ++ i ) {
    auto output = mMgr->dff_output(i);
    auto id = output->id();
    mIdMap.emplace(id, I + i);
  }

  // 残りのANDノードは入力からのトポロジカル順で
  // 番号を割り振る．
  for ( SizeType i = 0; i < A; ++ i ) {
    auto node = mAndList[i];
    auto id = node->id();
    mIdMap.emplace(id, I + L + i);
  }

  // ヘッダの出力
  s << "aig " << M << " " << I << " " << L
    << " " << O << " " << A
    << std::endl;

  // DFF の入力のリテラルを出力する．
  for ( SizeType i = 0; i < L; ++ i ) {
    auto edge = mMgr->dff_input(i)->edge();
    auto lit = edge2lit(edge);
    s << lit
      << std::endl;
  }

  // 出力の枝のリテラルを出力する．
  for ( SizeType i = 0; i < O; ++ i ) {
    auto edge = mMgr->output_edge(i);
    auto lit = edge2lit(edge);
    s << lit
      << std::endl;
  }

  // ANDノードの内容を出力する．
  for ( auto node: mAndList ) {
    auto id = node->id();
    auto fanin0 = node->fanin0();
    auto fanin1 = node->fanin1();
    auto lhs = id2lit(id);
    auto rhs0 = edge2lit(fanin0);
    auto rhs1 = edge2lit(fanin1);
    if ( rhs0 < rhs1 ) {
      std::swap(rhs0, rhs1);
    }
    if ( lhs < rhs0 ) {
      throw std::logic_error{"wrong order"};
    }
    auto delta0 = lhs - rhs0;
    auto delta1 = rhs0 - rhs1;
    encode(s, delta0);
    encode(s, delta1);
  }

  // シンボルとコメントを出力する．
  write_symbols(s);
}

// @brief DFSを行い mAndList を作る．
void
AigWriter::mark_dfs(
  AigEdge edge
)
{
  if ( edge.is_const() ) {
    return;
  }

  auto node = edge.node();
  if ( node->is_input() ) {
    return;
  }

  auto id = node->id();
  if ( mMark.count(id) > 0 ) {
    // 処理済み
    return;
  }
  mMark.emplace(id);

  // node->is_and() == true
  mark_dfs(node->fanin0());
  mark_dfs(node->fanin1());

  mAndList.push_back(node);
}

// @brief シンボルとコメントを出力する．
void
AigWriter::write_symbols(
  std::ostream& s
)
{
  // これは内部のID番号と無関係なので mIdMap の影響を受けない．

  { // 入力シンボルの出力
    auto ni = mMgr->input_num();
    for ( SizeType input_id = 0; input_id < ni; ++ input_id ) {
      auto name = mMgr->input_name(input_id);
      if ( !name.empty() ) {
	s << "i" << input_id
	  << " "
	  << name
	  << std::endl;
      }
    }
  }

  { // ラッチシンボルの出力
    auto nf = mMgr->dff_num();
    for ( SizeType dff_id = 0; dff_id < nf; ++ dff_id ) {
      auto name = mMgr->dff_name(dff_id);
      if ( !name.empty() ) {
	s << "l" << dff_id
	  << " "
	  << name
	  << std::endl;
      }
    }
  }

  { // 出力シンボルの出力
    auto no = mMgr->output_num();
    for ( SizeType output_id = 0; output_id < no; ++ output_id ) {
      auto name = mMgr->output_name(output_id);
      if ( !name.empty() ) {
	s << "o" << output_id
	  << " "
	  << name
	  << std::endl;
      }
    }
  }

  { // コメントの出力
    auto& comment_list = mMgr->comment_list();
    if ( !comment_list.empty() ) {
      s << "c"
	<< std::endl;
      for ( auto& comment: comment_list ) {
	s << comment
	  << std::endl;
      }
    }
  }
}

// @brief 符号化を行って数値を書き込む．
void
AigWriter::encode(
  std::ostream& s,
  SizeType val
)
{
  static std::uint8_t c;
  while ( true ) {
    bool overflow = ( val >= 0x80 );
    c = static_cast<std::uint8_t>(val & 0x7f);
    if ( overflow ) {
      c |= 0x80;
    }
    s.write(reinterpret_cast<const char*>(&c), 1);
    val >>= 7;
    if ( !overflow ) {
      break;
    }
  }
}

END_NAMESPACE_YM_AIG
