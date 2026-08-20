
/// @file AstDumper.cc
/// @brief AstDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/AstDumper.h"
#include "ym/vl/AstModule.h"
#include "ym/vl/AstUdp.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/AstDecl.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstStmt.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstMisc.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/VlUdpVal.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstHeader AstDumper.cc "AstDumper.cc"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief マーカを出力するクラス
//////////////////////////////////////////////////////////////////////
class AstHeader
{
public:

  /// @brief コンストラクタ
  AstHeader(
    AstDumper& s,         ///< [in] 出力ストリーム
    const char* name,    ///< [in] 名前
    const char* type,    ///< [in] 属性
    bool need_cr = true  ///< [in] マーカの直後に改行するとき true とするフラグ
  );

  /// @brief デストラクタ
  ~AstHeader();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  AstDumper& mS;

  // マーカー名
  std::string mName;

};


//////////////////////////////////////////////////////////////////////
// Ast クラスの内容を出力するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstDumper::AstDumper(
  std::ostream& s
) : mStream(s),
    mIndent(0),
    mFileLocMode{false}
{
}

// @brief デストラクタ
AstDumper::~AstDumper()
{
}

// @brief ファイル位置情報を出力するモードににする
void
AstDumper::enable_file_loc_mode()
{
  mFileLocMode = true;
}

// @brief ファイル位置情報の出力を抑制するモードにする
void
AstDumper::disable_file_loc_mode()
{
  mFileLocMode = false;
}

// @brief ファイル位置情報の出力モードの取得
bool
AstDumper::file_loc_mode() const
{
  return mFileLocMode;
}

// @brief パーサーの内部情報の表示
void
AstDumper::put(
  const std::vector<const AstUdp*>& udp_list,
  const std::vector<const AstModule*>& module_list
)
{
  for ( auto udp: udp_list ) {
    put(udp);
  }
  for ( auto module: module_list ) {
    put(module);
  }
}

// @brief UDP を表示する
void
AstDumper::put(
  const AstUdp* udp
)
{
  AstHeader x(*this, "UDP", "UDP");

  put("mFileRegion", udp->file_region());
#if 0
  put("mAttrInst",  udp->attr_top());
#endif

  put("mPrimType", udp->prim_type());

  put("mName", udp->name());

  for ( auto port: udp->port_list() ) {
    put("mPort", port->ext_name());
  }
  for ( auto io: udp->iohead_list() ) {
    put("mIO", io);
  }

  put("mInitial", udp->init_value());

  for ( auto entry: udp->table_list() ) {
    AstHeader x(*this, "mTable", "UdpEntry");

    put("mFileRegion", entry->file_region());

    for ( auto v: entry->input_list() ) {
      put("mInput", v);
    }
    put("mCurrent", entry->current());
    put("mOutput", entry->output());
  }
}

// @brief UDP テーブルの値を表示する
void
AstDumper::put(
  const char* label,
  const AstUdpValue* v
)
{
  if ( v == nullptr ) {
    return;
  }

  AstHeader x(*this, label, "UdpValue");

  put("mFileRegion", v->file_region());
  put("mSymbol", v->symbol().to_string());
}

BEGIN_NONAMESPACE

// 時間の単位を表す文字列を得る．
std::string
unit2str(
  int unit
)
{
  if ( unit > 2 || unit < -15 ) {
    return "illegal time unit";
  }

  unit += 15;
  std::string ans;
  switch ( unit % 3 ) {
  case 0: ans = "1"; break;
  case 1: ans = "10"; break;
  case 2: ans = "100"; break;
  }
  switch ( unit / 3 ) {
  case 5: ans += "s"; break;
  case 4: ans += "ms"; break;
  case 3: ans += "us"; break;
  case 2: ans += "ns"; break;
  case 1: ans += "ps"; break;
  case 0: ans += "fs"; break;
  }
  return ans;
}

END_NONAMESPACE

// @brief module を表示する
void
AstDumper::put(
  const AstModule* m
)
{
  AstHeader x(*this, "Module", "Module");

  put("mFileRegion", m->file_region());
#if 0
  put("mAttrInst", m->attr_top());
#endif

  put("mName", m->name());

  put("mCellDefine", m->is_cell());
  put("mProtected", m->is_protected());
  put("mMacroModule", m->is_macromodule());

  if ( m->time_unit() != -16 ) {
    put("mTimeUnit", unit2str(m->time_unit()));
    put("mTimePrecision", unit2str(m->time_precision()));
  }

  put("mDefNetType", m->nettype());
  put("mUnconnDrive", m->unconn_drive());
  put("mDelayMode", m->delay_mode());
  put("mDecayTime", m->decay_time());
  put("config", m->config());
  put("library", m->library());
  put("cell", m->cell());

  for ( auto param: m->paramport_list() ) {
    put("mParamPort", param);
  }

  for ( auto port: m->port_list() ) {
    AstHeader x(*this, "mPort", "Port");

    put("mFileRegion", port->file_region());
    if ( port->ext_name() != nullptr ) {
      put("mExprname", port->ext_name());
    }

    if ( port->portref_size() == 1 ) {
      auto expr = port->expr();
      auto dir = port->portref_dir(0);
      put_portref(expr, dir);
    }
    else if ( port->portref_size() > 1 ) {
      for ( SizeType index = 0; index < port->portref_size(); ++ index ) {
	auto expr = port->portref(index);
	auto dir = port->portref_dir(index);
	put_portref(expr, dir);
      }
    }
  }

  put_decls(m->iohead_list(), m->declhead_list());
  for ( auto item: m->item_list() ) {
    put("mItem", item);
  }
}

