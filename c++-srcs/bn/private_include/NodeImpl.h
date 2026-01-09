#ifndef NODEIMPL_H
#define NODEIMPL_H

/// @file NodelImpl.h
/// @brief NodeImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"
#include "ym/BnNode.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class NodeImpl NodeImpl.h "NodeImpl.h"
/// @brief BnNode の実装クラス
//////////////////////////////////////////////////////////////////////
class NodeImpl
{
public:

  /// @brief コンストラクタ
  NodeImpl() = default;

  /// @brief デストラクタ
  virtual
  ~NodeImpl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラス生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードを作る．
  static
  NodeImpl*
  new_primary_input(
    SizeType input_id ///< [in] 入力番号
  );

  /// @brief DFF出力ノードを作る．
  static
  NodeImpl*
  new_dff_output(
    SizeType dff_id ///< [in] DFF番号
  );

  /// @brief 論理ノードを作る．
  static
  NodeImpl*
  new_logic(
    SizeType func_id,                       ///< [in] 関数番号
    const std::vector<SizeType>& fanin_list ///< [in] ファンインのノード番号のりスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 共通のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を返す．
  virtual
  BnNode::Type
  type() const = 0;

  /// @brief 入力ノードの時 true を返す．
  ///
  /// 外部入力ノードとDFFの出力ノードのどちらかの時 true となる．
  virtual
  bool
  is_input() const;

  /// @brief 論理ノードの時 true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief 複製を作る．
  virtual
  std::unique_ptr<NodeImpl>
  copy() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ノードのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードの時 true を返す．
  virtual
  bool
  is_primary_input() const;

  /// @brief DFFの出力の時 true を返す．
  virtual
  bool
  is_dff_output() const;

  /// @brief 入力番号を返す．
  virtual
  SizeType
  input_id() const;

  /// @brief DFF番号を返す．
  virtual
  SizeType
  dff_id() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理ノードのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数番号を返す．
  virtual
  SizeType
  func_id() const;

  /// @brief ファンイン数を返す．
  virtual
  SizeType
  fanin_num() const;

  /// @brief ファンインのノード番号を返す．
  virtual
  SizeType
  fanin_id(
    SizeType pos ///< [in] 位置 ( 0 <= pos < fanin_num() )
  ) const;

  /// @brief ファンイン番号のリストを返す．
  virtual
  const std::vector<SizeType>&
  fanin_id_list() const;

};

END_NAMESPACE_YM_BN

#endif // NODEIMPL_H
