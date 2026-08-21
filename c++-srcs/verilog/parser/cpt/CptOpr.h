#ifndef CPTOPR_H
#define CPTOPR_H

/// @file CptOpr.h
/// @brief CptOpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptOpr :
  public CptExpr
{
protected:

  // コンストラクタ
  CptOpr(
    VpiOpType op_type ///< [in] 演算子の型
  ) : mOpType{op_type}
  {
  }

  // デストラクタ
  ~CptOpr() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  // このクラスの場合は kOpr を返す．
  Type
  type() const override;

  // 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算子を表すトークン番号
  VpiOpType mOpType;

};


//////////////////////////////////////////////////////////////////////
// 単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr1 :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr1(
    const FileRegion& file_region,
    VpiOpType op_type,
    const AstExpr* opr
  ) : CptOpr(op_type),
      mFileRegion{file_region},
      mOpr{opr}
  {
    if ( opr == nullptr ) {
      throw std::logic_error{"opr == nullptr"};
    }
  }

  // デストラクタ
  ~CptOpr1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  // 階層名の添字として使える式の時に true を返す．
  bool
  is_index_expr() const override;

  // 階層名の添字として使える式の時にその値を返す．
  int
  index_value() const override;

  /// @brief 0番目のオペランドの取得
  const AstExpr*
  operand0() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // オペランド
  const AstExpr* mOpr;

};


//////////////////////////////////////////////////////////////////////
// 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr2 :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr2(
    VpiOpType op_type,
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr(op_type),
      mOpr{opr1, opr2}
  {
    if ( opr1 == nullptr ) {
      throw std::logic_error{"opr1 == nullptr"};
    }
    if ( opr2 == nullptr ) {
      throw std::logic_error{"opr2 == nullptr"};
    }
  }

  // デストラクタ
  ~CptOpr2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 0番目のオペランドの取得
  const AstExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const AstExpr*
  operand1() const override;

  /// @brief インデックスとして使える式のチェック
  /// @retval true 階層名の添字として使える式
  /// @retval false 使えない式
  bool
  is_index_expr() const override;

  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  int
  index_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  const AstExpr* mOpr[2];

};


//////////////////////////////////////////////////////////////////////
// 三項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr3 :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr3(
    VpiOpType op_type,
    const AstExpr* opr1,
    const AstExpr* opr2,
    const AstExpr* opr3
  ) : CptOpr(op_type),
      mOpr{opr1, opr2, opr3}
  {
    if ( opr1 == nullptr ) {
      throw std::logic_error{"opr1 == nullptr"};
    }
    if ( opr2 == nullptr ) {
      throw std::logic_error{"opr2 == nullptr"};
    }
    if ( opr3 == nullptr ) {
      throw std::logic_error{"opr3 == nullptr"};
    }
  }

  // デストラクタ
  ~CptOpr3() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

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

  // オペランド
  const AstExpr* mOpr[3];

};


//////////////////////////////////////////////////////////////////////
// concatenation を表すクラス
//////////////////////////////////////////////////////////////////////
class CptConcat :
  public CptExpr
{
public:

  // コンストラクタ
  CptConcat(
    const FileRegion& file_region,
    const AstExprList* expr_list
  ) : mFileRegion{file_region},
      mExprList{expr_list}
  {
  }

  // デストラクタ
  ~CptConcat() {}


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

  // 結合する式のリスト
  const AstExprList* mExprList;

};


//////////////////////////////////////////////////////////////////////
// multiple concatenation を表すクラス
//////////////////////////////////////////////////////////////////////
class CptMultiConcat :
  public CptConcat
{
public:

  // コンストラクタ
  CptMultiConcat(
    const FileRegion& file_region,
    const AstExpr* rep,
    const AstExprList* expr_list
  ) : CptConcat(file_region, expr_list),
      mRep{rep}
  {
  }

  // デストラクタ
  ~CptMultiConcat() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  ///演算子の種類の取得
  VpiOpType
  op_type() const override;

  /// @brief multi-concat の繰り返し数
  ///
  /// - type() != Opr および op_type() != MultiConcat の時 std::logic 例外を送出する．
  const AstExpr*
  rep() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数
  const AstExpr* mRep;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_H
