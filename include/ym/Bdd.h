#ifndef BDD_H
#define BDD_H

/// @file Bdd.h
/// @brief Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddBase.h"
#include "ym/SopCover.h"
#include "ym/BinEnc.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class Bdd Bdd.h "ym/Bdd.h"
/// @brief BDD を表すクラス
///
/// - 基本的に個々の Bdd は一つの BddMgr に属す．
/// - 例外は invalid な Bdd で関連する BddMgr を持たない．
/// - Bdd 間の演算は同じ BddMgr に属するもののみ可とする．
/// - 異なる BddMgr に属する Bdd の演算は std::invalid_argument 例外を
///   送出する．
//////////////////////////////////////////////////////////////////////
class Bdd :
  public BddBase
{
  friend class BddMgrHolder;

public:

  /// @brief compose 用の変換マップの型
  using ComposeMap = std::unordered_map<BddVar, Bdd>;

  /// @brief remap_vars 用の変換マップの型
  using VarMap = std::unordered_map<BddVar, BddLit>;


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  Bdd() = default;

  /// @brief BddBase(の派生クラス)からの変換コンストラクタ
  Bdd(
    const BddBase& src
  );

  /// @brief デストラクタ
  ~Bdd();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name オブジェクト生成用のクラスメソッド
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 恒偽関数を作る．
  static
  Bdd
  zero();

  /// @brief 恒真関数を作る．
  static
  Bdd
  one();

  /// @brief 不正な値を作る．
  static
  Bdd
  invalid();

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 否定した関数を返す．
  /// @return 結果を返す．
  Bdd
  invert() const;

  /// @brief invert の別名
  /// @return 結果を返す．
  Bdd
  operator~() const
  {
    return invert();
  }

  /// @brief 極性をかけ合わせる．
  Bdd
  operator*(
    bool inv ///< [in] 反転フラグ
  ) const;

  /// @brief 論理積を返す．
  /// @return 結果を返す．
  Bdd
  and_op(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return _and_op(*this, right);
  }

  /// @brief and_op の別名
  /// @return 結果を返す．
  Bdd
  operator&(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return and_op(right);
  }

  /// @brief 論理和を返す．
  /// @return 結果を返す．
  Bdd
  or_op(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return _or_op(*this, right);
  }

  /// @brief or_op の別名
  /// @return 結果を返す．
  Bdd
  operator|(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return or_op(right);
  }

  /// @brief 排他的論理和を返す．
  /// @return 結果を返す．
  Bdd
  xor_op(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return _xor_op(*this, right);
  }

  /// @brief xor_op の別名
  /// @return 結果を返す．
  Bdd
  operator^(
    const Bdd& right ///< [in] オペランド
  ) const
  {
    return xor_op(right);
  }

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    const BddVar& var, ///< [in] 変数
    bool inv           ///< [in] 反転フラグ
                       ///<      - false: 反転なし (正極性)
                       ///<      - true:  反転あり (負極性)
  ) const;

  /// @brief コファクターを計算する．
  /// @return 結果を返す．
  Bdd
  cofactor(
    const BddLit& lit ///< [in] リテラル
  ) const;

  /// @brief cofactor の別名
  Bdd
  operator/(
    const BddLit& lit ///< [in] リテラル
  ) const
  {
    return cofactor(lit);
  }

  /// @brief コファクターを計算する．
  Bdd
  cofactor(
    const BddCube& cube ///< [in] コファクターのキューブ
  ) const;

  /// @brief cofactor の別名
  Bdd
  operator/(
    const BddCube& cube ///< [in] コファクターのキューブ
  ) const
  {
    return cofactor(cube);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算を伴った代入
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自分自身への参照を返す．
  Bdd&
  invert_int();

  /// @brief 極性をかけ合わせて代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  operator*=(
    bool inv ///< [in] 反転フラグ
  );

  /// @brief 論理積を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  and_int(
    const Bdd& right ///< [in] オペランド
  )
  {
    *this = and_op(right);
    return *this;
  }

  /// @brief and_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator&=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return and_int(right);
  }

  /// @brief 論理和を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  or_int(
    const Bdd& right ///< [in] オペランド
  )
  {
    *this = or_op(right);
    return *this;
  }

  /// @brief or_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator|=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return or_int(right);
  }

  /// @brief 排他的論理和を計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  xor_int(
    const Bdd& right ///< [in] オペランド
  )
  {
    *this = xor_op(right);
    return *this;
  }

  /// @brief xor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator^=(
    const Bdd& right ///< [in] オペランド
  )
  {
    return xor_int(right);
  }

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    const BddVar& var, ///< [in] 変数
    bool inv           ///< [in] 反転フラグ
                       ///<      - false: 反転なし (正極性)
                       ///<      - true:  反転あり (負極性)
  )
  {
    *this = cofactor(var, inv);
    return *this;
  }

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    const BddLit& lit ///< [in] リテラル
  )
  {
    *this = cofactor(lit);
    return *this;
  }

  /// @brief cofactor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator/=(
    const BddLit& lit ///< [in] リテラル
  )
  {
    return cofactor_int(lit);
  }

  /// @brief コファクターを計算して代入する．
  /// @return 自分自身への参照を返す．
  Bdd&
  cofactor_int(
    const BddCube& cube ///< [in] コファクターのキューブ
  )
  {
    *this = cofactor(cube);
    return *this;
  }

  /// @brief cofactor_int の別名
  /// @return 自分自身への参照を返す．
  Bdd&
  operator/=(
    const BddCube& cube ///< [in] コファクターのキューブ
  )
  {
    return cofactor_int(cube);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // その他の演算
  //////////////////////////////////////////////////////////////////////

  /// @brief (単一)compose演算
  Bdd
  compose(
    const BddVar& var, ///< [in] 対象の変数
    const Bdd& cfunc   ///< [in] 置き換える関数
  ) const;

  /// @brief (単一)compose演算を行って代入する．
  Bdd&
  compose_int(
    const BddVar& var, ///< [in] 対象の変数
    const Bdd& cfunc   ///< [in] 置き換える関数
  )
  {
    *this = compose(var, cfunc);
    return *this;
  }

  /// @brief 複合compose演算
  Bdd
  multi_compose(
    const ComposeMap& compose_map ///< [in] 変換マップ
  ) const
  {
    return _compose(*this, compose_map);
  }

  /// @brief 複合compose演算を行って代入する．
  Bdd&
  multi_compose_int(
    const ComposeMap& compose_map ///< [in] 変換マップ
  )
  {
    *this = _compose(*this, compose_map);
    return *this;
  }

  /// @brief 変数順を入れ替える演算
  ///
  /// 極性も入れ替え可能
  Bdd
  remap_vars(
    const VarMap& var_map ///< [in] 変数の変換マップ
  ) const
  {
    return _remap_vars(*this, var_map);
  }

  /// @brief 変数順を入れ替える演算付き代入
  ///
  /// 極性も入れ替え可能
  Bdd&
  remap_vars_int(
    const VarMap& var_map ///< [in] 変数の変換マップ
  )
  {
    *this = _remap_vars(*this, var_map);
    return *this;
  }

  /// @}
  //////////////////////////////////////////////////////////////////////

public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const;

  /// @brief 与えられた変数がサポートの時 true を返す．
  bool
  check_sup(
    const BddVar& var ///< [in] 変数
  ) const;

  /// @brief 与えられた変数に対して対称の時 true を返す．
  bool
  check_sym(
    const BddVar& var1, ///< [in] 変数1
    const BddVar& var2, ///< [in] 変数2
    bool inv = false    ///< [in] 反転フラグ
  ) const;

  /// @brief サポート変数のサイズを返す．
  SizeType
  support_size() const;

  /// @brief サポート変数を表すBDD(BddVarSet)を返す．
  BddVarSet
  get_support() const;

  /// @brief サポート変数のリスト(vector)を得る．
  std::vector<BddVar>
  get_support_list() const;

  /// @brief 1となるパスを求める．
  /// @return キューブを返す．
  BddCube
  get_onepath() const;

  /// @brief 0となるパスを求める．
  /// @return キューブを返す．
  BddCube
  get_zeropath() const;

  /// @brief 根の変数とコファクターを求める．
  ///
  /// 自身が葉のノードの場合，invalid な BddVar を返す．
  /// f0, f1 には自分自身が入る．
  BddVar
  root_decomp(
    Bdd& f0, ///< [out] 負のコファクター
    Bdd& f1  ///< [out] 正のコファクター
  ) const;

  /// @brief 根の変数を得る．
  ///
  /// 自身が葉のノードの場合，invalid な BddVar を返す．
  BddVar
  root_var() const;

  /// @brief 負のコファクターを返す．
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Bdd
  root_cofactor0() const;

  /// @brief 正のコファクターを返す．
  ///
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Bdd
  root_cofactor1() const;

  /// @brief 評価を行う．
  bool
  eval(
    const std::vector<bool>& inputs ///< [in] 入力値ベクタ
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を真理値表の文字列に変換する．
  ///
  /// - var_list の先頭がMSBとなる．
  /// - var_list に含まれていない変数があった場合には
  ///   std::invalid_argument 例外を送出する．
  std::string
  to_truth(
    const std::vector<BddVar> var_list ///< [in] 入力変数のリスト
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // BddMgrHolder から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ///
  /// root が非終端ノードで mgr が nullptr の場合は
  /// std::invalid_argument 例外が送出される．
  Bdd(
    const BddMgrHolder& mgr, ///< [in] マネージャ
    DdEdge root              ///< [in] 根の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // Bdd 用の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND 演算の下請け関数
  static
  Bdd
  _and_op(
    const Bdd& left,
    const Bdd& right
  );

  /// @brief OR 演算の下請け関数
  static
  Bdd
  _or_op(
    const Bdd& left,
    const Bdd& right
  );

  /// @brief XOR 演算の下請け関数
  static
  Bdd
  _xor_op(
    const Bdd& left,
    const Bdd& right
  );

  /// @brief コファクターを計算する．
  static
  Bdd
  _cofactor(
    const Bdd& left,
    const Bdd& right
  );

  /// @brief 複合compose演算
  static
  Bdd
  _compose(
    const Bdd& left,
    const ComposeMap& compose_map ///< [in] 変換マップ
  );

  /// @brief 変数順を入れ替える演算
  ///
  /// 極性も入れ替え可能
  static
  Bdd
  _remap_vars(
    const Bdd& left,
    const VarMap& var_map ///< [in] 変換マップ
  );

  /// @brief 非終端ノードのチェック
  void
  _check_nonterminal() const;

  friend
  Bdd
  ite(
    const Bdd& cond,
    const Bdd& then_f,
    const Bdd& else_f
  );

  friend
  Bdd
  simplify(
    const Bdd& on,
    const Bdd& dc
  );

  friend
  SopCover
  isop(
    const Bdd& on,
    const Bdd& dc
  );

  friend
  bool
  check_intersect(
    const Bdd& left,
    const Bdd& right
  );

  friend
  SizeType
  bdd_size(
    const std::vector<Bdd>& bdd_list
  );

  friend
  void
  display(
    std::ostream& s,
    const std::vector<Bdd>& bdd_list
  );

  friend
  void
  gen_dot(
    std::ostream& s,
    const std::vector<Bdd>& bdd_list,
    const JsonValue& option
  );

  friend
  std::vector<SizeType>
  rep_data(
    const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
  );

  friend
  void
  dump(
    BinEnc& s,                       ///< [in] 出力ストリーム
    const std::vector<Bdd>& bdd_list ///< [in] 対象の BDDのリスト
  );

};


//////////////////////////////////////////////////////////////////////
/// @name BDD もしくは複数のBDDを扱う関数
/// @{
//////////////////////////////////////////////////////////////////////

/// @brief 複数のBDDのノード数を数える．
///
/// bdd_list 中の BDD は同一のマネージャに属していなければならない．
SizeType
bdd_size(
  const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
);

/// @brief If-Then-Else 演算
Bdd
ite(
  const Bdd& cond,   ///< [in] 条件
  const Bdd& then_f, ///< [in] 条件が成り立ったとき選ばれる関数
  const Bdd& else_f  ///< [in] 条件が成り立たなかった時選ばれる関数
);

/// @brief ドントケアを利用した簡単化を行う．
Bdd
simplify(
  const Bdd& on,  ///< [in] オンセット
  const Bdd& dc   ///< [in] ドントケアセット
);

/// @brief 非冗長積和形を求める．
SopCover
isop(
  const Bdd& on,              ///< [in] オンセット
  const Bdd& dc = Bdd::zero() ///< [in] ドントケアセット
);

/// @brief 交差チェック
/// @return left と right が交差していたら true を返す．
bool
check_intersect(
  const Bdd& left,
  const Bdd& right
);

/// @brief 包含チェック
/// @return left が right を包含していたら true を返す．
inline
bool
check_containment(
  const Bdd& left,
  const Bdd& right
)
{
  return !check_intersect(~left, right);
}


/// @brief 複数のBDDの内容を出力する．
///
/// bdd_list 中の BDD は同一のマネージャに属していなければならない．
void
display(
  std::ostream& s,                 ///< [in] 出力ストリーム
  const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
);

/// @brief 内容を出力する．
inline
void
display(
  std::ostream& s, ///< [in] 出力ストリーム
  const Bdd& bdd   ///< [in] 対象のBDD
)
{
  display(s, std::vector<Bdd>{bdd});
}

/// @brief 内容を出力する．
inline
void
display(
  const std::string& filename, ///< [in] ファイル名
  const Bdd& bdd               ///< [in] 対象のBDD
)
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << "Could not open " << filename;
    throw std::invalid_argument{buf.str()};
  }
  display(s, std::vector<Bdd>{bdd});
}

/// @brief 複数のBDDの内容を出力する．
///
/// bdd_list 中の BDD は同一のマネージャに属していなければならない．
inline
void
display(
  const std::string& filename,     ///< [in] ファイル名
  const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
)
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << "Could not open " << filename;
    throw std::invalid_argument{buf.str()};
  }
  display(s, bdd_list);
}

