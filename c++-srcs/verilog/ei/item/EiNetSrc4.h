#ifndef EINETSRC4_H
#define EINETSRC4_H

/// @file EiNetSrc4.h
/// @brief EiNetSrc4 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "EiNetSrc4.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiNetSrc4 EiNetSrc4.h "EiNetSrc4.h"
/// @brief 左辺がビット/部分選択を持ち，右辺を分割した VlNetSrc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiNetSrc4 :
  public EiNetSrc2
{
public:

  /// @brief コンストラクタ
  EiNetSrc4(
    SizeType bit_offset, ///< [in] ビット位置のオフセット
    const VlExpr* rhs,   ///< [in] 右辺式
    SizeType rhs_width,  ///< [in] 右辺式のビット幅
    SizeType rhs_offset  ///< [in] 右辺式のオフセット
  ) : EiNetSrc2(bit_offset, rhs),
      mRhsWidth{rhs_width},
      mRhsOffset{rhs_offset}
  {
  }

  /// @brief デストラクタ
  ~EiNetSrc4() = default;


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

#endif // EINETSRC4_H
