#ifndef BNFUNC_H
#define BNFUNC_H

/// @file BnFunc.h
/// @brief BnFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_BN

class FuncImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnFunc BnFunc.h "BnFunc.h"
/// @brief BnModel に登録された関数情報を表すクラス
///
/// BnNode の局所関数を表すことを目的としている．
///
/// 共通の属性として入力数を持つ．
///
/// 以下のタイプがある．
///
/// - プリミティブ型(PRIMITIVE)
///   PrimType を持つ．
///   同じ型でも入力数が異なれば別の関数と考える．
/// - カバー型(COVER)
///   SopCover と出力の反転属性を持つ．
/// - 論理式型(EXPR)
///   Expr を持つ．
/// - 真理値表型(TVFUNC)
///   TvFunc を持つ．
/// - BDD型(BDD)
///   Bdd を持つ．
//////////////////////////////////////////////////////////////////////
class BnFunc
{
  friend class BnModel;
  friend class BnNode;

public:
  //////////////////////////////////////////////////////////////////////
  // BnFunc で用いられるデータ型
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を表す列挙型
  enum Type {
    NONE,      ///< [in] 不正値
    PRIMITIVE, ///< [in] プリミティブ型
    COVER,     ///< [in] カバー型
    EXPR,      ///< [in] 論理式型
    TVFUNC,    ///< [in] 真理値表型
    BDD        ///< [in] BDD型
  };


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnFunc() = default;

  /// @brief コピーコンストラクタ
  BnFunc(
    const BnFunc& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BnFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通なインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持つ時 true を返す．
  bool
  is_valid() const
  {
    return mPtr != nullptr;
  }

  /// @brief 適正な値を持たないとき true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 関数番号を返す．
  SizeType
  id() const;

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 種類を返す．
  Type
  type() const;

  /// @brief 種類を表す文字列を返す．
  std::string
  type_str() const;

  /// @brief プリミティブ型の時 true を返す．
  bool
  is_primitive() const;

  /// @brief カバー型の時 true を返す．
  bool
  is_cover() const;

  /// @brief 論理式型の時 true を返す．
  bool
  is_expr() const;

  /// @brief 真理値表型の時 true を返す．
  bool
  is_tvfunc() const;

  /// @brief BDD型の時 true を返す．
  bool
  is_bdd() const;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // プリミティブ型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブ型を返す．
  PrimType
  primitive_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // カバー型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カバーを返す．
  const SopCover&
  input_cover() const;

  /// @brief 出力の反転属性を返す．
  bool
  output_inv() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理式型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理式を返す．
  /// has_expr() == false の時は std::invalid_argument 例外を送出する．
  Expr
  expr() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 真理値表型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 真理値表を返す．
  const TvFunc&
  tvfunc() const;


public:
  //////////////////////////////////////////////////////////////////////
  // BDD型のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BDDを返す．
  Bdd
  bdd() const;



public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnFunc& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnFunc& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BnFunc(
    const FuncImpl* ptr ///< [in] 実装オブジェクトのポインタ
  );

  /// @brief 実装を取り出す．
  const FuncImpl&
  _func_impl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装オブジェクトのポインタ
  const FuncImpl* mPtr{nullptr};

};

END_NAMESPACE_YM_BN

#endif // BNFUNC_H