/// @brief 複数のBDDを dot 形式で出力する．
///
/// - bdd_list 中の BDD は同一のマネージャに属していなければならない．
/// - option は以下のようなキーを持った JSON オブジェクト
///   * attr: dot の各種属性値を持った辞書
///     属性値は <グループ名> ':' <属性名> で表す．
///     グループ名は以下の通り
///     - graph:     グラフ全体
///     - root:      根のノード
///     - node:      通常のノード
///     - terminal:  終端ノード
///     - terminal0: 定数0の終端ノード
///     - terminal1: 定数1の終端ノード
///     - edge:      枝
///     - edge0:     0-枝
///     - edge1:     1-枝
///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
///     を適用する．
///     具体的な属性名と属性値については graphviz の仕様を参照すること．
///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
///     var_label は無視される．
void
gen_dot(
  std::ostream& s,                  ///< [in] 出力ストリーム
  const std::vector<Bdd>& bdd_list, ///< [in] BDDのリスト
  const JsonValue& option           ///< [in] オプションを表す JSON オブジェクト
  = JsonValue{}
);

/// @brief dot 形式で出力する．
///
/// - option は以下のようなキーを持った JSON オブジェクト
///   * attr: dot の各種属性値を持った辞書
///     属性値は <グループ名> ':' <属性名> で表す．
///     グループ名は以下の通り
///     - graph:     グラフ全体
///     - root:      根のノード
///     - node:      通常のノード
///     - terminal:  終端ノード
///     - terminal0: 定数0の終端ノード
///     - terminal1: 定数1の終端ノード
///     - edge:      枝
///     - edge0:     0-枝
///     - edge1:     1-枝
///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
///     を適用する．
///     具体的な属性名と属性値については graphviz の仕様を参照すること．
///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
///     var_label は無視される．
inline
void
gen_dot(
  std::ostream& s,        ///< [in] 出力ストリーム
  const Bdd& bdd,         ///< [in] 対象のBDD
  const JsonValue& option ///< [in] オプションを表す JSON オブジェクト
  = JsonValue{}
)
{
  gen_dot(s, std::vector<Bdd>{bdd}, option);
}

