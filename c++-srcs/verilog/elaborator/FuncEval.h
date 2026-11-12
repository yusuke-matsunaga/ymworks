#ifndef FUNCEVAL_H
#define FUNCEVAL_H

/// @file FuncEval.h
/// @brief FuncEval のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/Ast.h"
#include "ym/vl/VlFwd.h"
#include "ym/vl/VlValue.h"
#include "common/LogMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class FuncEval FuncEval.h "FuncEval.h"
/// @brief 関数の値の評価を行うクラス
///
/// constant expression を elaboration 中に評価するために用いる．
/// constant expression は constant function call を含むので中には
/// ステートメントの実行を伴う．
/// しかし再帰呼び出しはないので一つの変数に対するオブジェクトは
/// 唯一に定まる．
/// そこで，変数に対する値割り当てを unordered_map で保持する．
/// キーは decl_id()
//////////////////////////////////////////////////////////////////////
class FuncEval
{
public:

  /// @brief コンストラクタ
  FuncEval(
    LogMgr& log_mgr,           ///< [in] ログマネージャ
    const VlTaskFunc* function ///< [in] 関数
  );

  /// @brief デストラクタ
  ~FuncEval();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数を評価する．
  VlValue
  operator()(
    const std::vector<VlValue>& arg_list ///< [in] 引数のリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // evaluate_stmt() で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ステートメントの実行を行う．
  /// @return disable が実行された場合はスコープを返す．
  const VlScope*
  evaluate_stmt(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief begin-end ブロックの実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_seqblock(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief 名前付き begin-end ブロックの実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_namedseqblock(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief 代入文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_assign(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief while 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_while(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief repeat 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_repeat(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief for 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_for(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief forever 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_forever(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief if 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_if(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief if-else 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_ifelse(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief case 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_case(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief disable 文の実行を行う．
  /// @return break 対象のスコープを返す．
  const VlScope*
  evaluate_disable(
    const VlStmt* stmt ///< [in] 対象のステートメント
  );

  /// @brief caseitem のラベルと一致するか調べる．
  bool
  match(
    VpiCaseType case_type,     ///< [in] case 文の種類
    const VlValue& val,        ///< [in] case 文の値
    const VlCaseItem* caseitem ///< [in] 対象の caseitem
  );

  /// @brief プライマリに値を代入する．
  void
  assign_primary(
    const VlExpr* expr, ///< [in] プライマリ式
    const VlValue& val  ///< [in] 値
  );


private:
  //////////////////////////////////////////////////////////////////////
  // evaluate_expr() の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の評価を行う．
  VlValue
  evaluate_expr(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 演算子に対して式の値を評価する．
  VlValue
  evaluate_opr(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 定数に対して式の値を評価する．
  VlValue
  evaluate_const(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 関数呼び出しに対して式の値を評価する．
  VlValue
  evaluate_funccall(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief プライマリに対して式の値を評価する．
  VlValue
  evaluate_primary(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 単独の宣言要素の値を評価する．
  VlValue
  evaluate_decl(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 固定インデックスの配列型要素の値を評価する．
  VlValue
  evaluate_const_decl_elem(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 可変インデックスの配列型要素の値を評価する．
  VlValue
  evaluate_var_decl_elem(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 式を評価して整数値を返す．
  int
  evaluate_int(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief 式を評価して整数値を返す．
  bool
  evaluate_bool(
    const VlExpr* expr ///< [in] 対象の式
  );

  /// @brief インデックス式を評価する．
  std::vector<int>
  evaluate_index(
    const std::vector<const VlExpr*>& index_list
  );

  /// @brief プライマリ式から宣言要素の番号を取り出す．
  SizeType
  get_decl_id(
    const VlExpr* expr
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 値を管理する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を登録する．
  void
  reg_val(
    SizeType key,      ///< [in] キー
    const VlValue& val ///< [in] 値
  );

#if 0
  /// @brief 値を登録する．
  ///
  /// 固定インデックスのビット/部分選択つき
  void
  reg_val_const_select(
    SizeType key,       ///< [in] キー
    const VlValue& val, ///< [in] 値
    const VlExpr* expr  ///< [in] 対象の式
  );

  /// @brief 値を登録する．
  ///
  /// 可変プラスインデックスのビット/部分選択つき
  void
  reg_val_plus_select(
    SizeType key,       ///< [in] キー
    const VlValue& val, ///< [in] 値
    const VlExpr* expr  ///< [in] 対象の式
  );

  /// @brief 値を登録する．
  ///
  /// 可変プラスインデックスのビット/部分選択つき
  void
  reg_val_minus_select(
    SizeType key,       ///< [in] キー
    const VlValue& val, ///< [in] 値
    const VlExpr* expr  ///< [in] 対象の式
  );

  /// @brief reg_val() の下請け関数
  void
  _reg_val(
    SizeType key,
    const VlValue& val,
    SizeType msb,
    SizeType lsb
  );
#endif

  /// @brief 値を取り出す．
  VlValue
  get_val(
    const VlDecl* obj ///< [in] 宣言要素
  ) const;

  /// @brief 値を取り出す．
  VlValue
  _get_val(
    SizeType key ///< [in] キー
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ログマネージャ
  LogMgr& mLogMgr;

  // 対象の関数
  const VlTaskFunc* mFunction;

  // 値割り当ての辞書
  std::unordered_map<SizeType, VlValue> mValMap;

};

END_NAMESPACE_YM_VERILOG

#endif // FUNCEVAL_H