void
AstDumper::put_portref(
  const AstExpr* expr,
  VpiDir dir
)
{
  AstHeader x(*this, "mPortRef", "PortRef");

  put("mFileRegion", expr->file_region());
  put("mDir", dir);
  put("mName", expr->name());
  if ( expr->index_num() == 1 ) {
    put("mIndex", expr->index_list().front());
  }
  else {
    ASSERT_COND( expr->index_num() == 0 );
  }
  if ( expr->part() != nullptr ) {
    auto part = expr->part();
    put("mRangeMode", part->mode());
    put("mLeftRange", part->left());
    put("mRightRange", part->right());
  }
}

// @brief IO のヘッダを出力する．
void
AstDumper::put(
  const char* label,
  const AstIOHead* io
)
{
  const char* nm = nullptr;
  switch ( io->direction() ) {
  case VpiDir::Input:  nm = "Input"; break;
  case VpiDir::Output: nm = "Output"; break;
  case VpiDir::Inout:  nm = "Inout"; break;
  default: ASSERT_NOT_REACHED;
  }
  AstHeader x(*this, label, nm);


  put("mFileRegion", io->file_region());
#if 0
  put("mAttrInst", io->attr_top());
#endif

  put("mAuxType", io->aux_type());
  put("mNetType", io->net_type());
  put("mVarType", io->var_type());
  put("mSigned", io->is_signed());
  put("mLeftRange", io->range()->left());
  put("mRightRange", io->range()->right());

  for ( auto item: io->item_list() ) {
    AstHeader x(*this, "mElem", "IOElem");

    put("mFileRegion", item->file_region());
    put("mName", item->name());
    put("mInitValue", item->init_value());
  }
}

