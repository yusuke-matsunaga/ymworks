
/// @file ElbMgr.cc
/// @brief ElbMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "ym/vl/VlAttribute.h"
#include "ym/vl/VlContAssign.h"
#include "ym/vl/VlControl.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlDelay.h"
#include "ym/vl/VlParamAssign.h"
#include "ym/vl/VlPrimitive.h"
#include "ym/vl/VlProcess.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlUserSystf.h"
#include "ym/vl/VlUdp.h"
#include "ym/vl/AstUdp.h"
#include "ym/vl/AstMisc.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbUdp.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbModuleArray.h"
#include "elaborator/ElbIOHead.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbCaHead.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbTaskFunc.h"
#include "elaborator/ElbUserSystf.h"
#include "elaborator/ElbProcess.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/RangeVal.h"


BEGIN_NAMESPACE_YM_VERILOG

#define DOUT std::cerr

const int debug_none       = 0x00000000;
const int debug_find_scope = 0x00000010;
const int debug_all        = 0xFFFFFFFF;

#if 1
const int debug = debug_none;
#else
const int debug = debug_all;
#endif

// @brief コンストラクタ
ElbMgr::ElbMgr() :
  mFactory{ElbFactory::new_obj()}
{
}

// @brief デストラクタ
ElbMgr::~ElbMgr()
{
  clear();
}

// @brief 内容をクリアする．
void
ElbMgr::clear()
{
  for ( auto head: mHeadList ) {
    delete head;
  }
  mHeadList.clear();

  for ( auto obj: mObjList ) {
    delete obj;
  }
  mObjList.clear();

  mUdpList.clear();
  mUdpHash.clear();
  mTopmoduleList.clear();
  mSystfHash.clear();
  mTagDict.clear();
  mAttrHash.clear();
  mTopLevel = nullptr;
}

// @brief UDP 定義のリストを返す．
const std::vector<const VlUdpDefn*>&
ElbMgr::udp_list() const
{
  return mUdpList;
}

// @brief 名前から UDP 定義を取出す．
const VlUdpDefn*
ElbMgr::find_udp(
  const std::string& name
) const
{
  if ( mUdpHash.count(name) > 0 ) {
    return mUdpHash.at(name);
  }
  else {
    return nullptr;
  }
}

// @brief topmodule のリストを返す．
const std::vector<const VlModule*>&
ElbMgr::topmodule_list() const
{
  return mTopmoduleList;
}

// @brief 名前から UserSystf を取出す．
const ElbUserSystf*
ElbMgr::find_user_systf(
  const std::string& name
) const
{
  if ( mSystfHash.count(name) > 0 ) {
    return mSystfHash.at(name);
  }
  else {
    return nullptr;
  }
}

// @brief システムタスク/システム関数を登録する．
void
ElbMgr::reg_user_systf(
  const ElbUserSystf* systf
)
{
  mSystfHash[systf->name()] = systf;
}

// @brief スコープと名前から名前付き要素を取り出す．
ObjHandle*
ElbMgr::find_obj(
  const VlScope* parent,
  const std::string& name
) const
{
  return mObjDict.find(parent, name);
}

// @brief スコープと名前からスコープを取り出す．
const VlScope*
ElbMgr::find_namedobj(
  const VlScope* parent,
  const std::string& name
) const
{
  auto handle{find_obj(parent, name)};
  if ( handle != nullptr ) {
    return handle->scope();
  }
  else {
    return nullptr;
  }
}

// @brief 名前によるオブジェクトの探索
ObjHandle*
ElbMgr::find_obj_up(
  const VlScope* base_scope,
  const AstHierNamedBase* ast_obj,
  const VlScope* ulimit
)
{
  // まず nb の部分の解決を行う．
  base_scope = find_scope_up(base_scope, ast_obj, ulimit);
  if ( base_scope == nullptr ) {
    return nullptr;
  }

  if ( debug & debug_find_scope ) {
    DOUT << "find_obj_up( " << ast_obj->name() << " )@"
	 << base_scope->full_name() << std::endl;
  }

  // base_scope を起点として name というオブジェクトを探す．
  for ( ; base_scope; base_scope = base_scope->parent_scope() ) {
    auto handle{find_obj(base_scope, ast_obj->name())};
    if ( handle ) {
      // 見つけた
      if ( debug & debug_find_scope ) {
	DOUT << "--> Found: " << handle->name() << " @ "
	     << base_scope->name() << std::endl << std::endl;
      }
      return handle;
    }
    // base_scope が上限だったのでこれ以上 upward search できない．
    if ( base_scope == ulimit ) {
      if ( debug & debug_find_scope ) {
	DOUT << "--> Not found: reaches to ulimit" << std::endl << std::endl;
      }
      return nullptr;
    }
  }
  // ダミー
  return nullptr;
}

