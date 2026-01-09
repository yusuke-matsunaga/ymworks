#ifndef ENGINE_NAIVE_H
#define ENGINE_NAIVE_H

/// @file Engine_Naive.h
/// @brief Engine_Naive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EngineBase.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class Engine_Naive Engine_Naive.h "Engine_Naive.h"
/// @brief 通常の ExSynEngine
//////////////////////////////////////////////////////////////////////
class Engine_Naive:
  public EngineBase
{
public:

  /// @brief コンストラクタ
  Engine_Naive(
    const TruthTable& tt,   ///< [in] 対象の論理関数
    SizeType op_num,        ///< [in] 演算ノード数
    SizeType max_input,     ///< [in] 演算の最大入力数
    const JsonValue& option ///< [in] オプション
  );

  /// @brief デストラクタ
  ~Engine_Naive();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // EngineBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算ノードと出力の制約を作る．
  void
  make_op_list() override;

  /// @brief use_all_inputs 制約を作る．
  void
  add_use_all_inputs() override;

  /// @brief use_all_steps 制約を作る．
  void
  add_use_all_steps() override;

  /// @brief no_reapplication 制約を作る．
  void
  add_no_reapplication() override;

  /// @brief op_order 制約を作る．
  void
  add_op_order() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最大入力数
  SizeType mMaxInput;

};

END_NAMESPACE_YM_EXSYN

#endif // ENGINE_NAIVE_H
