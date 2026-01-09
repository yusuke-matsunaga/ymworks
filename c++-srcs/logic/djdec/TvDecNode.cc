
/// @file TvDecNode.cc
/// @brief TvDecNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TvDecNode.h"
#include "TvDecNode_int.h"
#include "TvDecEdge.h"


BEGIN_NAMESPACE_YM_DJDEC

//////////////////////////////////////////////////////////////////////
// クラス TvDecNode
//////////////////////////////////////////////////////////////////////

// @brief LITタイプのオブジェクトを生成する．
TvDecNode*
TvDecNode::new_lit(
  SizeType id,
  SizeType var
)
{
  return new TvDecNode_Lit(id, var);
}

// @brief ORタイプのオブジェクトを生成する．
TvDecNode*
TvDecNode::new_or(
  SizeType id,
  const std::vector<TvDecEdge>& child_list
)
{
  return new TvDecNode_Or(id, child_list);
}

// @brief XORタイプのオブジェクトを生成する．
TvDecNode*
TvDecNode::new_xor(
  SizeType id,
  const std::vector<TvDecEdge>& child_list
)
{
  return new TvDecNode_Xor(id, child_list);
}

// @brief CPLXタイプのオブジェクトを生成する．
TvDecNode*
TvDecNode::new_cplx(
  SizeType id,
  const TvFunc& local_func,
  const std::vector<TvDecEdge>& child_list
)
{
  return new TvDecNode_Cplx(id, local_func, child_list);
}

BEGIN_NONAMESPACE

SizeType
get_support_size(
  const std::vector<TvDecEdge>& edge_list
)
{
  SizeType max_size = 0;
  for ( auto edge: edge_list ) {
    auto& support = edge.node()->support();
    for ( auto x: support ) {
      max_size = std::max(max_size, x);
    }
  }
  return max_size + 1;
}

END_NONAMESPACE

// @brief ローカル関数を求める．
TvFunc
TvDecNode::local_func(
  const std::vector<TvDecEdge>& child_list
) const
{
  // 真理値表を作る．
  auto ni = child_list.size();
  auto ni_exp = 1 << ni;
  std::vector<int> val_list(ni_exp);
  // child_list のサポートサイズを求める．
  auto ns = get_support_size(child_list);
  auto gf = global_func(ns);
  std::vector<TvFunc> child_func(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    child_func[i] = child_list[i].global_func(ns);
  }
  auto ns_exp = 1 << ns;
  for ( SizeType index = 0; index < ns_exp; ++ index ) {
    auto val = gf.value(index);
    SizeType local_index = 0;
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( child_func[i].value(index) ) {
	local_index |= (1 << i);
      }
    }
    val_list[local_index] = val;
  }
  return TvFunc(ni, val_list);
}


//////////////////////////////////////////////////////////////////////
// クラス TvDecNode_Base
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TvDecNode_Base::TvDecNode_Base(
  SizeType id,
  const std::vector<SizeType>& support
) : mId{id},
    mSupport{support}
{
}

// @brief ID番号を返す．
SizeType
TvDecNode_Base::id() const {
  return mId;
}

// @brief LITタイプの時に true を返す．
bool
TvDecNode_Base::is_lit() const
{
  return false;
}

// @brief ORタイプの時に true を返す．
bool
TvDecNode_Base::is_or() const
{
  return false;
}

// @brief XORタイプの時に true を返す．
bool
TvDecNode_Base::is_xor() const
{
  return false;
}

// @brief CPLXタイプの時に true を返す．
bool
TvDecNode_Base::is_cplx() const
{
  return false;
}

// @brief 先頭の変数を返す．
SizeType
TvDecNode_Base::top_var() const
{
  return mSupport.front();
}

// @brief サポートを返す．
const std::vector<SizeType>&
TvDecNode_Base::support() const
{
  return mSupport;
}

// @brief subfunction の数を得る．
SizeType
TvDecNode_Base::child_num() const
{
  return 0;
}

// @brief subfunction を表す枝を得る．
TvDecEdge
TvDecNode_Base::child(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return TvDecEdge::zero();
}

// @brief print の共通部分
void
TvDecNode_Base::print_base(
  std::ostream& s,
  const std::string& type
) const
{
  s << "#" << id() << ": " << type;
}


//////////////////////////////////////////////////////////////////////
// クラス TvDecNode_Lit
//////////////////////////////////////////////////////////////////////

// @brief LITタイプの時に true を返す．
bool
TvDecNode_Lit::is_lit() const
{
  return true;
}

// @brief グローバル関数を返す．
TvFunc
TvDecNode_Lit::global_func(
  SizeType input_num
) const
{
  return TvFunc::literal(input_num, top_var());
}

// @brief ローカル関数を求める．
TvFunc
TvDecNode_Lit::local_func() const
{
  return TvFunc::literal(1, 0);
}

