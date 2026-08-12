
/// @file EiModule.cc
/// @brief EiModule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiModule.h"
#include "ei/EiPort.h"
#include "ei/EiIODecl.h"
#include "ei/EiDeclHead.h"
#include "ei/EiExpr.h"

#include "ym/vl/AstModule.h"
#include "ym/vl/AstDecl.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief module を生成する．
ElbModule*
EiFactory::new_Module(
  const VlScope* parent,
  const AstModule* ast_module,
  const AstItem* ast_head,
  const AstInst* ast_inst
)
{
  return new EiModule2(parent, ast_module, ast_head, ast_inst);
}

// @brief module array を生成する．
ElbModuleArray*
EiFactory::new_ModuleArray(
  const VlScope* parent,
  const AstModule* ast_module,
  const AstItem* ast_head,
  const AstInst* ast_inst,
  const AstRange* ast_range,
  const RangeVal& range
)
{
  return new EiModuleArray(parent, ast_module, ast_head,
			   ast_inst, ast_range, range);
}


//////////////////////////////////////////////////////////////////////
// クラス EiModuleHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModuleHead::EiModuleHead(
  const VlScope* parent,
  const AstModule* ast_module,
  const AstItem* ast_head,
  const AstInst* ast_inst
) : mParent{parent},
    mAstModule{ast_module},
    mAstHead{ast_head},
    mAstInst{ast_inst}
{
}

