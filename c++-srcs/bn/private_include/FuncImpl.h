#ifndef FUNCIMPL_H
#define FUNCIMPL_H

/// @file FuncImpl.h
/// @brief FuncImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"
#include "ym/BnFunc.h"
#include "ym/BddMgr.h"
#include "ImplBase.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncImpl FuncImpl.h "FuncImpl.h"
/// @brief BnFunc の中身を表すクラス
///
/// - 実際には個々のタイプごとに固有の情報を持つ．
/// - このクラスは仮想関数の宣言のみ行う．
/// - ただし，派生クラスの生成関数をクラスメソッドとして用意したので
///   個々の派生クラスについて知る必要はない．
//////////////////////////////////////////////////////////////////////
class FuncImpl:
  public ImplBase
{
public:

  /// @brief コンストラクタ
  FuncImpl(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id             ///< [in] ID番号
  ) : ImplBase(model),
      mId{id}
  {
  }

  /// @brief デストラクタ
  virtual
  ~FuncImpl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブ型のインスタンスを作る．
  static
  FuncImpl*
  new_primitive(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id,            ///< [in] ID番号
    SizeType input_num,     ///< [in] 入力数
    PrimType primitive_type ///< [in] プリミティブの種類
  );

  /// @brief カバー型のインスタンスを作る．
  static
  FuncImpl*
  new_cover(
    const ModelImpl* model,      ///< [in] 親のモデル
    SizeType id,                 ///< [in] ID番号
    const SopCover& input_cover, ///< [in] 入力カバー
    bool output_inv              ///< [in] 出力の反転属性
  );

  /// @brief 論理式型のインスタンスを作る．
  static
  FuncImpl*
  new_expr(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id,            ///< [in] ID番号
    const Expr& expr        ///< [in] 論理式
  );

  /// @brief 真理値表型のインスタンスを作る．
  static
  FuncImpl*
  new_tvfunc(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id,            ///< [in] ID番号
    const TvFunc& func      ///< [in] 真理値表
  );

  /// @brief BDD型のインスタンスを作る．
  static
  FuncImpl*
  new_bdd(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id,            ///< [in] ID番号
    const Bdd& bdd          ///< [in] BDD
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 種類を取り出す関数
  // この関数は例外を創出しない．
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 関数の種類を返す．
  virtual
  BnFunc::Type
  type() const = 0;

  /// @brief プリミティブ型の時 true を返す．
  virtual
  bool
  is_primitive() const;

  /// @brief カバー型の時 true を返す．
  virtual
  bool
  is_cover() const;

  /// @brief 論理式型の時 true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 真理値表型の時 true を返す．
  virtual
  bool
  is_tvfunc() const;

  /// @brief BDD型の時 true を返す．
  virtual
  bool
  is_bdd() const;

  /// @brief 入力数を返す．
  virtual
  SizeType
  input_num() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 個々の型に関連した情報を取り出す関数
  // この関数は場合によっては std::invalid_argument 例外を送出する．
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブ型を返す．
  ///
  /// - is_primitive() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  PrimType
  primitive_type() const;

  /// @brief 入力カバーを返す．
  ///
  /// - is_cover() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  const SopCover&
  input_cover() const;

  /// @brief 出力の反転属性を返す．
  ///
  /// - is_cover() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  bool
  output_inv() const;

  /// @brief 論理式を返す．
  ///
  /// - is_expr() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  Expr
  expr() const;

  /// @brief 真理値表を返す．
  ///
  /// - is_tvfunc() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  const TvFunc&
  tvfunc() const;

  /// @brief BDDを返す．
  ///
  /// - is_bdd() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  virtual
  Bdd
  bdd() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーを作る．
  virtual
  std::unique_ptr<FuncImpl>
  copy(
    BddMgr& bdd_mgr ///< [in] BddMgr 親のBDDマネージャ
  ) const = 0;

  /// @brief ハッシュ用のユニークな文字列を返す．
  virtual
  std::string
  signature() const = 0;

  /// @brief 内容を出力する．
  virtual
  void
  print(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

};

/// @brief FuncImpl* 用のハッシュ関数
struct FuncHash {
  SizeType
  operator()(
    const FuncImpl* func
  ) const
  {
    auto sig = func->signature();
    std::hash<std::string> str_hash;
    return str_hash(sig);
  }
};

/// @brief FuncImpl* 用の等価比較関数
struct FuncEq {
  bool
  operator()(
    const FuncImpl* left,
    const FuncImpl* right
  ) const
  {
    return left->signature() == right->signature();
  }
};

END_NAMESPACE_YM_BN

#endif // FUNCIMPL_H
