#ifndef CPTSTMT_H
#define CPTSTMT_H

/// @file CptStmt.h
/// @brief CptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtStmt.h"
#include "ym/FileRegion.h"
#include "parser/PtArray.h"


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

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const AstNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;

  /// @brief 名前の取得
  /// @return 名前
  const char*
  name() const override;

  /// @brief ステートメントの種類を表す文字列の取得
  /// @return ステートメントの種類を表す文字列
  const char*
  stmt_name() const override;

  /// @brief 引数の数の取得
  SizeType
  arg_num() const override;

  /// @brief 引数の取得
  const AstExpr*
  arg(
    SizeType pos ///< [in] 位置 ( 0 <= pos < arg_num() )
  ) const override;

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

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  const AstCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const override;

  /// @brief 初期化代入文の取得
  /// @return 初期化代入文
  const AstStmt*
  init_stmt() const override;

  /// @brief 繰り返し代入文の取得
  /// @return 繰り返し代入文
  const AstStmt*
  next_stmt() const override;

  /// @brief 宣言ヘッダ配列の要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const AstDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief 子供のステートメント配列の要素数の取得
  SizeType
  stmt_num() const override;

  /// @brief 子供のステートメントの取得
  const AstStmt*
  stmt(
    SizeType pos ///< [in] 位置 ( 0 <= pos < stmt_num() )
  ) const override;


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


