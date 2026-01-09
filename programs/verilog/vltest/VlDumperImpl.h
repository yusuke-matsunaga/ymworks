#ifndef VLDUMPERIMPL_H
#define VLDUMPERIMPL_H

/// @file VlDumperImpl.h
/// @brief VlDumperImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlFwd.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;

//////////////////////////////////////////////////////////////////////
/// @class VlDumperImpl VlDumperImpl.h "VlDumperImpl.h"
/// @ingroup Vlgroup
/// @brief VlDumper の実装クラス
//////////////////////////////////////////////////////////////////////
class VlDumperImpl
{
public:

  /// @brief コンストラクタ
  VlDumperImpl(
    std::ostream& s ///< [in] 出力ストリーム
  );

  /// @brief デストラクタ
  ~VlDumperImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を出力する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief VlMgr の持っている内容を出力する
  void
  put(
    const VlMgr& vlmgr ///< [in] マネージャ
  );


public:
  //////////////////////////////////////////////////////////////////////
  // オプションの設定/取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置情報を出力するモードにする
  void
  enable_file_loc_mode();

  /// @brief ファイル位置情報の出力を抑制するモードにする．
  void
  disable_file_loc_mode();

  /// @brief ファイル位置情報を出力するモードのとき true を返す．
  bool
  file_loc_mode() const;

  /// @brief nullptr ポインタの出力を抑止する．
  void
  enable_nullptr_suppress_mode();

  /// @brief nullptr ポインタの出力を行う．
  void
  disable_nullptr_suppress_mode();

  /// @brief nullptr ポインタの出力抑止モードの状態を得る．
  bool
  nullptr_suppress_mode() const;

  /// @brief bit 展開した内容を出力する．
  void
  enable_bitexpand_mode();

  /// @brief bit 展開しない．
  void
  disable_bitexpand_mode();

  /// @brief bit 展開モードの時 true を返す．
  bool
  bitexpand_mode() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module の内容を出力する関数
  void
  put_module(
    const char* label,     ///< [in] ラベル
    const VlMgr& mgr,      ///< [in] VlMgr
    const VlModule* module ///< [in] オブジェクト
  );

  /// @brief module array の内容を出力する関数
  void
  put_modulearray(
    const char* label,               ///< [in] ラベル
    const VlMgr& mgr,                ///< [in] VlMgr
    const VlModuleArray* modulearray ///< [in] オブジェクト
  );

  /// @brief module array のリストの内容を出力する関数
  void
  put_modulearray_list(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const std::vector<const VlModuleArray*>& module_array_list
                       ///< [in] オブジェクト
  );

  /// @brief scope の内容を出力する関数
  void
  put_scope(
    const char* label,   ///< [in] ラベル
    const VlMgr& mgr,	 ///< [in] VlMgr
    const VlScope* scope ///< [in] オブジェクト
  );

  void
  put_scope_sub(
    const VlMgr& mgr,    ///< [in] VlMgr
    const VlScope* scope ///< [in] オブジェクト
  );

  /// @brief io decl の内容を出力する関数
  void
  put_iodecl(
    const char* label,     ///< [in] ラベル
    const VlMgr& mgr,	   ///< [in] VlMgr
    const VlIODecl* iodecl ///< [in] オブジェクト
  );

  /// @brief port の内容を出力する関数
  void
  put_port(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlPort* port ///< [in] オブジェクト
  );

  /// @brief 宣言要素の内容を出力する関数
  void
  put_decl(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlDecl* decl ///< [in] オブジェクト
  );

  /// @brief 宣言要素の内容を出力する関数
  void
  put_declarray(
    const char* label,           ///< [in] ラベル
    const VlMgr& mgr,		 ///< [in] VlMgr
    const VlDeclArray* declarray ///< [in] オブジェクト
  );

