#ifndef CPTEXPR_H
#define CPTEXPR_H

/// @file CptExpr.h
/// @brief CptExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtExpr.h"
#include "parser/PtMisc.h"
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
  // AstHierNamedBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;

  // 末尾の名前を返す．
  const char*
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  VpiOpType
  op_type() const override;

  /// @brief 0番目のオペランドの取得
  ///
  /// - 単項，二項，三項演算以外は std::logic_error 例外を送出する．
  const AstExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  ///
  /// - 二項，三項演算以外は std::logic_error 例外を送出する．
  const AstExpr*
  operand1() const override;

  /// @brief 2番目のオペランドの取得
  ///
  /// - 三項演算以外は std::logic_error 例外を送出する．
  const AstExpr*
  operand2() const override;

  /// @brief オペランドのリストの取得
  AstExprList
  operand_list() const override;

  /// @brief multi-concat の繰り返し数
  ///
  /// - type() != Opr および op_type() != MultiConcat の時 std::logic 例外を送出する．
  const AstExpr*
  rep() const override;

  /// @brief 引数リストの取得
  AstExprList
  arg_list() const override;

  /// @brief 定数インデックスのチェック
  /// @retval true インデックスもしくは範囲が定数にならなければならないとき
  /// @retval false 上記以外
  bool
  is_const_index() const override;

  /// @brief インデックスリストの取得
  AstExprList
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

END_NAMESPACE_YM_VERILOG

#endif // CPTEXPR_H
