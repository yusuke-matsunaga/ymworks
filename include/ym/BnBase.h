#ifndef BNBASE_H
#define BNBASE_H

/// @file BnBase.h
/// @brief BnBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnBase BnBase.h "BnBase.h"
/// @brief ModelImpl の共有ポインタを持つ基底クラス
///
/// 意味的には BnModel のみが ModelImpl を所有しているが，BnNode や BnFunc
/// の情報を ModelImpl が保持しているので BnNode や BnFunc も ModelImpl
/// への参照を持つ．そのため，複数のインスタンスが ModelImpl への参照を持つ
/// ことになるので共有ポインタを用いて管理するようにしている．
/// このクラスはその共有ポインタを持つクラスの基底クラスとなっている．
//////////////////////////////////////////////////////////////////////
class BnBase
{
protected:

  /// @brief 空のコンストラクタ
  ///
  /// nullptr を持つ．
  BnBase();

  /// @brief 内容を指定したコンストラクタ
  BnBase(
    const std::shared_ptr<ModelImpl>& impl
  );

  /// @brief 内容を指定したコンストラクタ
  BnBase(
    std::shared_ptr<ModelImpl>&& impl
  );

  /// @brief デストラクタ
  ~BnBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mImpl.get() != nullptr;
  }

  /// @brief 不正な値を持っている時 true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 等価比較関数
  ///
  /// 意味的ではなく形式的な等価を調べる．
  bool
  operator==(
    const BnBase& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較関数
  ///
  /// 意味的ではなく形式的な等価を調べる．
  bool
  operator!=(
    const BnBase& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ModelImpl のオブジェクトをセットする．
  void
  _set_impl(
    ModelImpl* impl ///< [in] 設定するオブジェクト
  );

  /// @brief DFF番号を BnDff に変換する．
  BnDff
  _id2dff(
    SizeType id ///< [in] ノード番号
  ) const;

  /// @brief DFF番号のリストを BnDff のリストに変換する．
  BnDffList
  _id2dff_list(
    const std::vector<SizeType>& id_list ///< [in] DFF番号のりスト
  ) const;

  /// @brief ノード番号を BnNode に変換する．
  BnNode
  _id2node(
    SizeType id ///< [in] ノード番号
  ) const;

  /// @brief ノード番号のリストを BnNode のリストに変換する．
  BnNodeList
  _id2node_list(
    const std::vector<SizeType>& id_list ///< [in] ノード番号のりスト
  ) const;

  /// @brief BnNode をノード番号に変換する．
  ///
  /// BnNode が同じ ModelImpl に属していない場合には例外を送出する．
  SizeType
  _node2id(
    const BnNode& node ///< [in] ノード
  ) const;

  /// @brief 関数番号を BnFunc に変換する．
  BnFunc
  _id2func(
    SizeType id ///< [in] 関数番号
  ) const;

  /// @brief 同じ ModelImpl を持つオブジェクトかチェックする．
  bool
  _check_impl(
    const BnBase& base
  ) const;

  /// @brief BnDff のチェックを行う．
  ///
  /// 同じ Model に属していないとエラーとなる．
  void
  _check_dff(
    const BnDff& dff
  ) const;

  /// @brief BnNode のチェックを行う．
  ///
  /// 同じ Model に属していないとエラーとなる．
  void
  _check_node(
    const BnNode& node
  ) const;

  /// @brief BnFunc のチェックを行う．
  ///
  /// 同じ Model に属していないとエラーとなる．
  void
  _check_func(
    const BnFunc& func
  ) const;

  /// @brief ModelImpl を返す．
  const ModelImpl&
  _model_impl() const;

  /// @brief ModelImpl を返す．
  ModelImpl&
  _model_impl();

  const std::shared_ptr<ModelImpl>&
  _model() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装本体
  std::shared_ptr<ModelImpl> mImpl{nullptr};

};

END_NAMESPACE_YM_BN

#endif // BNBASE_H
