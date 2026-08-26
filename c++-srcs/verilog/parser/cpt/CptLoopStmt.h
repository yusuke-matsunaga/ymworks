#ifndef CPTLOOPSTMT_H
#define CPTLOOPSTMT_H

/// @file CptStmt.h
/// @brief CptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

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

END_NAMESPACE_YM_VERILOG

#endif // CPTLOOPSTMT_H
