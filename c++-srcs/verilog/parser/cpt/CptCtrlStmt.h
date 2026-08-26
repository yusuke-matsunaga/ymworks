#ifndef CPTCTRLSTMT_H
#define CPTCTRLSTMT_H

/// @file CptStmt.h
/// @brief CptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

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

END_NAMESPACE_YM_VERILOG

#endif // CPTCTRLSTMT_H
