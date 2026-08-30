#ifndef CPTSTMT_ASSIGN_H
#define CPTSTMT_ASSIGN_H

/// @file CptStmt_Assign.h
/// @brief CptStmt_Assign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptStmt_AssignBase CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief 代入文系の基底クラス
//////////////////////////////////////////////////////////////////////
class CptStmt_AssignBase :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmt_AssignBase(
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
  ~CptStmt_AssignBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の仮想関数
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
/// @class CptStmt_Assign CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief 代入文
//////////////////////////////////////////////////////////////////////
class CptStmt_Assign :
  public CptStmt_AssignBase
{
public:

  /// @brief コンストラクタ
  CptStmt_Assign(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : CptStmt_AssignBase(file_region, lhs),
      mRhs{rhs}
  {
    if ( rhs == nullptr ) {
      throw std::logic_error{"rhs = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptStmt_Assign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief rhs を得る．
  const AstExpr*
  rhs() const override;

  /// @brief コントロールを返す．
  const AstControl*
  control() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺
  const AstExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_AssignC CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief コントロールつきの代入文
//////////////////////////////////////////////////////////////////////
class CptStmt_AssignC :
  public CptStmt_Assign
{
public:

  /// @brief コンストラクタ
  CptStmt_AssignC(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs,
    const AstControl* control
  ) : CptStmt_Assign(file_region, lhs, rhs),
      mControl{control}
  {
    if ( control == nullptr ) {
      throw std::logic_error{"control = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptStmt_AssignC() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の仮想関数
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
/// @class CptStmt_NbAssign CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief ノンブロッキング代入文
//////////////////////////////////////////////////////////////////////
class CptStmt_NbAssign :
  public CptStmt_Assign
{
public:

  /// @brief コンストラクタ
  CptStmt_NbAssign(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : CptStmt_Assign(file_region, lhs, rhs)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_NbAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_NbAssignC CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief コントロールつきノンブロッキング代入文
//////////////////////////////////////////////////////////////////////
class CptStmt_NbAssignC :
  public CptStmt_AssignC
{
public:

  /// @brief コンストラクタ
  CptStmt_NbAssignC(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs,
    const AstControl* control
  ) : CptStmt_AssignC(file_region, lhs, rhs, control)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_NbAssignC() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_PcAssign CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief procedural continuous assigment 文
//////////////////////////////////////////////////////////////////////
class CptStmt_PcAssign :
  public CptStmt_AssignBase
{
public:

  /// @brief コンストラクタ
  CptStmt_PcAssign(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : CptStmt_AssignBase(file_region, lhs),
      mRhs{rhs}
  {
    if ( rhs == nullptr ) {
      throw std::logic_error{"rhs = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptStmt_PcAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
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
/// @class CptStmt_Deassign CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief deassigment 文
//////////////////////////////////////////////////////////////////////
class CptStmt_Deassign :
  public CptStmt_AssignBase
{
public:

  /// @brief コンストラクタ
  CptStmt_Deassign(
    const FileRegion& file_region,
    const AstExpr* lhs
  ) : CptStmt_AssignBase(file_region, lhs)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_Deassign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_Force CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief force 文
//////////////////////////////////////////////////////////////////////
class CptStmt_Force :
  public CptStmt_PcAssign
{
public:

  /// @brief コンストラクタ
  CptStmt_Force(
    const FileRegion& file_region,
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : CptStmt_PcAssign(file_region, lhs, rhs)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_Force() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_Release CptStmt_Assign.h "CptStmt_Assign.h"
/// @brief release 文
//////////////////////////////////////////////////////////////////////
class CptStmt_Release :
  public CptStmt_Deassign
{
public:

  /// @brief コンストラクタ
  CptStmt_Release(
    const FileRegion& file_region,
    const AstExpr* lhs
  ) : CptStmt_Deassign(file_region, lhs)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_Release() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTSTMT_ASSIGN_H
