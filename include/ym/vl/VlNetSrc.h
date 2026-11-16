#ifndef VLNETSRC_H
#define VLNETSRC_H

/// @file VlNetSrc.h
/// @brief VlNetSrc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlNetSrc VlNetSrc.h "VlNetSrc.h"
/// @brief 継続的代入文のソースの情報を表すクラス
///
/// 通常は単一の VlExpr* だが複数のビット/部分選択に対する
/// VlExpr* の場合もある．
//////////////////////////////////////////////////////////////////////
class VlNetSrc
{
public:

  /// @brief デストラクタ
  virtual
  ~VlNetSrc() {}


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ビット/部分選択のタイプを返す．
  ///
  /// 有効な値は NONE と CONST のみ
  virtual
  VlSelectType
  select_type() const = 0;

  /// @brief ビット/部分選択のビット位置のオフセットを返す．
  virtual
  SizeType
  bit_offset() const = 0;

  /// @brief ソースの式を返す．
  virtual
  const VlExpr*
  rhs_expr() const = 0;

  /// @brief ソースを分割している場合のビット位置オフセットを返す．
  ///
  /// 分割していない場合は 0 を返す．
  virtual
  SizeType
  rhs_offset() const = 0;

  /// @brief ソースを分割している場合のビット幅を返す．
  ///
  /// 分割していない場合は rhs_expr()->bit_size() を返す．
  virtual
  SizeType
  rhs_width() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VLNETSRC_H
