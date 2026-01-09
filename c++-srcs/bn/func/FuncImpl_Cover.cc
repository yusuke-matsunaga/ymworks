
/// @file FuncImpl_Cover.cc
/// @brief FuncImpl_Cover の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "FuncImpl_Cover.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl
//////////////////////////////////////////////////////////////////////

// @brief カバー型のインスタンスを作る．
FuncImpl*
FuncImpl::new_cover(
  const SopCover& input_cover,
  bool output_inv
)
{
  return new FuncImpl_Cover(input_cover, output_inv);
}


//////////////////////////////////////////////////////////////////////
// クラス FuncImpl_Cover
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncImpl_Cover::FuncImpl_Cover(
  const SopCover& input_cover,
  bool output_inv
) : mInputCover{input_cover},
    mOutputInv{output_inv}
{
}

// @brief デストラクタ
FuncImpl_Cover::~FuncImpl_Cover()
{
}

// @brief 関数の種類を返す．
BnFunc::Type
FuncImpl_Cover::type() const
{
  return BnFunc::COVER;
}

// @brief カバー型の時 true を返す．
bool
FuncImpl_Cover::is_cover() const
{
  return true;
}

// @brief 入力数を返す．
SizeType
FuncImpl_Cover::input_num() const
{
  return mInputCover.variable_num();
}

// @brief 入力カバーを返す．
const SopCover&
FuncImpl_Cover::input_cover() const
{
  return mInputCover;
}

// @brief 出力の反転属性を返す．
bool
FuncImpl_Cover::output_inv() const
{
  return mOutputInv;
}

// @brief コピーを作る．
std::unique_ptr<FuncImpl>
FuncImpl_Cover::copy(
  BddMgr& bdd_mgr
) const
{
  return std::unique_ptr<FuncImpl>{new FuncImpl_Cover{*this}};
}

// @brief ハッシュ用のユニークな文字列を返す．
std::string
FuncImpl_Cover::signature() const
{
  std::ostringstream buf;
  buf << "c" << mInputCover.variable_num() << ":";
  auto nc = mInputCover.cube_num();
  auto ni = mInputCover.variable_num();
  for ( SizeType c = 0; c < nc; ++ c ) {
    for ( SizeType i = 0; i < ni; ++ i ) {
      buf << mInputCover.get_pat(c, i);
    }
  }
  buf << ":" << mOutputInv;
  return buf.str();
}

// @brief 内容を出力する．
void
FuncImpl_Cover::print(
  std::ostream& s
) const
{
  auto nc = mInputCover.cube_num();
  auto ni = mInputCover.variable_num();
  s << "Cover" << std::endl;
  for ( SizeType c = 0; c < nc; ++ c ) {
    for ( SizeType i = 0; i < ni; ++ i ) {
      s << mInputCover.get_pat(c, i);
    }
    if ( ni > 0 ) {
      s << ' ';
    }
    if ( mOutputInv ) {
      s << '0';
    }
    else {
      s << '1';
    }
    s << std::endl;
  }
}

END_NAMESPACE_YM_BN
