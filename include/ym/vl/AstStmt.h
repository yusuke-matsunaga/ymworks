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
  // 外部インターフェイス
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

  /// @brief 引数の数の取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  virtual
  SizeType
  arg_num() const = 0;

  /// @brief 引数の取得
  ///
  /// - pos >= arg_num() の時 std::out_of_range 例外を送出する．
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  arg(
    SizeType pos ///< [in] 位置 ( 0 <= pos < arg_num() )
  ) const = 0;

  /// @brief 引数のリストの取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  std::vector<const AstExpr*>
  arg_list() const
  {
    auto n = arg_num();
    std::vector<const AstExpr*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(arg(i));
    }
    return vec;
  }

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  ///
  /// - type() != DelayControl|EventControl の時 std::logic_error 例外を送出する．
  virtual
  const AstControl*
  control() const = 0;

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

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  ///
  /// - type() != EventControl の時 std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  primary() const = 0;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  ///
  /// - type() != If の時 std::logic_error 例外を送出する．
  virtual
  const AstStmt*
  else_body() const = 0;

  /// @brief case item のリストの要素数の取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  virtual
  SizeType
  caseitem_num() const = 0;

  /// @brief case item の取得
  ///
  /// - pos >= caseitem_num() の時 std::out_of_range 例外を送出する．
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  virtual
  const AstCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const = 0;

  /// @brief case item のリストの取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  std::vector<const AstCaseItem*>
  caseitem_list() const
  {
    auto n = caseitem_num();
    std::vector<const AstCaseItem*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(caseitem(i));
    }
    return vec;
  }

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

  /// @brief 宣言ヘッダ配列の要素数の取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  virtual
  SizeType
  declhead_num() const = 0;

  /// @brief 宣言ヘッダの取得
  ///
  /// - pos >= declhead_num() の時 std::out_of_range 例外を送出する．
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  virtual
  const AstDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const = 0;

  /// @brief 宣言ヘッダのリストの取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  std::vector<const AstDeclHead*>
  declhead_list() const
  {
    auto n = declhead_num();
    std::vector<const AstDeclHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(declhead(i));
    }
    return vec;
  }

  /// @brief 子供のステートメント配列の要素数の取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  virtual
  SizeType
  stmt_num() const = 0;

  /// @brief 子供のステートメントの取得
  /// @note kParBlock/kSeqBlock で意味のある関数
  ///
  /// - pos >= stmt_num() の時 std::out_of_range 例外を送出する．
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  virtual
  const AstStmt*
  stmt(
    SizeType pos ///< [in] 位置 ( 0 <= pos < stmt_num() )
  ) const = 0;

  /// @brief 子供のステートメントのリストの取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  std::vector<const AstStmt*>
  stmt_list() const
  {
    auto n = stmt_num();
    std::vector<const AstStmt*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(stmt(i));
    }
    return vec;
  }

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
  ///
  /// - pos >= label_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstExpr*
  label(
    SizeType pos ///< [in] 位置 ( 0 <= pos < label_num() )
  ) const = 0;

  /// @brief ラベルリストの取得
  std::vector<const AstExpr*>
  label_list() const
  {
    auto n = label_num();
    std::vector<const AstExpr*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(label(i));
    }
    return vec;
  }

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const AstStmt*
  body() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTSTMT_H
