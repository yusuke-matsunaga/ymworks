#ifndef CPTREG_H
#define CPTREG_H

/// @file CptReg.h
/// @brief CptReg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptDeclHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptRegH(
    const FileRegion& file_region ///< [in] ファイル位置の情報
  ) : CptDeclHead(file_region)
  {
  }

  /// @brief デストラクタ
  ~CptRegH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHV :
  public CptRegH
{
public:

  /// @brief コンストラクタ
  CptRegHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const AstRange* range	   ///< [in] 範囲
  ) : CptRegH(file_region),
      mRange{range}
  {
  }

  /// @brief デストラクタ
  ~CptRegHV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無の取得
  /// @return このクラスでは常に false を返す．
  bool
  is_signed() const override;

  /// @brief 範囲の取得
  /// @return 範囲
  const AstRange*
  range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const AstRange* mRange;

};


//////////////////////////////////////////////////////////////////////
/// 符号つきビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptRegHSV :
  public CptRegHV
{
public:

  /// @brief コンストラクタ
  CptRegHSV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const AstRange* range           ///< [in] パース木の範囲定義
  ) : CptRegHV(file_region, range)
  {
  }

  /// @brief デストラクタ
  ~CptRegHSV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無の取得
  /// @return このクラスでは常に true を返す．
  bool
  is_signed() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTREG_H
