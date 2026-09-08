#ifndef COMMON_LOGMGR_H
#define COMMON_LOGMGR_H

/// @file LogMgr.h
/// @brief LogMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

class AstConnection;
class AstDefParam;
class AstExpr;
class AstIOItem;
class AstInst;
class AstItem;
class AstModule;
class AstStmt;
class ElbDecl;
class ElbError;
class ElbGenvar;
class ElbModule;
class ElbParamCon;
class ElbParameter;
class ElbPrimitive;
class ElbPrimArray;
class ObjHandle;
class RangeVal;
class VlContAssign;
class VlDecl;
class VlDeclArray;
class VlScope;

//////////////////////////////////////////////////////////////////////
/// @class LogMgr LogMgr.h "common/LogMgr.h"
/// @brief ログ出力を管理するクラス
//////////////////////////////////////////////////////////////////////
class LogMgr
{
public:

  /// @brief コンストラクタ
  LogMgr() = default;

  /// @brief デストラクタ
  ~LogMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// エラー出力
  ///
  /// これらの関数は ElbError 例外を送出する．
  /// ログに出力するためには以下のようなコードを用いる．
  /// @code
  /// try {
  ///   error_XXXX(....);
  /// }
  /// catch ( const ElbError& error ) {
  ///   put_error(error);
  /// }
  /// @endcode
  //////////////////////////////////////////////////////////////////////

  /// @brief 同名のモジュール定義がある．
  void
  error_module_redefined(
    const char* file,                  ///< [in] ファイル名
    int line,                          ///< [in] 行番号
    const AstModule& ast_module,       ///< [in] モジュール定義
    const FileRegion& prev_file_region ///< [in] 前の定義位置
  );

  /// @brief 同名の関数定義がある．
  void
  error_function_redefined(
    const char* file,                  ///< [in] ファイル名
    int line,                          ///< [in] 行番号
    const AstItem& ast_funcdef,        ///< [in] 関数定義
    const FileRegion& prev_file_region ///< [in] 前の定義位置
  );

  /// @brief 未解決の defparam 文がある．
  void
  error_defparam_unresolved(
    const char* file_name,          ///< [in] ファイル名
    int line,                       ///< [in] 行番号
    const AstDefParam& ast_defparam ///< [in] defparam 定義
  );

  /// @brief パラメータポートの割り当て数が多すぎる．
  void
  error_too_many_params(
    const char* file_name,        ///< [in] ファイル名
    int line,                     ///< [in] 行番号
    const FileRegion& file_region ///< [in] ファイル位置
  );

  /// @brief パラメータポートに現れるパラメータが存在しない．
  void
  error_param_not_found(
    const char* file_name,       ///< [in] ファイル名
    int line,                    ///< [in] 行番号
    const AstConnection& ast_con ///< [in] パラメータポート割り当てのパース木
  );

  /// @brief ポートに配列が使われている．
  void
  error_array_in_port_connection(
    const char* file_name, ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& loc  ///< [in] ファイル位置
  );

  /// @brief ポートに使われている要素が宣言要素でなかった．
  void
  error_illegal_port(
    const char* file_name, ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& file_region ///< [in] ファイル位置
  );

  /// @brief 重複した名前を持つ．
  void
  error_dup_name(
    const char* file,          ///< [in] ソースプログラムのファイル名
    int line,                  ///< [in] ソースプログラムの行番号
    const FileRegion& loc,     ///< [in] name のファイル位置
    const char* name,          ///< [in] 対象の名前
    const FileRegion& prev_loc ///< [in] 直前に現れたファイル位置
  );

  /// @brief 対象の要素が見つからない．
  void
  error_not_found(
    const char* file,              ///< [in] ファイル名
    int line,                      ///< [in] 行番号
    const FileRegion& file_region, ///< [in] ファイル位置
    const char* name               ///< [in] 名前
  );

  /// @brief IO 宣言に aux_type と宣言が重複している．
  void
  error_duplicate_type(
    const char* file,          ///< [in] ファイル名
    int line,                  ///< [in] 行番号
    const AstIOItem& ast_item, ///< [in] IO要素定義
    const FileRegion& prev_loc ///< [in] 同名の宣言要素のファイル位置
  );

