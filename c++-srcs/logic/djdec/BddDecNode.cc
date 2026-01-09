
/// @file BddDecNode.cc
/// @brief BddDecNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BddDecNode.h"
#include "BddDecNode_int.h"
#include "BddDecEdge.h"
#include "ym/BddMgr.h"
#include "ym/BddCube.h"


BEGIN_NAMESPACE_YM_DJDEC

//////////////////////////////////////////////////////////////////////
// クラス BddDecNode
//////////////////////////////////////////////////////////////////////

// @brief LITタイプのオブジェクトを生成する．
BddDecNode*
BddDecNode::new_lit(
  BddMgr& mgr,
  SizeType id,
  const Bdd& f,
  const BddVarSet& support
)
{
  return new BddDecNode_Lit(mgr, id, f, support);
}

// @brief ORタイプのオブジェクトを生成する．
BddDecNode*
BddDecNode::new_or(
  BddMgr& mgr,
  SizeType id,
  const Bdd& f,
  const BddVarSet& support,
  const std::vector<BddDecEdge>& child_list
)
{
  return new BddDecNode_Or(mgr, id, f, support, child_list);
}

// @brief XORタイプのオブジェクトを生成する．
BddDecNode*
BddDecNode::new_xor(
  BddMgr& mgr,
  SizeType id,
  const Bdd& f,
  const BddVarSet& support,
  const std::vector<BddDecEdge>& child_list
)
{
  return new BddDecNode_Xor(mgr, id, f, support, child_list);
}

// @brief CPLXタイプのオブジェクトを生成する．
BddDecNode*
BddDecNode::new_cplx(
  BddMgr& mgr,
  SizeType id,
  const Bdd& f,
  const BddVarSet& support,
  const std::vector<BddDecEdge>& child_list
)
{
  return new BddDecNode_Cplx(mgr, id, f, support, child_list);
}


//////////////////////////////////////////////////////////////////////
// クラス BddDecNode_Base
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BddDecNode_Base::BddDecNode_Base(
  BddMgr& mgr,
  SizeType id,
  const Bdd& f,
  const BddVarSet& support
) : mBddMgr{mgr},
    mId{id},
    mGlobalFunc{f},
    mSupport{support},
    mPat1{f.get_onepath()},
    mPat0{f.get_zeropath()}
{
}

// @brief ID番号を返す．
SizeType
BddDecNode_Base::id() const {
  return mId;
}

// @brief LITタイプの時に true を返す．
bool
BddDecNode_Base::is_lit() const
{
  return false;
}

// @brief ORタイプの時に true を返す．
bool
BddDecNode_Base::is_or() const
{
  return false;
}

// @brief XORタイプの時に true を返す．
bool
BddDecNode_Base::is_xor() const
{
  return false;
}

// @brief CPLXタイプの時に true を返す．
bool
BddDecNode_Base::is_cplx() const
{
  return false;
}

// @brief グローバル関数を返す．
Bdd
BddDecNode_Base::global_func() const
{
  return mGlobalFunc;
}

// @brief 先頭の変数を返す．
BddVar
BddDecNode_Base::top() const
{
  auto v_list = mSupport.var_list();
  return v_list.front();
}

// @brief サポートを返す．
const BddVarSet&
BddDecNode_Base::support() const
{
  return mSupport;
}

// @brief グローバル関数が1となるパタンを得る．
BddCube
BddDecNode_Base::pat_1() const
{
  return mPat1;
}

// @brief グローバル関数が0となるパタンを得る．
BddCube
BddDecNode_Base::pat_0() const
{
  return mPat0;
}

// @brief subfunction の数を得る．
SizeType
BddDecNode_Base::child_num() const
{
  return 0;
}

// @brief subfunction を表す枝を得る．
BddDecEdge
BddDecNode_Base::child(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return BddDecEdge::zero();
}

// @brief print の共通部分
void
BddDecNode_Base::print_base(
  std::ostream& s,
  const std::string& type
) const
{
  s << "#" << id() << ": " << type;
}


