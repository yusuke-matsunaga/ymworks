#ifndef CPTUDP_H
#define CPTUDP_H

/// @file CptUdp.h
/// @brief CptUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtUdp.h"
#include "parser/PtPort.h"
#include "parser/PtDecl.h"
#include "ym/vl/VlUdpVal.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief UDP を表すノード
//////////////////////////////////////////////////////////////////////
class CptUdp :
  public PtUdp
{
public:

  /// @brief コンストラクタ
  CptUdp(
    const FileRegion& file_region,
    const char* name,
    PtPort* port_top,
    PtIOHead* iohead_top,
    bool is_seq,
    const AstExpr* init_value,
    PtUdpEntry* entry_top
  ) : mFileRegion{file_region},
      mName{name},
      mPortTop{port_top},
      mIOHeadTop{iohead_top},
      mSeq{is_seq},
      mInitValue{init_value},
      mTableTop{entry_top}
  {
  }

  /// @brief デストラクタ
  ~CptUdp() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdp の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief primitivie type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief ポートのリストを取り出す．
  AstPortList
  port_list() const override;

  /// @brief 入出力宣言ヘッダのリストの取得
  AstIOHeadList
  iohead_list() const override;

  /// @brief 初期値を取出す．
  const AstExpr*
  init_value() const override;

  /// @brief テーブルのリストを返す．
  AstUdpEntryList
  table_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // プリミティブ名
  const char* mName;

  // ポートの先頭
  PtPort* mPortTop;

  // 入出力宣言の先頭
  PtIOHead* mIOHeadTop;

  // sequential primitive の時 true
  bool mSeq;

  // 初期値
  const AstExpr* mInitValue;

  // テーブル要素の先頭
  PtUdpEntry* mTableTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP の中身のテーブルを表すクラス(組合せ回路用)
//////////////////////////////////////////////////////////////////////
class CptUdpEntry :
  public PtUdpEntry
{
public:

  /// @brief コンストラクタ
  CptUdpEntry(
    const FileRegion& file_region,
    PtUdpValue* input_top,
    const AstUdpValue* output
  ) : mFileRegion{file_region},
      mInputTop{input_top},
      mOutput{output}
  {
  }

  /// @brief デストラクタ
  ~CptUdpEntry() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 入力値のリストを取り出す．
  AstUdpValueList
  input_list() const override;

  /// @brief 現状態の値を取り出す．
  const AstUdpValue*
  current() const override;

  /// @brief 出力の値を取り出す．
  const AstUdpValue*
  output() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 入力パタンの先頭
  PtUdpValue* mInputTop;

  // 出力のパタン
  const AstUdpValue* mOutput;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP の中身のテーブルを表すクラス(順序回路用)
//////////////////////////////////////////////////////////////////////
class CptUdpEntryS :
  public CptUdpEntry
{
public:

  /// @brief コンストラクタ
  CptUdpEntryS(
    const FileRegion& file_region,
    PtUdpValue* input_top,
    const AstUdpValue* current,
    const AstUdpValue* output
  ) : CptUdpEntry(file_region, input_top, output),
      mCurrent{current}
  {
  }

  /// @brief デストラクタ
  ~CptUdpEntryS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstUdpEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現状態の値を取り出す．
  const AstUdpValue*
  current() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現状態のパタン
  const AstUdpValue* mCurrent;

};


//////////////////////////////////////////////////////////////////////
/// @brief UDP のテーブルの値を表すクラス
//////////////////////////////////////////////////////////////////////
class CptUdpValue :
  public PtUdpValue
{
public:

  /// @brief コンストラクタ
  CptUdpValue(
    const FileRegion& file_region,
    char symbol
  ) : mFileRegion{file_region},
      mSymbol{symbol}
  {
  }

  /// @brief コンストラクタ
  CptUdpValue(
    const FileRegion& file_region,
    char symbol1,
    char symbol2
  ) : mFileRegion{file_region},
      mSymbol{symbol1, symbol2}
  {
  }

  /// @brief デストラクタ
  ~CptUdpValue() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstUdpValue の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief シンボルを取り出す．
  VlUdpVal
  symbol() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // シンボル
  VlUdpVal mSymbol;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTUDP_H
