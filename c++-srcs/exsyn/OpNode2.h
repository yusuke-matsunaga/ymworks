#ifndef OPNODE2_H
#define OPNODE2_H

/// @file OpNode2.h
/// @brief OpNode2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "OpNode.h"
#include "TemplNode2.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class OpNode2 OpNode2.h "OpNode2.h"
/// @brief 二項演算の OpNode
///
/// 意味的には OpNode と TemplNode2 を継承したクラスだが
/// 二重継承は使いたくないので TemplNode2 をメンバとして持っている．
/// そのため TemplNode2 の関数を呼び出すだけの関数を定義する必要がある．
//////////////////////////////////////////////////////////////////////
class OpNode2 :
public OpNode
{
public:

  /// @brief コンストラクタ
  OpNode2(
    SatSolver& solver,  ///< [in] SATソルバ
    SizeType input_num, ///< [in] 入力数
    SizeType op_id      ///< [in] 演算ノード番号
  );

  /// @brief デストラクタ
  ~OpNode2() = default;


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
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2項演算の出力値に関する制約を追加する．
  void
  add_op_cond(
    SatSolver& solver, ///< [in] SATソルバ
    SatLiteral ivar0,  ///< [in] 入力0のリテラル
    SatLiteral ivar1,  ///< [in] 入力1のリテラル
    SatLiteral ovar    ///< [in] 出力のリテラル
  ) const
  {
    mTemplate.add_op_cond(solver, ivar0, ivar1, ovar);
  }

  /// @brief 2項演算の出力値に関する制約を追加する．
  ///
  /// 入力0の値が固定している場合
  void
  add_op_cond(
    SatSolver& solver, ///< [in] SATソルバ
    bool ival0,        ///< [in] 入力0の値
    SatLiteral ivar1,  ///< [in] 入力1のリテラル
    SatLiteral ovar    ///< [in] 出力のリテラル
  ) const
  {
    mTemplate.add_op_cond(solver, ival0, ivar1, ovar);
  }

  /// @brief 2項演算の出力値に関する制約を追加する．
  ///
  /// 入力0，入力1の値が固定している場合
  void
  add_op_cond(
    SatSolver& solver, ///< [in] SATソルバ
    bool ival0,        ///< [in] 入力0の値
    bool ival1,        ///< [in] 入力1の値
    SatLiteral ovar    ///< [in] 出力のリテラル
  ) const
  {
    mTemplate.add_op_cond(solver, ival0, ival1, ovar);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算タイプを表すノード
  TemplNode2 mTemplate;

};

END_NAMESPACE_YM_EXSYN

#endif // OPNODE2_H
