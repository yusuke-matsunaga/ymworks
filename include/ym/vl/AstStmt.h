#ifndef YM_VL_ASTSTMT_H
#define YM_VL_ASTSTMT_H

/// @file ym/vl/AstStmt.h
/// @brief AstStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstStmt AstStmt.h "ym/vl/AstStmt.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief statement の共通の親クラス
///
/// 実際には様々な型を一つのでクラスで表しているので型によっては
/// 使用できない関数がある．
/// その条件が複雑なので注意が必要
//////////////////////////////////////////////////////////////////////
class AstStmt :
  public AstHierNamedBase
{
  friend class AstList<const AstStmt>;
  friend class AstListIter<const AstStmt>;

public:

  /// @brief AstStmt の派生クラスの型を表す列挙型
  enum Type : std::uint8_t {
    Disable,
    Enable,
    SysEnable,
    DelayControl,
    EventControl,
    Assign,
    NbAssign,
    Event,
    Null,
    If,
    Case,
    CaseX,
    CaseZ,
    Wait,
    Forever,
    Repeat,
    While,
    For,
    PcAssign,
    Deassign,
    Force,
    Release,
    ParBlock,
    SeqBlock,
    NamedParBlock,
    NamedSeqBlock
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型の取得
  /// @return 型
  virtual
  Type
  type() const = 0;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  virtual
  const char*
  stmt_name() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 複数の型で有効な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  ///
  /// - type() != DelayControl|EventControl|Forever|Repeat|While|For|If
  //    の時 std::logic_error 例外を送出する．
  virtual
  const AstStmt*
  body() const = 0;

  /// @brief 式の取得
  /// @return 式
  ///
  /// - type() != Wait|Repeat|While|For|If|Case|CaseX|CaseZ の時
  ///   std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  expr() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Enable の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数のリストの取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  virtual
  AstExprList
  arg_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Delay/Event/Repeat Control の関数
  // および代入系でも用いられる．
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  ///
  /// - type() != DelayControl|EventControl|Assign|NbAssign の時 std::logic_error 例外を送出する．
  virtual
  const AstControl*
  control() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Assign 系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式の取得
  /// @return 左辺式
  ///
  /// - type() != Assign|NbAssign|Force|PcAssign|Release|Deassign の時
  ///   std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @return 右辺式
  ///
  /// - type() != Assign|NbAssign|Force|PcAssign の時 std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  rhs() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // EventStmt の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  ///
  /// - type() != EventStmt の時 std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  primary() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // If の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  ///
  /// - type() != If の時 std::logic_error 例外を送出する．
  virtual
  const AstStmt*
  else_body() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Case 系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief case item のリストの取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  virtual
  AstCaseItemList
  caseitem_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // For の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  ///
  /// - type() != For の時 std::logic_error 例外を送出する．
  virtual
  const AstStmt*
  init_stmt() const = 0;

  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  ///
  /// - type() != For の時 std::logic_error 例外を送出する．
  virtual
  const AstStmt*
  next_stmt() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Block Stmt 系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言ヘッダのリストの取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  virtual
  AstDeclHeadList
  declhead_list() const = 0;

  /// @brief 子供のステートメントのリストの取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  virtual
  AstStmtList
  stmt_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を JsonValue に変換する．
  JsonValue
  json_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstStmt*
  link() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstCaseItem AstStmt.h "ym/vl/AstStmt.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class AstCaseItem :
  public AstBase
{
  friend class AstList<const AstCaseItem>;
  friend class AstListIter<const AstCaseItem>;

public:
  //////////////////////////////////////////////////////////////////////
  // AstCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルリストの取得
  virtual
  AstExprList
  label_list() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const AstStmt*
  body() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を JsonValue に変換する．
  JsonValue
  json_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstCaseItem*
  link() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTSTMT_H
