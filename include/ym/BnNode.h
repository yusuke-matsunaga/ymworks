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
#include "ym/BnBase.h"


BEGIN_NAMESPACE_YM_BN

class NodeImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnNode BnNode.h "BnNode.h"
/// @brief BnModel のノードを表すクラス
///
/// 以下の情報を持つ．
/// - ID番号
/// - 種類(None, Input, Logic)
///
/// - None は通常用いられない．不正値を表す
///
/// - Input は外部入力もしくはDFFの出力を表す．
///   * 外部入力の場合には入力番号を持つ．
///   * DFFの出力の場合にはDFF番号とDFFの入力のノード番号を持つ．
///
/// - Logic は論理ノードを表す．
///   論理ノードはファンイン番号のリストとローカル関数の情報を持つ．
///
/// - ローカル関数以下のいずれかのタイプで表される．
///   * プリミティブタイプ
///   * 入力カバーと出力の反転フラグ
///   * 論理式
///   * 真理値表
///   * BDD
///
/// 空のコンストラクタで作られたインスタンスは不正値となる．
/// その場合，is_valid() は false を返し，id() は BAD_ID を返すが，
/// それ以外のメンバ関数の呼び出しは std::invalid_argument 例外を送出する．
///
/// 公開されているメンバ関数はすべて const であり，内容を変更することは
/// できない．
///
/// 通常は BnModel のみが生成/設定を行う．
//////////////////////////////////////////////////////////////////////
class BnNode :
  public BnBase
{
  friend class BnBase;

public:
  //////////////////////////////////////////////////////////////////////
  // BnNode に関係する型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類を表す列挙型
  enum Type {
    NONE,  ///< 不正値
    INPUT, ///< 入力ノード
    LOGIC  ///< 論理ノード
  };


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnNode() = default;

  /// @brief デストラクタ
  ~BnNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通なインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief ノードの種類を返す．
  Type
  type() const;

  /// @brief ノードの種類を表す文字列を返す．
  std::string
  type_str() const;

  /// @brief 入力ノードの時 true を返す．
  bool
  is_input() const;

  /// @brief 論理ノードの時 true を返す．
  bool
  is_logic() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ノードに対してのみ有効なインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードの時 true を返す．
  ///
  /// is_input() を包含している．
  bool
  is_primary_input() const;

  /// @brief DFFの出力ノードの時 true を返す．
  ///
  /// is_input() を包含している．
  bool
  is_dff_output() const;

  /// @brief 入力番号を返す．
  ///
  /// - is_primary_input() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  SizeType
  input_id() const;

  /// @brief DFF番号を返す．
  ///
  /// - is_dff_output() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  SizeType
  dff_id() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理ノードに対してのみ有効なインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数情報を返す．
  BnFunc
  func() const;

  /// @brief ファンイン数を返す．
  ///
  /// - is_logic() が true の時のみ意味を持つ．
  /// - is_logic() が false の時は 0 を返す．
  SizeType
  fanin_num() const;

  /// @brief ファンインのノードを返す．
  ///
  /// - is_logic() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  /// - pos が範囲外の時は std::out_of_range 例外を送出する．
  BnNode
  fanin(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < fanin_num() )
  ) const;

  /// @brief ファンインのノードのリストを返す．
  ///
  /// - is_logic() が true の時のみ意味を持つ．
  /// - is_logic() が false の時は空リストを返す．
  BnNodeList
  fanin_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnNode& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return BnBase::operator==(right) && mId == right.mId;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnNode& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ///
  /// これは BnBase のみが使用する．
  BnNode(
    const std::shared_ptr<ModelImpl>& model, ///< [in] 親のモデル．
    SizeType id                              ///< [in] ノード番号
  );

  /// @brief ノードの実体を返す．
  const NodeImpl&
  _node_impl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  SizeType mId{BAD_ID};

};

END_NAMESPACE_YM_BN

#endif // BNNODE_H
