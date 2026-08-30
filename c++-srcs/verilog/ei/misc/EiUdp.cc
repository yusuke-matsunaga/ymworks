
/// @file EiUdp.cc
/// @brief EiUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiUdp.h"

#include "elaborator/ElbExpr.h"

#include "ym/vl/AstUdp.h"
#include "ym/vl/AstDecl.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief UDP定義を生成する．
ElbUdpDefn*
EiFactory::new_UdpDefn(
  const AstUdp* ast_udp,
  bool is_protected
)
{
  auto port_num = ast_udp->port_list().size();
  auto table_size = ast_udp->table_list().size();
  return new EiUdpDefn(ast_udp, is_protected,
		       port_num, table_size);
}


//////////////////////////////////////////////////////////////////////
// クラス EiUdpDefn
//////////////////////////////////////////////////////////////////////

// @param[in] ast_udp パース木の UDP 定義
EiUdpDefn::EiUdpDefn(
  const AstUdp* ast_udp,
  bool is_protected,
  SizeType io_num,
  SizeType table_num
) : mAstUdp{ast_udp},
    mProtected{is_protected},
    mIODeclList(io_num),
    mInitExpr{nullptr},
    mInitVal{VlScalarVal::x()},
    mTableEntryList(table_num)
{
}

// @brief デストラクタ
EiUdpDefn::~EiUdpDefn()
{
}

// @brief 型の取得
VpiObjType
EiUdpDefn::type() const
{
  return VpiObjType::UdpDefn;
}

// @brief ファイル位置を返す．
FileRegion
EiUdpDefn::file_region() const
{
  return mAstUdp->file_region();
}

// @brief 定義された名前を返す．
std::string
EiUdpDefn::def_name() const
{
  return mAstUdp->name();
}

// @brief primitive type を返す．
VpiPrimType
EiUdpDefn::prim_type() const
{
  return mAstUdp->prim_type();
}

// @brief ポート数を返す．
SizeType
EiUdpDefn::port_num() const
{
  return mIODeclList.size();
}

