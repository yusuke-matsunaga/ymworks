#ifndef PARSER_PTFWD_H
#define PARSER_PTFWD_H

/// @file PtFwd.h
/// @brief Pt クラスの名前の宣言のみおこなうヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

// in PtFactory.h
class PtFactory;

// in PtAttrInfo.h
class PtAttrInfo;

// in PtDecl.h
class PtIOHead;
class PtIOItem;
class PtDeclHead;
class PtDeclItem;
class PtRange;

// in PtExpr.h
class PtExpr;
class PtExprList;
class PtPart;

// in PtHierName.h
class PtHierName;

// in PtItem.h
class PtItem;
class PtDefParam;
class PtContAssign;
class PtInst;
class PtGenCaseItem;
class PtPathDecl;
class PtPathDelay;

// in PtMisc.h
class PtControl;
class PtConnection;
class PtStrength;
class PtDelay;
class PtNameBranch;
class PtAttrInstList;
class PtAttrInst;
class PtAttrSpec;

// in PtModule.h
class PtModule;

// in PtPort.h
class PtPort;

// in PtStmt.h
class PtStmt;
class PtCaseItem;

// in PtUdp.h
class PtUdp;
class PtUdpEntry;
class PtUdpValue;

END_NAMESPACE_YM_VERILOG

#endif // PTIFWD_H
