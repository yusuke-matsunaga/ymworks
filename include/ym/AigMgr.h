#ifndef AIGMGR_H
#define AIGMGR_H

/// @file AigMgr.h
/// @brief AigMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "ym/AigLiteral.h"
#include "ym/Expr.h"
#include "ym/sop.h"
#include "ym/bn.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigMgr AigMgr.h "AigMgr.h"
/// @brief AIG 全体を管理するクラス
///
/// 純粋な AIG は組み合わせ論理回路もしくは論理関数を表すものであるが，
/// .aag/.aig ではラッチ(DFF)を含んでいる．
/// このクラスではラッチの入出力を擬似外部入出力として扱う．
///
/// 以下の部品からなる．
/// - 外部入力ノード
///   * 入力番号
///   * 名前(オプション)
///
/// - DFF出力ノード
///   * DFF番号
///
/// - ANDノード
///   * ファンイン0の枝
///   * ファンイン1の枝
///
/// - 外部出力端子
///   * 出力番号
///   * 名前(オプション)
///   * 関連する枝
///
/// - DFF入力端子
///   * DFF番号
///   * 名前(オプション)
///   * 関連する枝
///
/// - DFF
///   * DFF番号
///   * 名前(オプション)
///
/// ノードそのものは外部からは参照できない．
/// その代わりにノード番号を指定した情報取得用のAPIを持つ．
/// ANDノードのファンインや出力のソースはノードではなくノードと極性の情報を持つ．
/// そのために ノード番号と極性を合わせて持つ AigLiteral を用いる．
/// AigLiteral は内部でノードIDを持っているが，ノードIDは不変ではないため，
/// AIG の構造に変更があった場合には以前取得した AigLiteral の値は不正になっている
/// ので注意が必要である．
/// 今の所，ノードIDを変更する可能性のある関数は rewrite() のみである．
///
/// 実体は実装クラスへのスマートポインタなのでコピーしても
/// 実装クラスは共有される．
/// 実装クラスはすべての参照がなくなった時に開放される．
///
/// '深い'コピーを行うためには copy() メソッドを用いる．
//////////////////////////////////////////////////////////////////////
class AigMgr
{
public:

  /// @brief コンストラクタ
  AigMgr();

  /// @brief コピーコンストラクタ
  ///
  /// '浅い'コピーを行う．
  AigMgr(
    const AigMgr& src
  ) = default;

  /// @brief コピー代入演算
  ///
  /// '浅い'コピーを行う．
  AigMgr&
  operator=(
    const AigMgr& src
  ) = default;

  /// @brief デストラクタ
  ~AigMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル入力
  //////////////////////////////////////////////////////////////////////