// @brief デストラクタ
EiModuleHead::~EiModuleHead()
{
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiModuleHead::parent() const
{
  return mParent;
}

// @brief ファイル位置の取得
FileRegion
EiModuleHead::file_region() const
{
  if ( mAstInst ) {
    return mAstInst->file_region();
  }
  else {
    return mAstModule->file_region();
  }
}

// @brief インスタンス名を返す．
std::string
EiModuleHead::name() const
{
  if ( mAstInst ) {
    return mAstInst->name();
  }
  else {
    return mAstModule->name();
  }
}

// @brief definition location を返す．
FileRegion
EiModuleHead::def_file_region() const
{
  return mAstModule->file_region();
}

// @brief definition name を返す．
std::string
EiModuleHead::def_name() const
{
  return mAstModule->name();
}

// @brief ポート数を返す．
SizeType
EiModuleHead::port_num() const
{
  return mAstModule->port_num();
}

// @brief 入出力宣言数を返す．
SizeType
EiModuleHead::io_num() const
{
  return mAstModule->iodecl_num();
}

// @brief cell instance のチェック
bool
EiModuleHead::is_cell_instance() const
{
  return mAstModule->is_cell();
}

// @brief protect のチェック
bool
EiModuleHead::is_protected() const
{
  return mAstModule->is_protected();
}

// @brief top module の時 true を返す．
bool
EiModuleHead::is_top_module() const
{
  return mAstInst == nullptr;
}

// @brief time unit を返す．
int
EiModuleHead::time_unit() const
{
  return mAstModule->time_unit();
}

// @brief time precision を返す．
int
EiModuleHead::time_precision() const
{
  return mAstModule->time_precision();
}

// @brief default net type を返す．
VpiNetType
EiModuleHead::def_net_type() const
{
  return mAstModule->nettype();
}

// @brief unconnected drive を返す．
VpiUnconnDrive
EiModuleHead::unconn_drive() const
{
  return mAstModule->unconn_drive();
}

// @brief default delay mode を返す．
VpiDefDelayMode
EiModuleHead::def_delay_mode() const
{
  return mAstModule->delay_mode();
}

// @brief default decay time を返す．
int
EiModuleHead::def_decay_time() const
{
  return mAstModule->decay_time();
}

// @brief config 情報を返す．
std::string
EiModuleHead::config() const
{
  return mAstModule->config();
}

// @brief library 情報を返す．
std::string
EiModuleHead::library() const
{
  return mAstModule->library();
}

// @brief cell 情報を返す．
std::string
EiModuleHead::cell() const
{
  return mAstModule->cell();
}


//////////////////////////////////////////////////////////////////////
// クラス EiModule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModule::EiModule()
{
}

// @brief デストラクタ
EiModule::~EiModule()
{
}

// @brief ポート配列とIO配列を初期化する．
void
EiModule::init(
  SizeType port_num,
  SizeType io_num
)
{
  mPortList = std::vector<EiPort>(port_num);
  mIODeclList.reserve(io_num);
}

// @brief 型の取得
VpiObjType
EiModule::type() const
{
  return VpiObjType::Module;
}

// @brief ファイル位置の取得
FileRegion
EiModule::file_region() const
{
  return head().file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiModule::parent_scope() const
{
  return head().parent();
}

// @brief definition location を返す．
FileRegion
EiModule::def_file_region() const
{
  return head().def_file_region();
}

// @brief definition name を返す．
std::string
EiModule::def_name() const
{
  return head().def_name();
}

// @brief cell instance のチェック
bool
EiModule::is_cell_instance() const
{
  return head().is_cell_instance();
}

// @brief protect のチェック
bool
EiModule::is_protected() const
{
  return head().is_protected();
}

// @brief top module の時 true を返す．
bool
EiModule::is_top_module() const
{
  return head().is_top_module();
}

// @brief time unit を返す．
int
EiModule::time_unit() const
{
  return head().time_unit();
}

// @brief time precision を返す．
int
EiModule::time_precision() const
{
  return head().time_precision();
}

// @brief default net type を返す．
VpiNetType
EiModule::def_net_type() const
{
  return head().def_net_type();
}

// @brief unconnected drive を返す．
VpiUnconnDrive
EiModule::unconn_drive() const
{
  return head().unconn_drive();
}

// @brief default delay mode を返す．
VpiDefDelayMode
EiModule::def_delay_mode() const
{
  return head().def_delay_mode();
}

// @brief default decay time を返す．
int
EiModule::def_decay_time() const
{
  return head().def_decay_time();
}

// @brief config 情報を返す．
std::string
EiModule::config() const
{
  return head().config();
}

// @brief library 情報を返す．
std::string
EiModule::library() const
{
  return head().library();
}

// @brief cell 情報を返す．
std::string
EiModule::cell() const
{
  return head().cell();
}

// @brief ポート数を返す．
SizeType
EiModule::port_num() const
{
  return head().port_num();
}

// @brief ポートの取得
const VlPort*
EiModule::port(
  SizeType pos
) const
{
  if ( pos >= port_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return &mPortList[pos];
}

// @brief ポートのリストの取得
std::vector<const VlPort*>
EiModule::port_list() const
{
  std::vector<const VlPort*> ans_list;
  ans_list.reserve(port_num());
  for ( auto& port: mPortList ) {
    ans_list.push_back(&port);
  }
  return ans_list;
}

// @brief 入出力数を得る．
SizeType
EiModule::io_num() const
{
  return head().io_num();
}

// @brief 入出力の取得
const VlIODecl*
EiModule::io(
  SizeType pos
) const
{
  if ( pos >= io_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return &mIODeclList[pos];
}

// @brief 入出力のリストの取得
std::vector<const VlIODecl*>
EiModule::io_list() const
{
  std::vector<const VlIODecl*> ans_list;
  ans_list.reserve(io_num());
  for ( auto& io: mIODeclList ) {
    ans_list.push_back(&io);
  }
  return ans_list;
}

// @brief 宣言要素に対応する入出力を得る．
const VlIODecl*
EiModule::find_io(
  const VlDecl* decl
) const
{
  if ( mIODict.count(decl) > 0 ) {
    return mIODict.at(decl);
  }
  return nullptr;
}

// @brief 入出力を追加する．
void
EiModule::add_iodecl(
  ElbIOHead* head,
  const AstIOItem* ast_item,
  const VlDecl* decl
)
{
  mIODeclList.push_back({head, ast_item, decl});
  auto io_decl = &mIODeclList.back();
  mIODict.emplace(decl, io_decl);
}

// @brief ポートの初期設定を行う．
void
EiModule::init_port(
  SizeType index,
  const AstPort* ast_port,
  ElbExpr* low_conn,
  VpiDir dir
)
{
  if ( index >= port_num() ) {
    throw std::out_of_range{"index is out of range"};
  }
  mPortList[index].init(this, ast_port, index, low_conn, dir);
}

// @brief ポートの high_conn を接続する．
void
EiModule::set_port_high_conn(
  SizeType index,
  ElbExpr* high_conn,
  bool conn_by_name
)
{
  if ( index >= port_num() ) {
    throw std::out_of_range{"index is out of range"};
  }
  mPortList[index].set_high_conn(high_conn, conn_by_name);
}


//////////////////////////////////////////////////////////////////////
// クラス EiModule1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModule1::EiModule1()
{
}

// @brief デストラクタ
EiModule1::~EiModule1()
{
}

// @brief 初期設定を行う．
void
EiModule1::init(
  SizeType port_num,
  SizeType io_num,
  EiModuleArray* module_array,
  int index
)
{
  EiModule::init(port_num, io_num);

  mModuleArray = module_array;
  mIndex = index;

  // 名前の生成
}

// @brief 名前の取得
std::string
EiModule1::name() const
{
  std::ostringstream buf;
  buf << mModuleArray->name() << "[" << mIndex << "]";
  return buf.str();
}

// @brief 配列要素の時 true を返す．
bool
EiModule1::is_array() const
{
  return true;
}

// @brief インデックスの値を返す．
int
EiModule1::index() const
{
  return mIndex;
}

// @brief 親の配列を返す．
const VlModuleArray*
EiModule1::module_array() const
{
  return mModuleArray;
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModule1::head() const
{
  return mModuleArray->head();
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModule1::head()
{
  return mModuleArray->head();
}


//////////////////////////////////////////////////////////////////////
// クラス EiModule2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
EiModule2::EiModule2(
  const VlScope* parent,
  const AstModule* ast_module,
  const AstItem* ast_head,
  const AstInst* ast_inst
) : mHead{parent, ast_module, ast_head, ast_inst}
{
  auto port_num = ast_module->port_num();
  auto io_num = ast_module->iodecl_num();
  init(port_num, io_num);
}

// デストラクタ
EiModule2::~EiModule2()
{
}

// @brief 名前の取得
std::string
EiModule2::name() const
{
  return mHead.name();
}

// @brief 配列要素の時 true を返す．
bool
EiModule2::is_array() const
{
  return false;
}

// @brief インデックスの値を返す．
int
EiModule2::index() const
{
  return 0;
}

// @brief 親の配列を返す．
const VlModuleArray*
EiModule2::module_array() const
{
  return nullptr;
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModule2::head() const
{
  return mHead;
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModule2::head()
{
  return mHead;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiModuleArray
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiModuleArray::EiModuleArray(
  const VlScope* parent,
  const AstModule* ast_module,
  const AstItem* ast_head,
  const AstInst* ast_inst,
  const AstRange* ast_range,
  const RangeVal& range
) : mHead(parent, ast_module, ast_head, ast_inst),
    mRange(ast_range, range),
    mArray(mRange.calc_size())
{
  auto port_num = ast_module->port_num();
  auto io_num = ast_module->iodecl_num();
  auto n = mArray.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    int index = range.index(i);
    mArray[i].init(port_num, io_num, this, index);
  }
}

// @brief デストラクタ
EiModuleArray::~EiModuleArray()
{
}

// @brief 型の取得
VpiObjType
EiModuleArray::type() const
{
  return VpiObjType::Module;
}

// @brief ファイル位置の取得
FileRegion
EiModuleArray::file_region() const
{
  return mHead.file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiModuleArray::parent_scope() const
{
  return mHead.parent();
}

// @brief 名前の取得
std::string
EiModuleArray::name() const
{
  return mHead.name();
}

// @brief 範囲の MSB の値を返す．
int
EiModuleArray::left_range_val() const
{
  return mRange.left;
}

// @brief 範囲の LSB の値を返す．
int
EiModuleArray::right_range_val() const
{
  return mRange.right;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiModuleArray::left_range_string() const
{
  return mRange.left_string();
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiModuleArray::right_range_string() const
{
  return mRange.right_string();
}

// @brief 要素数を返す．
SizeType
EiModuleArray::elem_num() const
{
  return mRange.calc_size();
}

// @brief 要素を返す．
const VlModule*
EiModuleArray::elem_by_offset(
  SizeType offset
) const
{
  if ( offset >= elem_num() ) {
    throw std::out_of_range{"offset is out of range"};
  }
  return &mArray[offset];
}

// @brief 要素を返す．
const VlModule*
EiModuleArray::elem_by_index(
  int index
) const
{
  SizeType offset;
  if ( mRange.calc_offset(index, offset) ) {
    return elem_by_offset(offset);
  }
  // index が範囲外だった．
  throw std::logic_error{"Should not be reached"};
}

// @brief 要素を取り出す．
ElbModule*
EiModuleArray::elem(
  SizeType index
)
{
  return &mArray[index];
}

// @brief ヘッダ情報を返す．
const EiModuleHead&
EiModuleArray::head() const
{
  return mHead;
}

// @brief ヘッダ情報を返す．
EiModuleHead&
EiModuleArray::head()
{
  return mHead;
}

// @brief 範囲を返す．
const RangeVal&
EiModuleArray::range() const
{
  return mRange;
}

END_NAMESPACE_YM_VERILOG
