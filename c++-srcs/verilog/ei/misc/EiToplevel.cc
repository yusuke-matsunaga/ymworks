
/// @file EiToplevel.cc
/// @brief EiToplevel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiToplevel.h"

#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief トップレベルのスコープを生成する
const VlScope*
ElbMgr::new_Toplevel()
{
  auto scope = new EiToplevel;
  reg_obj(scope);
  mTopLevel = scope;
  return scope;
}


//////////////////////////////////////////////////////////////////////
// elaboration 中の toplevel scope を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置の取得
FileRegion
EiToplevel::file_region() const
{
  return FileRegion();
}

/// @brief このオブジェクトの属している module を返す．
const VlModule*
EiToplevel::parent_module() const
{
  return nullptr;
}

// @brief 名前の取得
std::string
EiToplevel::name() const
{
  return "";
}

// @brief fullname の取得
std::string
EiToplevel::full_name() const
{
  return "";
}

END_NAMESPACE_YM_VERILOG
