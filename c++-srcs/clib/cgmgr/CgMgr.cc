
/// @file CgMgr.cc
/// @brief CgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "cgmgr/CgMgr.h"
#include "cgmgr/CgSignature.h"
#include "ci/CiCellLibrary.h"
#include "ci/CiCellGroup.h"
#include "ci/CiCellClass.h"
#include "ym/ClibSeqAttr.h"
#include "ym/TvFunc.h"
#include "ym/NpnMap.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CgMgr::CgMgr(
  CiCellLibrary& library
) : mLibrary{library}
{
  for ( SizeType i = 0; i < 4; ++ i ) {
    mLogicGroup[i] = nullptr;
  }

  logic_init();
}

// @brief 論理セルグループの初期化を行なう．
void
CgMgr::logic_init()
{
  { // 定数0グループの登録
    auto sig = CgSignature::make_logic_sig(0, Expr::zero());
    auto func0 = find_group(sig);
    mLogicGroup[C0_BASE] = func0;
  }
  { // 定数1グループの登録
    auto sig = CgSignature::make_logic_sig(0, Expr::one());
    auto func1 = find_group(sig);
    mLogicGroup[C1_BASE] = func1;
  }
  { // バッファグループの登録
    auto sig = CgSignature::make_logic_sig(1, Expr::positive_literal(0));
    auto func2 = find_group(sig);
    mLogicGroup[BUF_BASE] = func2;
  }
  { // インバーターグループの登録
    auto sig = CgSignature::make_logic_sig(1, Expr::negative_literal(0));
    auto func3 = find_group(sig);
    mLogicGroup[INV_BASE] = func3;
  }

  // 以降の処理は必須ではないが，
  // クラス一覧に現れる順序を統一する効果がある．

  // AND2 〜 AND4 のグループの登録
  // NAND2 〜 NAND4 のグループの登録
  // OR2 〜 OR4 のグループの登録
  // NOR2 〜 NOR4 のグループの登録
  for ( auto ni: {2, 3, 4} ) {
    auto and_expr = Expr::positive_literal(0);
    auto or_expr = Expr::positive_literal(0);
    for ( SizeType i = 1; i < ni; ++ i ) {
      and_expr &= Expr::positive_literal(i);
      or_expr |= Expr::positive_literal(i);
    }

    SizeType index = ni - 2;

    auto and_sig = CgSignature::make_logic_sig(ni, and_expr);
    auto and_func = find_group(and_sig);
    mLogicGroup[AND_BASE + index] = and_func;

    auto nand_sig = CgSignature::make_logic_sig(ni, ~and_expr);
    auto nand_func = find_group(nand_sig);
    mLogicGroup[NAND_BASE + index] = nand_func;

    auto or_sig = CgSignature::make_logic_sig(ni, or_expr);
    auto or_func = find_group(or_sig);
    mLogicGroup[OR_BASE + index] = or_func;

    auto nor_sig = CgSignature::make_logic_sig(ni, ~or_expr);
    auto nor_func = find_group(nor_sig);
    mLogicGroup[NOR_BASE + index] = nor_func;
  }

  // XOR2 〜 XOR4 のグループの登録
  // XNOR2 〜 XNOR4 のグループの登録
  for ( auto ni: {2, 3, 4} ) {
    auto xor_expr = Expr::positive_literal(0);
    for ( SizeType i = 1; i < ni; ++ i ) {
      xor_expr ^= Expr::positive_literal(i);
    }

    SizeType index = ni - 2;

    auto xor_sig = CgSignature::make_logic_sig(ni, xor_expr);
    auto xor_func = find_group(xor_sig);
    mLogicGroup[XOR_BASE + index] = xor_func;

    auto xnor_sig = CgSignature::make_logic_sig(ni, ~xor_expr);
    auto xnor_func = find_group(xnor_sig);
    mLogicGroup[XNOR_BASE + index] = xnor_func;
  }

  { // MUX2 のグループの登録．
    SizeType ni = 3;
    auto lit0 = Expr::positive_literal(0);
    auto lit1 = Expr::positive_literal(1);
    auto lit2 = Expr::positive_literal(2);
    auto mux2_func = lit0 & ~lit2 | lit1 & lit2;
    auto sig = CgSignature::make_logic_sig(ni, mux2_func);
    auto mux_func = find_group(sig);
    mLogicGroup[MUX2_BASE] = mux_func;
  }

  { // MUX4 のグループの登録．
    SizeType ni = 6;
    auto lit0 = Expr::positive_literal(0);
    auto lit1 = Expr::positive_literal(1);
    auto lit2 = Expr::positive_literal(2);
    auto lit3 = Expr::positive_literal(3);
    auto lit4 = Expr::positive_literal(4);
    auto lit5 = Expr::positive_literal(5);
    auto mux4_func =
      lit0 & ~lit4 & ~lit5 |
      lit1 &  lit4 & ~lit5 |
      lit2 & ~lit4 &  lit5 |
      lit3 &  lit4 &  lit5;
    auto sig = CgSignature::make_logic_sig(ni, mux4_func);
    auto mux_func = find_group(sig);
    mLogicGroup[MUX4_BASE] = mux_func;
  }
}

