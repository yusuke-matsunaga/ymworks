#ifndef YM_VL_ASTBASE_H
#define YM_VL_ASTBASE_H

/// @file ym/vl/AstBase.h
/// @brief Ast の基底クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"
#include "ym/vl/AstFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 全ての Ast クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class AstBase
{
public:

  /// @brief デストラクタ
  virtual
  ~AstBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstNamedBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 名前を持つ AstBase
//////////////////////////////////////////////////////////////////////
class AstNamedBase :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstNamedBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstHierNamedBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 階層付き名前を持つ AstBase
//////////////////////////////////////////////////////////////////////
class AstHierNamedBase :
  public AstNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstHierNamedBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  virtual
  AstNameBranchList
  namebranch_list() const = 0;

  /// @brief 階層名を展開する．
  std::string
  decompile_name() const;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTBASE_H
