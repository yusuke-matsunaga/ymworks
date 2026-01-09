#ifndef ISCAS89SCANNER_H
#define ISCAS89SCANNER_H

/// @file Iscas89Scanner.h
/// @brief Iscas89Scanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/Scanner.h"
#include "Iscas89Token.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class Iscas89Scanner Iscas89Scanner.h "Iscas89Scanner.h"
/// @brief iscas89 用の字句解析器
//////////////////////////////////////////////////////////////////////
class Iscas89Scanner :
  public Scanner
{
public:

  /// @brief コンストラクタ
  Iscas89Scanner(
    std::istream& s,          ///< [in] 入力ストリーム
    const FileInfo& file_info ///< [in] ファイル情報
  );

  /// @brief デストラクタ
  ~Iscas89Scanner() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンを一つ読み出す．
  Iscas89Token
  read_token();

  /// @brief 最後の read_token() で読み出した字句の文字列を返す．
  std::string
  cur_string() { return mCurString; }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token() の下請け関数
  /// @return トークンを返す．
  Iscas89Token::Type
  scan();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ型
  //////////////////////////////////////////////////////////////////////

  // 予約語の情報
  struct RsvInfo
  {
    Iscas89Token::Type type; // トークンの種類
    PrimType gate_type;      // ゲートの種類
    SizeType ex_id;          // 拡張ID
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列バッファ
  std::string mCurString;

  // 文字列の先頭の位置
  FileLoc mFirstLoc;

  // 予約語の辞書
  std::unordered_map<std::string, RsvInfo> mRsvDict;

};

END_NAMESPACE_YM_BN

#endif // ISCAS89SCANNER_H
