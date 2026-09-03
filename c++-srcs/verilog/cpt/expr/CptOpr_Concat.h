#ifndef CPTOPR_CONCAT_H
#define CPTOPR_CONCAT_H

/// @file CptOpr_Concat.h
/// @brief CptOpr_Concat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Concat CptOpr_Concat.h "CptOpr_Concat.h"
/// @brief Concat 演算を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Concat :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr_Concat(
    const FileRegion& file_region,
    const PtExpr* expr_top
  ) : mFileRegion{file_region},
      mExprTop{expr_top}
  {
  }

  // デストラクタ
  ~CptOpr_Concat() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  ///演算子の種類の取得
  VpiOpType
  op_type() const override;

  /// @brief オペランドのリストの先頭の取得
  const PtExpr*
  operand_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 結合する式の先頭
  const PtExpr* mExprTop;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_CONCAT_H
