#ifndef CPTPRIMARY_H
#define CPTPRIMARY_H

/// @file CptPrimary.h
/// @brief CptPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// 解説
// もとは generic な一つのクラスだったがメモリ使用量を減らすために
// カスタマイズした．
// 直交した2つの属性で分類している．
// - 階層の有無
// - インデックス/範囲指定の有無, 定数式か否か
// この2つを多重継承で組み合わせると仮想関数テーブルがその数だけ必要に
// なってメモリ節約の意味がなくなるのでかっこ悪さを承知で階層の有無に関
// してはクラスごとに仮想関数を書く．
// また，これらは全ての組合わせがあるわけではないので実際に使われている
// クラスだけ考える．具体的には以下の組み合わせ．
//
//                             simple             hierarchical
// none                          X                     X
// index                                               X
// constant index                X                     X
// range                                               X
// constant range                X
// index and range                                     X
// constant index and range

//////////////////////////////////////////////////////////////////////
// Primary のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryBase :
  public CptExpr
{
public:

  // コンストラクタ
  CptPrimaryBase(
    const char* name
  ) : mName{name}
  {
  }

  // デストラクタ
  ~CptPrimaryBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  Type
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前を取り出す．
  const char*
  name() const override;

  // index_list も range も持たないとき true を返す．
  bool
  is_simple() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 末尾の名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
// Primary のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptPrimary :
  public CptPrimaryBase
{
public:

  // コンストラクタ
  CptPrimary(
    const FileRegion& file_region,
    const char* name
  ) : CptPrimaryBase(name),
      mFileRegion{file_region}
  {
  }

  // デストラクタ
  ~CptPrimary() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
// インデックスつきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryI :
  public CptPrimaryBase
{
public:

  // コンストラクタ
  CptPrimaryI(
    const FileRegion& file_region,
    const char* name,
    const AstExprList* index_list
  ) : CptPrimaryBase(name),
      mFileRegion{file_region},
      mIndexList{index_list}
  {
  }

  // デストラクタ
  ~CptPrimaryI() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief インデックスリストのサイズの取得
  /// @return インデックスリストのサイズ
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  SizeType
  index_num() const override;

  /// @brief インデックスの取得
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  const AstExpr*
  index(
    SizeType i ///< [in] インデックス ( 0 <= i < index_num() )
  ) const override;

  /// @brief インデックスリストの取得
  AstExprVec
  index_list() const override;

  // index_list も range も持たないとき true を返す．
  bool
  is_simple() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // インデックスの配列
  const AstExprList* mIndexList;

};


//////////////////////////////////////////////////////////////////////
// 定数インデックスつきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryCI :
  public CptPrimaryI
{
public:

  // コンストラクタ
  CptPrimaryCI(
    const FileRegion& file_region,
    const char* name,
    const AstExprList* index_list
  ) : CptPrimaryI(file_region, name, index_list)
  {
  }

  // デストラクタ
  ~CptPrimaryCI() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // インデックスもしくは範囲が定数にならなければならないとき true を返す．
  bool
  is_const_index() const override;

};


//////////////////////////////////////////////////////////////////////
// 範囲指定つきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryR :
  public CptPrimaryBase
{
public:

  // コンストラクタ
  CptPrimaryR(
    const FileRegion& file_region,
    const char* name,
    const AstPart* part
  ) : CptPrimaryBase(name),
      mFileRegion{file_region},
      mPart{part}
  {
  }

  // デストラクタ
  ~CptPrimaryR() {}


public:
  //////////////////////////////////////////////////////////////////////
  // Primary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // 範囲指定の取得
  const AstPart*
  part() const override;

  // index_list も range も持たないとき true を返す．
  bool
  is_simple() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 範囲指定
  const AstPart* mPart;

};


//////////////////////////////////////////////////////////////////////
// 定数の範囲指定つきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryCR :
  public CptPrimaryR
{
public:

  // コンストラクタ
  CptPrimaryCR(
    const FileRegion& file_region,
    const char* name,
    const AstPart* part
  ) : CptPrimaryR(file_region, name, part)
  {
  }

  // デストラクタ
  ~CptPrimaryCR() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // インデックスもしくは範囲が定数にならなければならないとき true を返す．
  bool
  is_const_index() const override;

};


//////////////////////////////////////////////////////////////////////
// インデックスと範囲指定つきの primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryIR :
  public CptPrimaryI
{
public:

  // コンストラクタ
  CptPrimaryIR(
    const FileRegion& file_region,
    const char* name,
    const AstExprList* index_list,
    const AstPart* part
  ) : CptPrimaryI(file_region, name, index_list),
      mPart{part}
  {
  }

  // デストラクタ
  ~CptPrimaryIR() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // 範囲指定を取出す．
  const AstPart*
  part() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲指定
  const AstPart* mPart;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つ primary operatorを表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryH :
  public CptPrimaryBase
{
public:

  // コンストラクタ
  CptPrimaryH(
    const FileRegion& file_region,
    PtNameBranchArray&& nb_list,
    const char* tail_name
  ) : CptPrimaryBase(tail_name),
      mFileRegion{file_region},
      mNbList{std::move(nb_list)}
  {
  }

  // デストラクタ
  ~CptPrimaryH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチを返す．
  const AstNameBranch*
  namebranch(
    SizeType index ///< [in] インデックス ( 0 <= index < namebranch_num() )
  ) const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchVec
  namebranch_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 階層ブランチのリスト
  PtNameBranchArray mNbList;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つインデックスつき primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryHI :
  public CptPrimaryI
{
public:

  // コンストラクタ
  CptPrimaryHI(
    const FileRegion& file_region,
    PtNameBranchArray&& nb_list,
    const char* tail_name,
    const AstExprList* index_list
  ) : CptPrimaryI(file_region, tail_name, index_list),
      mNbList{std::move(nb_list)}
  {
  }

  // デストラクタ
  ~CptPrimaryHI() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチを返す．
  const AstNameBranch*
  namebranch(
    SizeType index ///< [in] インデックス ( 0 <= index < namebranch_num() )
  ) const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchVec
  namebranch_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchArray mNbList;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つ定数インデックスつき primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryHCI :
  public CptPrimaryHI
{
public:

  // コンストラクタ
  CptPrimaryHCI(
    const FileRegion& file_region,
    PtNameBranchArray&& nb_list,
    const char* tail_name,
    const AstExprList* index_list
  ) : CptPrimaryHI(file_region, std::move(nb_list),
		   tail_name, index_list)
  {
  }

  // デストラクタ
  ~CptPrimaryHCI() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  // インデックスもしくは範囲が定数にならなければならないとき true を返す．
  bool
  is_const_index() const override;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つ範囲指定つき primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryHR :
  public CptPrimaryR
{
public:

  // コンストラクタ
  CptPrimaryHR(
    const FileRegion& file_region,
    PtNameBranchArray&& nb_list,
    const char* tail_name,
    const AstPart* part
  ) : CptPrimaryR(file_region, tail_name, part),
      mNbList{std::move(nb_list)}
  {
  }

  // デストラクタ
  ~CptPrimaryHR() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチを返す．
  const AstNameBranch*
  namebranch(
    SizeType index ///< [in] インデックス ( 0 <= index < namebranch_num() )
  ) const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchVec
  namebranch_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchArray mNbList;

};


//////////////////////////////////////////////////////////////////////
// 階層名を持つインデックスと範囲指定つき primary を表すクラス
//////////////////////////////////////////////////////////////////////
class CptPrimaryHIR :
  public CptPrimaryIR
{
public:

  // コンストラクタ
  CptPrimaryHIR(
    const FileRegion& file_region,
    PtNameBranchArray&& nb_list,
    const char* tail_name,
    const AstExprList* index_list,
    const AstPart* part
  ) : CptPrimaryIR(file_region, tail_name,
		   index_list, part),
      mNbList{std::move(nb_list)}
  {
  }

  // デストラクタ
  ~CptPrimaryHIR() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPrimary の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチを返す．
  const AstNameBranch*
  namebranch(
    SizeType index ///< [in] インデックス ( 0 <= index < namebranch_num() )
  ) const override;

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchVec
  namebranch_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchArray mNbList;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTPRIMARY_H
