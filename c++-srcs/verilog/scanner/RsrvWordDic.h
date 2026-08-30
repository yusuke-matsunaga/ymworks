#ifndef RSRVWORDDIC_H
#define RSRVWORDDIC_H

/// @file RsrvWordDic.h
/// @brief RsrvWordDic のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class RsrvWordDic RsrvWordDic.h "RsrvWordDic.h"
/// @ingroup VlParser
/// @brief Verilog-HDL の予約語テーブル
//////////////////////////////////////////////////////////////////////
class RsrvWordDic
{
public:

  /// @brief 唯一のインスタンスを取ってくる関数
  /// @return 唯一のインスタンスを返す．
  static
  const RsrvWordDic&
  the_dic();

  /// @brief str が予約語ならそのトークン番号を返す．
  /// @return str が予約語ならそのトークン番号を返す．
  /// そうでないときは
  /// - 先頭が '$' なら SYS_IDENTIFIER を返す．
  /// - 先頭が '`' なら CD_DEFINED_SYMBOL を返す．
  /// - 上記以外ならば IDENTIFIER を返す．
  int
  token(
    const std::string& str ///< [in] 文字列
  ) const;

  /// @brief トークンから文字列を取り出す．
  /// @return token に対応した文字列を返す．\n
  /// トークンが正しくなければ nullptr を返す．
  const std::string&
  str(
    int token ///< [in] トークン番号
  ) const;


private:

  /// @brief コンストラクタ
  RsrvWordDic();

  /// @brief デストラクタ
  ~RsrvWordDic();


private:

  // 値を覚えておくためのセル
  struct Cell
  {
    // 文字列
    std::string mStr;

    // トークン
    int mTok;

    // 文字列をキーとしたハッシュ用のリンク
    Cell* mLink1;

    // トークンをキーとしたハッシュ用のリンク
    Cell* mLink2;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の本体の配列
  Cell* mCellArray;

  // 文字列をキーとしてトークンを持つハッシュ表
  Cell** mTable1;

  // トークンをキーとして文字列を持つハッシュ表
  Cell** mTable2;

  // テーブルサイズ
  std::uint32_t mSize;

};

END_NAMESPACE_YM_VERILOG

#endif // RSRVWORDDIC_H
