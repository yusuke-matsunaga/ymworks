#ifndef YM_VERILOG_H
#define YM_VERILOG_H

/// @file ym/verilog.h
/// @brief ym-verilog に共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

/// @namespace nsYm::nsVerilog
/// @brief Verilog パーサ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

/// @defgroup verilog Verilog パッケージ
///
/// Verilog-HDL(IEEE1364, IEEE1364-2001) フォーマットの
/// フロントエンド


#include "ym_config.h"
#include "vpi_user.h"

/// @brief ym_verilog 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_VERILOG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsVerilog)

/// @brief ym_verilog 用の名前空間の終了
#define END_NAMESPACE_YM_VERILOG \
END_NAMESPACE(nsVerilog) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_VERILOG

class BitVector;
class VlLineWatcher;
class VlMgr;
class VlScalarVal;
class VlTime;
class VlUdpVal;
class VlValue;
class VlValueType;

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_YM

/// @defgroup VlCommon Verilog 用基本ユーティリティ
/// @ingroup ym-verilog
/// @{

//////////////////////////////////////////////////////////////////////
// 列挙型の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// @brief オブジェクトの型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiObjType : std::uint8_t {
  Always          = vpiAlways,
  AssignStmt      = vpiAssignStmt,
  Assignment      = vpiAssignment,
  Begin           = vpiBegin,
  Case            = vpiCase,
  CaseItem        = vpiCaseItem,
  Constant        = vpiConstant,
  ContAssign      = vpiContAssign,
  Deassign        = vpiDeassign,
  DefParam        = vpiDefParam,
  DelayControl    = vpiDelayControl,
  Disable         = vpiDisable,
  EventControl    = vpiEventControl,
  EventStmt       = vpiEventStmt,
  For             = vpiFor,
  Force           = vpiForce,
  Forever         = vpiForever,
  Fork            = vpiFork,
  FuncCall        = vpiFuncCall,
  Function        = vpiFunction,
  Gate            = vpiGate,
  If              = vpiIf,
  IfElse          = vpiIfElse,
  Initial         = vpiInitial,
  IntegerVar      = vpiIntegerVar,
  InterModPath    = vpiInterModPath,
  Iterator        = vpiIterator,
  IODecl          = vpiIODecl,
  Memory          = vpiMemory,
  MemoryWord      = vpiMemoryWord,
  ModPath         = vpiModPath,
  Module          = vpiModule,
  NamedBegin      = vpiNamedBegin,
  NamedEvent      = vpiNamedEvent,
  NamedFork       = vpiNamedFork,
  Net             = vpiNet,
  NetBit          = vpiNetBit,
  NullStmt        = vpiNullStmt,
  Operation       = vpiOperation,
  ParamAssign     = vpiParamAssign,
  Parameter       = vpiParameter,
  PartSelect      = vpiPartSelect,
  PathTerm        = vpiPathTerm,
  Port            = vpiPort,
  PortBit         = vpiPortBit,
  PrimTerm        = vpiPrimTerm,
  RealVar         = vpiRealVar,
  Reg             = vpiReg,
  RegBit          = vpiRegBit,
  Release         = vpiRelease,
  Repeat          = vpiRepeat,
  RepeatControl   = vpiRepeatControl,
  SchedEvent      = vpiSchedEvent,
  SpecParam       = vpiSpecParam,
  Switch          = vpiSwitch,
  SysFuncCall     = vpiSysFuncCall,
  SysTaskCall     = vpiSysTaskCall,
  TableEntry      = vpiTableEntry,
  Task            = vpiTask,
  TaskCall        = vpiTaskCall,
  Tchk            = vpiTchk,
  TchkTerm        = vpiTchkTerm,
  TimeVar         = vpiTimeVar,
  TimeQueue       = vpiTimeQueue,
  Udp             = vpiUdp,
  UdpDefn         = vpiUdpDefn,
  UserSystf       = vpiUserSystf,
  VarSelect       = vpiVarSelect,
  Wait            = vpiWait,
  While           = vpiWhile,
  Attribute       = vpiAttribute,
  BitSelect       = vpiBitSelect,
  Callback        = vpiCallback,
  DelayTerm       = vpiDelayTerm,
  DelayDevice     = vpiDelayDevice,
  Frame           = vpiFrame,
  GateArray       = vpiGateArray,
  ModuleArray     = vpiModuleArray,
  PrimitiveArray  = vpiPrimitiveArray,
  NetArray        = vpiNetArray,
  Range           = vpiRange,
  RegArray        = vpiRegArray,
  SwitchArray     = vpiSwitchArray,
  UdpArray        = vpiUdpArray,
  ContAssignBit   = vpiContAssignBit,
  NamedEventArray = vpiNamedEventArray,
  Scope           = vpiScope
};

