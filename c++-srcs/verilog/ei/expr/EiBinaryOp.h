#ifndef EIBINARYOP_H
#define EIBINARYOP_H

/// @file EiBinaryOp.h
/// @brief EiExpr の2項演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

//////////////////////////////////////////////////////////////////////
/// expression を表すクラス
/// IEEE Std 1364-2001 26.6.25 Simple expressions
/// IEEE Std 1364-2001 26.6.26 Expressions
///
/// operation の分類
/// オペランドの数
///
/// - 2個
///   - 返り値 scalar, オペランド any
///     - EqOp, NeqOp, CaseEqOp, CaseNeqOp, GtOp, GeOp, LtOp, LeOp
///   - 返り値 scalar, オペランド scalar
///     - LogAndOp, LogOrOp
///   - 返り値 オペランドと同一, オペランド bitvector
///     - BitAndOp, BitOrOp, BitXorOp, BitXNorOp
///   - 返り値 オペランドと同一, オペランド any
///     - AddOp, SubOp, MultOp, DivOp, ModOp
///     - PowerOp
///   - 返り値 第1オペランドと同一, オペランド any
///     - LShiftOp, RShiftOp, ArithLShiftOp, ArithRShiftOp
//////////////////////////////////////////////////////////////////////

#include "EiOperation.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBinaryOp EiBinaryOp.h "EiBinaryOp.h"
/// @brief 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class EiBinaryOp :
  public EiOperation
{
public:

  /// @brief コンストラクタ
  EiBinaryOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,           ///< [in] オペランド1
    ElbExpr* opr2            ///< [in] オペランド2
  ) : EiOperation(ast_expr),
    mOpr{opr1, opr2}
  {
  }

  /// @brief デストラクタ
  ~EiBinaryOp() = default;


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
  ElbExpr* mOpr[2];

};

END_NAMESPACE_YM_VERILOG

#endif // EIBINARYOP_H
