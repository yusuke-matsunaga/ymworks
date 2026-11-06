
/// @file EiTaskFunc.cc
/// @brief EiTaskFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiTask.h"
#include "EiFunction.h"
#include "elaborator/ElbDecl.h"
#include "ym/vl/VlIODecl.h"
#include "ym/vl/VlTime.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief function を生成する．
ElbTaskFunc*
ElbMgr::new_Function(
  const VlScope* parent,
  const AstItem& ast_item,
  bool const_func
)
{
  // IO数を数え配列を初期化する．
  auto io_num = ast_item.ioitem_num();
  auto func = new EiFunction(parent, ast_item, io_num, const_func);
  reg_func(func);
  return func;
}

// @brief function を生成する．
ElbTaskFunc*
ElbMgr::new_Function(
  const VlScope* parent,
  const AstItem& ast_item,
  const AstRange& ast_range,
  const RangeVal& range,
  bool const_func
)
{
  if ( ast_range.is_invalid() ) {
    throw std::logic_error{"ast_range == nullptr"};
  }

  // IO数を数え配列を初期化する．
  auto io_num = ast_item.ioitem_num();
  auto func = new EiFunctionV(parent, ast_item, io_num,
			      ast_range, range, const_func);
  reg_func(func);
  return func;
}

// @brief task を生成する．
ElbTaskFunc*
ElbMgr::new_Task(
  const VlScope* parent,
  const AstItem& ast_item
)
{
  // IO数を数え配列を初期化する．
  auto io_num = ast_item.ioitem_num();
  auto task = new EiTask(parent, ast_item, io_num);
  reg_obj(task);
  mObjDict.add(task);
  mTagDict.add_task(task);
  return task;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskFunc
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置の取得
FileRegion
EiTaskFunc::file_region() const
{
  return mAstItem.file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiTaskFunc::parent_scope() const
{
  return mParent;
}

// @brief 名前の取得
std::string
EiTaskFunc::name() const
{
  return mAstItem.name();
}

// @brief automatic 宣言されていたら true を返す．
bool
EiTaskFunc::automatic() const
{
  return mAstItem.automatic();
}

// @brief 入出力数を得る．
SizeType
EiTaskFunc::io_num() const
{
  return mIODeclList.size();
}

// @brief 入出力を得る．
const VlIODecl*
EiTaskFunc::io(
  SizeType pos
) const
{
  if ( pos >= io_num() ) {
    throw std::out_of_range{"EiTaskFunc::io(pos): pos is out of range"};
  }
  return mIODeclList[pos].get();
}

// @brief 入出力のリストの取得
std::vector<const VlIODecl*>
EiTaskFunc::io_list() const
{
  std::vector<const VlIODecl*> ans_list;
  ans_list.reserve(io_num());
  for ( auto& io: mIODeclList ) {
    ans_list.push_back(io.get());
  }
  return ans_list;
}

// @brief 本体のステートメントを得る．
const VlStmt*
EiTaskFunc::stmt() const
{
  return mStmt;
}

// @brief 入出力の初期設定を行う．
void
EiTaskFunc::add_iodecl(
  const VlIODecl* io_decl
)
{
  mIODeclList.push_back(std::unique_ptr<const VlIODecl>{io_decl});
}

// @brief 本体のステートメントをセットする．
void
EiTaskFunc::set_stmt(
  const VlStmt* stmt
)
{
  mStmt = stmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTask
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiTask::type() const
{
  return VpiObjType::Task;
}

// @brief function type を返す．
VpiFuncType
EiTask::func_type() const
{
  return VpiFuncType::Int;
}

// @brief 符号付きの時 true を返す．
bool
EiTask::is_signed() const
{
  return false;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiTask::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
int
EiTask::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
int
EiTask::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiTask::left_range_string() const
{
  return {};
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiTask::right_range_string() const
{
  return {};
}

// @brief 出力のビット幅を返す．
SizeType
EiTask::bit_size() const
{
  return 0;
}

// @brief 出力変数をセットする．
void
EiTask::set_ovar(
  ElbDecl* ovar
)
{
  ASSERT_NOT_REACHED;
}

// @brief constant function の時に true を返す．
bool
EiTask::is_constant_function() const
{
  ASSERT_NOT_REACHED;
  return false;
}

// @brief 出力変数を返す．
//
// 出力変数とは関数名と同名の変数
VlDecl*
EiTask::ovar() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunction
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiFunction::type() const
{
  return VpiObjType::Function;
}

// @brief function type を返す．
VpiFuncType
EiFunction::func_type() const
{
  switch ( ast_item().data_type() ) {
  case VpiVarType::None:
    return VpiFuncType::Sized;

  case VpiVarType::Integer:
    return VpiFuncType::Int;

  case VpiVarType::Real:
    return VpiFuncType::Real;

  case VpiVarType::Time:
    return VpiFuncType::Time;

  case VpiVarType::Realtime:
    return VpiFuncType::Realtime;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return VpiFuncType::Int;
}

// @brief 符号付きの時 true を返す．
bool
EiFunction::is_signed() const
{
  return ast_item().is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiFunction::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
int
EiFunction::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
int
EiFunction::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiFunction::left_range_string() const
{
  return std::string();
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiFunction::right_range_string() const
{
  return std::string();
}

// @brief 出力のビット幅を返す．
SizeType
EiFunction::bit_size() const
{
  switch ( ast_item().data_type() ) {
  case VpiVarType::None:
    return 1;

  case VpiVarType::Integer:
    return kVpiSizeInteger;

  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return kVpiSizeReal;

  case VpiVarType::Time:
    return kVpiSizeTime;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief 出力変数をセットする．
void
EiFunction::set_ovar(
  ElbDecl* ovar
)
{
  mOvar = ovar;
}

// @brief constant function の時に true を返す．
bool
EiFunction::is_constant_function() const
{
  return mConstFunc;
}

// @brief 出力変数を返す．
//
// 出力変数とは関数名と同名の変数
VlDecl*
EiFunction::ovar() const
{
  return mOvar;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunctionV
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定を持つとき true を返す．
bool
EiFunctionV::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
int
EiFunctionV::left_range_val() const
{
  return mRange.left;
}

// @brief 範囲の LSB の値を返す．
int
EiFunctionV::right_range_val() const
{
  return mRange.right;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiFunctionV::left_range_string() const
{
  return mRange.left_string();
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiFunctionV::right_range_string() const
{
  return mRange.right_string();
}

// @brief 出力のビット幅を返す．
SizeType
EiFunctionV::bit_size() const
{
  return mRange.calc_size();
}

END_NAMESPACE_YM_VERILOG
