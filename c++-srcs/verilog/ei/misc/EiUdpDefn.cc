
/// @file EiUdpDefn.cc
/// @brief EiUdpDefn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiUdpDefn.h"
#include "ei/EiUdpIO.h"
#include "ei/EiTableEntry.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief UDP定義を生成する．
ElbUdpDefn*
EiFactory::new_UdpDefn(
  const AstUdp& ast_udp,
  bool is_protected
)
{
  auto io_num = ast_udp.io_num();
  auto table_size = ast_udp.table_list().size();
  return new EiUdpDefn(ast_udp, is_protected,
		       io_num, table_size);
}


//////////////////////////////////////////////////////////////////////
// クラス EiUdpDefn
//////////////////////////////////////////////////////////////////////

// @param[in] ast_udp パース木の UDP 定義
EiUdpDefn::EiUdpDefn(
  const AstUdp& ast_udp,
  bool is_protected,
  SizeType io_num,
  SizeType table_num
) : mAstUdp{ast_udp},
    mProtected{is_protected},
    mInitExpr{nullptr},
    mInitVal{VlScalarVal::x()}
{
  mIODeclList.reserve(io_num);
  mTableEntryList.reserve(table_num);
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
  return mAstUdp.file_region();
}

// @brief 定義された名前を返す．
std::string
EiUdpDefn::def_name() const
{
  return mAstUdp.name();
}

// @brief primitive type を返す．
VpiPrimType
EiUdpDefn::prim_type() const
{
  return mAstUdp.prim_type();
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
  if ( mInitExpr.is_invalid() ) {
    return {};
  }
  return mInitExpr.decompile();
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

// @brief 入出力オブジェクトを追加する．
void
EiUdpDefn::add_io(
  const AstIOHead& ast_header,
  const AstIOItem& ast_item
)
{
  mIODeclList.push_back(EiUdpIO(this, ast_header, ast_item));
}

// @brief 初期値を設定する．
void
EiUdpDefn::set_initial(
  const AstExpr& init_expr,
  const VlScalarVal& init_val
)
{
  mInitExpr = init_expr;
  mInitVal = init_val;
}

// @brief table entry の内容を追加する．
void
EiUdpDefn::add_tableentry(
  const AstUdpEntry& ast_udp_entry,
  const std::vector<VlUdpVal>& vals
)
{
  mTableEntryList.push_back(EiTableEntry(this, ast_udp_entry, vals));
}

END_NAMESPACE_YM_VERILOG