// @brief 内容を出力する．
void
TvDecNode_Lit::print(
  std::ostream& s
) const
{
  print_base(s, "LIT");
  s << "(" << top_var() << ")" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス TvDecNode_Mid
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

std::vector<SizeType>
merge(
  const std::vector<SizeType>& a_list,
  const std::vector<SizeType>& b_list
)
{
  auto a_iter = a_list.begin();
  auto a_end = a_list.end();
  auto b_iter = b_list.begin();
  auto b_end = b_list.end();
  std::vector<SizeType> ans_list;
  ans_list.reserve(a_list.size() + b_list.size());
  while ( a_iter != a_end && b_iter != b_end ) {
    auto a = *a_iter;
    auto b = *b_iter;
    if ( a < b ) {
      ans_list.push_back(a);
      ++ a_iter;
    }
    else if ( a > b ) {
      ans_list.push_back(b);
      ++ b_iter;
    }
    else {
      ans_list.push_back(a);
      ++ a_iter;
      ++ b_iter;
    }
  }
  for ( ; a_iter != a_end; ++ a_iter ) {
    auto a = *a_iter;
    ans_list.push_back(a);
  }
  for ( ; b_iter != b_end; ++ b_iter ) {
    auto b = *b_iter;
    ans_list.push_back(b);
  }
  return ans_list;
}

std::vector<SizeType>
get_support(
  const std::vector<TvDecEdge>& edge_list
)
{
  auto n = edge_list.size();
  if ( n == 0 ) {
    return {};
  }
  auto support = edge_list[0].support();
  for ( SizeType i = 1; i < n; ++ i ) {
    support = merge(support, edge_list[i].support());
  }
  return support;
}

END_NONAMESPACE

// @brief コンストラクタ
TvDecNode_Mid::TvDecNode_Mid(
  SizeType id,
  const std::vector<TvDecEdge>& child_list
) : TvDecNode_Base(id, get_support(child_list)),
    mChildList{child_list}
{
}

// @brief subfunction の数を得る．
SizeType
TvDecNode_Mid::child_num() const
{
  return mChildList.size();
}

// @brief subfunction を表す枝を得る．
TvDecEdge
TvDecNode_Mid::child(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < child_num() );
  return mChildList[pos];
}

// @brief pint() の下請け処理を行う．
void
TvDecNode_Mid::print_sub(
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
// クラス TvDecNode_Or
//////////////////////////////////////////////////////////////////////

// @brief ORタイプの時に true を返す．
bool
TvDecNode_Or::is_or() const
{
  return true;
}

// @brief グローバル関数を返す．
TvFunc
TvDecNode_Or::global_func(
  SizeType input_num
) const
{
  SizeType n = child_num();
  auto f = child(0).global_func(input_num);
  for ( SizeType i = 1; i < n; ++ i ) {
    f |= child(i).global_func(input_num);
  }
  return f;
}

// @brief ローカル関数を求める．
TvFunc
TvDecNode_Or::local_func() const
{
  SizeType n = child_num();
  auto f = TvFunc::literal(n, 0);
  for ( SizeType i = 1; i < n; ++ i ) {
    f |= TvFunc::literal(n, i);
  }
  return f;
}

// @brief 内容を出力する．
void
TvDecNode_Or::print(
  std::ostream& s
) const
{
  print_sub(s, "OR");
}


//////////////////////////////////////////////////////////////////////
// クラス TvDecNode_Xor
//////////////////////////////////////////////////////////////////////

// @brief XORタイプの時に true を返す．
bool
TvDecNode_Xor::is_xor() const
{
  return true;
}

// @brief グローバル関数を返す．
TvFunc
TvDecNode_Xor::global_func(
  SizeType input_num
) const
{
  SizeType n = child_num();
  auto f = child(0).global_func(input_num);
  for ( SizeType i = 1; i < n; ++ i ) {
    f ^= child(i).global_func(input_num);
  }
  return f;
}

// @brief ローカル関数を求める．
TvFunc
TvDecNode_Xor::local_func() const
{
  SizeType n = child_num();
  auto f = TvFunc::literal(n, 0);
  for ( SizeType i = 1; i < n; ++ i ) {
    f ^= TvFunc::literal(n, i);
  }
  return f;
}

// @brief 内容を出力する．
void
TvDecNode_Xor::print(
  std::ostream& s
) const
{
  print_sub(s, "XOR");
}


//////////////////////////////////////////////////////////////////////
// クラス TvDecNode_Cplx
//////////////////////////////////////////////////////////////////////

// @brief CPLXタイプの時に true を返す．
bool
TvDecNode_Cplx::is_cplx() const
{
  return true;
}

// @brief グローバル関数を返す．
TvFunc
TvDecNode_Cplx::global_func(
  SizeType input_num
) const
{
  // ベタなやり方
  auto ni_exp = 1 << input_num;
  std::vector<int> val_list(ni_exp, 0);
  SizeType nc = child_num();
  std::vector<TvFunc> lf_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    lf_list[i] = child(i).global_func(input_num);
  }
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    SizeType local_index = 0;
    for ( SizeType i = 0; i < nc; ++ i ) {
      if ( lf_list[i].value(b) ) {
	local_index |= (1 << i);
      }
    }
    val_list[b] = mLocalFunc.value(local_index);
  }
  return TvFunc(input_num, val_list);
}

// @brief ローカル関数を求める．
TvFunc
TvDecNode_Cplx::local_func() const
{
  return mLocalFunc;
}

// @brief 内容を出力する．
void
TvDecNode_Cplx::print(
  std::ostream& s
) const
{
  std::ostringstream buf;
  buf << "CPLX[" << local_func().hex_str() << "]";
  print_sub(s, buf.str());
}

END_NAMESPACE_YM_DJDEC
