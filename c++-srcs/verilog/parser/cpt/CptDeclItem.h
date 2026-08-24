#ifndef CPTDECLITEM_H
#define CPTDECLITEM_H

/// @file CptDeclItem.h
/// @brief CptDeclItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtDecl.h"
#include "ym/FileRegion.h"
#include "ym/vl/Ast.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// 宣言要素のベースクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemBase :
  public PtDeclItem
{
public:

  /// @brief コンストラクタ
  CptDeclItemBase(
    const char* name ///< [in] 名前
  ) : mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptDeclItemBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  /// @return 名前
  const char*
  name() const override;

  /// @brief 範囲のリストを返す．
  const AstRangeList
  range_list() const override;

  /// @brief 初期値を取り出す．
  /// @return ここでは常に nullptr を返す．
  const AstExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// 宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItem :
  public CptDeclItemBase
{
public:

  /// @brief コンストラクタ
  CptDeclItem(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name               ///< [in] 名前
  ) : CptDeclItemBase(name),
      mLoc{file_region}
  {
  }

  /// @brief デストラクタ
  ~CptDeclItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  FileRegion
  file_region() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トークンの位置
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// 範囲を持った宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemR :
  public CptDeclItemBase
{
public:

  /// @brief コンストラクタ
  CptDeclItemR(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name,              ///< [in] 名前
    const AstRange* range_top      ///< [in] 範囲の先頭
  ) : CptDeclItemBase(name),
      mFileRegion{file_region},
      mRangeTop{range_top}
  {
  }

  /// @brief デストラクタ
  ~CptDeclItemR() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 範囲のリストを返す．
  const AstRangeList
  range_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 範囲の先頭
  const AstRange* mRangeTop;

};


//////////////////////////////////////////////////////////////////////
/// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////
class CptDeclItemI :
  public CptDeclItem
{
public:

  /// @brief コンストラクタ
  CptDeclItemI(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    const char* name,              ///< [in] 名前
    const AstExpr* init_value      ///< [in] 初期値
  ) : CptDeclItem(file_region, name),
      mInitValue{init_value}
  {
    if ( init_value == nullptr ) {
      throw std::logic_error{"init_value == nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptDeclItemI() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDeclItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 初期値を取り出す．
  /// @retval 初期値
  /// @retval nullptr 設定がない場合
  const AstExpr*
  init_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期値
  const AstExpr* mInitValue;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTDECLITEM_H
