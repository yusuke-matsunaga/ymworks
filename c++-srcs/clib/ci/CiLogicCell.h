#ifndef CILOGICCELL_H
#define CILOGICCELL_H

/// @file CiLogicCell.h
/// @brief CiLogicCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCell.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class CiLogicCell CiLogicCell.h "CiLogicCell.h"
/// @brief 論理セルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLogicCell :
  public CiCell
{
public:

  /// @brief 空のコンストラクタ
  CiLogicCell() = default;

  /// @brief コンストラクタ
  CiLogicCell(
    const ShString& name, ///< [in] 名前
    ClibArea area         ///< [in] 面積
  ) : CiCell{name, area}
  {
  }

  /// @brief デストラクタ
  ~CiLogicCell() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;

  /// @brief restore() の下請け関数
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) override;

};

END_NAMESPACE_YM_CLIB

#endif // CILOGICCELL_H
