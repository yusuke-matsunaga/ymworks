#ifndef PARSER_PTDECLHEAD_H
#define PARSER_PTDECLHEAD_H

/// @file PtDeclHead.h
/// @brief PtDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstDeclHead.h"
#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtDeclItem;
class PtRange;
class PtStrength;
class PtDelay;

//////////////////////////////////////////////////////////////////////
/// @class PtDeclHead PtDeclHead.h "parser/PtDeclHead.h"
/// @brief AstDeclHead の実装クラス
//////////////////////////////////////////////////////////////////////
class PtDeclHead :
  public PtListElem<PtDeclHead>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclHead の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素の型の取得
  /// @return 宣言要素の型
  virtual
  AstDeclHead::Type
  type() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲Bの取得
  ///
  /// 範囲を持たないときは無効なオブエジェクトが返される．
  virtual
  const PtRange*
  range() const = 0;

  /// @brief データ型の取得
  /// @retval データ型 type() = Param, LocalParam, Var の場合
  /// @retval VpiVarType::None 上記以外
  virtual
  VpiVarType
  data_type() const = 0;

  /// @brief ネット型の取得
  /// @retval ネット型 type() = Net の場合
  /// @retval VpiNetType::None ネット型でない場合
  virtual
  VpiNetType
  net_type() const = 0;

  /// @brief vectored|scalared 属性の取得
  /// @retval None vectored|scalared 指定なし
  /// @retval Vectored vectored 指定あり
  /// @retval Scalared scalared 指定あり
  virtual
  VpiVsType
  vs_type() const = 0;

  /// @brief strength の取得
  /// @retval strength
  /// @retval nullptr strength の指定なし
  virtual
  const PtStrength*
  strength() const = 0;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  virtual
  const PtDelay*
  delay() const = 0;

  /// @brief 先頭の要素を返す．
  virtual
  const PtDeclItem*
  item_top() const = 0;

};

/// @brief PtDeclHead のリスト
using PtDeclHeadList = PtList<PtDeclHead>;

/// @brief PtDeclHead と PtDeclItem の二重構造のリスト
using PtDeclDList = PtDList<PtDeclHead, PtDeclItem>;

/// @brief PtDeclHead と PtDeclItem のペア
struct PtDeclHeadItem {
  PtDeclHead* head;
  PtDeclItem* item;
};

END_NAMESPACE_YM_VERILOG

#endif // PTDECLHEAD_H
