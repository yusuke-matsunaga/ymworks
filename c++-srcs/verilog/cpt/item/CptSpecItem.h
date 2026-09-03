#ifndef CPTSPECITEM_H
#define CPTSPECITEM_H

/// @file CptSpecItem.h
/// @brief CptSpecItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtPathDecl.h"
#include "parser/PtPathDelay.h"
#include "parser/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief specify_block item を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSpecItem :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptSpecItem(
    const FileRegion& file_region,
    VpiSpecItemType id,
    const PtExpr* terminal_top
  ) : mFileRegion{file_region},
      mId{id},
      mTerminalTop{terminal_top}
  {
  }

  /// @brief デストラクタ
  ~CptSpecItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

  /// @brief specify block item の種類を返す．
  VpiSpecItemType
  specitem_type() const override;

  /// @brief ターミナルリストの先頭の取得
  const PtExpr*
  terminal_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // トークン番号
  VpiSpecItemType mId;

  // ターミナルのリストの先頭
  const PtExpr* mTerminalTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief path 仕様 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSpecPath :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptSpecPath(
    const FileRegion& file_region,
    VpiSpecPathType id,
    const PtExpr* expr,
    const PtPathDecl* path_decl
  ) : mFileRegion{file_region},
      mId{id},
      mExpr{expr},
      mPathDecl{path_decl}
  {
  }

  /// @brief デストラクタ
  ~CptSpecPath() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

  /// @brief specify block path の種類を返す．
  VpiSpecPathType
  specpath_type() const override;

  /// @brief モジュールパスの式を返す．
  const PtExpr*
  cond_expr() const override;

  /// @brief パス記述を返す．
  const PtPathDecl*
  path_decl() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // トークン番号
  VpiSpecPathType mId;

  // モジュールパスの式
  const PtExpr* mExpr;

  // パス記述
  const PtPathDecl* mPathDecl;

};


