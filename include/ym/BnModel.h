#ifndef BNMODEL_H
#define BNMODEL_H

/// @file BnModel.h
/// @brief BnModel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"
#include "ym/json.h"
#include "ym/BnDff.h"
#include "ym/BnDffList.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnFunc.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnModel BnModel.h "BnModel.h"
/// @brief Boolean Network を表すクラス
///
/// 以下の情報を持つ．
/// - 入力ノードのリスト
/// - 出力ノードのリスト
/// - 論理ノードのリスト
/// - DFFのリスト
///
///
/// 入力ノードには以下の2種類がある．
/// - 外部入力: 入力番号を持つ．
/// - DFF出力: DFF番号を持つ
///
/// 論理ノードは以下のいずれかの論理関数情報を持つ．
/// - プリミティブ(PrimType)
/// - カバー(SopCover)
/// - 論理式(Expr)
/// - 関数(TvFunc)
/// - BDD(Bdd)
/// いずれの場合にファンインのノードのリストを持つ．
///
/// 出力ノードは実際にはノードではなく他のノードの参照である．
///
/// 純粋な Boolean Network は抽象化した組み合わせ論理回路を表すものであるが、
/// DFFの入力ノードと出力ノードをそれぞれ疑似外部出力、疑似外部入力とみなすことで
/// 同期式順序回路を表すことができる．
/// そこでDFF出力(を表す入力ノード)とDFF入力ノードへの参照を持ったDFFクラスを用意する．
/// ただし、クロックや非同期リセットなどの制御は表せない．
///
/// 扱えるファイルタイプは以下の通り
///   * blif(.blif)
///   * iscas89(.bench)
///   * truth(IWLS2022)
///
/// BnModel クラスとしてはノード名とは別に入力名，出力名，DFF名を持つことが可能である．
/// この場合，入力名とその入力に対応するノードのノード名が異なる場合もある．
/// ただし，blif/iscas89の場合は入出力名は対応するノード名と同一になっている．
/// そのため入力ノードがそのまま出力となっている場合には
/// 入力と出力に同じ名前を持つことになる．
/// どちらにせよ，名前に関する制限は一切ないのでここでは与えられた情報をただ保存するだけとなる．
///
/// - 実装は本体の ModelImpl を指す共有ポインタのみを持つ．
///   ただし，ModelImpl は同一の BnModel に属する BnNode 内でしか共有しない．
//////////////////////////////////////////////////////////////////////
class BnModel
{
public:

  /// @brief 空のコンストラクタ
  BnModel();

