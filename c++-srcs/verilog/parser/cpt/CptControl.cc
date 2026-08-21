
/// @file CptControl.cc
/// @brief CptControl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptControl.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief  遅延式の取得
const AstExpr*
CptControl::delay() const
{
  throw std::logic_error{"Not an Delay control type"};
}

// @brief イベントリストの要素数の取得
SizeType
CptControl::event_num() const
{
  throw std::logic_error{"Not an Eent|Repeat control type"};
}

// @brief イベントの取得
const AstExpr*
CptControl::event(
  SizeType index
) const
{
  throw std::logic_error{"Not an Eent|Repeat control type"};
}

// @brief イベントリストの取得
AstExprVec
CptControl::event_list() const
{
  throw std::logic_error{"Not an Eent|Repeat control type"};
}

// @brief 繰り返し数の取得
const AstExpr*
CptControl::rep_expr() const
{
  throw std::logic_error{"Not a Repeat control type"};
}


//////////////////////////////////////////////////////////////////////
// delay control を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptDelayControl::file_region() const
{
  return FileRegion{mTopLoc, mDelay->file_region().end_loc()};
}

// 型を返す．
AstControl::Type
CptDelayControl::type() const
{
  return Delay;
}

// 遅延式を返す．
const AstExpr*
CptDelayControl::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス CptEventControl
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptEventControl::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstControl::Type
CptEventControl::type() const
{
  return Event;
}

// @brief イベントリストの要素数の取得
SizeType
CptEventControl::event_num() const
{
  return 0;
}

// @brief イベントの取得
const AstExpr*
CptEventControl::event(
  SizeType index
) const
{
  throw std::out_of_range{"event(index): index is out of range"};
}

// @brief イベントリストの取得
AstExprVec
CptEventControl::event_list() const
{
  return {};
}


//////////////////////////////////////////////////////////////////////
// クラス CptEventControl1
//////////////////////////////////////////////////////////////////////

// @brief イベントリストの要素数の取得
SizeType
CptEventControl1::event_num() const
{
  return 1;
}

// @brief イベントの取得
const AstExpr*
CptEventControl1::event(
  SizeType index
) const
{
  if ( index >0 ) {
    throw std::out_of_range{"event(index): index is out of range"};
  }
  return mEvent;
}

// @brief イベントリストの取得
AstExprVec
CptEventControl1::event_list() const
{
  return {mEvent};
}


//////////////////////////////////////////////////////////////////////
// クラス CptEventControl2
//////////////////////////////////////////////////////////////////////

// @brief イベントリストの要素数の取得
SizeType
CptEventControl2::event_num() const
{
  return mEventList.size();
}

// @brief イベントの取得
const AstExpr*
CptEventControl2::event(
  SizeType index
) const
{
  return mEventList[index];
}

// @brief イベントリストの取得
AstExprVec
CptEventControl2::event_list() const
{
  return mEventList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstControl::Type
CptRepeatControl::type() const
{
  return Repeat;
}

// 繰り返し数を得る．
const AstExpr*
CptRepeatControl::rep_expr() const
{
  return mRepExpr;
}


//////////////////////////////////////////////////////////////////////
// repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstControl::Type
CptRepeatControl1::type() const
{
  return Repeat;
}

// 繰り返し数を得る．
const AstExpr*
CptRepeatControl1::rep_expr() const
{
  return mRepExpr;
}


//////////////////////////////////////////////////////////////////////
// repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstControl::Type
CptRepeatControl2::type() const
{
  return Repeat;
}

// 繰り返し数を得る．
const AstExpr*
CptRepeatControl2::rep_expr() const
{
  return mRepExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// ディレイコントロールを生成する．
PtControl*
PtFactory::new_DelayControl(
  const FileRegion& file_region,
  const AstExpr* value
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelayControl));
  return new (p) CptDelayControl(file_region, value);
}

// イベントコントロールを生成する．
PtControl*
PtFactory::new_EventControl(
  const FileRegion& file_region
)
{
  void* p = mAlloc.get_memory(sizeof(CptEventControl));
  return new (p) CptEventControl(file_region);
}

// イベントコントロールを生成する．
PtControl*
PtFactory::new_EventControl(
  const FileRegion& file_region,
  const AstExpr* event
)
{
  void* p = mAlloc.get_memory(sizeof(CptEventControl1));
  return new (p) CptEventControl1(file_region, event);
}

// イベントコントロールを生成する．
PtControl*
PtFactory::new_EventControl(
  const FileRegion& file_region,
  PtExprList* event_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptEventControl2));
  return new (p) CptEventControl2(file_region,
				  event_list->to_array(mAlloc));
}

// リピートコントロールを生成する．
PtControl*
PtFactory::new_RepeatControl(
  const FileRegion& file_region,
  const AstExpr* rep
)
{
  void* p = mAlloc.get_memory(sizeof(CptRepeatControl));
  return new (p) CptRepeatControl(file_region, rep);
}

// リピートコントロールを生成する．
PtControl*
PtFactory::new_RepeatControl(
  const FileRegion& file_region,
  const AstExpr* rep,
  const AstExpr* event
)
{
  void* p = mAlloc.get_memory(sizeof(CptRepeatControl1));
  return new (p) CptRepeatControl1(file_region, rep, event);
}

// リピートコントロールを生成する．
PtControl*
PtFactory::new_RepeatControl(
  const FileRegion& file_region,
  const AstExpr* rep,
  PtExprList* event_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptRepeatControl2));
  return new (p) CptRepeatControl2(file_region, rep,
				   event_list->to_array(mAlloc));
}

END_NAMESPACE_YM_VERILOG
