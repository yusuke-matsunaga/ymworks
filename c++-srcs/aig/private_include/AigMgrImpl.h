#ifndef AIGMGRIMPL_H
#define AIGMGRIMPL_H

/// @file AigMgrImpl.h
/// @brief AigMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigNode.h"
#include "AigTable.h"
#include "AigTerminal.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_AIG

class EdgeDict;

//////////////////////////////////////////////////////////////////////
/// @class AigMgrImpl AigMgrImpl.h "AigMgrImpl.h"
/// @brief AigMgr の実装クラス
///
/// AigNode は参照カウンタを持つ．
/// 接続の変更に伴って参照数が0になったノードは自動的に削除される．
/// そのため，通常は外部入力側からボトムアップにAIGを構築する必要がある．
/// ただし，AAG フォーマットを読み込む際や，AigMgr の構造をまるごとコピー
/// する際には，ファンインで用いられているノードがあとから生成される
/// 場合がある．
/// そのような場合には最終的に _finalize() を呼んで参照回数を正しく
/// 設定する必要がある．
//////////////////////////////////////////////////////////////////////
class AigMgrImpl
{
  friend class AigParser; // _initialize(), _set_and() など

public:

  /// @brief コンストラクタ
  AigMgrImpl();

  /// @brief デストラクタ
  ~AigMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  ///
  /// 場合によっては未使用のノードを含むこともある．
  SizeType
  node_num() const
  {
    return mNodeArray.size();
  }

