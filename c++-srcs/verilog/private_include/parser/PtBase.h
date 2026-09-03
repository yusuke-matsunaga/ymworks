#ifndef PARSER_PTBASE_H
#define PARSER_PTBASE_H

/// @file PtBase.h
/// @brief PtBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtBase PtBase.h "PtBase.h"
/// @brief AstBase に対応する実装用基底クラス
//////////////////////////////////////////////////////////////////////
class PtBase
{
public:

  /// @brief デストラクタ
  virtual
  ~PtBase() {};


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

  /// @brief 自身のアドレスを基にした識別値を返す．
  PtrIntType
  ptr() const
  {
    return reinterpret_cast<PtrIntType>(this);
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTBASE_H
