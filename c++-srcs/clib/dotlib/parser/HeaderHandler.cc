
/// @file HeaderHandler.cc
/// @brief HeaderHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/HeaderHandler.h"
#include "dotlib/DotlibScanner.h"
#include "dotlib/AstValue.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス HeaderHandler
//////////////////////////////////////////////////////////////////////

// @brief ヘッダの開始処理
// @param[in] loc '(' のファイル上の位置
//
// '(' を読み込んだ時に呼ばれる．
void
HeaderHandler::begin_header(
  const FileRegion& loc
)
{
  mLpLoc = loc;
  mValueList.clear();
  _begin_header();
}

// @brief ヘッダの値を読み込む処理
bool
HeaderHandler::read_header_value(
  DotlibScanner& scanner,
  const FileRegion& loc,
  int count
)
{
  mRpLoc = loc;
  auto value{_read_header_value(scanner, count)};
  if ( value != nullptr ) {
    mValueList.push_back(std::move(value));
    return true;
  }
  else {
    return false;
  }
}

// @brief 読み込みが終了した時の処理を行う．
AstValuePtr
HeaderHandler::end_header(
  const FileRegion& loc,
  int count
)
{
  mRpLoc = loc;
  _end_header(count);
  FileRegion loc1{mLpLoc, mRpLoc};
  return AstValue::new_complex(mValueList, loc1);
}

// @brief ヘッダの開始処理
//
// '(' を読み込んだ時に呼ばれる．
void
HeaderHandler::_begin_header()
{
  // デフォルトではなにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス FixedElemHeader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FixedElemHeader::FixedElemHeader(
  std::initializer_list<SimpleHandler> handler_list
) : mHandlerList{handler_list}
{
}

// @brief 値を読み込む処理
AstValuePtr
FixedElemHeader::_read_header_value(
  DotlibScanner& scanner,
  int count
)
{
  ASSERT_COND( count >= 0 );
  if ( count < mHandlerList.size() ) {
    auto handler{mHandlerList[count]};
    return handler(scanner);
  }
  else {
    std::ostringstream buf;
    buf << "Syntax error: "
	<< "Too many values, expected " << mHandlerList.size() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(lp_loc(), rp_loc()),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    buf.str());
    throw std::invalid_argument{"Syntax error"};
  }
}

// @brief 読み込みが終了した時の処理を行う．
void
FixedElemHeader::_end_header(
  int count
)
{
  ASSERT_COND( count <= mHandlerList.size() );
  if ( count < mHandlerList.size() ) {
    std::ostringstream buf;
    buf << "Syntax error: "
	<< "Too few values, expected " << mHandlerList.size() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(lp_loc(), rp_loc()),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    buf.str());
    throw std::invalid_argument{"Syntax error"};
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FanoutLengthHeader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FanoutLengthHeader::FanoutLengthHeader()
{
}

// @brief ヘッダの値を読み込む処理
AstValuePtr
FanoutLengthHeader::_read_header_value(
  DotlibScanner& scanner,
  int count
)
{
  switch ( count ) {
  case 0: return scanner.read_int();
  case 1: return scanner.read_float();
  case 2: return scanner.read_float();
  case 3: return scanner.read_float();
  case 4: return scanner.read_float();
  default: break;
  }
  std::ostringstream buf;
  buf << "Syntx error: "
      << "Too many values, expected " << 5 << ".";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  FileRegion(lp_loc(), rp_loc()),
		  MsgType::Error,
		  "DOTLIB_PARSER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
}

// @brief 読み込みが終了した時の処理を行う．
void
FanoutLengthHeader::_end_header(
  int count
)
{
}


//////////////////////////////////////////////////////////////////////
// クラス ListHeader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ListHeader::ListHeader(
  SimpleHandler handler
) : mHandler{handler}
{
}

// @brief 値を読み込む処理
AstValuePtr
ListHeader::_read_header_value(
  DotlibScanner& scanner,
  int count
)
{
  // count は無視
  return mHandler(scanner);
}

// @brief 読み込みが終了した時の処理を行う．
void
ListHeader::_end_header(
  int count
)
{
  // count は無視
}


//////////////////////////////////////////////////////////////////////
// クラス OptElemHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
OptElemHeader::OptElemHeader(
  SimpleHandler handler ///< [in] 要素のハンドラ
) : mHandler{handler}
{
}

// @brief ヘッダの値を読み込む処理
AstValuePtr
OptElemHeader::_read_header_value(
  DotlibScanner& scanner, ///< [in] 字句解析器
  int count               ///< [in] read_header_value() の呼ばれた回数
)
{
  if ( count == 0 ) {
    return mHandler(scanner);
  }

  std::ostringstream buf;
  buf << "Syntx error: "
      << "Too many values, expected at most one.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  FileRegion(lp_loc(), rp_loc()),
		  MsgType::Error,
		  "DOTLIB_PARSER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
}

// @brief 読み込みが終了した時の処理を行う．
void
OptElemHeader::_end_header(
  int count ///< [in] 読み込んだ要素数
)
{
  // 0 でも可
}

END_NAMESPACE_YM_DOTLIB
