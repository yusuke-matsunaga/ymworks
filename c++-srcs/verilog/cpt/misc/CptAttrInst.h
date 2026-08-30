#ifndef CPTATTRINST_H
#define CPTATTRINST_H

/// @file CptMisc.h
/// @brief CptMisc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtMisc.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class CptAttrInst :
  public PtAttrInst
{
public:

  /// @brief コンストラクタ
  CptAttrInst(
    const FileRegion& file_region,
    PtAttrSpec* as_top
  ) : mFileRegion{file_region},
      mAttrSpecTop{as_top}
  {
  }

  /// @brief デストラクタ
  ~CptAttrInst() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstAttrInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 要素のリストの取得
  AstAttrSpecList
  attrspec_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mFileRegion;

  // attr spec の先頭
  AstAttrSpec* mAttrSpecTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief attr_spec を表すクラス
//////////////////////////////////////////////////////////////////////
class CptAttrSpec :
  public PtAttrSpec
{
public:

  /// @brief コンストラクタ
  CptAttrSpec(
    const FileRegion& file_region,
    const char* name,
    const AstExpr* expr
  ) : mFileRegion{file_region},
      mName{name},
      mExpr{expr}
  {
  }

  /// @brief デストラクタ
  ~CptAttrSpec() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstAttrSpec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief 式を取り出す．nullptr の場合もある．
  const AstExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // 式
  const AstExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTATTRINST_H
