#ifndef CPTITEM_DEFPARAMH_H
#define CPTITEM_DEFPARAMH_H

/// @file CptItem_DefParamH.h
/// @brief CptItem_DefParamH のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtDefParam.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptItem_DefParamH CptItem_DefParamH.h "CptItem_DefParamH.h"
/// @brief defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptItem_DefParamH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptItem_DefParamH(
    const FileRegion& file_region,
    const PtDefParam* dp_top
  ) : mFileRegion{file_region},
      mTop{dp_top}
  {
  }

  /// @brief デストラクタ
  ~CptItem_DefParamH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

  /// @brief defparam リストの取得
  const PtDefParam*
  defparam_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の先頭
  const PtDefParam* mTop;

};


//////////////////////////////////////////////////////////////////////
/// @class CptDefParam CptItem_DefParamH.h "CptItem_DefParamH.h"
/// @brief 階層名を持たない defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDefParam :
  public PtDefParam
{
public:

  /// @brief コンストラクタ
  CptDefParam(
    const FileRegion& file_region,
    const char* name,
    const PtExpr* value
  ) : mTopLoc{file_region.start_loc()},
      mName{name},
      mExpr{value}
  {
    if ( value == nullptr ) {
      throw std::logic_error{"value = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptDefParam() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチのリストを返す．
  const PtNameBranch*
  namebranch_top() const override;

  /// @brief 末尾の名前の取得
  /// @return 末尾の名前
  const char*
  name() const override;

  /// @brief 値の取得
  /// @return 値を表す式
  const PtExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  FileLoc mTopLoc;

  // 末尾の名前
  const char* mName;

  // 値
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief CptDefParam2 CptItem_DefParamH.h "CptItem_DefParamH.h"
/// @brief 階層名を持つ defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDefParam2 :
  public CptDefParam
{
public:

  /// @brief コンストラクタ
  CptDefParam2(
    const FileRegion& file_region,
    const PtNameBranch* nb_top,
    const char* tail_name,
    const PtExpr* value
  ) : CptDefParam(file_region, tail_name, value),
      mNbTop{nb_top}
  {
  }

  /// @brief デストラクタ
  ~CptDefParam2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  const PtNameBranch*
  namebranch_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の階層ブランチ
  const PtNameBranch* mNbTop;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_DEFPARAMH_H