  /// @brief 宣言要素のリストの内容を出力する関数
  void
  put_decl_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlDecl*>& decl_list
                        ///< [in] オブジェクト
  );

  /// @brief 宣言要素のリストの内容を出力する関数
  void
  put_declarray_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlDeclArray*>& decl_list
                        ///< [in] オブジェクト
  );

  /// @brief def param のリストの内容を出力する関数
  void
  put_defparam_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlDefParam*>& defparam_list
                        ///< [in] オブジェクト
  );

  /// @brief param assign のリストの内容を出力する関数
  void
  put_paramassign_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlParamAssign*>& paramassign_list
                        ///< [in] オブジェクト
  );

  /// @brief primitive array のリストの内容を出力する関数
  void
  put_primarray_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlPrimArray*>& primarray_list
                        ///< [in] オブジェクト
  );

  /// @brief primitive の内容を出力する関数
  void
  put_primitive(
    const char* label,           ///< [in] ラベル
    const VlMgr& mgr,		 ///< [in] VlMgr
    const VlPrimitive* primitive ///< [in] オブジェクト
  );

  /// @brief primitive のリスト内容を出力する関数
  void
  put_primitive_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlPrimitive*>& primitive_list
                        ///< [in] オブジェクト
  );

  /// @brief prim term の内容を出力する関数
  void
  put_primterm(
    const char* label,         ///< [in] ラベル
    const VlMgr& mgr,	       ///< [in] VlMgr
    const VlPrimTerm* primterm ///< [in] オブジェクト
  );

  /// @brief udp defn の内容を出力する関数
  void
  put_udp_defn(
    const char* label,   ///< [in] ラベル
    const VlMgr& mgr,	 ///< [in] VlMgr
    const VlUdpDefn* udp ///< [in] オブジェクト
  );

  /// @brief task の内容を出力する関数
  void
  put_task(
    const char* label,     ///< [in] ラベル
    const VlMgr& mgr,	   ///< [in] VlMgr
    const VlTaskFunc* task ///< [in] オブジェクト
  );

  /// @brief function の内容を出力する関数
  void
  put_function(
    const char* label,        ///< [in] ラベル
    const VlMgr& mgr,	      ///< [in] VlMgr
    const VlTaskFunc* taskfun ///< [in] オブジェクトc
  );

#if 0
  /// @brief frame の内容を出力する関数
  void
  put_frame(
    const char* label,
    const VlHandle& handle
  );