  /// @brief IO 宣言に配列型の要素が現れている．
  void
  error_array_in_io_decl(
    const char* file,         ///< [in] ファイル名
    int line,                 ///< [in] 行番号
    const AstIOItem& ast_item ///< [in] IO要素定義
  );

  /// @brief IO 宣言に不適切な型の要素が現れている．
  void
  error_illegal_io_decl(
    const char* file,          ///< [in] ファイル名
    int line,                  ///< [in] 行番号
    const AstIOItem& ast_item, ///< [in] IO要素定義
    bool is_module             ///< [in] モジュールの時 true
  );

  /// @brief IO宣言と要素宣言の範囲が異なる．
  void
  error_conflict_io_range(
    const char* file,          ///< [in] ファイル名
    int line,                  ///< [in] 行番号
    const AstIOItem& ast_item, ///< [in] IO要素定義
    ElbDecl* decl = nullptr    ///< [in] 同名の要素
  );

  /// @brief 暗黙のネット宣言は許されていない．
  void
  error_no_impnet(
    const char* file,          ///< [in] ファイル名
    int line,                  ///< [in] 行番号
    const AstIOItem& ast_item  ///< [in] IO要素定義
  );

  /// @brief 暗黙のネット宣言は初期値を持てない．
  void
  error_impnet_with_init(
    const char* file,          ///< [in] ファイル名
    int line,                  ///< [in] 行番号
    const AstIOItem& ast_item  ///< [in] IO要素定義
  );

  /// @brief 対象がパラメータではなかった．
  void
  error_not_a_parameter(
    const char* file_name,
    int line,
    const FileRegion& file_region,
    const std::string& name
  );

  /// @brief defparam の割り当て対象が localparameter だった．
  void
  error_localparam_override(
    const char* file_name,
    int line,
    const AstDefParam& ast_defparam
  );

  /// @brief モジュールの依存関係がループしている．
  void
  error_cyclic_dependency(
    const char* file,           ///< [in] ファイル名
    int line,                   ///< [in] 行番号
    const AstModule& ast_module ///< [in] モジュール定義
  );

  /// @brief 名無しのモジュールインスタンス
  void
  error_noname_module(
    const char* file_name,
    int line,
    const AstInst& ast_inst
  );

  /// @brief UDP インスタンスにパラメータ割り当てがある．
  void
  error_udp_with_paramassign(
    const char* file,        ///< [in] ファイル名
    int line,                ///< [in] 行番号
    const AstItem& ast_head  ///< [in] インスタンス記述のヘッダ
  );

  /// @brief セルインスタンスにパラメータ割り当てがある．
  void
  error_cell_with_paramassign(
    const char* file,        ///< [in] ファイル名
    int line,                ///< [in] 行番号
    const AstItem& ast_head  ///< [in] インスタンス記述のヘッダ
  );

  /// @brief モジュールインスタンスのポート数が合わない．
  void
  error_port_num_mismatch(
    const char* file,       ///< [in] ファイル名
    int line,               ///< [in] 行番号
    const AstInst& ast_inst ///< [in] インスタンス記述
  );

  /// @brief ポートが見つからない．
  void
  error_port_not_found(
    const char* file,             ///< [in] ファイル名
    int line,                     ///< [in] 行番号
    const AstConnection& ast_conn ///< [in] ポート結合記述
  );

