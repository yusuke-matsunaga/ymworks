#ifndef MODELIMPL_H
#define MODELIMPL_H

/// @file ModelImpl.h
/// @brief ModelImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"
#include "ym/JsonValue.h"
#include "NodeImpl.h"
#include "DffImpl.h"
#include "FuncMgr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class ModelImpl ModelImpl.h "ModelImpl.h"
/// @brief BnModel の内部情報を表すクラス
///
/// 関連する全てのオブジェクトの所有権を持つ．
/// ModelImpl 自体は複数のオブジェクトから参照されるので共有ポインタ
/// を使いたいケースだが，BnNode のように実際には NodeImpl を参照して
/// いるが，意味的にはその親である ModelImpl を参照していることにしたい
/// ので手製の shared_ptr を用いる．
//////////////////////////////////////////////////////////////////////
class ModelImpl
{
public:

  /// @brief コンストラクタ
  ModelImpl();

  /// @brief コピーコンストラクタ
  ModelImpl(
    const ModelImpl& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~ModelImpl();

  /// @brief 複製を返す．
  ModelImpl*
  copy() const
  {
    return new ModelImpl{*this};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション情報を返す．
  ///
  /// 以下の情報を持つ JSON の辞書オブジェクトが返される．
  /// - 名前: 'name'
  /// - 入力名:
  /// - 出力名:
  /// - DFF名:
  /// 該当する情報を持たない場合にはその項目は含まれない．
  JsonValue
  option() const;

  /// @brief 名前を返す．
  std::string
  name() const
  {
    return mName;
  }

  /// @brief コメントを返す．
  const std::vector<std::string>&
  comment_list() const
  {
    return mCommentList;
  }

  /// @brief DFF数を返す．
  SizeType
  dff_num() const
  {
    return mDffList.size();
  }

  /// @brief DFFを取り出す．
  const DffImpl*
  dff_impl(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    _check_dff_id(dff_id, "dff_impl");
    return mDffList[dff_id];
  }

  /// @brief DFF名を返す．
  std::string
  dff_name(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    auto key = dff_key(dff_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return {};
  }

  /// @brief DFFのリストを返す．
  const std::vector<const DffImpl*>&
  dff_list() const
  {
    return mDffList;
  }

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeList.size();
  }

  /// @brief ノードを取り出す．
  const NodeImpl*
  node_impl(
    SizeType id ///< [in] ID番号
  ) const
  {
    _check_node_id(id, "node_impl");
    return mNodeList[id];
  }

  /// @brief ノードのリストを返す．
  const std::vector<const NodeImpl*>&
  node_list() const
  {
    return mNodeList;
  }

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputList.size();
  }

  /// @brief 入力のノードを返す．
  const NodeImpl*
  input(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    _check_input_id(input_id, "input_id");
    return mInputList[input_id];
  }

  /// @brief 入力名を返す．
  std::string
  input_name(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    auto key = input_key(input_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return {};
  }

  /// @brief 入力のノードのリストを返す．
  const std::vector<const NodeImpl*>&
  input_list() const
  {
    return mInputList;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputList.size();
  }

  /// @brief 出力のノードを返す．
  const NodeImpl*
  output(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    _check_output_id(output_id, "output_id");
    return mOutputList[output_id];
  }

  /// @brief 出力名を返す．
  std::string
  output_name(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    auto key = output_key(output_id);
    if ( mSymbolDict.count(key) > 0 ) {
      return mSymbolDict.at(key);
    }
    return {};
  }

  /// @brief 出力のノードのリストを返す．
  const std::vector<const NodeImpl*>&
  output_list() const
  {
    return mOutputList;
  }

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const
  {
    return mLogicList.size();
  }

  /// @brief 論理ノードを返す．
  const NodeImpl*
  logic(
    SizeType pos ///< [in] 位置 ( 0 <= pos < logic_num() )
  ) const
  {
    _check_logic_id(pos, "logic_id");
    return mLogicList[pos];
  }

  /// @brief 論理ノードのリストを返す．
  const std::vector<const NodeImpl*>&
  logic_list() const
  {
    return mLogicList;
  }

  /// @brief 関数の数を返す．
  SizeType
  func_num() const
  {
    return mFuncMgr.func_num();
  }

  /// @brief 関数情報を返す．
  const FuncImpl*
  func_impl(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() )
  ) const
  {
    return mFuncMgr.func(func_id);
  }

  /// @brief 内容を出力する．
  void
  write(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす
  void
  inc_ref() const
  {
    ++ mRefCount;
  }

  /// @brief 参照回数を減らす
  void
  dec_ref() const
  {
    -- mRefCount;
    if ( mRefCount == 0 ) {
      // 自殺
      delete this;
    }
  }

  /// @brief オプション情報をセットする．
  void
  set_option(
    const JsonValue& option ///< [in] 設定するオプション
  );

  /// @brief 名前を設定する．
  void
  set_name(
    const std::string& name ///< [in] 名前
  )
  {
    mName = name;
  }

  /// @brief コメントを追加する．
  void
  add_comment(
    const std::string& comment ///< [in] コメント
  )
  {
    mCommentList.push_back(comment);
  }

  /// @brief 入力名をセットする．
  void
  set_input_name(
    SizeType input_id,      ///< [in] 入力番号 ( 0 <= input_id < input_num() )
    const std::string& name ///< [in] 名前
  )
  {
    auto key = input_key(input_id);
    mSymbolDict[key] = name;
  }

  /// @brief 出力名をセットする．
  void
  set_output_name(
    SizeType output_id,      ///< [in] 出力番号 ( 0 <= output_id < output_num() )
    const std::string& name  ///< [in] 名前
  )
  {
    auto key = output_key(output_id);
    mSymbolDict[key] = name;
  }

  /// @brief DFF名をセットする．
  void
  set_dff_name(
    SizeType dff_id,        ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
    const std::string& name ///< [in] 名前
  )
  {
    auto key = dff_key(dff_id);
    mSymbolDict[key] = name;
  }

  /// @brief DFFの入力のノード番号をセットする．
  void
  set_dff_src(
    SizeType dff_id,    ///< [in] DFF番号
    const NodeImpl* src ///< [in] DFFの入力のノード
  )
  {
    _check_dff_id(dff_id, "set_dff_src");
    mDffArray[dff_id]->mSrc = src;
  }

  /// @brief 新しい DFFを作る．
  /// @return DFF番号を返す．
  SizeType
  new_dff(
    const std::string& name = {}, ///< [in] 名前
    char reset_val = 'X'          ///< [in] リセット値
  )
  {
    auto id = mDffArray.size();
    auto dff = new DffImpl(this, id, nullptr, nullptr, reset_val);
    mDffArray.push_back(std::unique_ptr<DffImpl>{dff});
    mDffList.push_back(dff);
    if ( name != std::string{} ) {
      set_dff_name(id, name);
    }
    return id;
  }

  /// @brief 新しい入力ノードを作る．
  /// @return ノードを返す．
  const NodeImpl*
  new_input(
    const std::string& name = {} ///< [in] 名前
  );

  /// @brief 新しいDFF出力ノードを作る．
  /// @return ノードを返す．
  const NodeImpl*
  new_dff_output(
    SizeType dff_id ///< [in] DFF番号
  );

  /// @brief 新しい出力ノードを作る．
  /// @return 出力番号を返す．
  SizeType
  new_output(
    const NodeImpl* src,         ///< [in] ソースのノード
    const std::string& name = {} ///< [in] 名前
  );

  /// @brief 新しい論理ノードを作る．
  ///
  /// @return ノードを返す．
  const NodeImpl*
  new_logic(
    const FuncImpl* func,                          ///< [in] 関数
    const std::vector<const NodeImpl*>& fanin_list ///< [in] ファンインのノードのリスト
  );

  /// @brief プリミティブを登録する．
  /// @return 関数を返す．
  const FuncImpl*
  reg_primitive(
    SizeType input_num,     ///< [in] 入力数
    PrimType primitive_type ///< [in] プリミティブの種類
  )
  {
    return mFuncMgr.reg_primitive(this, input_num, primitive_type);
  }

  /// @brief カバーを登録する．
  /// @return 関数を返す．
  const FuncImpl*
  reg_cover(
    const SopCover& input_cover, ///< [in] 入力カバー
    bool output_inv              ///< [in] 出力の反転属性
  )
  {
    return mFuncMgr.reg_cover(this, input_cover, output_inv);
  }

  /// @brief 論理式を登録する．
  /// @return 関数を返す．
  const FuncImpl*
  reg_expr(
    const Expr& expr ///< [in] 論理式
  )
  {
    return mFuncMgr.reg_expr(this, expr);
  }

  /// @brief 真理値表を登録する．
  /// @return 関数を返す．
  const FuncImpl*
  reg_tvfunc(
    const TvFunc& func ///< [in] 真理値表型の関数
  )
  {
    return mFuncMgr.reg_tvfunc(this, func);
  }

  /// @brief BDDを登録する．
  /// @return 関数を返す．
  const FuncImpl*
  reg_bdd(
    const Bdd& bdd ///< [in] BDD
  )
  {
    return mFuncMgr.reg_bdd(this, bdd);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを生成して登録する．
  /// @return ノードを返す．
  const NodeImpl*
  new_node(
    std::function<NodeImpl*(SizeType id)> new_func ///< [in] 生成関数
  )
  {
    auto id = mNodeArray.size();
    auto node = new_func(id);
    mNodeArray.push_back(std::unique_ptr<NodeImpl>{node});
    mNodeList.push_back(node);
    return node;
  }

  /// @brief mSymbolDict 用の入力名のキーを返す．
  std::string
  input_key(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < input_num() )
  ) const
  {
    _check_input_id(input_id, "input_key");
    std::ostringstream buf;
    buf << "i" << input_id;
    auto key = buf.str();
    return key;
  }

  /// @brief mSymbolDict 用の出力名のキーを返す．
  std::string
  output_key(
    SizeType output_id ///< [in] 出力番号 ( 0 <= output_id < output_num() )
  ) const
  {
    _check_output_id(output_id, "set_output_id");
    std::ostringstream buf;
    buf << "o" << output_id;
    auto key = buf.str();
    return key;
  }

  /// @brief mSymbolDict 用のDFF名のキーを返す．
  std::string
  dff_key(
    SizeType dff_id ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
  ) const
  {
    _check_dff_id(dff_id, "set_dff_name");
    std::ostringstream buf;
    buf << "q" << dff_id;
    auto key = buf.str();
    return key;
  }

  /// @brief 入力番号をチェックする．
  void
  _check_input_id(
    SizeType input_id,
    const char* func_name
  ) const
  {
    if ( input_id >= input_num() ) {
      std::ostringstream buf;
      buf << "Error in "
	  << func_name << ": input_id is out of range";
      throw std::out_of_range{buf.str()};
    }
  }

  /// @brief 出力番号をチェックする．
  void
  _check_output_id(
    SizeType output_id,
    const char* func_name
  ) const
  {
    if ( output_id >= output_num() ) {
      std::ostringstream buf;
      buf << "Error in "
	  << func_name << ": output_id is out of range";
      throw std::out_of_range{buf.str()};
    }
  }

  /// @brief DFF番号をチェックする．
  void
  _check_dff_id(
    SizeType dff_id,
    const char* func_name
  ) const
  {
    if ( dff_id >= dff_num() ) {
      std::ostringstream buf;
      buf << "Error in "
	  << func_name << ": dff_id is out of range";
      throw std::out_of_range{buf.str()};
    }
  }

  /// @brief ノード番号をチェックする．
  void
  _check_node_id(
    SizeType node_id,
    const char* func_name
  ) const
  {
    if ( node_id >= node_num() ) {
      std::ostringstream buf;
      buf << "Error in "
	  << func_name << ": node_id is out of range";
      throw std::out_of_range{buf.str()};
    }
  }

  /// @brief 論理ノード番号をチェックする．
  void
  _check_logic_id(
    SizeType pos,
    const char* func_name
  ) const
  {
    if ( pos >= logic_num() ) {
      std::ostringstream buf;
      buf << "Error in "
	  << func_name << ": pos is out of range";
      throw std::out_of_range{buf.str()};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  mutable
  SizeType mRefCount{0};

  // 名前
  std::string mName;

  // コメントのりスト
  std::vector<std::string> mCommentList;

  // シンボル名の辞書
  std::unordered_map<std::string, std::string> mSymbolDict;

  // NodeImplの配列
  // NodeImpl の所有権を持つ．
  std::vector<std::unique_ptr<NodeImpl>> mNodeArray;

  // 全ノードのリスト
  // 内容は mNodeArray と同じ
  std::vector<const NodeImpl*> mNodeList;

  // 入力のノードのリスト
  std::vector<const NodeImpl*> mInputList;

  // 出力のノードのリスト
  // 厳密には出力の入力となっているノードのリスト
  std::vector<const NodeImpl*> mOutputList;

  // DFF情報の配列
  // DffImpl の所有権を持つ．
  std::vector<std::unique_ptr<DffImpl>> mDffArray;

  // DFFのリスト
  // 内容は mDffArray と同じ
  std::vector<const DffImpl*> mDffList;

  // 論理ノードのリスト
  std::vector<const NodeImpl*> mLogicList;

  // 関数情報のマネージャ
  FuncMgr mFuncMgr;

};

END_NAMESPACE_YM_BN

#endif // MODELIMPL_H