#endif

  /// @brief continuous assignment の内容を出力する関数
  void
  put_contassign(
    const char* label,             ///< [in] ラベル
    const VlMgr& mgr,		   ///< [in] VlMgr
    const VlContAssign* contassign ///< [in] オブジェクト
  );

  /// @brief continuous assignment のリストの内容を出力する関数
  void
  put_contassign_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlContAssign*>& ca_list
                        ///< [in] オブジェクト
  );

  /// @brief initial/always の内容を出力する関数
  void
  put_process(
    const char* label,       ///< [in] ラベル
    const VlMgr& mgr,	     ///< [in] VlMgr
    const VlProcess* process ///< [in] オブジェクト
  );

  /// @brief initial/always のリストの内容を出力する関数
  void
  put_process_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlProcess*>& process_list
                        ///< [in] オブジェクト
  );

  /// @brief statement の内容を出力する関数
  void
  put_stmt(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlStmt* stmt ///< [in] オブジェクト
  );

  /// @brief statement のリストの内容を出力する関数
  void
  put_stmt_list(
    const char* label,  ///< [in] ラベル
    const VlMgr& mgr,   ///< [in] VlMgr
    const std::vector<const VlStmt*>& stmt_list
                        ///< [in] オブジェクト
  );

  /// @brief delay/event/repeat control の内容を出力する関数
  void
  put_control(
    const char* label,   ///< [in] ラベル
    const VlMgr& mgr,	 ///< [in] VlMgr
    const VlControl* obj ///< [in] オブジェクト
  );

  /// @brief ブロック内の stmt リストを出力する関数
  void
  put_child_stmt_list(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlStmt* stmt ///< [in] オブジェクト
  );

  /// @brief 引数のリストを出力する関数
  void
  put_argument_list(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlStmt* stmt ///< [in] オブジェクト
  );

  /// @brief 式の内容を出力する関数
  void
  put_expr(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlExpr* expr ///< [in] オブジェクト
  );

  /// @brief 左辺式の内容を出力する関数
  void
  put_lhs(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlExpr* expr ///< [in] オブジェクト
  );

  /// @brief プライマリ式を出力する関数
  void
  put_primary(
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlExpr* expr ///< [in] オブジェクト
  );

  /// @brief 遅延値を出力する関数
  void
  put_delay(
    const char* label,   ///< [in] ラベル
    const VlMgr& mgr,	 ///< [in] VlMgr
    const VlDelay* delay ///< [in] オブジェクト
  );

  /// @brief range の内容を出力する関数
  void
  put_range(
    const char* label, ///< [in] ラベル
    const VlMgr& mgr,  ///< [in] VlMgr
    const VlRange* obj ///< [in] オブジェクト
  );


  //////////////////////////////////////////////////////////////////////
  // その他の型
  //////////////////////////////////////////////////////////////////////

  /// @brief VlValue の情報を出力する．
  void
  put(
    const char* label,   ///< [in] ラベル
    const VlValue& value ///< [in] 値
  );

  /// @brief FileLoc の情報を出力する．
  void
  put(
    const char* label,      ///< [in] ラベル
    const FileLoc& file_loc ///< [in] ファイルの位置情報
  );

  /// @brief FileRegion の情報をに出力する．
  void
  put(
    const char* label,            ///< [in] ラベル
    const FileRegion& file_region ///< [in] ファイル領域の情報
  );

  /// @brief direction を出力する関数
  void
  put(
    const char* label, ///< [in] ラベル
    VpiDir direction   ///< [in] 向き
  );

  /// @brief case type を出力する関数
  void
  put(
    const char* label,    ///< [in] ラベル
    VpiCaseType case_type ///< [in] CASE文の型
  );

  /// @brief const type を出力する関数
  void
  put(
    const char* label,      ///< [in] ラベル
    VpiConstType const_type ///< [in] 定数の型
  );

  /// @brief prim type を出力する関数
  void
  put(
    const char* label,    ///< [in] ラベル
    VpiPrimType prim_type ///< [in] プリミティブの型
  );

  /// @brief func type の出力
  void
  put(
    const char* label,    ///< [in] ラベル
    VpiFuncType func_type ///< [in] 関数の種類
  );

  /// @brief op type の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiOpType op_type  ///< [in] 演算子の種類
  );

  /// @brief strength を出力する関数
  void
  put(
    const char* label,      ///< [in] ラベル
    VpiStrength str,        ///< [in] 信号強度
    bool put_on_null = true ///< [in] true の時, 空でも出力する．
  );

  /// @brief unconnected drive の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiUnconnDrive ud  ///< [in] unconnected drive
  );

  /// @brief default delay mode の出力
  void
  put(
    const char* label,         ///< [in] ラベル
    VpiDefDelayMode delay_mode ///< [in] default delay mode
  );

  /// @brief ネット型の出力
  void
  put(
    const char* label,  ///< [in] ラベル
    VpiNetType net_type ///< [in] ネット型
  );

  /// @brief ブール値の出力
  void
  put(
    const char* label, ///< [in] ラベル
    bool b             ///< [in] ブール値
  );

  /// @brief 整数値の出力
  void
  put(
    const char* label, ///< [in] ラベル
    int d              ///< [in] 整数値
  );

  /// @brief 整数値の出力
  void
  put(
    const char* label, ///< [in] ラベル
    unsigned int d     ///< [in] 整数値
  );

  /// @brief 整数値の出力
  void
  put(
    const char* label, ///< [in] ラベル
    SizeType d         ///< [in] 整数値
  );

  /// @brief スカラー値の出力
  void
  put(
    const char* label,     ///< [in] ラベル
    const VlScalarVal& val ///< [in] スカラー値
  );

  /// @brief time値の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VlTime val         ///< [in] time値
  );

  /// @brief 実数値の出力
  void
  put(
    const char* label, ///< [in] ラベル
    double val         ///< [in] 値
  );

  /// @brief ビットベクタ値の出力
  void
  put(
    const char* label,   ///< [in] ラベル
    const BitVector& val ///< [in] 値
  );

  /// @brief 文字列の出力
  void
  put(
    const char* label,     ///< [in] ラベル
    const std::string& str ///< [in] 文字列
  );

  /// @brief 文字列の出力
  void
  put(
    const char* label, ///< [in] ラベル
    const char* str    ///< [in] 文字列
  );

  /// @brief 開始マーカを出力する．
  void
  begin_marker(
    const char* label,       ///< [in] ラベル
    const std::string& type, ///< [in] 属性
    bool need_cr = true      ///< [in] マーカ出力直後に改行するときに true とするフラグ
  );

  /// @brief 終了マーカを出力する．
  void
  end_marker(
    const char* label ///< [in] ラベル
  );

  // 改行が必要なら改行する．
  void
  cr(
    bool force = false
  );

  /// @brief file_loc の親のファイル情報を出力する．
  void
  put_parent_file(
    const FileLoc& file_loc ///< [in] ファイル位置の情報
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  std::ostream& mStream;

  // マーカ出力後に改行していないことを示すフラグ
  std::vector<bool> mDoCR;

  // インデント量
  int mIndent;

  // ファイル位置情報の出力フラグ
  bool mFileLocMode;

  // nullptr ポインタ出力抑止フラグ
  bool mNullptrSuppressMode;

  // bit 展開フラグ
  bool mBitExpandMode;

};


//////////////////////////////////////////////////////////////////////
/// マーカを出力するクラス
//////////////////////////////////////////////////////////////////////
class VlDumpHeader
{
public:

  /// @brief コンストラクタ
  VlDumpHeader(
    VlDumperImpl* dumper, ///< [in] 出力先
    const char* label,    ///< [in] ラベル
    const char* type,     ///< [in] 属性
    bool need_cr = true   ///< [in] true の時マーカ出力直後に改行する
  );

  /// @brief デストラクタ
  ~VlDumpHeader();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  VlDumperImpl* mDumper;

  // マーカー名
  const char* mName;

};

END_NAMESPACE_YM_VERILOG

#endif // VLDUMPERIMPL_H
