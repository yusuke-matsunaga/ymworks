
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
#include "ym/vl/VlUdpDefn.h"
#include "ym/vl/AstUdp.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbUdpDefn.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbModuleArray.h"
#include "elaborator/ElbIOHead.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbDeclHead.h"
#include "elaborator/ElbDeclRef.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbCaHead.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbPrimHead.h"
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
ElbMgr::ElbMgr()
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
  mHeadList.clear();
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
  if ( mUdpHash.count(name) == 0 ) {
    return nullptr;
  }
  return mUdpHash.at(name);
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
  auto handle = find_obj(parent, name);
  if ( handle == nullptr ) {
    return nullptr;
  }
  return handle->scope();
}

// @brief 名前によるオブジェクトの探索
ObjHandle*
ElbMgr::find_obj_up(
  const VlScope* base_scope,
  const AstHierNamedBase& ast_obj,
  const VlScope* ulimit
)
{
  // まず nb の部分の解決を行う．
  base_scope = find_scope_up(base_scope, ast_obj, ulimit);
  if ( base_scope == nullptr ) {
    return nullptr;
  }

  if ( debug & debug_find_scope ) {
    DOUT << "find_obj_up( " << ast_obj.name() << " )@"
	 << base_scope->full_name() << std::endl;
  }

  // base_scope を起点として name というオブジェクトを探す．
  for ( ; base_scope; base_scope = base_scope->parent_scope() ) {
    auto handle{find_obj(base_scope, ast_obj.name())};
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
  const AstHierNamedBase& ast_obj,
  const VlScope* ulimit
)
{
  if ( debug & debug_find_scope ) {
    DOUT << "find_scope_up( "
	 << ast_obj.decompile_name()
	 << " ) @"
	 << base_scope->full_name() << std::endl;
  }

  auto ast_nb_list = ast_obj.namebranch_list();
  auto n = ast_nb_list.size();
  auto cur_scope{base_scope};
  for ( auto name_branch: ast_nb_list ) {
    auto top_name = name_branch.name();
    const VlScope* top_scope{nullptr};
    // まず普通に探す．
    auto handle = find_obj(cur_scope, top_name);
    if ( handle ) {
      if ( name_branch.has_index() ) {
	top_scope = handle->array_elem(name_branch.index());
      }
      else {
	top_scope = handle->scope();
      }
    }
    else if ( !name_branch.has_index() ) {
      // モジュール定義名として探す．
      auto module = mModuleDefDict.find(cur_scope, top_name);
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

#if 0
// @brief net 宣言中の continuous assignment を生成する．
const VlContAssign*
ElbMgr::new_ContAssign(
  const VlModule* module,
  const FileRegion& loc,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  auto contassign = factory().new_ContAssign(module, loc, lhs, rhs);
  reg_obj(contassign);
#if 0
  SizeType offset = 0;
  for ( auto lhs_elem: lhs->lhs_elem_list() ) {
    reg_ca(lhs_elem, offset, rhs);
    auto dst_bw = lhs_elem->bit_size();
    offset += dst_bw;
  }
#endif
  mTagDict.add_contassign(contassign);
  return contassign;
}

// @brief continuous assignment の情報を登録する．
void
ElbMgr::reg_ca(
  const VlExpr* lhs,
  SizeType offset,
  const VlExpr* rhs
)
{
}
#endif

#if 0
// @brief ゲートプリミティブのヘッダを生成する．
ElbPrimHead*
ElbMgr::new_PrimHead(
  const VlScope* parent,
  const AstItem& ast_header,
  bool has_delay
)
{
  auto head = factory().new_PrimHead(parent, ast_header, has_delay);
  mHeadList.push_back(std::unique_ptr<const ElbHead>{head});
  return head;
}

// @brief UDPプリミティブのヘッダを生成する．
ElbPrimHead*
ElbMgr::new_UdpHead(
  const VlScope* parent,
  const AstItem& ast_header,
  const VlUdpDefn* udp,
  bool has_delay
)
{
  auto head = factory().new_UdpHead(parent, ast_header, udp, has_delay);
  mHeadList.push_back(std::unique_ptr<const ElbHead>{head});
  return head;
}

// @brief セルプリミティブのヘッダを生成する．
ElbPrimHead*
ElbMgr::new_CellHead(
  const VlScope* parent,
  const AstItem& ast_header,
  const ClibCell& cell
)
{
  auto head = factory().new_CellHead(parent, ast_header, cell);
  mHeadList.push_back(std::unique_ptr<const ElbHead>{head});
  return head;
}
#endif

// @brief function を登録する．
void
ElbMgr::reg_func(
  const VlTaskFunc* func
)
{
  reg_obj(func);
  mObjDict.add(func);
  mTagDict.add_function(func);
}

END_NAMESPACE_YM_VERILOG
