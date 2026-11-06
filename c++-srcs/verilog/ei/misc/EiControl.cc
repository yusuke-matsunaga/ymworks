
/// @file EiControl.cc
/// @brief EiControl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiDelayControl.h"
#include "EiEventControl.h"
#include "EiRepeatControl.h"

#include "elaborator/ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief 遅延コントロールを生成する．
const VlControl*
ElbMgr::new_DelayControl(
  const AstControl& ast_control,
  ElbExpr* delay
)
{
  auto control = new EiDelayControl(ast_control, delay);
  reg_obj(control);
  return control;
}

// @brief イベントコントロールを生成する．
const VlControl*
ElbMgr::new_EventControl(
  const AstControl& ast_control,
  const std::vector<ElbExpr*>& event_list
)
{
  auto control = new EiEventControl(ast_control, event_list);
  reg_obj(control);
  return control;
}

// @brief リピートコントロールを生成する．
const VlControl*
ElbMgr::new_RepeatControl(
  const AstControl& ast_control,
  ElbExpr* rep,
  const std::vector<ElbExpr*>& event_list
)
{
  auto control = new EiRepeatControl(ast_control, rep, event_list);
  reg_obj(control);
  return control;
}


//////////////////////////////////////////////////////////////////////
// クラス EiControl
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置を返す．
FileRegion
EiControl::file_region() const
{
  return mAstControl.file_region();
}

// @brief 遅延式を返す．
const VlExpr*
EiControl::delay() const
{
  return nullptr;
}

// @brief 繰り返し式を返す．
const VlExpr*
EiControl::expr() const
{
  return nullptr;
}

// @brief イベント条件式の数を返す．
SizeType
EiControl::event_num() const
{
  return 0;
}

// @brief イベント条件式を返す．
const VlExpr*
EiControl::event(
  SizeType pos
) const
{
  return nullptr;
}

// @brief イベント条件式のリストを返す．
std::vector<const VlExpr*>
EiControl::event_list() const
{
  return {};
}


//////////////////////////////////////////////////////////////////////
/// クラス EiDelayControl
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiDelayControl::type() const
{
  return VpiObjType::DelayControl;
}

// 遅延式を返す．
const VlExpr*
EiDelayControl::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiEventControl EiControl.h "EiControl.h"
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiEventControl::type() const
{
  return VpiObjType::EventControl;
}

// @brief イベント条件式の数を返す．
SizeType
EiEventControl::event_num() const
{
  return mEventList.size();
}

// @brief イベント条件式を返す．
const VlExpr*
EiEventControl::event(
  SizeType pos
) const
{
  if ( pos >= event_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mEventList[pos];
}

// @brief イベント条件式のリストを返す．
std::vector<const VlExpr*>
EiEventControl::event_list() const
{
  return std::vector<const VlExpr*>{mEventList.begin(), mEventList.end()};
}


//////////////////////////////////////////////////////////////////////
/// クラス EiRepeatControl EiControl.h "EiControl.h"
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiRepeatControl::type() const
{
  return VpiObjType::RepeatControl;
}

// 繰り返し式を返す．
const VlExpr*
EiRepeatControl::expr() const
{
  return mExpr;
}

END_NAMESPACE_YM_VERILOG
