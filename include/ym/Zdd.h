#ifndef ZDD_H
#define ZDD_H

/// @file Zdd.h
/// @brief Zdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddBase.h"
#include "ym/BinEnc.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class Zdd Zdd.h "ym/Zdd.h"
/// @ingroup DdGroup
/// @brief ZDD を表すクラス
//////////////////////////////////////////////////////////////////////
class Zdd :
  public ZddBase
{
  friend class ZddMgrHolder;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  Zdd() = default;

  /// @brief デストラクタ
  ~Zdd();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name オブジェクト生成用のクラスメソッド
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 空集合(定数0)を作る．
  static
  Zdd
  empty_set();

  /// @brief 全集合(定数1)を作る．
  static
  Zdd
  universal_set();

  /// @brief 不正な値を作る．
  static
  Zdd
  invalid();

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 集合演算
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 補集合演算
  /// @return 結果を返す．
  Zdd
  invert() const;

  /// @brief invert の別名
  /// @return 結果を返す．
  Zdd
  operator~() const
  {
    return invert();
  }

  /// @brief 共通集合演算
  /// @return 結果を返す．
  Zdd
  cap(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief cap_op の別名
  /// @return 結果を返す．
  Zdd
  operator&(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return cap(right);
  }

  /// @brief ユニオン演算
  /// @return 結果を返す．
  Zdd
  cup(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief cup の別名
  /// @return 結果を返す．
  Zdd
  operator|(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return cup(right);
  }

  /// @brief 集合差演算
  /// @return 結果を返す．
  Zdd
  diff(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief diff の別名
  /// @return 結果を返す．
  Zdd
  operator-(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return diff(right);
  }

  /// @brief 直積演算
  /// @return 結果を返す．
  Zdd
  product(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief product の別名
  /// @return 結果を返す．
  Zdd
  operator*(
    const Zdd& right ///< [in] オペランド
  ) const
  {
    return product(right);
  }

  /// @brief 要素を含む集合を求める．
  /// @return 結果を返す．
  Zdd
  onset(
    const ZddItem& item ///< [in] 要素
  ) const;

  /// @brief 要素を含まない集合を求める．
  /// @return 結果を返す．
  Zdd
  offset(
    const ZddItem& item ///< [in] 要素
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 集合演算を伴った代入
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 自分自身を否定する．
  /// @return 自分自身への参照を返す．
  Zdd&
  invert_int();

  /// @brief 共通集合を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  cap_int(
    const Zdd& right ///< [in] オペランド
  );

  /// @brief cap_int の別名
  /// @return 自分自身への参照を返す．
  Zdd&
  operator&=(
    const Zdd& right ///< [in] オペランド
  )
  {
    return cap_int(right);
  }

  /// @brief ユニオンを計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  cup_int(
    const Zdd& right ///< [in] オペランド
  );

  /// @brief cup_int の別名
  /// @return 自分自身への参照を返す．
  Zdd&
  operator|=(
    const Zdd& right ///< [in] オペランド
  )
  {
    return cup_int(right);
  }

  /// @brief 集合差を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  diff_int(
    const Zdd& right ///< [in] オペランド
  );

  /// @brief diff_int の別名
  /// @return 自分自身への参照を返す．
  Zdd&
  operator-=(
    const Zdd& right ///< [in] オペランド
  )
  {
    return diff_int(right);
  }

  /// @brief 直積を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  product_int(
    const Zdd& right ///< [in] オペランド
  );

  /// @brief product_int の別名
  /// @return 自分自身への参照を返す．
  Zdd&
  operator*=(
    const Zdd& right ///< [in] オペランド
  )
  {
    return product_int(right);
  }

  /// @brief onset を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  onset_int(
    const ZddItem& item ///< [in] 要素
  );

  /// @brief offset を計算して代入する．
  /// @return 自分自身への参照を返す．
  Zdd&
  offset_int(
    const ZddItem& item ///< [in] 要素
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief サポートのリストを返す．
  std::vector<ZddItem>
  get_support_list() const;

  /// @brief 根の要素とコファクターを求める．
  ///
  /// 自身が葉のノードの場合，ZddItem::invalid() を返す．
  /// f0, f1 には自分自身が入る．
  ZddItem
  root_decomp(
    Zdd& f0, ///< [out] 負のコファクター
    Zdd& f1  ///< [out] 正のコファクター
  ) const;

  /// @brief 根の要素を得る．
  ///
  /// 自身が葉のノードの場合，ZddItem::invalid() を返す．
  ZddItem
  root_item() const;

  /// @brief 負のコファクターを返す．
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Zdd
  root_cofactor0() const;

  /// @brief 正のコファクターを返す．
  ///
  ///
  /// 自身が葉のノードの場合，自分自身を返す．
  Zdd
  root_cofactor1() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 集合の要素数を数える．
  SizeType
  count() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:


private:
  //////////////////////////////////////////////////////////////////////
  // ZddMgrHolder から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ///
  /// root が非終端ノードで mgr が nullptr の場合は
  /// std::invalid_argument 例外が送出される．
  Zdd(
    const ZddMgrHolder& mgr, ///< [in] マネージャ
    DdEdge root              ///< [in] 根の枝
  );

  friend
  SizeType
  zdd_size(
    const std::vector<Zdd>& zdd_list
  );

  friend
  void
  display(
    std::ostream& s,
    const std::vector<Zdd>& zdd_list
  );

  friend
  void
  gen_dot(
    std::ostream& s,
    const std::vector<Zdd>& zdd_list,
    const JsonValue& option
  );

  friend
  std::vector<SizeType>
  rep_data(
    const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

  friend
  void
  dump(
    BinEnc& s,                       ///< [in] 出力ストリーム
    const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
  );

};


//////////////////////////////////////////////////////////////////////
/// @name ZDD もしくは複数のZDDを扱う関数
/// @{
//////////////////////////////////////////////////////////////////////

// @brief 複数のZDDのノード数を数える．
SizeType
zdd_size(
  const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
);

/// @brief 複数のZDDの内容を出力する．
void
display(
  std::ostream& s,                 ///< [in] 出力ストリーム
  const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
);

/// @brief 内容を出力する．
inline
void
display(
  std::ostream& s, ///< [in] 出力ストリーム
  const Zdd& zdd   ///< [in] 対象のZDD
)
{
  display(s, std::vector<Zdd>{zdd});
}

/// @brief 内容を出力する．
inline
void
display(
  const std::string& filename, ///< [in] ファイル名
  const Zdd& zdd               ///< [in] 対象のZDD
)
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << "Could not open " << filename;
    throw std::invalid_argument{buf.str()};
  }
  display(s, std::vector<Zdd>{zdd});
}

