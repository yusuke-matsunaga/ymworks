
/// @file EiPort.cc
/// @brief EiPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiInputPort.h"
#include "EiNamedInputPort.h"
#include "EiOutputPort.h"
#include "EiNamedOutputPort.h"
#include "elaborator/ElbExpr.h"
#include "ym/vl/VlTerm.h"
#include "ym/vl/AstPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiPortBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPortBase::EiPortBase(
  const FileRegion& loc,
  const VlModule* module,
  SizeType index,
  const VlTerm* low_conn,
  VpiDir dir
) : mFileRegion{loc},
    mModule{module},
    mLowConn{low_conn}
{
  mChunk = (index << 1);
  switch ( dir ) {
  case VpiDir::Input:       mChunk |= 0; break;
  case VpiDir::Output:      mChunk |= 1; break;
  case VpiDir::Inout:       mChunk |= 2; break;
  case VpiDir::MixedIO:     mChunk |= 3; break;
  case VpiDir::NoDirection: mChunk |= 0; break;
  }
}

// @brief 型の取得
VpiObjType
EiPortBase::type() const
{
  return VpiObjType::Port;
}

// @brief ファイル位置の取得
FileRegion
EiPortBase::file_region() const
{
  return mFileRegion;
}

// @brief 入出力の区別を得る．
VpiDir
EiPortBase::direction() const
{
  auto val = (mChunk >> 1) & 3;
  switch ( val ) {
  case 0: return VpiDir::Input;
  case 1: return VpiDir::Output;
  case 2: return VpiDir::Inout;
  case 3: return VpiDir::MixedIO;
  }
  // ダミー
  return VpiDir::NoDirection;
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
  return static_cast<bool>(mChunk & 1);
}

// @brief 明示的に名前がついているとき true を返す．
bool
EiPortBase::has_explicit_name() const
{
  return false;
}

// @brief 名前を返す．
std::string
EiPortBase::name() const
{
  return "";
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
  return mChunk >> 3;
}

// @brief 上位の接続先を返す(入力ポート用)．
const VlExpr*
EiPortBase::high_conn_expr() const
{
  throw std::logic_error{"EiPortBase::high_conn_expr()"};
}

// @brief 上位の接続先を返す(出力ポート用)．
const VlTerm*
EiPortBase::high_conn_term() const
{
  throw std::logic_error{"EiPortBase::high_conn_term()"};
}

// @brief 下位の接続先を返す．
const VlTerm*
EiPortBase::low_conn() const
{
  return mLowConn;
}

// @brief high_conn を接続する．
void
EiPortBase::set_high_conn(
  ElbExpr* high_conn,
  bool conn_by_name
)
{
  throw std::logic_error{"EiPortBase::set_high_conn()"};
}

// @brief high_conn を接続する．
void
EiPortBase::set_high_conn(
  const VlTerm* high_conn,
  bool conn_by_name
)
{
  throw std::logic_error{"EiPortBase::set_high_conn()"};
}


//////////////////////////////////////////////////////////////////////
// クラス EiInputPort
//////////////////////////////////////////////////////////////////////

// @brief 上位の接続先を返す(入力ポート用)．
const VlExpr*
EiInputPort::high_conn_expr() const
{
  return mHighConn;
}

// @brief high_conn を接続する．
void
EiInputPort::set_high_conn(
  ElbExpr* high_conn,
  bool conn_by_name
)
{
  mHighConn = high_conn;
  set_conn_by_name(conn_by_name);
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedInputPort
//////////////////////////////////////////////////////////////////////

// @brief 明示的に名前がついているとき true を返す．
bool
EiNamedInputPort::has_explicit_name() const
{
  return true;
}

// @brief 名前を返す．
std::string
EiNamedInputPort::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス EiOutput
//////////////////////////////////////////////////////////////////////

// @brief 上位の接続先を返す．
const VlTerm*
EiOutputPort::high_conn_term() const
{
  return mHighConn;
}

// @brief high_conn を接続する．
void
EiOutputPort::set_high_conn(
  const VlTerm* high_conn,
  bool conn_by_name
)
{
  mHighConn = high_conn;
  set_conn_by_name(conn_by_name);
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedOutputPort
//////////////////////////////////////////////////////////////////////

// @brief 明示的に名前がついているとき true を返す．
bool
EiNamedOutputPort::has_explicit_name() const
{
  return true;
}

// @brief 名前を返す．
std::string
EiNamedOutputPort::name() const
{
  return mName;
}

END_NAMESPACE_YM_VERILOG
