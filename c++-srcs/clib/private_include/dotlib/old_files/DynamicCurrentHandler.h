#ifndef DYNAMICCURRENTHANDLER_H
#define DYNAMICCURRENTHANDLER_H

/// @file DynamicCurrentHandler.h
/// @brief DynamicCurrentHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DynamicCurrentHandler DynamicCurrentHandler.h "DynamicCurrentHandler.h"
/// @brief 'dynamic_current' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
///
/// * マニュアル中の記述で '"' が要るのか要らないのか不明．
///   たぶん，related_inputs, related_outputs に複数のピンがある時は "
///   が要るはず．
/// * typical_capacitance 中の要素の区切りに ',' が要るのか要らないのかもわからない．
/// * Simple Attributes
///   - related_inputs : "
///   - related_outputs :
///   - when : "Boolean expression" ;
/// * Complex Attributes
///   - typical_capacitance ( "<float>, ..." ) ;
/// * Group Statements
///   - switching_group
//////////////////////////////////////////////////////////////////////
class DynamicCurrentHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  DynamicCurrentHandler();

  /// @brief デストラクタ
  ~DynamicCurrentHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // related_inputs
  const AstString* mRelatedInputs;

  // related_outputs
  const AstString* mRelatedOutputs;

  // when
  const AstExpr* mWhen;

  // typical_capacitances
  const AstFloatVector* mTypicalCapacitances;

  // switching_group
  const AstSwitchingGroup* mSwitchingGroup;

};

END_NAMESPACE_YM_DOTLIB

#endif // DYNAMICCURRENTHANDLER_H
