#ifndef COEFSHANDLER_H
#define COEFSHANDLER_H

/// @file CoefsHandler.h
/// @brief CoefsHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib/HeaderHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class CoefsHandler CoefsHandler.h "CoefsHandler.h"
/// @brief look-up table 中の 'coefs' Complex Attribute 用のハンドラ
///
/// 内容はひとつの文字列だが中はコンマで区切られた float のリストとなっている．
//////////////////////////////////////////////////////////////////////
class CoefsHandler :
  public HeaderHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  CoefsHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~CoefsHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 'coefs' Complex Attribute の記述をパースする．
  /// @param[in] dst 読み込んだ値を格納する変数
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(const AstFloatVector*& dst);


protected:
  //////////////////////////////////////////////////////////////////////
  // HeaderHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダの開始処理
  ///
  /// '(' を読み込んだ時に呼ばれる．
  void
  begin_header() override;

  /// @brief 値を読み込む処理
  /// @param[in] value_type 型
  /// @param[in] value_loc トークンの位置
  /// @param[in] count read_value() の呼ばれた回数
  bool
  read_header_value(TokenType value_type,
		    const FileRegion& value_loc,
		    int count) override;

  /// @brief 読み込みが終了した時の処理を行う．
  /// @param[in] header_loc '(' から ')' までのファイル上の位置
  /// @param[in] count 読み込んだ要素数
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  end_header(const FileRegion& header_loc,
	     int count) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値のリスト
  vector<double> mValueList;

  // 読み込んだ値
  const AstFloatVector* mValue;

};

END_NAMESPACE_YM_DOTLIB

#endif // COEFSHANDLER_H