  /// @brief ノードを返す．
  AigNode*
  node(
    SizeType id ///< [in] ノード番号 ( 0 <= id < node_num() )
  )
  {
    _check_node_id(id);
    return mNodeArray[id].get();
  }

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputList.size();
  }

  /// @brief 入力ノードを返す．
  AigNode*
  input_node(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    _check_input_id(input_id);
    auto node = mInputList[input_id].node;
    return node;
  }

  /// @brief 入力名を返す．
  ///
  /// 空の場合もある．
  std::string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    _check_input_id(input_id);
    return mInputList[input_id].name;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputList.size();
  }

  /// @brief 出力端子を返す．
  AigTerminal*
  output(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    _check_output_id(output_id);
    return mOutputList[output_id].terminal.get();
  }

  /// @brief 出力に対応する枝を返す．
  AigEdge
  output_edge(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    return output(output_id)->edge();
  }

  /// @brief 出力名を返す．
  ///
  /// 空の場合もある．
  std::string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    _check_output_id(output_id);
    return mOutputList[output_id].name;
  }

  /// @brief DFF数を返す．
  SizeType
  dff_num() const
  {
    return mDffList.size();
  }

  /// @brief DFFの出力に対応するノードを返す．
  AigNode*
  dff_output(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    _check_dff_id(dff_id);
    return mDffList[dff_id].output;
  }

  /// @brief DFFの入力端子を返す．
  AigTerminal*
  dff_input(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    _check_dff_id(dff_id);
    return mDffList[dff_id].input.get();
  }

  /// @brief DFFの入力端子の枝を返す．
  AigEdge
  dff_input_edge(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    return dff_input(dff_id)->edge();
  }

  /// @brief DFF名を返す．
  std::string
  dff_name(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    _check_dff_id(dff_id);
    return mDffList[dff_id].name;
  }

  /// @brief ANDノード数を返す．
  SizeType
  and_num() const;

  /// @brief ANDノードの入力側からのトポロジカル順のリストを得る．
  std::vector<AigNode*>
  and_list() const;

  /// @brief コメントを返す．
  ///
  /// - コメントは文字列のリストで表される．
  /// - 文字列は終端の改行を含んでいない．
  /// - 空の場合もある．
  const std::vector<std::string>&
  comment_list() const
  {
    return mCommentList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を操作する関数
  // この関数を使う限り参照回数は正しく管理されている．
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする．
  void
  copy(
    AigMgrImpl* src_mgr ///< [in] コピー元のマネージャ
  );

  /// @brief 入力ノードを作る．
  /// @return 作成したノードを指す枝を返す．
  AigEdge
  new_input(
    const std::string& name = {} ///< [in] 名前
  )
  {
    auto id = mNodeArray.size();
    auto input_id = mInputList.size();
    auto node = new AigNode(id, input_id);
    mNodeArray.push_back(std::unique_ptr<AigNode>{node});
    mInputList.push_back({node, name});
    return AigEdge(node, false);
  }

  /// @brief 入力名をセットする．
  void
  set_input_name(
    SizeType input_id,      ///< [in] 入力番号
    const std::string& name ///< [in] 名前
  )
  {
    _check_input_id(input_id);
    mInputList[input_id].name = name;
  }

  /// @brief 出力端子を作る．
  AigTerminal*
  new_output(
    AigEdge edge,                ///< [in] 出力に接続する枝
    const std::string& name = {} ///< [in] 名前
  )
  {
    auto oid = mOutputList.size();
    auto output = AigTerminal::output(oid, edge);
    mOutputList.push_back({std::unique_ptr<AigTerminal>{output}, name});
    _set_terminal(output, edge);
    _inc_node_ref(output);
    return output;
  }

  /// @brief 出力名をセットする．
  void
  set_output_name(
    SizeType output_id,     ///< [in] 出力番号
    const std::string& name ///< [in] 名前
  )
  {
    _check_output_id(output_id);
    mOutputList[output_id].name = name;
  }

  /// @brief ANDノードを作る．
  /// @return 作成したノードを指す枝を返す．
  AigEdge
  new_and(
    AigEdge fanin0, ///< [in] ファンイン0
    AigEdge fanin1  ///< [in] ファンイン1
  )
  {
    { // step1: 境界条件
      // 結果が定数になるケースをチェック
      AigEdge ans_edge;
      if ( _special_case(fanin0, fanin1, ans_edge) ) {
	return ans_edge;
      }
    }

    // ファンインの順序を正規化する．
    // ここに来たということは fanin0, fanin1 は定数でない
    // また同一でもない．
    if ( fanin0.node()->id() > fanin1.node()->id() ) {
      std::swap(fanin0, fanin1);
    }

    { // step2: 構造テーブルを探す．
      auto node = find_and(fanin0, fanin1);
      if ( node != nullptr ) {
	return AigEdge(node, false);
      }
    }

    { // step3: 新しいノードを作る．
      auto id = mNodeArray.size();
      auto node = new AigNode(id, fanin0, fanin1);
      mNodeArray.push_back(std::unique_ptr<AigNode>{node});
      _set_and(node, fanin0, fanin1);
      return AigEdge(node, false);
    }
  }

  /// @brief ORノードを作る．
  /// @return 作成したノードを指す枝を返す．
  AigEdge
  new_or(
    AigEdge fanin0, ///< [in] ファンイン0
    AigEdge fanin1  ///< [in] ファンイン1
  )
  {
    // DeMorgan の法則
    return ~new_and(~fanin0, ~fanin1);
  }

  /// @brief xORノードを作る．
  /// @return 作成したノードを指す枝を返す．
  AigEdge
  new_xor(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  )
  {
    // ab' + a'b を作る．
    auto h1 = new_and(fanin0, ~fanin1);
    auto h2 = new_and(~fanin0, fanin1);
    return new_or(h1, h2);
  }

  /// @brief AND ノードを探す．
  ///
  /// なければ nullptr を返す．
  AigNode*
  find_and(
    const AigEdge& fanin0, ///< [in] ファンイン0のハンドル
    const AigEdge& fanin1  ///< [in] ファンイン1のハンドル
  ) const
  {
    auto key = AigNode(0, fanin0, fanin1);
    auto p = mAndTable.find(&key);
    if ( p != mAndTable.end() ) {
      return *p;
    }
    return nullptr;
  }

  /// @brief DFF を作る．
  /// @return DFF番号を返す．
  SizeType
  new_dff(
    const std::string& name = {} ///< [in] 名前
  )
  {
    auto dff_id = mDffList.size();
    auto oid = mNodeArray.size();
    auto onode = new AigNode(oid, dff_id, 1);
    mNodeArray.push_back(std::unique_ptr<AigNode>{onode});
    mDffList.push_back(Dff{onode, nullptr, name});
    return dff_id;
  }

  /// @brief DFFのデータ入力をセットする．
  void
  set_dff_input(
    SizeType dff_id, ///< [in] 対象のDFF番号
    AigEdge src      ///< [in] データ入力の枝
  )
  {
    _check_dff_id(dff_id);
    auto input = AigTerminal::dff_input(dff_id, src);
    mDffList[dff_id].input = std::unique_ptr<AigTerminal>{input};
    _set_terminal(input, src);
    _inc_node_ref(input);
  }

  /// @brief DFF名をセットする．
  void
  set_dff_name(
    SizeType dff_id,        ///< [in] DFF番号
    const std::string& name ///< [in] 名前
  )
  {
    _check_dff_id(dff_id);
    mDffList[dff_id].name = name;
  }

  /// @brief local rewriting を行う．
  void
  rewrite();

  /// @brief 参照回数が0のノードを取り除く
  ///
  /// ノードのID番号が変わる可能性がある．
  void
  cleanup();

  /// @brief コメントを追加する．
  void
  add_comment(
    const std::string& comment ///< [in] コメント
  )
  {
    mCommentList.push_back(comment);
  }

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
  // その他
  //////////////////////////////////////////////////////////////////////

  /// @brief dot 形式で出力する．
  void
  gen_dot(
    std::ostream& s,        ///< [in] 出力ストリーム
    const JsonValue& option ///< [in] オプションを表す JSON オブジェクト
  ) const;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // ちょっとお行儀のわるい関数
  // 必ずしもボトムアップにノードを作るとは限らないのでこの関数では
  // 参照回数の更新を行わない．
  // 最後に _finalize() で参照回数を正しく設定する必要がある．
  // なので private 関数となっている．
  // read_aig() や copy() で用いられる．
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を指定して配列のサイズを設定する．
  ///
  /// - 内容は未定
  void
  _initialize(
    SizeType node_num,   ///< [in] ノード数
    SizeType input_num,  ///< [in] 入力数
    SizeType output_num, ///< [in] 出力数
    SizeType dff_num     ///< [in] DFF数
  );

  /// @brief ノードを入力ノードとする．
  ///
  /// - id のノードの内容を入力ノードとする．
  /// - そのノードを input_id 番目の入力とする．
  void
  _set_input(
    SizeType id,      ///< [in] ID番号
    SizeType input_id ///< [in] 入力番号
  )
  {
    if ( mNodeArray.size() <= id ) {
      throw std::invalid_argument{"id is larger than mNodeArray"};
    }
    if ( mInputList.size() <= input_id ) {
      throw std::invalid_argument{"input_id is larger than mInputList"};
    }

    auto node = mNodeArray[id].get();
    node->_set_input(input_id);
    mInputList[input_id].node = node;
  }

  /// @brief ノードをDFFの出力ノードにする．
  ///
  /// - id のノードの内容を DFF の出力ノードとする．
  /// - そのノードを dff_id 番目の DFF の出力とする．
  void
  _set_dff_output(
    SizeType id,    ///< [in] ID番号
    SizeType dff_id ///< [in] DFF番号
  )
  {
    auto node = mNodeArray[id].get();
    node->_set_dff_output(dff_id);
    mDffList[dff_id].output = node;
  }

  /// @brief AND ノードの情報を設定する．
  void
  _set_and(
    AigNode* node,  ///< [in] 対象のノード
    AigEdge fanin0, ///< [in] ファンイン0
    AigEdge fanin1  ///< [in] ファンイン1
  )
  {
    node->_set_and(fanin0, fanin1);
    mAndTable.insert(node);
    _add_fanout(fanin0, node);
    _add_fanout(fanin1, node);
  }

  /// @brief 端子のソースを設定する．
  void
  _set_terminal(
    AigTerminal* terminal, ///< [in] 対象の端子
    AigEdge edge           ///< [in] ソースの枝
  )
  {
    terminal->_set_edge(edge);
    _add_fanout(edge, terminal);
  }

  /// @brief 出力端子のソースを設定する．
  void
  _set_output(
    SizeType output_id, ///< [in] 出力番号
    AigEdge edge        ///< [in] ソースの枝
  )
  {
    _set_terminal(output(output_id), edge);
  }

  /// @brief DFFのソースを設定する．
  void
  _set_dff_input(
    SizeType dff_id, ///< [in] DFF番号
    AigEdge edge     ///< [in] ソースの枝
  )
  {
    _set_terminal(dff_input(dff_id), edge);
  }

  /// @brief ノードの情報をコピーする．
  void
  _copy_node(
    AigNode* src_node ///< [in] コピー元のノード
  );

  /// @brief コピー元の枝に対応する枝を返す．
  AigEdge
  _copy_edge(
    AigEdge src_edge
  )
  {
    if ( src_edge.is_const() ) {
      // 定数ならそのまま返す．
      return src_edge;
    }
    auto src_node = src_edge.node();
    auto inv = src_edge.inv();
    auto dst_node = node(src_node->id());
    return AigEdge(dst_node, inv);
  }

  /// @brief 参照回数を適正化する．
  ///
  /// - ただし，参照回数が0のノードは削除されない．
  void
  _finalize();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード(枝)の置き換えを行う．
  void
  _replace(
    AigNode* old_node, ///< [in] 置き換え対象のノード
    AigEdge new_edge   ///< [in] 新しい枝
  );

  /// @brief 境界条件を調べる．
  /// @return 条件が成り立った時に true を返す．
  bool
  _special_case(
    AigEdge fanin0,   ///< [in] ファンイン0
    AigEdge fanin1,   ///< [in] ファンイン1
    AigEdge& new_edge ///< [out] 結果の枝
  )
  {
    if ( fanin0.is_zero() ) {
      new_edge = AigEdge::zero();
      return true;
    }
    if ( fanin1.is_zero() ) {
      new_edge = AigEdge::zero();
      return true;
    }
    if ( fanin0.is_one() ) {
      new_edge = fanin1;
      return true;
    }
    if ( fanin1.is_one() ) {
      new_edge = fanin0;
      return true;
    }
    if ( fanin0 == fanin1 ) {
      new_edge = fanin0;
      return true;
    }
    if ( fanin0 == ~fanin1 ) {
      new_edge = AigEdge::zero();
      return true;
    }
    return false;
  }

  /// @brief ファンアウト情報を追加する．
  void
  _add_fanout(
    AigEdge edge, ///< [in] ソースの枝
    AigNode* node ///< [in] ファンアウト先のノード
  )
  {
    if ( !edge.is_const() ) {
      auto src_node = edge.node();
      src_node->add_fanout(node);
    }
  }

  /// @brief ファンアウト情報を追加する．
  void
  _add_fanout(
    AigEdge edge,         ///< [in] ソースの枝
    AigTerminal* terminal ///< [in] ファンアウト先の端子
  )
  {
    if ( !edge.is_const() ) {
      auto src_node = edge.node();
      src_node->add_fanout(terminal);
    }
  }

  /// @brief ノードのファンインの順序を正規化する．
  void
  _normalize(
    AigNode* node
  )
  {
    auto fanin0 = node->fanin0();
    auto fanin1 = node->fanin1();
    if ( fanin0.node()->id() > fanin1.node()->id() ) {
      node->mFanins[0] = fanin1.mPackedData;
      node->mFanins[1] = fanin0.mPackedData;
    }
  }

  /// @brief ノードを無効化する．
  void
  _deactivate(
    AigNode* node ///< [in] 対象のノード
  );

  /// @brief ノードの参照回数を増やす．
  ///
  /// 場合によってはファンインのノードに再帰する．
  void
  _inc_node_ref(
    AigNode* node ///< [in] 対象のノード
  );

  /// @brief 出力端子に接続しているノードの参照回数を増やす．
  void
  _inc_node_ref(
    AigTerminal* terminal
  )
  {
    if ( terminal != nullptr ) {
      auto edge = terminal->edge();
      if ( !edge.is_const() ) {
	_inc_node_ref(edge.node());
      }
    }
  }

  /// @brief ノードの参照回数を減らす．
  ///
  /// 場合によってはファンインのノードに再帰する．
  /// 参照回数が 0 になったノードは sweep() で削除される．
  void
  _dec_node_ref(
    AigNode* node
  );

  /// @brief 整合性が取れているかチェックする．
  /// @return 正常なら true を返す．
  ///
  /// - 以下の項目をチェックする．
  ///   * 外部入力ノードの入力番号が入力数の範囲内に入っている．
  ///   * DFF出力ノードのDFF番号がDFF数の範囲内に入っている．
  ///   * ラッチ出力ノードのラッチ番号がラッチ数の範囲内に入っている．
  ///   * 外部出力の出力番号が出力数の範囲内に入っている．
  ///   * DFFの制御信号のDFF番号がDFF数の範囲内に入っている．
  ///   * ラッチの制御信号のラッチ番号がラッチ数の範囲内に入っている．
  ///   * ANDノードのファンインが定数ノードではない．
  ///   * ANDノードの2つのファンインが同じではない．
  ///     ANDノードの2つのファンインが同じノードを指す逆相の枝ならOK
  ///   * ANDノードの参照が循環していない．
  bool
  _sanity_check(
    std::vector<std::string>& err_list ///< [out] エラーメッセージを入れるリスト
  );

  /// @brief ファンインの情報とファンアウトの情報の整合性が取れているか調べる．
  void
  _fo_check();

  /// @brief ノード番号が適正か調べる．
  void
  _check_node_id(
    SizeType id
  ) const
  {
    if ( id >= node_num() ) {
      throw std::out_of_range{"'id' is out of range"};
    }
  }

  /// @brief 入力番号が適正か調べる．
  void
  _check_input_id(
    SizeType input_id
  ) const
  {
    if ( input_id >= input_num() ) {
      throw std::out_of_range{"'input_id' is out of range"};
    }
  }

  /// @brief 出力番号が適正か調べる．
  void
  _check_output_id(
    SizeType output_id
  ) const
  {
    if ( output_id >= output_num() ) {
      throw std::out_of_range{"'output_id' is out of range"};
    }
  }

  /// @brief DFF番号が適正か調べる．
  void
  _check_dff_id(
    SizeType dff_id
  ) const
  {
    if ( dff_id >= dff_num() ) {
      throw std::out_of_range{"'dff_id' is out of range"};
    }
  }

  /// @brief 入力のチェックを行う．
  void
  _check_input(
    SizeType input_id,
    std::vector<std::string>& err_list ///< [out] エラーメッセージを入れるリスト
  );

  /// @brief 出力のチェックを行う．
  void
  _check_output(
    SizeType output_id,
    std::vector<std::string>& err_list ///< [out] エラーメッセージを入れるリスト
  );

  /// @brief DFFのチェックを行う．
  void
  _check_dff(
    SizeType dff_id,
    std::vector<std::string>& err_list ///< [out] エラーメッセージを入れるリスト
  );

  /// @brief ノードのチェックを行う．
  void
  _check_node(
    SizeType node_id,
    std::vector<std::string>& err_list ///< [out] エラーメッセージを入れるリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力の情報を表す構造体
  struct Input {
    AigNode* node{nullptr}; ///< ノード
    std::string name;       ///< 名前
  };

  // 出力の情報を表す構造体
  struct Output {
    std::unique_ptr<AigTerminal> terminal; ///< 出力端子
    std::string name;                      ///< 名前
  };

  // DFFの情報を表す構造体
  struct Dff {
    AigNode* output{nullptr};           ///< 出力に対応するノード
    std::unique_ptr<AigTerminal> input; ///< 入力端子
    std::string name;                   ///< 名前
  };

  // ID番号をキーにして AigNode を収めた配列
  // AigNode の所有権を持つ．
  std::vector<std::unique_ptr<AigNode>> mNodeArray;

  // 入力情報のリスト
  std::vector<Input> mInputList;

  // 出力のリスト
  std::vector<Output> mOutputList;

  // DFFのリスト
  std::vector<Dff> mDffList;

  // ANDノードの構造ハッシュ
  AigTable mAndTable;

  // コメントのリスト
  std::vector<std::string> mCommentList;

};

END_NAMESPACE_YM_AIG

#endif // AIGMGRIMPL_H
