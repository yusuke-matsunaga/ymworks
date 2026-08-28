#ifndef CPTITEM_H
#define CPTITEM_H

/// @file CptItem.h
/// @brief CptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtItem.h"
#include "parser/PtHierName.h"
#include "ym/FileRegion.h"
#include "ym/vl/AstDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// item の基底クラス
//////////////////////////////////////////////////////////////////////
class CptItem :
  public PtItem
{
protected:

  /// @brief コンストラクタ
  CptItem() {}

  /// @brief デストラクタ
  ~CptItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstNamedBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  const char*
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式の取得
  /// @return 条件式
  const AstExpr*
  cond_expr() const override;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  const AstStmt*
  body() const override;

  /// @brief strength の取得
  /// @return 信号強度
  const AstStrength*
  strength() const override;

  /// @brief delay の取得
  /// @return 遅延
  const AstDelay*
  delay() const override;

  /// @brief defparam リストの取得
  AstDefParamList
  defparam_list() const override;

  /// @brief continuous assign リストの取得
  AstContAssignList
  contassign_list() const override;

  /// @brief パラメータ割り当てのリストの取得
  AstConnectionList
  paramassign_list() const override;

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  VpiPrimType
  prim_type() const override;

  /// @brief module/UDP/gate instance リストの取得
  AstInstList
  inst_list() const override;

  /// @brief 宣言ヘッダリストの取得
  AstDeclHeadList
  declhead_list() const override;

  /// @brief item リストの取得
  AstItemList
  item_list() const override;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  bool
  automatic() const override;

  /// @brief IO宣言ヘッダリストの取得
  AstIOHeadList
  iohead_list() const override;

  /// @brief IO宣言の要素数の取得
  SizeType
  ioitem_num() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲の取得
  /// @return 範囲
  const AstRange*
  range() const override;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  VpiVarType
  data_type() const override;

  /// @brief constant function の展開中の印をつける．
  void
  set_in_use() const override;

  /// @brief constant function の展開中の印を消す．
  void
  clear_in_use() const override;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  bool
  is_in_use() const override;

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  VpiSpecItemType
  specitem_type() const override;

  /// @brief ターミナルリストの取得
  AstExprList
  terminal_list() const override;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  VpiSpecPathType
  specpath_type() const override;

  /// @brief パス記述の取得
  /// @return パス記述
  const AstPathDecl*
  path_decl() const override;

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
  AstDeclHeadList
  then_declhead_list() const override;

  /// @brief 条件が成り立った時に生成されるitemリストの取得
  AstItemList
  then_item_list() const override;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  AstDeclHeadList
  else_declhead_list() const override;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  AstItemList
  else_item_list() const override;

  /// @brief case item リストの取得
  AstGenCaseItemList
  caseitem_list() const override;

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  const char*
  loop_var() const override;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  const AstExpr*
  init_expr() const override;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  const AstExpr*
  next_expr() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_H