// @brief FFクラスを得る．
std::vector<const CiCellClass*>
CgMgr::find_ff_class(
  ClibSeqAttr seq_attr
) const
{
  auto index = seq_attr.index();
  if ( mFFClassDict.count(index) > 0 ) {
    return mFFClassDict.at(index);
  }
  return {};
}

// @brief ラッチクラスを得る．
std::vector<const CiCellClass*>
CgMgr::find_latch_class(
  ClibSeqAttr seq_attr
) const
{
  auto index = seq_attr.index();
  if ( mLatchClassDict.count(index) > 0 ) {
    return mLatchClassDict.at(index);
  }
  return {};
}

// @brief シグネチャに一致するグループを探す．
CiCellGroup*
CgMgr::find_group(
  const CgSignature& sig
)
{
  // シグネチャ文字列を作る．
  auto sig_str = sig.str();

  // このシグネチャを持つグループを探す．
  if ( mGroupDict.count(sig_str) == 0 ) {
    // 未登録の場合
    // 代表シグネチャに対する変換を求める．
    auto rep_map = sig.rep_map();
    // 代表シグネチャを求める．
    auto rep_sig = sig.xform(rep_map);
    // クラスを求める．
    auto rep_class = _find_class(rep_sig);
    // グループを作る．
    auto group = mLibrary.add_cell_group(rep_class, rep_map);
    // そのクラスに新しいグループを追加する．
    rep_class->add_group(group);

    // 登録する．
    mGroupDict.emplace(sig_str, group);
    return group;
  }
  else {
    // 登録済みのグループを返す．
    auto group = mGroupDict.at(sig_str);
    return group;
  }
}

/// @brief 代表クラスを得る．
CiCellClass*
CgMgr::_find_class(
  const CgSignature& sig
)
{
  // 代表シグネチャの文字列を求める．
  auto sig_str = sig.str();
  // このシグネチャを持つクラスを探す．
  CiCellClass* rep_class = nullptr;
  if ( mClassDict.count(sig_str) == 0 ) {
    // 同位体変換リストを作る．
    auto idmap_list = sig.idmap_list();
    // 新しいクラスを作って登録する．
    rep_class = mLibrary.add_cell_class(sig.cell_type(),
					sig.seq_attr(),
					idmap_list);
    mClassDict.emplace(sig_str, rep_class);
    mExprListDict.emplace(rep_class, std::vector<Expr>{});
    auto cell_type = rep_class->cell_type();
    if ( cell_type == ClibCellType::FF ) {
      auto seq_attr = rep_class->seq_attr();
      auto index = seq_attr.index();
      if ( mFFClassDict.count(index) == 0 ) {
	mFFClassDict.emplace(index, std::vector<const CiCellClass*>{rep_class});
      }
      else {
	mFFClassDict.at(index).push_back(rep_class);
      }
    }
    else if ( cell_type == ClibCellType::Latch ) {
      auto seq_attr = rep_class->seq_attr();
      auto index = seq_attr.index();
      if ( mLatchClassDict.count(index) == 0 ) {
	mLatchClassDict.emplace(index, std::vector<const CiCellClass*>{rep_class});
      }
      else {
	mLatchClassDict.at(index).push_back(rep_class);
      }
    }
  }
  else {
    rep_class = mClassDict.at(sig_str);
  }

  // 単純な論理セルの場合，パタングラフ用の論理式を登録する．
  auto expr = sig.expr();
  if ( expr.is_valid() && expr.input_size() >= 2 ) {
    mExprListDict.at(rep_class).push_back(expr);
  }

  return rep_class;
}

// @brief パタングラフを生成する．
void
CgMgr::gen_pat()
{
  for ( auto rep_class: mLibrary.npn_class_list() ) {
    bool has_cell = false;
    for ( auto group: rep_class->cell_group_list() ) {
      if ( group->cell_num() > 0 ) {
	has_cell = true;
	break;
      }
    }
    if ( has_cell ) {
      const auto& expr_list = mExprListDict.at(rep_class);
      for ( const auto& expr: expr_list ) {
	mPatMgr.reg_pat(expr, rep_class);
      }
    }
  }
}

// @brief 全ノード数を返す．
SizeType
CgMgr::pat_node_num() const
{
  return mPatMgr.node_num();
}

// @brief ノードを返す．
const PatNode&
CgMgr::pat_node(
  SizeType pos
) const
{
  return mPatMgr.node(pos);
}

// @brief パタン数を返す．
SizeType
CgMgr::pat_num() const
{
  return mPatMgr.pat_num();
}

// @brief パタンのノードリストを返す．
void
CgMgr::get_pat_info(
  SizeType id,
  const CiCellClass*& rep_class,
  SizeType& input_num,
  std::vector<SizeType>& node_list
) const
{
  rep_class = mPatMgr.rep_class(id);
  mPatMgr.get_pat_info(id, input_num, node_list);
}

END_NAMESPACE_YM_CLIB
