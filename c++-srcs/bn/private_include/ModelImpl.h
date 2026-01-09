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
#include "FuncMgr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class DffImpl ModelImpl.h "ModelImpl.h"
/// @brief DFFの情報を表す構造体
//////////////////////////////////////////////////////////////////////
struct DffImpl
{
  std::string name; ///< [in] 名前
  SizeType id;      ///< [in] 出力のノード番号
  SizeType src_id;  ///< [in] 入力のノード番号
  char reset_val;   ///< [in] リセット値 ('X', '0', '1')
};


//////////////////////////////////////////////////////////////////////
/// @class ModelImpl ModelImpl.h "ModelImpl.h"
/// @brief BnModel の内部情報を表すクラス
///
/// 関連する全てのオブジェクトの所有権を持つ．
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
  const DffImpl&
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
    _check_dff_id(dff_id, "dff_name");
    return mDffList[dff_id].name;
  }

  /// @brief ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeArray.size();
  }

  /// @brief ノードを取り出す．
  const NodeImpl&
  node_impl(
    SizeType id ///< [in] ID番号
  ) const
  {
    _check_node_id(id, "node_impl");
    return *mNodeArray[id];
  }

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputList.size();
  }

  /// @brief 入力のノード番号を返す．
  SizeType
  input_id(
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
    _check_input_id(input_id, "input_name");
    auto id = mInputList[input_id];
    if ( mNameDict.count(id) > 0 ) {
      return mNameDict.at(id);
    }
    return {};
  }

  /// @brief 入力のノード番号のリストを返す．
  const std::vector<SizeType>&
  input_id_list() const
  {
    return mInputList;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputList.size();
  }

  /// @brief 出力のノード番号を返す．
  SizeType
  output_id(
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
    _check_output_id(output_id, "output_name");
    return mOutputNameList[output_id];
  }

  /// @brief 出力のノード番号のリストを返す．
  const std::vector<SizeType>&
  output_id_list() const
  {
    return mOutputList;
  }

  /// @brief 論理ノード数を返す．
  SizeType
  logic_num() const
  {
    return mLogicList.size();
  }

  /// @brief 論理ノードのノード番号を返す．
  SizeType
  logic_id(
    SizeType pos ///< [in] 位置 ( 0 <= pos < logic_num() )
  ) const
  {
    _check_logic_id(pos, "logic_id");
    return mLogicList[pos];
  }

  /// @brief 論理ノード番号のリストを返す．
  const std::vector<SizeType>&
  logic_id_list() const
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
  const FuncImpl&
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

  /// @brief 内容をクリアする．
  void
  clear();

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
    _check_input_id(input_id, "set_input_name");
    auto id = mInputList[input_id];
    mNameDict.emplace(id, name);
  }

  /// @brief 出力名をセットする．
  void
  set_output_name(
    SizeType output_id,      ///< [in] 出力番号 ( 0 <= output_id < output_num() )
    const std::string& name  ///< [in] 名前
  )
  {
    _check_output_id(output_id, "set_output_id");
    mOutputNameList[output_id] = name;
  }

  /// @brief DFF名をセットする．
  void
  set_dff_name(
    SizeType dff_id,        ///< [in] DFF番号 ( 0 <= dff_id < dff_num() )
    const std::string& name ///< [in] 名前
  )
  {
    _check_dff_id(dff_id, "set_dff_name");
    mDffList[dff_id].name = name;
  }

  /// @brief DFFの入力のノード番号をセットする．
  void
  set_dff_src(
    SizeType dff_id, ///< [in] DFF番号
    SizeType src_id  ///< [in] DFFの入力のノード番号
  )
  {
    _check_dff_id(dff_id, "set_dff_src");
    mDffList[dff_id].src_id = src_id;
  }

  /// @brief 新しいノード用の番号を確保する．
  ///
  /// @return ID番号を返す．
  SizeType
  alloc_node()
  {
    auto id = mNodeArray.size();
    mNodeArray.push_back(std::unique_ptr<NodeImpl>{nullptr});
    return id;
  }

  /// @brief 対応するID番号のノードを入力に設定する．
  void
  set_input(
    SizeType id,                 ///< [in] ID番号
    const std::string& name = {} ///< [in] 名前
  );

  /// @brief 対応するID番号のノードをDFFの出力に設定する．
  void
  set_dff_output(
    SizeType id,    ///< [in] ID番号
    SizeType dff_id ///< [in] DFF番号
  );

  /// @brief 論理ノードの情報をセットする．
  void
  set_logic(
    SizeType id,                            ///< [in] ID番号
    SizeType func_id,                       ///< [in] 関数番号
    const std::vector<SizeType>& fanin_list ///< [in] 入力の識別子番号のリスト
  );

  /// @brief ノード名をセットする．
  void
  set_node_name(
    SizeType id,            ///< [in] ノードID
    const std::string& name ///< [in] 名前
  );

  /// @brief 新しい DFFを作る．
  ///
  /// @return DFF番号を返す．
  SizeType
  new_dff(
    const std::string& name = {}, ///< [in] 名前
    char reset_val = 'X'          ///< [in] リセット値
  )
  {
    auto dff_id = mDffList.size();
    mDffList.push_back({name, BAD_ID, BAD_ID, reset_val});
    return dff_id;
  }

  /// @brief 新しい入力ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_input(
    const std::string& name = {} ///< [in] 名前
  )
  {
    auto id = alloc_node();
    set_input(id, name);
    return id;
  }

  /// @brief 新しいDFF出力ノードを作る．
  ///
  /// ID番号を返す．
  SizeType
  new_dff_output(
    SizeType dff_id ///< [in] DFF番号
  )
  {
    auto id = alloc_node();
    set_dff_output(id, dff_id);
    return id;
  }

  /// @brief 新しい出力ノードを作る．
  ///
  /// @return 出力番号を返す．
  SizeType
  new_output(
    SizeType src_id,             ///< [in] ソースのID番号
    const std::string& name = {} ///< [in] 名前
  )
  {
    auto oid = mOutputList.size();
    mOutputList.push_back(src_id);
    mOutputNameList.push_back(name);
    return oid;
  }

  /// @brief 新しい論理ノードを作る．
  ///
  /// @return ID番号を返す．
  SizeType
  new_logic(
    SizeType func_id,                       ///< [in] 関数番号
    const std::vector<SizeType>& fanin_list ///< [in] 入力の識別子番号のリスト
  )
  {
    auto id = alloc_node();
    set_logic(id, func_id, fanin_list);
    return id;
  }

  /// @brief 論理ノードのリストを作る．
  void
  make_logic_list();

  /// @brief プリミティブを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_primitive(
    SizeType input_num,     ///< [in] 入力数
    PrimType primitive_type ///< [in] プリミティブの種類
  )
  {
    return mFuncMgr.reg_primitive(input_num, primitive_type);
  }

  /// @brief カバーを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_cover(
    const SopCover& input_cover, ///< [in] 入力カバー
    bool output_inv              ///< [in] 出力の反転属性
  )
  {
    return mFuncMgr.reg_cover(input_cover, output_inv);
  }

  /// @brief 論理式を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_expr(
    const Expr& expr ///< [in] 論理式
  )
  {
    return mFuncMgr.reg_expr(expr);
  }

  /// @brief 真理値表を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_tvfunc(
    const TvFunc& func ///< [in] 真理値表型の関数
  )
  {
    return mFuncMgr.reg_tvfunc(func);
  }

  /// @brief BDDを登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_bdd(
    const Bdd& bdd ///< [in] BDD
  )
  {
    return mFuncMgr.reg_bdd(bdd);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トポロジカルソートを行い mLogicList にセットする．
  void
  order_node(
    SizeType id,                       ///< [in] ID番号
    std::unordered_set<SizeType>& mark ///< [in] マーク
  );

  /// @brief print() 中でノード名を出力する関数
  std::string
  node_name(
    SizeType id ///< [in] ID番号
  ) const;

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

  // ノード番号と名前を表す構造体
  struct NodeInfo {
    std::string name; ///< 名前
    SizeType id;      ///< ノード番号
  };

  // 名前
  std::string mName;

  // コメントのりスト
  std::vector<std::string> mCommentList;

  // NodeImplの配列
  // NodeImpl の所有権を持つ．
  std::vector<std::unique_ptr<NodeImpl>> mNodeArray;

  // 入力のノード番号のリスト
  std::vector<SizeType> mInputList;

  // 出力のノード番号のリスト
  std::vector<SizeType> mOutputList;

  // 出力名のりスト
  std::vector<std::string> mOutputNameList;

  // DFF情報のリスト
  std::vector<DffImpl> mDffList;

  // 論理ノード番号のリスト
  std::vector<SizeType> mLogicList;

  // ノード番号をキーにしてノード名を記録する辞書
  std::unordered_map<SizeType, std::string> mNameDict;

  // 関数情報のマネージャ
  FuncMgr mFuncMgr;

};

END_NAMESPACE_YM_BN

#endif // MODELIMPL_H
