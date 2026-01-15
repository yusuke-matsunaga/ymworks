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
#include "ImplBase.h"


BEGIN_NAMESPACE_YM_BN

class FuncImpl;

//////////////////////////////////////////////////////////////////////
/// @class NodeImpl NodeImpl.h "NodeImpl.h"
/// @brief BnNode の実装クラス
//////////////////////////////////////////////////////////////////////
class NodeImpl :
  public ImplBase
{
public:

  /// @brief コンストラクタ
  NodeImpl(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id             ///< [in] ID番号
  ) : ImplBase(model),
      mId{id}
  {
  }

  /// @brief デストラクタ
  virtual
  ~NodeImpl()
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラス生成用のクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードを作る．
  static
  NodeImpl*
  new_primary_input(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id,            ///< [in] ID番号
    SizeType input_id       ///< [in] 入力番号
  );

  /// @brief DFF出力ノードを作る．
  static
  NodeImpl*
  new_dff_output(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id,            ///< [in] ID番号
    SizeType dff_id         ///< [in] DFF番号
  );

  /// @brief 論理ノードを作る．
  static
  NodeImpl*
  new_logic(
    const ModelImpl* model,                        ///< [in] 親のモデル
    SizeType id,                                   ///< [in] ID番号
    const FuncImpl* func,                          ///< [in] 関数
    const std::vector<const NodeImpl*>& fanin_list ///< [in] ファンインのリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 共通のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

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

  /// @brief 関数を返す．
  virtual
  const FuncImpl*
  func() const;

  /// @brief ファンイン数を返す．
  virtual
  SizeType
  fanin_num() const;

  /// @brief ファンインのノードを返す．
  virtual
  const NodeImpl*
  fanin(
    SizeType pos ///< [in] 位置 ( 0 <= pos < fanin_num() )
  ) const;

  /// @brief ファンインのリストを返す．
  virtual
  const std::vector<const NodeImpl*>&
  fanin_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

};

END_NAMESPACE_YM_BN

#endif // NODEIMPL_H
