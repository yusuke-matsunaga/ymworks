
/// @file FuncImpl_Bdd.cc
/// @brief FuncImpl_Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "FuncImpl_Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl
//////////////////////////////////////////////////////////////////////

// @brief BDD型のインスタンスを作る．
FuncImpl*
FuncImpl::new_bdd(
  const ModelImpl* model,
  SizeType id,
  const Bdd& bdd
)
{
  return new FuncImpl_Bdd(model, id, bdd);
}


//////////////////////////////////////////////////////////////////////
// クラス FuncImpl_Bdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncImpl_Bdd::FuncImpl_Bdd(
  const ModelImpl* model, ///< [in] 親のモデル
  SizeType id,
  Bdd bdd
) : FuncImpl(model, id),
    mBdd{bdd}
{
}

// @brief 関数の種類を返す．
BnFunc::Type
FuncImpl_Bdd::type() const
{
  return BnFunc::BDD;
}

// @brief BDD型の時 true を返す．
bool
FuncImpl_Bdd::is_bdd() const
{
  return true;
}

// @brief 入力数を返す．
SizeType
FuncImpl_Bdd::input_num() const
{
  if ( mBdd.is_zero() || mBdd.is_one() ) {
    return 0;
  }
  return mBdd.support_size();
}

// @brief BDDを返す．
Bdd
FuncImpl_Bdd::bdd() const
{
  return mBdd;
}

// @brief コピーを作る．
std::unique_ptr<FuncImpl>
FuncImpl_Bdd::copy(
  BddMgr& bdd_mgr
) const
{
  auto my_bdd = bdd_mgr.copy(mBdd);
  return std::unique_ptr<FuncImpl>{new FuncImpl_Bdd{_model(), id(), my_bdd}};
}

// @brief ハッシュ用のユニークな文字列を返す．
std::string
FuncImpl_Bdd::signature() const
{
  auto data_list = rep_data(mBdd);
  std::ostringstream buf;
  buf << "b";
  for ( auto data: data_list ) {
    buf << ":" << data;
  }
  return buf.str();
}

// @brief 内容を出力する．
void
FuncImpl_Bdd::print(
  std::ostream& s
) const
{
  s << "BDD" << std::endl;
  display(s, mBdd);
}

END_NAMESPACE_YM_BN
