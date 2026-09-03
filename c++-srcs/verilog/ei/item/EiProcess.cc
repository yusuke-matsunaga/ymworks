
/// @file EiProcess.cc
/// @brief EiProcess の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiProcess.h"

#include "ym/vl/AstItem.h"

#include "ym/vl/VlStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief プロセス文を生成する．
ElbProcess*
EiFactory::new_Process(
  const VlScope* parent,
  const AstItem& ast_item
)
{
  auto process = new EiProcess{parent, ast_item};
  return process;
}


//////////////////////////////////////////////////////////////////////
// クラス EiProcess
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiProcess::EiProcess(
  const VlScope* parent,
  const AstItem& ast_item
) : mParent{parent},
    mAstItem{ast_item}
{
}

// @brief デストラクタ
EiProcess::~EiProcess()
{
}

// @brief クラスの型の取得
VpiObjType
EiProcess::type() const
{
  switch ( mAstItem.type() ) {
  case AstItem::Initial: return VpiObjType::Initial;
  case AstItem::Always:  return VpiObjType::Always;
  default: break;
  }
  ASSERT_NOT_REACHED;
  return VpiObjType::Initial;
}

// @brief ファイル位置を返す．
FileRegion
EiProcess::file_region() const
{
  return mAstItem.file_region();
}

// @brief 親のスコープを返す．
const VlScope*
EiProcess::parent_scope() const
{
  return mParent;
}

// @brief 本体のステートメントの取得
const VlStmt*
EiProcess::stmt() const
{
  return mStmt;
}

// @brief 本体のステートメントをセットする．
// @param[in] stmt 本体のステートメント
void
EiProcess::set_stmt(
  const VlStmt* stmt
)
{
  mStmt = stmt;
}

END_NAMESPACE_YM_VERILOG
