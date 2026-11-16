#ifndef EINETSRC1_H
#define EINETSRC1_H

/// @file EiNetSrc1.h
/// @brief EiNetSrc1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlNetSrc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiNetSrc1 EiNetSrc1.h "EiNetSrc1.h"
/// @brief 単純な VlNetSrc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiNetSrc1 :
  public VlNetSrc
{
public:

  /// @brief コンストラクタ
  EiNetSrc1(
    const VlExpr* rhs ///< [in] 右辺式
  ) : mRhs{rhs}
  {
  }

  /// @brief デストラクタ
  ~EiNetSrc1() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNetSrc の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット/部分選択のタイプを返す．
  ///
  /// 有効な値は NONE と CONST のみ
  VlSelectType
  select_type() const override;

  /// @brief ビット/部分選択のビット位置のオフセットを返す．
  SizeType
  bit_offset() const override;

  /// @brief ソースの式を返す．
  const VlExpr*
  rhs_expr() const override;

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

  // 右辺式
  const VlExpr* mRhs;

};

END_NAMESPACE_YM_VERILOG

#endif // EINETSRC1_H
