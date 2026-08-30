#ifndef CPTFUNCCALL_H
#define CPTFUNCCALL_H

/// @file CptFuncCall.h
/// @brief CptFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"
#include "parser/PtHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptFuncCallBase CptFuncCall.h "CptFuncCall.h"
/// @brief function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////
class CptFuncCallBase :
  public CptExpr
{
public:

  // コンストラクタ
  CptFuncCallBase(
    const FileRegion& file_region,
    const char* name,
    const AstExpr* arg_top
  ) : mFileRegion{file_region},
      mName{name},
      mArgTop{arg_top}
  {
  }

  // デストラクタ
  ~CptFuncCallBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;

  /// @brief 引数リストの取得
  AstExprList
  arg_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 末尾の名前
  const char* mName;

  // 引数の先頭
  const AstExpr* mArgTop;

};


//////////////////////////////////////////////////////////////////////
/// @class CptFuncCall CptFuncCall.h "CptFuncCall.h"
/// @brief 階層なし名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFuncCall :
  public CptFuncCallBase
{
public:

  // コンストラクタ
  CptFuncCall(
    const FileRegion& file_region,
    const char* name,
    const AstExpr* arg_top
  ) : CptFuncCallBase(file_region, name, arg_top)
  {
  }

  // デストラクタ
  ~CptFuncCall() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  // このクラスの場合は FunCall を返す．
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptFuncCallH CptFuncCall.h "CptFuncCall.h"
/// @brief 階層つき名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFuncCallH :
  public CptFuncCall
{
public:

  // コンストラクタ
  CptFuncCallH(
    const FileRegion& file_region,
    const PtHierName& hname,
    const AstExpr* arg_top
  ) : CptFuncCall(file_region, hname.tail_name, arg_top),
      mNbTop{hname.nb_list.top}
  {
  }

  // デストラクタ
  ~CptFuncCallH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstHierNamedBase の仮想関数
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


//////////////////////////////////////////////////////////////////////
/// @class CptSysFuncCall CptFuncCall.h "CptFuncCall.h"
/// @brief system function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSysFuncCall :
  public CptFuncCallBase
{
public:

  // コンストラクタ
  CptSysFuncCall(
    const FileRegion& file_region,
    const char* name,
    const AstExpr* arg_top
  ) : CptFuncCallBase(file_region, name, arg_top)
  {
  }

  // デストラクタ
  ~CptSysFuncCall() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  Type
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTEXPR_H
