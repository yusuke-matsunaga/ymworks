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
#include "parser/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
/// @brief AttrInst のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class CptAttrInstList :
  public PtAttrInstList
{
public:

  /// @brief コンストラクタ
  CptAttrInstList(
    PtAttrInstArray&& ai_list
  ) : mList{std::move(ai_list)}
  {
  }

  /// @brief デストラクタ
  ~CptAttrInstList() { }


public:
  //////////////////////////////////////////////////////////////////////
  // AstAttrInstList の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  attrinst_num() const override;

  /// @brief 要素を返す．
  const AstAttrInst*
  attrinst(
    SizeType index ///< [in] インデックス ( 0 <= index < attrinst_num() )
  ) const override;

  /// @brief 要素のリストを返す．
  AstAttrInstVec
  attrinst_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  PtAttrInstArray&& mList;

};


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
    PtAttrSpecArray&& as_list
  ) : mFileRegion{file_region},
      mAttrSpecList{std::move(as_list)}
  {
  }

  /// @brief デストラクタ
  ~CptAttrInst() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 要素数の取得
  SizeType
  attrspec_num() const override;

  /// @brief 要素の取得
  const AstAttrSpec*
  attrspec(
    SizeType index ///< [in] インデックス ( 0 <= index < attrspec_num() )
  ) const override;

  /// @brief 要素のリストの取得
  AstAttrSpecVec
  attrspec_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mFileRegion;

  // attr spec のリスト
  PtAttrSpecArray mAttrSpecList;

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
  // PtAttrSpec の仮想関数
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