// @brief 入力の宣言要素を返す．
const VlIODecl*
EiUdpDefn::input(
  SizeType pos
) const
{
  if ( pos >= port_num() - 1 ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return &mIODeclList[pos];
}

// @brief 出力の宣言要素を返す．
const VlIODecl*
EiUdpDefn::output() const
{
  return &mIODeclList[port_num() - 1];
}

// @brief protected かどうかを返す．
bool
EiUdpDefn::is_protected() const
{
  return mProtected;
}

// @brief 初期値を返す．
VlScalarVal
EiUdpDefn::init_val() const
{
  return mInitVal;
}

// @brief 初期値を表す文字列を返す．
std::string
EiUdpDefn::init_val_string() const
{
  if ( mInitExpr ) {
    return mInitExpr->decompile();
  }
  return {};
}

// @brief table entry の行数を返す．
SizeType
EiUdpDefn::table_size() const
{
  return mTableEntryList.size();
}

// @brief table entry を返す．
const VlTableEntry*
EiUdpDefn::table_entry(
  SizeType pos
) const
{
  if ( pos >= table_size() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return &mTableEntryList[pos];
}

// @brief 入出力オブジェクトの内容を設定する．
void
EiUdpDefn::set_io(
  SizeType pos,
  const AstIOHead* ast_header,
  const AstIOItem* ast_item
)
{
  if ( pos >= table_size() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  mIODeclList[pos].set(ast_header, ast_item);
}

// @brief 初期値を設定する．
void
EiUdpDefn::set_initial(
  const AstExpr* init_expr,
  const VlScalarVal& init_val
)
{
  mInitExpr = init_expr;
  mInitVal = init_val;
}

// @brief table entry の内容を設定する．
void
EiUdpDefn::set_tableentry(
  SizeType pos,
  const AstUdpEntry* ast_udp_entry,
  const std::vector<VlUdpVal>& vals
)
{
  mTableEntryList[pos].set(ast_udp_entry, vals);
}


//////////////////////////////////////////////////////////////////////
/// クラス EiUdpIO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiUdpIO::EiUdpIO()
{
}

// @brief デストラクタ
EiUdpIO::~EiUdpIO()
{
}

// @brief 型の取得
VpiObjType
EiUdpIO::type() const
{
  return VpiObjType::IODecl;
}

// ファイル位置を返す．
FileRegion
EiUdpIO::file_region() const
{
  return mAstItem->file_region();
}

// @brief 名前を返す．
std::string
EiUdpIO::name() const
{
  return mAstItem->name();
}

// @brief 方向を返す．
VpiDir
EiUdpIO::direction() const
{
  if ( mAstHeader->direction() == VpiDir::Inout ) {
    throw std::logic_error{"mAstHeader->direction() == VpiDir::Inout"};
  }
  return mAstHeader->direction();
}

// @brief 符号の属性の取得
bool
EiUdpIO::is_signed() const
{
  return false;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiUdpIO::has_range() const
{
  return false;
}

// @brief MSB の値を返す．
int
EiUdpIO::left_range_val() const
{
  return 0;
}

// @brief LSB の値を返す．
int
EiUdpIO::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiUdpIO::left_range_string() const
{
  return {};
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiUdpIO::right_range_string() const
{
  return {};
}

// @brief サイズを返す．
SizeType
EiUdpIO::bit_size() const
{
  return 1;
}

// @brief 対応する宣言要素を返す．
const VlDecl*
EiUdpIO::decl() const
{
  return nullptr;
}

// @brief 親のモジュールの取得
const VlModule*
EiUdpIO::module() const
{
  return nullptr;
}

// @brief 親のUDP定義を返す．
const VlUdpDefn*
EiUdpIO::udp_defn() const
{
  return mUdp;
}

// @brief 親のタスク/の取得
const VlTaskFunc*
EiUdpIO::task() const
{
  return nullptr;
}

// @brief 親の関数の取得
const VlTaskFunc*
EiUdpIO::function() const
{
  return nullptr;
}

// @brief 親のUDPを設定する．
void
EiUdpIO::set_udp(
  ElbUdpDefn* udp
)
{
  mUdp = udp;
}

// @brief 内容を設定する．
void
EiUdpIO::set(
  const AstIOHead* ast_header,
  const AstIOItem* ast_item
)
{
  mAstHeader = ast_header;
  mAstItem = ast_item;
}


//////////////////////////////////////////////////////////////////////
/// クラス EiTableEntry
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiTableEntry::EiTableEntry()
{
}

// @brief デストラクタ
EiTableEntry::~EiTableEntry()
{
}

// @brief 型の取得
VpiObjType
EiTableEntry::type() const
{
  return VpiObjType::TableEntry;
}

// @brief ファイル位置を返す．
FileRegion
EiTableEntry::file_region() const
{
  return mAstUdpEntry->file_region();
}

// @brief 一行の要素数を返す．
SizeType
EiTableEntry::size() const
{
  SizeType row_size = mUdp->port_num();
  if ( mUdp->prim_type() == VpiPrimType::Seq ) {
    ++ row_size;
  }
  return row_size;
}

// @brief pos 番目の位置の値を返す．
VlUdpVal
EiTableEntry::val(
  SizeType pos
) const
{
  if ( pos >= size() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mValArray[pos];
}

// @brief 一行文の内容を表す文字列をつくる．
std::string
EiTableEntry::str() const
{
  auto n = size();
  auto in = n - 1; // 出力変数の分を減らす
  if ( mUdp->prim_type() == VpiPrimType::Seq ) {
    -- in; // さらに状態変数の分を減らす．
  }
  auto in1 = in - 1;
  auto n1 = n - 1;
  std::string s;
  for ( int pos = 0; pos < n; ++ pos ) {
    s += val(pos).to_string();
    if ( pos < in1 ) {
      s += " ";
    }
    else if ( pos < n1 ) {
      s += " : ";
    }
  }
  return s;
}

// @brief 初期化する．
void
EiTableEntry::init(
  ElbUdpDefn* udp
)
{
  mUdp = udp;
}

// @brief 設定する．
void
EiTableEntry::set(
  const AstUdpEntry* ast_entry,
  const std::vector<VlUdpVal>& vals
)
{
  mAstUdpEntry = ast_entry;
  mValArray = vals;
}

END_NAMESPACE_YM_VERILOG
