
/// @file Elaborator.cc
/// @brief Elaborator の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/Elaborator.h"

#include "UdpGen.h"
#include "ModuleGen.h"
#include "DeclGen.h"
#include "ItemGen.h"
#include "StmtGen.h"
#include "ExprGen.h"
#include "ExprEval.h"
#include "AttrGen.h"
#include "DefParamStub.h"
#include "ElbStub.h"

#include "ym/vl/AstModule.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlUdp.h"

#include "ym/ClibCellLibrary.h"

#include "parser/AstMgr.h"

#include "elaborator/ElbModule.h"
#include "elaborator/ElbModuleArray.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ElbProcess.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbError.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

#define DOUT cerr

const int debug_none       = 0x00000000;
const int debug_find_scope = 0x00000010;
const int debug_all        = 0xFFFFFFFF;

#if 1
const int debug = debug_none;
#else
const int debug = debug_all;
#endif

BEGIN_NONAMESPACE

// mFuncDict 用のキー生成関数
inline
std::string
gen_funckey(
  const AstModule& module,
  const std::string& name
)
{
  return std::string{module.name()} + " " + name;
}

END_NONAMESPACE

// @brief コンストラクタ
Elaborator::Elaborator(
  ElbMgr& elb_mgr,
  LogMgr& log_mgr,
  const ClibCellLibrary& cell_library
) : mDone{false},
    mElbMgr{elb_mgr},
    mLogMgr{log_mgr},
    mCellLibrary{cell_library},
    mUdpGen{new UdpGen(*this)},
    mModuleGen{new ModuleGen(*this)},
    mDeclGen{new DeclGen(*this)},
    mItemGen{new ItemGen(*this)},
    mStmtGen{new StmtGen(*this)},
    mExprGen{new ExprGen(*this)},
    mExprEval{new ExprEval(*this)},
    mAttrGen{new AttrGen(*this)}
{
  mAllowEmptyIORange = true;

  mUdpGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		mExprGen.get(), mExprEval.get(), mAttrGen.get());
  mModuleGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		   mExprGen.get(), mExprEval.get(), mAttrGen.get());
  mDeclGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mExprEval.get(), mAttrGen.get());
  mItemGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mExprEval.get(), mAttrGen.get());
  mStmtGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mExprEval.get(), mAttrGen.get());
  mExprGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mExprEval.get(), mAttrGen.get());
  mExprEval->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		  mExprGen.get(), mExprEval.get(), mAttrGen.get());
  mAttrGen->init(mModuleGen.get(), mDeclGen.get(), mItemGen.get(), mStmtGen.get(),
		 mExprGen.get(), mExprEval.get(), mAttrGen.get());
}

// @brief デストラクタ
Elaborator::~Elaborator()
{
}

