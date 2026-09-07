#ifndef ELBPROXY_H
#define ELBPROXY_H

/// @file ElbProxy.h
/// @brief ElbProxy のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlValue.h"
#include "ym/vl/Ast.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"
#include "elaborator/Elaborator.h"
#include "elaborator/ElbMgr.h"
#include "elaborator/ElbFactory.h"
#include "ElbStub.h"


BEGIN_NAMESPACE_YM_VERILOG

class BitVector;
class ElbEnv;
struct RangeVal;
class ElbError;
class ElbParamCon;
class ElbScope;
class AstAttrInfo;

//////////////////////////////////////////////////////////////////////
/// @class ElbProxy ElbProxy.h "ElbProxy.h"
/// @brief Elaborator の代理クラス
/// @note このクラスを継承して用いる．
//////////////////////////////////////////////////////////////////////
class ElbProxy
{
protected:

  /// @brief コンストラクタ
  ElbProxy(
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///< [in] Elbオブジェクトを管理するクラス
  );

  /// @brief デストラクタ
  ~ElbProxy();


public:
  //////////////////////////////////////////////////////////////////////
  // 初期化
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  void
  init(
    ModuleGen* module_gen, ///< [in] モジュール生成用のオブジェクト
    DeclGen* decl_gen,     ///< [in] 宣言要素生成用のオブジェクト
    ItemGen* item_gen,     ///< [in] 構成要素生成用のオブジェクト
    StmtGen* stmt_gen,     ///< [in] ステートメント生成用のオブジェクト
    ExprGen* expr_gen,     ///< [in] 式生成用のオブジェクト
    ExprEval* expr_eval,   ///< [in] 定数式評価用のオブジェクト
    AttrGen* attr_gen      ///< [in] 属性生成用のオブジェクト
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // コンパイルオプション
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する宣言に範囲があり，IO宣言に範囲がない記述を認めるか
  bool
  allow_empty_io_range();


protected:
  //////////////////////////////////////////////////////////////////////
  // 検索用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前からモジュール定義を取り出す．
  /// @return name という名のモジュール定義
  /// @return なければ nullptr を返す．
  AstModule
  find_moduledef(
    const std::string& name ///< [in] 名前
  ) const
  {
    return mElaborator.find_moduledef(name);
  }

  /// @brief 関数定義を探す．
  AstItem
  find_funcdef(
    const VlModule* module, ///< [in] 親のモジュール
    const std::string& name      ///< [in] 関数名
  ) const
  {
    return mElaborator.find_funcdef(module, name);
  }

  /// @brief オブジェクトを探す．
  ///
  /// 見つからなかった時は nullptr を返す．
  ObjHandle*
  find_obj(
    const VlScope* parent,  ///< [in] 検索対象のスコープ
    const std::string& name ///< [in] 名前
  ) const
  {
    return mElaborator.find_obj(parent, name);
  }

  /// @brief constant function を取り出す．
  /// @return parent というスコープ内の name という関数を返す．
  /// @return なければ nullptr を返す．
  const VlTaskFunc*
  find_constant_function(
    const VlScope* parent, ///< [in] 検索対象のスコープ
    const std::string& name     ///< [in] 名前
  ) const
  {
    return mElaborator.find_constant_function(parent, name);
  }

  /// @brief セルの探索
  /// @return name という名のセルを返す．
  ///
  /// なければ不正値を返す．
  ClibCell
  find_cell(
    const std::string& name ///< [in] セル名
  ) const
  {
    return mElaborator.find_cell(name);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief constant function を登録する．
  void
  reg_constant_function(
    const VlTaskFunc* func ///< [in] 関数
  )
  {
    mElaborator.reg_constant_function(func);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // モジュールのインスタンス化に関する操作
  //////////////////////////////////////////////////////////////////////

  /// @brief インスタンス化の印を付ける．
  void
  set_instance_mark(
    const AstModule& ast_module
  )
  {
    mElaborator.set_instance_mark(ast_module);
  }

  /// @brief インスタンス化の印を消す．
  void
  clear_instance_mark(
    const AstModule& ast_module
  )
  {
    mElaborator.clear_instance_mark(ast_module);
  }

  /// @brief インスタンス化の印を調べる．
  bool
  check_instance_mark(
    const AstModule& ast_module
  ) const
  {
    return mElaborator.check_instance_mark(ast_module);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // elaboration で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 後で処理する defparam 文を登録する．
  void
  add_defparamstub(
    const VlModule* module, ///< [in] 親のモジュール
    const AstItem& header   ///< [in] アイテムテンプレートのヘッダ (defparam を持つ)
  )
  {
    mElaborator.add_defparamstub(module, header);
  }

  /// @brief phase1 で行う処理を登録する．
  void
  add_phase1stub(
    ElbStub* stub ///< [in] phase1 で行う処理を表すスタブ
  )
  {
    mElaborator.add_phase1stub(stub);
  }

  /// @brief phase2 で行う処理を登録する．
  void
  add_phase2stub(
    ElbStub* stub ///< [in] phase2 で行う処理を表すスタブ
  )
  {
    mElaborator.add_phase2stub(stub);
  }

  /// @brief phase3 で行う処理を登録する．
  void
  add_phase3stub(
    ElbStub* stub ///< [in] phase3 で行う処理を表すスタブ
  )
  {
    mElaborator.add_phase3stub(stub);
  }

  /// @brief 1引数版の ElbStub を作る．
  template<typename T,
	   typename A>
  ElbStub*
  make_stub(
    T* obj,
    void (T::*memfunc)(A),
    A a
  )
  {
    return new ElbStubT1<T, A>(obj, memfunc, a);
  }

  /// @brief 2引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B>
  ElbStub*
  make_stub(
    T* obj,
    void (T::*memfunc)(A, B),
    A a,
    B b
  )
  {
    return new ElbStubT2<T, A, B>(obj, memfunc, a, b);
  }

  /// @brief 3引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B,
	   typename C>
  ElbStub*
  make_stub(
    T* obj,
    void (T::*memfunc)(A, B, C),
    A a,
    B b,
    C c
  )
  {
    return new ElbStubT3<T, A, B, C>(obj, memfunc, a, b, c);
  }

  /// @brief 4引数版の ElbStub を作る．
  template<typename T,
	   typename A,
	   typename B,
	   typename C,
	   typename D>
  ElbStub*
  make_stub(
    T* obj,
    void (T::*memfunc)(A, B, C, D),
    A a,
    B b,
    C c,
    D d
  )
  {
    return new ElbStubT4<T, A, B, C, D>(obj, memfunc, a, b, c, d);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // ElbModule の生成関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module の中身のうちスコープに関係するインスタンス化を行う．
  void
  phase1_module_item(
    ElbModule* module,                        ///< [in] モジュール
    const AstModule& ast_module,              ///< [in] モジュール定義
    const std::vector<ElbParamCon>& param_con_list ///< [in] パラメータ割り当ての情報
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 宣言要素のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief parameter と genvar を実体化する．
  void
  phase1_decl(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstDeclHeadList& ast_head_list, ///< [in] 宣言ヘッダのリスト
    bool force_to_local    ///< [in] true なら parameter を localparam にする．
  );

  /// @brief IO宣言要素を実体化する．
  void
  instantiate_iodecl(
    ElbModule* module,                 ///< [in] 親のモジュール
    const AstIOHeadList& ast_head_list ///< [in] IO宣言ヘッダの配列
  );

  /// @brief IO宣言要素を実体化する．
  void
  instantiate_iodecl(
    ElbTaskFunc* taskfunc,             ///< [in] 親のタスク/関数
    const AstIOHeadList& ast_head_list ///< [in] IO宣言ヘッダの配列
  );

  /// @brief 宣言要素のリストをインスタンス化する．
  void
  instantiate_decl(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstDeclHeadList& ast_head_list ///< [in] 宣言ヘッダの配列
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 構成要素 (module item) のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係する要素を実体化する．
  void
  phase1_items(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItemList& ast_item_list ///< [in] 要素定義の配列
  );

  /// @brief constant function の生成を行う．
  const VlTaskFunc*
  instantiate_constant_function(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstItem& ast_function ///< [in] 関数定義
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // ステートメントのインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープに関係するステートメントの実体化を行う．
  void
  phase1_stmt(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstStmt& ast_stmt, ///< [in] 対象のステートメント
    bool cf = false          ///< [in] constant function 中のステートメントの時 true
  );

  /// @brief ステートメントの実体化を行う．
  const VlStmt*
  instantiate_stmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス
    const ElbEnv& env,        ///< [in] 生成時の環境
    const AstStmt& ast_stmt   ///< [in] 対象のステートメント
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 式のインスタンス化関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AstExpr から ElbExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_expr(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr から定数式の ElbExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_constant_expr(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr からイベント式の ElbExpr を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_event_expr(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr からシステム関数の引数を生成する．
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_arg(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr から左辺式を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_lhs(
    const VlScope* parent,  ///< [in] 親のスコープ
    const ElbEnv& env,      ///< [in] 生成時の環境
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstExpr から右辺式を生成する
  /// @return 生成された ElbExpr のポインタを返す．
  ///
  /// 不適切な式ならばエラーメッセージを出力し nullptr を返す．
  ElbExpr*
  instantiate_rhs(
    const VlScope* parent,   ///< [in] 親のスコープ
    const ElbEnv& env,       ///< [in] 生成時の環境
    const AstExpr& ast_expr, ///< [in] 式を表すパース木
    ElbExpr* lhs             ///< [in] 左辺式
  );

  /// @brief AstExpr(primary) から named_event を生成する．
  ElbExpr*
  instantiate_namedevent(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief AstDelay から ElbExpr を生成する．
  const VlDelay*
  instantiate_delay(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstDelay& ast_delay ///< [in] 遅延を表すパース木
  );

  /// @brief AstOrderedCon から ElbExpr を生成する．
  ///
  /// これは AstInst の前にある # つきの式がパラメータ割り当てなのか
  /// 遅延なのかわからないので AstOrderedCon で表していることによる．
  const VlDelay*
  instantiate_delay(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstItem& ast_head ///< [in] 順序付きわりあて式
  );

  /// @brief 定数式の値を評価する．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  VlValue
  evaluate_expr(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief 定数式を評価し int 値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * 評価結果が int でなければ EvalError 例外を送出する．
  int
  evaluate_int(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief 定数式ならばを評価し int 値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 評価結果が int でなければ EvalError 例外を送出する．
  int
  evaluate_int_if_const(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstExpr& ast_expr, ///< [in] 式を表すパース木
    bool& is_const           ///< [out] 定数式の時に true を返す．
  );

  /// @brief 定数式を評価しスカラー値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * いかなる型でもスカラー値に変換可能
  VlScalarVal
  evaluate_scalar(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief 定数式を評価し bool 値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * いかなる型でも bool 値に変換可能
  bool
  evaluate_bool(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief 定数式を評価しビットベクタ値を返す．
  /// @return 評価した値を返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * 評価結果がビットベクタ型でなければ EvalError 例外を送出する．
  BitVector
  evaluate_bitvector(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstExpr& ast_expr ///< [in] 式を表すパース木
  );

  /// @brief 範囲を表す式を評価する．
  /// @param[return] 範囲の MSB と LSB の値のペアを返す．
  ///
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * 評価結果が int でなければ EvalError 例外を送出する．
  RangeVal
  evaluate_range(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstRange& ast_range ///< [in] 範囲を表すパーす木
  );

  /// @brief 範囲を表す式を評価する．
  /// @param[return] 範囲の MSB と LSB の値のペアを返す．
  ///
  /// * ast_part->mode() が VpiRangeMode::Const でなければ例外を送出する．
  /// * 定数式でなければ EvalError 例外を送出する．
  /// * 評価結果が int でなければ EvalError 例外を送出する．
  RangeVal
  evaluate_range(
    const VlScope* parent,  ///< [in] 親のスコープ
    const AstPart& ast_part ///< [in] 範囲指定を表すパーす木
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // attribute instance の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 構文木要素に対応する属性リストを返す．
  const std::vector<const VlAttribute*>&
  attribute_list(
    const AstBase& ast_obj ///< [in] 元となる構文木要素
  );

  /// @brief 構文木要素に対応する属性リストを返す．
  std::vector<const VlAttribute*>
  attribute_list(
    const AstBase& ast_obj1,
    const AstBase& ast_obj2
  );

protected:
  //////////////////////////////////////////////////////////////////////
  // チェック関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 同名のオブジェクトが定義されていないか調べる．
  ///
  /// エラーの場合には ElbError 例外を送出する．
  void
  check_name(
    const VlScope* parent,        ///< [in] 親のスコープ
    const char* name,             ///< [in] 名前
    const FileRegion& file_region ///< [in] name のファイル位置
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // エラー出力
  //////////////////////////////////////////////////////////////////////

  /// @brief 重複した名前を持つ．
  void
  error_dup_name(
    const char* file_name,      ///< [in] ソースプログラムのファイル名
    int line,                   ///< [in] ソースプログラムの行番号
    const FileRegion& loc,      ///< [in] name のファイル位置
    const char* name,           ///< [in] 対象の名前
    const FileRegion& prev_loc  ///< [in] 直前に現れたファイル位置
  );

  /// @brief 対象の要素が見つからない．
  void
  error_not_found(
    const char* file_name,         ///< [in] ファイル名
    int line,                      ///< [in] 行番号
    const FileRegion& file_region, ///< [in] ファイル位置
    const char* name               ///< [in] 名前
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // メッセージ出力
  //////////////////////////////////////////////////////////////////////

  /// @brief エラーメッセージを出力する．
  void
  put_error(
    const ElbError& error ///< [in] エラー情報
  );

  /// @brief 警告メッセージを出力する．
  void
  put_warning(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 警告箇所
    const char* label,     ///< [in] ラベル
    const std::string& msg ///< [in] メッセージ
  );

  /// @brief 情報メッセージを出力する．
  void
  put_info(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 対象の箇所
    const char* label,     ///< [in] ラベル
    const std::string& msg ///< [in] メッセージ
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // メンバに対するアクセス
  //////////////////////////////////////////////////////////////////////

  /// @brief ElbMgr を返す．
  ElbMgr&
  mgr()
  {
    return mMgr;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の生成器
  Elaborator& mElaborator;

  // 生成したオブジェクトを管理するマネージャクラス
  ElbMgr& mMgr;

  // モジュール生成用のオブジェクト
  ModuleGen* mModuleGen;

  // 宣言要素生成用のオブジェクト
  DeclGen* mDeclGen;

  // 構成要素生成用のオブジェクト
  ItemGen* mItemGen;

  // ステートメント生成用のオブジェクト
  StmtGen* mStmtGen;

  // 式生成用のオブジェクト
  ExprGen* mExprGen;

  // 定数式評価用のオブジェクト
  ExprEval* mExprEval;

  // attribute instance 生成用のオブジェクト
  AttrGen* mAttrGen;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBPROXY_H
