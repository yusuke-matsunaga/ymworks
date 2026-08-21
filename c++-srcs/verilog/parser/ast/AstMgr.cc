
/// @file AstMgr.cc
/// @brief AstMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/AstMgr.h"
#include "parser/PtModule.h"
#include "parser/PtUdp.h"
#include "parser/PtMisc.h"
#include "alloc/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstMgr::AstMgr() :
  mAlloc{Alloc::new_obj()}
{
}

// @brief デストラクタ
AstMgr::~AstMgr()
{
  clear();
}

// @brief 登録されているモジュールのリストを返す．
const std::vector<const AstModule*>&
AstMgr::module_list() const
{
  return mModuleList;
}

// @brief 登録されている UDP のリストを返す．
const std::vector<const AstUdp*>&
AstMgr::udp_list() const
{
  return mUdpList;
}

// @brief インスタンス記述で用いられている名前かどうか調べる．
bool
AstMgr::check_def_name(
  const std::string& name
) const
{
  return mDefNames.count(name) > 0;
}

// @brief attribute instance を取り出す．
AstAttrInstVec
AstMgr::find_attr_list(
  const AstBase* obj
) const
{
  PtAttrInfo key(obj, {});
  if ( mAttrDict.count(key) > 0 ) {
    const auto& attr_info = mAttrDict.find(key);
    return attr_info->attr_list()->attrinst_list();
  }
  return {};
}

// @brief 全ての属性リストのリストを返す．
std::vector<PtAttrInfo>
AstMgr::all_attr_list() const
{
  std::vector<PtAttrInfo> ans;
  ans.reserve(mAttrDict.size());
  for ( const auto& ai: mAttrDict ) {
    ans.push_back(ai);
  }
  return ans;
}

// @brief 今までに生成したインスタンスをすべて破壊する．
void
AstMgr::clear()
{
  mUdpList.clear();
  mModuleList.clear();
  mDefNames.clear();
  mStringPool.clear();

  FileInfo::clear();
  mAlloc->destroy();
}

// UDP の登録
void
AstMgr::reg_udp(
  const AstUdp* udp
)
{
  mUdpList.push_back(udp);
}

// モジュールの登録
void
AstMgr::reg_module(
  const AstModule* module
)
{
  mModuleList.push_back(module);
}

// @brief インスタンス定義名を追加する．
void
AstMgr::reg_defname(
  const std::string& name
)
{
  mDefNames.insert(name);
}

// @brief attribute instance を登録する．
void
AstMgr::reg_attrinst(
  const AstBase* obj,
  PtAttrInstList* ai_list,
  bool def
)
{
  if ( ai_list ) {
    mAttrDict.emplace(PtAttrInfo(obj, ai_list, def));
  }
}

// @brief 文字列領域を確保する．
const char*
AstMgr::save_string(
  const std::string& str
)
{
  auto p = mStringPool.find(str);
  if ( p == mStringPool.end() ) {
    // str と同じ内容は登録されていなかった．
    // 新しい領域を確保して登録する．
    mStringPool.insert(str);
  }
  p = mStringPool.find(str);
  ASSERT_COND( p != mStringPool.end() );
  return (*p).c_str();
}

// @brief メモリアロケーターを返す．
Alloc&
AstMgr::alloc()
{
  return *mAlloc;
}

END_NAMESPACE_YM_VERILOG
