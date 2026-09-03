#ifndef CPTIO_H
#define CPTIO_H

/// @file CptIO.h
/// @brief CptIO のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtIOHead.h"
#include "parser/PtIOItem.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief IO宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptIOHBase :
  public PtIOHead
{
protected:

  /// @brief コンストラクタ
  CptIOHBase(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiDir dir,                    ///< [in] IOの種類
    VpiAuxType aux_type,           ///< [in] 補助的な型
    VpiNetType net_type,           ///< [in] 補助的なネット型
    VpiVarType var_type,           ///< [in] 補助的な変数型
    bool sign,                     ///< [in] 符号つきの時 true にするフラグ
    const PtIOItem* item_top       ///< [in] 要素の先頭
  ) : mFileRegion{file_region},
      mItemTop{item_top}
  {
    mAttr =
      static_cast<unsigned int>(sign) |
      (static_cast<unsigned int>(dir) << 1) |
      (static_cast<unsigned int>(aux_type) << 8) |
      (static_cast<unsigned int>(net_type) << 16) |
      (static_cast<unsigned int>(var_type) << 24);
  }

  /// @brief デストラクタ
  ~CptIOHBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 型の取得
  VpiDir
  direction() const override;

  /// @brief 補助的な型の取得
  VpiAuxType
  aux_type() const override;

  /// @brief 補助的なネット型の取得
  VpiNetType
  net_type() const override;

  /// @brief 補助的な変数型の取得
  VpiVarType
  var_type() const override;

  /// @brief 符号の取得
  /// @retval true 符号付き
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲の取得
  /// @retval 範囲
  /// @retval nullptr 範囲を持たないとき
  const PtRange*
  range() const override;

  /// @brief 要素のリストの先頭の取得
  const PtIOItem*
  item_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // type と aux_type と符号を格納するメンバ
  std::uint32_t mAttr;

  // 要素の先頭
  const PtIOItem* mItemTop{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @brief 範囲を持たない IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
class CptIOH :
  public CptIOHBase
{
public:

  /// @brief コンストラクタ
  CptIOH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiDir dir,			   ///< [in] IOの種類
    VpiAuxType aux_type,	   ///< [in] 補助的な型
    VpiNetType net_type,	   ///< [in] 補助的なネット型
    VpiVarType var_type, 	   ///< [in] 補助的な変数型
    const PtIOItem* item_top       ///< [in] 要素の先頭
  ) : CptIOHBase(file_region, dir, aux_type,
		 net_type, var_type, false, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptIOH() {}

};


//////////////////////////////////////////////////////////////////////
/// @brief ビットベクタ型の IO宣言のヘッダ
//////////////////////////////////////////////////////////////////////
class CptIOHV :
  public CptIOHBase
{
public:

  /// @brief コンストラクタ
  CptIOHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiDir dir,			   ///< [in] IOの種類
    VpiAuxType aux_type,	   ///< [in] 補助的な型
    VpiNetType net_type,	   ///< [in] 補助的なネット型
    bool sign,                     ///< [in] 符号つきの時 true にするフラグ
    const PtRange* range,          ///< [in] パース木の範囲定義
    const PtIOItem* item_top       ///< [in] 要素の先頭
  ) : CptIOHBase(file_region, dir, aux_type,
		 net_type, VpiVarType::None, sign, item_top),
      mRange{range}
  {
  }

  /// @brief デストラクタ
  ~CptIOHV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の取得
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
/// @brief IO宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptIOItem :
  public PtIOItem
{
public:

  /// @brief コンストラクタ
  CptIOItem(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name               ///< [in] 名前
  ) : mLoc{file_region},
      mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptIOItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 名前の取得
  const char*
  name() const override;

  /// @brief 初期値の取得
  /// @retval 初期値
  /// @retval nullptr 初期値を持たないとき
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンの位置
  FileRegion mLoc;

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 初期値をもった IO宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class CptIOItemI :
  public CptIOItem
{
public:

  /// @brief コンストラクタ
  CptIOItemI(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name,              ///< [in] 名前
    const PtExpr* init_value      ///< [in] 初期値
  ) : CptIOItem(file_region, name),
      mInitValue{init_value}
  {
    if ( init_value == nullptr ) {
      throw std::logic_error{"init_value == nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptIOItemI() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtIOItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 初期値の取得
  const PtExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  const PtExpr* mInitValue;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTIO_H
