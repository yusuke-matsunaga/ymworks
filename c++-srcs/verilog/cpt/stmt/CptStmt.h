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
/// @class CptStmt_Disable CptStmt.h "CptStmt.h"
/// @brief disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_Disable :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptStmt_Disable(
    const FileRegion& file_region,
    const char* name
  ) : CptStmt(file_region),
    mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptStmt_Disable() {}


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
/// @class CptStmt_DisableH CptStmt.h "CptStmt.h"
/// @brief 階層つき名前を持つ disable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_DisableH :
  public CptStmt_Disable
{
public:

  /// @brief コンストラクタ
  CptStmt_DisableH(
    const FileRegion& file_region,
    const PtHierName& hname
  ) : CptStmt_Disable(file_region, hname.tail_name),
      mNbTop{hname.nb_list.top}
  {
  }

  /// @brief デストラクタ
  ~CptStmt_DisableH() {}


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

  // 階層ブランチの先頭
  const AstNameBranch* mNbTop;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_EnableBase CptStmt.h "CptStmt.h"
/// @brief task enable 文 / system task enable 文に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class CptStmt_EnableBase :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmt_EnableBase(
    const FileRegion& file_region,
    const char* name,
    PtExpr* arg_top
  ) : CptStmt(file_region),
      mName{name},
      mArgTop{arg_top}
  {
  }

  /// @brief デストラクタ
  ~CptStmt_EnableBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;

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
/// @class CptStmt_Enable CptStmt.h "CptStmt.h"
/// @brief 階層なしの名前を持った enable 文
//////////////////////////////////////////////////////////////////////
class CptStmt_Enable :
  public CptStmt_EnableBase
{
public:

  /// @brief コンストラクタ
  CptStmt_Enable(
    const FileRegion& file_region,
    const char* name,
    PtExpr* arg_top
  ) : CptStmt_EnableBase(file_region, name, arg_top)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_Enable() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_EnableH CptStmt.h "CptStmt.h"
/// @brief 階層つきの名前をもった enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_EnableH :
  public CptStmt_Enable
{
public:

  /// @brief コンストラクタ
  CptStmt_EnableH(
    const FileRegion& file_region,
    const PtHierName& hname,
    PtExpr* arg_top
  ) : CptStmt_Enable(file_region, hname.tail_name, arg_top),
      mNbTop{hname.nb_list.top}
  {
  }

  /// @brief デストラクタ
  ~CptStmt_EnableH() {}


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
/// @class CptStmt_SysEnable CptStmt.h "CptStmt.h"
/// @brief system task enable 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_SysEnable :
  public CptStmt_EnableBase
{
public:

  /// @brief コンストラクタ
  CptStmt_SysEnable(
    const FileRegion& file_region,
    const char* task_name,
    PtExpr* arg_top
  ) : CptStmt_EnableBase(file_region, task_name, arg_top)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_SysEnable() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_Event CptStmt.h "CptStmt.h"
/// @brief event statement を表すノード
//////////////////////////////////////////////////////////////////////
class CptStmt_Event :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptStmt_Event(
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
  ~CptStmt_Event() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
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
/// @class CptStmt_Null CptStmt.h "CptStmt.h"
/// @brief null statement を表すノード
//////////////////////////////////////////////////////////////////////
class CptStmt_Null :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptStmt_Null(
    const FileRegion& file_region
  ) : CptStmt(file_region)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_Null() {}


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
