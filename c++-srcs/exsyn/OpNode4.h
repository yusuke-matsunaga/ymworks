#ifndef OPNODE4_H
#define OPNODE4_H

/// @file OpNode4.h
/// @brief OpNode4 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode.h"
#include "TemplNode4.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class OpNode4 OpNode4.h "OpNode4.h"
/// @brief 4項演算の OpNode
///
/// 意味的には OpNode と TemplNode2 を継承したクラスだが
/// 二重継承は使いたくないので TemplNode4 をメンバとして持っている．
/// そのため TemplNode4 の関数を呼び出すだけの関数を定義する必要がある．
//////////////////////////////////////////////////////////////////////
class OpNode4 :
public OpNode
{
public:

  /// @brief コンストラクタ
  OpNode4(
    SatSolver& solver,  ///< [in] SATソルバ
    SizeType input_num, ///< [in] 入力数
    SizeType op_id      ///< [in] 演算ノード番号
  );

  /// @brief デストラクタ
  virtual
  ~OpNode4() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 制約を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief no_reapplication 制約を追加する．
  void
  add_no_reapplication_cond(
    SatSolver& solver, ///< [in] SATソルバ
    const OpNode* op2  ///< [in] 演算ノードのリスト
  ) const override;

  /// @brief オペランドの組み合わせを禁止する制約を追加する．
  void
  add_ng_operand_cond(
    SatSolver& solver, ///< [in] SATソルバ
    SizeType opr0,     ///< [in] オペランド0
    SizeType opr1      ///< [in] オペランド1
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // SAT モデルから値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SATモデルから演算タイプを得る．
  MapInfo
  get_map_info(
    const SatModel& model ///< [in] SATモデル
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 3項演算の出力値に関する制約を追加する．
  void
  add_op_cond(
    SatSolver& solver, ///< [in] SATソルバ
    SatLiteral ivar0,  ///< [in] 入力0のリテラル
    SatLiteral ivar1,  ///< [in] 入力1のリテラル
    SatLiteral ivar2,  ///< [in] 入力2のリテラル
    SatLiteral ivar3,  ///< [in] 入力3のリテラル
    SatLiteral ovar    ///< [in] 出力のリテラル
  ) const
  {
    mTemplate.add_op_cond(solver, ivar0, ivar1, ivar2, ivar3, ovar);
  }

  /// @brief 3項演算の出力値に関する制約を追加する．
  ///
  /// 入力0が固定
  void
  add_op_cond(
    SatSolver& solver, ///< [in] SATソルバ
    bool ival0,        ///< [in] 入力0の値
    SatLiteral ivar1,  ///< [in] 入力1のリテラル
    SatLiteral ivar2,  ///< [in] 入力2のリテラル
    SatLiteral ivar3,  ///< [in] 入力3のリテラル
    SatLiteral ovar    ///< [in] 出力のリテラル
  ) const
  {
    mTemplate.add_op_cond(solver, ival0, ivar1, ivar2, ivar3, ovar);
  }

  /// @brief 3項演算の出力値に関する制約を追加する．
  ///
  /// 入力0と入力1が固定
  void
  add_op_cond(
    SatSolver& solver, ///< [in] SATソルバ
    bool ival0,        ///< [in] 入力0の値
    bool ival1,        ///< [in] 入力1の値
    SatLiteral ivar2,  ///< [in] 入力2のリテラル
    SatLiteral ivar3,  ///< [in] 入力3のリテラル
    SatLiteral ovar    ///< [in] 出力のリテラル
  ) const
  {
    mTemplate.add_op_cond(solver, ival0, ival1, ivar2, ivar3, ovar);
  }

  /// @brief 3項演算の出力値に関する制約を追加する．
  ///
  /// 入力0と入力1，入力2が固定
  void
  add_op_cond(
    SatSolver& solver, ///< [in] SATソルバ
    bool ival0,        ///< [in] 入力0の値
    bool ival1,        ///< [in] 入力1の値
    bool ival2,        ///< [in] 入力2の値
    SatLiteral ivar3,  ///< [in] 入力3のリテラル
    SatLiteral ovar    ///< [in] 出力のリテラル
  ) const
  {
    mTemplate.add_op_cond(solver, ival0, ival1, ival2, ivar3, ovar);
  }

  /// @brief 3項演算の出力値に関する制約を追加する．
  ///
  /// すべての入力が固定
  void
  add_op_cond(
    SatSolver& solver, ///< [in] SATソルバ
    bool ival0,        ///< [in] 入力0の値
    bool ival1,        ///< [in] 入力1の値
    bool ival2,        ///< [in] 入力2の値
    bool ival3,        ///< [in] 入力3の値
    SatLiteral ovar    ///< [in] 出力のリテラル
  ) const
  {
    mTemplate.add_op_cond(solver, ival0, ival1, ival2, ival3, ovar);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算タイプを表すノード
  TemplNode4 mTemplate;

};

END_NAMESPACE_YM_EXSYN

#endif // OPNODE4_H
