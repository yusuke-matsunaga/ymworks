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

#if 0
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
#endif


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

  /// @brief PtAttrInst* 型のリンクを返す．
  const AstAttrInst*
  link() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtAttrInst* 型のリンクを返す．
  PtAttrInst*
  _link() const override;

  /// @brief link を設定する．
  void
  set_link(
    PtAttrInst* link
  ) override;

  /// @brief リンクトリストを逆順にする．
  /// @return 新しい先頭を返す．
  PtAttrInst*
  reverse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mFileRegion;

  // attr spec の先頭
  AstAttrSpec* mAttrSpecTop;

  // 次の要素
  PtAttrInst* mLink{nullptr};

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

  /// @brief 次の要素を返す．
  const AstAttrSpec*
  link() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrSpec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief link を設定する．
  void
  set_link(
    PtAttrSpec* link
  ) override;

  /// @brief PtAttrSpec* 型のリンクを返す．
  PtAttrSpec*
  _link() const override;

  /// @brief リンクトリストを逆順にする．
  /// @return 新しい先頭を返す．
  PtAttrSpec*
  reverse() override;


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

  // 次の世嘘
  PtAttrSpec* mLink{nullptr};

};

END_NAMESPACE_YM_VERILOG

#endif // CPTATTRINST_H
