#ifndef OPNODE_H
#define OPNODE_H

/// @file OpNode.h
/// @brief OpNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/sat.h"
#include "ym/SatLiteral.h"
#include "ym/SatOrderedSet.h"
#include "ym/bc.h"
#include "TemplNode.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class OpNode OpNode.h "OpNode.h"
/// @brief 演算ノードを表す基底クラス
///
/// 項数の定まっていない演算を表す．
/// 個々のオペランドは0から始まる番号を持つ．
/// - 0 〜 (input_num() - 1) は入力を表す．
/// - input_num() 〜 (input_num() + op_num() - 1) は演算ノードを表す．
///   演算ノードの場合には実際の数字から input_num() を引いた値が演算番号を
///   表す．
//////////////////////////////////////////////////////////////////////
class OpNode
{
public:

  using MapInfo = TemplNode::MapInfo;

public:

  /// @brief コンストラクタ
  OpNode(
    SizeType input_num, ///< [in] 入力数
    SizeType op_id      ///< [in] 演算ノード番号
  );

  /// @brief デストラクタ
  virtual
  ~OpNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 演算ノード番号を返す．
  SizeType
  op_id() const
  {
    return mOpId;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 制約を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力値に関する制約を追加する．
  /// @return 出力値を表す変数を返す．
  virtual
  SatLiteral
  add_ovar_cond(
    SatSolver& solver,                        ///< [in] SATソルバ
    const std::vector<bool>& ival_list,       ///< [in] 入力値のリスト
    const std::vector<SatLiteral>& opvar_list ///< [in] 演算ノードの出力の変数のリスト
  ) const = 0;

  /// @brief no_reapplication 制約を追加する．
  virtual
  void
  add_no_reapplication_cond(
    SatSolver& solver, ///< [in] SATソルバ
    const OpNode* op2  ///< [in] 演算ノード
  ) const = 0;

  /// @brief オペランドの組み合わせを禁止する制約を追加する．
  virtual
  void
  add_ng_operand_cond(
    SatSolver& solver, ///< [in] SATソルバ
    SizeType opr0,     ///< [in] オペランド0
    SizeType opr1      ///< [in] オペランド1
  ) const = 0;

  /// @brief 直前の演算ノードに対する制約を追加する．
  virtual
  void
  add_op_order_cond(
    SatSolver& solver, ///< [in] SATソルバ
    const OpNode* op   ///< [in] 直前の演算ノード
  ) const = 0;

  /// @brief op_id 番目の演算を確定で使用している時 true を返す．
  virtual
  bool
  use(
    SizeType op_id ///< [in] 演算番号
  ) const;

  /// @breif step 番目の入力(演算)を使用している時 true となるリテラルを追加する．
  ///
  /// 入力の場合，入力番号が step になる．
  /// 演算の場合，演算番号 + 入力数が step になる．
  virtual
  void
  add_use_lit(
    SizeType step,                    ///< [in] ステップ番号
    std::vector<SatLiteral>& lit_list ///< [out] 格納先の変数
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // SAT モデルから値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SATモデルから演算タイプを得る．
  virtual
  MapInfo
  get_map_info(
    const SatModel& model ///< [in] SATモデル
  ) const = 0;

  /// @brief SATモデルから入力の選択結果を得る．
  /// @return オペランドの結果(ステップ番号の対)を返す．
  virtual
  std::vector<SizeType>
  get_opr(
    const SatModel& model ///< [in] SATモデル
  ) const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum;

  // 演算番号
  SizeType mOpId;

};

END_NAMESPACE_YM_EXSYN

#endif // OPNODE_H
