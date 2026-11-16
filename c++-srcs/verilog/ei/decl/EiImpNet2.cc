
/// @file EiImpNet2.cc
/// @brief EiImpNet2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiImpNet2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief 暗黙のネットを生成する．
ElbDecl*
ElbMgr::new_ImpNet(
  const VlScope* parent,
  const FileRegion& loc,
  SizeType width
)
{
  auto decl = new_decl(
    [&](SizeType decl_id) -> ElbDecl* {
      return new EiImpNet2(parent, loc, width, decl_id);
    }, vpiNet);
  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiImpNet2
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiImpNet2::type() const
{
  return VpiObjType::Net;
}

// @brief ファイル位置の取得
FileRegion
EiImpNet2::file_region() const
{
  return mFileRegion;
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiImpNet2::parent_scope() const
{
  return mParent;
}

// @brief 名前の取得
std::string
EiImpNet2::name() const
{
  return {};
}

// @brief 値の型を返す．
VlValueType
EiImpNet2::value_type() const
{
  return VlValueType(false, true, mWidth);
}

// @brief 符号の取得
bool
EiImpNet2::is_signed() const
{
  return false;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiImpNet2::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
int
EiImpNet2::left_range_val() const
{
  return mWidth - 1;
}

// @brief 範囲の LSB の値を返す．
int
EiImpNet2::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiImpNet2::left_range_string() const
{
  return {};
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiImpNet2::right_range_string() const
{
  return {};
}

// @brief left_range >= right_range の時に true を返す．
bool
EiImpNet2::is_big_endian() const
{
  return false;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiImpNet2::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
SizeType
EiImpNet2::bit_size() const
{
  return mWidth;
}

// @brief オフセット値の取得
bool
EiImpNet2::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  offset = index;
  return true;
}

// @brief データ型の取得
VpiVarType
EiImpNet2::data_type() const
{
  return VpiVarType::None;
}

// @brief net 型の取得
VpiNetType
EiImpNet2::net_type() const
{
  return VpiNetType::Wire;
}

// @brief vectored|scalared 属性の取得
VpiVsType
EiImpNet2::vs_type() const
{
  return VpiVsType::None;
}

// @brief drive0 strength の取得
VpiStrength
EiImpNet2::drive0() const
{
  return VpiStrength::NoStrength;
}

// @brief drive1 strength の取得
VpiStrength
EiImpNet2::drive1() const
{
  return VpiStrength::NoStrength;
}

// @brief charge strength の取得
VpiStrength
EiImpNet2::charge() const
{
  return VpiStrength::NoStrength;
}

// @brief delay の取得
const VlDelay*
EiImpNet2::delay() const
{
  return nullptr;
}

// @brief 定数値を持つ型のときに true を返す．
bool
EiImpNet2::is_consttype() const
{
  return false;
}

// @brief 初期値の取得
const VlExpr*
EiImpNet2::init_value() const
{
  return nullptr;
}

// @brief localparam のときに true 返す．
bool
EiImpNet2::is_local_param() const
{
  return false;
}

// @brief 要素番号を返す．
SizeType
EiImpNet2::decl_id() const
{
  return mDeclId;
}

// @brief 符号付きに補正する．
void
EiImpNet2::set_signed()
{
  throw std::logic_error{"EiImpNet2::set_signed()"};
}

// @brief 初期値の設定
void
EiImpNet2::set_init(
  const VlExpr* expr
)
{
  throw std::logic_error{"EiImpNet2::set_init()"};
}

END_NAMESPACE_YM_VERILOG