  /// @brief ポート結合式に real タイプの式が使われている．
  void
  error_real_type_in_port_list(
    const char* file,       ///< [in] ファイル名
    int line,               ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] ポート結合式
  );

  /// @brief モジュールインスタンスのポート結合式のビット幅が合わない．
  void
  error_port_size_mismatch(
    const char* file,       ///< [in] ファイル名
    int line,               ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] ポート結合式
  );

  /// @brief UDP instance に名前付きポート割り当てがあった．
  void
  error_named_port_in_udp_instance(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstInst& ast_inst ///< [in] 対象の構文木要素
  );

  /// @brief cell instance のピン名が合わない．
  void
  error_pin_name_not_found(
    const char* file,          ///< [in] ファイル名
    int line,		       ///< [in] 行番号
    const AstConnection& ast_con ///< [in] 対象の構文木要素
  );

  /// @brief 空のポート式
  void
  error_empty_port_expression(
    const char* file,          ///< [in] ファイル名
    int line,		       ///< [in] 行番号
    const AstConnection& ast_con ///< [in] 対象の構文木要素
  );

  /// @brief genvar が見つからなかった．
  void
  error_genvar_not_found(
    const char* file_name,
    int line,
    const AstItem& ast_genfor
  );

  /// @brief genvar が見つからなかった．
  void
  error_not_a_genvar(
    const char* file_name,
    int line,
    const AstItem& ast_genfor
  );

  /// @brief genvar が他のループで用いられている．
  void
  error_genvar_in_use(
    const char* file_name,
    int line,
    const AstItem& ast_genfor
  );

  /// @brief genvar の値が負になった．
  void
  error_genvar_negative(
    const char* file_name,
    int line,
    const AstItem& ast_genfor
  );

  /// @brief gate instance のポート結合の数が少ない．
  void
  error_few_gate_conn(
    const char* file_name,
    int line,
    const AstInst& ast_inst
  );

  /// @brief gate instance のポート結合の数が多い．
  void
  error_many_gate_conn(
    const char* file_name,
    int line,
    const AstInst& ast_inst
  );

  /// @brief int 型が要求されている所で互換性のない型があった．
  void
  error_int_required(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const FileRegion& loc ///< [in] エラー箇所
  );

  /// @brief ビットベクタ型が要求されている所で互換性のない型があった．
  void
  error_bv_required(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const FileRegion& loc ///< [in] エラー箇所
  );

  /// @brief 通常の式中に edge descriptor
  void
  error_illegal_edge_descriptor(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief real 型のオペランドをとれない
  void
  error_illegal_real_type(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 該当する関数が存在しない．
  void
  error_no_such_function(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 該当するシステム関数が存在しない．
  void
  error_no_such_sysfunction(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 該当するシステムタスクが存在しない．
  void
  error_no_such_systask(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstStmt& ast_stmt ///< [in] 対象の構文木要素
  );

  /// @brief 関数ではない．
  void
  error_not_a_function(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 引数の数が合わない．
  void
  error_argument_num_mismatch(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 引数の数が合わない．
  void
  error_argument_num_mismatch(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstStmt& ast_stmt ///< [in] 対象の構文木要素
  );

  /// @brief 引数の型が合わない．
  void
  error_argument_type_mismatch(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトの型が不適切
  void
  error_illegal_object(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトが named-event でなかった
  void
  error_not_a_namedevent(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 要素の範囲の順番と範囲指定の順番が異なる．
  void
  error_range_order(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief named-event に対する範囲指定
  void
  error_select_for_namedevent(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief assign/deassign に不適切なビット/範囲指定
  void
  error_select_in_pca(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief force/release に不適切なビット/範囲指定
  void
  error_select_in_force(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief assign/deassign に不適切な配列要素
  void
  error_array_in_pca(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief force/release に不適切な配列要素
  void
  error_array_in_force(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 配列の次元が合わない
  void
  error_dimension_mismatch(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief real 型に対するビット選択あるいは部分選択があった
  void
  error_select_for_real(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief constant function 中にシステム関数呼び出し
  void
  error_illegal_sysfunccall_in_cf(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief constant expression 中にシステム関数呼び出し
  void
  error_illegal_sysfunccall_in_ce(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 定数関数は自己再帰できない．
  void
  error_uses_itself(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 定数関数ではない．
  void
  error_not_a_constant_function(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトの型が constant function 用として不適切
  void
  error_illegal_object_cf(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 階層名が constant expression 中にあった
  void
  error_hname_in_ce(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 階層名が constant function 中にあった
  void
  error_hname_in_cf(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief オブジェクトが parameter でなかった
  void
  error_not_a_parameter(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief イベント式の根元に定数
  void
  error_illegal_constant_in_event_expression(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief イベント式の根元に関数呼び出し
  void
  error_illegal_funccall_in_event_expression(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief イベント式の根元にシステム関数呼び出し
  void
  error_illegal_sysfunccall_in_event_expression(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 左辺式で用いることのできない演算子
  void
  error_illegal_operator_in_lhs(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 左辺式に定数
  void
  error_illegal_constant_in_lhs(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 左辺式に関数呼び出し
  void
  error_illegal_funccall_in_lhs(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );

  /// @brief 左辺式にシステム関数呼び出し
  void
  error_illegal_sysfunccall_in_lhs(
    const char* file,     ///< [in] ファイル名
    int line,		  ///< [in] 行番号
    const AstExpr& ast_expr ///< [in] 対象の構文木要素
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 警告出力
  //
  // 内部で put_warning() を呼ぶ．
  //////////////////////////////////////////////////////////////////////

  /// @brief 添字が範囲外
  void
  warning_index_out_of_range(
    const char* file_name, ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& file_region ///< [in] ファイル位置
  );

  /// @brief 左の範囲が範囲外
  void
  warning_left_index_out_of_range(
    const char* file_name, ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& file_region ///< [in] ファイル位置
  );

  /// @brief 右の範囲が範囲外
  void
  warning_right_index_out_of_range(
    const char* file_name, ///< [in] ファイル名
    int line,              ///< [in] 行番号
    const FileRegion& file_region ///< [in] ファイル位置
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 情報出力
  //
  // 内部で put_info() を呼ぶ．
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュール配列のインスタンス生成
  void
  info_module_array(
    const char* file,
    int line,
    const AstItem& ast_head,
    const AstInst& ast_inst,
    const RangeVal& range
  );

  /// @brief モジュールのインスタンス生成
  void
  info_module(
    const char* file,
    int line,
    ElbModule* module
  );

  /// @brief IO宣言のインスタンス生成
  void
  info_iodecl(
    const char* file,
    int line,
    const AstIOItem& ast_item,
    const VlScope* scope
  );

  /// @brief パラメータのインスタンス生成
  void
  info_param(
    const char* file,
    int line,
    const VlDecl* decl
  );

  /// @brief ネット配列のインスタンス生成
  void
  info_net_array(
    const char* file,
    int line,
    const VlDeclArray* decl_array
  );

  /// @brief ネットのインスタンス生成
  void
  info_net(
    const char* file,
    int line,
    const VlDecl* decl
  );

  /// @brief Reg配列のインスタンス生成
  void
  info_reg_array(
    const char* file,
    int line,
    const VlDeclArray* decl_array
  );

  /// @brief Regのインスタンス生成
  void
  info_reg(
    const char* file,
    int line,
    const VlDecl* decl
  );

  /// @brief Var配列のインスタンス生成
  void
  info_var_array(
    const char* file,
    int line,
    const VlDeclArray* decl_array
  );

  /// @brief Varのインスタンス生成
  void
  info_var(
    const char* file,
    int line,
    const VlDecl* decl
  );

  /// @brief イベント配列のインスタンス生成
  void
  info_event_array(
    const char* file,
    int line,
    const VlDeclArray* decl_array
  );

  /// @brief イベントのインスタンス生成
  void
  info_event(
    const char* file,
    int line,
    const VlDecl* decl
  );

  /// @brief genvarのインスタンス生成
  void
  info_genvar(
    const char* file,
    int line,
    ElbGenvar* genvar
  );

  /// @brief defparam の生成
  void
  info_defparam(
    const char* file,
    int line,
    const FileRegion& file_region,
    ElbParameter* param,
    const AstExpr& ast_expr
  );

  /// @brief continuous assign の生成
  void
  info_contassign(
    const char* file,
    int line,
    const VlContAssign* cont_assign
  );

  /// @brief プリミティブ配列インスタンスの生成
  void
  info_prim_array(
    const char* file,
    int line,
    ElbPrimArray* prim_array
  );

  /// @brief プリミティブインスタンスの生成
  void
  info_primitive(
    const char* file,
    int line,
    ElbPrimitive* prim
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 低レベルの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief エラーメッセージを出力する．
  void
  put_error(
    const ElbError& error ///< [in] エラー情報
  );

  /// @brief 失敗メッセージを出力する．
  void
  put_error(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 対象の箇所
    const char* label,     ///< [in] ラベル
    const std::string& msg ///< [in] メッセージ
  );

  /// @brief 失敗メッセージを出力する．
  void
  put_failure(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 対象の箇所
    const char* label,     ///< [in] ラベル
    const std::string& msg ///< [in] メッセージ
  );

  /// @brief 警告メッセージを出力する．
  void
  put_warning(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 対象の箇所
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

  /// @brief デバッグメッセージを出力する．
  void
  put_debug(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 対象の箇所
    const char* label,     ///< [in] ラベル
    const std::string& msg ///< [in] メッセージ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_VERILOG

#endif // COMMON_LOGMGR_H
