#ifndef EISTMTBLOCKSCOPE_H
#define EISTMTBLOCKSCOPE_H

/// @file EiStmtBlockScope.h
/// @brief EiStmtBlockScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiScope.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiStmtBlockScope EiStmtBlockScope.h "EiStmtBlockScope.h"
/// @brief ステートメントブロックに対応するスコープ
//////////////////////////////////////////////////////////////////////
class EiStmtBlockScope :
  public EiScope
{
public:

  /// @brief コンストラクタ
  EiStmtBlockScope(
    const VlScope* parent,  ///< [in] 親のスコープ環境
    const AstStmt& ast_stmt ///< [in] 対応するパース木の要素
  );

  /// @brief デストラクタ
  ~EiStmtBlockScope();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlScope の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の要素
  AstStmt mAstStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EISTMTBLOCKSCOPE_H
