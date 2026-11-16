#ifndef EINETSRC2_H
#define EINETSRC2_H

/// @file EiNetSrc2.h
/// @brief EiNetSrc2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "EiNetSrc1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiNetSrc2 EiNetSrc2.h "EiNetSrc2.h"
/// @brief 左辺がビット/部分選択を持つ VlNetSrc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiNetSrc2 :
  public EiNetSrc1
{
public:

  /// @brief コンストラクタ
  EiNetSrc2(
    SizeType bit_offset, ///< [in] ビット位置のオフセット
    const VlExpr* rhs    ///< [in] 右辺式
  ) : EiNetSrc1(rhs),
      mBitOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiNetSrc2() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット/部分選択のタイプを返す．
  ///
  /// 有効な値は NONE と CONST のみ
  VlSelectType
  select_type() const override;

  /// @brief ビット/部分選択のビット位置のオフセットを返す．
  SizeType
  bit_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺のビット位置のオフセット
  SizeType mBitOffset;

};

END_NAMESPACE_YM_VERILOG

#endif // EINETSRC2_H
