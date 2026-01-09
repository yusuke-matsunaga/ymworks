#ifndef CIFSMCELL_H
#define CIFSMCELL_H

/// @file CiFsmCell.h
/// @brief CiFsmCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCell.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class CiFsmCell CiFsmCell.h "CiFsmCell.h"
/// @brief 単純なFSMセルを表す CiCell の派生クラス
//////////////////////////////////////////////////////////////////////
class CiFsmCell :
  public CiCell
{
public:

  /// @brief 空のコンストラクタ
  CiFsmCell() = default;

  /// @brief コンストラクタ
  CiFsmCell(
    const ShString& name,     ///< [in] 名前
    ClibArea area             ///< [in] 面積
  ) : CiCell{name, area}
  {
  }

  /// @brief デストラクタ
  ~CiFsmCell() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief セルの種類を返す．
  ClibCellType
  type() const override;

  /// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
  bool
  is_fsm() const override;


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

#endif // CIFSMCELL_H
