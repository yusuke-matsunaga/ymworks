#ifndef TEMPLNODE4_H
#define TEMPLNODE4_H

/// @file TemplNode4.h
/// @brief TemplNode4 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TemplNode.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class TemplNode4 TemplNode4.h "TemplNode4.h"
/// @brief 4項演算の TemplNode
///
/// 4項演算の演算タイプの情報を持つ．
//////////////////////////////////////////////////////////////////////
class TemplNode4 :
public TemplNode
{
public:

  /// @brief コンストラクタ
  TemplNode4(
    SatSolver& solver ///< [in] SATソルバ
  );

  /// @brief デストラクタ
  virtual
  ~TemplNode4() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // SAT モデルから値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief SATモデルから演算タイプを得る．
  MapInfo
  get_map_info(
    const SatModel& model ///< [in] SATモデル
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 入出力の関係を表す制約を生成する関数
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
  ) const;

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
  ) const;

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
  ) const;

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
  ) const;

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
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力値を表す変数のリスト
  std::vector<SatLiteral> mTval;

};

END_NAMESPACE_YM_EXSYN

#endif // TEMPLNODE4_H