// base_scope を起点として (name_branch, "" ) という名前のスコープを探す．
// なければ親のスコープに対して同様の探索を繰り返す．
const VlScope*
ElbMgr::find_scope_up(
  const VlScope* base_scope,
  const AstHierNamedBase* ast_obj,
  const VlScope* ulimit
)
{
  if ( debug & debug_find_scope ) {
    DOUT << "find_scope_up( "
	 << ast_obj->decompile_name()
	 << " ) @"
	 << base_scope->full_name() << std::endl;
  }

  SizeType n{ast_obj->namebranch_num()};
  auto cur_scope{base_scope};
  for ( auto name_branch: ast_obj->namebranch_list() ) {
    auto top_name{name_branch->name()};
    const VlScope* top_scope{nullptr};
    // まず普通に探す．
    auto handle{find_obj(cur_scope, top_name)};
    if ( handle ) {
      if ( name_branch->has_index() ) {
	top_scope = handle->array_elem(name_branch->index());
      }
      else {
	top_scope = handle->scope();
      }
    }
    else if ( !name_branch->has_index() ) {
      // モジュール定義名として探す．
      auto module{mModuleDefDict.find(cur_scope, top_name)};
      if ( module ) {
	top_scope = module;
      }
    }
    if ( top_scope == nullptr) {
      // cur_scope が上限もしくは cur_scope の親がいなければ
      // upward search できない．
      if ( cur_scope == ulimit || cur_scope->parent_scope() == nullptr ) {

	if ( debug & debug_find_scope ) {
	  DOUT << "--> Not Found" << std::endl << std::endl;
	}

	return nullptr;
      }

      // upward search を行う．
      if ( debug & debug_find_scope ) {
	DOUT << " upward search" << std::endl;
      }
      cur_scope = cur_scope->parent_scope();
    }
    else {
      // downward search を行う．
      if ( debug & debug_find_scope ) {
	DOUT << " downward search" << std::endl;
      }
      cur_scope = top_scope;
    }
  }

  if ( debug & debug_find_scope ) {
    DOUT << "--> Found: " << cur_scope->full_name() << std::endl << std::endl;
  }
  return cur_scope;
}

// @brief internal scope を登録する．
void
ElbMgr::reg_internalscope(
  const VlScope* obj
)
{
  mObjDict.add(obj);
  mTagDict.add_internalscope(obj);
}

// @brief 属性リストを登録する．
void
ElbMgr::reg_attr(
  const VlObj* obj,
  const std::vector<const VlAttribute*>& attr_list
)
{
  mAttrHash.emplace(obj, attr_list);
}

// @brief トップレベルのスコープを生成する
const VlScope*
ElbMgr::new_Toplevel()
{
  auto scope = factory().new_Toplevel();
  mObjList.push_back(scope);
  mTopLevel = scope;
  return scope;
}

// @brief ステートメントブロックのスコープを生成する．
const VlScope*
ElbMgr::new_StmtBlockScope(
  const VlScope* parent,
  const AstStmt* ast_stmt
)
{
  auto scope = factory().new_StmtBlockScope(parent, ast_stmt);
  mObjList.push_back(scope);
  reg_internalscope(scope);
  return scope;
}

// @brief generate block を生成する．
const VlScope*
ElbMgr::new_GenBlock(
  const VlScope* parent,
  const AstItem* ast_item
)
{
  auto scope = factory().new_GenBlock(parent, ast_item);
  mObjList.push_back(scope);
  reg_internalscope(scope);
  return scope;
}

// @brief GfBlock 検索用の親の名前付きオブジェクトを作る．
ElbGfRoot*
ElbMgr::new_GfRoot(
  const VlScope* parent,
  const AstItem* ast_item
)
{
  auto gfroot = factory().new_GfRoot(parent, ast_item);
  mObjList.push_back(gfroot);
  mObjDict.add(gfroot);
  return gfroot;
}

// @brief generate for block を生成する．
const VlScope*
ElbMgr::new_GfBlock(
  const VlScope* parent,
  const AstItem* ast_item,
  int gvi
)
{
  auto gfblock = factory().new_GfBlock(parent, ast_item, gvi);
  mObjList.push_back(gfblock);
  reg_internalscope(gfblock);
  return gfblock;
}

// @brief UDP定義を生成する．
ElbUdpDefn*
ElbMgr::new_UdpDefn(
  const AstUdp* ast_udp,
  bool is_protected
)
{
  auto udp = factory().new_UdpDefn(ast_udp, is_protected);
  mObjList.push_back(udp);
  mUdpList.push_back(udp);
  mUdpHash[ast_udp->name()] = udp;
  return udp;
}

