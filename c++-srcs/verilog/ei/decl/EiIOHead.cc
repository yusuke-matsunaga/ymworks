
/// @file EiIOHead.cc
/// @brief EiIOHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiIOHead.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief module IO ヘッダを生成する．
ElbIOHead*
ElbMgr::new_IOHead(
  const VlModule* mod,
  const AstIOHead& ast_head
)
{
  return new_iohead(
    [&]() -> ElbIOHead* {
    return new EiModIOHead(mod, ast_head);
  });
}

// @brief タスク/関数用の IO ヘッダを生成する．
ElbIOHead*
ElbMgr::new_IOHead(
  const VlTaskFunc* taskfunc,
  const AstIOHead& ast_head
)
{
  return new_iohead([&]() -> ElbIOHead* {
    if ( taskfunc->type() == VpiObjType::Task ) {
      return new EiTaskIOHead(taskfunc, ast_head);
    }
    else { // VpiObjType::Function
      return new EiFunctionIOHead(taskfunc, ast_head);
    }
  });
}


//////////////////////////////////////////////////////////////////////
// クラス EiIOHead
//////////////////////////////////////////////////////////////////////

// @brief 方向を返す．
VpiDir
EiIOHead::direction() const
{
  return mAstHead.direction();
}

// @brief 親のモジュールの取得
const VlModule*
EiIOHead::module() const
{
  return nullptr;
}

// @brief 親のタスクの取得
const VlTaskFunc*
EiIOHead::task() const
{
  return nullptr;
}

// @brief 親の関数の取得
const VlTaskFunc*
EiIOHead::function() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiModIOHead
//////////////////////////////////////////////////////////////////////

// @brief 親のモジュールの取得
const VlModule*
EiModIOHead::module() const
{
  return mModule;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskIOHead
//////////////////////////////////////////////////////////////////////

// @brief 親のタスクの取得
const VlTaskFunc*
EiTaskIOHead::task() const
{
  return mTask;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunctionIOHead
//////////////////////////////////////////////////////////////////////

// @brief 親の関数の取得
const VlTaskFunc*
EiFunctionIOHead::function() const
{
  return mFunction;
}

END_NAMESPACE_YM_VERILOG
