
/// @file EiIOHead.cc
/// @brief EiIOHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiIOHead.h"
#include "ei/EiFactory.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief module IO ヘッダを生成する．
ElbIOHead*
EiFactory::new_IOHead(
  const VlModule* module,
  const AstIOHead& ast_header
)
{
  return new EiModIOHead(module, ast_header);
}

// @brief タスク/関数 IO ヘッダを生成する．
ElbIOHead*
EiFactory::new_IOHead(
  const VlTaskFunc* taskfunc,
  const AstIOHead& ast_header
)
{
  if ( taskfunc->type() == VpiObjType::Task ) {
    return new EiTaskIOHead(taskfunc, ast_header);
  }
  else { // VpiObjType::Function
    return new EiFunctionIOHead(taskfunc, ast_header);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiIOHead::EiIOHead(
  const AstIOHead& ast_header
) : mAstHead{ast_header}
{
}

// @brief デストラクタ
EiIOHead::~EiIOHead()
{
}

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

// @brief コンストラクタ
EiModIOHead::EiModIOHead(
  const VlModule* module,
  const AstIOHead& ast_header
) : EiIOHead(ast_header),
    mModule{module}
{
}

// @brief デストラクタ
EiModIOHead::~EiModIOHead()
{
}

// @brief 親のモジュールの取得
const VlModule*
EiModIOHead::module() const
{
  return mModule;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiTaskIOHead::EiTaskIOHead(
  const VlTaskFunc* task,
  const AstIOHead& ast_header
) : EiIOHead(ast_header),
    mTask{task}
{
}

// @brief デストラクタ
EiTaskIOHead::~EiTaskIOHead()
{
}

// @brief 親のタスクの取得
const VlTaskFunc*
EiTaskIOHead::task() const
{
  return mTask;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFunctionIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFunctionIOHead::EiFunctionIOHead(
  const VlTaskFunc* func,
  const AstIOHead& ast_header
) : EiIOHead(ast_header),
    mFunction{func}
{
}

// @brief デストラクタ
EiFunctionIOHead::~EiFunctionIOHead()
{
}

// @brief 親の関数の取得
const VlTaskFunc*
EiFunctionIOHead::function() const
{
  return mFunction;
}

END_NAMESPACE_YM_VERILOG