/// @brief dot 形式で出力する．
///
/// - option は以下のようなキーを持った JSON オブジェクト
///   * attr: dot の各種属性値を持った辞書
///     属性値は <グループ名> ':' <属性名> で表す．
///     グループ名は以下の通り
///     - graph:     グラフ全体
///     - root:      根のノード
///     - node:      通常のノード
///     - terminal:  終端ノード
///     - terminal0: 定数0の終端ノード
///     - terminal1: 定数1の終端ノード
///     - edge:      枝
///     - edge0:     0-枝
///     - edge1:     1-枝
///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
///     を適用する．
///     具体的な属性名と属性値については graphviz の仕様を参照すること．
///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
///     var_label は無視される．
inline
void
gen_dot(
  const std::string& filename, ///< [in] ファイル名
  const Bdd& bdd,              ///< [in] 対象のBDD
  const JsonValue& option      ///< [in] オプションを表す JSON オブジェクト
  = JsonValue{}
)
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << "Could not open " << filename;
    throw std::invalid_argument{buf.str()};
  }
  gen_dot(s, std::vector<Bdd>{bdd}, option);
}

/// @brief 複数のBDDを dot 形式で出力する．
///
/// - bdd_list 中の BDD は同一のマネージャに属していなければならない．
/// - option は以下のようなキーを持った JSON オブジェクト
///   * attr: dot の各種属性値を持った辞書
///     属性値は <グループ名> ':' <属性名> で表す．
///     グループ名は以下の通り
///     - graph:     グラフ全体
///     - root:      根のノード
///     - node:      通常のノード
///     - terminal:  終端ノード
///     - terminal0: 定数0の終端ノード
///     - terminal1: 定数1の終端ノード
///     - edge:      枝
///     - edge0:     0-枝
///     - edge1:     1-枝
///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
///     を適用する．
///     具体的な属性名と属性値については graphviz の仕様を参照すること．
///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
///     var_label は無視される．
inline
void
gen_dot(
  std::string& filename,            ///< [in] 出力ストリーム
  const std::vector<Bdd>& bdd_list, ///< [in] BDDのリスト
  const JsonValue& option           ///< [in] オプションを表す JSON オブジェクト
  = JsonValue{}
)
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << "Could not open " << filename;
    throw std::invalid_argument{buf.str()};
  }
  gen_dot(s, bdd_list, option);
}

