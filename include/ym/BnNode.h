#ifndef BNNODE_H
#define BNNODE_H

/// @file BnNode.h
/// @brief BnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_BN

class NodeImpl;

//////////////////////////////////////////////////////////////////////
/// @ingroup BnGroup
/// @class BnNode BnNode.h "BnNode.h"
/// @brief BnModel のノードを表すクラス
/// @sa BnFunc, BnNodeList
///
/// 以下の情報を持つ．
/// - ID番号
/// - 種類(Input, Logic)
///
/// - Input は外部入力もしくはDFFの出力を表す．
///   * 外部入力の場合には入力番号を持つ．
///   * DFFの出力の場合にはDFF番号を持つ．
///
/// - Logic は論理ノードを表す．
///   論理ノードはファンインのリストとローカル関数の情報を持つ．
///
/// - ローカル関数の情報は BnFunc で表す．
///
/// 空のコンストラクタで作られたインスタンスは不正値となる．
/// その場合，is_valid() は false を返し， is_invalid() は true を返す．
/// それ以外のメンバ関数の呼び出しは std::logic_error 例外を送出する．
///
/// 公開されているメンバ関数はすべて const であり，内容を変更することは
/// できない．
//////////////////////////////////////////////////////////////////////
class BnNode
{
  friend class BnModel;
  friend class BnNodeList;
  friend class BnNodeIter;
  friend class BnNodeIter2;
  friend class BnDff;

public:
  //////////////////////////////////////////////////////////////////////
  /// @name BnNode に関係する型の定義
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を表す列挙型
  enum Type {
    INPUT, ///< 入力ノード
    LOGIC  ///< 論理ノード
  };

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 生成と破壊
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnNode() = default;

  /// @brief コピーコンストラクタ
  BnNode(
    const BnNode& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  BnNode&
  operator=(
    const BnNode& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BnNode();

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 共通なインターフェイス
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時に true を返す．
  bool
  is_valid() const
  {
    return mPtr != nullptr;
  }

  /// @brief 適正な値を持っていない時に true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief ノード番号を返す．
  /// @exception std::logic_error is_valid() = false の時
  SizeType
  id() const;

  /// @brief ノードの種類を返す．
  /// @exception std::logic_error is_valid() = false の時
  Type
  type() const;

  /// @brief ノードの種類を表す文字列を返す．
  /// @exception std::logic_error is_valid() = false の時
  std::string
  type_str() const;

  /// @brief 入力ノードの時 true を返す．
  /// @exception std::logic_error is_valid() = false の時
  bool
  is_input() const;

  /// @brief 論理ノードの時 true を返す．
  /// @exception std::logic_error is_valid() = false の時
  bool
  is_logic() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////



public:
  //////////////////////////////////////////////////////////////////////
  /// @name 入力ノードに対してのみ有効なインターフェイス
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードの時 true を返す．
  /// @exception std::logic_error is_valid() = false の時
  ///
  /// is_input() を包含している．
  bool
  is_primary_input() const;

  /// @brief DFFの出力ノードの時 true を返す．
  /// @exception std::logic_error is_valid() = false の時
  ///
  /// is_input() を包含している．
  bool
  is_dff_output() const;

  /// @brief 入力番号を返す．
  /// @exception std::logic_error is_valid() = false の時
  /// @exception std::is_primary_input() = false の時
  ///
  /// @code
  /// // node1.is_primary_input() = true が成り立っていると仮定する．
  /// auto node2 = model.input(node1.input_id())
  /// // node2 == node1
  /// @endcode
  SizeType
  input_id() const;

  /// @brief DFF番号を返す．
  /// @exception std::logic_error is_valid() = false の時
  /// @exception std::logic_error is_dff_output() = false の時
  ///
  /// @code
  /// // node1.is_dff_output() = true が成り立っていると仮定する．
  /// auto dff = model.dff(node1.dff_id());
  /// // dff.output() = node1
  /// @endcode
  SizeType
  dff_id() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理ノードに対してのみ有効なインターフェイス
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数情報を返す．
  /// @exception std::logic_error is_valid() = false の時
  /// @exception std::logic_error is_logic() = false の時
  /// @sa BnFunc
  BnFunc
  func() const;

  /// @brief ファンイン数を返す．
  /// @exception std::logic_error is_valid() = false の時
  ///
  /// - is_logic() = false の時は 0 を返す．
  SizeType
  fanin_num() const;

  /// @brief ファンインのノードを返す．
  /// @exception std::logic_error is_valid() = false の時
  /// @exception std::out_of_range 範囲外のアクセス
  BnNode
  fanin(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < fanin_num() )
  ) const;

  /// @brief ファンインのノードのリストを返す．
  /// @excneption std::logic_error is_valid() = false の時
  /// @sa BnNodeList
  ///
  /// - is_logic() が false の時は空リストを返す．
  ///
  /// @code
  /// // node1.is_logic() = true が成り立っていると仮定する．
  /// for ( auto node2: node1.fanin_list() ) {
  ///   ...
  /// }
  /// @endcode
  BnNodeList
  fanin_list() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////



public:
  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnNode& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnNode& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BnNode(
    const NodeImpl* ptr ///< [in] 実装オブジェクトへのポインタ
  );

  /// @brief ノードの実体を返す．
  const NodeImpl&
  _node_impl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装オブジェクトへのポインタ
  const NodeImpl* mPtr{nullptr};

};

END_NAMESPACE_YM_BN

#endif // BNNODE_H
