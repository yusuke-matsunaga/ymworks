#ifndef BDDMGR_H
#define BDDMGR_H

/// @file BddMgr.h
/// @brief BddMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/dd.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgrHolder.h"
#include "ym/Expr.h"
#include "ym/JsonValue.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddMgr BddMgr.h "ym/BddMgr.h"
/// @ingroup DdGroup
/// @brief BDD を管理するためのクラス
///
/// - Bdd の演算は同一の BddMgr に属している Bdd の間でのみ有効
/// - 異なる BddMgr に属する Bdd の演算は std::invalid_argument
///   例外を送出する．
/// - BddMgr は変数順を保持しており，異なる BddMgr 間では変数順が
///   異なる可能性がある．
///
/// - 実体は BddMgrImpl でこのクラスはただの shared pointer となっている．
///   コピーしても同一の BddMgrImpl を持つインスタンスが生成される．
/// - BddMgrImpl は内部で参照回数を持っており，参照回数がゼロになると
///   自動的に開放される．
/// - BddMgrImpl は BddMgr だけでなく Bdd なども所有している．
//////////////////////////////////////////////////////////////////////
class BddMgr :
  public BddMgrHolder
{
public:

  /// @brief コンストラクタ
  BddMgr();

  /// @brief BddMgrHolder を指定したコンストラクタ
  /// @sa BddMgrHolder
  explicit
  BddMgr(
    const BddMgrHolder& holder
  );

  /// @brief コピーコンストラクタ
  BddMgr(
    const BddMgr& src
  );

  /// @brief デストラクタ
  ~BddMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 変数に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の数を返す．
  SizeType
  variable_num() const;

  /// @brief 変数を返す．
  /// @sa BddVar
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  ///
  /// @code
  /// // mgr は BddMgr のインスタンス
  /// auto var = mgr.variable(id);
  /// // var.id() == id が成り立つ．
  /// @endcode
  BddVar
  variable(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 変数を表すBDDを返す．
  /// @sa Bdd
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  ///
  /// @code
  /// // mgr は BddMgr のインスタンス
  /// auto var = mgr.variable(id);
  /// auto var_bdd = mgr.variable_bdd(id);
  /// // var.bdd() = var_bdd が成り立つ
  /// @endcode
  Bdd
  variable_bdd(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief リテラルを返す．
  /// @sa BddLit
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  ///
  /// @code
  /// // mgr は BddMgr のインスタンス
  /// auto lit = literal(id, inv);
  /// // lit.var().id() == id が成り立つ．
  /// // lit.is_negative() = inv が成り立つ．
  /// @endcode
  BddLit
  literal(
    SizeType varid,  ///< [in] 変数番号
    bool inv = false ///< [in] 反転属性
  );

  /// @brief リテラルを表すBDDを返す．
  /// @sa Bdd
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  ///
  /// @code
  /// // mgr は BddMgr のインスタンス
  /// auto lit = literal(id, inv);
  /// auto lit_bdd = literal_bdd(id, inv);
  /// // lit.bdd() = lit_bdd が成り立つ．
  /// @endcode
  Bdd
  literal_bdd(
    SizeType varid,  ///< [in] 変数番号
    bool inv = false ///< [in] 反転属性
  );

  /// @brief 正のリテラルを返す．
  /// @sa BddLit
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  ///
  /// @code
  /// // mgr は BddMgr のインスタンス
  /// auto lit = positive_literal(id);
  /// // lit.var().id() == id が成り立つ．
  /// // lit.is_positive() = true が成り立つ．
  /// @endcode
  BddLit
  positive_literal(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 正のリテラルを表すBDDを返す．
  /// @sa Bdd
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  ///
  /// @code
  /// // mgr は BddMgr のインスタンス
  /// auto lit = positive_literal(id);
  /// auto lit_bdd = positive_literal_bdd(id);
  /// // lit.bdd() == lit_bdd が成り立つ．
  /// @endcode
  Bdd
  positive_literal_bdd(
    SizeType varid ///< [in] 変数番号
  )
  {
    return literal_bdd(varid);
  }

  /// @brief 負のリテラルを返す．
  /// @sa BddLit
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  ///
  /// @code
  /// // mgr は BddMgr のインスタンス
  /// auto lit = negative_literal(id);
  /// // lit.var().id() == id が成り立つ．
  /// // lit.is_negative() = true が成り立つ．
  /// @endcode
  BddLit
  negative_literal(
    SizeType varid ///< [in] 変数番号
  );

  /// @brief 負のリテラルを表すBDDを返す．
  ///
  /// - varid >= variable_num() の場合には新しい変数が確保される．
  ///
  /// @code
  /// // mgr は BddMgr のインスタンス
  /// auto lit = negative_literal(id);
  /// auto lit_bdd = negative_literal_bdd(id);
  /// // lit.bdd() == lit_bdd が成り立つ．
  /// @endcode
  Bdd
  negative_literal_bdd(
    SizeType varid ///< [in] 変数番号
  )
  {
    return literal_bdd(varid, true);
  }

  /// @brief 変数のリストを返す．
  /// @sa BddVar
  ///
  /// 変数番号の昇順に並んでいる
  std::vector<BddVar>
  variable_list() const;

  /// @brief 変数順を表す変数のリストを返す．
  /// @sa BddVar
  ///
  /// インデックスの昇順に並んでいる
  std::vector<BddVar>
  variable_order() const;

  /// @brief 変数順を設定する．
  /// @sa BddVar
  void
  set_variable_order(
    const std::vector<BddVar>& order_list ///< [in] 変数順を表すリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // BDD を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD をコピーする．
  /// @sa Bdd
  ///
  /// 通常は同じものを返すが，src のマネージャが異なる場合には
  /// 同じ構造のコピーを作る．
  ///
  /// ただし，変数順が異なる場合には効率の悪い処理となる．
  Bdd
  copy(
    const Bdd& src
  );

  /// @brief 真理値表形式の文字列からBDDを作る．
  /// @sa Bdd, BddVar
  ///
  /// - str は '0' か '1' の文字列．
  /// - ただし，長さはvar_listのサイズのべき乗である必要がある．
  /// - for some reason, この文字列は big endian となっている．
  /// - 0文字目が(1, 1, 1, 1)に対応する
  /// - var_list が省略された場合は自動的に適切な変数リストを用いる．
  /// - 不正な形式の場合は std::invalid_argument 例外を送出する．
  Bdd
  from_truth(
    const std::string& str,             ///< [in] 01の文字列
    const std::vector<BddVar>& var_list ///< [in] 変数のリスト
    = {}
  );

  /// @brief 論理式から BDD を作る．
  /// @sa Bdd, BddVar, Expr
  ///
  /// - var_list が省略された場合は自動的に適切な変数リストを用いる．
  Bdd
  from_expr(
    const Expr& expr,                   ///< [in] 論理式
    const std::vector<BddVar>& var_list ///< [in] 変数のリスト
    = {}
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 複数の BDD の情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリダンプから復元する．
  /// @return 生成されたBDDのリストを返す．
  /// @sa Bdd, BinDec
  ///
  /// 不正な形式の場合は std::invalid_argument 例外を送出する．
  std::vector<Bdd>
  restore(
    BinDec& s ///< [in] 入力ストリーム
  );


public:
  //////////////////////////////////////////////////////////////////////
  // マネージャの諸元に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return _has_valid_mgr();
  }

  /// @brief ガーベージコレクションを行う．
  void
  garbage_collection();

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief GC を起動するしきい値を返す．
  SizeType
  gc_limit() const;

  /// @brief GC を起動するしきい値を設定する．
  void
  set_gc_limit(
    SizeType limit
  );

  /// @brief GC を許可する．
  ///
  /// ただちに GC が起動するわけではない．
  void
  enable_gc();

  /// @brief GC を禁止する．
  void
  disable_gc();

  /// @brief 等価比較演算子
  bool
  operator==(
    const BddMgr& right
  ) const
  {
    return get() == right.get();
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BddMgr& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝のリストをBddのリストに変換する．
  std::vector<Bdd>
  conv_to_bddlist(
    const std::vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;

  /// @brief 枝のリストを変数のリストに変換する．
  std::vector<BddVar>
  conv_to_varlist(
    const std::vector<DdEdge>& edge_list ///< [in] 枝のリスト
  ) const;

};

END_NAMESPACE_YM_DD

#endif // BDDMGR_H
