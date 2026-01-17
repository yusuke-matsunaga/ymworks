#ifndef YM_DD_H
#define YM_DD_H

/// @file ym/dd.h
/// @brief logic/dd サブモジュールの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup DdGroup Decision Diagram を扱うクラス
/// @ingroup LogicGroup
///
/// 具体的には BDD(Binary Decision Diagram) と ZDD(Zero-surpressed Decision Diagram)
/// を扱う．


#include "ym_config.h"


/// @brief DD 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_DD \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsDd)

/// @brief DD 用の名前空間の終了
#define END_NAMESPACE_YM_DD \
END_NAMESPACE(nsDd) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_DD

class Bdd;
class BddVar;
class BddLit;
class BddVarSet;
class BddCube;
class BddMgr;

class Zdd;
class ZddItem;
class ZddMgr;

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_YM

using nsDd::Bdd;
using nsDd::BddVar;
using nsDd::BddLit;
using nsDd::BddVarSet;
using nsDd::BddCube;
using nsDd::BddMgr;

using nsDd::Zdd;
using nsDd::ZddItem;
using nsDd::ZddMgr;

END_NAMESPACE_YM

#endif // YM_LOGIC_H