//////////////////////////////////////////////////////////////////////
/// @brief path_delay_declaration を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPathDecl :
  public PtPathDecl
{
public:

  /// @brief コンストラクタ
  CptPathDecl(
    const FileRegion& file_region,
    int edge,
    const PtExpr* input_top,
    int input_pol,
    VpiPathType op,
    const PtExpr* output_top,
    int output_pol,
    const PtExpr* expr,
    const PtPathDelay* path_delay
  ) : mFileRegion{file_region},
      mEdge{edge},
      mInputTop{input_top},
      mInputPol{input_pol},
      mOp{op},
      mOutputTop{output_top},
      mOutputPol{output_pol},
      mExpr{expr},
      mPathDelay{path_delay}
  {
  }

  /// @brief デストラクタ
  ~CptPathDecl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief edge_descriptor を取り出す．
  int
  edge() const override;

  /// @brief 入力のリストの先頭の取得
  const PtExpr*
  input_top() const override;

  /// @brief 入力の極性を取り出す．
  int
  input_pol() const override;

  /// @brief パス記述子(?)を得る．vpiParallel か vpiFull
  VpiPathType
  op() const override;

  /// @brief 出力リストの先頭の取得
  const PtExpr*
  output_top() const override;

  /// @brief 出力の極性を取り出す．
  int
  output_pol() const override;

  /// @brief 式を取り出す．
  const PtExpr*
  expr() const override;

  /// @brief path_delay_value を取り出す．
  const PtPathDelay*
  path_delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  int mEdge;
  const PtExpr* mInputTop;
  int mInputPol;
  VpiPathType mOp;
  const PtExpr* mOutputTop;
  int mOutputPol;
  const PtExpr* mExpr;
  const PtPathDelay* mPathDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief path_delay_value を表す基底クラス
//////////////////////////////////////////////////////////////////////
class CptPathDelayBase :
  public PtPathDelay
{
public:

  /// @brief コンストラクタ
  CptPathDelayBase(
    const FileRegion& file_region
  ) : mFileRegion{file_region}
  {
  }

  /// @brief デストラクタ
  ~CptPathDelayBase() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 値を取り出す．
  const PtExpr*
  value(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
/// @brief 値を１つ持つ CptPathDelay
//////////////////////////////////////////////////////////////////////
class CptPathDelay1 :
  public CptPathDelayBase
{
public:

  /// @brief コンストラクタ
  CptPathDelay1(
    const FileRegion& file_region,
    const PtExpr* value1
  ) : CptPathDelayBase(file_region),
      mValue{value1}
  {
  }

  /// @brief デストラクタ
  ~CptPathDelay1() { }


public:
  //////////////////////////////////////////////////////////////////////
  // AtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  const PtExpr*
  value(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ディレイ値
  const PtExpr* mValue;

};


//////////////////////////////////////////////////////////////////////
/// @brief 値を2つ持つ CptPathDelay
//////////////////////////////////////////////////////////////////////
class CptPathDelay2 :
  public CptPathDelayBase
{
public:

  /// @brief コンストラクタ
  CptPathDelay2(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2
  ) : CptPathDelayBase(file_region),
      mValues{value1, value2}
  {
  }

  /// @brief デストラクタ
  ~CptPathDelay2() { }


public:
  //////////////////////////////////////////////////////////////////////
  // AtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  const PtExpr*
  value(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ディレイ値
  const PtExpr* mValues[2];

};


//////////////////////////////////////////////////////////////////////
/// @brief 値を3つ持つ CptPathDelay
//////////////////////////////////////////////////////////////////////
class CptPathDelay3 :
  public CptPathDelayBase
{
public:

  /// @brief コンストラクタ
  CptPathDelay3(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3
  ) : CptPathDelayBase(file_region),
      mValues{value1, value2, value3}
  {
  }

  /// @brief デストラクタ
  ~CptPathDelay3() { }


public:
  //////////////////////////////////////////////////////////////////////
  // AtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  const PtExpr*
  value(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ディレイ値
  const PtExpr* mValues[3];

};


//////////////////////////////////////////////////////////////////////
/// @brief 値を6つ持つ CptPathDelay
//////////////////////////////////////////////////////////////////////
class CptPathDelay6 :
  public CptPathDelayBase
{
public:

  /// @brief コンストラクタ
  CptPathDelay6(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3,
    const PtExpr* value4,
    const PtExpr* value5,
    const PtExpr* value6
  ) : CptPathDelayBase(file_region),
      mValues{value1, value2, value3,
	      value4, value5, value6}
  {
  }

  /// @brief デストラクタ
  ~CptPathDelay6() { }


public:
  //////////////////////////////////////////////////////////////////////
  // AtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  const PtExpr*
  value(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ディレイ値
  const PtExpr* mValues[6];

};


//////////////////////////////////////////////////////////////////////
/// @brief 値を12個持つ CptPathDelay
//////////////////////////////////////////////////////////////////////
class CptPathDelay12 :
  public CptPathDelayBase
{
public:

  /// @brief コンストラクタ
  CptPathDelay12(
    const FileRegion& file_region,
    const PtExpr* value1,
    const PtExpr* value2,
    const PtExpr* value3,
    const PtExpr* value4,
    const PtExpr* value5,
    const PtExpr* value6,
    const PtExpr* value7,
    const PtExpr* value8,
    const PtExpr* value9,
    const PtExpr* value10,
    const PtExpr* value11,
    const PtExpr* value12
  ) : CptPathDelayBase(file_region),
      mValues{value1, value2, value3,
	      value4, value5, value6,
	      value7, value8, value9,
	      value10, value11, value12}
  {
  }

  /// @brief デストラクタ
  ~CptPathDelay12() { }


public:
  //////////////////////////////////////////////////////////////////////
  // AtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  const PtExpr*
  value(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ディレイ値
  const PtExpr* mValues[12];

};

END_NAMESPACE_YM_VERILOG

#endif // CPTSPECITEM_H
