#ifndef ENGINE_PD_H
#define ENGINE_PD_H

/// @file Engine_PD.h
/// @brief Engine_PD のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EngineBase.h"
#include "PD.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class Engine_PD Engine_PD.h "Engine_PD.h"
/// @brief 通常の ExSynEngine
//////////////////////////////////////////////////////////////////////
class Engine_PD:
  public EngineBase
{
public:

  /// @brief コンストラクタ
  Engine_PD(
    const TruthTable& tt,   ///< [in] 対象の論理関数
    const PD& pd,           ///< [in] Partial DAG
    const JsonValue& option ///< [in] オプション
  );

  /// @brief デストラクタ
  ~Engine_PD();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // EnginBase の仮想関数
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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  gen_op_list_SSV();

  void
  gen_op_list_MSV();

  void
  gen_op_list_OSV();

  void
  gen_op_list_SMART();

  /// @brief add_op_order_cond() の下請け関数
  void
  add_op_order_sub(
    const std::vector<SizeType>& group ///< [in] グループ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Partial DAG
  const PD& mPD;

};

END_NAMESPACE_YM_EXSYN

#endif // ENGINE_PD_H
