#ifndef CPTSTMT_H
#define CPTSTMT_H

/// @file CptStmt.h
/// @brief CptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtStmt.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"
#include "parser/PtHierName.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief statement の共通の親クラス
//////////////////////////////////////////////////////////////////////
class CptStmt :
  public PtStmt
{
public:

  /// @brief コンストラクタ
  CptStmt(
    const FileRegion& file_region
  ) : mFileRegion{file_region}
  {
  }

  /// @brief デストラクタ
  ~CptStmt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;

  /// @brief 名前の取得
  /// @return 名前
  const char*
  name() const override;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  const char*
  stmt_name() const override;

  /// @brief 引数のリストの取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  AstExprList
  arg_list() const override;

  /// @brief コントロールの取得
  /// @return ディレイ/イベントコントロール
  const AstControl*
  control() const override;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  const AstStmt*
  body() const override;

  /// @brief 式の取得
  /// @return 式
  const AstExpr*
  expr() const override;

  /// @brief 左辺式の取得
  /// @return 左辺式
  const AstExpr*
  lhs() const override;

  /// @brief 右辺式の取得
  /// @return 右辺式
  const AstExpr*
  rhs() const override;

  /// @brief イベントプライマリの取得
  /// @return イベントプライマリ
  const AstExpr*
  primary() const override;

  /// @brief 条件が成り立たなかったとき実行されるステートメントの取得
  /// @return 条件が成り立たなかったとき実行されるステートメント
  const AstStmt*
  else_body() const override;

  /// @brief case item のリストの取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  AstCaseItemList
  caseitem_list() const override;

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  const AstStmt*
  init_stmt() const override;

  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  const AstStmt*
  next_stmt() const override;

  /// @brief 宣言ヘッダのリストの取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  AstDeclHeadList
  declhead_list() const override;

  /// @brief 子供のステートメントのリストの取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  AstStmtList
  stmt_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
/// @brief disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDisable :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptDisable(
    const FileRegion& file_region,
    const char* name
  ) : CptStmt(file_region),
    mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptDisable() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層つき名前を持つ disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDisableH :
  public CptDisable
{
public:

  /// @brief コンストラクタ
  CptDisableH(
    const FileRegion& file_region,
    const PtHierName& hname
  ) : CptDisable(file_region, hname.tail_name),
      mNbTop{hname.nb_list.top}
  {
  }

  /// @brief デストラクタ
  ~CptDisableH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチの先頭
  const AstNameBranch* mNbTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief task enable 文 / system task enable 文に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class CptEnableBase :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptEnableBase(
    const FileRegion& file_region,
    const char* name,
    PtExpr* arg_top
  ) : CptStmt(file_region),
      mName{name},
      mArgTop{arg_top}
  {
  }

  /// @brief デストラクタ
  ~CptEnableBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief 引数のリストの取得
  ///
  /// - type() != Enable の時 std::logic_error 例外を送出する．
  AstExprList
  arg_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

  // 引数のリスト
  const AstExpr* mArgTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層なしの名前を持った enable 文
//////////////////////////////////////////////////////////////////////
class CptEnable :
  public CptEnableBase
{
public:

  /// @brief コンストラクタ
  CptEnable(
    const FileRegion& file_region,
    const char* name,
    PtExpr* arg_top
  ) : CptEnableBase(file_region, name, arg_top)
  {
  }

  /// @brief デストラクタ
  ~CptEnable() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 階層つきの名前をもった enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptEnableH :
  public CptEnable
{
public:

  /// @brief コンストラクタ
  CptEnableH(
    const FileRegion& file_region,
    const PtHierName& hname,
    PtExpr* arg_top
  ) : CptEnable(file_region, hname.tail_name, arg_top),
      mNbTop{hname.nb_list.top}
  {
  }

  /// @brief デストラクタ
  ~CptEnableH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのトップ
  const AstNameBranch* mNbTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief system task enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSysEnable :
  public CptEnableBase
{
public:

  /// @brief コンストラクタ
  CptSysEnable(
    const FileRegion& file_region,
    const char* task_name,
    PtExpr* arg_top
  ) : CptEnableBase(file_region, task_name, arg_top)
  {
  }

  /// @brief デストラクタ
  ~CptSysEnable() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief event statement を表すノード
//////////////////////////////////////////////////////////////////////
class CptEventStmt :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptEventStmt(
    const FileRegion& file_region,
    const AstExpr* event
  ) : CptStmt(file_region),
      mPrimary{event}
  {
    if ( event == nullptr ) {
      throw std::logic_error{"event = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptEventStmt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief イベントプライマリを返す．
  const AstExpr*
  primary() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベント名を表すプライマリ
  const AstExpr* mPrimary;

};


//////////////////////////////////////////////////////////////////////
/// @brief null statement を表すノード
//////////////////////////////////////////////////////////////////////
class CptNullStmt :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptNullStmt(
    const FileRegion& file_region
  ) : CptStmt(file_region)
  {
  }

  /// @brief デストラクタ
  ~CptNullStmt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTSTMT_H