  /// @brief コピーコンストラクタ
  BnModel(
    const BnModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  BnModel&
  operator=(
    const BnModel& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BnModel();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 読み込みを行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief blif ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_blif(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief iscas89(.bench) ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_iscas89(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief truth ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read_truth(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief ファイルの読み込みを行う．
  /// @return 結果の BnModel を返す．
  ///
  /// - format は以下のいずれか
  ///   * blif
  ///   * iscas89|bench
  ///   * truth
  /// - format が指定されていない時は拡張子から推測する．
  ///   * .blif -> blif
  ///   * .bench -> iscas89
  ///   * .truth -> truth
  /// - 読み込みが失敗したら std::invalid_argument 例外を送出する．
  static
  BnModel
  read(
    const std::string& filename,   ///< [in] ファイル名
    const std::string& format = "" ///< [in] 形式
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 書き出しを行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を出力する．
  void
  write(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を読み出す関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief '深い'コピーを作る．
  BnModel
  copy() const;

  /// @brief DFF数を返す．
  SizeType
  dff_num() const;

  /// @brief DFFを返す．
  BnDff
  dff(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const;

  /// @brief DFFのリストを返す．
  BnDffList
  dff_list() const;

  /// @brief ノード数を返す．
  SizeType
  node_num() const;

  /// @brief ノードを返す．
  BnNode
  node(
    SizeType id ///< [in] ノード番号 ( 0 <= id < node_num() )
  ) const;

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 入力ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  input(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const;

  /// @brief 入力ノードのリストを返す．
  BnNodeList
  input_list() const;

  /// @brief 出力数を返す．
  SizeType
  output_num() const;

  /// @brief 出力ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  output(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const;

  /// @brief 出力のノードのリストを返す．
  BnNodeList
  output_list() const;

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const;

  /// @brief 論理ノードを返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnNode
  logic(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < logic_num() )
  ) const;

  /// @brief 論理ノードのリストを返す．
  ///
  /// 入力からのトポロジカル順になっている．
  BnNodeList
  logic_list() const;

  /// @brief 関数情報の数を返す．
  SizeType
  func_num() const;

  /// @brief 関数情報を取り出す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  BnFunc
  func(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name オプション情報を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション情報を表す JSON オブジェクトを返す．
  JsonValue
  option() const;

  /// @brief 名前を返す．
  std::string
  name() const;

  /// @brief コメントを返す．
  const std::vector<std::string>&
  comment_list() const;

  /// @brief 入力名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  std::string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const;

  /// @brief 出力名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  std::string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const;

  /// @brief DFF名を返す．
  ///
  /// - 範囲外のアクセスは std::out_of_range 例外を送出する．
  std::string
  dff_name(
    SizeType dff_id ///< [in] ラッチ番号 ( 0 <= dff_id < dff_num() )
  ) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を設定する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション情報をセットする．
  void
  set_option(
    const JsonValue& option ///< [in] 追加するオプション
  );

  /// @brief 名前を設定する．
  void
  set_name(
    const std::string& name ///< [in] 名前
  );

  /// @brief コメントを追加する．
  void
  add_comment(
    const std::string& comment ///< [in] コメント
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name DFF/ノードの生成
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFを作る．
  /// @return 生成したDFFを返す．
  BnDff
  new_dff(
    const std::string& name = {}, ///< [in] 名前
    char reset_val = 'X'          ///< [in] リセット値
  );

  /// @brief DFFの入力ノードを設定する．
  void
  set_dff_src(
    const BnDff& dff, ///< [in] DFF
    BnNode src        ///< [in] 入力に設定するノード
  );

  /// @brief 入力ノードを作る．
  /// @return 生成したノードを返す．
  BnNode
  new_input(
    const std::string& name = {} ///< [in] 名前
  );

  /// @brief 出力ノードを作る．
  /// @return 生成した出力ノードの出力番号を返す．
  ///
  /// - src は同じ BnModel に属するノードでなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  SizeType
  new_output(
    BnNode src,                  ///< [in] ソースノード
    const std::string& name = {} ///< [in] 名前
  );

  /// @brief プリミティブ型の論理ノードを作る．
  /// @return 生成したノードを返す．
  ///
  /// - fanin_list の要素は同じ BnModel に属するノードでなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_primitive(
    PrimType primitive_type,              ///< [in] プリミティブの種類
    const std::vector<BnNode>& fanin_list ///< [in] ファンインのリスト
  );

  /// @brief カバー型の論理ノードを登録する．
  /// @return 生成したノードを返す．
  ///
  /// - fanin_list の要素は同じ BnModel に属するノードでなければならない．
  /// - fanin_list の要素数は input_cover の入力数と等しくなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_cover(
    const SopCover& input_cover,          ///< [in] 入力カバー
    bool output_inv,                      ///< [in] 出力の反転属性
    const std::vector<BnNode>& fanin_list ///< [in] ファンインのリスト
  );

  /// @brief 論理式型の論理ノードを登録する．
  /// @return 生成したノードを返す．
  ///
  /// - fanin_list の要素は同じ BnModel に属するノードでなければならない．
  /// - fanin_list の要素数は expr の入力数と等しくなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_expr(
    const Expr& expr,                     ///< [in] 論理式
    const std::vector<BnNode>& fanin_list ///< [in] ファンインのリスト
  );

  /// @brief 真理値表型の論理ノードを登録する．
  /// @return 生成したノードを返す．
  ///
  /// - fanin_list の要素は同じ BnModel に属するノードでなければならない．
  /// - fanin_list の要素数は func の入力数と等しくなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_tvfunc(
    const TvFunc& func,                   ///< [in] 真理値表型の関数
    const std::vector<BnNode>& fanin_list ///< [in] ファンインのリスト
  );

  /// @brief BDD型の論理ノードを登録する．
  /// @return 生成したノードを返す．
  ///
  /// - fanin_list の要素は同じ BnModel に属するノードでなければならない．
  /// - fanin_list の要素数は bdd の入力数と等しくなければならない．
  /// - 条件に合わない時は std::invalid_argument 例外を送出する．
  BnNode
  new_bdd(
    const Bdd& bdd,                       ///< [in] BDD
    const std::vector<BnNode>& fanin_list ///< [in] ファンインのリスト
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 比較演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnModel& right ///< [in] オペランド
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnModel& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BnModel(
    ModelImpl* impl
  );

  /// @brief ノードのリストをポインタのリストに変換する．
  static
  std::vector<const NodeImpl*>
  make_ptr_list(
    const std::vector<BnNode>& node_list
  )
  {
    std::vector<const NodeImpl*> ptr_list;
    ptr_list.reserve(node_list.size());
    for ( auto& node: node_list ) {
      ptr_list.push_back(node.mPtr);
    }
    return ptr_list;
  }

  /// @brief ModelImpl を返す．
  const ModelImpl&
  _model_impl() const;

  /// @brief ModelImpl を返す．
  ModelImpl&
  _model_impl();

  /// @brief 入力番号のチェックを行う．
  void
  _check_input_id(
    SizeType input_id
  ) const;

  /// @brief 出力番号のチェックを行う．
  void
  _check_output_id(
    SizeType output_id
  ) const;

  /// @brief 論理ノード番号のチェックを行う．
  void
  _check_logic_id(
    SizeType logic_id
  ) const;

  /// @brief SDFF番号のチェックを行う．
  void
  _check_dff_id(
    SizeType dff_id
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装オブジェクトのポインタ
  ModelImpl* mPtr;

};

END_NAMESPACE_YM_BN

#endif // BNMODEL_H
