#ifndef CPTOPR_MULTICONCAT_H
#define CPTOPR_MULTICONCAT_H

/// @file CptOpr_MultiConcat.h
/// @brief CptOpr_MultiConcat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr_Concat.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_MultiConcat CptOpr_MultiConcat.h "CptOpr_MultiConcat.h"
/// @brief MultiConcat 演算を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_MultiConcat :
  public CptOpr_Concat
{
public:

  // コンストラクタ
  CptOpr_MultiConcat(
    const FileRegion& file_region,
    const AstExpr* rep,
    const AstExpr* expr_top
  ) : CptOpr_Concat(file_region, expr_top),
      mRep{rep}
  {
  }

  // デストラクタ
  ~CptOpr_MultiConcat() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  ///演算子の種類の取得
  VpiOpType
  op_type() const override;

  /// @brief multi-concat の繰り返し数
  ///
  /// - type() != Opr および op_type() != MultiConcat の時 std::logic 例外を送出する．
  const AstExpr*
  rep() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数
  const AstExpr* mRep;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_MULTICONCAT_H
