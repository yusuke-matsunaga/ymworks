#ifndef CPTSTMT_LOOP_H
#define CPTSTMT_LOOP_H

/// @file CptStmt_Loop.h
/// @brief CptStmt_Loop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptStmt_Loop CptStmt_Loop.h "CptStmt_Loop.h"
/// @brief Loop Statement のベースクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_Loop :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmt_Loop(
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
  ~CptStmt_Loop() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
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
/// @class CptStmt_Forever CptStmt_Loop.h "CptStmt_Loop.h"
/// @brief forever 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_Forever :
  public CptStmt_Loop
{
public:

  /// @brief コンストラクタ
  CptStmt_Forever(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptStmt_Loop(file_region, body)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_Forever() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_Repeat CptStmt_Loop.h "CptStmt_Loop.h"
/// @brief repeat 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_Repeat :
  public CptStmt_Loop
{
public:

  /// @brief コンストラクタ
  CptStmt_Repeat(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* body
  ) : CptStmt_Loop(file_region, body),
      mExpr{expr}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptStmt_Repeat() {}


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
/// @class CptStmt_Whire CptStmt_Loop.h "CptStmt_Loop.h"
/// @brief while 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_While :
  public CptStmt_Repeat // ちょっと汚い継承
{
public:

  /// @brief コンストラクタ
  CptStmt_While(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* body
  ) : CptStmt_Repeat(file_region, expr, body)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_While() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_For CptStmt_Loop.h "CptStmt_Loop.h"
/// @brief for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_For :
  public CptStmt_Repeat
{
public:

  /// @brief コンストラクタ
  CptStmt_For(
    const FileRegion& file_region,
    const AstStmt* init,
    const AstExpr* cond,
    const AstStmt* next,
    const AstStmt* body
  ) : CptStmt_Repeat(file_region, cond, body),
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
  ~CptStmt_For() {}


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

#endif // CPTSTMT_LOOP_H
