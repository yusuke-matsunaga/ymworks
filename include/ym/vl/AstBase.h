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


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

// in AstBase.h
class AstBase;
class AstNamedBase;
class AstHierNamedBase;

// in AstUdp.h
class AstUdp;
class AstUdpEntry;
class AstUdpValue;

// in AstModule.h
class AstModule;

// in AstPort.h
class AstPort;

// in AstDecl.h
class AstIOHead;
class AstIOItem;
class AstDeclHead;
class AstDeclItem;
class AstRange;

// in AstItem.h
class AstItem;
class AstDefParam;
class AstContAssign;
class AstInst;
class AstGenCaseItem;
class AstPathDecl;
class AstPathDelay;

// in AstStmt.h
class AstStmt;
class AstCaseItem;

// in AstExpr.h
class AstExpr;
class AstExprList;
class AstPart;

// in AstMisc.h
class AstStrength;
class AstDelay;
class AstControl;
class AstConnection;
class AstNameBranch;
class AstAttrInstList;
class AstAttrInst;
class AstAttrSpec;

using AstAttrInstVec    = std::vector<const AstAttrInst*>;
using AstAttrSpecVec    = std::vector<const AstAttrSpec*>;
using AstCaseItemVec    = std::vector<const AstCaseItem*>;
using AstConnectionVec  = std::vector<const AstConnection*>;
using AstContAssignVec  = std::vector<const AstContAssign*>;
using AstDeclHeadVec    = std::vector<const AstDeclHead*>;
using AstDeclItemVec    = std::vector<const AstDeclItem*>;
using AstDefParamVec    = std::vector<const AstDefParam*>;
using AstExprVec        = std::vector<const AstExpr*>;
using AstGenCaseItemVec = std::vector<const AstGenCaseItem*>;
using AstInstVec        = std::vector<const AstInst*>;
using AstIOHeadVec      = std::vector<const AstIOHead*>;
using AstIOItemVec      = std::vector<const AstIOItem*>;
using AstItemVec        = std::vector<const AstItem*>;
using AstModuleVec      = std::vector<const AstModule*>;
using AstNameBranchVec  = std::vector<const AstNameBranch*>;
using AstPortVec        = std::vector<const AstPort*>;
using AstRangeVec       = std::vector<const AstRange*>;
using AstStmtVec        = std::vector<const AstStmt*>;
using AstUdpVec         = std::vector<const AstUdp*>;
using AstUdpEntryVec    = std::vector<const AstUdpEntry*>;
using AstUdpValueVec    = std::vector<const AstUdpValue*>;


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

  /// @brief 階層ブランチの要素数の取得
  virtual
  SizeType
  namebranch_num() const = 0;

  /// @brief 先頭の階層ブランチを返す．
  ///
  /// 階層ブランチを持たない時は nullptr を返す．
  virtual
  const AstNameBranch*
  namebranch_top() const = 0;

  /// @brief 階層ブランチのリストを返す．
  virtual
  AstNameBranchVec
  namebranch_list() const = 0;

  /// @brief 階層名を展開する．
  std::string
  decompile_name() const;

};

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_STD

// AstModule へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::nsVerilog::AstModule*>
{
  SizeType
  operator()(YM_NAMESPACE::nsVerilog::AstModule* module) const
  {
    auto tmp = reinterpret_cast<PtrIntType>(module)/sizeof(void*);
    return static_cast<SizeType>(tmp);
  }
};

// const AstModule へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<const YM_NAMESPACE::nsVerilog::AstModule*>
{
  SizeType
  operator()(const YM_NAMESPACE::nsVerilog::AstModule* module) const
  {
    auto tmp = reinterpret_cast<PtrIntType>(module)/sizeof(void*);
    return static_cast<SizeType>(tmp);
  }
};

END_NAMESPACE_STD

#endif // YM_VL_ASTBASE_H