/// @brief 複数のZDDの内容を出力する．
inline
void
display(
  const std::string& filename,     ///< [in] ファイル名
  const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
)
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << "Could not open " << filename;
    throw std::invalid_argument{buf.str()};
  }
  display(s, zdd_list);
}

/// @brief 複数のZDDを dot 形式で出力する．
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
///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
///     を適用する．
///     具体的な属性名と属性値については graphviz の使用を参照すること．
///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
///     var_label は無視される．
void
gen_dot(
  std::ostream& s,                  ///< [in] 出力ストリーム
  const std::vector<Zdd>& zdd_list, ///< [in] ZDDのリスト
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
///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
///     を適用する．
///     具体的な属性名と属性値については graphviz の使用を参照すること．
///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
///     var_label は無視される．
inline
void
gen_dot(
  std::ostream& s,        ///< [in] 出力ストリーム
  const Zdd& zdd,         ///< [in] 対象のZDD
  const JsonValue& option ///< [in] オプションを表す JSON オブジェクト
  = JsonValue{}
)
{
  gen_dot(s, std::vector<Zdd>{zdd}, option);
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
///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
///     を適用する．
///     具体的な属性名と属性値については graphviz の使用を参照すること．
///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
///     var_label は無視される．
inline
void
gen_dot(
  const std::string& filename, ///< [in] ファイル名
  const Zdd& zdd,         ///< [in] 対象のZDD
  const JsonValue& option ///< [in] オプションを表す JSON オブジェクト
  = JsonValue{}
)
{
  std::ofstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << "Could not open " << filename;
    throw std::invalid_argument{buf.str()};
  }
  gen_dot(s, std::vector<Zdd>{zdd}, option);
}

/// @brief 複数のZDDを dot 形式で出力する．
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
///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
///     を適用する．
///     具体的な属性名と属性値については graphviz の使用を参照すること．
///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
///     var_label は無視される．
inline
void
gen_dot(
  const std::string& filename, ///< [in] ファイル名
  const std::vector<Zdd>& zdd_list, ///< [in] ZDDのリスト
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
  gen_dot(s, zdd_list, option);
}

/// @brief 構造を表す整数配列を作る．
std::vector<SizeType>
rep_data(
  const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
);

/// @brief 構造を表す整数配列を作る．
inline
std::vector<SizeType>
rep_data(
  const Zdd& zdd ///< [in] 対象のZDD
)
{
  return rep_data(std::vector<Zdd>{zdd});
}

/// @brief 複数のZDDを独自形式でバイナリダンプする．
///
/// 復元には ZddMgr::restore() を用いる．
void
dump(
  BinEnc& s,                       ///< [in] 出力ストリーム
  const std::vector<Zdd>& zdd_list ///< [in] ZDDのリスト
);

/// @brief 独自形式でバイナリダンプする．
///
/// 復元には ZddMgr::restore() を用いる．
inline
void
dump(
  BinEnc& s,     ///< [in] 出力ストリーム
  const Zdd& zdd ///< [in] 対象のZDD
)
{
  dump(s, std::vector<Zdd>{zdd});
}

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_YM

using nsDd::zdd_size;
using nsDd::display;
using nsDd::gen_dot;
using nsDd::rep_data;
using nsDd::dump;

END_NAMESPACE_YM

BEGIN_NAMESPACE_STD

// Zdd をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::Zdd>
{
  SizeType
  operator()(
    const YM_NAMESPACE::Zdd& bdd
  ) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_STD

#endif // ZDD_H
