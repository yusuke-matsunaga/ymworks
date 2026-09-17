
/// @file EiDecl.cc
/// @brief EiDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiDecl.h"
#include "elaborator/ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素を生成する．
ElbDecl*
EiFactory::new_Decl(
  ElbDeclHead* head,
  const AstDeclItem& ast_item,
  const VlExpr* init
)
{
  if ( init != nullptr ) {
    return new EiDeclI(head, ast_item, init);
  }
  else {
    return new EiDecl(head, ast_item);
  }
}

// @brief 宣言要素を生成する．
ElbDecl*
EiFactory::new_Decl(
  ElbDeclHead* head,
  const AstIOItem& ast_item,
  const VlExpr* init
)
{
  if ( init != nullptr ) {
    return new EiDecl2I(head, ast_item, init);
  }
  else {
    return new EiDecl2(head, ast_item);
  }
}

// @brief 宣言要素を生成する．
ElbDecl*
EiFactory::new_Decl(
  ElbDeclHead* head,
  const AstItem& ast_item
)
{
  return new EiDecl3(head, ast_item);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclBase::EiDeclBase(
  ElbDeclHead* head
) : mHead{head},
    mAuxSign{false}
{
}

// @brief デストラクタ
EiDeclBase::~EiDeclBase()
{
}

// @brief 型の取得
VpiObjType
EiDeclBase::type() const
{
  return mHead->type();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlScope*
EiDeclBase::parent_scope() const
{
  return mHead->parent_scope();
}

// @brief 値の型を返す．
VlValueType
EiDeclBase::value_type() const
{
  switch ( type() ) {
  case VpiObjType::Net:
  case VpiObjType::Reg:
    return VlValueType(is_signed(), true, bit_size());

  case VpiObjType::IntegerVar:
    return VlValueType::int_type();

  case VpiObjType::RealVar:
    return VlValueType::real_type();

  case VpiObjType::TimeVar:
    return VlValueType::time_type();

  case VpiObjType::Parameter:
  case VpiObjType::SpecParam:
  case VpiObjType::Constant:
    // ここにはこない
    ASSERT_NOT_REACHED;
    break;

  default:
    // 上記以外は形無し
    break;
  }

  return VlValueType();
}

// @brief 符号の取得
bool
EiDeclBase::is_signed() const
{
  return mHead->is_signed() || mAuxSign;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDeclBase::has_range() const
{
  return mHead->has_range();
}

// @brief 範囲の MSB の値を返す．
int
EiDeclBase::left_range_val() const
{
  return mHead->left_range_val();
}

// @brief 範囲の LSB の値を返す．
int
EiDeclBase::right_range_val() const
{
  return mHead->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiDeclBase::left_range_string() const
{
  return mHead->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiDeclBase::right_range_string() const
{
  return mHead->right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDeclBase::is_big_endian() const
{
  return mHead->is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDeclBase::is_little_endian() const
{
  return mHead->is_little_endian();
}

// @brief ビット幅を返す．
SizeType
EiDeclBase::bit_size() const
{
  return mHead->bit_size();
}

// @brief オフセット値の取得
bool
EiDeclBase::calc_bit_offset(
  int index,
  SizeType& offset
) const
{
  return mHead->calc_bit_offset(index, offset);
}

// @brief データ型の取得
VpiVarType
EiDeclBase::data_type() const
{
  return mHead->data_type();
}

// @brief net 型の取得
VpiNetType
EiDeclBase::net_type() const
{
  return mHead->net_type();
}

// @brief vectored|scalared 属性の取得
VpiVsType
EiDeclBase::vs_type() const
{
  return mHead->vs_type();
}

// @brief drive0 strength の取得
VpiStrength
EiDeclBase::drive0() const
{
  return mHead->drive0();
}

// @brief drive1 strength の取得
VpiStrength
EiDeclBase::drive1() const
{
  return mHead->drive1();
}

// @brief charge strength の取得
VpiStrength
EiDeclBase::charge() const
{
  return mHead->charge();
}

// @brief delay の取得
const VlDelay*
EiDeclBase::delay() const
{
  return mHead->delay();
}

// @brief 定数値を持つ型のときに true を返す．
bool
EiDeclBase::is_consttype() const
{
  return false;
}

// @brief 初期値の取得
const VlExpr*
EiDeclBase::init_value() const
{
  return nullptr;
}

// @brief localparam のときに true 返す．
bool
EiDeclBase::is_local_param() const
{
  return false;
}

// @brief 符号付きに補正する．
void
EiDeclBase::set_signed()
{
  mAuxSign = true;
}

// @brief 初期値の設定
void
EiDeclBase::set_init(
  const VlExpr* expr
)
{
  throw std::logic_error{"EiDeclBase::set_init()"};
}


//////////////////////////////////////////////////////////////////////
// クラス EiDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDecl::EiDecl(
  ElbDeclHead* head,
  const AstDeclItem& ast_item
) : EiDeclBase(head),
    mAstItem{ast_item}
{
}

// @brief デストラクタ
EiDecl::~EiDecl()
{
}

// @brief ファイル位置の取得
FileRegion
EiDecl::file_region() const
{
  return mAstItem.file_region();
}

// @brief 名前の取得
std::string
EiDecl::name() const
{
  return mAstItem.name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclI
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDeclI::EiDeclI(
  ElbDeclHead* head,
  const AstDeclItem& ast_item,
  const VlExpr* init
) : EiDecl(head, ast_item),
    mInit{init}
{
}

// @brief デストラクタ
EiDeclI::~EiDeclI()
{
}

// @brief 初期値の取得
const VlExpr*
EiDeclI::init_value() const
{
  return mInit;
}

// @brief 初期値の設定
void
EiDeclI::set_init(
  const VlExpr* expr
)
{
  mInit = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDecl2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDecl2::EiDecl2(
  ElbDeclHead* head,
  const AstIOItem& ast_item
) : EiDeclBase(head),
    mAstItem{ast_item}
{
}

// @brief デストラクタ
EiDecl2::~EiDecl2()
{
}

// @brief ファイル位置の取得
FileRegion
EiDecl2::file_region() const
{
  return mAstItem.file_region();
}

// @brief 名前の取得
std::string
EiDecl2::name() const
{
  return mAstItem.name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiDecl2I
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDecl2I::EiDecl2I(
  ElbDeclHead* head,
  const AstIOItem& ast_item,
  const VlExpr* init
) : EiDecl2(head, ast_item),
    mInit{init}
{
}

// @brief デストラクタ
EiDecl2I::~EiDecl2I()
{
}

// @brief 初期値の取得
const VlExpr*
EiDecl2I::init_value() const
{
  return mInit;
}

// @brief 初期値の設定
void
EiDecl2I::set_init(
  const VlExpr* expr
)
{
  mInit = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDecl3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDecl3::EiDecl3(
  ElbDeclHead* head,
  const AstItem& ast_item
) : EiDeclBase(head),
    mAstItem{ast_item}
{
}

// @brief デストラクタ
EiDecl3::~EiDecl3()
{
}

// @brief ファイル位置の取得
FileRegion
EiDecl3::file_region() const
{
  return mAstItem.file_region();
}

// @brief 名前の取得
std::string
EiDecl3::name() const
{
  return mAstItem.name();
}

END_NAMESPACE_YM_VERILOG
