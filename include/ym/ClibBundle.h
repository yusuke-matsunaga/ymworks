#ifndef YM_CLIBBUNDLE_H
#define YM_CLIBBUNDLE_H

/// @file ym/ClibBundle.h
/// @brief ClibBundle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellElem.h"
#include "ym/ClibList2.h"


BEGIN_NAMESPACE_YM_CLIB

class CiBundle;

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibBundle ClibBundle.h "ym/ClibBundle.h"
/// @brief バンドルを表すクラス
//////////////////////////////////////////////////////////////////////
class ClibBundle :
  public ClibCellElem<CiBundle>
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// - 不正値となる．
  ClibBundle() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibBundle(
    const CiCell* cell,  ///< [in] 親のセル
    const CiBundle* impl ///< [in] 本体
  ) : ClibCellElem{cell, impl}
  {
  }

  /// @brief デストラクタ
  ~ClibBundle() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @exception std::logic_error is_valid() = false の場合
  std::string
  name() const;

  /// @brief ピン数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  pin_num() const;

  /// @brief ピンの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPin
  ClibPin
  pin(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < pin_num() )
  ) const;

  /// @brief ピンのリストの取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibPinList
  ClibPinList
  pin_list() const;

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBBUNDLE_H