  /// @brief aag ファイルの読み込みを行う．
  /// @return 結果の AigMgr を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  AigMgr
  read_aag(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief aig ファイルの読み込みを行う．
  /// @return 結果の AigMgr を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  AigMgr
  read_aig(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief blif ファイルの読み込みを行う．
  /// @return 結果の AigMgr を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  AigMgr
  read_blif(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief iscas89 ファイルの読み込みを行う．
  /// @return 結果の AigMgr を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  AigMgr
  read_iscas89(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief 任意の形式のファイルを読み込む．
  /// @return 結果の AigMgr を返す．
  ///
  /// format が省略された場合には拡張子から推測する．
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  /// 対応する拡張子は以下の通り
  /// - '.aag':   read_aag()
  /// - '.aig':   read_aig()
  /// - '.blif':  read_blif()
  /// - '.bench': read_iscas89()
  static
  AigMgr
  read(
    const std::string& filename,   ///< [in] ファイル名
    const std::string& format = {} ///< [in] ファイルの形式
  );

  /// @brief BnModel からの変換
  /// @return 新しいマネージャを返す．
  static
  AigMgr
  from_bn(
    const BnModel& bn_model
  );


public:
  //////////////////////////////////////////////////////////////////////
  // ファイル出力
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を aag ファイル形式で出力する．
  void
  write_aag(
    const std::string& filename ///< [in] ファイル名
  ) const;

  /// @brief 内容を aag ファイル形式で出力する．
  void
  write_aag(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief 内容を aig ファイル形式で出力する．
  void
  write_aig(
    const std::string& filename ///< [in] ファイル名
  ) const;

  /// @brief 内容を aig ファイル形式で出力する．
  void
  write_aig(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief 内容を出力する．
  ///
  /// format は以下のいずれか
  /// - aag
  /// - aig
  void
  write(
    const std::string& filename, ///< [in] ファイル名
    const std::string& format    ///< [in] 形式
  ) const;

  /// @brief 内容を出力する．
  ///
  /// format は以下のいずれか
  /// - aag
  /// - aig
  void
  write(
    std::ostream& s,          ///< [in] 出力ストリーム
    const std::string& format ///< [in] 形式
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正なポインタを持っている時 true を返す．
  bool
  is_valid() const
  {
    return mPtr != nullptr;
  }

  /// @brief 適正なポインタを持っていない時 true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief '深い'コピーを作る．
  AigMgr
  copy() const;

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief 外部入力数を返す．
  SizeType
  input_num() const;

  /// @brief 外部出力数を返す．
  SizeType
  output_num() const;

  /// @brief ANDノード数を返す．
  SizeType
  and_num() const;

  /// @brief DFF数を返す．
  SizeType
  dff_num() const;

  /// @brief 外部入力を表すリテラルを返す．
  AigLiteral
  input(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const;

  /// @brief 入力名を返す．
  std::string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const;

  /// @brief 出力に対応するリテラルを返す．
  AigLiteral
  output(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const;

  /// @brief 出力名を返す．
  std::string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const;

  /// @brief ラッチ(DFF)の出力を表すリテラルを返す．
  AigLiteral
  dff_output(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const;

  /// @brief ラッチ(DFF)の入力を返す．
  AigLiteral
  dff_input(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const;

  /// @brief ラッチ(DFF)名を返す．
  std::string
  dff_name(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const;

  /// @brief コメントのリストを返す．
  const std::vector<std::string>&
  comment_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ノードに関する情報取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの時 true を返す．
  ///
  /// 入力ノードは外部入力とDFF出力
  bool
  is_input(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;

  /// @brief 外部入力ノードの時 true を返す．
  bool
  is_primary_input(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;

  /// @brief 外部入力ノードの時 入力番号を返す．
  SizeType
  input_id(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;

  /// @brief DFF出力の時 true を返す．
  bool
  is_dff_output(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;

  /// @brief DFF出力の時 DFF番号を返す．
  SizeType
  dff_id(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;

  /// @brief ANDノードの時 true を返す．
  bool
  is_and(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;

  /// @brief ANDノードの時 ファンインを返す．
  AigLiteral
  fanin(
    SizeType node_id, ///< [in] ノード番号 ( 0 <= node_id < node_num() )
    SizeType pos      ///< [in] 位置 ( 0 or 1 )
  ) const;

  /// @brief ANDノードの時 ファンイン0を返す．
  AigLiteral
  fanin0(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;

  /// @brief ANDノードの時 ファンイン1を返す．
  AigLiteral
  fanin1(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;

  /// @brief ANDグループのファンインのリストを返す．
  ///
  /// ANDグループとは反転していない枝で繋がったANDノードの集合
  std::vector<AigLiteral>
  ex_fanin_list(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を操作する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力を作る．
  /// @return 作成した入力を表すリテラルを返す．
  AigLiteral
  new_input(
    const std::string& name = {} ///< [in] 名前
  );

  /// @brief 出力を作る．
  /// @return 出力番号を返す．
  SizeType
  new_output(
    AigLiteral lit,              ///< [in] 対応するリテラル
    const std::string& name = {} ///< [in] 名前
  );

  /// @brief ラッチ(DFF)を作る．
  /// @return DFF番号を返す．
  ///
  /// - DFFの出力に対応するノードは生成される．
  /// - DFFの入力は未設定
  SizeType
  new_dff(
    const std::string& name = {} ///< [in] 名前
  );

  /// @brief ラッチ(DFF)の入力を設定する．
  void
  set_dff_input(
    SizeType dff_id, ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
    AigLiteral edge  ///< [in] 設定するリテラル
  );

  /// @brief 複数の入力の AND を作る．
  AigLiteral
  new_and(
    const std::vector<AigLiteral>& fanin_list ///< [in] ファンインの枝のリスト
  );

  /// @brief 複数の入力の OR を作る．
  AigLiteral
  new_or(
    const std::vector<AigLiteral>& fanin_list ///< [in] ファンインの枝のリスト
  );

  /// @brief 複数の入力の XOR を作る．
  AigLiteral
  new_xor(
    const std::vector<AigLiteral>& fanin_list ///< [in] ファンインの枝のリスト
  );

  /// @brief PrimType の演算を行う．
  AigLiteral
  new_primitive(
    PrimType type,                            ///< [in] 演算の種類
    const std::vector<AigLiteral>& fanin_list ///< [in] ファンインの枝のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 他の形式からの変換
  //////////////////////////////////////////////////////////////////////

  /// @brief Expr から変換する．
  ///
  /// 論理式の i 番目の変数は input(i) を用いる．
  AigLiteral
  from_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief Expr から変換する．
  ///
  /// 論理式の i 番目の変数は input_list[i] を用いる．
  AigLiteral
  from_expr(
    const Expr& expr,                         ///< [in] 論理式
    const std::vector<AigLiteral>& input_list ///< [in] 変数に対応する枝のリスト
  );

  /// @brief 複数の Expr から変換する．
  ///
  /// 論理式の i 番目の変数は input(i) を用いる．
  std::vector<AigLiteral>
  from_expr_list(
    const std::vector<Expr>& expr_list ///< [in] 論理式のリスト
  );

  /// @brief 複数の Expr から変換する．
  ///
  /// 論理式の i 番目の変数は input_list[i] を用いる．
  std::vector<AigLiteral>
  from_expr_list(
    const std::vector<Expr>& expr_list,       ///< [in] 論理式のリスト
    const std::vector<AigLiteral>& input_list ///< [in] 変数に対応する枝のリスト
  );

  /// @brief SopCover から変換する．
  ///
  /// カバーの i 番目の変数は input(i) を用いる．
  AigLiteral
  from_cover(
    const SopCover& cover ///< [in] カバー
  );

  /// @brief SopCover から変換する．
  ///
  /// カバーの i 番目の変数は input_list[i] を用いる．
  AigLiteral
  from_cover(
    const SopCover& cover,                    ///< [in] カバー
    const std::vector<AigLiteral>& input_list ///< [in] 変数に対応する枝のリスト
  );

  /// @brief SopCube から変換する．
  ///
  /// キューブの i 番目の変数は i 番目の入力を用いる．
  AigLiteral
  from_cube(
    const SopCube& cube ///< [in] キューブ
  );

  /// @brief SopCube から変換する．
  ///
  /// キューブの i 番目の変数は input_list[i] を用いる．
  AigLiteral
  from_cube(
    const SopCube& cube,                      ///< [in] カバー
    const std::vector<AigLiteral>& input_list ///< [in] 変数に対応する枝のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // AIG 上の操作
  //////////////////////////////////////////////////////////////////////

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  AigLiteral
  cofactor(
    AigEdge edge,                       ///< [in] 対象の枝
    const std::vector<AigLiteral>& cube ///< [in] コファクターキューブ
  );

  /// @brief コファクター演算
  ///
  /// - cube に含まれるノードの値を固定した AIG を作る．
  std::vector<AigLiteral>
  cofactor(
    const std::vector<AigLiteral>& edge_list, ///< [in] 対象の枝のリスト
    const std::vector<AigLiteral>& cube       ///< [in] コファクターキューブ
  );

  /// @brief local rewriting を行う．
  void
  rewrite();

  /// @brief シミュレーションを行う．
  /// @return 出力値を格納した配列を返す．
  ///
  /// - input_vals は入力番号をキーとして値を格納している．
  /// - DFFの出力，入力はそれぞれ疑似入力，疑似出力として扱う．
  std::vector<AigBitVect>
  simulate(
    const std::vector<AigBitVect>& input_vals ///< [in] 入力値のりスト
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  ///
  /// 意味的な等価ではなく，同一のマネージャかどうか調べる．
  bool
  operator==(
    const AigMgr& right
  ) const
  {
    return check_mgr(right);
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AigMgr& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 構造が等しいか調べる．
  bool
  is_identical(
    const AigMgr& right
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief dot 形式で出力する．
  ///
  /// - option は以下のようなキーを持った JSON オブジェクト
  ///   * attr: dot の各種属性値を持った辞書
  ///     属性値は <グループ名> ':' <属性名> で表す．
  ///     グループ名は以下の通り
  ///     - graph:    グラフ全体
  ///     - input:    入力ノード
  ///     - output:   出力ノード
  ///     - and:      ANDノード
  ///     - dff:      DFF
  ///     - latch:    ラッチ
  ///     - edge:     枝
  ///     グループ名と ':' がない場合には全てのグループに対して同一の属性値
  ///     を適用する．
  ///     具体的な属性名と属性値については graphviz の仕様を参照すること．
  ///   * var_label: 変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_texlbl: TeX用の変数ラベルを表す配列．配列のキーは変数番号
  ///   * var_label と var_texlbl は排他的となる．var_texlbl がある時，
  ///     var_label は無視される．
  void
  gen_dot(
    std::ostream& s,        ///< [in] 出力ストリーム
    const JsonValue& option ///< [in] オプションを表す JSON オブジェクト
    = JsonValue{}
  ) const;

  /// @brief 内容を出力する(デバッグ用)．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用される関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  AigMgr(
    AigMgrImpl* impl
  );

  /// @brief new_and() の下請け関数
  AigEdge
  and_sub(
    const std::vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                         ///< [in] 開始位置
    SizeType end                            ///< [in] 終了位置
  );

  /// @brief new_or() の下請け関数
  AigEdge
  or_sub(
    const std::vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                         ///< [in] 開始位置
    SizeType end                            ///< [in] 終了位置
  );

  /// @brief new_xor() の下請け関数
  AigEdge
  xor_sub(
    const std::vector<AigEdge>& fanin_list, ///< [in] ファンインのリスト
    SizeType begin,                         ///< [in] 開始位置
    SizeType end                            ///< [in] 終了位置
  );

  /// @brief dereference 演算子
  AigMgrImpl*
  get() const
  {
    _check_valid();
    return mPtr.get();
  }

  /// @brief get() の別名
  AigMgrImpl*
  operator->() const
  {
    return get();
  }

  /// @brief 0 - (n - 1) 番目の入力のリストを返す．
  ///
  /// 入力がない場合は作成する．
  std::vector<AigLiteral>
  _input_list(
    SizeType n ///< [in] 要素数
  );

  /// @brief 同じマネージャを持つかチェックする．
  bool
  check_mgr(
    const AigMgr& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 適正なポインタを持っていない時，例外を送出する．
  void
  _check_valid() const
  {
    if ( is_invalid() ) {
      throw std::logic_error{"mPtr is null"};
    }
  }

  /// @brief AigLiteral を AigEdge に変換する
  AigEdge
  literal2edge(
    AigLiteral lit
  ) const;

  /// @brief AigLiteral のりストを AigEdge のりストに変換する
  std::vector<AigEdge>
  literal2edge_list(
    const std::vector<AigLiteral>& lit_list
  ) const;

  /// @brief AigEdge を AigLiteral に変換する
  static
  AigLiteral
  edge2literal(
    AigEdge edge
  );

  /// @brief AigEdge のりストを AigLiteral のりストに変換する
  static
  std::vector<AigLiteral>
  edge2literal_list(
    const std::vector<AigEdge>& edge_list
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタ本体
  std::shared_ptr<AigMgrImpl> mPtr;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGR_H
