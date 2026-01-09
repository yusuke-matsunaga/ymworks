
/// @file FuncImpl_Primitive.cc
/// @brief FuncImpl_Primitive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "FuncImpl_Primitive.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl
//////////////////////////////////////////////////////////////////////

// @brief プリミティブ型のインスタンスを作る．
FuncImpl*
FuncImpl::new_primitive(
  SizeType input_num,
  PrimType primitive_type
)
{
  return new FuncImpl_Primitive(input_num, primitive_type);
}


//////////////////////////////////////////////////////////////////////
// クラス FuncImpl_Primitive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncImpl_Primitive::FuncImpl_Primitive(
  SizeType input_num,
  PrimType primitive_type
) : mInputNum{input_num},
    mPrimType{primitive_type}
{
}

// @brief 関数の種類を返す．
BnFunc::Type
FuncImpl_Primitive::type() const
{
  return BnFunc::PRIMITIVE;
}

// @brief プリミティブ型の時 true を返す．
bool
FuncImpl_Primitive::is_primitive() const
{
  return true;
}

// @brief 入力数を返す．
SizeType
FuncImpl_Primitive::input_num() const
{
  return mInputNum;
}

// @brief プリミティブの種類を返す．
PrimType
FuncImpl_Primitive::primitive_type() const
{
  return mPrimType;
}

// @brief コピーを作る．
std::unique_ptr<FuncImpl>
FuncImpl_Primitive::copy(
  BddMgr& bdd_mgr
) const
{
  return std::unique_ptr<FuncImpl>{new FuncImpl_Primitive{*this}};
}

// @brief ハッシュ用のユニークな文字列を返す．
std::string
FuncImpl_Primitive::signature() const
{
  std::ostringstream buf;
  buf << "p" << mPrimType << "[" << mInputNum << "]";
  return buf.str();
}

// @brief 内容を出力する．
void
FuncImpl_Primitive::print(
  std::ostream& s
) const
{
  s << "Primitive: " << mPrimType
    << "(" << mInputNum << ")" << std::endl;
}

END_NAMESPACE_YM_BN
