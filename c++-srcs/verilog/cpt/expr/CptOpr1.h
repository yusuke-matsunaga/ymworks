#ifndef CPTOPR1_H
#define CPTOPR1_H

/// @file CptOpr1.h
/// @brief CptOpr1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr1 CptOpr1.h "CptOpr1.h"
/// @brief 単項演算子を表す基底クラス
//////////////////////////////////////////////////////////////////////
class CptOpr1 :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr1(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : mFileRegion{file_region},
      mOpr{opr}
  {
    if ( opr == nullptr ) {
      throw std::logic_error{"opr == nullptr"};
    }
  }

  // デストラクタ
  ~CptOpr1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // 階層名の添字として使える式の時に true を返す．
  bool
  is_index_expr() const override;

  // 階層名の添字として使える式の時にその値を返す．
  int
  index_value() const override;

  /// @brief 0番目のオペランドの取得
  const AstExpr*
  operand0() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // オペランド
  const AstExpr* mOpr;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR1_H
