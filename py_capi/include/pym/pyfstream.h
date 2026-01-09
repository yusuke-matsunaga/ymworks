#ifndef PYFSTREAM_H
#define PYFSTREAM_H

/// @file pyfstream
/// @brief pyfstreambuf/ipyfstream/opyfstream のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym_config.h"
#include "pym/PyString.h"
#include "pym/PyFileObject.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class pyfstreambuf pyfstream "pyfstream"
/// @brief Python のファイルオブジェクトに対応した streambuf
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _TraitsT = std::char_traits<_CharT>>
class pyfstreambuf :
  public std::basic_streambuf<_CharT, _TraitsT>
{
public:
  using char_type = _CharT;
  using traits_type = _TraitsT;
  using int_type = typename traits_type::int_type;
  using pos_type = typename traits_type::pos_type;
  using off_type = typename traits_type::off_type;

  using streambuf_type = std::basic_streambuf<char_type, traits_type>;

public:

  /// @brief コンストラクタ
  ///
  /// obj はファイルオブジェクトである必要がある．
  explicit
   pyfstreambuf(
    PyObject* obj ///< [in] Python のオブジェクト
  ) : mObj{obj}
  {
    bool readable = false;
    bool writable = false;
    if ( !PyFileObject::Check(obj, readable, writable) ) {
      // obj がファイルオブジェクトではなかった．
      std::cout << "obj is not a FileObject" << std::endl;
      throw std::invalid_argument{"obj is not a FileObject"};
    }
    if ( readable ) {
      set_rdbuf();
    }
    if ( writable ) {
      set_wrbuf();
    }
  }

  /// @brief デストラクタ
  ~pyfstreambuf()
  {
    delete [] mRdBuf;
    delete [] mWrBuf;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // basic_streambuf<_CharT, _Traits> の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力モードでバッファが空になったときの処理
  int_type
  underflow() override
  {
    if ( this->gptr() == nullptr || this->gptr() == this->egptr() ) {
      // バッファが空
      auto byte_size = BUF_SIZE * sizeof(char_type);
      auto str_obj = PyFile_GetLine(mObj, byte_size);
      if ( str_obj == nullptr ) {
	throw std::runtime_error{"Error in PyFile_GetLine()"};
      }
      std::string str;
      if ( !PyString::FromPyObject(str_obj, str) ) {
	throw std::runtime_error{"PyFile_GetLine() returns non-string object"};
      }
      Py_DECREF(str_obj);
      auto n = str.size();
      for ( SizeType i = 0; i < n; ++ i ) {
	mRdBuf[i] = str[i];
      }
      this->setg(mRdBuf, mRdBuf, mRdBuf + n);
    }
    if ( this->gptr() < this->egptr() ) {
      return traits_type::to_int_type(*this->gptr());
    }
    else {
      return traits_type::eof();
    }
  }

  /// @brief 入力モードでデータを読み込む．
  std::streamsize
  xsgetn(
    char_type* s,     ///< [in] データ領域のポインタ
    std::streamsize n ///< [in] 読み込むデータの最大値
  ) override
  {
    char_type* s1 = s;
    std::streamsize counts = 0;
    while ( n > 0 ) {
      if ( this->gptr() == this->egptr() ) {
	if ( underflow() == traits_type::eof() ) {
	  // 終端
	  break;
	}
      }
      auto cur = this->gptr();
      auto end = this->egptr();
      auto read_count = static_cast<std::streamsize>(end - cur);
      auto size1 = std::min(n, read_count);
      copy_data(cur, s1, size1);
      s1 += size1;
      n -= size1;
      counts += size1;
      this->gbump(static_cast<int>(size1));
    }
    return counts;
  }

  /// @brief 出力モードでバッファが一杯になったときの処理
  int_type
  overflow(
    int_type c = traits_type::eof() ///< [in] 溢れた文字
  ) override
  {
    if ( !traits_type::eq_int_type(c, traits_type::eof()) ) {
      auto c1 = traits_type::to_char_type(c);
      auto n = xsputn(&c1, 1);
      if ( n != 1 ) {
	// 失敗した．
	return traits_type::eof();
      }
    }
    return traits_type::not_eof(c);
  }

  /// @brief 出力モードでデータを書き込む．
  std::streamsize
  xsputn(
    const char_type* s, ///< [in] データ領域のポインタ
    std::streamsize n   ///< [in] データの文字数
  ) override
  {
    auto s1 = s;
    std::streamsize counts = 0;
    while ( n > 0 ) {
      if ( this->pptr() == this->epptr() ) {
	if ( this->pubsync() == -1 ) {
	  // 失敗
	  return 0;
	}
      }
      auto cur = this->pptr();
      auto end = this->epptr();
      auto capacity = static_cast<std::streamsize>(end - cur);
      auto size1 = std::min(n, capacity);
      copy_data(s1, cur, size1);
      s1 += size1;
      n -= size1;
      counts += size1;
      this->pbump(static_cast<int>(size1));
    }
    return counts;
  }

  /// @brief バッファを同期させる．
  int
  sync() override
  {
    if ( this->pptr() != this->pbase() ) {
      // 未同期のデータがある．
      // c文字列に見せかけるために末尾に end-of-string を書き込む．
      *this->pptr() = '\0';
      auto s = static_cast<const char*>(this->pbase());
      if ( PyFile_WriteString(s, mObj) == -1 ) {
	return -1;
      }
      // バッファを初期化する．
      this->setp(this->pbase(), this->epptr());
    }
    return 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込みバッファを用意する．
  void
  set_rdbuf()
  {
    mRdBuf = new char_type[BUF_SIZE];
    this->setg(nullptr, nullptr, nullptr);
  }

  /// @brief 書き込みバッファを用意する．
  void
  set_wrbuf()
  {
    mWrBuf = new char_type[BUF_SIZE];
    this->setp(mWrBuf, mWrBuf + BUF_SIZE - 1);
  }

  /// @brief データを転送する．
  void
  copy_data(
    const char_type* src,
    char_type* dst,
    std::streamsize size
  )
  {
    for ( SizeType i = 0; i < size; ++ i, ++ src, ++ dst ) {
      *dst = *src;
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のファイルオブジェクト
  PyObject* mObj;

  // バッファサイズ
  static
  const SizeType BUF_SIZE = 4096;

  // 読み込みバッファ
  char_type* mRdBuf{nullptr};

  // 書き込みバッファ
  char_type* mWrBuf{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class basic_iPYFSTREAM pyfstream "pyfstream"
/// @brief pyfstreambuf を使った basic_istream
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_ipyfstream :
  public std::basic_istream<_CharT, _Traits>
{
public:
  using char_type = _CharT;
  using traits_type = _Traits;

  using istream_type = std::basic_istream<char_type, traits_type>;

public:

  /// @brief コンストラクタ
  explicit
  basic_ipyfstream(
    PyObject* obj ///< [in] Python のファイルオブジェクト
  ) : istream_type(new pyfstreambuf<char_type, traits_type>(obj))
  {
  }

  /// @brief デストラクタ
  ~basic_ipyfstream()
  {
  }

};


//////////////////////////////////////////////////////////////////////
/// @class basic_opyfstream pyfstream "pyfstream"
/// @brief pyfstreambuf を使った baisc_ostream
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_opyfstream :
  public std::basic_ostream<_CharT, _Traits>
{
public:
  using char_type = _CharT;
  using traits_type = _Traits;

  using ostream_type = std::basic_ostream<char_type, traits_type>;

public:

  /// @brief コンストラクタ
  basic_opyfstream(
    PyObject* obj ///< [in] Python のファイルオブジェクト
  ) : ostream_type(new pyfstreambuf<char_type, traits_type>(obj))
  {
  }

  /// @brief デストラクタ
  ~basic_opyfstream()
  {
    this->flush();
  }

};

// お約束の定義
using ipyfstream = basic_ipyfstream<char>;
using opyfstream = basic_opyfstream<char>;

END_NAMESPACE_YM

#endif // pyfstream_H
