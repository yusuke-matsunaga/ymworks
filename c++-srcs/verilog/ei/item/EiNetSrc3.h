#ifndef EINETSRC3_H
#define EINETSRC3_H

/// @file EiNetSrc3.h
/// @brief EiNetSrc3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "EiNetSrc1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiNetSrc3 EiNetSrc3.h "EiNetSrc3.h"
/// @brief 右辺式を分割した場合の EiNetSrc1 の派生クラス
//////////////////////////////////////////////////////////////////////
class EiNetSrc3 :
  public EiNetSrc1
{
public:

  /// @brief コンストラクタ
  EiNetSrc3(
    const VlExpr* rhs,  ///< [in] 右辺式
    SizeType rhs_width, ///< [in] 右辺式のビット幅
    SizeType rhs_offset ///< [in] 右辺式のオフセット
  ) : EiNetSrc1(rhs),
      mRhsWidth{rhs_width},
      mRhsOffset{rhs_offset}
  {
  }

  /// @brief デストラクタ
  ~EiNetSrc3() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ソースを分割している場合のビット幅を返す．
  ///
  /// 分割していない場合は rhs_expr()->bit_size() を返す．
  SizeType
  rhs_width() const override;

  /// @brief ソースを分割している場合のビット位置オフセットを返す．
  ///
  /// 分割していない場合は 0 を返す．
  SizeType
  rhs_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 右辺式のビット幅
  SizeType mRhsWidth;

  // 右辺式のオフセット
  SizeType mRhsOffset;

};

END_NAMESPACE_YM_VERILOG

#endif // EINETSRC3_H
