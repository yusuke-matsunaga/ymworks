
/// @file CptMisc.cc
/// @brief CptMisc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptMisc.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "parser/PtExpr.h"
#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptControl::CptControl()
{
}

// デストラクタ
CptControl::~CptControl()
{
}

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

// @brief イベントリストの要素の取得
const AstExpr*
CptControl::event(
  SizeType pos
) const
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
// event を表すクラス
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
  return mEventArray.size();
}

// @brief イベントリストの要素の取得
const AstExpr*
CptEventControl::event(
  SizeType pos
) const
{
  return mEventArray[pos];
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
// ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を取出す．
FileRegion
CptConnection::file_region() const
{
  return mFileRegion;
}

// @brief 名前の取得
const char*
CptConnection::name() const
{
  return nullptr;
}

// 式を取出す
const AstExpr*
CptConnection::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス NamedCon
//////////////////////////////////////////////////////////////////////

// 名前を取り出す．
const char*
CptNamedCon::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStrength
//////////////////////////////////////////////////////////////////////

// ファイル位置を取出す．
FileRegion
CptStrength::file_region() const
{
  return mFileRegion;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStrength1
//////////////////////////////////////////////////////////////////////

// drive strength0 を返す．
VpiStrength
CptStrength1::drive0() const
{
  return mValue[0];
}

// drive strength1 を返す．
VpiStrength
CptStrength1::drive1() const
{
  return mValue[1];
}

// charge strength を返す．
VpiStrength
CptStrength1::charge() const
{
  return VpiStrength::NoStrength;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStrength2
//////////////////////////////////////////////////////////////////////

// drive strength0 を返す．
VpiStrength
CptStrength2::drive0() const
{
  return VpiStrength::NoStrength;
}

// drive strength1 を返す．
VpiStrength
CptStrength2::drive1() const
{
  return VpiStrength::NoStrength;
}

// charge strength を返す．
VpiStrength
CptStrength2::charge() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDelay1
//////////////////////////////////////////////////////////////////////

// ファイル位置を取出す．
FileRegion
CptDelay1::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value0() const
{
  return mValue;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value1() const
{
  return nullptr;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value2() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDelay2
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptDelay2::value1() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDelay3
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptDelay3::value2() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 階層名を表すクラス
//////////////////////////////////////////////////////////////////////

// 名前を取り出す．
const char*
CptNameBranch::name() const
{
  return mName;
}

// @brief インデックスの有無のチェック
bool
CptNameBranch::has_index() const
{
  return false;
}

// インデックスの取得
int
CptNameBranch::index() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// インデックスつきの階層名を表すクラス
//////////////////////////////////////////////////////////////////////

// インデックスを持っている時 true を返す．
bool
CptNameBranchI::has_index() const
{
  return true;
}

// インデックスを取り出す．
int
CptNameBranchI::index() const
{
  return mIndex;
}


//////////////////////////////////////////////////////////////////////
// attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置を返す．
FileRegion
CptAttrInst::file_region() const
{
  return mFileRegion;
}

// @brief 要素数の取得
SizeType
CptAttrInst::attrspec_num() const
{
  return mAttrSpecArray.size();
}

// @brief 要素の取得
const AstAttrSpec*
CptAttrInst::attrspec(
  SizeType pos
) const
{
  return mAttrSpecArray[pos];
}


//////////////////////////////////////////////////////////////////////
// attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptAttrSpec::file_region() const
{
  return mFileRegion;
}

// 名前を取り出す．
const char*
CptAttrSpec::name() const
{
  return mName;
}

// 式を取り出す．nullptr の場合もある．
const AstExpr*
CptAttrSpec::expr() const
{
  return mExpr;
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
  const FileRegion& file_region,
  PtExprArray&& event_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptEventControl));
  return new (p) CptEventControl(file_region, std::move(event_array));
}

// リピートコントロールを生成する．
PtControl*
PtFactory::new_RepeatControl(
  const FileRegion& file_region,
  const AstExpr* expr,
  PtExprArray&& event_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptRepeatControl));
  return new (p) CptRepeatControl(file_region, expr, std::move(event_array));
}

// 順序つき結合子を生成する．
PtConnection*
PtFactory::new_OrderedCon(
  const FileRegion& file_region,
  const AstExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(file_region, expr);
}

// 順序つき結合子を生成する．
PtConnection*
PtFactory::new_OrderedCon(
  const AstExpr* expr
)
{
  FileRegion file_region;
  if ( expr ) {
    file_region = expr->file_region();
  }
  void* p = mAlloc.get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(file_region, expr);
}

// 名前つき結合子を生成する．
PtConnection*
PtFactory::new_NamedCon(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptNamedCon));
  return new (p) CptNamedCon(file_region, name, expr);
}

// drive strength を生成する．
PtStrength*
PtFactory::new_Strength(
  const FileRegion& file_region,
  VpiStrength value1,
  VpiStrength value2
)
{
  void* p = mAlloc.get_memory(sizeof(CptStrength1));
  return new (p) CptStrength1(file_region, value1, value2);
}

// charge strength を生成する．
PtStrength*
PtFactory::new_Strength(
  const FileRegion& file_region,
  VpiStrength value1
)
{
  void* p = mAlloc.get_memory(sizeof(CptStrength2));
  return new (p) CptStrength2(file_region, value1);
}

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay1));
  return new (p) CptDelay1(file_region, value1);
}

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay2));
  return new (p) CptDelay2(file_region, value1, value2);
}

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay3));
  return new (p) CptDelay3(file_region, value1, value2, value3);
}

// 階層名を生成する．
PtNameBranch*
PtFactory::new_NameBranch(
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptNameBranch));
  return new (p) CptNameBranch(name);
}

// 階層名を生成する．
PtNameBranch*
PtFactory::new_NameBranch(
  const char* name,
  int index
)
{
  void* p = mAlloc.get_memory(sizeof(CptNameBranchI));
  return new (p) CptNameBranchI(name, index);
}


//////////////////////////////////////////////////////////////////////
// attribute instance 関係
//////////////////////////////////////////////////////////////////////

// attribute instance を生成する．
PtAttrInst*
PtFactory::new_AttrInst(
  const FileRegion& file_region,
  PtAttrSpecArray&& as_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrInst));
  return new (p) CptAttrInst(file_region, std::move(as_array));
}

// attribute spec を生成する．
PtAttrSpec*
PtFactory::new_AttrSpec(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrSpec));
  return new (p) CptAttrSpec(file_region, name, expr);
}

END_NAMESPACE_YM_VERILOG
