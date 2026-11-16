#ifndef EXPRGEN_H
#define EXPRGEN_H

/// @file ExprGen.h
/// @brief ExprGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlValue.h"
#include "ym/vl/Ast.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

class ElbEnv;
class ElbPrimary;
class ElbDeclRef;
class ElbSelecter;

//////////////////////////////////////////////////////////////////////
/// @class ExprGen ExprGen.h "ExprGen.h"
/// @brief ElbExpr を生成するクラス
//////////////////////////////////////////////////////////////////////
class ExprGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  ExprGen(
    Elaborator& elab ///< [in] 生成器
  );

  /// @brief デストラクタ
  ~ExprGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ExprGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AstExpr から通常の ElbiExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_expr(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr から定数式の ElbExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_constant_expr(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr からイベント式の ElbiExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_event_expr(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr からシステム関数の引数を生成する．
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_arg(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,	    ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr から左辺式を生成する
  /// @return 生成された ElbPrimary (のリスト)を返す．
  ///
  /// - 不適切な式ならば ElbError 例外を送出する．
  const VlExpr*
  instantiate_lhs(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,	    ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr から継続的代入文の左辺式を生成する
  /// @return 生成された ElbPrimary (のリスト)を返す．
  ///
  /// - 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  /// - instantiate_lhs() との違いは可変インデックスを許さないこと．
  const VlExpr*
  instantiate_net_lhs(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr(primary) から named_event を生成する．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_namedevent(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstDelay から ElbExpr を生成する．
  ///
  /// 不適切な式ならば内部でエラーメッセージを出力して nullptr を返す．
  /// 例外は送出しない
  const VlDelay*
  instantiate_delay(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstDelay& ast_delay ///< [in] 遅延を表すパース木
  );

  /// @brief AstOrderedCon から ElbExpr を生成する．
  ///
  /// これは AstInst の前にある # つきの式がパラメータ割り当てなのか
  /// 遅延なのかわからないので AstOrderedCon で表していることによる．
  ///
  /// 不適切な式ならば内部でエラーメッセージを出力して nullptr を返す．
  /// 例外は送出しない
  const VlDelay*
  instantiate_delay(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstItem& ast_head ///< [in] 順序付き割り当て式
  );

  /// @brief instantiate_delay の下請け関数
  ///
  /// * 配列の要素数は最大で 3
  /// 不適切な式ならば内部でエラーメッセージを出力して nullptr を返す．
  const VlDelay*
  instantiate_delay_sub(
    const VlScope* parent,                    ///< [in] 親のスコープ
    const FileRegion& loc,                    ///< [in] ファイル位置
    const std::vector<AstExpr>& ast_expr_list ///< [in] 遅延式の配列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プライマリ式の生成を行う．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_primary(
    const VlScope* parent,   ///< [in] 親のスコープ
    const ElbEnv& env,	     ///< [in] 生成時の環境
    const AstExpr& ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief イベント式用のプライマリ式の生成を行う．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_event_primary(
    const VlScope* parent,   ///< [in] 親のスコープ
    const ElbEnv& env,	     ///< [in] 生成時の環境
    const AstExpr& ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief instantiate_lhs() の下請け関数
  void
  instantiate_lhs_sub(
    const VlScope* parent,                ///< [in] 親のスコープ
    const ElbEnv& env,	                  ///< [in] 生成時の環境
    const AstExpr& ast_expr,              ///< [in] 式を表すパース木
    std::vector<const VlExpr*>& expr_list ///< [out] 結果を格納するリスト
  );

  /// @brief 左辺のプライマリ式の生成を行う．
  /// @return 生成された ElbPrimary を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  void
  instantiate_lhs_primary(
    const VlScope* parent,                ///< [in] 親のスコープ
    const ElbEnv& env,        	          ///< [in] 生成時の環境
    const AstExpr& ast_expr,              ///< [in] 式を表すパース木
    std::vector<const VlExpr*>& expr_list ///< [out] 結果を格納するリスト
  );

  /// @brief AstOpr から ElbExpr を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_opr(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,	    ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstFuncCall から ElbExpr を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_funccall(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,	    ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );


  /// @brief AstSysFuncCall から引数を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_sysfunccall(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,	    ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief genvar に対応した定数を生成する．
  ///
  /// ast_expr に添字が付いていたらエラーとなる．
  ElbExpr*
  instantiate_genvar(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstExpr& ast_expr, ///< [in] 式を表すパース木
    int val                  ///< [in] 値
  );

  /// @brief 宣言要素への参照を作る．
  ElbDeclRef
  instantiate_decl_ref(
    ObjHandle* handle,       ///< [in] オブジェクトハンドル
    const VlScope* parent,   ///< [in] 親のスコープ
    const ElbEnv& index_env, ///< [in] インデックス用の環境
    const AstExpr& ast_expr, ///< [in] 式を表すパース木
    AstExpr& ast_bit         ///< [out] ビット選択式
  );

  /// @brief ビット選択/部分選択の情報を実体化する．
  ElbSelecter
  instantiate_selecter(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstExpr& ast_expr,    ///< [in] 式を表すパース木
    const VlDeclBase* declbase, ///< [in] 対象の宣言要素
    const ElbEnv& index_env,    ///< [in] インデックス用の環境
    const AstExpr& ast_bit,     ///< [in] ビット選択式
    bool var_index              ///< [in] 可変インデックスを許可する時 true
  );

  /// @brief ビット選択式を作る．
  ElbSelecter
  instantiate_bitselect(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstExpr& ast_expr,    ///< [in] 式を表すパース木
    const VlDeclBase* declbase, ///< [in] 対象の宣言要素
    const ElbEnv& index_env,    ///< [in] インデックス用の環境
    const AstExpr& ast_bit,     ///< [in] ビット選択式
    bool var_index              ///< [in] 可変インデックスを許可する時 true
  );

  /// @brief 範囲選択式を作る．
  ElbSelecter
  instantiate_partselect(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstExpr& ast_expr,    ///< [in] 式を表すパース木
    const VlDeclBase* declbase, ///< [in] 対象の宣言要素
    const ElbEnv& index_env,    ///< [in] インデックス用の環境
    const AstPart& ast_part,    ///< [in] 範囲選択式
    bool var_index              ///< [in] 可変インデックスを許可する時 true
  );

  /// @brief 固定型の範囲選択式を作る．
  ElbSelecter
  instantiate_const_partselect(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstExpr& ast_expr,    ///< [in] 式を表すパース木
    const VlDeclBase* declbase, ///< [in] 対象の宣言要素
    const AstExpr& ast_left,    ///< [in] 範囲選択式1
    const AstExpr& ast_right    ///< [in] 範囲選択式2
  );

  /// @brief プラス型の範囲選択式を作る．
  ElbSelecter
  instantiate_plus_partselect(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstExpr& ast_expr,    ///< [in] 式を表すパース木
    const VlDeclBase* declbase, ///< [in] 対象の宣言要素
    const ElbEnv& index_env,    ///< [in] インデックス用の環境
    const AstExpr& ast_left,    ///< [in] 範囲選択式1
    const AstExpr& ast_right,   ///< [in] 範囲選択式2
    bool var_index              ///< [in] 可変インデックスを許可する時 true
  );

  /// @brief マイナス型の範囲選択式を作る．
  ElbSelecter
  instantiate_minus_partselect(
    const VlScope* parent,      ///< [in] 親のスコープ
    const AstExpr& ast_expr,    ///< [in] 式を表すパース木
    const VlDeclBase* declbase, ///< [in] 対象の宣言要素
    const ElbEnv& index_env,    ///< [in] インデックス用の環境
    const AstExpr& ast_left,    ///< [in] 範囲選択式1
    const AstExpr& ast_right,   ///< [in] 範囲選択式2
    bool var_index              ///< [in] 可変インデックスを許可する時 true
  );

  /// @brief システム関数/タスクの引数を返す．
  ElbExpr*
  instantiate_system_tf_arg(
    ObjHandle* handle,      ///< [in] オブジェクトハンドル
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief プライマリに対応するハンドルを求める．
  ObjHandle*
  find_handle(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] 環境
    const AstExpr& ast_expr ///< [in] プライマリを表す式
  );

  /// @brief 定数識別子を探す．
  ObjHandle*
  find_const_handle(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief decl の型が適切がチェックする．
  ///
  /// 不適切な場合，例外が送出される．
  void
  check_decl(
    const ElbEnv& env,          ///< [in] 環境
    const AstExpr& ast_expr,    ///< [in] 式を表すパース木
    const ElbDeclRef& decl_ref, ///< [in] 宣言要素の参照
    bool has_bit_select         ///< [in] ビット選択の有無
  );

};

END_NAMESPACE_YM_VERILOG

#endif // EXPRGEN_H
