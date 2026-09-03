
/// @file EiDeclHead.cc
/// @brief EiDeclHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiDeclHead.h"
#include "ym/vl/VlDelay.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素のヘッダを生成する．
ElbDeclHead*
EiFactory::new_DeclHead(
  const VlScope* parent,
  const AstDeclHead& ast_head,
  const AstRange& ast_range,
  const RangeVal& range,
  bool has_delay
)
{
  if ( ast_range.is_invalid() ) {
    throw std::logic_error{"ast_range.is_invalid()"};
  }

  if ( has_delay ) {
    return new EiDeclHeadAstVD(parent, ast_head, ast_range, range);
  }
  else {
    return new EiDeclHeadAstV(parent, ast_head, ast_range, range);
  }
}

// @brief 宣言要素のヘッダを生成する．
ElbDeclHead*
EiFactory::new_DeclHead(
  const VlScope* parent,
  const AstDeclHead& ast_head,
  bool delay
)
{
  if ( delay ) {
    return new EiDeclHeadAstD{parent, ast_head};
  }
  else {
    return new EiDeclHeadAst{parent, ast_head};
  }
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
ElbDeclHead*
EiFactory::new_DeclHead(
  const VlScope* parent,
  const AstIOHead& ast_head,
  VpiAuxType aux_type,
  const AstRange& ast_range,
  const RangeVal& range
)
{
  if ( ast_range.is_invalid() ) {
    throw std::logic_error{"ast_range.is_invalid()"};
  }

  return new EiDeclHeadAst2V(parent, ast_head, aux_type,
			    ast_range, range);
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
ElbDeclHead*
EiFactory::new_DeclHead(
  const VlScope* parent,
  const AstIOHead& ast_head,
  VpiAuxType aux_type
)
{
  return new EiDeclHeadAst2(parent, ast_head, aux_type);
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
ElbDeclHead*
EiFactory::new_DeclHead(
  const VlScope* parent,
  const AstItem& ast_item,
  const AstRange& ast_range,
  const RangeVal& range
)
{
  if ( ast_range.is_invalid() ) {
    throw std::logic_error{"ast_range.is_invalid()"};
  }

  return new EiDeclHeadAst3V(parent, ast_item, ast_range, range);
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
ElbDeclHead*
EiFactory::new_DeclHead(
  const VlScope* parent,
  const AstItem& ast_item
)
{
  return new EiDeclHeadAst3(parent, ast_item);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHead::EiDeclHead(
  const VlScope* parent
) : mParent{parent}
{
}

// @brief デストラクタ
EiDeclHead::~EiDeclHead()
{
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiDeclHead::parent_scope() const
{
  return mParent;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadAst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHeadAst::EiDeclHeadAst(
  const VlScope* parent,
  const AstDeclHead& ast_header
) : EiDeclHead{parent},
    mAstHead{ast_header}
{
}

// @brief デストラクタ
EiDeclHeadAst::~EiDeclHeadAst()
{
}

// @brief 型の取得
VpiObjType
EiDeclHeadAst::type() const
{
  switch ( mAstHead.type() ) {
  case AstDeclHead::Param:
  case AstDeclHead::LocalParam:
    return VpiObjType::Parameter;

  case AstDeclHead::Reg:
    return VpiObjType::Reg;

  case AstDeclHead::Var:
    switch ( mAstHead.data_type() ) {
    case VpiVarType::Integer:  return VpiObjType::IntegerVar;
    case VpiVarType::Real:     return VpiObjType::RealVar;
    case VpiVarType::Time:     return VpiObjType::TimeVar;
    default: break;
    }
    break;

  case AstDeclHead::Net:
    return VpiObjType::Net;

  case AstDeclHead::Event:
    return VpiObjType::NamedEvent;

  case AstDeclHead::SpecParam:
    return VpiObjType::SpecParam;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return VpiObjType::Parameter;
}

// @brief 符号の取得
bool
EiDeclHeadAst::is_signed() const
{
  return mAstHead.is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadAst::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
int
EiDeclHeadAst::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
int
EiDeclHeadAst::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiDeclHeadAst::left_range_string() const
{
  return {};
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiDeclHeadAst::right_range_string() const
{
  return {};
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadAst::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadAst::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
SizeType
EiDeclHeadAst::bit_size() const
{
  switch ( mAstHead.type() ) {
  case AstDeclHead::Reg:
  case AstDeclHead::Net:
    // この型は範囲指定を含まないので 1ビットとなる．
    return 1;

  case AstDeclHead::Param:
  case AstDeclHead::LocalParam:
  case AstDeclHead::Var:
    switch ( mAstHead.data_type() ) {
    case VpiVarType::Integer:
      return kVpiSizeInteger;

    case VpiVarType::Real:
      return kVpiSizeReal;

    case VpiVarType::Time:
      return kVpiSizeTime;

    default:
      // int とみなす．
      return kVpiSizeInteger;
    }
    break;

  case AstDeclHead::Event:
    return 0;

  case AstDeclHead::SpecParam:
    return kVpiSizeInteger;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief オフセット値の取得
bool
EiDeclHeadAst::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  switch ( mAstHead.type() ) {
  case AstDeclHead::Reg:
  case AstDeclHead::Net:
    // この型は範囲指定を含まないので 1ビットとなる．
    if ( index == 0 ) {
      offset = 0;
      return true;
    }
    // 0 以外のインデックスは無効
    return false;

  case AstDeclHead::Param:
  case AstDeclHead::LocalParam:
  case AstDeclHead::Var:
    switch ( mAstHead.data_type() ) {
    case VpiVarType::Real:
      // 実数タイプの部分ビット指定は無効
      return false;

    case VpiVarType::Time:
      if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
	offset = index;
	return true;
      }
      // 範囲外は無効
      return false;

    case VpiVarType::Integer:
    default:
      // int とみなす．
      if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
	offset = index;
	return true;
      }
      // 範囲外は無効
      return false;
    }
    break;

  case AstDeclHead::Event:
    // イベントオブジェクトは部分指定できない．
    // というかたぶん，ここには来ないはず．
    ASSERT_NOT_REACHED;
    return false;

  case AstDeclHead::SpecParam:
    // int とみなす．
    if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
      offset = index;
      return true;
    }
    // 範囲外は無効
    return false;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return false;
}

// @brief データ型の取得
VpiVarType
EiDeclHeadAst::data_type() const
{
  return mAstHead.data_type();
}

// @brief net 型の取得
VpiNetType
EiDeclHeadAst::net_type() const
{
  return mAstHead.net_type();
}

// @brief vectored|scalared 属性の取得
VpiVsType
EiDeclHeadAst::vs_type() const
{
  return mAstHead.vs_type();
}

// @brief drive0 strength の取得
VpiStrength
EiDeclHeadAst::drive0() const
{
  if ( mAstHead.strength().is_invalid() ) {
    return VpiStrength::NoStrength;
  }
  return mAstHead.strength().drive0();
}

// @brief drive1 strength の取得
VpiStrength
EiDeclHeadAst::drive1() const
{
  if ( mAstHead.strength().is_invalid() ) {
    return VpiStrength::NoStrength;
  }
  return mAstHead.strength().drive1();
}

// @brief charge strength の取得
VpiStrength
EiDeclHeadAst::charge() const
{
  if ( mAstHead.strength().is_invalid() ) {
    return VpiStrength::NoStrength;
  }
  return mAstHead.strength().charge();
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadAstD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHeadAstD::EiDeclHeadAstD(
  const VlScope* parent,
  const AstDeclHead& ast_header
) : EiDeclHeadAst{parent, ast_header}
{
}

// @brief デストラクタ
EiDeclHeadAstD::~EiDeclHeadAstD()
{
}

// @brief 遅延式の取得
const VlDelay*
EiDeclHeadAstD::delay() const
{
  return mDelay;
}

// @brief 遅延式の設定
void
EiDeclHeadAstD::set_delay(
  const VlDelay* delay
)
{
  mDelay = delay;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadAstV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHeadAstV::EiDeclHeadAstV(
  const VlScope* parent,
  const AstDeclHead& ast_header,
  const AstRange& ast_range,
  const RangeVal& range
) : EiDeclHeadAst{parent, ast_header},
    mRange(ast_range, range)
{
}

// @brief デストラクタ
EiDeclHeadAstV::~EiDeclHeadAstV()
{
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadAstV::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
int
EiDeclHeadAstV::left_range_val() const
{
  return mRange.left;
}

// @brief 範囲の LSB の値を返す．
int
EiDeclHeadAstV::right_range_val() const
{
  return mRange.right;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiDeclHeadAstV::left_range_string() const
{
  return mRange.left_string();
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiDeclHeadAstV::right_range_string() const
{
  return mRange.right_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadAstV::is_big_endian() const
{
  return mRange.is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadAstV::is_little_endian() const
{
  return mRange.is_little_endian();
}

// @brief ビット幅を返す．
SizeType
EiDeclHeadAstV::bit_size() const
{
  return mRange.calc_size();
}

// @brief オフセット値の取得
bool
EiDeclHeadAstV::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  return mRange.calc_offset(index, offset);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadAstVD
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHeadAstVD::EiDeclHeadAstVD(
  const VlScope* parent,
  const AstDeclHead& ast_header,
  const AstRange& ast_range,
  const RangeVal& range
) : EiDeclHeadAstV(parent, ast_header, ast_range, range),
    mDelay{nullptr}
{
}

// @brief デストラクタ
EiDeclHeadAstVD::~EiDeclHeadAstVD()
{
}

// @brief delay の取得
const VlDelay*
EiDeclHeadAstVD::delay() const
{
  return mDelay;
}

// @brief 遅延式の設定
void
EiDeclHeadAstVD::set_delay(
  const VlDelay* delay
)
{
  mDelay = delay;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadAst2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHeadAst2::EiDeclHeadAst2(
  const VlScope* parent,
  const AstIOHead& ast_header,
  VpiAuxType aux_type
) : EiDeclHead(parent),
    mAstHead{ast_header},
    mAuxType{aux_type}
{
}

// @brief デストラクタ
EiDeclHeadAst2::~EiDeclHeadAst2()
{
}

// @brief 型の取得
VpiObjType
EiDeclHeadAst2::type() const
{
  switch ( mAuxType ) {
  case VpiAuxType::Net: return VpiObjType::Net;
  case VpiAuxType::Reg: return VpiObjType::Reg;
  case VpiAuxType::Var:
    switch ( mAstHead.var_type() ) {
    case VpiVarType::Integer:  return VpiObjType::IntegerVar;
    case VpiVarType::Real:     return VpiObjType::RealVar;
    case VpiVarType::Time:     return VpiObjType::TimeVar;
    default: break;
    }
    break;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return VpiObjType::Net;
}

// @brief 符号の取得
bool
EiDeclHeadAst2::is_signed() const
{
  return mAstHead.is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadAst2::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
int
EiDeclHeadAst2::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
int
EiDeclHeadAst2::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiDeclHeadAst2::left_range_string() const
{
  return {};
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiDeclHeadAst2::right_range_string() const
{
  return {};
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadAst2::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadAst2::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
SizeType
EiDeclHeadAst2::bit_size() const
{
  switch ( mAuxType ) {
  case VpiAuxType::Net: return 1;
  case VpiAuxType::Reg: return 1;
  case VpiAuxType::Var:
    switch ( mAstHead.var_type() ) {
    case VpiVarType::Integer:  return kVpiSizeInteger;
    case VpiVarType::Real:     return kVpiSizeReal;
    case VpiVarType::Time:     return kVpiSizeTime;
    default: break;
    }
    break;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief オフセット値の取得
bool
EiDeclHeadAst2::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  switch ( mAuxType ) {
  case VpiAuxType::Net:
  case VpiAuxType::Reg:
    // 範囲指定なしは1ビットとみなす．
    if ( index == 0 ) {
      offset = 0;
      return true;
    }
    // 0 以外は無効
    return false;

  case VpiAuxType::Var:
    switch ( mAstHead.var_type() ) {
    case VpiVarType::Integer:
      if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
	offset = index;
	return true;
      }
      // 範囲外は無効
      return false;

    case VpiVarType::Real:
      // 実数の部分指定は無効
      return false;

    case VpiVarType::Time:
      if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
	return index;
      }
      // 範囲外は無効
      return false;

    default:
      break;
    }
    break;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return false;
}

// @brief データ型の取得
VpiVarType
EiDeclHeadAst2::data_type() const
{
  return mAstHead.var_type();
}

// @brief net 型の取得
VpiNetType
EiDeclHeadAst2::net_type() const
{
  return mAstHead.net_type();
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadAst2V
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHeadAst2V::EiDeclHeadAst2V(
  const VlScope* parent,
  const AstIOHead& ast_header,
  VpiAuxType aux_type,
  const AstRange& ast_range,
  const RangeVal& range
) : EiDeclHeadAst2(parent, ast_header, aux_type),
    mRange(ast_range, range)
{
}

// @brief デストラクタ
EiDeclHeadAst2V::~EiDeclHeadAst2V()
{
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadAst2V::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
int
EiDeclHeadAst2V::left_range_val() const
{
  return mRange.left;
}

// @brief 範囲の LSB の値を返す．
int
EiDeclHeadAst2V::right_range_val() const
{
  return mRange.right;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiDeclHeadAst2V::left_range_string() const
{
  return mRange.left_string();
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiDeclHeadAst2V::right_range_string() const
{
  return mRange.right_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadAst2V::is_big_endian() const
{
  return mRange.is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadAst2V::is_little_endian() const
{
  return mRange.is_little_endian();
}

// @brief ビット幅を返す．
SizeType
EiDeclHeadAst2V::bit_size() const
{
  return mRange.calc_size();
}

// @brief オフセット値の取得
bool
EiDeclHeadAst2V::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  return mRange.calc_offset(index, offset);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadAst3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHeadAst3::EiDeclHeadAst3(
  const VlScope* parent,
  const AstItem& ast_item
) : EiDeclHead(parent),
    mAstItem{ast_item}
{
}

// @brief デストラクタ
EiDeclHeadAst3::~EiDeclHeadAst3()
{
}

// @brief 型の取得
VpiObjType
EiDeclHeadAst3::type() const
{
  switch ( data_type() ) {
  case VpiVarType::None:     return VpiObjType::Reg;
  case VpiVarType::Integer:  return VpiObjType::IntegerVar;
  case VpiVarType::Real:     return VpiObjType::RealVar;
  case VpiVarType::Time:     return VpiObjType::TimeVar;
  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return VpiObjType::Reg;
}

// @brief 符号の取得
bool
EiDeclHeadAst3::is_signed() const
{
  return mAstItem.is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadAst3::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
int
EiDeclHeadAst3::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
int
EiDeclHeadAst3::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiDeclHeadAst3::left_range_string() const
{
  return {};
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiDeclHeadAst3::right_range_string() const
{
  return {};
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadAst3::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadAst3::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
SizeType
EiDeclHeadAst3::bit_size() const
{
  switch ( data_type() ) {
  case VpiVarType::None:     return 1;
  case VpiVarType::Integer:  return kVpiSizeInteger;
  case VpiVarType::Real:     return kVpiSizeReal;
  case VpiVarType::Time:     return kVpiSizeTime;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return 0;
}

// @brief オフセット値の取得
bool
EiDeclHeadAst3::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  switch ( data_type() ) {
  case VpiVarType::None:
    // 指定なしは1ビットとみなす．
    if ( index == 0 ) {
      offset = 0;
      return true;
    }
    return false;

  case VpiVarType::Integer:
    if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
      offset = index;
      return true;
    }
    // 範囲外は無効
    return false;

  case VpiVarType::Real:
    // 実数の部分指定は無効
    return false;

  case VpiVarType::Time:
    if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
      offset = index;
      return true;
    }
    // 範囲外は無効
    return false;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return false;
}

// @brief データ型の取得
VpiVarType
EiDeclHeadAst3::data_type() const
{
  return mAstItem.data_type();
}

// @brief net 型の取得
VpiNetType
EiDeclHeadAst3::net_type() const
{

  return VpiNetType::None;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclHeadAst3V
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclHeadAst3V::EiDeclHeadAst3V(
  const VlScope* parent,
  const AstItem& ast_item,
  const AstRange& ast_range,
  const RangeVal& range
) : EiDeclHeadAst3(parent, ast_item),
    mRange(ast_range, range)
{
}

// @brief デストラクタ
EiDeclHeadAst3V::~EiDeclHeadAst3V()
{
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclHeadAst3V::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
int
EiDeclHeadAst3V::left_range_val() const
{
  return mRange.left;
}

// @brief 範囲の LSB の値を返す．
int
EiDeclHeadAst3V::right_range_val() const
{
  return mRange.right;
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiDeclHeadAst3V::left_range_string() const
{
  return mRange.left_string();
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiDeclHeadAst3V::right_range_string() const
{
  return mRange.right_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclHeadAst3V::is_big_endian() const
{
  return mRange.is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclHeadAst3V::is_little_endian() const
{
  return mRange.is_little_endian();
}

// @brief ビット幅を返す．
SizeType
EiDeclHeadAst3V::bit_size() const
{
  return mRange.calc_size();
}

// @brief オフセット値の取得
bool
EiDeclHeadAst3V::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  return mRange.calc_offset(index, offset);
}

END_NAMESPACE_YM_VERILOG
