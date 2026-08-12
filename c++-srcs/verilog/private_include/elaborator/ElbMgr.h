#ifndef ELBMGR_H
#define ELBMGR_H

/// @file ElbMgr.h
/// @brief ElbMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/Ast.h"
#include "ym/vl/VlFwd.h"
#include "ym/clib.h"

#include "elaborator/ElbFactory.h"
#include "elaborator/ObjDict.h"
#include "elaborator/ModDefDict.h"
#include "elaborator/TagDict.h"
#include "elaborator/AttrHash.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbMgr ElbMgr.h "ElbMgr.h"
/// @brief ElbMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class ElbMgr
{
public:

  /// @brief コンストラクタ
  ElbMgr();

  /// @brief デストラクタ
  ~ElbMgr();


public:

  /// @brief 内容をクリアする．
  void
  clear();


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を検索する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief UDP 定義のリストを返す．
  const std::vector<const VlUdpDefn*>&
  udp_list() const;

  /// @brief 名前から UDP 定義を取出す．
  /// @return name という名の UDP を返す．
  /// @return なければ nullptr を返す．
  const VlUdpDefn*
  find_udp(
    const std::string& name ///< [in] 名前
  ) const;

  /// @brief topmodule のリストを返す．
  const std::vector<const VlModule*>&
  topmodule_list() const;

  /// @brief 名前から UserSystf を取出す．
  /// @return name という名のユーザー定義関数を返す．
  /// @return なければ nullptr を返す．
  const ElbUserSystf*
  find_user_systf(
    const std::string& name ///< [in] 名前
  ) const;

  /// @brief スコープに属する internal scope のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlScope*>
  find_internalscope_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_internalscope_list(parent);
  }

  /// @brief スコープとタグから宣言要素を取り出す．
  /// @return 結果のリストを返す．
  ///
  /// parent のスコープ内の tag というタグを持つ要素のリストを返す．
  std::vector<const VlDecl*>
  find_decl_list(
    const VlScope* parent, ///< [in] 検索対象のスコープ
    int tag                ///< [in] タグ
  ) const
  {
    return mTagDict.find_decl_list(parent, tag);
  }

  /// @brief スコープとタグから宣言要素の配列を取り出す．
  /// @retrun 結果のリストを返す．
  ///
  /// parent というスコープ内の tag というタグを持つ要素のリストを返す．
  std::vector<const VlDeclArray*>
  find_declarray_list(
    const VlScope* parent, ///< [in] 検索対象のスコープ
    int tag                ///< [in] タグ
  ) const
  {
    if ( tag == vpiVariables ) {
      // ちょっと汚い補正
      tag += 100;
    }
    return mTagDict.find_declarray_list(parent, tag);
  }

  /// @brief スコープに属する defparam のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlDefParam*>
  find_defparam_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_defparam_list(parent);
  }

  /// @brief スコープに属する param assign のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlParamAssign*>
  find_paramassign_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_paramassign_list(parent);
  }

  /// @brief スコープに属する module のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlModule*>
  find_module_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_module_list(parent);
  }

  /// @brief スコープに属する module arrayのリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlModuleArray*>
  find_modulearray_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_modulearray_list(parent);
  }

  /// @brief スコープに属する primitive のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlPrimitive*>
  find_primitive_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_primitive_list(parent);
  }

  /// @brief スコープに属する primitive array のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlPrimArray*>
  find_primarray_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_primarray_list(parent);
  }

  /// @brief スコープに属する continuous assignment のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlContAssign*>
  find_contassign_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_contassign_list(parent);
  }

  /// @brief スコープに属するタスクのリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlTaskFunc*>
  find_task_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_task_list(parent);
  }

  /// @brief スコープに属する関数のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlTaskFunc*>
  find_function_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_function_list(parent);
  }

  /// @brief スコープに属する process のリストを取り出す．
  /// @return 結果のリストを返す．
  std::vector<const VlProcess*>
  find_process_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const
  {
    return mTagDict.find_process_list(parent);
  }

  /// @brief スコープと名前から名前付き要素を取り出す．
  /// @return parent というスコープ内の name という要素を返す．
  /// @return なければ nullptr を返す．
  ObjHandle*
  find_obj(
    const VlScope* parent,  ///< [in] 検索対象のスコープ
    const std::string& name ///< [in] 名前
  ) const;

  /// @brief スコープと名前からスコープを取り出す．
  /// @return parent というスコープ内の name というスコープを返す．
  /// @return なければ nullptr を返す．
  const VlScope*
  find_namedobj(
    const VlScope* parent,  ///< [in] 検索対象のスコープ
    const std::string& name ///< [in] 名前
  ) const;

  /// @brief スコープと階層名から要素を取り出す．
  /// @return 見付かったオブジェクトを返す．
  /// 見付からなかったら nullptr を返す．
  ObjHandle*
  find_obj_up(
    const VlScope* base_scope,     ///< [in] 起点となるスコープ
    const AstHierNamedBase* ast_obj, ///< [in] 階層名付きのオブジェクト
    const VlScope* ulimit          ///< [in] 探索する名前空間の上限
  );

  /// @brief 名前からモジュール定義を取り出す．
  /// @return name という名のモジュール定義
  /// @return なければ nullptr を返す．
  const AstModule*
  find_moduledef(
    const std::string& name ///< [in] 名前
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 検索の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief base_scope を起点として (nb, "") という名前のスコープを探す．
  /// なければ親のスコープに対して同様の探索を繰り返す．
  const VlScope*
  find_scope_up(
    const VlScope* base_scope,
    const AstHierNamedBase* ast_obj,
    const VlScope* ulimit
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief システムタスク/システム関数を登録する．
  /// @param[in] systf 登録するシステムタスク/システム関数
  void
  reg_user_systf(
    const ElbUserSystf* systf
  );

  /// @brief internal scope を登録する．
  /// @param[in] obj 登録するオブジェクト
  void
  reg_internalscope(
    const VlScope* obj
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の検索/登録
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性リストを得る．
  /// @param[in] obj 対象のオブジェクト
  ///
  /// 対象が存在しない場合には空リストを返す．
  std::vector<const VlAttribute*>
  find_attr(
    const VlObj* obj
  ) const
  {
    if ( mAttrHash.count(obj) > 0 ) {
      return mAttrHash.at(obj);
    }
    return {};
  }

  /// @brief 属性リストを登録する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] attr_list 属性リスト
  void
  reg_attr(
    const VlObj* obj,
    const std::vector<const VlAttribute*>& attr_list
  );


public:
  //////////////////////////////////////////////////////////////////////
  // インスタンス生成関係のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  const VlScope*
  new_Toplevel();

  /// @brief ステートメントブロックのスコープを生成する．
  const VlScope*
  new_StmtBlockScope(
    const VlScope* parent, ///< [in] 親のスコープ環境
    const AstStmt* ast_stmt  ///< [in] 対応するパース木の要素
  );

  /// @brief generate block を生成する．
  const VlScope*
  new_GenBlock(
    const VlScope* parent, ///< [in] 親のスコープ環境
    const AstItem* ast_item  ///< [in] 対応するパース木の要素
  );

  /// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
  ElbGfRoot*
  new_GfRoot(
    const VlScope* parent,
    const AstItem* ast_item
  );

  /// @brief generate for block を生成する．
  const VlScope*
  new_GfBlock(
    const VlScope* parent, ///< [in] 親のスコープ環境
    const AstItem* ast_item, ///< [in] 対応するパース木の要素
    int gvi                ///< [in] 対応する genvar の値
  );

  /// @brief UDP定義を生成する．
  ElbUdpDefn*
  new_UdpDefn(
    const AstUdp* ast_udp, ///< [in] パース木の UDP 定義
    bool is_protected    ///< [in] プロテクト属性
  );

  /// @brief module を生成する．
  ElbModule*
  new_Module(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstModule* ast_module, ///< [in] モジュールテンプレート
    const AstItem* ast_head,     ///< [in] パース木のヘッダ定義
    const AstInst* ast_inst      ///< [in] パース木のインスタンス定義
  );

  /// @brief module array を生成する．
  ElbModuleArray*
  new_ModuleArray(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstModule* ast_module, ///< [in] モジュールテンプレート
    const AstItem* ast_head,     ///< [in] パース木のヘッダ定義
    const AstInst* ast_inst,     ///< [in] パース木のインスタンス定義
    const AstRange* ast_range,   ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  );

  /// @brief module IO ヘッダを生成する．
  ElbIOHead*
  new_IOHead(
    const VlModule* module,   ///< [in] 親のモジュール
    const AstIOHead* ast_header ///< [in] パース木のIO宣言ヘッダ
  );

  /// @brief タスク/関数用の IO ヘッダを生成する．
  ElbIOHead*
  new_IOHead(
    const VlTaskFunc* taskfunc, ///< [in] 親のタスク/関数
    const AstIOHead* ast_header   ///< [in] パース木のIO宣言ヘッダ
  );

  /// @brief 宣言要素のヘッダを生成する．
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstDeclHead* ast_head, ///< [in] パース木の宣言ヘッダ
    bool has_delay = false     ///< [in] 遅延値を持つとき true
  );

  /// @brief 宣言要素のヘッダを生成する．
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstDeclHead* ast_head, ///< [in] パース木の宣言ヘッダ
    const AstRange* ast_range,   ///< [in] パース木の範囲定義
    const RangeVal& range,     ///< [in] 範囲の値
    bool has_delay = false     ///< [in] 遅延値を持つとき true
  );

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstIOHead* ast_head, ///< [in] パース木のIO宣言ヘッダ
    VpiAuxType aux_type      ///< [in] 補助的なデータ型
  );

  /// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstIOHead* ast_head, ///< [in] パース木のIO宣言ヘッダ
    VpiAuxType aux_type,     ///< [in] 補助的なデータ型
    const AstRange* ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range    ///< [in] 範囲の値
  );

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_item  ///< [in] パース木の関数定義
  );

  /// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
  ElbDeclHead*
  new_DeclHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem* ast_item,   ///< [in] パース木の関数定義
    const AstRange* ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range    ///< [in] 範囲の値
  );

  /// @brief 宣言要素を生成する．
  ElbDecl*
  new_Decl(
    int tag,                     ///< [in] タグ
    ElbDeclHead* head,           ///< [in] ヘッダ
    const AstNamedBase* ast_item,  ///< [in] パース木の宣言要素
    const VlExpr* init = nullptr ///< [in] 初期値
  );

  /// @brief 暗黙のネットを生成する．
  ElbDecl*
  new_ImpNet(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstExpr* ast_expr, ///< [in] パース木のプライマリ式
    VpiNetType net_type    ///< [in] ネットタイプ
  );

  /// @brief 宣言要素の配列を生成する．
  const VlDeclArray*
  new_DeclArray(
    int tag,                                  ///< [in] タグ
    ElbDeclHead* head,                        ///< [in] ヘッダ
    const AstNamedBase* ast_item,               ///< [in] パース木の宣言要素
    const std::vector<ElbRangeSrc>& range_src ///< [in] 範囲の配列
  );

  /// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
  ElbParamHead*
  new_ParamHead(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstDeclHead* ast_head ///< [in] パース木の宣言ヘッダ
  );

  /// @brief parameter 宣言のヘッダを生成する(範囲指定あり)．
  ElbParamHead*
  new_ParamHead(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstDeclHead* ast_head, ///< [in] パース木の宣言ヘッダ
    const AstRange* ast_range,   ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  );

  /// @brief parameter 宣言を生成する．
  ElbParameter*
  new_Parameter(
    ElbParamHead* head,         ///< [in] ヘッダ
    const AstNamedBase* ast_item, ///< [in] パース木の宣言要素
    bool is_local = false       ///< [in] localparam の時 true
  );

  /// @brief genvar を生成する．
  ElbGenvar*
  new_Genvar(
    const VlScope* parent,     ///< [in] 親のスコープ環境
    const AstDeclItem* ast_item, ///< [in] 対応するパース木の要素
    int val                    ///< [in] 初期値
  );

  /// @brief continuous assignment のヘッダを生成する．
  ///
  /// 遅延を持たないときは nullptr を与える．
  ElbCaHead*
  new_CaHead(
    const VlModule* module,        ///< [in] 親のモジュール
    const AstItem* ast_head,         ///< [in] パース木のヘッダ定義
    const VlDelay* delay = nullptr ///< [in] 遅延値
  );

  /// @brief continuous assignment を生成する．
  const VlContAssign*
  new_ContAssign(
    ElbCaHead* head,      ///< [in] ヘッダ
    const AstBase* ast_obj, ///< [in] 対応するパース木中の定義要素
    const VlExpr* lhs,    ///< [in] 左辺式
    const VlExpr* rhs     ///< [in] 右辺式
  );

  /// @brief net 宣言中の continuous assignment を生成する．
  const VlContAssign*
  new_ContAssign(
    const VlModule* module, ///< [in] 親のモジュール
    const AstBase* ast_obj,   ///< [in] パース木の定義要素
    const VlExpr* lhs,      ///< [in] 左辺式
    const VlExpr* rhs       ///< [in] 右辺式
  );

  /// @brief パラメータ割り当て文を生成する．
  const VlParamAssign*
  new_ParamAssign(
    const VlModule* module,  ///< [in] 親のモジュール
    const AstBase* ast_obj,    ///< [in] パース木の定義要素
    ElbParameter* param,     ///< [in] 対象の parameter
    const AstExpr* rhs_expr,  ///< [in] 割り当て式の右辺
    const VlValue& rhs_value ///< [in] 右辺の値
  );

  /// @brief 名前によるパラメータ割り当て文を生成する．
  const VlParamAssign*
  new_NamedParamAssign(
    const VlModule* module,  ///< [in] 親のモジュール
    const AstBase* ast_obj,    ///< [in] パース木の定義要素
    ElbParameter* param,     ///< [in] 対象の parameter
    const AstExpr* rhs_expr,  ///< [in] 割り当て式の右辺
    const VlValue& rhs_value ///< [in] 右辺の値
  );

  /// @brief defparam 文を生成する．
  const VlDefParam*
  new_DefParam(
    const VlModule* module,        ///< [in] 親のモジュール
    const AstItem* ast_header,       ///< [in] パース木の defparam ヘッダ
    const AstDefParam* ast_defparam, ///< [in] パース木の defparam 定義
    ElbParameter* param,           ///< [in] 対象の parameter
    const AstExpr* rhs_expr,        ///< [in] 割り当て式の右辺式
    const VlValue& rhs_value       ///< [in] 右辺の値
  );

  /// @brief ゲートプリミティブのヘッダを生成する．
  ElbPrimHead*
  new_PrimHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem* ast_header, ///< [in] パース木の定義
    bool has_delay           ///< [in] 遅延値を持つとき true
  );

  /// @brief UDPプリミティブのヘッダを生成する．
  ElbPrimHead*
  new_UdpHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem* ast_header, ///< [in] パース木の定義
    const VlUdpDefn* udp,    ///< [in] 定義
    bool has_delay           ///< [in] 遅延値を持つとき true
  );

  /// @brief セルプリミティブのヘッダを生成する．
  ElbPrimHead*
  new_CellHead(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem* ast_header, ///< [in] パース木の定義
    const ClibCell& cell     ///< [in] セル
  );

  /// @brief プリミティブインスタンスを生成する．
  ElbPrimitive*
  new_Primitive(
    ElbPrimHead* head,    ///< [in] ヘッダ
    const AstInst* ast_inst ///< [in] インスタンス定義
  );

  /// @brief プリミティブ配列インスタンスを生成する．
  ElbPrimArray*
  new_PrimitiveArray(
    ElbPrimHead* head,       ///< [in] ヘッダ
    const AstInst* ast_inst,   ///< [in] インスタンス定義
    const AstRange* ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range    ///< [in] 範囲の値
  );

  /// @brief セルプリミティブインスタンスを生成する．
  ElbPrimitive*
  new_CellPrimitive(
    ElbPrimHead* head,    ///< [in] ヘッダ
    const ClibCell& cell, ///< [in] セル
    const AstInst* ast_inst ///< [in] インスタンス定義
  );

  /// @brief セルプリミティブ配列インスタンスを生成する．
  ElbPrimArray*
  new_CellPrimitiveArray(
    ElbPrimHead* head,       ///< [in] ヘッダ
    const ClibCell& cell,    ///< [in] セル
    const AstInst* ast_inst,   ///< [in] インスタンス定義
    const AstRange* ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range    ///< [in] 範囲の値
  );

  /// @brief function を生成する．
  ElbTaskFunc*
  new_Function(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_item, ///< [in] パース木の定義
    bool const_func        ///< [in] 定数関数フラグ
  );

  /// @brief function を生成する．
  ElbTaskFunc*
  new_Function(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem* ast_item,   ///< [in] パース木の定義
    const AstRange* ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range,   ///< [in] 範囲の値
    bool const_func          ///< [in] 定数関数フラグ
  );

  /// @brief task を生成する．
  ElbTaskFunc*
  new_Task(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_item  ///< [in] パース木の定義
  );

  /// @brief プロセス文を生成する．
  ElbProcess*
  new_Process(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_item  ///< [in] パース木の要素定義
  );

  /// @brief 代入文を生成する．
  const VlStmt*
  new_Assignment(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* lhs,        ///< [in] 左辺の式
    const VlExpr* rhs,        ///< [in] 右辺の式
    bool block,               ///< [in] ブロッキング代入の時 true
    const VlControl* control  ///< [in] コントロール
    = nullptr
  );

  /// @brief assign ステートメントを生成する．
  const VlStmt*
  new_AssignStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* lhs,        ///< [in] 左辺の式
    const VlExpr* rhs         ///< [in] 右辺の式
  );

  /// @brief deassign ステートメントを生成する．
  const VlStmt*
  new_DeassignStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* lhs         ///< [in] 左辺の式
  );

  /// @brief force ステートメントを生成する．
  const VlStmt*
  new_ForceStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* lhs,        ///< [in] 左辺の式
    const VlExpr* rhs         ///< [in] 右辺の式
  );

  /// @brief release ステートメントを生成する．
  const VlStmt*
  new_ReleaseStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* lhs         ///< [in] 左辺の式
  );

  /// @brief begin ブロックを生成する．
  const VlStmt*
  new_Begin(
    const VlScope* parent,                      ///< [in] 親のスコープ
    const VlProcess* process,                   ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,                      ///< [in] パース木のステートメント定義
    const std::vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  );

  /// @brief fork ブロックを生成する．
  const VlStmt*
  new_Fork(
    const VlScope* parent,                      ///< [in] 親のスコープ
    const VlProcess* process,                   ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,                      ///< [in] パース木のステートメント定義
    const std::vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  );

  /// @brief 名前付き begin ブロックを生成する．
  const VlStmt*
  new_NamedBegin(
    const VlScope* block,                       ///< [in] 自分自身に対応するスコープ
    const VlProcess* process,                   ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,                      ///< [in] パース木のステートメント定義
    const std::vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  );


  /// @brief 名前付き fork ブロックを生成する．
  const VlStmt*
  new_NamedFork(
    const VlScope* block,                       ///< [in] 自分自身に対応するスコープ
    const VlProcess* process,                   ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,                      ///< [in] パース木のステートメント定義
    const std::vector<const VlStmt*>& stmt_list ///< [in] 子のステートメントリスト
  );

  /// @brief while 文を生成する．
  const VlStmt*
  new_WhileStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

  /// @brief repeat 文を生成する．
  const VlStmt*
  new_RepeatStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

  /// @brief wait 文を生成する．
  const VlStmt*
  new_WaitStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

  /// @brief for 文を生成する．
  const VlStmt*
  new_ForStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* init_stmt,  ///< [in] 初期化式
    const VlStmt* inc_stmt,   ///< [in] 増加式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

  /// @brief forever 文を生成する．
  const VlStmt*
  new_ForeverStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

  /// @brief if 文を生成する．
  const VlStmt*
  new_IfStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt,       ///< [in] 本体のステートメント
    const VlStmt* else_stmt   ///< [in] else節のステートメント(or nullptr)
  );

  /// @brief case 文を生成する．
  const VlStmt*
  new_CaseStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlExpr* expr,       ///< [in] 条件式
    const std::vector<const VlCaseItem*>& caseitem_list ///< [in] caseitem のリスト
  );

  /// @brief caseitem を生成する．
  const VlCaseItem*
  new_CaseItem(
    const AstCaseItem* ast_item,               ///< [in] パース木の caseitem 要素
    const std::vector<ElbExpr*>& label_list, ///< [in] ラベルを表す式のリスト
    const VlStmt* body                       ///< [in] 本体のステートメント
  );

  /// @brief イベント文を生成する．
  const VlStmt*
  new_EventStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    ElbExpr* named_event      ///< [in] 対象のイベント
  );

  /// @brief nullptr ステートメントを生成する．
  const VlStmt*
  new_NullStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt     ///< [in] パース木のステートメント定義
  );

  /// @brief タスクコール文を生成する．
  const VlStmt*
  new_TaskCall(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,              ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,                 ///< [in] パース木のステートメント定義
    const VlTaskFunc* task,                ///< [in] 対象のタスク
    const std::vector<ElbExpr*>& arg_array ///< [in] 引数の配列
  );

  /// @brief システムタスクコール文を生成する．
  const VlStmt*
  new_SysTaskCall(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,              ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,                 ///< [in] パース木のステートメント定義
    const VlUserSystf* user_systf,         ///< [in] システムタスク
    const std::vector<ElbExpr*>& arg_array ///< [in] 引数の配列
  );

  /// @brief disable 文を生成する．
  const VlStmt*
  new_DisableStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlScope* target     ///< [in] 対象のスコープ
  );

  /// @brief コントロール文を生成する．
  const VlStmt*
  new_CtrlStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt* ast_stmt,    ///< [in] パース木のステートメント定義
    const VlControl* control, ///< [in] コントロール
    const VlStmt* stmt        ///< [in] 本体のステートメント
  );

  /// @brief 遅延コントロールを生成する．
  const VlControl*
  new_DelayControl(
    const AstControl* ast_control, ///< [in] パース木の定義要素
    ElbExpr* delay               ///< [in] 遅延式
  );

  /// @brief イベントコントロールを生成する．
  const VlControl*
  new_EventControl(
    const AstControl* ast_control,            ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& event_list ///< [in] イベントリストを表す配列
  );

  /// @brief リピートコントロールを生成する．
  const VlControl*
  new_RepeatControl(
    const AstControl* ast_control,            ///< [in] パース木の定義要素
    ElbExpr* rep,                           ///< [in] 繰り返し式
    const std::vector<ElbExpr*>& event_list ///< [in] イベントリストを表す配列
  );

  /// @brief 単項演算子を生成する．
  ElbExpr*
  new_UnaryOp(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,     ///< [in] 演算子のタイプ
    ElbExpr* opr1          ///< [in] オペランド
  );

  /// @brief 2項演算子を生成する．
  ElbExpr*
  new_BinaryOp(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,     ///< [in] 演算子のタイプ
    ElbExpr* opr1,         ///< [in] オペランド
    ElbExpr* opr2          ///< [in] オペランド
  );

  /// @brief 3項演算子を生成する．
  ElbExpr*
  new_TernaryOp(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    VpiOpType op_type,     ///< [in] 演算子のタイプ
    ElbExpr* opr1,         ///< [in] 第１オペランド
    ElbExpr* opr2,         ///< [in] 第２オペランド
    ElbExpr* opr3          ///< [in] 第３オペランド
  );

  /// @brief 連結演算子を生成する．
  ElbExpr*
  new_ConcatOp(
    const AstExpr* ast_expr,                ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& opr_list ///< [in] オペランドのリスト
  );

  /// @brief 反復連結演算子を生成する．
  ElbExpr*
  new_MultiConcatOp(
    const AstExpr* ast_expr,                ///< [in] パース木の定義要素
    SizeType rep_num,                     ///< [in] 繰り返し数を表す式
    ElbExpr* rep_expr,                    ///< [in] 繰り返し数
    const std::vector<ElbExpr*>& opr_list ///< [in] オペランドのリスト
  );

  /// @brief プライマリ式を生成する．
  ElbExpr*
  new_Primary(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj      ///< [in] 本体のオブジェクト
  );

  /// @brief プライマリ式を生成する(net decl の初期値用)．
  ElbExpr*
  new_Primary(
    const AstDeclItem* ast_item, ///< [in] パース木の定義要素
    const VlDecl* obj          ///< [in] 本体のオブジェクト
  );

  /// @brief プライマリ式を生成する．
  ElbExpr*
  new_Primary(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    ElbParameter* obj      ///< [in] 本体のオブジェクト
  );

  /// @brief プライマリ式を生成する(配列要素版)．
  ElbExpr*
  new_Primary(
    const AstExpr* ast_expr,                  ///< [in] パース木の定義要素
    const VlDeclArray* obj,                 ///< [in] 本体のオブジェクト
    const std::vector<ElbExpr*>& index_list ///< [in] インデックスのリスト
  );

  /// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
  ElbExpr*
  new_Primary(
    const AstExpr* ast_expr,  ///< [in] パース木の定義要素
    const VlDeclArray* obj, ///< [in] 本体のオブジェクト
    int offset              ///< [in] オフセット
  );

  /// @brief 固定ビット選択式を生成する．
  ElbExpr*
  new_BitSelect(
    const AstExpr* ast_expr,   ///< [in] パース木の定義要素
    ElbExpr* expr,           ///< [in] 本体のオブジェクト
    const AstExpr* bit_index, ///< [in] ビット選択式
    int bit_index_val        ///< [in] ビット選択式の値
  );

  /// @brief 固定ビット選択式を生成する．
  ElbExpr*
  new_BitSelect(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    ElbExpr* expr,         ///< [in] 本体の式
    int bit_index_val      ///< [in] ビット選択式の値
  );

  /// @brief 可変ビット選択式を生成する．
  ElbExpr*
  new_BitSelect(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    ElbExpr* expr,         ///< [in] 本体のオブジェクト
    ElbExpr* bit_index     ///< [in] ビット選択式
  );

  /// @brief 固定部分選択式を生成する．
  ElbExpr*
  new_PartSelect(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    ElbExpr* obj,          ///< [in] 本体のオブジェクト
    const AstExpr* index1,  ///< [in] インデックス1の式
    const AstExpr* index2,  ///< [in] インデックス2の式
    int index1_val,        ///< [in] インデックス1の値
    int index2_val         ///< [in] インデックス2の値
  );

  /// @brief 固定部分選択式を生成する．
  ElbExpr*
  new_PartSelect(
    const AstExpr* ast_expr, ///< [in] パース木の定義要素
    ElbExpr* expr,         ///< [in] 本体の式
    int index1,            ///< [in] インデックス1の値
    int index2             ///< [in] インデックス2の値
  );

  /// @brief 可変部分選択式を生成する．
  ElbExpr*
  new_PlusPartSelect(
    const AstExpr* ast_expr,    ///< [in] パース木の定義要素
    ElbExpr* obj,             ///< [in] 本体のオブジェクト
    ElbExpr* base,            ///< [in] 範囲のベースアドレスを表す式
    const AstExpr* range_expr, ///< [in] 範囲を表す式
    int range_val             ///< [in] 範囲の値
  );

  /// @brief 可変部分選択式を生成する．
  ElbExpr*
  new_MinusPartSelect(
    const AstExpr* ast_expr,    ///< [in] パース木の定義要素
    ElbExpr* obj,             ///< [in] 本体のオブジェクト
    ElbExpr* base,            ///< [in] 範囲のベースアドレスを表す式
    const AstExpr* range_expr, ///< [in] 範囲を表す式
    int range_val             ///< [in] 範囲の値
  );

  /// @brief 定数式を生成する．
  ElbExpr*
  new_Constant(
    const AstExpr* ast_expr ///< [in] パース木の定義要素
  );

  /// @brief genvar 起因の定数式を生成する．
  ElbExpr*
  new_GenvarConstant(
    const AstExpr* ast_primary, ///< [in] パース木の定義要素
    int val                   ///< [in] 値
  );

  /// @brief 関数呼び出し式を生成する．
  ElbExpr*
  new_FuncCall(
    const AstExpr* ast_expr,                ///< [in] パース木の定義要素
    const VlTaskFunc* func,               ///< [in] 関数
    const std::vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  );

  /// @brief システム関数呼び出し式を生成する．
  ElbExpr*
  new_SysFuncCall(
    const AstExpr* ast_expr,                ///< [in] パース木の定義要素
    const VlUserSystf* user_systf,        ///< [in] システム関数
    const std::vector<ElbExpr*>& arg_list ///< [in] 引数のリスト
  );

  /// @brief システム関数/システムタスクの引数を生成する．
  ElbExpr*
  new_ArgHandle(
    const AstExpr* ast_expr, ///< [in] パース木中で参照している要素
    const VlScope* arg     ///< [in] 引数本体
  );

  /// @brief システム関数/システムタスクの引数を生成する．
  ElbExpr*
  new_ArgHandle(
    const AstExpr* ast_expr, ///< [in] パース木中で参照している要素
    const VlPrimitive* arg ///< [in] 引数本体
  );

  /// @brief システム関数/システムタスクの引数を生成する．
  ElbExpr*
  new_ArgHandle(
    const AstExpr* ast_expr, ///< [in] パース木中で参照している要素
    const VlDeclArray* arg ///< [in] 引数本体
  );

  /// @brief 連結演算子の左辺式を生成する．
  ElbExpr*
  new_Lhs(
    const AstExpr* ast_expr,                      ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& opr_array,     ///< [in] オペランドを格納する配列
    const std::vector<ElbExpr*>& lhs_elem_array ///< [in] 左辺の要素の配列
  );

  /// @brief 遅延値を生成する．
  const VlDelay*
  new_Delay(
    const AstBase* ast_obj,                  ///< [in] パース木の定義要素
    const std::vector<ElbExpr*>& expr_list ///< [in] 式の配列
  );

  /// @brief attribute instance のリストを生成する．
  const VlAttribute*
  new_Attribute(
    const AstAttrSpec* ast_attr, ///< [in] パース木の定義要素
    const VlExpr* expr,        ///< [in] 値
    bool def = false           ///< [in] 定義側の属性の時 true
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Elbオブジェクト用のファクトリを返す．
  ElbFactory&
  factory()
  {
    return *mFactory;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オブジェクト生成用のファクトリクラス
  std::unique_ptr<ElbFactory> mFactory;

  // UDP のリスト
  std::vector<const VlUdpDefn*> mUdpList;

  // UDP の辞書
  std::unordered_map<std::string, const VlUdpDefn*> mUdpHash;

  // topmodule のリスト
  std::vector<const VlModule*> mTopmoduleList;

  // 名前をキーにしたオブジェクトの辞書
  ObjDict mObjDict;

  // モジュール定義名の辞書
  ModDefDict mModuleDefDict;

  // UserSystf の辞書
  std::unordered_map<std::string, const ElbUserSystf*> mSystfHash;

  // ヘッダのリスト
  std::vector<const ElbHead*> mHeadList;

  // 全てのオブジェクトのリスト
  std::vector<const VlObj*> mObjList;

  // タグをキーにした各スコープごとのオブジェクトのリストの辞書
  TagDict mTagDict;

  // 属性リストの辞書
  std::unordered_map<const VlObj*, std::vector<const VlAttribute*>> mAttrHash;

  // トップレベルスコープ
  const VlScope* mTopLevel;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBMGR_H
