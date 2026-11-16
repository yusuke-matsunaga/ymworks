#ifndef YM_VL_VLTERM_H
#define YM_VL_VLTERM_H

#error "obsolete"

/// @file ym/vl/VlTerm.h
/// @brief VlTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlTerm VlTerm.h "VlTerm.h"
/// @brief 端子を表すクラス
///
/// プライマリを表す VlExpr のリストを表す．
///
/// シンタックス上は連結演算が入れ子になることができるが，
/// ここでは平坦化して表す．
//////////////////////////////////////////////////////////////////////
class VlTerm :
  public VlObj
{
public:

  /// @brief デストラクタ
  virtual
  ~VlTerm() {}


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の値のタイプを返す．
  ///
  /// このタイプで評価する．
  virtual
  VlValueType
  value_type() const = 0;

  /// @brief Verilog-HDL の文字列を得る．
  virtual
  std::string
  decompile() const = 0;

  /// @brief 式のビット幅を返す．
  ///
  /// value_type().size() を同じ
  virtual
  SizeType
  bit_size() const = 0;

  /// @brief 要素数を返す．
  virtual
  SizeType
  elem_num() const = 0;

  /// @brief 要素を返す．
  virtual
  const VlExpr*
  elem(
    SizeType index ///< [in] インデックス ( 0 <= index < elem_num() )
  ) const = 0;

  /// @brief elem() の別名
  const VlExpr*
  operator[](
    SizeType index ///< [in] インデックス ( 0 <= index < elem_num() )
  ) const
  {
    return elem(index);
  }

};

END_NAMESPACE_YM_VERILOG

#endif // VLTERM_H