/// @brief VpiObjType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiObjType type  ///< [in] 型
)
{
  switch( type ) {
  case VpiObjType::Always:          s << "Always"; break;
  case VpiObjType::AssignStmt:      s << "AssignStmt"; break;
  case VpiObjType::Assignment:      s << "Assignment"; break;
  case VpiObjType::Begin:           s << "Begin"; break;
  case VpiObjType::Case:            s << "Case"; break;
  case VpiObjType::CaseItem:        s << "CaseItem"; break;
  case VpiObjType::Constant:        s << "Constant"; break;
  case VpiObjType::ContAssign:      s << "ContAssign"; break;
  case VpiObjType::Deassign:        s << "Deassign"; break;
  case VpiObjType::DefParam:        s << "DefParam"; break;
  case VpiObjType::DelayControl:    s << "DelayControl"; break;
  case VpiObjType::Disable:         s << "Disable"; break;
  case VpiObjType::EventControl:    s << "EventControl"; break;
  case VpiObjType::EventStmt:       s << "EventStmt"; break;
  case VpiObjType::For:             s << "For"; break;
  case VpiObjType::Force:           s << "Force"; break;
  case VpiObjType::Forever:         s << "Forever"; break;
  case VpiObjType::Fork:            s << "Fork"; break;
  case VpiObjType::FuncCall:        s << "FuncCall"; break;
  case VpiObjType::Function:        s << "Function"; break;
  case VpiObjType::Gate:            s << "Gate"; break;
  case VpiObjType::If:              s << "If"; break;
  case VpiObjType::IfElse:          s << "IfElse"; break;
  case VpiObjType::Initial:         s << "Initial"; break;
  case VpiObjType::IntegerVar:      s << "IntegerVar"; break;
  case VpiObjType::InterModPath:    s << "InterModPath"; break;
  case VpiObjType::Iterator:        s << "Iterator"; break;
  case VpiObjType::IODecl:          s << "IODecl"; break;
  case VpiObjType::Memory:          s << "Memory"; break;
  case VpiObjType::MemoryWord:      s << "MemoryWord"; break;
  case VpiObjType::ModPath:         s << "ModPath"; break;
  case VpiObjType::Module:          s << "Module"; break;
  case VpiObjType::NamedBegin:      s << "NamedBegin"; break;
  case VpiObjType::NamedEvent:      s << "NamedEvent"; break;
  case VpiObjType::NamedFork:       s << "NamedFork"; break;
  case VpiObjType::Net:             s << "Net"; break;
  case VpiObjType::NetBit:          s << "NetBit"; break;
  case VpiObjType::NullStmt:        s << "NullStmt"; break;
  case VpiObjType::Operation:       s << "Operation"; break;
  case VpiObjType::ParamAssign:     s << "ParamAssign"; break;
  case VpiObjType::Parameter:       s << "Parameter"; break;
  case VpiObjType::PartSelect:      s << "PartSelect"; break;
  case VpiObjType::PathTerm:        s << "PathTerm"; break;
  case VpiObjType::Port:            s << "Port"; break;
  case VpiObjType::PortBit:         s << "PortBit"; break;
  case VpiObjType::PrimTerm:        s << "PrimTerm"; break;
  case VpiObjType::RealVar:         s << "RealVar"; break;
  case VpiObjType::Reg:             s << "Reg"; break;
  case VpiObjType::RegBit:          s << "RegBit"; break;
  case VpiObjType::Release:         s << "Release"; break;
  case VpiObjType::Repeat:          s << "Repeat"; break;
  case VpiObjType::RepeatControl:   s << "RepeatControl"; break;
  case VpiObjType::SchedEvent:      s << "SchedEvent"; break;
  case VpiObjType::SpecParam:       s << "SpecParam"; break;
  case VpiObjType::Switch:          s << "Switch"; break;
  case VpiObjType::SysFuncCall:     s << "SysFuncCall"; break;
  case VpiObjType::SysTaskCall:     s << "SysTaskCall"; break;
  case VpiObjType::TableEntry:      s << "TableEntry"; break;
  case VpiObjType::Task:            s << "Task"; break;
  case VpiObjType::TaskCall:        s << "TaskCall"; break;
  case VpiObjType::Tchk:            s << "Tchk"; break;
  case VpiObjType::TchkTerm:        s << "TchkTerm"; break;
  case VpiObjType::TimeVar:         s << "TimeVar"; break;
  case VpiObjType::TimeQueue:       s << "TimeQueue"; break;
  case VpiObjType::Udp:             s << "Udp"; break;
  case VpiObjType::UdpDefn:         s << "UdpDefn"; break;
  case VpiObjType::UserSystf:       s << "UserSystf"; break;
  case VpiObjType::VarSelect:       s << "VarSelect"; break;
  case VpiObjType::Wait:            s << "Wait"; break;
  case VpiObjType::While:           s << "While"; break;
  case VpiObjType::Attribute:       s << "Attribute"; break;
  case VpiObjType::BitSelect:       s << "BitSelect"; break;
  case VpiObjType::Callback:        s << "Callback"; break;
  case VpiObjType::DelayTerm:       s << "DelayTerm"; break;
  case VpiObjType::DelayDevice:     s << "DelayDevice"; break;
  case VpiObjType::Frame:           s << "Frame"; break;
  case VpiObjType::GateArray:       s << "GateArray"; break;
  case VpiObjType::ModuleArray:     s << "ModuleArray"; break;
  case VpiObjType::PrimitiveArray:  s << "PrimitiveArray"; break;
  case VpiObjType::NetArray:        s << "NetArray"; break;
  case VpiObjType::Range:           s << "Range"; break;
  case VpiObjType::RegArray:        s << "RegArray"; break;
  case VpiObjType::SwitchArray:     s << "SwitchArray"; break;
  case VpiObjType::UdpArray:        s << "UdpArray"; break;
  case VpiObjType::ContAssignBit:   s << "ContAssignBit"; break;
  case VpiObjType::NamedEventArray: s << "NamedEventArray"; break;
  case VpiObjType::Scope:           s << "Scope"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief unconnected port drive の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiUnconnDrive : std::uint8_t {
  HighZ = vpiHighZ,
  Pull1 = vpiPull1,
  Pull0 = vpiPull0
};

/// @brief VpiUnconnDriver のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,   ///< [in] 出力ストリーム
  VpiUnconnDrive val ///< [in] 値
)
{
  switch ( val ) {
  case VpiUnconnDrive::HighZ: s << "HighZ"; break;
  case VpiUnconnDrive::Pull1: s << "Pull1"; break;
  case VpiUnconnDrive::Pull0: s << "Pull0"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief default delay Mode の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiDefDelayMode : std::uint8_t {
  None    = vpiDelayModeNone,
  Path    = vpiDelayModePath,
  Distrib = vpiDelayModeDistrib,
  Unit    = vpiDelayModeUnit,
  Zero    = vpiDelayModeZero,
  MTM     = vpiDelayModeMTM
};

/// @brief VpiDefDelayModel のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,    ///< [in] ストリーム出力
  VpiDefDelayMode val ///< [in] 値
)
{
  switch ( val ) {
  case VpiDefDelayMode::None:    s << "None"; break;
  case VpiDefDelayMode::Path:    s << "Path"; break;
  case VpiDefDelayMode::Distrib: s << "Distrib"; break;
  case VpiDefDelayMode::Unit:    s << "Unit"; break;
  case VpiDefDelayMode::Zero:    s << "Zero"; break;
  case VpiDefDelayMode::MTM:     s << "MTM"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief direction の値
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiDir : std::uint8_t {
  Input       = vpiInput,
  Output      = vpiOutput,
  Inout       = vpiInout,
  MixedIO     = vpiMixedIO,
  NoDirection = vpiNoDirection
};

/// @brief VpiDir のストリーム出力
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiDir dir)      ///< [in] 方向
{
  switch ( dir ) {
  case VpiDir::Input:       s << "Input"; break;
  case VpiDir::Output:      s << "Output"; break;
  case VpiDir::Inout:       s << "Inout"; break;
  case VpiDir::MixedIO:     s << "Mixed IO"; break;
  case VpiDir::NoDirection: s << "No Direction"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief net の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiNetType : std::uint8_t {
  Wire     = vpiWire,
  Wand     = vpiWand,
  Wor      = vpiWor,
  Tri      = vpiTri,
  Tri0     = vpiTri0,
  Tri1     = vpiTri1,
  TriReg   = vpiTriReg,
  TriAnd   = vpiTriAnd,
  TriOr    = vpiTriOr,
  Supply1  = vpiSupply1,
  Supply0  = vpiSupply0,
  None     = vpiNone
};

/// @brief VpiNetType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiNetType val   ///< [in] 値
)
{
  switch ( val ) {
  case VpiNetType::Wire:    s << "Wire"; break;
  case VpiNetType::Wand:    s << "Wand"; break;
  case VpiNetType::Wor:     s << "Wor"; break;
  case VpiNetType::Tri:     s << "Tri"; break;
  case VpiNetType::Tri0:    s << "Tri0"; break;
  case VpiNetType::Tri1:    s << "Tri1"; break;
  case VpiNetType::TriReg:  s << "TriReg"; break;
  case VpiNetType::TriAnd:  s << "TriAnd"; break;
  case VpiNetType::TriOr:   s << "TriOr"; break;
  case VpiNetType::Supply1: s << "Supply1"; break;
  case VpiNetType::Supply0: s << "Supply0"; break;
  case VpiNetType::None:    s << "None"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief primitive の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiPrimType : std::uint8_t {
  And       = vpiAndPrim,
  Nand      = vpiNandPrim,
  Nor       = vpiNorPrim,
  Or        = vpiOrPrim,
  Xor       = vpiXorPrim,
  Xnor      = vpiXnorPrim,
  Buf       = vpiBufPrim,
  Not       = vpiNotPrim,
  Bufif0    = vpiBufif0Prim,
  Bufif1    = vpiBufif1Prim,
  Notif0    = vpiNotif0Prim,
  Notif1    = vpiNotif1Prim,
  Nmos      = vpiNmosPrim,
  Pmos      = vpiPmosPrim,
  Cmos      = vpiCmosPrim,
  Rnmos     = vpiRnmosPrim,
  Rpmos     = vpiRpmosPrim,
  Rcmos     = vpiRcmosPrim,
  Rtran     = vpiRtranPrim,
  Rtranif0  = vpiRtranif0Prim,
  Rtranif1  = vpiRtranif1Prim,
  Tran      = vpiTranPrim,
  Tranif0   = vpiTranif0Prim,
  Tranif1   = vpiTranif1Prim,
  Pullup    = vpiPullupPrim,
  Pulldown  = vpiPulldownPrim,
  Seq       = vpiSeqPrim,
  Comb      = vpiCombPrim,
  // この値だけ vpi_user.h にはない
  // 値の重複に注意すること．
  Cell      = 29
};

/// @brief VpiPrimType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiPrimType val  ///< [in] 値
)
{
  switch ( val ) {
  case VpiPrimType::And:      s << "And"; break;
  case VpiPrimType::Nand:     s << "Nand"; break;
  case VpiPrimType::Nor:      s << "Nor"; break;
  case VpiPrimType::Or:       s << "Or"; break;
  case VpiPrimType::Xor:      s << "Xor"; break;
  case VpiPrimType::Xnor:     s << "Xnor"; break;
  case VpiPrimType::Buf:      s << "Buf"; break;
  case VpiPrimType::Not:      s << "Not"; break;
  case VpiPrimType::Bufif0:   s << "Bufif0"; break;
  case VpiPrimType::Bufif1:   s << "bufif1"; break;
  case VpiPrimType::Notif0:   s << "Notif0"; break;
  case VpiPrimType::Notif1:   s << "Notif1"; break;
  case VpiPrimType::Nmos:     s << "Nmos"; break;
  case VpiPrimType::Pmos:     s << "Pmos"; break;
  case VpiPrimType::Cmos:     s << "Cmos"; break;
  case VpiPrimType::Rnmos:    s << "Rnmos"; break;
  case VpiPrimType::Rpmos:    s << "Rpmos"; break;
  case VpiPrimType::Rcmos:    s << "Rcmos"; break;
  case VpiPrimType::Rtran:    s << "Rtran"; break;
  case VpiPrimType::Rtranif0: s << "Rtranif0"; break;
  case VpiPrimType::Rtranif1: s << "Rtranif1"; break;
  case VpiPrimType::Tran:     s << "Tran"; break;
  case VpiPrimType::Tranif0:  s << "Tranif0"; break;
  case VpiPrimType::Tranif1:  s << "Tranif1"; break;
  case VpiPrimType::Pullup:   s << "Pullup"; break;
  case VpiPrimType::Pulldown: s << "Pulldown"; break;
  case VpiPrimType::Seq:      s << "Seq"; break;
  case VpiPrimType::Comb:     s << "Comb"; break;
  case VpiPrimType::Cell:     s << "Cell"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief module path/data path の極性
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiPolarity : std::uint8_t {
  Positive = vpiPositive,
  Negative = vpiNegative,
  Unknown  = vpiUnknown
};

/// @brief VpiPolarity のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiPolarity val  ///< [in] 値
)
{
  switch( val ) {
  case VpiPolarity::Positive: s << "Positive"; break;
  case VpiPolarity::Negative: s << "Negative"; break;
  case VpiPolarity::Unknown:  s << "Unknown"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief module path の edge type
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiEdge : std::uint8_t {
  NoEdge  = vpiNoEdge,
  Edge01  = vpiEdge01,
  Edge10  = vpiEdge10,
  Edge0x  = vpiEdge0x,
  Edgex1  = vpiEdgex1,
  Edge1x  = vpiEdge1x,
  Edgex0  = vpiEdgex0,
  Posedge = vpiPosedge, // = (kVpiEdgex1 | kVpiEdge01 | kVpiEdge0x)
  Negedge = vpiNegedge, // = (kVpiEdgex0 | kVpiEdge10 | kVpiEdge1x)
  AnyEdge = vpiAnyEdge  // = (kVpiPosedge | kVpiNegedge)
};

/// @brief VpiEdge のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiEdge val      ///< [in] 値
)
{
  switch ( val ) {
  case VpiEdge::NoEdge:  s << "NoEdge"; break;
  case VpiEdge::Edge01:  s << "Edge01"; break;
  case VpiEdge::Edge10:  s << "Edge10"; break;
  case VpiEdge::Edge0x:  s << "Edge0x"; break;
  case VpiEdge::Edgex1:  s << "Edgex1"; break;
  case VpiEdge::Edge1x:  s << "Edge1x"; break;
  case VpiEdge::Edgex0:  s << "Edgex0"; break;
  case VpiEdge::Posedge: s << "Posedge"; break;
  case VpiEdge::Negedge: s << "Negedge"; break;
  case VpiEdge::AnyEdge: s << "AnyEdge"; break;
  }
  return s;
}

//////////////////////////////////////////////////////////////////////
/// @brief path delay connection の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiPathType : std::uint8_t {
  Full     = vpiPathFull,
  Parallel = vpiPathParallel
};

/// @brief VpiPathType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiPathType val  ///< [in] 値
)
{
  switch ( val ) {
  case VpiPathType::Full:     s << "Full"; break;
  case VpiPathType::Parallel: s << "Parallel"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief timing check の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiTchkType : std::uint8_t {
  Setup      = vpiSetup,
  Hold       = vpiHold,
  Period     = vpiPeriod,
  Width      = vpiWidth,
  Skew       = vpiSkew,
  Recovery   = vpiRecovery,
  NoChange   = vpiNoChange,
  SetupHold  = vpiSetupHold,
  Fullskew   = vpiFullskew,
  Recrem     = vpiRecrem,
  Removal    = vpiRemoval,
  Timeskew   = vpiTimeskew
};

/// @brief VpiTchkType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiTchkType val  ///< [in] 値
)
{
  switch ( val ) {
  case VpiTchkType::Setup:     s << "Setup"; break;
  case VpiTchkType::Hold:      s << "Hold"; break;
  case VpiTchkType::Period:    s << "Period"; break;
  case VpiTchkType::Width:     s << "Width"; break;
  case VpiTchkType::Skew:      s << "Skew"; break;
  case VpiTchkType::Recovery:  s << "Recoverty"; break;
  case VpiTchkType::NoChange:  s << "NoChange"; break;
  case VpiTchkType::SetupHold: s << "SetupHold"; break;
  case VpiTchkType::Fullskew:  s << "Fullskew"; break;
  case VpiTchkType::Recrem:    s << "Recrem"; break;
  case VpiTchkType::Removal:   s << "Removal"; break;
  case VpiTchkType::Timeskew:  s << "Timeskew"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief operation の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiOpType : std::uint8_t {
  Minus        = vpiMinusOp,
  Plus         = vpiPlusOp,
  Not          = vpiNotOp,
  BitNeg       = vpiBitNegOp,
  UnaryAnd     = vpiUnaryAndOp,
  UnaryNand    = vpiUnaryNandOp,
  UnaryOr      = vpiUnaryOrOp,
  UnaryNor     = vpiUnaryNorOp,
  UnaryXor     = vpiUnaryXorOp,
  UnaryXNor    = vpiUnaryXNorOp,
  Sub          = vpiSubOp,
  Div          = vpiDivOp,
  Mod          = vpiModOp,
  Eq           = vpiEqOp,
  Neq          = vpiNeqOp,
  CaseEq       = vpiCaseEqOp,
  CaseNeq      = vpiCaseNeqOp,
  Gt           = vpiGtOp,
  Ge           = vpiGeOp,
  Lt           = vpiLtOp,
  Le           = vpiLeOp,
  LShift       = vpiLShiftOp,
  RShift       = vpiRShiftOp,
  Add          = vpiAddOp,
  Mult         = vpiMultOp,
  LogAnd       = vpiLogAndOp,
  LogOr        = vpiLogOrOp,
  BitAnd       = vpiBitAndOp,
  BitOr        = vpiBitOrOp,
  BitXor       = vpiBitXorOp,
  BitXNor      = vpiBitXNorOp,
  Condition    = vpiConditionOp,
  Concat       = vpiConcatOp,
  MultiConcat  = vpiMultiConcatOp,
  EventOr      = vpiEventOrOp,
  Null         = vpiNullOp, // '(' opr ')' の形の単項演算
  List         = vpiListOp,
  MinTypMax    = vpiMinTypMaxOp,
  Posedge      = vpiPosedgeOp,
  Negedge      = vpiNegedgeOp,
  ArithLShift  = vpiArithLShiftOp,
  ArithRShift  = vpiArithRShiftOp,
  Power        = vpiPowerOp
};

/// @brief VpiOpType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,
  VpiOpType val
)
{
  switch ( val ) {
  case VpiOpType::Minus:       s << "Minus"; break;
  case VpiOpType::Plus:        s << "Plus"; break;
  case VpiOpType::Not:         s << "Not"; break;
  case VpiOpType::BitNeg:      s << "BitNeg"; break;
  case VpiOpType::UnaryAnd:    s << "UnaryAnd"; break;
  case VpiOpType::UnaryNand:   s << "UnaryNand"; break;
  case VpiOpType::UnaryOr:     s << "UnaryOr"; break;
  case VpiOpType::UnaryNor:    s << "UnaryNor"; break;
  case VpiOpType::UnaryXor:    s << "UnaryXor"; break;
  case VpiOpType::UnaryXNor:   s << "UnaryXNor"; break;
  case VpiOpType::Sub:         s << "Sub"; break;
  case VpiOpType::Div:         s << "Div"; break;
  case VpiOpType::Mod:         s << "Mod"; break;
  case VpiOpType::Eq:          s << "Eq"; break;
  case VpiOpType::Neq:         s << "Neq"; break;
  case VpiOpType::CaseEq:      s << "CaseEq"; break;
  case VpiOpType::CaseNeq:     s << "CaseNeq"; break;
  case VpiOpType::Gt:          s << "Gt"; break;
  case VpiOpType::Ge:          s << "Ge"; break;
  case VpiOpType::Lt:          s << "Lt"; break;
  case VpiOpType::Le:          s << "Le"; break;
  case VpiOpType::LShift:      s << "LShift"; break;
  case VpiOpType::RShift:      s << "RShift"; break;
  case VpiOpType::Add:         s << "Add"; break;
  case VpiOpType::Mult:        s << "Mult"; break;
  case VpiOpType::LogAnd:      s << "LogAnd"; break;
  case VpiOpType::LogOr:       s << "LogOr"; break;
  case VpiOpType::BitAnd:      s << "BitAnd"; break;
  case VpiOpType::BitOr:       s << "BitOr"; break;
  case VpiOpType::BitXor:      s << "BitXor"; break;
  case VpiOpType::BitXNor:     s << "BitXNor"; break;
  case VpiOpType::Condition:   s << "Condition"; break;
  case VpiOpType::Concat:      s << "Concat"; break;
  case VpiOpType::MultiConcat: s << "MultiConcat"; break;
  case VpiOpType::EventOr:     s << "EventOr"; break;
  case VpiOpType::Null:        s << "Null"; break;
  case VpiOpType::List:        s << "List"; break;
  case VpiOpType::MinTypMax:   s << "MinTypeMax"; break;
  case VpiOpType::Posedge:     s << "Posedge"; break;
  case VpiOpType::Negedge:     s << "Negedge"; break;
  case VpiOpType::ArithLShift: s << "ArithLShift"; break;
  case VpiOpType::ArithRShift: s << "ArithRShift"; break;
  case VpiOpType::Power:       s << "Power"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief constant の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
/// ただし kVpiSignedXXXXX はない．
//////////////////////////////////////////////////////////////////////
enum class VpiConstType : std::uint8_t {
  Dec          = vpiDecConst,
  Real         = vpiRealConst,
  Binary       = vpiBinaryConst,
  Oct          = vpiOctConst,
  Hex          = vpiHexConst,
  String       = vpiStringConst,
  Int          = vpiIntConst,
  SignedDec    = vpiDecConst    | 8,
  SignedBinary = vpiBinaryConst | 8,
  SignedOct    = vpiOctConst    | 8,
  SignedHex    = vpiHexConst    | 8
};

/// @brief VpiConstType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiConstType val ///< [in] 値
)
{
  switch ( val ) {
  case VpiConstType::Dec:          s << "DecConst"; break;
  case VpiConstType::Real:         s << "RealConst"; break;
  case VpiConstType::Binary:       s << "BinaryConst"; break;
  case VpiConstType::Oct:          s << "OctConst"; break;
  case VpiConstType::Hex:          s << "HexConst"; break;
  case VpiConstType::String:       s << "StringConst"; break;
  case VpiConstType::Int:          s << "IntConst"; break;
  case VpiConstType::SignedDec:    s << "SignedDec"; break;
  case VpiConstType::SignedBinary: s << "SignedBinary"; break;
  case VpiConstType::SignedOct:    s << "SignedOct"; break;
  case VpiConstType::SignedHex:    s << "SignedHex"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief case の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiCaseType : std::uint8_t {
  Exact = vpiCaseExact,
  X     = vpiCaseX,
  Z     = vpiCaseZ
};

/// @brief VpiCaseType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiCaseType val  ///< [in] 値
)
{
  switch ( val ) {
  case VpiCaseType::Exact: s << "Exact"; break;
  case VpiCaseType::X:     s << "X"; break;
  case VpiCaseType::Z:     s << "Z"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief function の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
/// ただし VpiRealtimeFunc はない．
//////////////////////////////////////////////////////////////////////
enum class VpiFuncType : std::uint8_t {
  Int         = vpiIntFunc,
  Real        = vpiRealFunc,
  Time        = vpiTimeFunc,
  Sized       = vpiSizedFunc,
  SizedSigned = vpiSizedSignedFunc,
  Realtime    = 6,
  SysInt      = vpiSysFuncInt,
  SysReal     = vpiSysFuncReal,
  SysTime     = vpiSysFuncTime,
  SysSized    = vpiSysFuncSized
};

/// @brief VpiFuncType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiFuncType val  ///< [in] 値
)
{
  switch ( val ) {
  case VpiFuncType::Int:         s << "IntFunc"; break;
  case VpiFuncType::Real:        s << "RealFunc"; break;
  case VpiFuncType::Time:        s << "TimeFunc"; break;
  case VpiFuncType::Sized:       s << "SizedFunc"; break;
  case VpiFuncType::SizedSigned: s << "SizedSignedFunc"; break;
  case VpiFuncType::Realtime:    s << "Realtime"; break;
  default: break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief delay の型
/// @note この列挙型の値を強制的に int にキャストしても
/// vpi_user.h の定義値として使える．
//////////////////////////////////////////////////////////////////////
enum class VpiDelayType : std::uint8_t {
  ModPath      = vpiModPathDelay,
  InterModPath = vpiInterModPathDelay,
  MIP          = vpiMIPDelay
};

/// @brief VpiDelayType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiDelayType val ///< [in] 値
)
{
  switch ( val ) {
  case VpiDelayType::ModPath:      s << "ModPath"; break;
  case VpiDelayType::InterModPath: s << "InterModPath"; break;
  case VpiDelayType::MIP:          s << "MIP"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief strength の値
//////////////////////////////////////////////////////////////////////
enum class VpiStrength : std::uint8_t {
  NoStrength   = 0,
  SupplyDrive  = 1,
  StrongDrive  = 2,
  PullDrive    = 3,
  WeakDrive    = 4,
  LargeCharge  = 5,
  MediumCharge = 6,
  SmallCharge  = 7,
  HiZ          = 8
};

/// @brief VpiStrength のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiStrength val  ///< [in] 値
)
{
  switch ( val ) {
  case VpiStrength::NoStrength:   s << "NoStrength"; break;
  case VpiStrength::SupplyDrive:  s << "SupplyDrive"; break;
  case VpiStrength::StrongDrive:  s << "StrongDrive"; break;
  case VpiStrength::PullDrive:    s << "PullDriver"; break;
  case VpiStrength::WeakDrive:    s << "WeakDriver"; break;
  case VpiStrength::LargeCharge:  s << "LargeCharge"; break;
  case VpiStrength::MediumCharge: s << "MediumCharge"; break;
  case VpiStrength::SmallCharge:  s << "SmallCharge"; break;
  case VpiStrength::HiZ:          s << "HiZ"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief 変数のデータ型
//////////////////////////////////////////////////////////////////////
enum class VpiVarType : std::uint8_t {
  None     = 0,
  Integer  = 1,
  Real     = 2,
  Time     = 3,
  Realtime = 4
};

/// @brief VpiVarType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiVarType val   ///< [in] 値
)
{
  switch ( val ) {
  case VpiVarType::None:     s << "None"; break;
  case VpiVarType::Integer:  s << "Integer"; break;
  case VpiVarType::Real:     s << "Real"; break;
  case VpiVarType::Time:     s << "Time"; break;
  case VpiVarType::Realtime: s << "Realtime"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief 範囲指定のモード
//////////////////////////////////////////////////////////////////////
enum class VpiRangeMode : std::uint8_t {
  No    = 0, ///< 範囲指定なし
  Const = 1, ///< 固定 (例: [ a : b ])
  Plus  = 2, ///< 可変(プラス) (例: [ a +: b ])
  Minus = 3  ///< 可変(マイナス) (例: [ a -: b ])
};

/// @brief VpiRangeMode のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiRangeMode val ///< [in] 値
)
{
  switch ( val ) {
  case VpiRangeMode::No:    s << "No"; break;
  case VpiRangeMode::Const: s << "Const"; break;
  case VpiRangeMode::Plus:  s << "Plus"; break;
  case VpiRangeMode::Minus: s << "Minus"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief IO宣言で用いられる補助的な型
//////////////////////////////////////////////////////////////////////
enum class VpiAuxType : std::uint8_t {
  None = 0, ///< [in] 型無し
  Net  = 1, ///< [in] net 型
  Reg  = 2, ///< [in] reg 型
  Var  = 3  ///< [in] 変数型
};

/// @brief VpiAuxType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiAuxType val   ///< [in] 値
)
{
  switch ( val ) {
  case VpiAuxType::None: s << "None"; break;
  case VpiAuxType::Net:  s << "Net"; break;
  case VpiAuxType::Reg:  s << "Reg"; break;
  case VpiAuxType::Var:  s << "Var"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief vectored/scalared 指定の値
//////////////////////////////////////////////////////////////////////
enum class VpiVsType : std::uint8_t {
  None     = 0, ///< [in] 指定なし
  Vectored = 1, ///< [in] ベクター型
  Scalared = 2  ///< [in] スカラー型
};

/// @brief VpiVsType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力ストリーム
  VpiVsType val    ///< [in] 値
)
{
  switch ( val ) {
  case VpiVsType::None:     s << "None"; break;
  case VpiVsType::Vectored: s << "Vectored"; break;
  case VpiVsType::Scalared: s << "Scalared"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief specify block item の種類
//////////////////////////////////////////////////////////////////////
enum class VpiSpecItemType : std::uint8_t {
  PulsestyleOnEvent  = 0,
  PulsestyleOnDetect = 1,
  Showcancelled      = 2,
  Noshowcancelled    = 3
};

/// @brief VpiSpecItemType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,    ///< [in] 出力ストリーム
  VpiSpecItemType val ///< [in: 値
)
{
  switch ( val ) {
  case VpiSpecItemType::PulsestyleOnEvent:  s << "PulsestyleOnEvent"; break;
  case VpiSpecItemType::PulsestyleOnDetect: s << "PulsestyleOnDetect"; break;
  case VpiSpecItemType::Showcancelled:      s << "Showcancelled"; break;
  case VpiSpecItemType::Noshowcancelled:    s << "Noshowcancelled"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @brief specify block path の種類
//////////////////////////////////////////////////////////////////////
enum class VpiSpecPathType : std::uint8_t {
  Null   = 0,
  If     = 1,
  Ifnone = 2
};

/// @brief VpiSpecPathType のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,    ///< [in] 出力ストリーム
  VpiSpecPathType val ///< [in] 値
)
{
  switch ( val ) {
  case VpiSpecPathType::Null:   s << "Null"; break;
  case VpiSpecPathType::If:     s << "If"; break;
  case VpiSpecPathType::Ifnone: s << "Ifnone"; break;
  }
  return s;
}

/// @}

using nsVerilog::BitVector;
using nsVerilog::VlLineWatcher;
using nsVerilog::VlMgr;
using nsVerilog::VlScalarVal;
using nsVerilog::VlTime;
using nsVerilog::VlUdpVal;
using nsVerilog::VlValueType;
using nsVerilog::VlValue;

END_NAMESPACE_YM

#endif // YM_VERILOG_H
