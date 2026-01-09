#ifndef YM_SATSOLVER_H
#define YM_SATSOLVER_H

/// @file ym/SatSolver.h
/// @brief SatSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sat.h"
#include "ym/SatBool3.h"
#include "ym/SatLiteral.h"
#include "ym/SatOneHotVar.h"
#include "ym/SatOrderedSet.h"
#include "ym/SatOneHotMatrix.h"
#include "ym/SatModel.h"
#include "ym/SatCondBlock.h"
#include "ym/SatInitParam.h"
#include "ym/SatStats.h"
#include "ym/CnfSize.h"
#include "ym/Expr.h"
#include "ym/AigLiteral.h"


BEGIN_NAMESPACE_YM_SAT

class SatSolverImpl;
class SatLogger;

//////////////////////////////////////////////////////////////////////
/// @class SatSolver SatSolver.h "ym/SatSolver.h"
/// @ingroup SatGroup
/// @brief CNF-SAT エンジンのインターフェイスを表すクラス
///
/// 実際の処理は SatSolverImpl (の継承クラス)が行う．
//////////////////////////////////////////////////////////////////////
class SatSolver
{
  friend class SatCondBlock;

public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  ///
  /// @sa SatInitParam
  SatSolver(
    const SatInitParam& init_param = SatInitParam{} ///< [in] 初期化パラメータ
  );

  /// @brief デストラクタ
  ~SatSolver();

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 変数と節の生成を行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を追加する．
  /// @return 新しい変数を表すリテラルを返す．
  ///
  /// * 変数番号は 0 から始まる．
  /// * lingeling においては decision = true のリテラルは freeze される．
  SatLiteral
  new_variable(
    bool decision = false ///< [in] 決定変数の時に true とする．
  );

  /// @brief 節を追加する．
  void
  add_clause(
    const std::vector<SatLiteral>& lits ///< [in] リテラルのリスト
  )
  {
    _add_clause(lits);
  }

  /// @brief 1項の節(リテラル)を追加する．
  void
  add_clause(
    SatLiteral lit1
  )
  {
    _add_clause(1, &lit1);
  }

  /// @brief 2項の節を追加する．
  void
  add_clause(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    SatLiteral tmp_lits[] = {lit1, lit2};

    _add_clause(2, tmp_lits);
  }

  /// @brief 3項の節を追加する．
  void
  add_clause(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3
  )
  {
    SatLiteral tmp_lits[] = {lit1, lit2, lit3};

    _add_clause(3, tmp_lits);
  }

  /// @brief 4項の節を追加する．
  void
  add_clause(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4
  )
  {
    SatLiteral tmp_lits[] = {lit1, lit2, lit3, lit4};

    _add_clause(4, tmp_lits);
  }

  /// @brief 5項の節を追加する．
  void
  add_clause(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5
  )
  {
    SatLiteral tmp_lits[] = {lit1, lit2, lit3, lit4, lit5};

    _add_clause(5, tmp_lits);
  }

  /// @brief 6項の節を追加する．
  void
  add_clause(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5,
    SatLiteral lit6
  )
  {
    SatLiteral tmp_lits[] = {lit1, lit2, lit3, lit4, lit5, lit6};

    _add_clause(6, tmp_lits);
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 条件変数用ブロックの生成
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件変数ブロックを返す．
  /// @return 生成されたブロックを返す．
  ///
  /// - このブロックの生存期間内では条件変数が有効となる．
  /// - 条件変数ブロックは入れ子にできる．
  SatCondBlock
  condblock(
    SatLiteral lit1 ///< [in] 条件変数リテラル1
  )
  {
    push_conditional_literal(lit1);
    return SatCondBlock(*this, 1);
  }

  /// @brief 条件変数ブロックを返す．
  /// @return 生成されたブロックを返す．
  ///
  /// - このブロックの生存期間内では条件変数が有効となる．
  /// - 条件変数ブロックは入れ子にできる．
  SatCondBlock
  condblock(
    SatLiteral lit1, ///< [in] 条件変数リテラル1
    SatLiteral lit2  ///< [in] 条件変数リテラル1
  )
  {
    push_conditional_literal(lit1);
    push_conditional_literal(lit2);
    return SatCondBlock(*this, 2);
  }

  /// @brief 条件変数ブロックを返す．
  /// @return 生成されたブロックを返す．
  ///
  /// - このブロックの生存期間内では条件変数が有効となる．
  /// - 条件変数ブロックは入れ子にできる．
  SatCondBlock
  condblock(
    SatLiteral lit1, ///< [in] 条件変数リテラル1
    SatLiteral lit2, ///< [in] 条件変数リテラル1
    SatLiteral lit3  ///< [in] 条件変数リテラル1
  )
  {
    push_conditional_literal(lit1);
    push_conditional_literal(lit2);
    push_conditional_literal(lit3);
    return SatCondBlock(*this, 3);
  }

  /// @brief 条件変数ブロックを返す．
  /// @return 生成されたブロックを返す．
  ///
  /// - このブロックの生存期間内では条件変数が有効となる．
  /// - 条件変数ブロックは入れ子にできる．
  SatCondBlock
  condblock(
    SatLiteral lit1, ///< [in] 条件変数リテラル1
    SatLiteral lit2, ///< [in] 条件変数リテラル1
    SatLiteral lit3, ///< [in] 条件変数リテラル1
    SatLiteral lit4  ///< [in] 条件変数リテラル1
  )
  {
    push_conditional_literal(lit1);
    push_conditional_literal(lit2);
    push_conditional_literal(lit3);
    push_conditional_literal(lit4);
    return SatCondBlock(*this, 4);
  }

  /// @brief 条件変数ブロックを返す．
  /// @return 生成されたブロックを返す．
  ///
  /// - このブロックの生存期間内では条件変数が有効となる．
  /// - 条件変数ブロックは入れ子にできる．
  SatCondBlock
  condblock(
    SatLiteral lit1, ///< [in] 条件変数リテラル1
    SatLiteral lit2, ///< [in] 条件変数リテラル1
    SatLiteral lit3, ///< [in] 条件変数リテラル1
    SatLiteral lit4, ///< [in] 条件変数リテラル1
    SatLiteral lit5  ///< [in] 条件変数リテラル1
  )
  {
    push_conditional_literal(lit1);
    push_conditional_literal(lit2);
    push_conditional_literal(lit3);
    push_conditional_literal(lit4);
    push_conditional_literal(lit5);
    return SatCondBlock(*this, 5);
  }

  /// @brief 条件変数ブロックを返す．
  /// @return 生成されたブロックを返す．
  ///
  /// - このブロックの生存期間内では条件変数が有効となる．
  /// - 条件変数ブロックは入れ子にできる．
  SatCondBlock
  condblock(
    const std::vector<SatLiteral> lit_list ///< [in] 条件変数リテラルのリスト
  )
  {
    push_conditional_literals(lit_list);
    return SatCondBlock(*this, lit_list.size());
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name Tseitin's encoding を行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのリテラルが等しいという条件を追加する．
  ///
  /// 具体的には (~lit1 + lit2)(lit1 + ~lit2) の２つの節を追加する．
  void
  add_buffgate(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    add_clause(~lit1,  lit2);
    add_clause( lit1, ~lit2);
  }

  /// @brief 2つのリテラルが等しくないという条件を追加する．
  ///
  /// 具体的には (~lit1 + ~lit2)(lit1 + lit2) の２つの節を追加する．
  void
  add_notgate(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    add_buffgate(~lit1, lit2);
  }

  /// @brief 2入力ANDゲートの入出力の関係を表す条件を追加する．
  void
  add_andgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2  ///< [in] 入力のリテラル2
  )
  {
    add_clause(~lit1, ~lit2,  olit);
    add_clause( lit1,        ~olit);
    add_clause(        lit2, ~olit);
  }

  /// @brief 3入力ANDゲートの入出力の関係を表す条件を追加する．
  void
  add_andgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3  ///< [in] 入力のリテラル3
  )
  {
    add_clause(~lit1, ~lit2, ~lit3,  olit);
    add_clause( lit1,               ~olit);
    add_clause(        lit2,        ~olit);
    add_clause(               lit3, ~olit);
  }

