
/// @file EiDelay.cc
/// @brief EiDelay の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiDelay.h"

#include "elaborator/ElbExpr.h"

#include "ym/vl/VlExpr.h"
#include "ym/pt/PtBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 遅延値を生成する．
const VlDelay*
EiFactory::new_Delay(
  const PtBase* pt_obj,
  const std::vector<ElbExpr*>& expr_list
)
{
  return new EiDelay(pt_obj, expr_list);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDelay
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiDelay::EiDelay(
  const PtBase* pt_obj,
  const std::vector<ElbExpr*>& expr_list
) : mPtObj{pt_obj},
    mElemList{expr_list}
{
}

// @brief デストラクタ
EiDelay::~EiDelay()
{
}

// @brief 型の取得
VpiObjType
EiDelay::type() const
{
  // 嘘
  return VpiObjType::Operation;
}

// @brief ファイル位置を返す．
FileRegion
EiDelay::file_region() const
{
  return mPtObj->file_region();
}

// @brief 要素数を返す．
SizeType
EiDelay::elem_num() const
{
  return mElemList.size();
}

// @brief 値を返す．
const VlExpr*
EiDelay::expr(
  SizeType pos
) const
{
  if ( pos >= elem_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mElemList[pos];
}

// @brief 値のリストを返す．
std::vector<const VlExpr*>
EiDelay::expr_list() const
{
  return std::vector<const VlExpr*>{mElemList.begin(), mElemList.end()};
}

// @brief Verilog-HDL の文字列を得る．
std::string
EiDelay::decompile() const
{
  std::string ans = "(";
  const char* comma = "";
  for ( const auto& expr: mElemList ) {
    ans += comma;
    ans += expr->decompile();
    comma = ", ";
  }
  ans += ")";
  return ans;
}

END_NAMESPACE_YM_VERILOG
