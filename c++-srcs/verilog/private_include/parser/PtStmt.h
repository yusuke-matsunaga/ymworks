#ifndef PARSER_PTSTMT_H
#define PARSER_PTSTMT_H

/// @file PtStmt.h
/// @brief PtStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"
#include "ym/vl/AstStmt.h" // for AstStmt::Type


BEGIN_NAMESPACE_YM_VERILOG

class PtCaseItem;
class PtControl;
class PtDeclHead;
class PtExpr;
class PtNameBranch;

//////////////////////////////////////////////////////////////////////
/// @class PtStmt PtStmt.h "parser/PtStmt.h"
/// @brief AstStmt の実装クラス
//////////////////////////////////////////////////////////////////////
class PtStmt :
  public PtListElem<PtStmt>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 階層ブランチのリストの先頭を返す．
  virtual
  const PtNameBranch*
  namebranch_top() const = 0;

  /// @brief クラスの型の取得
  /// @return 型
  virtual
  AstStmt::Type
  type() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  ///
  /// - type() != DelayControl|EventControl|Forever|Repeat|While|For|If
  //    の時 std::logic_error 例外を送出する．
  virtual
  const PtStmt*
  body() const = 0;

  /// @brief 式の取得
  /// @return 式
  ///
  /// - type() != Wait|Repeat|While|For|If|Case|CaseX|CaseZ の時
  ///   std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  expr() const = 0;

  /// @brief 引数のリストのトップの取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  arg_top() const = 0;

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  ///
  /// - type() != DelayControl|EventControl|Assign|NbAssign の時
  ///   std::logic_error 例外を送出する．
  virtual
  const PtControl*
  control() const = 0;

  /// @brief 左辺式の取得
  /// @return 左辺式
  ///
  /// - type() != Assign|NbAssign|Force|PcAssign|Release|Deassign の時
  ///   std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @return 右辺式
  ///
  /// - type() != Assign|NbAssign|Force|PcAssign の時 std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  rhs() const = 0;

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  ///
  /// - type() != EventStmt の時 std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  primary() const = 0;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  ///
  /// - type() != If の時 std::logic_error 例外を送出する．
  virtual
  const PtStmt*
  else_body() const = 0;

  /// @brief case item のリストの先頭の取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  virtual
  const PtCaseItem*
  caseitem_top() const = 0;

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  ///
  /// - type() != For の時 std::logic_error 例外を送出する．
  virtual
  const PtStmt*
  init_stmt() const = 0;

  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  ///
  /// - type() != For の時 std::logic_error 例外を送出する．
  virtual
  const PtStmt*
  next_stmt() const = 0;

  /// @brief 宣言ヘッダのリストの先頭の取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  virtual
  const PtDeclHead*
  declhead_top() const = 0;

  /// @brief 子供のステートメントのリストの先頭の取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  virtual
  const PtStmt*
  stmt_top() const = 0;

};

/// @brief PtStmt のリスト
using PtStmtList = PtList<PtStmt>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTSTMT_H