// @brief module を生成する．
ElbModule*
ElbMgr::new_Module(
  const VlScope* parent,
  const AstModule* ast_module,
  const AstItem* ast_head,
  const AstInst* ast_inst
)
{
  auto module = factory().new_Module(parent, ast_module, ast_head, ast_inst);
  mObjList.push_back(module);
  mObjDict.add(module);
  mModuleDefDict.add(module);
  mTagDict.add_module(module);
  if ( parent == mTopLevel ) {
    mTopmoduleList.push_back(module);
  }
  return module;
}

// @brief module array を生成する．
ElbModuleArray*
ElbMgr::new_ModuleArray(
  const VlScope* parent,
  const AstModule* ast_module,
  const AstItem* ast_head,
  const AstInst* ast_inst,
  const AstRange* ast_range,
  const RangeVal& range
)
{
  auto modulearray = factory().new_ModuleArray(parent, ast_module,
					       ast_head, ast_inst,
					       ast_range, range);
  mObjList.push_back(modulearray);
  mObjDict.add(modulearray);
  mTagDict.add_modulearray(modulearray);
  return modulearray;
}

// @brief module IO ヘッダを生成する．
ElbIOHead*
ElbMgr::new_IOHead(
  const VlModule* module,
  const AstIOHead* ast_header
)
{
  auto head = factory().new_IOHead(module, ast_header);
  mHeadList.push_back(head);
  return head;
}

