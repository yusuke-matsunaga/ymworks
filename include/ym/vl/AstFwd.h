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

using AstAttrInstList    = AstList<const AstAttrInst>;
using AstAttrSpecList    = AstList<const AstAttrSpec>;
using AstCaseItemList    = AstList<const AstCaseItem>;
using AstConnectionList  = AstList<const AstConnection>;
using AstContAssignList  = AstList<const AstContAssign>;
using AstDeclHeadList    = AstList<const AstDeclHead>;
using AstDeclItemList    = AstList<const AstDeclItem>;
using AstDefParamList    = AstList<const AstDefParam>;
using AstExprList        = AstList<const AstExpr>;
using AstGenCaseItemList = AstList<const AstGenCaseItem>;
using AstInstList        = AstList<const AstInst>;
using AstIOHeadList      = AstList<const AstIOHead>;
using AstIOItemList      = AstList<const AstIOItem>;
using AstItemList        = AstList<const AstItem>;
using AstModuleList      = AstList<const AstModule>;
using AstNameBranchList  = AstList<const AstNameBranch>;
using AstPortList        = AstList<const AstPort>;
using AstRangeList       = AstList<const AstRange>;
using AstStmtList        = AstList<const AstStmt>;
using AstUdpList         = AstList<const AstUdp>;
using AstUdpEntryList    = AstList<const AstUdpEntry>;
using AstUdpValueList    = AstList<const AstUdpValue>;

END_NAMESPACE_YM_VERILOG

#endif // ASTFWD_H