  /// @brief 4入力ANDゲートの入出力の関係を表す条件を追加する．
  void
  add_andgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3, ///< [in] 入力のリテラル3
    SatLiteral lit4  ///< [in] 入力のリテラル4
  )
  {
    add_clause(~lit1, ~lit2, ~lit3, ~lit4,  olit);
    add_clause( lit1,                      ~olit);
    add_clause(        lit2,               ~olit);
    add_clause(               lit3,        ~olit);
    add_clause(                      lit4, ~olit);
  }

  /// @brief n入力ANDゲートの入出力の関係を表す条件を追加する．
  void
  add_andgate(
    SatLiteral olit,                        ///< [in] 出力のリテラル
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief 2入力NANDゲートの入出力の関係を表す条件を追加する．
  void
  add_nandgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2  ///< [in] 入力のリテラル2
  )
  {
    add_andgate(~olit, lit1, lit2);
  }

  /// @brief 3入力NANDゲートの入出力の関係を表す条件を追加する．
  void
  add_nandgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3  ///< [in] 入力のリテラル3
  )
  {
    add_andgate(~olit, lit1, lit2, lit3);
  }

  /// @brief 4入力NANDゲートの入出力の関係を表す条件を追加する．
  void
  add_nandgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3, ///< [in] 入力のリテラル3
    SatLiteral lit4  ///< [in] 入力のリテラル4
  )
  {
    add_andgate(~olit, lit1, lit2, lit3, lit4);
  }

  /// @brief n入力NANDゲートの入出力の関係を表す条件を追加する．
  void
  add_nandgate(
    SatLiteral olit,                         ///< [in] 出力のリテラル
    const std::vector<SatLiteral>& lit_list  ///< [in] 入力のリテラルのリスト
  )
  {
    add_andgate(~olit, lit_list);
  }

  /// @brief 2入力ORゲートの入出力の関係を表す条件を追加する．
  void
  add_orgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2  ///< [in] 入力のリテラル2
  )
  {
    add_clause( lit1,  lit2, ~olit);
    add_clause(~lit1,         olit);
    add_clause(       ~lit2,  olit);
  }

  /// @brief 3入力ORゲートの入出力の関係を表す条件を追加する．
  void
  add_orgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3  ///< [in] 入力のリテラル3
  )
  {
    add_clause( lit1,  lit2,  lit3, ~olit);
    add_clause(~lit1,                olit);
    add_clause(       ~lit2,         olit);
    add_clause(              ~lit3,  olit);
  }

  /// @brief 4入力ORゲートの入出力の関係を表す条件を追加する．
  void
  add_orgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3, ///< [in] 入力のリテラル3
    SatLiteral lit4  ///< [in] 入力のリテラル4
  )
  {
    add_clause( lit1,  lit2,  lit3,  lit4, ~olit);
    add_clause(~lit1,                       olit);
    add_clause(       ~lit2,                olit);
    add_clause(              ~lit3,         olit);
    add_clause(                     ~lit4,  olit);
  }

  /// @brief n入力ORゲートの入出力の関係を表す条件を追加する．
  void
  add_orgate(
    SatLiteral olit,                        ///< [in] 出力のリテラル
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief 2入力NORゲートの入出力の関係を表す条件を追加する．
  void
  add_norgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2  ///< [in] 入力のリテラル2
  )
  {
    add_orgate(~olit, lit1, lit2);
  }

  /// @brief 3入力NORゲートの入出力の関係を表す条件を追加する．
  void
  add_norgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3  ///< [in] 入力のリテラル3
  )
  {
    add_orgate(~olit, lit1, lit2, lit3);
  }

  /// @brief 4入力NORゲートの入出力の関係を表す条件を追加する．
  void
  add_norgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3, ///< [in] 入力のリテラル3
    SatLiteral lit4  ///< [in] 入力のリテラル4
  )
  {
    add_orgate(~olit, lit1, lit2, lit3, lit4);
  }

  /// @brief n入力NORゲートの入出力の関係を表す条件を追加する．
  void
  add_norgate(
    SatLiteral olit,                        ///< [in] 出力のリテラル
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  )
  {
    add_orgate(~olit, lit_list);
  }

  /// @brief 2入力XORゲートの入出力の関係を表す条件を追加する．
  void
  add_xorgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2  ///< [in] 入力のリテラル2
  )
  {
    add_clause( lit1,  lit2, ~olit);
    add_clause( lit1, ~lit2,  olit);
    add_clause(~lit1,  lit2,  olit);
    add_clause(~lit1, ~lit2, ~olit);
  }

  /// @brief 3入力XORゲートの入出力の関係を表す条件を追加する．
  void
  add_xorgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3  ///< [in] 入力のリテラル3
  )
  {
    add_clause( lit1,  lit2,  lit3, ~olit);
    add_clause( lit1,  lit2, ~lit3,  olit);
    add_clause( lit1, ~lit2,  lit3,  olit);
    add_clause( lit1, ~lit2, ~lit3, ~olit);
    add_clause(~lit1,  lit2,  lit3,  olit);
    add_clause(~lit1,  lit2, ~lit3, ~olit);
    add_clause(~lit1, ~lit2,  lit3, ~olit);
    add_clause(~lit1, ~lit2, ~lit3,  olit);
  }

  /// @brief 4入力XORゲートの入出力の関係を表す条件を追加する．
  void
  add_xorgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3, ///< [in] 入力のリテラル3
    SatLiteral lit4  ///< [in] 入力のリテラル4
  )
  {
    _add_xorgate_sub(olit, std::vector<SatLiteral>{lit1, lit2, lit3, lit4}, 0, 4);
  }

  /// @brief n入力XORゲートの入出力の関係を表す条件を追加する．
  void
  add_xorgate(
    SatLiteral olit,                        ///< [in] 出力のリテラル
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  )
  {
    SizeType n = lit_list.size();
    _add_xorgate_sub(olit, lit_list, 0, n);
  }

  /// @brief 2入力XNORゲートの入出力の関係を表す条件を追加する．
  void
  add_xnorgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2  ///< [in] 入力のリテラル2
  )
  {
    add_xorgate(~olit, lit1, lit2);
  }

  /// @brief 3入力XNORゲートの入出力の関係を表す条件を追加する．
  void
  add_xnorgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3  ///< [in] 入力のリテラル3
  )
  {
    add_xorgate(~olit, lit1, lit2, lit3);
  }

  /// @brief 4入力XORゲートの入出力の関係を表す条件を追加する．
  void
  add_xnorgate(
    SatLiteral olit, ///< [in] 出力のリテラル
    SatLiteral lit1, ///< [in] 入力のリテラル1
    SatLiteral lit2, ///< [in] 入力のリテラル2
    SatLiteral lit3, ///< [in] 入力のリテラル3
    SatLiteral lit4  ///< [in] 入力のリテラル4
  )
  {
    add_xorgate(~olit, lit1, lit2, lit3, lit4);
  }

  /// @brief n入力XNORゲートの入出力の関係を表す条件を追加する．
  void
  add_xnorgate(
    SatLiteral olit,                        ///< [in] 出力のリテラル
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  )
  {
    add_xorgate(~olit, lit_list);
  }

  /// @brief 与えられた論理式を充足する条件を追加する．
  /// @return 条件を表すリテラルのリストを返す．
  ///
  /// 変数番号が lit_map に登録されていない時は例外が創出される．
  std::vector<SatLiteral>
  add_expr(
    const Expr& expr,            ///< [in] 対象の論理式
    const SatLiteralMap& lit_map ///< [in] 論理式中の変数番号とリテラルの対応関係を表す辞書
  );

  /// @brief 与えられたAIGを充足する条件を追加する．
  /// @return 各出力に対応した条件を表すリテラルのリストを返す．
  ///
  /// 変数番号が lit_map に登録されていない時は例外が創出される．
  std::vector<std::vector<SatLiteral>>
  add_aig(
    const AigMgr& aig_mgr,        ///< [in] 対象のAIG
    const SatLiteralMap& lit_map  ///< [in] AIGの入力番号とリテラルの対応関係を表す辞書
  );

  /// @brief half_adder の入出力の関係を表す条件を追加する．
  void
  add_half_adder(
    SatLiteral alit, ///< [in] 入力Aのリテラル
    SatLiteral blit, ///< [in] 入力Bのリテラル
    SatLiteral slit, ///< [in] 和の出力のリテラル
    SatLiteral olit  ///< [in] キャリー出力のリテラル
  );

  /// @brief full_adder の入出力の関係を表す条件を追加する．
  void
  add_full_adder(
    SatLiteral alit, ///< [in] 入力Aのリテラル
    SatLiteral blit, ///< [in] 入力Bのリテラル
    SatLiteral ilit, ///< [in] キャリー入力のリテラル
    SatLiteral slit, ///< [in] 和の出力のリテラル
    SatLiteral olit  ///< [in] キャリー出力のリテラル
  );

  /// @brief 多ビットadderの入出力の関係を表す条件を追加する．
  ///
  /// * alits, blits のサイズ <= slits のサイズでなければならない．
  /// * 片方が短い場合には上位ビットに0を仮定する．
  void
  add_adder(
    const std::vector<SatLiteral>& alits, ///< [in] 入力Aのリテラルのリスト
    const std::vector<SatLiteral>& blits, ///< [in] 入力Bのリテラルのリスト
    SatLiteral ilit,                      ///< [in] キャリー入力のリテラル
    const std::vector<SatLiteral>& slits, ///< [in] 出力のリテラルのリスト
    SatLiteral olit                       ///< [in] キャリー出力のリテラル
  );

  /// @brief 1's counter の入出力の関係を表す条件を追加する．
  /// @return 個数を表す2進数を表すリテラルのリストを返す．
  std::vector<SatLiteral>
  add_counter(
    const std::vector<SatLiteral>& ilits, ///< [in] 入力のリテラルのリスト
    bool decision = false                 ///< [in] 生成する変数を decision variable にする時 true にする．
  );

  /// @brief 与えられたリテラルのうち1つしか true にならない条件を追加する．
  void
  add_at_most_one(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    add_clause(~lit1, ~lit2);
  }

  /// @brief 与えられたリテラルのうち1つしか true にならない条件を追加する．
  void
  add_at_most_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3
  )
  {
    add_clause(~lit1, ~lit2);
    add_clause(~lit1, ~lit3);
    add_clause(~lit2, ~lit3);
  }

  /// @brief 与えられたリテラルのうち1つしか true にならない条件を追加する．
  void
  add_at_most_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4
  )
  {
    add_clause(~lit1, ~lit2);
    add_clause(~lit1, ~lit3);
    add_clause(~lit1, ~lit4);
    add_clause(~lit2, ~lit3);
    add_clause(~lit2, ~lit4);
    add_clause(~lit3, ~lit4);
  }

  /// @brief 与えられたリテラルのうち1つしか true にならない条件を追加する．
  void
  add_at_most_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5
  )
  {
    add_clause(~lit1, ~lit2);
    add_clause(~lit1, ~lit3);
    add_clause(~lit1, ~lit4);
    add_clause(~lit1, ~lit5);
    add_clause(~lit2, ~lit3);
    add_clause(~lit2, ~lit4);
    add_clause(~lit2, ~lit5);
    add_clause(~lit3, ~lit4);
    add_clause(~lit3, ~lit5);
    add_clause(~lit4, ~lit5);
  }

  /// @brief 与えられたリテラルのうち1つしか true にならない条件を追加する．
  void
  add_at_most_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5,
    SatLiteral lit6
  )
  {
    add_clause(~lit1, ~lit2);
    add_clause(~lit1, ~lit3);
    add_clause(~lit1, ~lit4);
    add_clause(~lit1, ~lit5);
    add_clause(~lit1, ~lit6);
    add_clause(~lit2, ~lit3);
    add_clause(~lit2, ~lit4);
    add_clause(~lit2, ~lit5);
    add_clause(~lit2, ~lit6);
    add_clause(~lit3, ~lit4);
    add_clause(~lit3, ~lit5);
    add_clause(~lit3, ~lit6);
    add_clause(~lit4, ~lit5);
    add_clause(~lit4, ~lit6);
    add_clause(~lit5, ~lit6);
  }

  /// @brief 与えられたリテラルのうち1つしか true にならない条件を追加する．
  void
  add_at_most_one(
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief 与えられたリテラルのうち2つしか true にならない条件を追加する．
  void
  add_at_most_two(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    // 無条件でなりたつ．
  }

  /// @brief 与えられたリテラルのうち2つしか true にならない条件を追加する．
  void
  add_at_most_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3
  )
  {
    add_clause(~lit1, ~lit2, ~lit3);
  }

  /// @brief 与えられたリテラルのうち2つしか true にならない条件を追加する．
  void
  add_at_most_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4
  )
  {
    add_clause(~lit1, ~lit2, ~lit3       );
    add_clause(~lit1, ~lit2       , ~lit4);
    add_clause(~lit1,        ~lit3, ~lit4);
    add_clause(       ~lit2, ~lit3, ~lit4);
  }

  /// @brief 与えられたリテラルのうち2つしか true にならない条件を追加する．
  void
  add_at_most_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5
  )
  {
    add_clause(~lit1, ~lit2, ~lit3              );
    add_clause(~lit1, ~lit2,        ~lit4       );
    add_clause(~lit1, ~lit2,               ~lit5);
    add_clause(~lit1,        ~lit3, ~lit4       );
    add_clause(~lit1,        ~lit3,        ~lit5);
    add_clause(~lit1,               ~lit4, ~lit5);
    add_clause(       ~lit2, ~lit3, ~lit4       );
    add_clause(       ~lit2, ~lit3,        ~lit5);
    add_clause(       ~lit2,        ~lit4, ~lit5);
    add_clause(              ~lit3, ~lit4, ~lit5);
  }

  /// @brief 与えられたリテラルのうち2つしか true にならない条件を追加する．
  void
  add_at_most_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5,
    SatLiteral lit6
  )
  {
    add_clause(~lit1, ~lit2, ~lit3                     );
    add_clause(~lit1, ~lit2,        ~lit4              );
    add_clause(~lit1, ~lit2,               ~lit5       );
    add_clause(~lit1, ~lit2,                      ~lit6);
    add_clause(~lit1,        ~lit3, ~lit4              );
    add_clause(~lit1,        ~lit3,        ~lit5       );
    add_clause(~lit1,        ~lit3,               ~lit6);
    add_clause(~lit1,               ~lit4, ~lit5       );
    add_clause(~lit1,               ~lit4,        ~lit6);
    add_clause(~lit1,                      ~lit5, ~lit6);
    add_clause(       ~lit2, ~lit3, ~lit4              );
    add_clause(       ~lit2, ~lit3,        ~lit5       );
    add_clause(       ~lit2, ~lit3,               ~lit6);
    add_clause(       ~lit2,        ~lit4, ~lit5       );
    add_clause(       ~lit2,        ~lit4,        ~lit6);
    add_clause(       ~lit2,               ~lit5, ~lit6);
    add_clause(              ~lit3, ~lit4, ~lit5       );
    add_clause(              ~lit3, ~lit4,        ~lit6);
    add_clause(              ~lit3,        ~lit5, ~lit6);
    add_clause(                     ~lit4, ~lit5, ~lit6);
  }

  /// @brief 与えられたリテラルのうち2つしか true にならない条件を追加する．
  void
  add_at_most_two(
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief 与えられたリテラルのうちk個しか true にならない条件を追加する．
  void
  add_at_most_k(
    const std::vector<SatLiteral>& lit_list, ///< [in] 入力のリテラルのリスト
    SizeType k                               ///< [in] しきい値
  );

  /// @brief 与えられたリテラルのうち1以上は true になる条件を追加する．
  void
  add_at_least_one(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    add_clause( lit1,  lit2);
  }

  /// @brief 与えられたリテラルのうち1つ以上は true になる条件を追加する．
  void
  add_at_least_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3
  )
  {
    add_clause( lit1,  lit2,  lit3);
  }

  /// @brief 与えられたリテラルのうち1つ以上は true になる条件を追加する．
  void
  add_at_least_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4
  )
  {
    add_clause( lit1,  lit2,  lit3,  lit4);
  }

  /// @brief 与えられたリテラルのうち1つ以上は true になる条件を追加する．
  void
  add_at_least_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5
  )
  {
    add_clause( lit1,  lit2,  lit3,  lit4,  lit5);
  }

  /// @brief 与えられたリテラルのうち1つ以上は true になる条件を追加する．
  void
  add_at_least_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5,
    SatLiteral lit6
  )
  {
    add_clause( lit1,  lit2,  lit3,  lit4,  lit5,  lit6);
  }

  /// @brief 与えられたリテラルのうち1つ以上は true になる条件を追加する．
  void
  add_at_least_one(
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  )
  {
    add_clause(lit_list);
  }

  /// @brief 与えられたリテラルのうち2以上は true になる条件を追加する．
  void
  add_at_least_two(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    add_clause(lit1);
    add_clause(lit2);
  }

  /// @brief 与えられたリテラルのうち2つ以上は true になる条件を追加する．
  void
  add_at_least_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3
  )
  {
    // 2つ以上が False にならない．
    add_clause( lit1,  lit2       );
    add_clause( lit1,         lit3);
    add_clause(        lit2,  lit3);
  }

  /// @brief 与えられたリテラルのうち2つ以上は true になる条件を追加する．
  void
  add_at_least_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4
  )
  {
    // 3つ以上が False にならない．
    add_clause( lit1,  lit2,  lit3       );
    add_clause( lit1,  lit2,         lit4);
    add_clause( lit1,         lit3,  lit4);
    add_clause(        lit2,  lit3,  lit4);
  }

  /// @brief 与えられたリテラルのうち2つ以上は true になる条件を追加する．
  void
  add_at_least_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5
  )
  {
    // 4つ以上が False にならない．
    add_clause( lit1,  lit2,  lit3,  lit4       );
    add_clause( lit1,  lit2,  lit3,         lit5);
    add_clause( lit1,  lit2,         lit4,  lit5);
    add_clause( lit1,         lit3,  lit4,  lit5);
    add_clause(        lit2,  lit3,  lit4,  lit5);
  }

  /// @brief 与えられたリテラルのうち2つ以上は true になる条件を追加する．
  void
  add_at_least_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5,
    SatLiteral lit6
  )
  {
    // 5つ以上が False にならない．
    add_clause( lit1,  lit2,  lit3,  lit4,  lit5       );
    add_clause( lit1,  lit2,  lit3,  lit4,         lit6);
    add_clause( lit1,  lit2,  lit3,         lit5,  lit6);
    add_clause( lit1,  lit2,         lit4,  lit5,  lit6);
    add_clause( lit1,         lit3,  lit4,  lit5,  lit6);
    add_clause(        lit2,  lit3,  lit4,  lit5,  lit6);
  }

  /// @brief 与えられたリテラルのうち2つ以上は true になる条件を追加する．
  void
  add_at_least_two(
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief 与えられたリテラルのうちk個以上は true になる条件を追加する．
  void
  add_at_least_k(
    const std::vector<SatLiteral>& lit_list, ///< [in] 入力のリテラルのリスト
    SizeType k                               ///< [in] しきい値
  );

  /// @brief 与えられたリテラルのうち厳密に1つが true になる条件を追加する．
  void
  add_exact_one(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    add_at_most_one(lit1, lit2);
    add_at_least_one(lit1, lit2);
  }

  /// @brief 与えられたリテラルのうち厳密に1つが true になる条件を追加する．
  void
  add_exact_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3
  )
  {
    add_at_most_one(lit1, lit2, lit3);
    add_at_least_one(lit1, lit2, lit3);
  }

  /// @brief 与えられたリテラルのうち厳密に1つが true になる条件を追加する．
  void
  add_exact_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4
  )
  {
    add_at_most_one(lit1, lit2, lit3, lit4);
    add_at_least_one(lit1, lit2, lit3, lit4);
  }

  /// @brief 与えられたリテラルのうち厳密に1つが true になる条件を追加する．
  void
  add_exact_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5
  )
  {
    add_at_most_one(lit1, lit2, lit3, lit4, lit5);
    add_at_least_one(lit1, lit2, lit3, lit4, lit5);
  }

  /// @brief 与えられたリテラルのうち厳密に1つが true になる条件を追加する．
  void
  add_exact_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5,
    SatLiteral lit6
  )
  {
    add_at_most_one(lit1, lit2, lit3, lit4, lit5, lit6);
    add_at_least_one(lit1, lit2, lit3, lit4, lit5, lit6);
  }

  /// @brief 与えられたリテラルのうち厳密に1つが true になる条件を追加する．
  void
  add_exact_one(
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief 与えられたリテラルのうち厳密に2つが true になる条件を追加する．
  void
  add_exact_two(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    add_at_most_two(lit1, lit2);
    add_at_least_two(lit1, lit2);
  }

  /// @brief 与えられたリテラルのうち厳密に2つが true になる条件を追加する．
  void
  add_exact_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3
  )
  {
    add_at_most_two(lit1, lit2, lit3);
    add_at_least_two(lit1, lit2, lit3);
  }

  /// @brief 与えられたリテラルのうち厳密に2つが true になる条件を追加する．
  void
  add_exact_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4
  )
  {
    add_at_most_two(lit1, lit2, lit3, lit4);
    add_at_least_two(lit1, lit2, lit3, lit4);
  }

  /// @brief 与えられたリテラルのうち厳密に2つが true になる条件を追加する．
  void
  add_exact_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5
  )
  {
    add_at_most_two(lit1, lit2, lit3, lit4, lit5);
    add_at_least_two(lit1, lit2, lit3, lit4, lit5);
  }

  /// @brief 与えられたリテラルのうち厳密に2つが true になる条件を追加する．
  void
  add_exact_two(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5,
    SatLiteral lit6
  )
  {
    add_at_most_two(lit1, lit2, lit3, lit4, lit5, lit6);
    add_at_least_two(lit1, lit2, lit3, lit4, lit5, lit6);
  }

  /// @brief 与えられたリテラルのうち厳密に2つが true になる条件を追加する．
  void
  add_exact_two(
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief 与えられたリテラルのうち厳密にk個が true になる条件を追加する．
  void
  add_exact_k(
    const std::vector<SatLiteral>& lit_list, ///< [in] 入力のリテラルのリスト
    SizeType k                               ///< [in] しきい値
  )
  {
    add_at_most_k(lit_list, k);
    add_at_least_k(lit_list, k);
  }

  /// @brief 与えられたリテラルのうちtrueになっている個数が1でない条件を追加する．
  void
  add_not_one(
    SatLiteral lit1,
    SatLiteral lit2
  )
  {
    add_clause( lit1, ~lit2);
    add_clause(~lit1,  lit2);
  }

  /// @brief 与えられたリテラルのうちtrueになっている個数が1でない条件を追加する．
  void
  add_not_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3
  )
  {
    add_clause( lit1,  lit2, ~lit3);
    add_clause( lit1, ~lit2,  lit3);
    add_clause(~lit1,  lit2,  lit3);
  }

  /// @brief 与えられたリテラルのうちtrueになっている個数が1でない条件を追加する．
  void
  add_not_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4
  )
  {
    add_clause( lit1,  lit2,  lit3, ~lit4);
    add_clause( lit1,  lit2, ~lit3,  lit4);
    add_clause( lit1, ~lit2,  lit3,  lit4);
    add_clause(~lit1,  lit2,  lit3,  lit4);
  }

  /// @brief 与えられたリテラルのうちtrueになっている個数が1でない条件を追加する．
  void
  add_not_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5
  )
  {
    add_clause( lit1,  lit2,  lit3,  lit4, ~lit5);
    add_clause( lit1,  lit2,  lit3, ~lit4,  lit5);
    add_clause( lit1,  lit2, ~lit3,  lit4,  lit5);
    add_clause( lit1, ~lit2,  lit3,  lit4,  lit5);
    add_clause(~lit1,  lit2,  lit3,  lit4,  lit5);
  }

  /// @brief 与えられたリテラルのうちtrueになっている個数が1でない条件を追加する．
  void
  add_not_one(
    SatLiteral lit1,
    SatLiteral lit2,
    SatLiteral lit3,
    SatLiteral lit4,
    SatLiteral lit5,
    SatLiteral lit6
  )
  {
    add_clause( lit1,  lit2,  lit3,  lit4,  lit5, ~lit6);
    add_clause( lit1,  lit2,  lit3,  lit4, ~lit5,  lit6);
    add_clause( lit1,  lit2,  lit3, ~lit4,  lit5,  lit6);
    add_clause( lit1,  lit2, ~lit3,  lit4,  lit5,  lit6);
    add_clause( lit1, ~lit2,  lit3,  lit4,  lit5,  lit6);
    add_clause(~lit1,  lit2,  lit3,  lit4,  lit5,  lit6);
  }

  /// @brief 与えられたリテラルのうちtrueになっている個数が1でない条件を追加する．
  void
  add_not_one(
    const std::vector<SatLiteral>& lit_list ///< [in] 入力のリテラルのリスト
  );

  /// @brief A == B という条件を追加する．
  ///
  /// a_vec と b_vec のビット長が異なるときは短い方の上位ビットを
  /// 0と仮定する．
  void
  add_eq(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    const std::vector<SatLiteral>& b_vec  ///< [in] B を表すビットベクタ
  );

  /// @brief A == B という条件を追加する．
  void
  add_eq(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    int b_val                             ///< [in] Bの値
  );

  /// @brief A != B という条件を追加する．
  ///
  /// a_vec と b_vec のビット長が異なるときは短い方の上位ビットを
  /// 0と仮定する．
  void
  add_ne(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    const std::vector<SatLiteral>& b_vec  ///< [in] B を表すビットベクタ
  );

  /// @brief A != B という条件を追加する．
  void
  add_ne(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    int b_val   			  ///< [in] Bの値
  );

  /// @brief A < B という条件を追加する．
  /// @param[in] a_vec, b_vec A,Bを表すビットベクタ
  ///
  /// a_vec と b_vec のビット長が異なるときは短い方の上位ビットを
  /// 0と仮定する．
  void
  add_lt(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    const std::vector<SatLiteral>& b_vec  ///< [in] B を表すビットベクタ
  );

  /// @brief A < B という条件を追加する．
  void
  add_lt(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    int b_val			          ///< [in] Bの値
  );

  /// @brief A <= B という条件を追加する．
  ///
  /// a_vec と b_vec のビット長が異なるときは短い方の上位ビットを
  /// 0と仮定する．
  void
  add_le(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    const std::vector<SatLiteral>& b_vec  ///< [in] B を表すビットベクタ
  );

  /// @brief A <= B という条件を追加する．
  void
  add_le(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    int b_val 			          ///< [in] Bの値
  );

  /// @brief A > B という条件を追加する．
  ///
  /// a_vec と b_vec のビット長が異なるときは短い方の上位ビットを
  /// 0と仮定する．
  void
  add_gt(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    const std::vector<SatLiteral>& b_vec  ///< [in] B を表すビットベクタ
  )
  {
    add_lt(b_vec, a_vec);
  }

  /// @brief A > B という条件を追加する．
  void
  add_gt(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    int b_val			          ///< [in] Bの値
  );

  /// @brief A >= B という条件を追加する．
  ///
  /// a_vec と b_vec のビット長が異なるときは短い方の上位ビットを
  /// 0と仮定する．
  void
  add_ge(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    const std::vector<SatLiteral>& b_vec  ///< [in] B を表すビットベクタ
  )
  {
    add_le(b_vec, a_vec);
  }

  /// @brief A >= B という条件を追加する．
  void
  add_ge(
    const std::vector<SatLiteral>& a_vec, ///< [in] A を表すビットベクタ
    int b_val			          ///< [in] Bの値
  );

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name SatOneHotVar に関する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief one-hot 符号化した変数を返す．
  SatOneHotVar
  new_onehot_var(
    SizeType size ///< [in] サイズ
  );

  /// @brief a == b という条件を追加する．
  void
  add_eq(
    const SatOneHotVar& a,
    const SatOneHotVar& b
  );

  /// @brief a != b という条件を追加する．
  void
  add_ne(
    const SatOneHotVar& a,
    const SatOneHotVar& b
  );


  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name SatOrderedSet に関する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 順序符号化した変数を返す．
  ///
  /// - min_val <= val <= max_val であるような val を表す．
  /// - min_val <= max_val である必要がある．
  /// - min_val == max_val は適切だが意味はない．
  SatOrderedSet
  new_ordered_set(
    int min_val, ///< [in] 最小値
    int max_val  ///< [in] 最大値
  );

  /// @brief a < b という条件を追加する．
  void
  add_lt(
    const SatOrderedSet& a,
    int b
  );

  /// @brief a <= b という条件を追加する．
  void
  add_le(
    const SatOrderedSet& a,
    int b
  );

  /// @brief a > b という条件を追加する．
  void
  add_gt(
    const SatOrderedSet& a,
    int b
  );

  /// @brief a >= b という条件を追加する．
  void
  add_ge(
    const SatOrderedSet& a,
    int b
  );

  /// @brief a == b という条件を追加する．
  void
  add_eq(
    const SatOrderedSet& a,
    const SatOrderedSet& b
  );

  /// @brief a != b という条件を追加する．
  void
  add_ne(
    const SatOrderedSet& a,
    const SatOrderedSet& b
  );

  /// @brief a < b という条件を追加する．
  void
  add_lt(
    const SatOrderedSet& a,
    const SatOrderedSet& b
  );

  /// @brief a <= b という条件を追加する．
  void
  add_le(
    const SatOrderedSet& a,
    const SatOrderedSet& b
  );

  /// @brief a > b という条件を追加する．
  void
  add_gt(
    const SatOrderedSet& a,
    const SatOrderedSet& b
  )
  {
    add_lt(b, a);
  }

  /// @brief a >= b という条件を追加する．
  void
  add_ge(
    const SatOrderedSet& a,
    const SatOrderedSet& b
  )
  {
    add_le(b, a);
  }

  /// @brief a の値が uval 以下か lval 以上という制約を作る．
  ///
  /// uval < lval を仮定する．
  /// そうでなければ常に成り立つ．
  void
  add_dropoff(
    const SatOrderedSet& a,
    int uval, ///< [in] 上限値
    int lval  ///< [in] 下限値
  );

  /// @brief a == b という条件を追加する．
  void
  add_eq(
    const std::vector<SatOrderedSet>& a,
    const std::vector<SatOrderedSet>& b
  );

  /// @brief a != b という条件を追加する．
  void
  add_ne(
    const std::vector<SatOrderedSet>& a,
    const std::vector<SatOrderedSet>& b
  );

  /// @brief a < b という条件を追加する．
  ///
  /// 辞書式順序で比較する．
  void
  add_lt(
    const std::vector<SatOrderedSet>& a,
    const std::vector<SatOrderedSet>& b
  );

  /// @brief a <= b という条件を追加する．
  ///
  /// 辞書式順序で比較する．
  void
  add_le(
    const std::vector<SatOrderedSet>& a,
    const std::vector<SatOrderedSet>& b
  );

  /// @brief a > b という条件を追加する．
  ///
  /// 辞書式順序で比較する．
  void
  add_gt(
    const std::vector<SatOrderedSet>& a,
    const std::vector<SatOrderedSet>& b
  )
  {
    add_lt(b, a);
  }

  /// @brief a >= b という条件を追加する．
  ///
  /// 辞書式順序で比較する．
  void
  add_ge(
    const std::vector<SatOrderedSet>& a,
    const std::vector<SatOrderedSet>& b
  )
  {
    add_le(b, a);
  }

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name SatOneHotMatrix に関する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief one-hot 符号化したマトリックスを返す．
  SatOneHotMatrix
  new_onehot_matrix(
    SizeType elem_size, ///< [in] 要素のサイズ
    SizeType sel_size   ///< [in] 選択する数
  );


  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 動作制御を行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief SAT 問題を解く．
  /// @return 結果(SatBool3)を返す．
  ///
  /// 結果の意味は以下の通り
  /// * SatBool3::True  充足した．
  /// * SatBool3::False 充足不能が判明した．
  /// * SatBool3::X     わからなかった．
  SatBool3
  solve(
    SizeType time_limit = 0 ///< [in] 時間制約(秒) 0 で制約なし
  )
  {
    // 空の assumptions を付けて solve() を呼ぶだけ
    return solve({}, time_limit);
  }

  /// @brief assumption 付きの SAT 問題を解く．
  /// @return 結果(SatBool3)を返す．
  ///
  /// 結果の意味は以下の通り
  /// * SatBool3::True  充足した．
  /// * SatBool3::False 充足不能が判明した．
  /// * SatBool3::X     わからなかった．
  SatBool3
  solve(
    const std::vector<SatLiteral>& assumptions, ///< [in] あらかじめ仮定する
                                                ///       変数の値割り当てリスト
    SizeType time_limit = 0                     ///< [in] 時間制約(秒) 0 で制約なし
  );

  /// @brief 直前に解いた問題のモデルを返す．
  ///
  /// 返される値は内部のデータへの参照なので solve()
  /// が呼ばれる度に内容が変わるので必要ならばコピー
  /// を作る必要がある．
  const SatModel&
  model() const
  {
    return mModel;
  }

  /// @brief 直前の問題の矛盾の原因のリテラルを返す．
  ///
  /// 返される値は内部のデータへの参照なので solve()
  /// が呼ばれる度に内容が変わるので必要ならばコピー
  /// を作る必要がある．
  const std::vector<SatLiteral>&
  conflict_literals()
  {
    return mConflictLiterals;
  }

  /// @brief 時間計測機能を制御する
  void
  timer_on(
    bool enable
  );

  /// @brief トータルの矛盾回数の制限値を設定する．
  /// @return 以前の設定値を返す．
  SizeType
  set_conflict_budget(
    SizeType val ///< [in] 設定する値
  );

  /// @brief トータルの implication 回数の制限値を設定する．
  /// @return 以前の設定値を返す．
  SizeType
  set_propagation_budget(
    SizeType val ///< [in] 設定する値
  );

  /// @brief solve() 中のリスタートのたびに呼び出されるメッセージハンドラの登録
  void
  reg_msg_handler(
    SatMsgHandler* msg_handler ///< [in] 登録するメッセージハンドラ
  );

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部状態の取得を行う関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 正しい状態のときに true を返す．
  bool
  sane() const;

  /// @brief 現在の内部状態を得る．
  SatStats
  get_stats() const;

  /// @brief 変数の数を得る．
  SizeType
  variable_num() const
  {
    return mVariableNum;
  }

  /// @brief 制約節の数を得る．
  SizeType
  clause_num() const
  {
    return mClauseList.size();
  }

  /// @brief 制約節のリテラルの総数を得る．
  SizeType
  literal_num() const
  {
    return mLiteralNum;
  }

  /// @brief 制約節のサイズを得る．
  CnfSize
  cnf_size() const
  {
    return CnfSize{clause_num(), literal_num()};
  }

  /// @brief DIMACS 形式で制約節を出力する．
  void
  write_DIMACS(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 条件リテラルに関する操作
  // SatCondBlock が用いる
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件リテラルを追加する．
  ///
  /// 以降の add_clause() にはこのリテラルの否定が追加される．
  void
  push_conditional_literal(
    SatLiteral lit ///< [in] 条件リテラル
  )
  {
    mConditionalLits.push_back(lit);
  }

  /// @brief 条件リテラルを追加する．
  ///
  /// 以降の add_clause() にはこのリテラルの否定が追加される．
  void
  push_conditional_literals(
    const std::vector<SatLiteral>& lit_list ///< [in] 条件リテラルのリスト
  )
  {
    mConditionalLits.insert(mConditionalLits.end(),
			    lit_list.begin(), lit_list.end());
  }

  /// @brief 条件リテラルを削除する．
  void
  pop_conditional_literals(
    SizeType num ///< [in] 取り出す要素数
  )
  {
    mConditionalLits.erase(mConditionalLits.end() - num,
			   mConditionalLits.end());
  }

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief add_lt() の下請け関数
  SatLiteral
  _add_lt(
    const std::vector<SatOrderedSet>& a,
    const std::vector<SatOrderedSet>& b,
    SizeType pos
  );

  /// @brief add_le() の下請け関数
  SatLiteral
  _add_le(
    const std::vector<SatOrderedSet>& a,
    const std::vector<SatOrderedSet>& b,
    SizeType pos
  );

  /// @brief add_clause() の下請け関数
  void
  _add_clause(
    const std::vector<SatLiteral>& lits ///< [in] リテラルのリスト
  );

  /// @brief add_clause() の下請け関数
  void
  _add_clause(
    SizeType n,       ///< [in] リテラル数
    SatLiteral lits[] ///< [in] リテラルの配列
  );

  /// @brief _add_clause() の下請け関数
  void
  _add_clause_sub(
    const std::vector<SatLiteral>& lits ///< [in] リテラルのリスト
  );

  /// @brief n入力XORゲートの入出力の関係を表す条件を追加する．
  void
  _add_xorgate_sub(
    SatLiteral olit,                         ///< [in] 出力のリテラル
    const std::vector<SatLiteral>& lit_list, ///< [in] 入力のリテラルのリスト
    SizeType start,                          ///< [in] 開始位置
    SizeType num                             ///< [in] 要素数
  );

  /// @brief add_at_most_one() の下請け関数
  void
  _add_at_most_one(
    const std::vector<SatLiteral>& lit_list,
    SatLiteral olit
  );

  /// @brief add_at_most_two() の下請け関数
  void
  _add_at_most_two(
    const std::vector<SatLiteral>& lit_list,
    SatLiteral olit1,
    SatLiteral olit0
  );

  /// @brief add_at_least_two() の下請け関数
  void
  _add_at_least_two(
    const std::vector<SatLiteral>& lit_list,
    SatLiteral olit1,
    SatLiteral olit0
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ソルバの型
  SatInitParam mType;

  // 実際の機能を実装しているクラス
  std::unique_ptr<SatSolverImpl> mImpl;

  // ロガー
  std::unique_ptr<SatLogger> mLogger;

  // 条件リテラルのスタック
  std::vector<SatLiteral> mConditionalLits;

  // 直前の問題のモデル
  SatModel mModel;

  // 直前の矛盾の原因
  std::vector<SatLiteral> mConflictLiterals;

  // 変数の数(リポート用)
  SizeType mVariableNum{0};

  // 節のリスト(リポート用)
  std::vector<std::vector<SatLiteral>> mClauseList;

  // リテラル数(リポート用)
  SizeType mLiteralNum{0};

};

END_NAMESPACE_YM_SAT

#endif // YM_SATSOLVER_H
