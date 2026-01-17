#ifndef YM_LOGIC_H
#define YM_LOGIC_H

/// @file ym/logic.h
/// @brief ym-logic パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup LogicGroup 論理式と論理関数を扱うクラス
///
/// 論理式(ファクタードフォーム)と論理関数を扱うためのクラスライブラリ
/// C++ の機能を活かして論理演算子のオーバーロードを使っている．
///
/// 主なクラスは以下の通り
/// - Literal: 変数リテラル(変数番号＋極性)
/// - Expr: 通常の論理式
/// - TvFunc: 真理値表を用いた論理関数表現
/// - Bdd: Binary Decision Diagram
/// - Zdd: Zero-surpressed Decision Diagram
/// - SopCover, SopCube: 積和形論理式とキューブ

/// @defgroup ExprGroup 論理式を扱うクラス
/// @ingroup LogicGroup
///

/// @defgroup TvGroup 真理値表を扱うクラス
/// @ingroup LogicGroup
///

/// @defgroup SopGroup 積和形論理式を扱うクラス
/// @ingroup LogicGroup
///

/// @defgroup NpnGroup NPN変換を扱うクラス
/// @ingroup LogicGroup
///

/// @defgroup BcGroup Boolean Chain を扱うクラス
/// @ingroup LogicGroup
///


#include "ym_config.h"

/// @brief ym-logic 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LOGIC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLogic)

/// @brief expr 用の名前空間の終了
#define END_NAMESPACE_YM_LOGIC \
END_NAMESPACE(nsLogic) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_LOGIC

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class Expr;
class TvFunc;
class TvFuncM;
class NpnVmap;
class NpnMap;
class NpnMapM;

END_NAMESPACE_YM_LOGIC

/// @brief RectCover 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_RC \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsRectCover)

/// @brief RectCover 用の名前空間の終了
#define END_NAMESPACE_YM_RC \
END_NAMESPACE(nsRectCover) \
END_NAMESPACE_YM

BEGIN_NAMESPACE_YM

using nsLogic::Expr;
using nsLogic::TvFunc;
using nsLogic::TvFuncM;
using nsLogic::NpnVmap;
using nsLogic::NpnMap;
using nsLogic::NpnMapM;

END_NAMESPACE_YM

#endif // YM_LOGIC_H
