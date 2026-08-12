#ifndef CPTSPECITEM_H
#define CPTSPECITEM_H

/// @file CptSpecItem.h
/// @brief CptSpecItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtArray.h"


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
    PtExprArray&& terminal_array
  );

  /// @brief デストラクタ
  ~CptSpecItem();


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief specify block item の種類を返す．
  VpiSpecItemType
  specitem_type() const override;

  /// @brief ターミナルの要素数の取得
  SizeType
  terminal_num() const override;

  /// @brief ターミナルの取得
  const AstExpr*
  terminal(
    SizeType pos ///< [in] 位置 ( 0 <= pos < terminal_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // トークン番号
  VpiSpecItemType mId;

  // ターミナルの配列
  PtExprArray mTerminalArray;

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
    const AstExpr* expr,
    const AstPathDecl* path_decl
  );

  /// @brief デストラクタ
  ~CptSpecPath();


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief specify block path の種類を返す．
  VpiSpecPathType
  specpath_type() const override;

  /// @brief モジュールパスの式を返す．
  const AstExpr*
  expr() const override;

  /// @brief パス記述を返す．
  const AstPathDecl*
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
  const AstExpr* mExpr;

  // パス記述
  const AstPathDecl* mPathDecl;

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
    PtExprArray&& input_array,
    int input_pol,
    VpiPathType op,
    PtExprArray&& output_array,
    int output_pol,
    const AstExpr* expr,
    const AstPathDelay* path_delay
  );

  /// @brief デストラクタ
  ~CptPathDecl();


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

  /// @brief 入力のリストの要素数の取得
  SizeType
  input_num() const override;

  /// @brief 入力の取得
  const AstExpr*
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const override;

  /// @brief 入力の極性を取り出す．
  int
  input_pol() const override;

  /// @brief パス記述子(?)を得る．vpiParallel か vpiFull
  VpiPathType
  op() const override;

  /// @brief 出力のリストの要素数の取得
  SizeType
  output_num() const override;

  /// @brief 出力の取得
  const AstExpr*
  output(
    SizeType pos ///< [in] 位置 ( 0 <= pos < output_num() )
  ) const override;

  /// @brief 出力の極性を取り出す．
  int
  output_pol() const override;

  /// @brief 式を取り出す．
  const AstExpr*
  expr() const override;

  /// @brief path_delay_value を取り出す．
  const AstPathDelay*
  path_delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  int mEdge;
  PtExprArray mInputArray;
  int mInputPol;
  VpiPathType mOp;
  PtExprArray mOutputArray;
  int mOutputPol;
  const AstExpr* mExpr;
  const AstPathDelay* mPathDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPathDelay :
  public PtPathDelay
{
public:

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const AstExpr* value1
  );

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const AstExpr* value1,
    const AstExpr* value2
  );

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const AstExpr* value1,
    const AstExpr* value2,
    const AstExpr* value3
  );

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const AstExpr* value1,
    const AstExpr* value2,
    const AstExpr* value3,
    const AstExpr* value4,
    const AstExpr* value5,
    const AstExpr* value6
  );

  /// @brief コンストラクタ
  CptPathDelay(
    const FileRegion& file_region,
    const AstExpr* value1,
    const AstExpr* value2,
    const AstExpr* value3,
    const AstExpr* value4,
    const AstExpr* value5,
    const AstExpr* value6,
    const AstExpr* value7,
    const AstExpr* value8,
    const AstExpr* value9,
    const AstExpr* value10,
    const AstExpr* value11,
    const AstExpr* value12
  );

  /// @brief デストラクタ
  ~CptPathDelay();


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDelay の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 値を取り出す．
  const AstExpr*
  value(
    SizeType pos
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ディレイ値
  const AstExpr* mValues[12];

};

END_NAMESPACE_YM_VERILOG

#endif // CPTSPECITEM_H
