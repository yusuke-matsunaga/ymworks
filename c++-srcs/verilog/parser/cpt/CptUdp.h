#ifndef CPTUDP_H
#define CPTUDP_H

/// @file CptUdp.h
/// @brief CptUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtUdp.h"
#include "ym/vl/VlUdpVal.h"
#include "ym/FileRegion.h"
#include "parser/PtArray.h"


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
    PtPortArray&& port_list,
    PtIOHeadArray&& iohead_list,
    bool is_seq,
    const AstExpr* init_value,
    PtUdpEntryArray&& entry_list
  ) : mFileRegion{file_region},
      mName{name},
      mPortList{std::move(port_list)},
      mIOHeadList{std::move(iohead_list)},
      mSeq{is_seq},
      mInitValue{init_value},
      mTableList{std::move(entry_list)}
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

  /// @brief ポート数を取り出す．
  SizeType
  port_num() const override;

  /// @brief ポートを取り出す．
  const AstPort*
  port(
    SizeType index ///< [in] インデックス ( 0 <= index < port_num() )
  ) const override;

  /// @brief ポートのリストを取り出す．
  AstPortVec
  port_list() const override;

  /// @brief 入出力宣言ヘッダ配列の要素数の取得
  SizeType
  iohead_num() const override;

  /// @brief 入出力宣言ヘッダの取得
  const AstIOHead*
  iohead(
    SizeType index ///< [in] インデックス ( 0 <= index < iohead_num() )
  ) const override;

  /// @brief 入出力宣言ヘッダのリストの取得
  AstIOHeadVec
  iohead_list() const override;

  /// @brief 初期値を取出す．
  const AstExpr*
  init_value() const override;

  /// @brief テーブルの要素数を取り出す．
  SizeType
  table_num() const override;

  /// @brief テーブルを返す．
  const AstUdpEntry*
  table(
    SizeType index ///< [in] インデックス ( 0 <= index < table_num() )
  ) const override;

  /// @brief テーブルのリストを返す．
  AstUdpEntryVec
  table_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // プリミティブ名
  const char* mName;

  // ポートのリスト
  PtPortArray mPortList;

  // 入出力宣言のリスト
  PtIOHeadArray mIOHeadList;

  // sequential primitive の時 true
  bool mSeq;

  // 初期値
  const AstExpr* mInitValue;

  // テーブル要素のリスト
  PtUdpEntryArray mTableList;

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
    PtUdpValueArray&& input_list,
    const AstUdpValue* output
  ) : mFileRegion{file_region},
      mInputList{std::move(input_list)},
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

  /// @brief 入力値の配列の要素数を取り出す．
  SizeType
  input_num() const override;

  /// @brief 入力値を取り出す．
  const AstUdpValue*
  input(
    SizeType index ///< [in] インデックス ( 0 <= index < input_num() )
  ) const override;

  /// @brief 入力値のリストを取り出す．
  AstUdpValueVec
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

  // 入力パタンのリスト
  PtUdpValueArray mInputList;

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
    PtUdpValueArray&& input_list,
    const AstUdpValue* current,
    const AstUdpValue* output
  ) : CptUdpEntry(file_region,
		  std::move(input_list),
		  output),
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
