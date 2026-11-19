
/// @file EiPort.cc
/// @brief EiPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPort.h"
#include "EiNamedPort.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiPort::EiPort(
  const FileRegion& loc,
  const VlModule* module,
  SizeType index,
  const VlExpr* low_conn,
  VpiDir dir
) : mFileRegion{loc},
    mModule{module},
    mHighConn{nullptr},
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
EiPort::type() const
{
  return VpiObjType::Port;
}

// @brief ファイル位置の取得
FileRegion
EiPort::file_region() const
{
  return mFileRegion;
}

// @brief 入出力の区別を得る．
VpiDir
EiPort::direction() const
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
EiPort::bit_size() const
{
  return mLowConn->bit_size();
}

// @brief 名前による接続を持つとき true を返す．
bool
EiPort::is_conn_by_name() const
{
  return static_cast<bool>(mChunk & 1);
}

// @brief 明示的に名前がついているとき true を返す．
bool
EiPort::has_explicit_name() const
{
  return false;
}

// @brief 名前を返す．
std::string
EiPort::name() const
{
  return "";
}

// @brief 親のモジュールを取出す
const VlModule*
EiPort::module() const
{
  return mModule;
}

// @brief ポートリストの何番目のポートかを表すインデックスを返す．
SizeType
EiPort::port_index() const
{
  return mChunk >> 3;
}

// @brief 上位の接続先を返す(入力ポート用)．
const VlExpr*
EiPort::high_conn() const
{
  return mHighConn;
}

// @brief 下位の接続先を返す．
const VlExpr*
EiPort::low_conn() const
{
  return mLowConn;
}

// @brief high_conn を接続する．
void
EiPort::set_high_conn(
  const VlExpr* high_conn,
  bool conn_by_name
)
{
  mHighConn = high_conn;
  if ( conn_by_name ) {
    mChunk |= 1ULL;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedPort
//////////////////////////////////////////////////////////////////////

// @brief 明示的に名前がついているとき true を返す．
bool
EiNamedPort::has_explicit_name() const
{
  return true;
}

// @brief 名前を返す．
std::string
EiNamedPort::name() const
{
  return mName;
}

END_NAMESPACE_YM_VERILOG
