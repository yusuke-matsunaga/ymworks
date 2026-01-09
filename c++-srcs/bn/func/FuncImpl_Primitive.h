#ifndef FUNCIMPL_PRIMITIVE_H
#define FUNCIMPL_PRIMITIVE_H

/// @file FuncImpl_Primitive.h
/// @brief FuncImpl_Primitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncImpl_Primitive FuncImpl_Primitive.h "FuncImpl_Primitive.h"
/// @brief プリミティブ型の関数情報を表すクラス
//////////////////////////////////////////////////////////////////////
class FuncImpl_Primitive :
  public FuncImpl
{
public:

  /// @brief コンストラクタ
  FuncImpl_Primitive(
    SizeType input_num,     ///< [in] 入力数
    PrimType primitive_type ///< [in] プリミティブの種類
  );

  /// @brief デストラクタ
  ~FuncImpl_Primitive() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の種類を返す．
  BnFunc::Type
  type() const override;

  /// @brief プリミティブ型の時 true を返す．
  bool
  is_primitive() const override;

  /// @brief 入力数を返す．
  SizeType
  input_num() const override;

  /// @brief プリミティブの種類を返す．
  PrimType
  primitive_type() const override;

  /// @brief コピーを作る．
  std::unique_ptr<FuncImpl>
  copy(
    BddMgr& bdd_mgr ///< [in] BddMgr
  ) const override;

  /// @brief ハッシュ用のユニークな文字列を返す．
  std::string
  signature() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum;

  // プリミティブの型
  PrimType mPrimType;

};

END_NAMESPACE_YM_BN

#endif // FUNCIMPL_PRIMITIVE_H
