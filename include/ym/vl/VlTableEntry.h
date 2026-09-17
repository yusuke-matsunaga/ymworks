#ifndef YM_VL_VLTABLEENTRY_H
#define YM_VL_VLTABLEENTRY_H

/// @file ym/vl/VlUdp.h
/// @brief VlUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlObj.h"
#include "ym/vl/VlFwd.h"
#include "ym/vl/VlUdpVal.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlTableEntry VlTableEntry.h "ym/vl/VlTableEntry.h"
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class VlTableEntry :
  public VlObj
{
public:

  /// @brief デストラクタ
  ~VlTableEntry() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTableEntry に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行の要素数を返す．
  virtual
  SizeType
  size() const = 0;

  /// @brief pos 番目の位置の値を返す．
  virtual
  VlUdpVal
  val(SizeType pos) const = 0;

  /// @brief 一行文の内容を表す文字列をつくる．
  virtual
  std::string
  str() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_VLTABLEENTRY_H
