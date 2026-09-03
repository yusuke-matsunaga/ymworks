#ifndef PARSER_PTEXPR_H
#define PARSER_PTEXPR_H

/// @file PtExpr.h
/// @brief PtExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"
#include "ym/vl/AstExpr.h" // for AstExpr::Type


BEGIN_NAMESPACE_YM_VERILOG

class PtNameBranch;
class PtPart;

//////////////////////////////////////////////////////////////////////
/// @class PtExpr PtExpr.h "parser/PtExpr.h"
/// @brief AstExpr の実装クラス
//////////////////////////////////////////////////////////////////////
class PtExpr :
  public PtListElem<PtExpr>
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストの先頭を返す．
  virtual
  const PtNameBranch*
  namebranch_top() const = 0;

  // 末尾の名前を返す．
  virtual
  const char*
  name() const = 0;

  /// @brief 式の型の取得
  /// @return 式の型
  virtual
  AstExpr::Type
  type() const = 0;

  /// @brief インデックスとして使える式のチェック
  /// @retval true 階層名の添字として使える式
  /// @retval false 使えない式
  virtual
  bool
  is_index_expr() const = 0;

  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  ///
  /// - is_index_expr() が false の時は std::logic_error 例外を送出する．
  virtual
  int
  index_value() const = 0;

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  ///
  /// - type() != Opr の時 std::logi_error 例外を送出する．
  virtual
  VpiOpType
  op_type() const = 0;

  /// @brief 固定オペランド数の取得
  ///
  /// Contcat/MultiConcat の場合は 0 を返す．
  virtual
  SizeType
  operand_num() const = 0;

  /// @brief 0番目のオペランドの取得
  ///
  /// - 単項，二項，三項演算以外は std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  operand0() const = 0;

  /// @brief 1番目のオペランドの取得
  ///
  /// - 二項，三項演算以外は std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  operand1() const = 0;

  /// @brief 2番目のオペランドの取得
  ///
  /// - 三項演算以外は std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  operand2() const = 0;

  /// @brief オペランドのリストの先頭の取得
  ///
  /// - Concat/MultiConcat 演算以外は
  ///   std::logi_error 例外を送出する．
  virtual
  const PtExpr*
  operand_top() const = 0;

  /// @brief multi-concat の繰り返し数
  ///
  /// - type() != Opr および op_type() != MultiConcat の時
  ///   std::logic 例外を送出する．
  virtual
  const PtExpr*
  rep() const = 0;

  /// @brief 引数リストの先頭の取得
  ///
  /// - FuncCall/SysFuncCall 演算以外は std::logi_error 例外を送出する．
  virtual
  const PtExpr*
  arg_top() const = 0;

  /// @brief 定数インデックスのチェック
  /// @retval true インデックスもしくは範囲が定数にならなければならないとき
  /// @retval false 上記以外
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  virtual
  bool
  is_const_index() const = 0;

  /// @brief インデックスリストの先頭の取得
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  index_top() const = 0;

  /// @brief 範囲指定を表す構文木を返す．
  /// @return 範囲指定
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  virtual
  const PtPart*
  part() const = 0;

  /// @brief simple primary のチェック
  /// @retval true index_list も part も持たないとき
  /// @retval false 上記以外
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  virtual
  bool
  is_simple() const = 0;

  /// @brief 定数の種類の取得
  /// @return 定数の種類
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  virtual
  VpiConstType
  const_type() const = 0;

  /// @brief 整数型の定数のサイズの取得
  /// @return サイズ
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  /// - サイズ指定の無い場合には 0 を返す．
  virtual
  SizeType
  const_size() const = 0;

  /// @brief ビットベクタ型の値の取得
  /// @return 値
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  virtual
  BitVector
  const_bitvect() const = 0;

  /// @brief 整数型および文字列型の定数の文字列表現の取得
  /// @return 値の文字列表現
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  /// - 整数型時のサイズと基数は含まない．
  /// - 整数型で文字列表現を持たない場合には nullptr を返す．
  virtual
  const char*
  const_str() const = 0;

  /// @brief 実数型の値の取得
  /// @return 値
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  virtual
  double
  const_real() const = 0;

};

/// @brief PtExpr のリスト
using PtExprList = PtList<PtExpr>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTEXPR_H
