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
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///< [in] Elbオブジェクトを管理するクラス
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
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,     ///< [in] 生成時の環境
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief AstExpr から定数式の ElbExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_constant_expr(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief AstExpr からイベント式の ElbiExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_event_expr(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,     ///< [in] 生成時の環境
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief AstExpr からシステム関数の引数を生成する．
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_arg(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief AstExpr から左辺式を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_lhs(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief AstExpr(primary) から named_event を生成する．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_namedevent(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief AstDelay から ElbExpr を生成する．
  ///
  /// 不適切な式ならば内部でエラーメッセージを出力して nullptr を返す．
  /// 例外は送出しない
  const VlDelay*
  instantiate_delay(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstDelay* ast_delay ///< [in] 遅延を表すパース木
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
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_head  ///< [in] 順序付き割り当て式
  );

  /// @brief instantiate_delay の下請け関数
  ///
  /// * ast_obj は AstDelay か AstItem のどちらか
  /// * 配列の要素数は最大で 3
  ///
  /// 不適切な式ならば内部でエラーメッセージを出力して nullptr を返す．
  const VlDelay*
  instantiate_delay_sub(
    const VlScope* parent,           ///< [in] 親のスコープ
    const AstBase* ast_obj,          ///< [in] 遅延式を表すパース木
    const std::vector<const AstExpr*>& ast_expr_list ///< [in] 遅延式の配列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AstExpr から左辺式を生成する
  /// @return 生成した式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_lhs_sub(
    const VlScope* parent,       ///< [in] 親のスコープ
    const ElbEnv& env,           ///< [in] 生成時の環境
    const AstExpr* ast_expr,       ///< [in] 式を表すパース木
    std::vector<ElbExpr*>& elem_array ///< [out] 生成した左辺式の要素を格納するベクタ
  );

  /// @brief AstPrimary から ElbExpr を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_primary(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief AstOpr から ElbExpr を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_opr(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief AstFuncCall から ElbExpr を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_funccall(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );


  /// @brief AstSysFuncCall から引数を生成する．
  /// @return 生成された式を返す．
  ///
  /// 不適切な式ならば ElbError 例外を送出する．
  ElbExpr*
  instantiate_sysfunccall(
    const VlScope* parent, ///< [in] 親のスコープ
    const ElbEnv& env,	   ///< [in] 生成時の環境
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief 定数識別子を探す．
  ObjHandle*
  find_const_handle(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstExpr* ast_expr  ///< [in] 式を表すパース木
  );

  /// @brief genvar に対応した定数を生成する．
  ///
  /// ast_expr に添字が付いていたらエラーとなる．
  ElbExpr*
  instantiate_genvar(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstExpr* ast_expr, ///< [in] 式を表すパース木
    int val                ///< [in] 値
  );

  /// @brief 宣言要素のインスタンス化を行う．
  ElbExpr*
  instantiate_primary_sub(
    ObjHandle* handle,      ///< [in] オブジェクトハンドル
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] インスタンス化している環境
    const AstExpr* ast_expr,  ///< [in] 式を表すパース木
    bool& is_array,         ///< [out] 対象が配列の時 true を返す．
    bool& has_range_select, ///< [out] 範囲指定を持っていたら true を返す．
    bool& has_bit_select    ///< [out] ビット指定を持っていたら true を返す．
  );

  /// @brief decl の型が適切がチェックする．
  ///
  /// 不適切な場合，例外が送出される．
  void
  check_decl(
    const ElbEnv& env,     ///< [in] インスタンス化している環境
    const AstExpr* ast_expr, ///< [in] 式を表すパース木
    VpiObjType decl_type,  ///< [in] 対象の宣言要素の型
    bool is_array,         ///< [in] 対象が配列の時 true を渡す．
    bool has_select        ///< [in] ビット/範囲指定を持つ時 true を渡す．
  );

};

END_NAMESPACE_YM_VERILOG

#endif // EXPRGEN_H
