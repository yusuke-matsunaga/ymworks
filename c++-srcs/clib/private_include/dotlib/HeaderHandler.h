#ifndef HEADERHANDLER_H
#define HEADERHANDLER_H

/// @file HeaderHandler.h
/// @brief HeaderHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class HeaderHandler HeaderHandler.h "dotlib/HeaderHandler.h"
/// @brief complex attribute/group statement のヘッダ用のハンドラ
///
/// ヘッダ部分が
/// '(' <value>, <value>, <value>, ... ')'
/// の形を仮定してパーズを行う．
/// '(' を読んだ直後に begin_header() を呼び出し，
/// 個々の <value> を読む度に read_value() を呼び出す．
/// 最後の ')' を読んだ直後に end_header() を呼び出す．
/// 継承クラスで begin_header(), read_value(), end_header()
/// を実装する必要がある．
/// ここではヘッダと呼んでいるが complex attribute の場合はそれが
/// 本体となる．
///
/// このクラスは実体を持たない純粋仮想基底クラスである．
/// 継承クラスは
/// * 要素数が固定
/// * 要素数が可変
/// の２種類に分類される．
/// 前者は FixedElemHeader で実装する．
/// 後者は ListHeader で実装する．
/// それぞれ要素用のハンドラ関数を指定する．
//////////////////////////////////////////////////////////////////////
class HeaderHandler
{
public:

  /// @brief コンストラクタ
  HeaderHandler() = default;

  /// @brief デストラクタ
  ~HeaderHandler() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダの開始処理
  ///
  /// '(' を読み込んだ時に呼ばれる．
  void
  begin_header(
    const FileRegion& loc ///< [in] '(' のファイル上の位置
  );

  /// @brief ヘッダの値を読み込む処理
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  read_header_value(
    DotlibScanner& scanner, ///< [in] 字句解析器
    const FileRegion& loc,  ///< [in] 現在のファイル上の位置
    int count               ///< [in] read_header_value() の呼ばれた回数
  );

  /// @brief 読み込みが終了した時の処理を行う．
  /// @retrun 結果の AstValue を返す．
  ///
  /// - ')' を読み込んだ直後に呼ばれる．
  /// - エラーの時は nullptr を返す．
  AstValuePtr
  end_header(
    const FileRegion& loc, ///< [in] ')' のファイル上の位置
    int count              ///< [in] 読み込んだ要素数
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief '(' の位置を返す．
  const FileRegion&
  lp_loc() const { return mLpLoc; }

  /// @brief ')' の位置を返す．
  const FileRegion&
  rp_loc() const { return mRpLoc; }


private:
  //////////////////////////////////////////////////////////////////////
  // HeaderHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダの開始処理
  ///
  /// '(' を読み込んだ時に呼ばれる．
  virtual
  void
  _begin_header();

  /// @brief ヘッダの値を読み込む処理
  /// @return 読み込んだ値を返す．
  virtual
  AstValuePtr
  _read_header_value(
    DotlibScanner& scanner, ///< [in] 字句解析器
    int count               ///< [in] read_header_value() の呼ばれた回数
  ) = 0;

  /// @brief 読み込みが終了した時の処理を行う．
  ///
  /// - ')' を読み込んだ直後に呼ばれる．
  virtual
  void
  _end_header(
    int count ///< [in] 読み込んだ要素数
  ) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // '(' の位置
  FileRegion mLpLoc;

  // ')' の位置
  FileRegion mRpLoc;

  // 読み込んだ値
  std::vector<AstValuePtr> mValueList;

};


//////////////////////////////////////////////////////////////////////
/// @class FixedElemHeader HeaderHandler.h "dotlib/HeaderHadler.h"
/// @brief 固定形式のヘッダ用ハンドラ
//////////////////////////////////////////////////////////////////////
class FixedElemHeader :
  public HeaderHandler
{
public:

  /// @brief コンストラクタ
  FixedElemHeader(
    std::initializer_list<SimpleHandler> handler_list ///< [in] 各要素のハンドラのリスト
  );

  /// @brief デストラクタ
  ~FixedElemHeader() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // HeaderHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダの値を読み込む処理
  AstValuePtr
  _read_header_value(
    DotlibScanner& scanner, ///< [in] 字句解析器
    int count               ///< [in] read_header_value() の呼ばれた回数
  ) override;

  /// @brief 読み込みが終了した時の処理を行う．
  void
  _end_header(
    int count ///< [in] 読み込んだ要素数
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各要素のハンドラのリスト
  std::vector<SimpleHandler> mHandlerList;

};


//////////////////////////////////////////////////////////////////////
/// @class FanoutLength HeaderHandler.h "dotlib/HeaderHadler.h"
/// @brief 'fanout_length' 用ハンドラ
//////////////////////////////////////////////////////////////////////
class FanoutLengthHeader :
  public HeaderHandler
{
public:

  /// @brief コンストラクタ
  FanoutLengthHeader();

  /// @brief デストラクタ
  ~FanoutLengthHeader() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // HeaderHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダの値を読み込む処理
  AstValuePtr
  _read_header_value(
    DotlibScanner& scanner, ///< [in] 字句解析器
    int count               ///< [in] read_header_value() の呼ばれた回数
  ) override;

  /// @brief 読み込みが終了した時の処理を行う．
  void
  _end_header(
    int count ///< [in] 読み込んだ要素数
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


//////////////////////////////////////////////////////////////////////
/// @class ListHeader HeaderHandler.h "dotlib/HeaderHadler.h"
/// @brief 可変要素のリストの形式のヘッダ用ハンドラ
//////////////////////////////////////////////////////////////////////
class ListHeader :
  public HeaderHandler
{
public:

  /// @brief コンストラクタ
  ListHeader(
    SimpleHandler handler ///< [in] 要素のハンドラ
  );

  /// @brief デストラクタ
  ~ListHeader() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // HeaderHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む処理
  AstValuePtr
  _read_header_value(
    DotlibScanner& scanner, ///< [in] 字句解析器
    int count               ///< [in] read_value() の呼ばれた回数
  ) override;

  /// @brief 読み込みが終了した時の処理を行う．
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  void
  _end_header(
    int count ///< [in] 読み込んだ要素数
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のハンドラ
  SimpleHandler mHandler;

};


//////////////////////////////////////////////////////////////////////
/// @class OptElemHeader HeaderHandler.h "dotlib/HeaderHadler.h"
/// @brief オプション形式のヘッダ用ハンドラ
//////////////////////////////////////////////////////////////////////
class OptElemHeader :
  public HeaderHandler
{
public:

  /// @brief コンストラクタ
  OptElemHeader(
    SimpleHandler handler ///< [in] 要素のハンドラ
  );

  /// @brief デストラクタ
  ~OptElemHeader() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // HeaderHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダの値を読み込む処理
  AstValuePtr
  _read_header_value(
    DotlibScanner& scanner, ///< [in] 字句解析器
    int count               ///< [in] read_header_value() の呼ばれた回数
  ) override;

  /// @brief 読み込みが終了した時の処理を行う．
  void
  _end_header(
    int count ///< [in] 読み込んだ要素数
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のハンドラ
  SimpleHandler mHandler;

};

END_NAMESPACE_YM_DOTLIB

#endif // HEADERHANDLER_H
