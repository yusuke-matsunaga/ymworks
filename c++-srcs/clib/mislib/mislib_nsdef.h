#ifndef MISLIB_NSDEF_H
#define MISLIB_NSDEF_H

/// @file mislib_nsdef.h
/// @brief mislib サブモジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ShString.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief mislib の名前空間の開始
#define BEGIN_NAMESPACE_YM_MISLIB \
BEGIN_NAMESPACE_YM_CLIB \
BEGIN_NAMESPACE(nsMislib)

/// @brief mislib の名前空間の終了
#define END_NAMESPACE_YM_MISLIB \
END_NAMESPACE(nsMislib) \
END_NAMESPACE_YM_CLIB


BEGIN_NAMESPACE_YM_MISLIB

// クラス名の前方宣言
class MislibMgr;
class MislibParser;
class MislibNode;
class MislibStr;
class MislibNum;
class MislibExpr;
class MislibPhase;
class MislibPin;
class MislibGate;

using MislibGatePtr = std::unique_ptr<const MislibGate>;
using MislibExprPtr = std::unique_ptr<const MislibExpr>;
using MislibNumPtr = std::unique_ptr<const MislibNum>;
using MislibPhasePtr = std::unique_ptr<const MislibPhase>;
using MislibPinPtr = std::unique_ptr<const MislibPin>;
using MislibStrPtr = std::unique_ptr<const MislibStr>;

using MislibNameMap = std::unordered_map<ShString, SizeType>;

END_NAMESPACE_YM_MISLIB

#endif // MISLIB_NSDEF_H