public:
  //////////////////////////////////////////////////////////////////////
  // PtDisable の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 末尾の名前を返す．
  const char*
  name() const override;


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
    PtNameBranchArray&& nb_array,
    const char* tail_name
  ) : CptDisable{file_region, tail_name},
    mNbArray{std::move(nb_array)}
  {
  }

  /// @brief デストラクタ
  ~CptDisableH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const AstNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchArray mNbArray;

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
    PtExprArray&& arg_array
  ) : CptStmt(file_region),
    mName{name},
    mArgArray{std::move(arg_array)}
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

  /// @brief 引数の数の取得
  SizeType
  arg_num() const override;

  /// @brief 引数の取得
  const AstExpr*
  arg(
    SizeType pos ///< [in] 位置 ( 0 <= pos < arg_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

  // 引数の配列
  PtExprArray mArgArray;

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
    PtExprArray&& arg_array
  ) : CptEnableBase(file_region, name, std::move(arg_array))
  {
  }

  /// @brief デストラクタ
  ~CptEnable() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

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
    PtNameBranchArray&& nb_array,
    const char* tail_name,
    PtExprArray&& arg_array
  ) : CptEnable(file_region, tail_name, std::move(arg_array)),
    mNbArray{std::move(nb_array)}
  {
  }

  /// @brief デストラクタ
  ~CptEnableH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const AstNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchArray mNbArray;

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
    PtExprArray&& arg_array
  ) : CptEnableBase(file_region, task_name, std::move(arg_array))
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
/// @brief CtrlStmt の基底クラス
//////////////////////////////////////////////////////////////////////
class CptCtrlStmt :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptCtrlStmt(
    const FileRegion& file_region,
    const AstControl* control,
    const AstStmt* body
  ) : CptStmt(file_region),
      mControl{control},
      mBody{body}
  {
    if ( control == nullptr ) {
      throw std::logic_error{"control = nullptr"};
    }
    if ( body == nullptr ) {
      throw std::logic_error{"body = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptCtrlStmt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  const AstControl*
  control() const override;

  /// @brief 本体を取り出す．
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  const AstControl* mControl;

  // 本体
  const AstStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay control statement
//////////////////////////////////////////////////////////////////////
class CptDcStmt :
  public CptCtrlStmt
{
public:

  /// @brief コンストラクタ
  CptDcStmt(
    const FileRegion& file_region,
    const AstControl* delay,
    const AstStmt* body
  ) : CptCtrlStmt(file_region, delay, body)
  {
  }

  /// @brief デストラクタ
  ~CptDcStmt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief event control statement
//////////////////////////////////////////////////////////////////////
class CptEcStmt :
  public CptCtrlStmt
{
public:

  /// @brief コンストラクタ
  CptEcStmt(
    const FileRegion& file_region,
    const AstControl* event,
    const AstStmt* body
  ) : CptCtrlStmt(file_region, event, body)
  {
  }

  /// @brief デストラクタ
  ~CptEcStmt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief wait 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptWait :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptWait(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* body
  ) : CptStmt(file_region),
      mExpr{expr},
      mBody{body}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
    if ( body == nullptr ) {
      throw std::logic_error{"body = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptWait() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 条件を返す．
  const AstExpr*
  expr() const override;

  /// @brief 実行すべき本体を返す．
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件
  const AstExpr* mExpr;

  // 実行すべき本体
  const AstStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief 代入文系の基底クラス
//////////////////////////////////////////////////////////////////////
class CptAssignBase :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptAssignBase(
    const FileRegion& file_region,
    const AstExpr* lhs
  ) : CptStmt(file_region),
    mLhs{lhs}
  {
    if ( lhs == nullptr ) {
      throw std::logic_error{"lhs = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptAssignBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief lhs を得る．
  const AstExpr*
  lhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  const AstExpr* mLhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief 代入文
//////////////////////////////////////////////////////////////////////
class CptAssign :
  public CptAssignBase
{
public:

  /// @brief コンストラクタ
  CptAssign(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : CptAssignBase(file_region, lhs),
      mRhs{rhs}
  {
    if ( rhs == nullptr ) {
      throw std::logic_error{"rhs = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief rhs を得る．
  const AstExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺
  const AstExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief コントロールつきの代入文
//////////////////////////////////////////////////////////////////////
class CptAssignC :
  public CptAssign
{
public:

  /// @brief コンストラクタ
  CptAssignC(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs,
    const AstControl* control
  ) : CptAssign(file_region, lhs, rhs),
      mControl{control}
  {
    if ( control == nullptr ) {
      throw std::logic_error{"control = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptAssignC() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  const AstControl*
  control() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  const AstControl* mControl;

};


//////////////////////////////////////////////////////////////////////
/// @brief ノンブロッキング代入文
//////////////////////////////////////////////////////////////////////
class CptNbAssign :
  public CptAssign
{
public:

  /// @brief コンストラクタ
  CptNbAssign(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : CptAssign(file_region, lhs, rhs)
  {
  }

  /// @brief デストラクタ
  ~CptNbAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief コントロールつきノンブロッキング代入文
//////////////////////////////////////////////////////////////////////
class CptNbAssignC :
  public CptAssignC
{
public:

  /// @brief コンストラクタ
  CptNbAssignC(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs,
    const AstControl* control
  ) : CptAssignC(file_region, lhs, rhs, control)
  {
  }

  /// @brief デストラクタ
  ~CptNbAssignC() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief procedural continuous assigment 文
//////////////////////////////////////////////////////////////////////
class CptPcAssign :
  public CptAssignBase
{
public:

  /// @brief コンストラクタ
  CptPcAssign(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : CptAssignBase(file_region, lhs),
      mRhs{rhs}
  {
    if ( rhs == nullptr ) {
      throw std::logic_error{"rhs = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptPcAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 右辺式を返す．
  const AstExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺
  const AstExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief deassigment 文
//////////////////////////////////////////////////////////////////////
class CptDeassign :
  public CptAssignBase
{
public:

  /// @brief コンストラクタ
  CptDeassign(
    const FileRegion& file_region,
    const AstExpr* lhs
  ) : CptAssignBase(file_region, lhs)
  {
  }

  /// @brief デストラクタ
  ~CptDeassign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief force 文
//////////////////////////////////////////////////////////////////////
class CptForce :
  public CptPcAssign
{
public:

  /// @brief コンストラクタ
  CptForce(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : CptPcAssign(file_region, lhs, rhs)
  {
  }

  /// @brief デストラクタ
  ~CptForce() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief release 文
//////////////////////////////////////////////////////////////////////
class CptRelease :
  public CptDeassign
{
public:

  /// @brief コンストラクタ
  CptRelease(
    const FileRegion& file_region,
    const AstExpr* lhs
  ) : CptDeassign(file_region, lhs)
  {
  }

  /// @brief デストラクタ
  ~CptRelease() {}


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


//////////////////////////////////////////////////////////////////////
/// @brief if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptIf :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptIf(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* then_body
  ) : CptStmt(file_region),
      mCond{expr},
      mThen{then_body}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptIf() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 条件式を返す．
  const AstExpr*
  expr() const override;

  /// @brief 成り立ったとき実行されるステートメント
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const AstExpr* mCond;

  // 成り立ったとき実行されるステートメント
  const AstStmt* mThen;

};


//////////////////////////////////////////////////////////////////////
/// @brief else 節付きの if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptIfElse :
  public CptIf
{
public:

  /// @brief コンストラクタ
  CptIfElse(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* then_body,
    const AstStmt* else_body
  ) : CptIf(file_region, expr, then_body),
      mElse{else_body}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptIfElse() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 成り立たなかったとき実行されるステートメント
  const AstStmt*
  else_body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 成り立たなかったとき実行されるステートメント
  const AstStmt* mElse;

};


//////////////////////////////////////////////////////////////////////
/// @brief caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseItem :
  public PtCaseItem
{
public:

  /// @brief コンストラクタ
  CptCaseItem(
    const FileRegion& file_region,
    PtExprArray&& label_array,
    const AstStmt* body
  ) : mFileRegion{file_region},
      mLabelArray{std::move(label_array)},
      mBody{body}
  {
    if ( body == nullptr ) {
      throw std::logic_error{"body = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptCaseItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief ラベルのリストの要素数の取得
  SizeType
  label_num() const override;

  /// @brief ラベルの取得
  const AstExpr*
  label(
    SizeType pos ///< [in] 位置 ( 0 <= pos < label_num() )
  ) const override;

  /// @brief 本体のステートメント得る．
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの配列
  PtExprArray mLabelArray;

  // ラベルが一致したときに実行されるステートメント
  const AstStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCase :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptCase(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtCaseItemArray&& caseitem_array
  ) : CptStmt(file_region),
      mExpr{expr},
      mCaseItemArray{std::move(caseitem_array)}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptCase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 比較される式を返す．
  const AstExpr*
  expr() const override;

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  const AstCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 比較される式
  const AstExpr* mExpr;

  // case item の配列
  PtCaseItemArray mCaseItemArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief casex 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseX :
  public CptCase
{
public:

  /// @brief コンストラクタ
  CptCaseX(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtCaseItemArray&& caseitem_array
  ) : CptCase(file_region, expr, std::move(caseitem_array))
  {
  }

  /// @brief デストラクタ
  ~CptCaseX() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief casez 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseZ :
  public CptCase
{
public:

  /// @brief コンストラクタ
  CptCaseZ(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtCaseItemArray&& caseitem_array
  ) : CptCase(file_region, expr, std::move(caseitem_array))
  {
  }

  /// @brief デストラクタ
  ~CptCaseZ() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief LoopStmt のベースクラス
//////////////////////////////////////////////////////////////////////
class CptLoopStmt :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptLoopStmt(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptStmt(file_region),
      mBody{body}
  {
    if ( body == nullptr ) {
      throw std::logic_error{"body = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptLoopStmt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実行すべき本体を返す．
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実行すべき本体
  const AstStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief forever 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptForever :
  public CptLoopStmt
{
public:

  /// @brief コンストラクタ
  CptForever(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptLoopStmt(file_region, body)
  {
  }

  /// @brief デストラクタ
  ~CptForever() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief repeat 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRepeat :
  public CptLoopStmt
{
public:

  /// @brief コンストラクタ
  CptRepeat(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* body
  ) : CptLoopStmt(file_region, body),
      mExpr{expr}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptRepeat() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 繰り返し式を返す．
  const AstExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し式
  const AstExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief while 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptWhile :
  public CptRepeat // ちょっと汚い継承
{
public:

  /// @brief コンストラクタ
  CptWhile(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* body
  ) : CptRepeat(file_region, expr, body)
  {
  }

  /// @brief デストラクタ
  ~CptWhile() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFor :
  public CptRepeat
{
public:

  /// @brief コンストラクタ
  CptFor(
    const FileRegion& file_region,
    const AstStmt* init,
    const AstExpr* cond,
    const AstStmt* next,
    const AstStmt* body
  ) : CptRepeat(file_region, cond, body),
      mInit{init},
      mNext{next}
  {
    if ( init == nullptr ) {
      throw std::logic_error{"init = nullptr"};
    }
    if ( next == nullptr ) {
      throw std::logic_error{"next = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptFor() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 初期化代入式を取出す
  const AstStmt*
  init_stmt() const override;

  /// @brief 繰り返し代入式を取出す
  const AstStmt*
  next_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化文
  const AstStmt* mInit;

  // 増加処理文
  const AstStmt* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @brief StmtBlock の基底クラス
//////////////////////////////////////////////////////////////////////
class CptStmtBlock :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmtBlock(
    const FileRegion& file_region,
    PtStmtArray&& stmt_array
  ) : CptStmt(file_region),
      mStmtArray{std::move(stmt_array)}
  {
  }

  /// @brief デストラクタ
  ~CptStmtBlock() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmtBlock の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメント配列の要素数の取得
  SizeType
  stmt_num() const override;

  /// @brief 子供のステートメントの取得
  const AstStmt*
  stmt(
    SizeType pos ///< [in] 位置 ( 0 <= pos < stmt_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ステートメントの配列
  PtStmtArray mStmtArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ StmtBlock
//////////////////////////////////////////////////////////////////////
class CptStmtBlockN :
  public CptStmtBlock
{
public:

  /// @brief コンストラクタ
  CptStmtBlockN(
    const FileRegion& file_region,
    const char* name,
    PtDeclHeadArray&& declhead_array,
    PtStmtArray&& stmt_array
  ) : CptStmtBlock(file_region, std::move(stmt_array)),
      mName{name},
      mDeclHeadArray{std::move(declhead_array)}
  {
  }

  /// @brief デストラクタ
  ~CptStmtBlockN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief 宣言ヘッダ配列の要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const AstDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 宣言の配列
  PtDeclHeadArray mDeclHeadArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptParBlock :
  public CptStmtBlock
{
public:

  /// @brief コンストラクタ
  CptParBlock(
    const FileRegion& file_region,
    PtStmtArray&& stmt_array
  ) : CptStmtBlock(file_region, std::move(stmt_array))
  {
  }

  /// @brief デストラクタ
  ~CptParBlock() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptParBlockN :
  public CptStmtBlockN
{
public:

  /// @brief コンストラクタ
  CptParBlockN(
    const FileRegion& file_region,
    const char* name,
    PtDeclHeadArray&& declhead_array,
    PtStmtArray&& stmt_array
  ) : CptStmtBlockN(file_region, name,
		    std::move(declhead_array),
		    std::move(stmt_array))
  {
  }

  /// @brief デストラクタ
  ~CptParBlockN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptSeqBlock :
  public CptStmtBlock
{
public:

  /// @brief コンストラクタ
  CptSeqBlock(
    const FileRegion& file_region,
    PtStmtArray&& stmt_array
  ) : CptStmtBlock(file_region, std::move(stmt_array))
  {
  }

  /// @brief デストラクタ
  ~CptSeqBlock() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptSeqBlockN :
  public CptStmtBlockN
{
public:

  /// @brief コンストラクタ
  CptSeqBlockN(
    const FileRegion& file_region,
    const char* name,
    PtDeclHeadArray&& declhead_array,
    PtStmtArray&& stmt_array
  ) : CptStmtBlockN(file_region, name,
		  std::move(declhead_array),
		  std::move(stmt_array))
  {
  }

  /// @brief デストラクタ
  ~CptSeqBlockN() {}


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
