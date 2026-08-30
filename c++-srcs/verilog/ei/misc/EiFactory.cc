
/// @file EiFactory.cc
/// @brief EiFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief オブジェクトを生成する関数
ElbFactory*
ElbFactory::new_obj()
{
  return new EiFactory{};
}


//////////////////////////////////////////////////////////////////////
// クラス EiFactory
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiFactory::EiFactory()
{
}

// @brief デストラクタ
EiFactory::~EiFactory()
{
}

// @brief 内訳を表示する．
void
EiFactory::dump_prof(
  std::ostream& s
)
{
  // 未完
}

END_NAMESPACE_YM_VERILOG
