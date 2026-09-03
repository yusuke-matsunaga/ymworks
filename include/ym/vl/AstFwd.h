#ifndef ASTFWD_H
#define ASTFWD_H

/// @file AstFwd.h
/// @brief AstFwd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/AstList.h"


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
class AstPart;

// in AstMisc.h
class AstStrength;
class AstDelay;
class AstControl;
class AstConnection;
class AstNameBranch;
class AstAttrInst;
class AstAttrSpec;

using AstAttrInstVec    = std::vector<AstAttrInst>;
using AstAttrSpecVec    = std::vector<AstAttrSpec>;
using AstCaseItemVec    = std::vector<AstCaseItem>;
using AstConnectionVec  = std::vector<AstConnection>;
using AstContAssignVec  = std::vector<AstContAssign>;
using AstDeclHeadVec    = std::vector<AstDeclHead>;
using AstDeclItemVec    = std::vector<AstDeclItem>;
using AstDefParamVec    = std::vector<AstDefParam>;
using AstExprVec        = std::vector<AstExpr>;
using AstGenCaseItemVec = std::vector<AstGenCaseItem>;
using AstInstVec        = std::vector<AstInst>;
using AstIOHeadVec      = std::vector<AstIOHead>;
using AstIOItemVec      = std::vector<AstIOItem>;
using AstItemVec        = std::vector<AstItem>;
using AstModuleVec      = std::vector<AstModule>;
using AstNameBranchVec  = std::vector<AstNameBranch>;
using AstPortVec        = std::vector<AstPort>;
using AstRangeVec       = std::vector<AstRange>;
using AstStmtVec        = std::vector<AstStmt>;
using AstUdpVec         = std::vector<AstUdp>;
using AstUdpEntryVec    = std::vector<AstUdpEntry>;
using AstUdpValueVec    = std::vector<AstUdpValue>;

using AstAttrInstList    = AstList<AstAttrInst>;
using AstAttrSpecList    = AstList<AstAttrSpec>;
using AstCaseItemList    = AstList<AstCaseItem>;
using AstConnectionList  = AstList<AstConnection>;
using AstContAssignList  = AstList<AstContAssign>;
using AstDeclHeadList    = AstList<AstDeclHead>;
using AstDeclItemList    = AstList<AstDeclItem>;
using AstDefParamList    = AstList<AstDefParam>;
using AstExprList        = AstList<AstExpr>;
using AstGenCaseItemList = AstList<AstGenCaseItem>;
using AstInstList        = AstList<AstInst>;
using AstIOHeadList      = AstList<AstIOHead>;
using AstIOItemList      = AstList<AstIOItem>;
using AstItemList        = AstList<AstItem>;
using AstModuleList      = AstList<AstModule>;
using AstNameBranchList  = AstList<AstNameBranch>;
using AstPortList        = AstList<AstPort>;
using AstRangeList       = AstList<AstRange>;
using AstStmtList        = AstList<AstStmt>;
using AstUdpList         = AstList<AstUdp>;
using AstUdpEntryList    = AstList<AstUdpEntry>;
using AstUdpValueList    = AstList<AstUdpValue>;

END_NAMESPACE_YM_VERILOG

#endif // ASTFWD_H
