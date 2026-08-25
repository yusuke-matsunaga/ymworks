#ifndef CPTDECLHEAD_H
#define CPTDECLHEAD_H

/// @file CptDeclHead.h
/// @brief CptDeclHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtDecl.h"
#include "ym/FileRegion.h"
#include "ym/vl/Ast.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class CptDeclHead :
  public PtDeclHead
{
protected:

  /// @brief コンストラクタ
  CptDeclHead(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    PtDeclItem* item_top           ///< [in] 要素の先頭
  ) : mFileRegion{file_region},
      mItemTop{item_top}
  {
  }

  /// デストラクタ
  ~CptDeclHead() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲Bの取得
  /// @retval 範囲 範囲を持つとき
  /// @retval nullptr 範囲を持たないとき
  const AstRange*
  range() const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  VpiNetType
  net_type() const override;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  VpiVsType
  vs_type() const override;

  /// @brief strength の取得
  /// @retval strength
  /// @retval nullptr strength の指定なし
  const AstStrength*
  strength() const override;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  const AstDelay*
  delay() const override;

  /// @brief 要素のリストを返す．
  AstDeclItemList
  item_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 先頭の要素を返す．
  PtDeclItem*
  _item_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の先頭
  PtDeclItem* mItemTop{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// 変数宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptVarH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptVarH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiVarType var_type,           ///< [in] データ型
    PtDeclItem* item_top = nullptr ///< [in] 要素の先頭
  ) : CptDeclHead(file_region, item_top),
      mVarType{var_type}
  {
  }

  /// @brief デストラクタ
  ~CptVarH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  Type
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
/// genvar宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenvarH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptGenvarH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    PtDeclItem* item_top           ///< [in] 要素の先頭
  ) : CptDeclHead(file_region, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptGenvarH() {}


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
/// event 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptEventH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptEventH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    PtDeclItem* item_top           ///< [in] 要素の先頭
  ) : CptDeclHead(file_region, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptEventH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  Type
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTDECLHEAD_H
