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

class PtStmt;

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
  public AstHierNamedBaseWithPtr<const PtStmt>
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

  /// @brief コンストラクタ
  explicit
  AstStmt(
    const PtStmt* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstHierNamedBaseWithPtr<const PtStmt>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型の取得
  /// @return 型
  Type
  type() const;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  const char*
  stmt_name() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 複数の型で有効な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  ///
  /// - type() != DelayControl|EventControl|Forever|Repeat|While|For|If
  //    の時 std::logic_error 例外を送出する．
  AstStmt
  body() const;

  /// @brief 式の取得
  /// @return 式
  ///
  /// - type() != Wait|Repeat|While|For|If|Case|CaseX|CaseZ の時
  ///   std::logic_error 例外を送出する．
  AstExpr
  expr() const;


public:
  //////////////////////////////////////////////////////////////////////
  // Enable の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数のリストの取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  AstExprList
  arg_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // Delay/Event/Repeat Control の関数
  // および代入系でも用いられる．
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  ///
  /// - type() != DelayControl|EventControl|Assign|NbAssign の時
  ///   std::logic_error 例外を送出する．
  AstControl
  control() const;


public:
  //////////////////////////////////////////////////////////////////////
  // Assign 系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式の取得
  /// @return 左辺式
  ///
  /// - type() != Assign|NbAssign|Force|PcAssign|Release|Deassign の時
  ///   std::logic_error 例外を送出する．
  AstExpr
  lhs() const;

  /// @brief 右辺式の取得
  /// @return 右辺式
  ///
  /// - type() != Assign|NbAssign|Force|PcAssign の時 std::logic_error 例外を送出する．
  AstExpr
  rhs() const;


public:
  //////////////////////////////////////////////////////////////////////
  // EventStmt の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  ///
  /// - type() != EventStmt の時 std::logic_error 例外を送出する．
  AstExpr
  primary() const;


public:
  //////////////////////////////////////////////////////////////////////
  // If の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  ///
  /// - type() != If の時 std::logic_error 例外を送出する．
  AstStmt
  else_body() const;


public:
  //////////////////////////////////////////////////////////////////////
  // Case 系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief case item のリストの取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  AstCaseItemList
  caseitem_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // For の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  ///
  /// - type() != For の時 std::logic_error 例外を送出する．
  AstStmt
  init_stmt() const;

  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  ///
  /// - type() != For の時 std::logic_error 例外を送出する．
  AstStmt
  next_stmt() const;


public:
  //////////////////////////////////////////////////////////////////////
  // Block Stmt 系の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言ヘッダのリストの取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  AstDeclHeadList
  declhead_list() const;

  /// @brief 子供のステートメントのリストの取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  AstStmtList
  stmt_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const override;

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 比較用のユニークなキーを返す．
  PtrIntType
  key() const override;


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
  // AstHierNamedBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstStmt
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief json_obj() の下請け関数
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const override;

  /// @brief Disable 用の json_sub()
  void
  json_sub_disable(
    JsonValue& jobj
  ) const;

  /// @brief Enable/SysEnable 用の json_sub()
  void
  json_sub_enable(
    JsonValue& jobj,
    const char* label
  ) const;

  /// @brief EventControl/DelayControl 用の json_sub()
  void
  json_sub_control(
    JsonValue& jobj,
    const char* label
  ) const;

  /// @brief Assign/NbAssign 用の json_sub()
  void
  json_sub_assign(
    JsonValue& jobj,
    const char* label
  ) const;

  /// @brief Event 用の json_sub()
  void
  json_sub_event(
    JsonValue& jobj
  ) const;

  /// @brief Null 用の json_sub()
  void
  json_sub_null(
    JsonValue& jobj
  ) const;

  /// @brief If 用の json_sub()
  void
  json_sub_if(
    JsonValue& jobj
  ) const;

  /// @brief Case 用の json_sub()
  void
  json_sub_case(
    JsonValue& jobj,
    const char* label
  ) const;

  /// @brief Wait 用の json_sub()
  void
  json_sub_wait(
    JsonValue& jobj
  ) const;

  /// @brief Forever 用の json_sub()
  void
  json_sub_forever(
    JsonValue& jobj
  ) const;

  /// @brief Repeat 用の json_sub()
  void
  json_sub_repeat(
    JsonValue& jobj
  ) const;

  /// @brief While 用の json_sub()
  void
  json_sub_while(
    JsonValue& jobj
  ) const;

  /// @brief For 用の json_sub()
  void
  json_sub_for(
    JsonValue& jobj
  ) const;

  /// @brief PcAssign/Force 用の json_sub()
  void
  json_sub_pcassign(
    JsonValue& jobj,
    const char* label
  ) const;

  /// @brief Deassign 用の json_sub()
  void
  json_sub_deassign(
    JsonValue& jobj,
    const char* label
  ) const;

  /// @brief ParBlock/SeqBlock 用の json_sub()
  void
  json_sub_block(
    JsonValue& jobj,
    const char* label
  ) const;

  /// @brief NamedParBlock/NamedSeqBlock 用の json_sub()
  void
  json_sub_namedblock(
    JsonValue& jobj,
    const char* label
  ) const;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTSTMT_H
