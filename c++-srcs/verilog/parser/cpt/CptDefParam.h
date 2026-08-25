#ifndef CPTDEFPARAM_H
#define CPTDEFPARAM_H

/// @file CptDefParam.h
/// @brief CptDefParam のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptDefParamH CptDefParam.h "CptDefParam.h"
/// @brief defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptDefParamH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptDefParamH(
    const FileRegion& file_region,
    PtDefParam* dp_top
  ) : mFileRegion{file_region},
      mTop{dp_top}
  {
  }

  /// @brief デストラクタ
  ~CptDefParamH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief defparam リストの取得
  AstDefParamList
  defparam_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の先頭
  const AstDefParam* mTop;

};


//////////////////////////////////////////////////////////////////////
/// @class CptDefParam CptDefParam.h "CptDefParam.h"
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
    const AstExpr* value
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
  // AstDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;

  /// @brief 末尾の名前の取得
  /// @return 末尾の名前
  const char*
  name() const override;

  /// @brief 値の取得
  /// @return 値を表す式
  const AstExpr*
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
  const AstExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief CptDefParam2 CptDefParam.h "CptDefParam.h"
/// @brief 階層名を持つ defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDefParam2 :
  public CptDefParam
{
public:

  /// @brief コンストラクタ
  CptDefParam2(
    const FileRegion& file_region,
    const AstNameBranch* nb_top,
    const char* tail_name,
    const AstExpr* value
  ) : CptDefParam(file_region, tail_name, value),
      mNbTop{nb_top}
  {
  }

  /// @brief デストラクタ
  ~CptDefParam2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の階層ブランチ
  const AstNameBranch* mNbTop;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_H
