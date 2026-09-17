
/// @file EiIODecl.cc
/// @brief EiIODecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiIODecl.h"
#include "elaborator/ElbDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiIODecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiIODecl::EiIODecl(
  ElbIOHead* head,
  const AstIOItem& ast_item,
  const VlDecl* decl
) : mHead{head},
    mAstItem{ast_item},
    mDecl{decl}
{
}

// @brief デストラクタ
EiIODecl::~EiIODecl()
{
}

// @brief 型の取得
VpiObjType
EiIODecl::type() const
{
  return VpiObjType::IODecl;
}

// @brief ファイル位置の取得
FileRegion
EiIODecl::file_region() const
{
  return mAstItem.file_region();
}

// @brief 名前を返す．
std::string
EiIODecl::name() const
{
  return mAstItem.name();
}

// @brief 方向を返す．
VpiDir
EiIODecl::direction() const
{
  return mHead->direction();
}

// @brief 符号の取得
bool
EiIODecl::is_signed() const
{
  return mDecl->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiIODecl::has_range() const
{
  return mDecl->has_range();
}

// @brief 範囲の MSB の値を返す．
int
EiIODecl::left_range_val() const
{
  return mDecl->left_range_val();
}

// @brief 範囲の LSB の値を返す．
int
EiIODecl::right_range_val() const
{
  return mDecl->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
std::string
EiIODecl::left_range_string() const
{
  return mDecl->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
std::string
EiIODecl::right_range_string() const
{
  return mDecl->right_range_string();
}

// @brief ビット幅を返す．
SizeType
EiIODecl::bit_size() const
{
  return mDecl->bit_size();
}

// @brief 対応する宣言要素を返す．
const VlDecl*
EiIODecl::decl() const
{
  return mDecl;
}

// @brief 親のモジュールの取得
const VlModule*
EiIODecl::module() const
{
  return mHead->module();
}

// @brief 親の UDP の取得
const VlUdpDefn*
EiIODecl::udp_defn() const
{
  return nullptr;
}

// @brief 親のタスクの取得
const VlTaskFunc*
EiIODecl::task() const
{
  return mHead->task();
}

// @brief 親の関数の取得
const VlTaskFunc*
EiIODecl::function() const
{
  return mHead->function();
}

END_NAMESPACE_YM_VERILOG