// @brief 宣言のヘッダを出力する．
void
AstDumper::put(
  const char* label,
  const AstDeclHead* decl
)
{
  const char* nm = nullptr;
  switch ( decl->type() ) {
  case AstDeclHead::Param:
    switch ( decl->data_type() ) {
    case VpiVarType::None:            nm = "Parameter"; break;
    case VpiVarType::Integer:         nm = "Parameter(integer)";  break;
    case VpiVarType::Real:            nm = "Parameter(real)"; break;
    case VpiVarType::Time:            nm = "Parameter(time)"; break;
    case VpiVarType::Realtime:        nm = "Parameter(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case AstDeclHead::LocalParam:
    switch ( decl->data_type() ) {
    case VpiVarType::None:            nm = "Localparam"; break;
    case VpiVarType::Integer:         nm = "Localparam(integer)";  break;
    case VpiVarType::Real:            nm = "Localparam(real)"; break;
    case VpiVarType::Time:            nm = "Localparam(time)"; break;
    case VpiVarType::Realtime:        nm = "Localparam(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case AstDeclHead::Reg:            nm = "Reg";        break;
  case AstDeclHead::Var:
    switch ( decl->data_type() ) {
    case VpiVarType::Integer:         nm = "Integer";    break;
    case VpiVarType::Real:            nm = "Real";       break;
    case VpiVarType::Time:            nm = "Time";       break;
    case VpiVarType::Realtime:        nm = "Realtime";  break;
    case VpiVarType::None: ASSERT_NOT_REACHED;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case AstDeclHead::Genvar:         nm = "Genvar";     break;
  case AstDeclHead::Event:          nm = "Event";      break;
  case AstDeclHead::SpecParam:      nm = "Specparam";  break;
  case AstDeclHead::Net:
    switch ( decl->net_type() ) {
    case VpiNetType::Supply0:          nm = "Supply0"; break;
    case VpiNetType::Supply1:          nm = "Supply1"; break;
    case VpiNetType::Tri:              nm = "Tri"; break;
    case VpiNetType::TriAnd:           nm = "Triand"; break;
    case VpiNetType::TriOr:            nm = "Trior"; break;
    case VpiNetType::TriReg:           nm = "Trireg"; break;
    case VpiNetType::Tri0:             nm = "Tri0"; break;
    case VpiNetType::Tri1:             nm = "Tri1"; break;
    case VpiNetType::Wire:             nm = "Wire"; break;
    case VpiNetType::Wand:             nm = "Wand"; break;
    case VpiNetType::Wor:              nm = "Wor"; break;
    case VpiNetType::None:             nm = "None"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  default: ASSERT_NOT_REACHED; break;
  }
  AstHeader x(*this, label, nm);

  put("mFileRegion", decl->file_region());
#if 0
  put("mAttrInst", decl->attr_top());
#endif

  put("mSigned", decl->is_signed());
  put("mLeftRange", decl->range()->left());
  put("mRightRange", decl->range()->right());
  put("mVsType", decl->vs_type());
  put("mStrength", decl->strength());
  put("mDelay", decl->delay());

  for ( auto item: decl->item_list() ) {
    AstHeader x(*this, "mElem", "DeclItem");

    put("mFileRegion", item->file_region());
    put("mName", item->name());
    for ( SizeType i = 0; i < item->range_num(); ++ i ) {
      auto range = item->range(i);
      AstHeader x(*this, "mDimension", "Range");

      put("mLeftRange", range->left());
      put("mRightRange", range->right());
    }
    put("mInitValue", item->init_value());
  }
}

// @brief 要素の出力
void
AstDumper::put(
  const char* label,
  const AstItem* item
)
{
  if ( item == nullptr ) {
    return;
  }

  const char* nm = nullptr;
  switch ( item->type() ) {
  case AstItem::DefParam:        nm = "DefparamHeader"; break;
  case AstItem::ContAssign:      nm = "ContAssignHeader"; break;
  case AstItem::Initial:         nm = "Initial"; break;
  case AstItem::Always:          nm = "Always"; break;
  case AstItem::Task:            nm = "Task"; break;
  case AstItem::Func:
    switch ( item->data_type() ) {
    case VpiVarType::None:            nm = "Function"; break;
    case VpiVarType::Integer:         nm = "Function(integer)"; break;
    case VpiVarType::Real:            nm = "Function(real)"; break;
    case VpiVarType::Time:            nm = "Function(time)"; break;
    case VpiVarType::Realtime:        nm = "Function(realtime)"; break;
    default: ASSERT_NOT_REACHED; break;
    }
    break;
  case AstItem::GateInst:        nm = "GateHeader"; break;
  case AstItem::MuInst:          nm = "MuHeader"; break;
  case AstItem::SpecItem:
    switch ( item->specitem_type() ) {
    case VpiSpecItemType::PulsestyleOnEvent:  nm = "Pulse_onevent"; break;
    case VpiSpecItemType::PulsestyleOnDetect: nm = "Pulse_ondetect"; break;
    case VpiSpecItemType::Showcancelled:      nm = "Showcancelled"; break;
    case VpiSpecItemType::Noshowcancelled:    nm = "Noshowcancelld"; break;
    default: ASSERT_NOT_REACHED; break;
    }
  case AstItem::SpecPath:        nm = "SpecPath"; break;
  case AstItem::Generate:        nm = "Generate"; break;
  case AstItem::GenBlock:        nm = "GenBlock"; break;
  case AstItem::GenIf:           nm = "GenIf"; break;
  case AstItem::GenCase:         nm = "GenCase"; break;
  case AstItem::GenFor:          nm = "GenFor"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  AstHeader x(*this, label, nm);

  put("mFileRegions", item->file_region());
#if 0
  put("mAttrInst", item->attr_top());
#endif

  switch ( item->type() ) {
  case AstItem::DefParam:
    for ( auto dp: item->defparam_list() ) {
      AstHeader x(*this, "mElem", "DefParam");

      put("mFileRegion", dp->file_region());
      put(dp->namebranch_list());
      put("mName", dp->name());
      put("mValue", dp->expr());
    }
    break;

  case AstItem::ContAssign:
    put("mStrength", item->strength());
    put("mDelay", item->delay());
    for ( auto ca: item->contassign_list() ) {
      AstHeader x(*this, "mElem", "ContAssign");

      put("mFileRegion", ca->file_region());
      put("mLhs", ca->lhs());
      put("mRhs", ca->rhs());
    }
    break;

  case AstItem::Initial:
  case AstItem::Always:
    put("mBody", item->body());
    break;

  case AstItem::Task:
  case AstItem::Func:
    put("mAutomatic", item->automatic());
    if ( item->range() ) {
      put("mSigned", item->is_signed());
      put("mLeftRange", item->range()->left());
      put("mRightRange", item->range()->right());
    }
    put("mName", item->name());
    put_decls(item->iohead_list(),
	      item->declhead_list());
    put("mBody", item->body());
    break;

  case AstItem::GateInst:
    put("mPrimType", item->prim_type());
    put("mStrength", item->strength());
    put("mDelay", item->delay());
    for ( auto gi: item->inst_list() ) {
      AstHeader x(*this, "mElem", "GateInst");

      put("mFileRegion", gi->file_region());
      if ( gi->name() != nullptr ) {
	put("mName", gi->name());
      }
      if ( gi->range() ) {
	put("mLeftRange", gi->range()->left());
	put("mRightrange", gi->range()->right());
      }
      for ( auto con: gi->port_list() ) {
	put("mPortCon", con);
      }
    }
    break;

  case AstItem::MuInst:
    put("mDefName", item->name());
    for ( auto con: item->paramassign_list() ) {
      put("mParamCon", con);
    }
    put("mStrength", item->strength());
    put("mDelay", item->delay());
    for ( auto mui: item->inst_list() ) {
      AstHeader x(*this, "mElem", "MuInst");

      put("mFileRegion", mui->file_region());
      put("mName", mui->name());
      if ( mui->range() ) {
	put("mLeftRange", mui->range()->left());
	put("mRightRange", mui->range()->right());
      }
      for ( auto con: mui->port_list() ) {
	put("mPortCon", con);
      }
    }
    break;

  case AstItem::SpecItem:
    for ( auto expr: item->terminal_list() ) {
      put("mTerminal", expr);
    }
    break;

  case AstItem::SpecPath:
#if 0 // PATH_DECL
    switch ( item->specpath_type() ) {
    case kVpiSpecPathNull:
      break;
    case kVpiSpecPathIf:
      put("mIf", item->expr());
      break;
    case kVpiSpecPathIfnone:
      {
	AstHeader x3(s, "mIfnone");
      }
      break;
    }
    {
      AstHeader x4(s, "mPathDecl");
      s << item->path_decl();
    }
#endif
    break;

  case AstItem::Generate:
  case AstItem::GenBlock:
    if ( item->name() != nullptr ) {
      put("mName", item->name());
    }
    put_decl_item("mBody", item->declhead_list(), item->item_list());
    break;

  case AstItem::GenIf:
    put("mCond", item->cond_expr());
    put_decl_item("mThenBody", item->then_declhead_list(), item->then_item_list());
    put_decl_item("mElseBody", item->else_declhead_list(), item->else_item_list());
    break;

  case AstItem::GenCase:
    put("mExpr", item->cond_expr());
    for ( auto gci: item->caseitem_list() ) {
      AstHeader x(*this, "mCaseItem", "GenCaseItem");

      put("mFileRegion", gci->file_region());
      for ( auto expr: gci->label_list() ) {
	put("mLabel", expr);
      }
      put_decl_item("mBody", gci->declhead_list(), gci->item_list());
    }
    break;

  case AstItem::GenFor:
    put("mLoopVar", item->loop_var());
    put("mInitehExpr", item->init_expr());
    put("mCond", item->cond_expr());
    put("mNext", item->next_expr());
    put("mName", item->name());
    put_decl_item("mBody", item->declhead_list(), item->item_list());
    break;

  default: ASSERT_NOT_REACHED; break;
  }
}

#if 0 // PATH_DECL
// path decl を表示する．
void
AstDumper::put(
  const char* label,
  const AstPathDecl* item
)
{
  AstHeader x(*this, label, "PathDecl");

  put("mFileRegion", item->file_region());

  switch ( item->edge() ) {
  case vpiPosedgeOp:
    { AstHeader x2(s, "mPosedge"); }
    break;
  case vpiNegedgeOp:
    { AstHeader x3(s, "mNegedge"); }
    break;
  }

  ymuint i = 0;
  for ( auto ei = item->input_top(); ei; ei = ei->next(), ++ i ) {
    AstHeader x4(s, "mInput", i);
    s << ei->expr();
  }

  if ( item->input_pol() ) {
    AstHeader x5(s, "InputPol");
    s << static_cast<char>(item->input_pol());
  }

  {
    AstHeader x6(s, "mOp");
    switch ( item->op() ) {
    case vpiPathParallel: s << "PathParallel(=>)"; break;
    case vpiPathFull:     s << "PathFull(*>)"; break;
    }
  }

  i = 0;
  for ( auto ei = item->output_top(); ei; ei = ei->next(), ++ i) {
    AstHeader x7(s, "mOutput", i);
    s << ei->expr();
  }

  if ( item->output_pol() ) {
    AstHeader x8(s, "mOutputPol");
    s << static_cast<char>(item->output_pol());
  }

  if ( item->expr() ) {
    AstHeader x9(s, "mExpr");
    s << item->expr();
  }

  {
    AstHeader x10(s, "mPathDelay");
    s << item->path_delay();
  }
}

// path delay を表示する．
AstDumper&
operator<<(
  AstDumper& s,
  const AstPathDelay* item
)
{
  AstHeader x1(s, "PathDelay");

  s << item->file_region();

  for ( int i = 0; i < 12; ++ i ) {
    if ( item->value(i) == nullptr ) {
      break;
    }
    AstHeader x2(s, "mValue", i);
    s << item->value(i);
  }

  return s;
}
#endif

// @brief ステートメントの出力
void
AstDumper::put(
  const char* label,
  const AstStmt* stmt
)
{
  if ( stmt == nullptr ) {
    return;
  }

  const char* nm = nullptr;
  switch ( stmt->type() ) {
  case AstStmt::Disable:       nm = "Disable"; break;
  case AstStmt::Enable:        nm = "Enable"; break;
  case AstStmt::SysEnable:     nm = "SysEnable"; break;
  case AstStmt::DelayControl:            nm = "ControlStmt"; break;
  case AstStmt::EventControl:            nm = "ControlStmt"; break;
  case AstStmt::Assign:        nm = "Assign"; break;
  case AstStmt::NbAssign:      nm = "NonBlockingAssign"; break;
  case AstStmt::Event:         nm = "EventTrigger"; break;
  case AstStmt::Null:          nm = "NullStmt"; break;
  case AstStmt::If:            nm = "If"; break;
  case AstStmt::Case:          nm = "Case"; break;
  case AstStmt::CaseX:         nm = "Casex"; break;
  case AstStmt::CaseZ:         nm = "Casez"; break;
  case AstStmt::Wait:          nm = "Wait"; break;
  case AstStmt::Forever:       nm = "Forever"; break;
  case AstStmt::Repeat:        nm = "Repeat"; break;
  case AstStmt::While:         nm = "While"; break;
  case AstStmt::For:           nm = "For"; break;
  case AstStmt::PcAssign:      nm = "PcAssign"; break;
  case AstStmt::Deassign:      nm = "Deassign"; break;
  case AstStmt::Force:         nm = "Force"; break;
  case AstStmt::Release:       nm = "Release"; break;
  case AstStmt::ParBlock:      nm = "Fork"; break;
  case AstStmt::SeqBlock:      nm = "Begin"; break;
  case AstStmt::NamedParBlock: nm = "Fork"; break;
  case AstStmt::NamedSeqBlock: nm = "Begin"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  AstHeader x(*this, label, nm);

  put("mFileRegion", stmt->file_region());
#if 0
  put("mAttrInst", stmt->attr_top());
#endif

  switch ( stmt->type() ) {
  case AstStmt::Disable:
    put(stmt->namebranch_list());
    put("mName", stmt->name());
    break;

  case AstStmt::Enable:
  case AstStmt::SysEnable:
    put(stmt->namebranch_list());
    put("mName", stmt->name());
    for ( auto arg: stmt->arg_list() ) {
      if ( arg ) {
	put("mArg", arg);
      }
      else {
	put("mArg", "null");
      }
    }
    break;

  case AstStmt::DelayControl:
  case AstStmt::EventControl:
    put("mControl", stmt->control());
    put("mBody", stmt->body());
    break;

  case AstStmt::Wait:
    put("mExpr", stmt->expr());
    put("mBody", stmt->body());
    break;

  case AstStmt::Assign:
  case AstStmt::NbAssign:
  case AstStmt::PcAssign:
  case AstStmt::Force:
  case AstStmt::Deassign:
  case AstStmt::Release:
    put("mControl", stmt->control());
    put("mLhs", stmt->lhs());
    put("mRhs", stmt->rhs());
    break;

  case AstStmt::Event:
    put("mEventName", stmt->primary());
    break;

  case AstStmt::Null:
    break;

  case AstStmt::If:
    put("mCond", stmt->expr());
    put("mThen", stmt->body());
    put("mElse", stmt->else_body());
    break;

  case AstStmt::Case:
  case AstStmt::CaseX:
  case AstStmt::CaseZ:
    put("mExpr", stmt->expr());
    for ( auto ci: stmt->caseitem_list() ) {
      AstHeader x(*this, "mCaseItem", "CaseItem");

      put("mFileRegion", ci->file_region());
      for ( auto expr: ci->label_list() ) {
	put("mLabel", expr);
      }
      put("mBody", ci->body());
    }
    break;

  case AstStmt::Forever:
  case AstStmt::Repeat:
  case AstStmt::While:
  case AstStmt::For:
    put("mInit", stmt->init_stmt());
    put("mExpr", stmt->expr());
    put("mNext", stmt->next_stmt());
    put("mBody", stmt->body());
    break;

  case AstStmt::NamedParBlock:
  case AstStmt::NamedSeqBlock:
    put("mName", stmt->name());
    // 次の case にわざと継続する．

  case AstStmt::ParBlock:
  case AstStmt::SeqBlock:
    for ( auto head: stmt->declhead_list() ) {
      put("mDecl", head);
    }
    for ( auto stmt1: stmt->stmt_list() ) {
      put("mStatement", stmt1);
    }
    break;

  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 式の内容を出力する
void
AstDumper::put(
  const char* label,
  const AstExpr* expr
)
{
  if ( expr == nullptr ) {
    return;
  }

  switch ( expr->type() ) {
  case AstExpr::Opr:
    if ( expr->op_type() == VpiOpType::Null ) {
      // '(' expression ')' なので無視
      return put(label, expr->operand0());
    }
    {
      AstHeader x(*this, label, "Opr");

      put("mFileRegion", expr->file_region());
#if 0
      put("mAttrInst", expr->attr_top());
#endif

      put("mOprType", expr->op_type());
      for ( auto expr1: expr->operand_list() ) {
	put("mOperand",  expr1);
      }
    }
    break;

  case AstExpr::Const:
    {
      AstHeader x(*this, label, "Constant");

      put("mFileRegion", expr->file_region());
      put("mConstType", expr->const_type());
      put("mConstBitVector", expr->const_bitvect());
      put("mConstStr", expr->const_str());
      put("mConstReal", expr->const_real());
    }
    break;

  case AstExpr::FuncCall:
  case AstExpr::SysFuncCall:
    {
      const char* nm = nullptr;
      if ( expr->type() == AstExpr::FuncCall ) {
	nm = "FuncCall";
      }
      else {
	nm = "SysFuncCall";
      }
      AstHeader x(*this, label, nm);

      put("mFileRegion", expr->file_region());
#if 0
      put("mAttrInst", expr->attr_top());
#endif
      put(expr->namebranch_list());
      put("mName", expr->name());
      for ( auto opr: expr->operand_list() ) {
	if ( opr ) {
	  put("mOperand", opr);
	}
	else {
	  put("mOperand", "null");
	}
      }
    }
    break;

  case AstExpr::Primary:
    {
      AstHeader x(*this, label, "Primary");

      put("mFileRegion", expr->file_region());
      put(expr->namebranch_list());
      put("mName", expr->name());
      for ( auto index: expr->index_list() ) {
	put("mIndex", index);
      }
      auto part = expr->part();
      if ( part != nullptr ) {
	put("mRangeMode", part->mode());
	put("mLeftRange", part->left());
	put("mRightRange", part->right());
      }
    }
    break;

  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief インクルード元のファイル情報の出力
void
AstDumper::put_parent_file(
  const FileLoc& file_loc
)
{
  auto file_list{file_loc.parent_loc_list()};
  for ( auto loc: file_list ) {
    AstHeader x(*this, "mParentFile", "IncFile", false);
    mStream << "name = " << loc.filename()
	    << ", line = " << loc.line();
  }
}

/// @brief 宣言を出力する．
void
AstDumper::put_decls(
  const AstIOHeadVec& iohead_list,
  const AstDeclHeadVec& declhead_list
)
{
  for ( auto io: iohead_list ) {
    put("mIODecl", io);
  }
  for ( auto decl: declhead_list ) {
    put("mDecl", decl);
  }
}

// @brief 宣言と要素を出力する．
void
AstDumper::put_decl_item(
  const char* label,
  const AstDeclHeadVec& decl_list,
  const AstItemVec& item_list
)
{
  AstHeader x(*this, label, "GenBlock");

  for ( auto decl: decl_list ) {
    put("mDecl", decl);
  }
  for ( auto item: item_list ) {
    put("mItem", item);
  }
}

// @brief 階層名の出力
void
AstDumper::put(
  const AstNameBranchVec& nb_list
)
{
  for ( auto nb: nb_list ) {
    put("mNameBranch", nb);
  }
}

// @brief ブール型データの出力
void
AstDumper::put(
  const char* label,
  bool b
)
{
  AstHeader x(*this, label, "bool", false);
  if ( b ) {
    mStream << "true";
  }
  else {
    mStream << "false";
  }
}

// @brief 整数型データの出力
void
AstDumper::put(
  const char* label,
  int d
)
{
  AstHeader x(*this, label, "int", false);
  mStream << d;
}

// @brief 符号なし整数型データの出力
void
AstDumper::put(
  const char* label,
  std::uint32_t d
)
{
  AstHeader x(*this, label, "uint32", false);
  mStream << d;
}

// @brief ビットベクタ型データの出力
void
AstDumper::put(
  const char* label,
  const BitVector& bv
)
{
  AstHeader x(*this, label, "bitvector", false);
  mStream << bv;
}

// @brief 実数型データの出力
void
AstDumper::put(
  const char* label,
  double r
)
{
  AstHeader x(*this, label, "real", false);
  mStream << r;
}

// @brief 文字列型データの出力
void
AstDumper::put(
  const char* label,
  const char* str
)
{
  AstHeader x(*this, label, "string", false);
  if ( str ) {
    mStream << str;
  }
}

// @brief 文字列型データの出力
void
AstDumper::put(
  const char* label,
  const std::string& str
)
{
  put(label, str.c_str());
}

// @brief 補助的なデータ型の出力
void
AstDumper::put(
  const char* label,
  VpiAuxType aux_type
)
{
  AstHeader x(*this, label, "aux_type", false);
  switch ( aux_type ) {
  case VpiAuxType::None: mStream << "none"; break;
  case VpiAuxType::Net:  mStream << "net"; break;
  case VpiAuxType::Reg:  mStream << "reg"; break;
  case VpiAuxType::Var:  mStream << "var"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief ネット型の出力
void
AstDumper::put(
  const char* label,
  VpiNetType net_type
)
{
  AstHeader x(*this, label, "net_type", false);
  switch ( net_type ) {
  case VpiNetType::Supply0: mStream << "supply0"; break;
  case VpiNetType::Supply1: mStream << "supply1"; break;
  case VpiNetType::Tri:     mStream << "tri"; break;
  case VpiNetType::TriAnd:  mStream << "triand"; break;
  case VpiNetType::TriOr:   mStream << "trior"; break;
  case VpiNetType::TriReg:  mStream << "trireg"; break;
  case VpiNetType::Tri0:    mStream << "tri0"; break;
  case VpiNetType::Tri1:    mStream << "tri1"; break;
  case VpiNetType::Wire:    mStream << "wire"; break;
  case VpiNetType::Wand:    mStream << "wand"; break;
  case VpiNetType::Wor:     mStream << "wor"; break;
  case VpiNetType::None:    mStream << "none"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 変数型の出力
void
AstDumper::put(
  const char* label,
  VpiVarType var_type
)
{
  AstHeader x(*this, label, "var_type", false);
  switch ( var_type ) {
  case VpiVarType::Integer:  mStream << "integer"; break;
  case VpiVarType::Real:     mStream << "real"; break;
  case VpiVarType::Time:     mStream << "time"; break;
  case VpiVarType::Realtime: mStream << "realtime"; break;
  case VpiVarType::None:     mStream << "none"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 方向の出力
void
AstDumper::put(
  const char* label,
  VpiDir direction
)
{
  AstHeader x(*this, label, "direction", false);
  mStream << direction;
}

// @brief unconnected drive の出力
void
AstDumper::put(
  const char* label,
  VpiUnconnDrive ud
)
{
  AstHeader x(*this, label, "unconn_drive", false);
  switch ( ud ) {
  case VpiUnconnDrive::HighZ: mStream << "high-Z"; break;
  case VpiUnconnDrive::Pull1: mStream << "pull-1"; break;
  case VpiUnconnDrive::Pull0: mStream << "pull-0"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief default delay mode の出力
void
AstDumper::put(
  const char* label,
  VpiDefDelayMode delay_mode
)
{
  AstHeader x(*this, label, "def_delay_mode", false);
  switch ( delay_mode ) {
  case VpiDefDelayMode::None:    mStream << "none"; break;
  case VpiDefDelayMode::Path:    mStream << "path"; break;
  case VpiDefDelayMode::Distrib: mStream << "distrib"; break;
  case VpiDefDelayMode::Unit:    mStream << "unit"; break;
  case VpiDefDelayMode::Zero:    mStream << "zero"; break;
  case VpiDefDelayMode::MTM:     mStream << "min-typ-max"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief primitive型の出力
void
AstDumper::put(
  const char* label,
  VpiPrimType prim_type
)
{
  AstHeader x(*this, label, "prim_type", false);
  switch ( prim_type ) {
  case VpiPrimType::And:      mStream << "and"; break;
  case VpiPrimType::Nand:     mStream << "nand"; break;
  case VpiPrimType::Nor:      mStream << "nor"; break;
  case VpiPrimType::Or:       mStream << "or"; break;
  case VpiPrimType::Xor:      mStream << "xor"; break;
  case VpiPrimType::Xnor:     mStream << "xnor"; break;
  case VpiPrimType::Buf:      mStream << "buf"; break;
  case VpiPrimType::Not:      mStream << "not"; break;
  case VpiPrimType::Bufif0:   mStream << "bufif0"; break;
  case VpiPrimType::Bufif1:   mStream << "bufif1"; break;
  case VpiPrimType::Notif0:   mStream << "notif0"; break;
  case VpiPrimType::Notif1:   mStream << "notif1"; break;
  case VpiPrimType::Nmos:     mStream << "nmos"; break;
  case VpiPrimType::Pmos:     mStream << "pmos"; break;
  case VpiPrimType::Cmos:     mStream << "cmos"; break;
  case VpiPrimType::Rnmos:    mStream << "rnmos"; break;
  case VpiPrimType::Rpmos:    mStream << "rpmos"; break;
  case VpiPrimType::Rcmos:    mStream << "rcmos"; break;
  case VpiPrimType::Rtran:    mStream << "rtran"; break;
  case VpiPrimType::Rtranif0: mStream << "rtranif0"; break;
  case VpiPrimType::Rtranif1: mStream << "rtranif1"; break;
  case VpiPrimType::Tran:     mStream << "tran"; break;
  case VpiPrimType::Tranif0:  mStream << "tranif0"; break;
  case VpiPrimType::Tranif1:  mStream << "tranif1"; break;
  case VpiPrimType::Pullup:   mStream << "pullup"; break;
  case VpiPrimType::Pulldown: mStream << "pulldown"; break;
  case VpiPrimType::Seq:      mStream << "seq"; break;
  case VpiPrimType::Comb:     mStream << "comb"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 演算子型の出力
void
AstDumper::put(
  const char* label,
  VpiOpType op_type
)
{
  AstHeader x(*this, label, "op_type", false);
  switch ( op_type ) {
  case VpiOpType::Minus:       mStream << "minus"; break;
  case VpiOpType::Plus:        mStream << "plus"; break;
  case VpiOpType::Not:         mStream << "not"; break;
  case VpiOpType::BitNeg:      mStream << "bitneg"; break;
  case VpiOpType::UnaryAnd:    mStream << "unary and"; break;
  case VpiOpType::UnaryNand:   mStream << "unary nand"; break;
  case VpiOpType::UnaryOr:     mStream << "unary or"; break;
  case VpiOpType::UnaryNor:    mStream << "unary nor"; break;
  case VpiOpType::UnaryXor:    mStream << "unary xor"; break;
  case VpiOpType::UnaryXNor:   mStream << "unary xnor"; break;
  case VpiOpType::Sub:         mStream << "sub"; break;
  case VpiOpType::Div:         mStream << "div"; break;
  case VpiOpType::Mod:         mStream << "mod"; break;
  case VpiOpType::Eq:          mStream << "eq"; break;
  case VpiOpType::Neq:         mStream << "neq"; break;
  case VpiOpType::CaseEq:      mStream << "caseeq"; break;
  case VpiOpType::CaseNeq:     mStream << "caseneq"; break;
  case VpiOpType::Gt:          mStream << "gt"; break;
  case VpiOpType::Ge:          mStream << "ge"; break;
  case VpiOpType::Lt:          mStream << "lt"; break;
  case VpiOpType::Le:          mStream << "le"; break;
  case VpiOpType::LShift:      mStream << "left shift"; break;
  case VpiOpType::RShift:      mStream << "right shift"; break;
  case VpiOpType::Add:         mStream << "add"; break;
  case VpiOpType::Mult:        mStream << "mult"; break;
  case VpiOpType::LogAnd:      mStream << "logical and"; break;
  case VpiOpType::LogOr:       mStream << "logical or"; break;
  case VpiOpType::BitAnd:      mStream << "bit and"; break;
  case VpiOpType::BitOr:       mStream << "bit or"; break;
  case VpiOpType::BitXor:      mStream << "bit xor"; break;
  case VpiOpType::BitXNor:     mStream << "bit xnor"; break;
  case VpiOpType::Condition:   mStream << "conditional"; break;
  case VpiOpType::Concat:      mStream << "concat"; break;
  case VpiOpType::MultiConcat: mStream << "multi concat"; break;
  case VpiOpType::EventOr:     mStream << "event or"; break;
  case VpiOpType::Null:        mStream << "null"; break;
  case VpiOpType::List:        mStream << "list"; break;
  case VpiOpType::MinTypMax:   mStream << "min-typ-max"; break;
  case VpiOpType::Posedge:     mStream << "posedge"; break;
  case VpiOpType::Negedge:     mStream << "negedge"; break;
  case VpiOpType::ArithLShift: mStream << "arithmetic left shift"; break;
  case VpiOpType::ArithRShift: mStream << "arithmetic right shift"; break;
  case VpiOpType::Power:       mStream << "power"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 定数型の出力
void
AstDumper::put(
  const char* label,
  VpiConstType const_type
)
{
  AstHeader x(*this, label, "const_type", false);
  switch ( const_type ) {
  case VpiConstType::Dec:          mStream << "dec"; break;
  case VpiConstType::Real:         mStream << "real"; break;
  case VpiConstType::Binary:       mStream << "binary"; break;
  case VpiConstType::Oct:          mStream << "oct"; break;
  case VpiConstType::Hex:          mStream << "hex"; break;
  case VpiConstType::String:       mStream << "string"; break;
  case VpiConstType::Int:          mStream << "int"; break;
  case VpiConstType::SignedDec:    mStream << "signed dec"; break;
  case VpiConstType::SignedBinary: mStream << "signed binary"; break;
  case VpiConstType::SignedOct:    mStream << "signed oct"; break;
  case VpiConstType::SignedHex:    mStream << "signed hex"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief strength の出力
void
AstDumper::put(
  const char* label,
  VpiStrength strength
)
{
  AstHeader x(*this, label, "strength", false);
  switch ( strength ) {
  case VpiStrength::NoStrength:   mStream << "none"; break;
  case VpiStrength::SupplyDrive:  mStream << "supply"; break;
  case VpiStrength::StrongDrive:  mStream << "strong"; break;
  case VpiStrength::PullDrive:    mStream << "pull"; break;
  case VpiStrength::WeakDrive:    mStream << "weak"; break;
  case VpiStrength::LargeCharge:  mStream << "large"; break;
  case VpiStrength::MediumCharge: mStream << "medium"; break;
  case VpiStrength::SmallCharge:  mStream << "small"; break;
  case VpiStrength::HiZ:          mStream << "high-Z"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief vectored/scalared 属性の出力
void
AstDumper::put(
  const char* label,
  VpiVsType vs_type
)
{
  AstHeader x(*this, label, "vs_type", false);
  switch ( vs_type ) {
  case VpiVsType::None:     mStream << "none"; break;
  case VpiVsType::Vectored: mStream << "vectored"; break;
  case VpiVsType::Scalared: mStream << "scalared"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief 範囲指定モードの出力
void
AstDumper::put(
  const char* label,
  VpiRangeMode range_mode
)
{
  AstHeader x(*this, label, "range_mode", false);
  switch ( range_mode ) {
  case VpiRangeMode::No:    mStream << "no range"; break;
  case VpiRangeMode::Const: mStream << "constant range"; break;
  case VpiRangeMode::Plus:  mStream << "plus range"; break;
  case VpiRangeMode::Minus: mStream << "minus range"; break;
  default: ASSERT_NOT_REACHED; break;
  }
}

// @brief FileLoc の情報を出力する．
void
AstDumper::put(
  const char* label,
  const FileLoc& file_loc
)
{
  if ( !file_loc_mode() ) {
    return;
  }

  AstHeader x(*this, label, "FileLoc");
  if ( file_loc.is_valid() ) {
    put_parent_file(file_loc);

    AstHeader x(*this, "mPosition", "FilePos", false);
    mStream << "name = " << file_loc.filename()
	    << ", line = " << file_loc.line()
	    << ", column = " << file_loc.column();
  }
  else {
    AstHeader x(*this, "mPosition", "FilePos", false);
    mStream << "invalid file_loc";
  }
}

// @brief FileRegion の情報を出力する．
void
AstDumper::put(
  const char* label,
  const FileRegion& file_region
)
{
  if ( !file_loc_mode() ) {
    return;
  }

  AstHeader x(*this, label, "FileRegion");

  const FileLoc& first = file_region.start_loc();
  const FileLoc& last = file_region.end_loc();

  if ( first.file_info() == last.file_info() ) {
    if ( first.is_valid() ) {
      // 同じファイル
      put_parent_file(first);

      AstHeader x(*this, "mRegion", "FilePos", false);
      mStream << "name = " << first.filename();
      if ( first.line() == last.line() ) {
	// 同じ行番号
	mStream << ", line = " << first.line();
	if ( first.column() == last.column() ) {
	  // 同じコラム位置
	  mStream << ", column = " << first.column();
	}
	else {
	  // 異なるコラム
	  mStream << ", column = " << first.column()
		  << " - " << last.column();
	}
      }
      else {
	// 異なる行
	mStream << ", line = " << first.line()
		<< ", column = " << first.column()
		<< " - line = " << last.line()
		<< ", column = " << last.column();
      }
    }
    else {
      AstHeader x(*this, "mRegion", "FilePos", false);
      mStream << "---";
    }
  }
  else {
    // 異なるファイル
    put("mFirstLoc", first);
    put("mLastLoc", last);
  }
}

// @brief 属性インスタンスを出力する．
void
AstDumper::put(
  const char* label,
  const AstAttrInst* attr
)
{
  if ( attr == nullptr ) {
    return;
  }

  AstHeader x(*this, label, "AttrInstList");
#if 0
  for ( ; attr; attr = attr->next()) {
    AstHeader x(*this, "mAttrInst", "AttrInst");
    for (const AstAttrSpec* as = attr->attr_spec_top();
	 as; as = as->next()) {
      AstHeader x(*this, "mAttrSpec", "AttrSpec");

      put("mFileRegion", as->file_region());
      put("mName", as->name());
      put("mExpr", as->expr());
    }
  }
#endif
}

// @brief コントロールの内容を表示する．
void
AstDumper::put(
  const char* label,
  const AstControl* ctrl
)
{
  if ( ctrl == nullptr ) {
    return;
  }

  const char* nm = nullptr;
  switch ( ctrl->type() ) {
  case AstControl::Delay:  nm = "DelayControl"; break;
  case AstControl::Event:  nm = "EventControl"; break;
  case AstControl::Repeat: nm = "RepeatControl"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  AstHeader x(*this, label, nm);

  put("mFileRegion", ctrl->file_region());
  put("mDelay", ctrl->delay());
  put("mRepExpr", ctrl->rep_expr());
  for ( auto expr: ctrl->event_list() ) {
    put("mEvent", expr);
  }
}

// @brief strength の内容を表示する．
void
AstDumper::put(
  const char* label,
  const AstStrength* str
)
{
  if ( str == nullptr ) {
    return;
  }

  AstHeader x(*this, label, "Strengh");

  put("mFileRegion", str->file_region());
  if ( str->drive0() != VpiStrength::NoStrength ) {
    put("mDrive0", str->drive0());
  }
  if ( str->drive1() != VpiStrength::NoStrength ) {
    put("mDrive1", str->drive1());
  }
  if ( str->charge() != VpiStrength::NoStrength ) {
    put("mCharge", str->charge());
  }
}

// @brief 遅延情報を出力する．
void
AstDumper::put(
  const char* label,
  const AstDelay* delay
)
{
  if ( delay == nullptr ) {
    return;
  }

  AstHeader x(*this, label, "Delay");

  put("mFileRegion", delay->file_region());
  put("mDelay0", delay->value0());
  put("mDelay1", delay->value1());
  put("mDelay2", delay->value2());
}

// @brief 接続情報を表示する．
void
AstDumper::put(
  const char* label,
  const AstConnection* con
)
{
  if ( con == nullptr ) {
    return;
  }

  const char* nm = nullptr;
  if ( con->name() != nullptr ) {
    nm = "NamedCon";
  }
  else {
    nm = "OrderedCon";
  }
  AstHeader x(*this, label, nm);

  put("mFileRegion", con->file_region());
#if 0
  put("mAttrInst", con->attr_top());
#endif

  if ( con->name() != nullptr ) {
    put("mName", con->name());
  }
  if ( con->expr() ) {
    put("mExpr", con->expr());
  }
  else {
    put("mExpr", "null");
  }
}

// @brief 階層ブランチの情報を出力する
void
AstDumper::put(
  const char* label,
  const AstNameBranch* nb
)
{
  AstHeader x(*this, label, "NameBranch");

  put("mName", nb->name());
  if ( nb->has_index() ) {
    put("mIndex", nb->index());
  }
}

// @brief 開始マーカを出力する．
void
AstDumper::put_begin(
  const char* label,
  const char* type,
  bool need_cr
)
{
  for (int i = 0; i < mIndent; ++ i) {
    mStream << "  ";
  }
  mStream << "<" << label << " type = \"" << type << "\">";
  if ( need_cr ) {
    mStream << std::endl;
  }
  mDoCR.push_back(need_cr);
  ++ mIndent;
}

// @brief 終了マーカを出力する．
void
AstDumper::put_end(
  const char* label
)
{
  -- mIndent;
  if ( mDoCR.back() ) {
    for (int i = 0; i < mIndent; ++ i) {
      mStream << "  ";
    }
  }
  mDoCR.pop_back();
  mStream << "</" << label << ">" << std::endl;
}


//////////////////////////////////////////////////////////////////////
// マーカを出力するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstHeader::AstHeader(
  AstDumper& s,
  const char* name,
  const char* type,
  bool need_cr
) : mS(s),
    mName(name)
{
  mS.put_begin(mName.c_str(), type, need_cr);
}

// @brief デストラクタ
AstHeader::~AstHeader()
{
  mS.put_end(mName.c_str());
}

END_NAMESPACE_YM_VERILOG
