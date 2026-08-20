#ifndef CPTRANGE_H
#define CPTRANGE_H

/// @file CptRange.h
/// @brief CptRangen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtDecl.h"
#include "ym/FileRegion.h"
#include "ym/vl/Ast.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptRange CptDecl.h "CptDecl.h"
/// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRange :
  public PtRange
{
public:

  /// @brief コンストラクタ
  CptRange(
    const FileRegion& fr, ///< [in] ファイル上の位置
    const AstExpr* msb,   ///< [in] 範囲の MSB
    const AstExpr* lsb    ///< [in] 範囲の LSB
  ) : mFileRegion{fr},
      mMsb{msb},
      mLsb{lsb}
  {
  }

  /// @brief デストラクタ
  ~CptRange() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 範囲の MSB を取り出す．
  const AstExpr*
  left() const override;

  /// @brief 範囲の LSB を取り出す．
  const AstExpr*
  right() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // MSB
  const AstExpr* mMsb;

  // LSB
  const AstExpr* mLsb;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTRANGE_H
