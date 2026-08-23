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

  /// @brief 引数の数の取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  virtual
  SizeType
  arg_num() const = 0;

  /// @brief 引数の取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  arg(
    SizeType index ///< [in] インデックス ( 0 <= index < arg_num() )
  ) const = 0;

  /// @brief 引数のリストの取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  virtual
  AstExprVec
  arg_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Delay/Event/Repeat Control の関数
  // および代入系でも用いられる．
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  ///
  /// - type() != DelayControl|EventControl の時 std::logic_error 例外を送出する．
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
  /// - type() != Assign|Force|PcAssign|Release|Deassign の時
  ///   std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @return 右辺式
  ///
  /// - type() != Assign|Force|PcAssign の時 std::logic_error 例外を送出する．
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

  /// @brief case item のリストの要素数の取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  virtual
  SizeType
  caseitem_num() const = 0;

  /// @brief case item の取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  virtual
  const AstCaseItem*
  caseitem(
    SizeType index ///< [in] インデックス ( 0 <= index < caseitem_num() )
  ) const = 0;

  /// @brief case item のリストの取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  virtual
  AstCaseItemVec
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

  /// @brief 宣言ヘッダ配列の要素数の取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  virtual
  SizeType
  declhead_num() const = 0;

  /// @brief 宣言ヘッダの取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  virtual
  const AstDeclHead*
  declhead(
    SizeType index ///< [in] インデックス ( 0 <= index < declhead_num() )
  ) const = 0;

  /// @brief 宣言ヘッダのリストの取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  virtual
  AstDeclHeadVec
  declhead_list() const = 0;

  /// @brief 子供のステートメントリストの要素数の取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  virtual
  SizeType
  stmt_num() const = 0;

  /// @brief 子供のステートメントの取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  virtual
  const AstStmt*
  stmt(
    SizeType index ///< [in] インデックス ( 0 <= index < stmt_num() )
  ) const = 0;

  /// @brief 子供のステートメントのリストの取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  virtual
  AstStmtVec
  stmt_list() const = 0;

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
public:
  //////////////////////////////////////////////////////////////////////
  // AstCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルのリストの要素数の取得
  virtual
  SizeType
  label_num() const = 0;

  /// @brief ラベルの取得
  virtual
  const AstExpr*
  label(
    SizeType index ///< [in] インデックス ( 0 <= index < label_num() )
  ) const = 0;

  /// @brief ラベルリストの取得
  virtual
  AstExprVec
  label_list() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const AstStmt*
  body() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTSTMT_H