//////////////////////////////////////////////////////////////////////
// クラス BddDecNode_Lit
//////////////////////////////////////////////////////////////////////

// @brief LITタイプの時に true を返す．
bool
BddDecNode_Lit::is_lit() const
{
  return true;
}

// @brief ローカル関数を求める．
Bdd
BddDecNode_Lit::local_func() const
{
  auto f = mgr().literal(0);
  return f;
}

// @brief 内容を出力する．
void
BddDecNode_Lit::print(
  std::ostream& s
) const
{
  print_base(s, "LIT");
  s << "(" << top() << ")" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス BddDecNode_Mid
//////////////////////////////////////////////////////////////////////

// @brief subfunction の数を得る．
SizeType
BddDecNode_Mid::child_num() const
{
  return mChildList.size();
}

// @brief subfunction を表す枝を得る．
BddDecEdge
BddDecNode_Mid::child(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < child_num() );
  return mChildList[pos];
}

// @brief pint() の下請け処理を行う．
void
BddDecNode_Mid::print_sub(
  std::ostream& s,
  const std::string& type
) const
{
  print_base(s, type);
  s << "(";
  const char* comma = "";
  for ( SizeType i = 0; i < child_num(); ++ i ) {
    auto cedge = child(i);
    s << comma;
    comma = ", ";
    cedge.print(s);
  }
  s << ")" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス BddDecNode_Or
//////////////////////////////////////////////////////////////////////

// @brief ORタイプの時に true を返す．
bool
BddDecNode_Or::is_or() const
{
  return true;
}

// @brief ローカル関数を求める．
Bdd
BddDecNode_Or::local_func() const
{
  SizeType n = child_num();
  auto f = mgr().literal_bdd(0);
  for ( SizeType i = 1; i < n; ++ i ) {
    f |= mgr().literal_bdd(i);
  }
  return f;
}

// @brief 内容を出力する．
void
BddDecNode_Or::print(
  std::ostream& s
) const
{
  print_sub(s, "OR");
}


//////////////////////////////////////////////////////////////////////
// クラス BddDecNode_Xor
//////////////////////////////////////////////////////////////////////

// @brief XORタイプの時に true を返す．
bool
BddDecNode_Xor::is_xor() const
{
  return true;
}

// @brief ローカル関数を求める．
Bdd
BddDecNode_Xor::local_func() const
{
  SizeType n = child_num();
  auto f = mgr().literal_bdd(0);
  for ( SizeType i = 1; i < n; ++ i ) {
    f ^= mgr().literal_bdd(i);
  }
  return f;
}

// @brief 内容を出力する．
void
BddDecNode_Xor::print(
  std::ostream& s
) const
{
  print_sub(s, "XOR");
}


//////////////////////////////////////////////////////////////////////
// クラス BddDecNode_Cplx
//////////////////////////////////////////////////////////////////////

// @brief CPLXタイプの時に true を返す．
bool
BddDecNode_Cplx::is_cplx() const
{
  return true;
}

// @brief ローカル関数を求める．
Bdd
BddDecNode_Cplx::local_func() const
{
  std::unordered_map<BddVar, BddLit> varmap;
  auto gf = global_func();
  SizeType n = child_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto node = child(i).node();
    auto f = node->global_func();
    Bdd f0;
    Bdd f1;
    auto var = f.root_decomp(f0, f1);
    auto f_diff = f1 & ~f0;
    bool inv;
    if ( f_diff.is_zero() ) {
      f_diff = f0 & ~f1;
      inv = true;
    }
    else {
      inv = false;
    }
    auto pat = f_diff.get_onepath();
    gf /= pat;
    auto lit = mgr().literal(i, inv);
    varmap.emplace(var, lit);
  }
  return gf.remap_vars(varmap);
}

// @brief 内容を出力する．
void
BddDecNode_Cplx::print(
  std::ostream& s
) const
{
  print_sub(s, "CPLX");

  auto f = local_func();
  display(s, f);
}

END_NAMESPACE_YM_DJDEC
