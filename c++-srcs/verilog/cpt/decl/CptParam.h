#ifndef CPTPARAM_H
#define CPTPARAM_H

/// @file CptParam.h
/// @brief CptParam のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptDeclHead.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptParamH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptParamH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptDeclHead(file_region, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptParamH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  AstDeclHead::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptLocalParamH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptDeclHead(file_region, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptLocalParamH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  AstDeclHead::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHV :
  public CptParamH
{
public:

  /// @brief コンストラクタ
  CptParamHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtRange* range,          ///< [in] 範囲
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptParamH(file_region, item_top),
      mRange{range}
  {
  }

  /// @brief デストラクタ
  ~CptParamHV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲Bの取得
  /// @return 範囲
  const PtRange*
  range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const PtRange* mRange;

};


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHSV :
  public CptParamHV
{
public:

  /// @brief コンストラクタ
  CptParamHSV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtRange* range,          ///< [in] 範囲
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptParamHV(file_region, range, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptParamHSV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

};


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHV :
  public CptParamHV
{
public:

  /// @brief コンストラクタ
  CptLocalParamHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtRange* range,          ///< [in] 範囲
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptParamHV(file_region, range, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptLocalParamHV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  AstDeclHead::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHSV :
  public CptParamHSV
{
public:

  /// @brief コンストラクタ
  CptLocalParamHSV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtRange* range,	   ///< [in] 範囲
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptParamHSV(file_region, range, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptLocalParamHSV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  AstDeclHead::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// 組み込み型 parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptParamHT :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptParamHT(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiVarType var_type,           ///< [in] データ型
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptDeclHead(file_region, item_top),
      mVarType{var_type}
  {
  }

  /// @brief デストラクタ
  ~CptParamHT() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  AstDeclHead::Type
  type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ型
  VpiVarType mVarType;

};


//////////////////////////////////////////////////////////////////////
/// 組み込み型 localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////
class CptLocalParamHT :
  public CptParamHT
{
public:

  /// @brief コンストラクタ
  CptLocalParamHT(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiVarType var_type,           ///< [in] データ型
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptParamHT(file_region, var_type, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptLocalParamHT() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  AstDeclHead::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// specparam 宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptSpecParamH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptSpecParamH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptDeclHead(file_region, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptSpecParamH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  AstDeclHead::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の specparam
//////////////////////////////////////////////////////////////////////
class CptSpecParamHV :
  public CptSpecParamH
{
public:

  /// @brief コンストラクタ
  CptSpecParamHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const PtRange* range,          ///< [in] パース木の範囲定義
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptSpecParamH(file_region, item_top),
      mRange{range}
  {
  }

  /// @brief デストラクタ
  ~CptSpecParamHV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲を返す．
  /// @return 範囲
  const PtRange*
  range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const PtRange* mRange;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTPARAM_H
