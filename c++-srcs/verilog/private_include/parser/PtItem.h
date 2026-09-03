#ifndef PTITEM_H
#define PTITEM_H

/// @file PtItem.h
/// @brief PtItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"
#include "ym/vl/AstItem.h" // for AstItem::Type


BEGIN_NAMESPACE_YM_VERILOG

class PtConnection;
class PtContAssign;
class PtDeclHead;
class PtDefParam;
class PtDelay;
class PtExpr;
class PtGenCaseItem;
class PtInst;
class PtIOHead;
class PtPathDecl;
class PtRange;
class PtStmt;
class PtStrength;

//////////////////////////////////////////////////////////////////////
/// @class PtItem PtItem.h "parser/PtItem.h"
/// @brief AstItem の実装クラス
//////////////////////////////////////////////////////////////////////
class PtItem :
  public PtListElem<PtItem>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 型の取得
  /// @return 型
  virtual
  AstItem::Type
  type() const = 0;

  /// @brief 条件式の取得
  /// @return 条件式
  virtual
  const PtExpr*
  cond_expr() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const PtStmt*
  body() const = 0;

  /// @brief strength の取得
  /// @return 信号強度
  virtual
  const PtStrength*
  strength() const = 0;

  /// @brief delay の取得
  /// @return 遅延
  virtual
  const PtDelay*
  delay() const = 0;

  /// @brief defparam リストの取得
  virtual
  const PtDefParam*
  defparam_top() const = 0;

  /// @brief continuous assign リストの取得
  virtual
  const PtContAssign*
  contassign_top() const = 0;

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief パラメータ割り当てのリストの取得
  virtual
  const PtConnection*
  paramassign_top() const = 0;

  /// @brief module/UDP/gate instance リストの取得
  virtual
  const PtInst*
  inst_top() const = 0;

  /// @brief 宣言ヘッダリストの取得
  virtual
  const PtDeclHead*
  declhead_top() const = 0;

  /// @brief item リストの取得
  virtual
  const PtItem*
  item_top() const = 0;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  virtual
  bool
  automatic() const = 0;

  /// @brief IO宣言ヘッダリストの取得
  virtual
  const PtIOHead*
  iohead_top() const = 0;

  /// @brief IO要素の数
  virtual
  SizeType
  ioitem_num() const = 0;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲の取得
  /// @return 範囲
  virtual
  const PtRange*
  range() const = 0;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  virtual
  VpiVarType
  data_type() const = 0;

  /// @brief constant function の展開中の印をつける．
  virtual
  void
  set_in_use() const = 0;

  /// @brief constant function の展開中の印を消す．
  virtual
  void
  clear_in_use() const = 0;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  virtual
  bool
  is_in_use() const = 0;

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  virtual
  VpiSpecItemType
  specitem_type() const = 0;

  /// @brief ターミナルリストの取得
  virtual
  const PtExpr*
  terminal_top() const = 0;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  virtual
  VpiSpecPathType
  specpath_type() const = 0;

  /// @brief パス記述の取得
  /// @return パス記述
  virtual
  const PtPathDecl*
  path_decl() const = 0;

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
  virtual
  const PtDeclHead*
  then_declhead_top() const = 0;

  /// @brief 条件が成り立った時に生成される要素リストの取得
  virtual
  const PtItem*
  then_item_top() const = 0;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  virtual
  const PtDeclHead*
  else_declhead_top() const = 0;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  virtual
  const PtItem*
  else_item_top() const = 0;

  /// @brief case item リストの取得
  virtual
  const PtGenCaseItem*
  caseitem_top() const = 0;

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  virtual
  const char*
  loop_var() const = 0;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  virtual
  const PtExpr*
  init_expr() const = 0;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  virtual
  const PtExpr*
  next_expr() const = 0;

};

/// @brief PtItem のリスト
using PtItemList = PtList<PtItem>;

END_NAMESPACE_YM_VERILOG

#endif // PTITEM_H
