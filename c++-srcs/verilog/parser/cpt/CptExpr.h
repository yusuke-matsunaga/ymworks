#ifndef CPTEXPR_H
#define CPTEXPR_H

/// @file CptExpr.h
/// @brief CptExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtExpr.h"
#include "parser/PtHierName.h"
#include "ym/vl/BitVector.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// expression を表す基底クラス
//////////////////////////////////////////////////////////////////////
class CptExpr :
  public PtExpr
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
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

  // 末尾の名前を返す．
  const char*
  name() const override;

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  VpiOpType
  op_type() const override;

  /// @brief 0番目のオペランドの取得
  const AstExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const AstExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const AstExpr*
  operand2() const override;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  ///
  /// - Concat/MultiConcat/FuncCall/SysFuncCall 演算以外は
  ///   std::logi_error 例外を送出する．
  SizeType
  operand_num() const override;

  /// @brief オペランドのリストの取得
  ///
  /// - Concat/MultiConcat/FuncCall/SysFuncCall 演算以外は
  ///   std::logi_error 例外を送出する．
  const AstExpr*
  operand(
    SizeType index ///< [in] インデックス ( 0 <= index < operand_num() )
  ) const override;

  /// @brief オペランドのリストの取得
  AstExprVec
  operand_list() const override;

  /// @brief multi-concat の繰り返し数
  ///
  /// - type() != Opr および op_type() != MultiConcat の時 std::logic 例外を送出する．
  const AstExpr*
  rep() const override;

  /// @brief 定数インデックスのチェック
  /// @retval true インデックスもしくは範囲が定数にならなければならないとき
  /// @retval false 上記以外
  bool
  is_const_index() const override;

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

  /// @brief 範囲指定を表す構文木を返す．
  /// @return 範囲指定
  const AstPart*
  part() const override;

  /// @brief 定数の種類の取得
  /// @return 定数の種類
  VpiConstType
  const_type() const override;

  /// @brief 整数型の定数のサイズの取得
  /// @return サイズ
  SizeType
  const_size() const override;

  /// @brief ビットベクタ型の値の取得
  /// @return 値
  BitVector
  const_bitvect() const override;

  /// @brief 整数型および文字列型の定数の文字列表現の取得
  /// @return 値の文字列表現
  const char*
  const_str() const override;

  /// @brief 実数型の値の取得
  /// @return 値
  double
  const_real() const override;

  /// @brief インデックスとして使える式のチェック
  /// @retval true 階層名の添字として使える式
  /// @retval false 使えない式
  bool
  is_index_expr() const override;

  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  int
  index_value() const override;

  /// @brief simple primary のチェック
  /// @retval true index_list も range も持たないとき
  /// @retval false 上記以外
  bool
  is_simple() const override;

};


//////////////////////////////////////////////////////////////////////
// min/typ/max delayを表すクラス
//////////////////////////////////////////////////////////////////////
class CptMinTypMax :
  public CptExpr
{
public:

  // コンストラクタ
  CptMinTypMax(
    const AstExpr* val0,
    const AstExpr* val1,
    const AstExpr* val2
  ) : mValue{val0, val1, val2}
  {
    if ( val0 == nullptr ) {
      throw std::logic_error{"val0 == nullptr"};
    }
    if ( val1 == nullptr ) {
      throw std::logic_error{"val1 == nullptr"};
    }
    if ( val2 == nullptr ) {
      throw std::logic_error{"val2 == nullptr"};
    }
  }

  // デストラクタ
  ~CptMinTypMax() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // クラスの型を返す．
  Type
  type() const override;

  ///演算子の種類の取得
  VpiOpType
  op_type() const override;

  /// @brief 0番目のオペランドの取得
  const AstExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const AstExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  const AstExpr*
  operand2() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const AstExpr* mValue[3];

};


//////////////////////////////////////////////////////////////////////
// function call / system function call に共通の基底クラス
//////////////////////////////////////////////////////////////////////
class CptFuncCallBase :
  public CptExpr
{
public:

  // コンストラクタ
  CptFuncCallBase(
    const FileRegion& file_region,
    const char* name,
    PtExprArray&& arg_list
  ) : mFileRegion{file_region},
      mName{name},
      mArgList{std::move(arg_list)}
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

  /// @brief オペランドの数の取得
  /// @return 子供の数
  ///
  /// - Concat/MultiConcat/FuncCall/SysFuncCall 演算以外は
  ///   std::logi_error 例外を送出する．
  SizeType
  operand_num() const override;

  /// @brief オペランドのリストの取得
  ///
  /// - Concat/MultiConcat/FuncCall/SysFuncCall 演算以外は
  ///   std::logi_error 例外を送出する．
  const AstExpr*
  operand(
    SizeType index ///< [in] インデックス ( 0 <= index < operand_num() )
  ) const override;

  /// @brief オペランドのリストの取得
  AstExprVec
  operand_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 末尾の名前
  const char* mName;

  // 引数の配列
  PtExprArray mArgList;

};


//////////////////////////////////////////////////////////////////////
// 階層なし名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFuncCall :
  public CptFuncCallBase
{
public:

  // コンストラクタ
  CptFuncCall(
    const FileRegion& file_region,
    const char* name,
    PtExprArray&& arg_list
  ) : CptFuncCallBase(file_region, name, std::move(arg_list))
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
// 階層つき名前を持つ function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFuncCallH :
  public CptFuncCall
{
public:

  // コンストラクタ
  CptFuncCallH(
    const FileRegion& file_region,
    PtNameBranchArray&& nb_list,
    const char* tail_name,
    PtExprArray&& arg_list
  ) : CptFuncCall(file_region, tail_name, std::move(arg_list)),
      mNbList{std::move(nb_list)}
  {
  }

  // デストラクタ
  ~CptFuncCallH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
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
// system function call を表すクラス
//////////////////////////////////////////////////////////////////////
class CptSysFuncCall :
  public CptFuncCallBase
{
public:

  // コンストラクタ
  CptSysFuncCall(
    const FileRegion& file_region,
    const char* name,
    PtExprArray&& arg_list
  ) : CptFuncCallBase(file_region, name, std::move(arg_list))
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
