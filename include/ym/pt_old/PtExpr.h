#ifndef YM_PT_PTEXPR_H
#define YM_PT_PTEXPR_H

/// @file ym/pt/PtExpr.h
/// @brief PtExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtExpr PtExpr.h <ym/pt/PtExpr.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief expression を表すクラスの共通の親クラス
///
/// 大まかには以下の種類(PtExprType)に分類される．
/// - Opr:         演算子
/// - FuncCall:    関数呼び出し
/// - SysFuncCall: システム関数呼び出し
/// - Primary:     プライマリ
/// - Const:       定数
///
/// 各グループごとに関係するメソッドが用意されている．
/// それ以外のグループの場合には std::logic_error 例外を送出する．
//////////////////////////////////////////////////////////////////////
class PtExpr :
  public PtHierNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 全グループに共通の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の型の取得
  /// @return 式の型
  virtual
  PtExprType
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

  /// @brief 式を表す文字列表現の取得
  virtual
  std::string
  decompile() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子グループの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  ///
  /// - op_type() != Opr の時 std::logi_error 例外を送出する．
  virtual
  VpiOpType
  op_type() const = 0;

  /// @brief オペランドの数の取得
  /// @return 子供の数
  ///
  /// - op_type() != Opr の時 std::logi_error 例外を送出する．
  virtual
  SizeType
  operand_num() const = 0;

  /// @brief 0番目のオペランドの取得
  ///
  /// - operand_num() == 0 の時は std::out_of_range 例外を送出する．
  /// - op_type() != Opr の時 std::logi_error 例外を送出する．
  virtual
  const PtExpr*
  operand0() const = 0;

  /// @brief 1番目のオペランドの取得
  ///
  /// - operand_num() <= 1 の時は std::out_of_range 例外を送出する．
  /// - op_type() != Opr の時 std::logi_error 例外を送出する．
  virtual
  const PtExpr*
  operand1() const = 0;

  /// @brief 2番目のオペランドの取得
  ///
  /// - operand_num() <= 2 の時は std::out_of_range 例外を送出する．
  /// - op_type() != Opr の時 std::logi_error 例外を送出する．
  virtual
  const PtExpr*
  operand2() const = 0;

  /// @brief オペランドの取得
  /// @return pos 番目のオペランド
  ///
  /// - operand_num() <= pos の時は std::out_of_range 例外を送出する．
  /// - op_type() != Opr の時 std::logi_error 例外を送出する．
  virtual
  const PtExpr*
  operand(
    SizeType pos ///< [in] 取り出すオペランンドの位置(最初の位置は 0)
  ) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // プライマリグループの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数インデックスのチェック
  /// @retval true インデックスもしくは範囲が定数にならなければならないとき
  /// @retval false 上記以外
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  virtual
  bool
  is_const_index() const = 0;

  /// @brief インデックスリストのサイズの取得
  /// @return インデックスリストのサイズ
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  virtual
  SizeType
  index_num() const = 0;

  /// @brief インデックスの取得
  ///
  /// - index_num() <= pos の時 std::out_of_range 例外を送出する．
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  virtual
  const PtExpr*
  index(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < index_num() )
  ) const = 0;

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


public:
  //////////////////////////////////////////////////////////////////////
  // 定数グループの関数
  //////////////////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////////////////////
/// @class PtPart PtDecl.h <ym/pt/PtDecl.h>
/// @ingroup VlParser
/// @ingroup PtGroup
/// @brief 範囲指定を表すクラス
///
/// - Const: [a:b]
/// - Plust: [a+:b]
/// - Minus: [a-:b]
//////////////////////////////////////////////////////////////////////
class PtPart :
  public PtBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtRange の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  virtual
  VpiRangeMode
  mode() const = 0;

  /// @brief 1番目の式を取り出す．
  virtual
  const PtExpr*
  left() const = 0;

  /// @brief 2番めの式を取り出す．
  virtual
  const PtExpr*
  right() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_PT_PTEXPR_H