// @brief エラボレーションを行う．
bool
Elaborator::operator()(
  const AstMgr& ast_mgr
)
{
  if ( mDone ) {
    throw std::logic_error{"mDone"};
  }

  auto& ast_udp_list = ast_mgr.udp_list();
  auto& ast_module_list = ast_mgr.module_list();

  // attribute instance の生成
  for ( const auto& attr_info: ast_mgr.all_attr_list() ) {
    mAttrGen->instantiate_attribute(attr_info);
  }

  // UDP の生成
  for ( auto ast_udp: ast_udp_list ) {
    try {
      mUdpGen->instantiate_udp(ast_udp);
    }
    catch ( const ElbError& error ) {
      // 処理を続ける．
    }
  }

  // モジュールテンプレートの辞書を作る．
  // と同時に UDP 名とモジュール名の重複チェックを行う．
  // と同時に関数定義の辞書を作る．
  for ( auto ast_module: ast_module_list ) {
    try {
      auto name = ast_module.name();
      auto prev_udp = mElbMgr.find_udp(name);
      auto prev_module = mModuleDict.at(name);
      if ( prev_udp != nullptr ) {
	log_mgr().error_module_redefined(__FILE__, __LINE__,
					 ast_module,
					 prev_udp->file_region());
      }
      else if ( prev_module.is_valid() ) {
	log_mgr().error_module_redefined(__FILE__, __LINE__,
					 ast_module,
					 prev_module.file_region());
      }
      else {
	// モジュール名をキーにして登録する．
	mModuleDict.emplace(name, ast_module);
      }
      // 関数の辞書を作る．
      for ( auto item: ast_module.item_list() ) {
	if ( item.type() == AstItem::Func ) {
	  auto key = gen_funckey(ast_module, item.name());
	  if ( mFuncDict.count(key) > 0 ) {
	    auto prev_func = mFuncDict.at(key);
	    log_mgr().error_function_redefined(__FILE__, __LINE__,
					       item,
					       prev_func.file_region());
	  }
	  mFuncDict.emplace(key, item);
	}
      }
    }
    catch ( const ElbError& error ) {
      // 無視して処理を続ける．
    }
  }

  if ( log_mgr().error_num() > 0 ) {
    return false;
  }

  // トップレベル階層の生成
  /// toplevel は実体を持たない仮想的なスコープ
  auto toplevel = mElbMgr.new_Toplevel();

  // トップモジュールの生成
  for ( auto ast_module: ast_module_list ) {
    if ( !ast_mgr.check_def_name(ast_module.name()) ) {
      // 他のモジュールから参照されていないモジュールをトップモジュールとみなす．
      mModuleGen->phase1_topmodule(toplevel, ast_module);
    }
  }

  // 残りの要素の生成

  // Phase 1
  // トップモジュールから名前空間を表す骨組みを作る．
  // 最下位レベルのモジュールに行き着くか配列型のモジュールインスタンス
  // で止る．
  //
  // * その時点で適用可能な defparam 文を適用する．
  // * 配列型 module instance および generate 文から骨組みの生成を行う．
  // これを繰り返す．
  for ( ; ; ) {
    // defparam 文で適用できるものがあれば適用する．
    log_mgr().debug(__FILE__, __LINE__,
		    FileRegion(),
		    "ELAB_DEFPARAM",
		    "\"instantiate_defparam\" starts.");

    // 未処理の defparam 文を処理する．
    // 処理された要素は mDefParamList から削除される．
    // 中にはまだ名前空間が構築されていないものもあるので mDefParamList
    // が空にならない場合もある．
    for ( auto p = mDefParamStubList.begin(); p != mDefParamStubList.end(); ) {
      // リストの中で要素の削除を行うので反復子の扱いがちょっと複雑
      auto q = p;
      ++ p;

      if ( mItemGen->defparam_override(*q, nullptr) ) {
	// オーバーライドがうまく行ったらこの要素は捨てる．
	mDefParamStubList.erase(q);
      }
    }

    // その結果にもとづいてモジュール配列インスタンスや
    // generate block の生成を行う．
    log_mgr().debug(__FILE__, __LINE__,
		    FileRegion(),
		    "ELAB_PHASE1",
		    "Phase 1 starts.");

    if ( mPhase1StubList1.empty() ) {
      // 処理する要素が残っていない．
      break;
    }

    // Phase1Stub の実行中に新しい stub が追加されるので
    // 追加用のリストと評価用のリストを分離している．
    mPhase1StubList2 = std::move(mPhase1StubList1);
    mPhase1StubList2.eval();
  }

  // 適用できなかった defparam 文のチェック
  for ( auto stub: mDefParamStubList ) {
    try {
      auto ast_defparam = stub.mAstDefparam;
      log_mgr().error_defparam_unresolved(__FILE__, __LINE__,
					  ast_defparam);
    }
    catch ( const ElbError& error ) {
      // 処理は続行する．
    }
  }

  // Phase 2
  // 配列要素やビット要素の生成を行う．
  log_mgr().debug(__FILE__, __LINE__,
		  FileRegion(),
		  "ELAB_PHASE2",
		  "Phase 2 starts.");

  mPhase2StubList.eval();

  // Phase 3
  // 名前の解決(リンク)を行う．
  log_mgr().debug(__FILE__, __LINE__,
		  FileRegion(),
		  "ELAB_PHASE3",
		  "Phase 3 starts.");

  mPhase3StubList.eval();

  return log_mgr().error_num() == 0;
}

// 後で処理する defparam 文を登録する．
void
Elaborator::add_defparamstub(
  const VlModule* module,
  const AstItem& ast_header
)
{
  for ( auto ast_defparam: ast_header.defparam_list() ) {
    mDefParamStubList.push_back(DefParamStub{module, ast_header, ast_defparam});
  }
}

// @brief 関数定義を探す．
AstItem
Elaborator::find_funcdef(
  const VlModule* module,
  const std::string& name
) const
{
  auto ast_module = find_moduledef(module->def_name());
  if ( ast_module.is_valid() ) {
    auto key = gen_funckey(ast_module, name);
    if ( mFuncDict.count(key) > 0 ) {
      return mFuncDict.at(key);
    }
  }
  return AstItem();
}

// @brief constant function を取り出す．
const VlTaskFunc*
Elaborator::find_constant_function(
  const VlScope* parent,
  const std::string& name
) const
{
  auto h = mCfDict.find(parent, name);
  if ( h != nullptr ) {
    return h->taskfunc();
  }

  return nullptr;
}

// @brief オブジェクトを探す．
ObjHandle*
Elaborator::find_obj(
  const VlScope* parent,
  const std::string& name
) const
{
  return mElbMgr.find_obj(parent, name);
}

// @brief インスタンス化の印を付ける．
void
Elaborator::set_instance_mark(
  const AstModule& ast_module
)
{
  mModuleMark.insert(ast_module.key());
}

// @brief インスタンス化の印を消す．
void
Elaborator::clear_instance_mark(
  const AstModule& ast_module
)
{
  mModuleMark.erase(ast_module.key());
}

// @brief インスタンス化の印を調べる．
bool
Elaborator::check_instance_mark(
  const AstModule& ast_module
) const
{
  return mModuleMark.count(ast_module.key()) > 0;
}

END_NAMESPACE_YM_VERILOG
