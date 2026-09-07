
/// @file EiPort.cc
/// @brief EiPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiPort.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbExpr.h"
#include "ym/vl/AstPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiPortBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPortBase::EiPortBase(
  const VlModule* module,
  SizeType index,
  ElbExpr* low_conn,
  VpiDir dir
) : mModule{module},
    mIndex{index},
    mHighConn{nullptr},
    mLowConn{low_conn},
    mDirection{dir},
    mConnByName{false}
{
}

// @brief high_conn を接続する．
void
EiPortBase::set_high_conn(
  ElbExpr* high_conn,
  bool conn_by_name
)
{
  mHighConn = high_conn;
  mConnByName = conn_by_name;
}

// @brief 型の取得
VpiObjType
EiPortBase::type() const
{
  return VpiObjType::Port;
}

// @brief 入出力の区別を得る．
VpiDir
EiPortBase::direction() const
{
  return mDirection;
}

// @brief ビット幅を返す．
SizeType
EiPortBase::bit_size() const
{
  return mLowConn->bit_size();
}

// @brief 名前による接続を持つとき true を返す．
bool
EiPortBase::is_conn_by_name() const
{
  return mConnByName;
}

// @brief 親のモジュールを取出す
const VlModule*
EiPortBase::module() const
{
  return mModule;
}

// @brief ポートリストの何番目のポートかを表すインデックスを返す．
SizeType
EiPortBase::port_index() const
{
  return mIndex;
}

// @brief 上位の接続先を返す．
const VlExpr*
EiPortBase::high_conn() const
{
  return mHighConn;
}

// @brief 下位の接続先を返す．
const VlExpr*
EiPortBase::low_conn() const
{
  return mLowConn;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPort1
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
EiPort1::EiPort1(
  const VlModule* module,
  const AstPort& ast_port,
  SizeType index,
  ElbExpr* low_conn,
  VpiDir dir
) : EiPortBase(module, index, low_conn, dir),
    mAstPort{ast_port}
{
}

// @brief ファイル位置の取得
FileRegion
EiPort1::file_region() const
{
  return mAstPort.file_region();
}

// @brief 明示的に名前がついているとき true を返す．
bool
EiPort1::has_explicit_name() const
{
  return mAstPort.ext_name() != nullptr;
}

// @brief 名前を返す．
std::string
EiPort1::name() const
{
  return mAstPort.ext_name();
}


//////////////////////////////////////////////////////////////////////
// クラス EiPort2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPort2::EiPort2(
  const VlModule* module,
  const AstIOItem& ast_ioitem,
  SizeType index,
  ElbExpr* low_conn,
  VpiDir dir
) : EiPortBase(module, index, low_conn, dir),
    mAstIOItem{ast_ioitem}
{
}

// @brief ファイル位置の取得
FileRegion
EiPort2::file_region() const
{
  return mAstIOItem.file_region();
}

// @brief 明示的に名前がついているとき true を返す．
bool
EiPort2::has_explicit_name() const
{
  return true;
}

// @brief 名前を返す．
std::string
EiPort2::name() const
{
  return mAstIOItem.name();
}

END_NAMESPACE_YM_VERILOG
