
/// @file CptFactory.cc
/// @brief CptFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/CptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// Pt クラス(の継承クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptFactory::CptFactory(
  Alloc& alloc
) : PtiFactory(alloc)
{
}

// デストラクタ
CptFactory::~CptFactory()
{
}

// 確保したオブジェクトに関する情報を出力する．
void
CptFactory::dump_profile(
  std::ostream& s
) const
{
  s << "mNumDefStub " << mNumDefStub<< std::endl;
  s << "mNumUdp " << mNumUdp<< std::endl;
  s << "mNumUdpEntry " << mNumUdpEntry<< std::endl;
  s << "mNumUdpEntryS " << mNumUdpEntryS<< std::endl;
  s << "mNumUdpValue " << mNumUdpValue<< std::endl;
  s << "mNumModule " << mNumModule<< std::endl;
  s << "mNumPort " << mNumPort<< std::endl;
  s << "mNumPortRef " << mNumPortRef<< std::endl;
  s << "mNumPortRefI " << mNumPortRefI<< std::endl;
  s << "mNumPortRefR " << mNumPortRefR<< std::endl;
  s << "mNumIOH " << mNumIOH<< std::endl;
  s << "mNumIOHV " << mNumIOHV<< std::endl;
  s << "mNumIOItem " << mNumIOItem<< std::endl;
  s << "mNumIOItemI " << mNumIOItemI<< std::endl;
  s << "mNumParamH " << mNumParamH<< std::endl;
  s << "mNumParamHV " << mNumParamHV<< std::endl;
  s << "mNumParamHT " << mNumParamHT<< std::endl;
  s << "mNumLocalParamH " << mNumLocalParamH<< std::endl;
  s << "mNumLocalParamHV " << mNumLocalParamHV<< std::endl;
  s << "mNumLocalParamHT " << mNumLocalParamHT<< std::endl;
  s << "mNumSpecParamH " << mNumSpecParamH<< std::endl;
  s << "mNumSpecParamHV " << mNumSpecParamHV<< std::endl;
  s << "mNumEventH " << mNumEventH<< std::endl;
  s << "mNumGenvarH " << mNumGenvarH<< std::endl;
  s << "mNumVarH " << mNumVarH<< std::endl;
  s << "mNumRegH " << mNumRegH<< std::endl;
  s << "mNumRegHV " << mNumRegHV<< std::endl;
  s << "mNumNetH " << mNumNetH<< std::endl;
  s << "mNumNetHS " << mNumNetHS<< std::endl;
  s << "mNumNetHD " << mNumNetHD<< std::endl;
  s << "mNumNetHSD " << mNumNetHSD<< std::endl;
  s << "mNumNetHV " << mNumNetHV<< std::endl;
  s << "mNumNetHVS " << mNumNetHVS<< std::endl;
  s << "mNumNetHVD " << mNumNetHVD<< std::endl;
  s << "mNumNetHVSD " << mNumNetHVSD<< std::endl;
  s << "mNumDeclItem " << mNumDeclItem<< std::endl;
  s << "mNumDeclItemI " << mNumDeclItemI<< std::endl;
  s << "mNumDeclItemR " << mNumDeclItemR<< std::endl;
  s << "mNumDefParamH " << mNumDefParamH<< std::endl;
  s << "mNumDefParam " << mNumDefParam<< std::endl;
  s << "mNumContAssignH " << mNumContAssignH<< std::endl;
  s << "mNumContAssignHS " << mNumContAssignHS<< std::endl;
  s << "mNumContAssignHD " << mNumContAssignHD<< std::endl;
  s << "mNumContAssignHSD " << mNumContAssignHSD<< std::endl;
  s << "mNumContAssign " << mNumContAssign<< std::endl;
  s << "mNumInitial " << mNumInitial<< std::endl;
  s << "mNumAlways " << mNumAlways<< std::endl;
  s << "mNumTask " << mNumTask<< std::endl;
  s << "mNumFunction " << mNumFunction<< std::endl;
  s << "mNumSizedFunc " << mNumSizedFunc<< std::endl;
  s << "mNumTypedFunc " << mNumTypedFunc<< std::endl;
  s << "mNumGateH " << mNumGateH<< std::endl;
  s << "mNumGateHS " << mNumGateHS<< std::endl;
  s << "mNumGateHD " << mNumGateHD<< std::endl;
  s << "mNumGateHSD " << mNumGateHSD<< std::endl;
  s << "mNumMuH " << mNumMuH<< std::endl;
  s << "mNumMuHS " << mNumMuHS<< std::endl;
  s << "mNumMuHD " << mNumMuHD<< std::endl;
  s << "mNumMuHSD " << mNumMuHSD<< std::endl;
  s << "mNumMuHP " << mNumMuHP<< std::endl;
  s << "mNumInst " << mNumInst<< std::endl;
  s << "mNumInstN " << mNumInstN<< std::endl;
  s << "mNumInstR " << mNumInstR<< std::endl;
  s << "mNumGenerate " << mNumGenerate<< std::endl;
  s << "mNumGenBlock " << mNumGenBlock<< std::endl;
  s << "mNumGenBlockN " << mNumGenBlockN<< std::endl;
  s << "mNumGenIf " << mNumGenIf<< std::endl;
  s << "mNumGenCase " << mNumGenCase<< std::endl;
  s << "mNumGenCaseItem " << mNumGenCaseItem<< std::endl;
  s << "mNumGenFor " << mNumGenFor<< std::endl;
  s << "mNumSpecItem " << mNumSpecItem<< std::endl;
  s << "mNumSpecPath " << mNumSpecPath<< std::endl;
  s << "mNumPathDecl " << mNumPathDecl<< std::endl;
  s << "mNumPathDelay " << mNumPathDelay<< std::endl;
  s << "mNumDisable " << mNumDisable<< std::endl;
  s << "mNumDisableH " << mNumDisableH<< std::endl;
  s << "mNumEnable " << mNumEnable<< std::endl;
  s << "mNumEnableH " << mNumEnableH<< std::endl;
  s << "mNumSysEnable " << mNumSysEnable<< std::endl;
  s << "mNumDcStmt " << mNumDcStmt<< std::endl;
  s << "mNumEcStmt " << mNumEcStmt<< std::endl;
  s << "mNumWait " << mNumWait<< std::endl;
  s << "mNumAssign " << mNumAssign<< std::endl;
  s << "mNumAssignC " << mNumAssignC<< std::endl;
  s << "mNumNbAssign " << mNumNbAssign<< std::endl;
  s << "mNumNbAssignC " << mNumNbAssignC<< std::endl;
  s << "mNumEventStmt " << mNumEventStmt<< std::endl;
  s << "mNumNullStmt " << mNumNullStmt<< std::endl;
  s << "mNumIf " << mNumIf<< std::endl;
  s << "mNumCase " << mNumCase<< std::endl;
  s << "mNumCaseX " << mNumCaseX<< std::endl;
  s << "mNumCaseZ " << mNumCaseZ<< std::endl;
  s << "mNumCaseItem " << mNumCaseItem<< std::endl;
  s << "mNumForever " << mNumForever<< std::endl;
  s << "mNumRepeat " << mNumRepeat<< std::endl;
  s << "mNumWhile " << mNumWhile<< std::endl;
  s << "mNumFor " << mNumFor<< std::endl;
  s << "mNumPcAssign " << mNumPcAssign<< std::endl;
  s << "mNumDeassign " << mNumDeassign<< std::endl;
  s << "mNumForce " << mNumForce<< std::endl;
  s << "mNumRelease " << mNumRelease<< std::endl;
  s << "mNumParBlock " << mNumParBlock<< std::endl;
  s << "mNumParBlockN " << mNumParBlockN<< std::endl;
  s << "mNumSeqBlock " << mNumSeqBlock<< std::endl;
  s << "mNumSeqBlockN " << mNumSeqBlockN<< std::endl;
  s << "mNumOpr1 " << mNumOpr1<< std::endl;
  s << "mNumOpr2 " << mNumOpr2<< std::endl;
  s << "mNumOpr3 " << mNumOpr3<< std::endl;
  s << "mNumConcat " << mNumConcat<< std::endl;
  s << "mNumMultiConcat " << mNumMultiConcat<< std::endl;
  s << "mNumMinTypMax3 " << mNumMinTypMax3<< std::endl;
  s << "mNumPrimary " << mNumPrimary<< std::endl;
  s << "mNumPrimaryI " << mNumPrimaryI<< std::endl;
  s << "mNumPrimaryR " << mNumPrimaryR<< std::endl;
  s << "mNumPrimaryIR " << mNumPrimaryIR<< std::endl;
  s << "mNumPrimaryH " << mNumPrimaryH<< std::endl;
  s << "mNumPrimaryHI " << mNumPrimaryHI<< std::endl;
  s << "mNumPrimaryHR " << mNumPrimaryHR<< std::endl;
  s << "mNumPrimaryHIR " << mNumPrimaryHIR<< std::endl;
  s << "mNumPrimaryCI " << mNumPrimaryCI<< std::endl;
  s << "mNumPrimaryCR " << mNumPrimaryCR<< std::endl;
  s << "mNumPrimaryHCI " << mNumPrimaryHCI<< std::endl;
  s << "mNumFuncCall " << mNumFuncCall<< std::endl;
  s << "mNumFuncCallH " << mNumFuncCallH<< std::endl;
  s << "mNumSysFuncCall " << mNumSysFuncCall<< std::endl;
  s << "mNumIntConstant1 " << mNumIntConstant1<< std::endl;
  s << "mNumIntConstant2 " << mNumIntConstant2<< std::endl;
  s << "mNumIntConstant3 " << mNumIntConstant3<< std::endl;
  s << "mNumRealConstant " << mNumRealConstant<< std::endl;
  s << "mNumStringConstant " << mNumStringConstant<< std::endl;
  s << "mNumDelayControl " << mNumDelayControl<< std::endl;
  s << "mNumEventControl " << mNumEventControl<< std::endl;
  s << "mNumRepeatControl " << mNumRepeatControl<< std::endl;
  s << "mNumOrderedCon " << mNumOrderedCon<< std::endl;
  s << "mNumAiOrderedCon " << mNumAiOrderedCon<< std::endl;
  s << "mNumNamedCon " << mNumNamedCon<< std::endl;
  s << "mNumAiNamedCon " << mNumAiNamedCon<< std::endl;
  s << "mNumStrength " << mNumStrength<< std::endl;
  s << "mNumDelay " << mNumDelay<< std::endl;
  s << "mNumNameBranch " << mNumNameBranch<< std::endl;
  s << "mNumNameBranchI " << mNumNameBranchI<< std::endl;
  s << "mNumRange " << mNumRange << std::endl;
  s << "mNumAttrInst " << mNumAttrInst<< std::endl;
  s << "mNumAttrSpec " << mNumAttrSpec<< std::endl;
}

END_NAMESPACE_YM_VERILOG
