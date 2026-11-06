#ifndef EIUNARYOP_H
#define EIUNARYOP_H

/// @file EiUnaryOp.h
/// @brief EiUnaryOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

//////////////////////////////////////////////////////////////////////
// expression を表すクラス
// IEEE Std 1364-2001 26.6.25 Simple expressions
// IEEE Std 1364-2001 26.6.26 Expressions
//
// operation の分類
// オペランドの数
//
// - 1個
//   - 返り値 scalar, オペランド scalar
//     - NotOp
//   - 返り値 scalar, オペランド bitvector
//     - UnaryAndOp, UnaryNandOp, UnaryOrOp, UnaryNorOp,
//       UnaryXorOp, UnaryXNorOp
//   - 返り値 オペランドと同一, オペランド bitvector
//     - BitNegOp
//   - 返り値 オペランドと同一, オペランド any
//     - PlusOp, MinusOp
//   - 返り値 none, オペランド none
//     - PosedgeOp, NegedgeOp
//////////////////////////////////////////////////////////////////////

#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiUnaryOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief 単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiUnaryOp :
  public EiOperation
{
public:

  /// @brief コンストラクタ
  EiUnaryOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1            ///< [in] オペランド
  );

  /// @brief デストラクタ
  ~EiUnaryOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の時 true を返す．
  bool
  is_const() const override;

  /// @brief オペランド数を返す．
  SizeType
  operand_num() const override;

  /// @brief オペランドを返す．
  const VlExpr*
  operand(
    SizeType pos
  ) const override;

  /// @brief オペランドのリストを返す．
  std::vector<const VlExpr*>
  operand_list() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  ElbExpr* mOpr1;

};

END_NAMESPACE_YM_VERILOG

#endif // EIUNARYOP_H
