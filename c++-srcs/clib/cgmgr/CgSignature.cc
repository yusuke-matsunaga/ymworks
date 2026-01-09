
/// @file CgSignature.cc
/// @brief CgSignature の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "cgmgr/CgSignature.h"
#include "ym/ClibIOMap.h"
#include "ym/ClibSeqAttr.h"
#include "ym/Expr.h"
#include "CgSigRep.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CgSignature
//
// 基本的にはただ CgSigRep の同名の関数を呼んでいるだけなのだが，
// CgSigRep.h が必要なのでヘッダファイル中では定義できない．
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
CgSignature::CgSignature()
{
}

// @brief コンストラクタ
CgSignature::CgSignature(
  std::unique_ptr<const CgSigRep>&& ptr
) : mRepPtr{std::move(ptr)}
{
}

// @brief ムーブコンストラクタ
CgSignature::CgSignature(
  CgSignature&& src
) : mRepPtr{std::move(src.mRepPtr)}
{
}

// @brief ムーブ代入演算子
CgSignature&
CgSignature::operator=(
  CgSignature&& src
)
{
  if ( this != &src ) {
    mRepPtr.swap(src.mRepPtr);
  }
  return *this;
}

// @brief デストラクタ
CgSignature::~CgSignature()
{
  // CgSigRep の定義をヘッダファイルから隠すために
  // ヘッダでは定義できない．
}

// @brief 1出力論理関数用のシグネチャを作る．
CgSignature
CgSignature::make_logic_sig(
  SizeType ni,
  const Expr& expr
)
{
  auto rep = new CgSigRep{ni, expr};
  return CgSignature{std::unique_ptr<CgSigRep>{rep}};
}

// @brief 一般的な組み合わせ回路用のシグネチャを作る．
CgSignature
CgSignature::make_logic_sig(
  SizeType ni,
  SizeType no,
  SizeType nb,
  const std::vector<TvFunc>& func_list,
  const std::vector<TvFunc>& tristate_list
)
{
  auto rep = new CgSigRep{ni, no, nb,
			  func_list,
			  tristate_list};
  return CgSignature{std::unique_ptr<CgSigRep>{rep}};
}

// @brief FF用のシグネチャを作る．
CgSignature
CgSignature::make_ff_sig(
  SizeType ni,
  SizeType no,
  SizeType nb,
  const std::vector<TvFunc>& func_list,
  const std::vector<TvFunc>& tristate_list,
  const TvFunc& clocked_on,
  const TvFunc& clocked_on_also,
  const TvFunc& next_state,
  const TvFunc& clear,
  const TvFunc& preset,
  ClibSeqAttr seq_attr
)
{
  SizeType ni2 = ni + nb;
  SizeType no2 = no + nb;
  SizeType xni = ni2 + 2;
  SizeType xno = no2 + 5;
  std::vector<TvFunc> sig_func_list(xno, TvFunc::invalid());
  std::vector<TvFunc> sig_tristate_list(xno, TvFunc::invalid());
  for ( SizeType i = 0; i < no2; ++ i ) {
    sig_func_list[i] = func_list[i];
    sig_tristate_list[i] = tristate_list[i];
  }
  sig_func_list[no2 + 0] = clocked_on;
  sig_func_list[no2 + 1] = clocked_on_also;
  sig_func_list[no2 + 2] = next_state;
  sig_func_list[no2 + 3] = clear;
  sig_func_list[no2 + 4] = preset;
  auto rep = new CgSigRep{ClibCellType::FF, seq_attr,
			  ni, no, nb,
			  sig_func_list,
			  sig_tristate_list};
  return CgSignature{std::unique_ptr<CgSigRep>{rep}};
}

// @brief ラッチ用のシグネチャを作る．
CgSignature
CgSignature::make_latch_sig(
  SizeType ni,
  SizeType no,
  SizeType nb,
  const std::vector<TvFunc>& func_list,
  const std::vector<TvFunc>& tristate_list,
  const TvFunc& enable_on,
  const TvFunc& enable_on_also,
  const TvFunc& data_in,
  const TvFunc& clear,
  const TvFunc& preset,
  ClibSeqAttr seq_attr
)
{
  SizeType ni2 = ni + nb;
  SizeType no2 = no + nb;
  SizeType xni = ni2 + 2;
  SizeType xno = no2 + 5;
  std::vector<TvFunc> sig_func_list(xno, TvFunc::invalid());
  std::vector<TvFunc> sig_tristate_list(xno, TvFunc::invalid());
  for ( SizeType i = 0; i < no2; ++ i ) {
    sig_func_list[i] = func_list[i];
    sig_tristate_list[i] = tristate_list[i];
  }
  sig_func_list[no2 + 0] = enable_on;
  sig_func_list[no2 + 1] = enable_on_also;
  sig_func_list[no2 + 2] = data_in;
  sig_func_list[no2 + 3] = clear;
  sig_func_list[no2 + 4] = preset;
  auto rep = new CgSigRep{ClibCellType::Latch, seq_attr,
			  ni, no, nb,
			  sig_func_list,
			  sig_tristate_list};
  return CgSignature{std::unique_ptr<CgSigRep>{rep}};
}

// @brief シグネチャ文字列を返す．
std::string
CgSignature::str() const
{
  if ( is_valid() ) {
    return mRepPtr->str();
  }
  return std::string{};
}

// @brief セルの種類を返す．
ClibCellType
CgSignature::cell_type() const
{
  if ( is_valid() ) {
    return mRepPtr->cell_type();
  }
  return ClibCellType::none;
}

// @brief 順序セルの属性を返す．
ClibSeqAttr
CgSignature::seq_attr() const
{
  if ( is_valid() ) {
    return mRepPtr->seq_attr();
  }
  return ClibSeqAttr{};
}

// @brief 単一の論理式を持つ時その式を返す．
Expr
CgSignature::expr() const
{
  if ( is_valid() ) {
    return mRepPtr->expr();
  }
  else {
    return Expr::invalid();
  }
}

// @brief 変換を施したシグネチャを返す．
CgSignature
CgSignature::xform(
  const ClibIOMap& iomap ///< [in] 変換マップ
) const
{
  CgSignature ans;
  if ( is_valid() ) {
    auto tmp_ptr = mRepPtr->xform(iomap);
    ans.mRepPtr.swap(tmp_ptr);
  }
  return ans;
}

// @brief 代表シグネチャに対する変換を求める．
ClibIOMap
CgSignature::rep_map() const
{
  if ( is_valid() ) {
    return mRepPtr->rep_map();
  }
  else {
    return ClibIOMap{};
  }
}

// @brief 同位体変換のリストを求める．
std::vector<ClibIOMap>
CgSignature::idmap_list() const
{
  if ( is_valid() ) {
    return mRepPtr->idmap_list();
  }
  else {
    return std::vector<ClibIOMap>{};
  }
}

END_NAMESPACE_YM_CLIB