// @brief タスク/関数用の IO ヘッダを生成する．
ElbIOHead*
ElbMgr::new_IOHead(
  const VlTaskFunc* taskfunc,
  const AstIOHead* ast_header
)
{
  auto head = factory().new_IOHead(taskfunc, ast_header);
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．
ElbDeclHead*
ElbMgr::new_DeclHead(
  const VlScope* parent,
  const AstDeclHead* ast_head,
  bool has_delay
)
{
  auto head = factory().new_DeclHead(parent, ast_head, has_delay);
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．
ElbDeclHead*
ElbMgr::new_DeclHead(
  const VlScope* parent,
  const AstDeclHead* ast_head,
  const AstRange* ast_range,
  const RangeVal& range,
  bool has_delay
)
{
  auto head = factory().new_DeclHead(parent, ast_head,
				     ast_range, range,
				     has_delay);
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
ElbDeclHead*
ElbMgr::new_DeclHead(
  const VlScope* parent,
  const AstIOHead* ast_head,
  VpiAuxType aux_type
)
{
  auto head = factory().new_DeclHead(parent, ast_head, aux_type);
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
ElbDeclHead*
ElbMgr::new_DeclHead(
  const VlScope* parent,
  const AstIOHead* ast_head,
  VpiAuxType aux_type,
  const AstRange* ast_range,
  const RangeVal& range
)
{
  auto head = factory().new_DeclHead(parent, ast_head, aux_type,
				     ast_range, range);
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
ElbDeclHead*
ElbMgr::new_DeclHead(
  const VlScope* parent,
  const AstItem* ast_item
)
{
  auto head = factory().new_DeclHead(parent, ast_item);
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
ElbDeclHead*
ElbMgr::new_DeclHead(
  const VlScope* parent,
  const AstItem* ast_item,
  const AstRange* ast_range,
  const RangeVal& range
)
{
  auto head = factory().new_DeclHead(parent, ast_item, ast_range, range);
  mHeadList.push_back(head);
  return head;
}

// @brief 宣言要素を生成する．
ElbDecl*
ElbMgr::new_Decl(
  int tag,
  ElbDeclHead* head,
  const AstNamedBase* ast_item,
  const VlExpr* init
)
{
  auto decl = factory().new_Decl(head, ast_item, init);
  mObjList.push_back(decl);
  mObjDict.add(decl);
  mTagDict.add_decl(tag, decl);
  return decl;
}

// @brief 暗黙のネットを生成する．
ElbDecl*
ElbMgr::new_ImpNet(
  const VlScope* parent,
  const AstExpr* ast_expr,
  VpiNetType net_type
)
{
  auto decl = factory().new_ImpNet(parent, ast_expr, net_type);
  mObjList.push_back(decl);
  mTagDict.add_decl(vpiNet, decl);
  return decl;
}

// @brief 宣言要素の配列を生成する．
const VlDeclArray*
ElbMgr::new_DeclArray(
  int tag,
  ElbDeclHead* head,
  const AstNamedBase* ast_item,
  const std::vector<ElbRangeSrc>& range_src
)
{
  auto decl = factory().new_DeclArray(head, ast_item, range_src);
  mObjList.push_back(decl);
  mObjDict.add(decl);
  if ( tag == vpiVariables ) {
    // ちょっと汚い補正
    tag += 100;
  }
  mTagDict.add_declarray(tag, decl);
  return decl;
}

// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
ElbParamHead*
ElbMgr::new_ParamHead(
  const VlScope* parent,
  const AstDeclHead* ast_head
)
{
  auto head = factory().new_ParamHead(parent, ast_head);
  mHeadList.push_back(head);
  return head;
}

// @brief parameter 宣言のヘッダを生成する(範囲指定あり)．
ElbParamHead*
ElbMgr::new_ParamHead(
  const VlScope* parent,
  const AstDeclHead* ast_head,
  const AstRange* ast_range,
  const RangeVal& range
)
{
  auto head = factory().new_ParamHead(parent, ast_head, ast_range, range);
  mHeadList.push_back(head);
  return head;
}

// @brief parameter 宣言を生成する．
ElbParameter*
ElbMgr::new_Parameter(
  ElbParamHead* head,
  const AstNamedBase* ast_item,
  bool is_local
)
{
  auto param = factory().new_Parameter(head, ast_item, is_local);
  mObjList.push_back(param);
  mObjDict.add(param);
  mTagDict.add_decl(vpiParameter, param);
  return param;
}

// @brief genvar を生成する．
ElbGenvar*
ElbMgr::new_Genvar(
  const VlScope* parent,
  const AstDeclItem* ast_item,
  int val
)
{
  auto genvar = factory().new_Genvar(parent, ast_item, val);
  mObjList.push_back(genvar);
  mObjDict.add(genvar);
  return genvar;
}

// @brief continuous assignment のヘッダを生成する．
ElbCaHead*
ElbMgr::new_CaHead(
  const VlModule* module,
  const AstItem* ast_head,
  const VlDelay* delay
)
{
  auto head = factory().new_CaHead(module, ast_head, delay);
  mHeadList.push_back(head);
  return head;
}

// @brief continuous assignment を生成する．
const VlContAssign*
ElbMgr::new_ContAssign(
  ElbCaHead* head,
  const AstBase* ast_obj,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  auto contassign = factory().new_ContAssign(head, ast_obj, lhs, rhs);
  mObjList.push_back(contassign);
  mTagDict.add_contassign(contassign);
  return contassign;
}

// @brief net 宣言中の continuous assignment を生成する．
const VlContAssign*
ElbMgr::new_ContAssign(
  const VlModule* module,
  const AstBase* ast_obj,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  auto contassign = factory().new_ContAssign(module, ast_obj, lhs, rhs);
  mObjList.push_back(contassign);
  mTagDict.add_contassign(contassign);
  return contassign;
}

// @brief パラメータ割り当て文を生成する．
const VlParamAssign*
ElbMgr::new_ParamAssign(
  const VlModule* module,
  const AstBase* ast_obj,
  ElbParameter* param,
  const AstExpr* rhs_expr,
  const VlValue& rhs_value
)
{
  auto paramassign = factory().new_ParamAssign(module, ast_obj, param,
					       rhs_expr, rhs_value);
  mObjList.push_back(paramassign);
  mTagDict.add_paramassign(paramassign);
  return paramassign;
}

// @brief 名前によるパラメータ割り当て文を生成する．
const VlParamAssign*
ElbMgr::new_NamedParamAssign(
  const VlModule* module,
  const AstBase* ast_obj,
  ElbParameter* param,
  const AstExpr* rhs_expr,
  const VlValue& rhs_value
)
{
  auto paramassign = factory().new_NamedParamAssign(module, ast_obj, param,
						    rhs_expr, rhs_value);
  mObjList.push_back(paramassign);
  mTagDict.add_paramassign(paramassign);
  return paramassign;
}

// @brief defparam 文を生成する．
const VlDefParam*
ElbMgr::new_DefParam(
  const VlModule* module,
  const AstItem* ast_header,
  const AstDefParam* ast_defparam,
  ElbParameter* param,
  const AstExpr* rhs_expr,
  const VlValue& rhs_value
)
{
  auto defparam = factory().new_DefParam(module, ast_header, ast_defparam,
					 param, rhs_expr, rhs_value);
  mObjList.push_back(defparam);
  mTagDict.add_defparam(defparam);
  return defparam;
}

// @brief ゲートプリミティブのヘッダを生成する．
ElbPrimHead*
ElbMgr::new_PrimHead(
  const VlScope* parent,
  const AstItem* ast_header,
  bool has_delay
)
{
  auto head = factory().new_PrimHead(parent, ast_header, has_delay);
  mHeadList.push_back(head);
  return head;
}

// @brief UDPプリミティブのヘッダを生成する．
ElbPrimHead*
ElbMgr::new_UdpHead(
  const VlScope* parent,
  const AstItem* ast_header,
  const VlUdpDefn* udp,
  bool has_delay
)
{
  auto head = factory().new_UdpHead(parent, ast_header, udp, has_delay);
  mHeadList.push_back(head);
  return head;
}

// @brief セルプリミティブのヘッダを生成する．
ElbPrimHead*
ElbMgr::new_CellHead(
  const VlScope* parent,
  const AstItem* ast_header,
  const ClibCell& cell
)
{
  auto head = factory().new_CellHead(parent, ast_header, cell);
  mHeadList.push_back(head);
  return head;
}

// @brief プリミティブインスタンスを生成する．
ElbPrimitive*
ElbMgr::new_Primitive(
  ElbPrimHead* head,
  const AstInst* ast_inst
)
{
  auto prim = factory().new_Primitive(head, ast_inst);
  mObjList.push_back(prim);
  mObjDict.add(prim);
  mTagDict.add_primitive(prim);
  return prim;
}

// @brief プリミティブ配列インスタンスを生成する．
ElbPrimArray*
ElbMgr::new_PrimitiveArray(
  ElbPrimHead* head,
  const AstInst* ast_inst,
  const AstRange* ast_range,
  const RangeVal& range
)
{
  auto prim = factory().new_PrimitiveArray(head, ast_inst, ast_range, range);
  mObjList.push_back(prim);
  mTagDict.add_primarray(prim);
  return prim;
}

// @brief セルプリミティブインスタンスを生成する．
ElbPrimitive*
ElbMgr::new_CellPrimitive(
  ElbPrimHead* head,
  const ClibCell& cell,
  const AstInst* ast_inst
)
{
  auto prim = factory().new_CellPrimitive(head, cell, ast_inst);
  mObjList.push_back(prim);
  return prim;
}

// @brief セルプリミティブ配列インスタンスを生成する．
ElbPrimArray*
ElbMgr::new_CellPrimitiveArray(
  ElbPrimHead* head,
  const ClibCell& cell,
  const AstInst* ast_inst,
  const AstRange* ast_range,
  const RangeVal& range
)
{
  auto prim = factory().new_CellPrimitiveArray(head, cell, ast_inst,
					       ast_range, range);
  mObjList.push_back(prim);
  return prim;
}

// @brief function を生成する．
ElbTaskFunc*
ElbMgr::new_Function(
  const VlScope* parent,
  const AstItem* ast_item,
  bool const_func
)
{
  auto func = factory().new_Function(parent, ast_item, const_func);
  #warning "reg_Function" で共通化すべき
  mObjList.push_back(func);
  mObjDict.add(func);
  mTagDict.add_function(func);
  return func;
}

// @brief function を生成する．
ElbTaskFunc*
ElbMgr::new_Function(
  const VlScope* parent,
  const AstItem* ast_item,
  const AstRange* ast_range,
  const RangeVal& range,
  bool const_func
)
{
  auto func = factory().new_Function(parent, ast_item,
				     ast_range, range,
				     const_func);
  mObjList.push_back(func);
  mObjDict.add(func);
  mTagDict.add_function(func);
  return func;
}

// @brief task を生成する．
ElbTaskFunc*
ElbMgr::new_Task(
  const VlScope* parent,
  const AstItem* ast_item
)
{
  auto task = factory().new_Task(parent, ast_item);
  mObjList.push_back(task);
  mObjDict.add(task);
  mTagDict.add_task(task);
  return task;
}

// @brief プロセス文を生成する．
ElbProcess*
ElbMgr::new_Process(
  const VlScope* parent,
  const AstItem* ast_item
)
{
  auto process = factory().new_Process(parent, ast_item);
  mObjList.push_back(process);
  mTagDict.add_process(process);
  return process;
}

// @brief 代入文を生成する．
const VlStmt*
ElbMgr::new_Assignment(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs,
  bool block,
  const VlControl* control
)
{
  auto stmt = factory().new_Assignment(parent, process, ast_stmt,
				       lhs, rhs, block, control);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief assign ステートメントを生成する．
const VlStmt*
ElbMgr::new_AssignStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  auto stmt = factory().new_AssignStmt(parent, process, ast_stmt,
				       lhs, rhs);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief deassign ステートメントを生成する．
const VlStmt*
ElbMgr::new_DeassignStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* lhs
)
{
  auto stmt = factory().new_DeassignStmt(parent, process, ast_stmt, lhs);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief force ステートメントを生成する．
const VlStmt*
ElbMgr::new_ForceStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  auto stmt = factory().new_ForceStmt(parent, process, ast_stmt, lhs, rhs);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief release ステートメントを生成する．
const VlStmt*
ElbMgr::new_ReleaseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* lhs
)
{
  auto stmt = factory().new_ReleaseStmt(parent, process, ast_stmt, lhs);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief begin ブロックを生成する．
const VlStmt*
ElbMgr::new_Begin(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const std::vector<const VlStmt*>& stmt_list
)
{
  auto stmt = factory().new_Begin(parent, process, ast_stmt, stmt_list);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief fork ブロックを生成する．
const VlStmt*
ElbMgr::new_Fork(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const std::vector<const VlStmt*>& stmt_list
)
{
  auto stmt = factory().new_Fork(parent, process, ast_stmt, stmt_list);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief 名前付き begin ブロックを生成する．
const VlStmt*
ElbMgr::new_NamedBegin(
  const VlScope* block,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const std::vector<const VlStmt*>& stmt_list
)
{
  auto stmt = factory().new_NamedBegin(block, process, ast_stmt, stmt_list);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief 名前付き fork ブロックを生成する．
const VlStmt*
ElbMgr::new_NamedFork(
  const VlScope* block,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const std::vector<const VlStmt*>& stmt_list
)
{
  auto stmt = factory().new_NamedFork(block, process, ast_stmt, stmt_list);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief while 文を生成する．
const VlStmt*
ElbMgr::new_WhileStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* cond,
  const VlStmt* body
)
{
  auto stmt = factory().new_WhileStmt(parent, process, ast_stmt, cond, body);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief repeat 文を生成する．
const VlStmt*
ElbMgr::new_RepeatStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* cond,
  const VlStmt* body
)
{
  auto stmt = factory().new_RepeatStmt(parent, process, ast_stmt, cond, body);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief wait 文を生成する．
const VlStmt*
ElbMgr::new_WaitStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* cond,
  const VlStmt* body
)
{
  auto stmt = factory().new_WaitStmt(parent, process, ast_stmt, cond, body);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief for 文を生成する．
const VlStmt*
ElbMgr::new_ForStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* cond,
  const VlStmt* init_stmt,
  const VlStmt* inc_stmt,
  const VlStmt* body
)
{
  auto stmt = factory().new_ForStmt(parent, process, ast_stmt, cond,
				    init_stmt, inc_stmt, body);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief forever 文を生成する．
const VlStmt*
ElbMgr::new_ForeverStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlStmt* body
)
{
  auto stmt = factory().new_ForeverStmt(parent, process, ast_stmt, body);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief if 文を生成する．
const VlStmt*
ElbMgr::new_IfStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* cond,
  const VlStmt* then_stmt,
  const VlStmt* else_stmt
)
{
  auto stmt = factory().new_IfStmt(parent, process, ast_stmt,
				   cond, then_stmt, else_stmt);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief case 文を生成する．
const VlStmt*
ElbMgr::new_CaseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlExpr* expr,
  const std::vector<const VlCaseItem*>& caseitem_list
)
{
  auto stmt = factory().new_CaseStmt(parent, process, ast_stmt,
				     expr, caseitem_list);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief caseitem を生成する．
const VlCaseItem*
ElbMgr::new_CaseItem(
  const AstCaseItem* ast_item,
  const std::vector<ElbExpr*>& label_list,
  const VlStmt* body
)
{
  auto caseitem = factory().new_CaseItem(ast_item, label_list, body);
  mObjList.push_back(caseitem);
  return caseitem;
}

// @brief イベント文を生成する．
const VlStmt*
ElbMgr::new_EventStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  ElbExpr* named_event
)
{
  auto stmt = factory().new_EventStmt(parent, process, ast_stmt,
				      named_event);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief nullptr ステートメントを生成する．
const VlStmt*
ElbMgr::new_NullStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt
)
{
  auto stmt = factory().new_NullStmt(parent, process, ast_stmt);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief タスクコール文を生成する．
const VlStmt*
ElbMgr::new_TaskCall(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlTaskFunc* task,
  const std::vector<ElbExpr*>& arg_array
)
{
  auto stmt = factory().new_TaskCall(parent, process, ast_stmt,
				     task, arg_array);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief システムタスクコール文を生成する．
const VlStmt*
ElbMgr::new_SysTaskCall(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlUserSystf* user_systf,
  const std::vector<ElbExpr*>& arg_array
)
{
  auto stmt = factory().new_SysTaskCall(parent, process, ast_stmt,
					user_systf, arg_array);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief disable 文を生成する．
const VlStmt*
ElbMgr::new_DisableStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlScope* target
)
{
  auto stmt = factory().new_DisableStmt(parent, process, ast_stmt, target);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief コントロール文を生成する．
const VlStmt*
ElbMgr::new_CtrlStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt,
  const VlControl* control,
  const VlStmt* body
)
{
  auto stmt = factory().new_CtrlStmt(parent, process, ast_stmt,
				     control, body);
  mObjList.push_back(stmt);
  return stmt;
}

// @brief 遅延コントロールを生成する．
const VlControl*
ElbMgr::new_DelayControl(
  const AstControl* ast_control,
  ElbExpr* delay
)
{
  auto control = factory().new_DelayControl(ast_control, delay);
  mObjList.push_back(control);
  return control;
}

// @brief イベントコントロールを生成する．
const VlControl*
ElbMgr::new_EventControl(
  const AstControl* ast_control,
  const std::vector<ElbExpr*>& event_list
)
{
  auto control = factory().new_EventControl(ast_control, event_list);
  mObjList.push_back(control);
  return control;
}

// @brief リピートコントロールを生成する．
const VlControl*
ElbMgr::new_RepeatControl(
  const AstControl* ast_control,
  ElbExpr* rep,
  const std::vector<ElbExpr*>& event_list
)
{
  auto control = factory().new_RepeatControl(ast_control, rep, event_list);
  mObjList.push_back(control);
  return control;
}

// @brief 単項演算子を生成する．
ElbExpr*
ElbMgr::new_UnaryOp(
  const AstExpr* ast_expr,
  VpiOpType op_type,
  ElbExpr* opr1
)
{
  auto expr = factory().new_UnaryOp(ast_expr, op_type,
				    opr1);
  mObjList.push_back(expr);
  return expr;
}

// @brief 2項演算子を生成する．
ElbExpr*
ElbMgr::new_BinaryOp(
  const AstExpr* ast_expr,
  VpiOpType op_type,
  ElbExpr* opr1,
  ElbExpr* opr2
)
{
  auto expr = factory().new_BinaryOp(ast_expr, op_type,
				     opr1, opr2);
  mObjList.push_back(expr);
  return expr;
}

// @brief 3項演算子を生成する．
ElbExpr*
ElbMgr::new_TernaryOp(
  const AstExpr* ast_expr,
  VpiOpType op_type,
  ElbExpr* opr1,
  ElbExpr* opr2,
  ElbExpr* opr3
)
{
  auto expr = factory().new_TernaryOp(ast_expr, op_type,
				      opr1, opr2, opr3);
  mObjList.push_back(expr);
  return expr;
}

// @brief 連結演算子を生成する．
ElbExpr*
ElbMgr::new_ConcatOp(
  const AstExpr* ast_expr,
  const std::vector<ElbExpr*>& opr_list
)
{
  auto expr = factory().new_ConcatOp(ast_expr, opr_list);
  mObjList.push_back(expr);
  return expr;
}

// @brief 反復連結演算子を生成する．
ElbExpr*
ElbMgr::new_MultiConcatOp(
  const AstExpr* ast_expr,
  SizeType rep_num,
  ElbExpr* rep_expr,
  const std::vector<ElbExpr*>& opr_list
)
{
  auto expr = factory().new_MultiConcatOp(ast_expr,
					  rep_num, rep_expr,
					  opr_list);
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する．
ElbExpr*
ElbMgr::new_Primary(
  const AstExpr* ast_expr,
  const VlDecl* obj
)
{
  auto expr = factory().new_Primary(ast_expr, obj);
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する(net decl の初期値用)．
ElbExpr*
ElbMgr::new_Primary(
  const AstDeclItem* ast_item,
  const VlDecl* obj
)
{
  auto expr = factory().new_Primary(ast_item, obj);
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する．
ElbExpr*
ElbMgr::new_Primary(
  const AstExpr* ast_expr,
  ElbParameter* obj
)
{
  auto expr = factory().new_Primary(ast_expr, obj);
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する(配列要素版)．
ElbExpr*
ElbMgr::new_Primary(
  const AstExpr* ast_expr,
  const VlDeclArray* obj,
  const std::vector<ElbExpr*>& index_list
)
{
  auto expr = factory().new_Primary(ast_expr, obj, index_list);
  mObjList.push_back(expr);
  return expr;
}

// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
ElbExpr*
ElbMgr::new_Primary(
  const AstExpr* ast_expr,
  const VlDeclArray* obj,
  int offset
)
{
  auto expr = factory().new_Primary(ast_expr, obj, offset);
  mObjList.push_back(expr);
  return expr;
}

// @brief 固定ビット選択式を生成する．
ElbExpr*
ElbMgr::new_BitSelect(
  const AstExpr* ast_expr,
  ElbExpr* base,
  const AstExpr* bit_index,
  int bit_index_val
)
{
  auto expr = factory().new_BitSelect(ast_expr, base,
				      bit_index, bit_index_val);
  mObjList.push_back(expr);
  return expr;
}

// @brief 固定ビット選択式を生成する．
ElbExpr*
ElbMgr::new_BitSelect(
  const AstExpr* ast_expr,
  ElbExpr* base,
  int bit_index_val
)
{
  auto expr = factory().new_BitSelect(ast_expr, base, bit_index_val);
  mObjList.push_back(expr);
  return expr;
}

// @brief 可変ビット選択式を生成する．
ElbExpr*
ElbMgr::new_BitSelect(
  const AstExpr* ast_expr,
  ElbExpr* base,
  ElbExpr* bit_index
)
{
  auto expr = factory().new_BitSelect(ast_expr, base, bit_index);
  mObjList.push_back(expr);
  return expr;
}

// @brief 固定部分選択式を生成する．
ElbExpr*
ElbMgr::new_PartSelect(
  const AstExpr* ast_expr,
  ElbExpr* obj,
  const AstExpr* index1,
  const AstExpr* index2,
  int index1_val,
  int index2_val
)
{
  auto expr = factory().new_PartSelect(ast_expr, obj,
				       index1, index2,
				       index1_val, index2_val);
  mObjList.push_back(expr);
  return expr;
}

// @brief 固定部分選択式を生成する．
ElbExpr*
ElbMgr::new_PartSelect(
  const AstExpr* ast_expr,
  ElbExpr* base,
  int index1,
  int index2
)
{
  auto expr = factory().new_PartSelect(ast_expr, base,
				       index1, index2);
  mObjList.push_back(expr);
  return expr;
}

// @brief 可変部分選択式を生成する．
ElbExpr*
ElbMgr::new_PlusPartSelect(
  const AstExpr* ast_expr,
  ElbExpr* obj,
  ElbExpr* base,
  const AstExpr* range_expr,
  int range_val
)
{
  auto expr = factory().new_PlusPartSelect(ast_expr, obj, base,
					   range_expr, range_val);
  mObjList.push_back(expr);
  return expr;
}

// @brief 可変部分選択式を生成する．
ElbExpr*
ElbMgr::new_MinusPartSelect(
  const AstExpr* ast_expr,
  ElbExpr* obj,
  ElbExpr* base,
  const AstExpr* range_expr,
  int range_val
)
{
  auto expr = factory().new_MinusPartSelect(ast_expr, obj, base,
					    range_expr, range_val);
  mObjList.push_back(expr);
  return expr;
}

// @brief 定数式を生成する．
ElbExpr*
ElbMgr::new_Constant(
  const AstExpr* ast_expr
)
{
  auto expr = factory().new_Constant(ast_expr);
  mObjList.push_back(expr);
  return expr;
}

// @brief genvar 起因の定数式を生成する．
ElbExpr*
ElbMgr::new_GenvarConstant(
  const AstExpr* ast_primary,
  int val
)
{
  auto expr = factory().new_GenvarConstant(ast_primary, val);
  mObjList.push_back(expr);
  return expr;
}

// @brief 関数呼び出し式を生成する．
ElbExpr*
ElbMgr::new_FuncCall(
  const AstExpr* ast_expr,
  const VlTaskFunc* func,
  const std::vector<ElbExpr*>& arg_list
)
{
  auto expr = factory().new_FuncCall(ast_expr, func, arg_list);
  mObjList.push_back(expr);
  return expr;
}

// @brief システム関数呼び出し式を生成する．
ElbExpr*
ElbMgr::new_SysFuncCall(
  const AstExpr* ast_expr,
  const VlUserSystf* user_systf,
  const std::vector<ElbExpr*>& arg_list
)
{
  auto expr = factory().new_SysFuncCall(ast_expr, user_systf,
					arg_list);
  mObjList.push_back(expr);
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
ElbMgr::new_ArgHandle(
  const AstExpr* ast_expr,
  const VlScope* arg
)
{
  auto expr = factory().new_ArgHandle(ast_expr, arg);
  mObjList.push_back(expr);
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
ElbMgr::new_ArgHandle(
  const AstExpr* ast_expr,
  const VlPrimitive* arg
)
{
  auto expr = factory().new_ArgHandle(ast_expr, arg);
  mObjList.push_back(expr);
  return expr;
}

// @brief システム関数/システムタスクの引数を生成する．
ElbExpr*
ElbMgr::new_ArgHandle(
  const AstExpr* ast_expr,
  const VlDeclArray* arg
)
{
  auto expr = factory().new_ArgHandle(ast_expr, arg);
  mObjList.push_back(expr);
  return expr;
}

// @brief 連結演算子の左辺式を生成する．
ElbExpr*
ElbMgr::new_Lhs(
  const AstExpr* ast_expr,
  const std::vector<ElbExpr*>& opr_array,
  const std::vector<ElbExpr*>& lhs_elem_array
)
{
  auto expr = factory().new_Lhs(ast_expr, opr_array, lhs_elem_array);
  mObjList.push_back(expr);
  return expr;
}

// @brief 遅延値を生成する．
const VlDelay*
ElbMgr::new_Delay(
  const AstBase* ast_obj,
  const std::vector<ElbExpr*>& expr_list
)
{
  auto delay = factory().new_Delay(ast_obj, expr_list);
  mObjList.push_back(delay);
  return delay;
}

// @brief attribute instance のリストを生成する．
const VlAttribute*
ElbMgr::new_Attribute(
  const AstAttrSpec* ast_attr,
  const VlExpr* expr,
  bool def
)
{
  auto attr = factory().new_Attribute(ast_attr, expr, def);
  mObjList.push_back(attr);
  return attr;
}

END_NAMESPACE_YM_VERILOG
