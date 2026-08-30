#ifndef CPTSTMT_CTRL_H
#define CPTSTMT_CTRL_H

/// @file CptStmt_Ctrl.h
/// @brief CptStmt_Ctrl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptStmt_Ctrl CptStmt_Ctrl.h "CptStmt_Ctrl.h"
/// @brief コントロールステートメントの基底クラス
//////////////////////////////////////////////////////////////////////
class CptStmt_Ctrl :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmt_Ctrl(
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
  ~CptStmt_Ctrl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の仮想関数
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
/// @class CptStmt_DelayControl CptStmt_Cond.h "CptStmt_Cond.h"
/// @brief delay control statement
//////////////////////////////////////////////////////////////////////
class CptStmt_DelayControl :
  public CptStmt_Ctrl
{
public:

  /// @brief コンストラクタ
  CptStmt_DelayControl(
    const FileRegion& file_region,
    const AstControl* delay,
    const AstStmt* body
  ) : CptStmt_Ctrl(file_region, delay, body)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_DelayControl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_EventControl
/// @brief event control statement
//////////////////////////////////////////////////////////////////////
class CptStmt_EventControl :
  public CptStmt_Ctrl
{
public:

  /// @brief コンストラクタ
  CptStmt_EventControl(
    const FileRegion& file_region,
    const AstControl* event,
    const AstStmt* body
  ) : CptStmt_Ctrl(file_region, event, body)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_EventControl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_Wait CptStmt_Cond.h "CptStmt_Cond.h"
/// @brief wait 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_Wait :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptStmt_Wait(
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
  ~CptStmt_Wait() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
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

END_NAMESPACE_YM_VERILOG

#endif // CPTSTMT_CTRL_H