/// @brief 構造を表す整数配列を作る．
///
/// bdd_list 中の BDD は同一のマネージャに属していなければならない．
std::vector<SizeType>
rep_data(
  const std::vector<Bdd>& bdd_list ///< [in] BDDのリスト
);

/// @brief 構造を表す整数配列を作る．
inline
std::vector<SizeType>
rep_data(
  const Bdd& bdd ///< [in] 対象のBDD
)
{
  return rep_data(std::vector<Bdd>{bdd});
}

/// @brief BDD の内容をバイナリダンプする．
///
/// bdd_list 中の BDD は同一のマネージャに属していなければならない．
void
dump(
  BinEnc& s,                       ///< [in] 出力ストリーム
  const std::vector<Bdd>& bdd_list ///< [in] 対象の BDDのリスト
);

/// @brief 独自形式でバイナリダンプする．
///
/// 復元には BddMgr::restore() を用いる．
inline
void
dump(
  BinEnc& s,     ///< [in] 出力ストリーム
  const Bdd& bdd ///< [in] 対象のBDD
)
{
  dump(s, std::vector<Bdd>{bdd});
}

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_YM

using nsDd::bdd_size;
using nsDd::ite;
using nsDd::simplify;
using nsDd::display;
using nsDd::gen_dot;
using nsDd::dump;

END_NAMESPACE_YM

BEGIN_NAMESPACE_STD

// Bdd をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::Bdd>
{
  SizeType
  operator()(
    const YM_NAMESPACE::Bdd& bdd
  ) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_STD

#endif // BDD_H
