
/// @file ItemGen_taskfunc.cc
/// @brief ElbMgr の実装ファイル(タスク/関数の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014, 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "ElbEnv.h"
#include "ElbStub.h"

#include "ym/vl/AstItem.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbTaskFunc.h"
#include "elaborator/RangeVal.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int debug = 0;
#define dout std::cerr

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// task/function 関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief task/function の生成を行う．
void
ItemGen::phase1_tf(
  const VlScope* parent,
  const AstItem* ast_item
)
{
  if ( debug ) {
    dout << std::endl
	 << "phase1_tf( "
	 << parent->full_name()
	 << " ["
	 << std::hex << reinterpret_cast<PtrIntType>(parent) << std::dec
	 << "], "
	 << ast_item->name() << " )" << std::endl
	 << std::endl;
  }

  auto taskfunc = (ElbTaskFunc*)nullptr;
  if ( ast_item->type() == AstItem::Task ) {
    taskfunc = mgr().new_Task(parent, ast_item);
  }
  else {
    if ( ast_item->type() != AstItem::Func ) {
      throw std::logic_error{"ast_item->type() != AstItem::Func"};
    }

    auto ast_range = ast_item->range();
    if ( ast_range != nullptr ) {
      auto range = evaluate_range(parent, ast_range);
      taskfunc = mgr().new_Function(parent, ast_item,
				    ast_range, range, false);
    }
    else {
      taskfunc = mgr().new_Function(parent, ast_item, false);
    }
    if ( taskfunc == nullptr ) {
      throw std::logic_error{"taskfunc == nullptr"};
    }
  }

  // 宣言要素の生成(phase1 では parameter と genvar のみ)
  phase1_decl(taskfunc, ast_item->declhead_list(), false);

  // attribute instance の生成
  auto attr_list = attribute_list(ast_item);
  mgr().reg_attr(taskfunc, attr_list);

  {
    std::ostringstream buf;
    buf << "instantiating task/func : " << taskfunc->full_name() << ".";
    put_info(__FILE__, __LINE__,
	     ast_item->file_region(),
	     "ELAB",
	     buf.str());
  }

  // 本体のステートメント内部のスコープの生成
  auto ast_body = ast_item->body();
  phase1_stmt(taskfunc, ast_body);

  // 残りの仕事は phase2, phase3 で行う．
  add_phase2stub(make_stub(this, &ItemGen::phase2_tf, taskfunc, ast_item));
  add_phase3stub(make_stub(this, &ItemGen::phase3_tf, taskfunc, ast_item));

  if ( debug ) {
    dout << "phase1_tf end" << std::endl
	 << std::endl;
  }
}

// @param[in] task/function 内の宣言要素の生成を行う．
void
ItemGen::phase2_tf(
  ElbTaskFunc* taskfunc,
  const AstItem* ast_item
)
{
  if ( debug ) {
    dout << std::endl
	 << "phase2_tf( "
	 << taskfunc->full_name()
	 << " ["
	 << std::hex << reinterpret_cast<PtrIntType>(taskfunc) << std::dec
	 << "] )" << std::endl
	 << std::endl;
  }

  // 宣言要素の生成
  instantiate_decl(taskfunc, ast_item->declhead_list());

  // 入出力の生成
  instantiate_iodecl(taskfunc, ast_item->iohead_list());

  if ( taskfunc->type() == VpiObjType::Function ) {
    // 関数名と同名の変数の生成
    int left_val = taskfunc->left_range_val();
    int right_val = taskfunc->right_range_val();
    RangeVal range{left_val, right_val};
    auto head = (ElbDeclHead*)nullptr;
    if ( taskfunc->has_range() ) {
      head = mgr().new_DeclHead(taskfunc, ast_item,
				ast_item->range(), range);
    }
    else {
      head = mgr().new_DeclHead(taskfunc, ast_item);
    }
    if ( head == nullptr ) {
      throw std::logic_error{"head == nullptr"};
    }

    int tag{ (ast_item->data_type() == VpiVarType::None) ? vpiReg : vpiVariables };
    auto decl = mgr().new_Decl(tag, head, ast_item);

    taskfunc->set_ovar(decl);
  }

  if ( debug ) {
    dout << "phase2_tf end" << std::endl
	 << std::endl;
  }
}

// @param[in] task/function 内のステートメントの生成を行う．
void
ItemGen::phase3_tf(
  ElbTaskFunc* taskfunc,
  const AstItem* ast_item
)
{
  if ( debug ) {
    dout << std::endl
	 << "phase3_tf( "
	 << taskfunc->full_name()
	 << " ["
	 << std::hex << reinterpret_cast<PtrIntType>(taskfunc) << std::dec
	 << "] )" << std::endl
	 << std::endl;
  }

  // 本体のステートメントの生成
  ElbTfEnv env(taskfunc);
  auto ast_body = ast_item->body();
  auto body = instantiate_stmt(taskfunc, nullptr, env, ast_body);
  if ( body ) {
    taskfunc->set_stmt(body);
  }

  if ( debug ) {
    dout << "phase3_tf end" << std::endl
	 << std::endl;
  }
}

// @brief constant function の生成を行う．
const VlTaskFunc*
ItemGen::instantiate_constant_function(
  const VlScope* parent,
  const AstItem* ast_function
)
{
  // 基本的には phase1_tf(), phase2_tf(), phase3_tf() と同じことを
  // やっているが，ElbConstantFunctionEnv を用いているところが異なる．
  if ( debug ) {
    dout << std::endl
	 << "instantiate_constant_function( "
	 << parent->full_name()
	 << " ["
	 << std::hex << reinterpret_cast<PtrIntType>(parent) << std::dec
	 << "] , "
	 << ast_function->name() << " )" << std::endl
	 << std::endl;
  }

  auto ast_range = ast_function->range();

  auto func = (ElbTaskFunc*)nullptr;
  auto scope = (const VlScope*)nullptr;
  auto head = (ElbDeclHead*)nullptr;
  if ( ast_range != nullptr ) {
    auto range = evaluate_range(parent, ast_range);
    func = mgr().new_Function(parent, ast_function,
			      ast_range, range, true);
    head = mgr().new_DeclHead(func, ast_function,
			      ast_range, range);
  }
  else {
    func = mgr().new_Function(parent, ast_function, true);
    head = mgr().new_DeclHead(func, ast_function);
  }
  if ( func == nullptr ) {
    throw std::logic_error{"func == nullptr"};
  }
  if ( head == nullptr ) {
    throw std::logic_error{"head == nullptr"};
  }

  // 登録しておく．
  reg_constant_function(func);

  // parameter の生成
  phase1_decl(func, ast_function->declhead_list(), false);

  // 宣言要素の生成
  instantiate_decl(func, ast_function->declhead_list());

  // 関数名と同名の変数の生成
  int tag{ (ast_function->data_type() == VpiVarType::None) ? vpiReg : vpiVariables };
  auto decl = mgr().new_Decl(tag, head, ast_function);

  func->set_ovar(decl);

  // 入出力の生成
  instantiate_iodecl(func, ast_function->iohead_list());

  // 本体のステートメント内部のスコープの生成
  auto ast_body = ast_function->body();
  phase1_stmt(func, ast_body, true);

  // 本体のステートメントの生成
  ElbConstantFunctionEnv env(scope);
  auto body = instantiate_stmt(scope, nullptr, env, ast_body);
  if ( body ) {
    func->set_stmt(body);
  }

  if ( debug ) {
    dout << "instantiate_constant_function end" << std::endl
	 << std::endl;
  }

  return func;
}

END_NAMESPACE_YM_VERILOG
