
/// @file AstMgr.cc
/// @brief AstMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/AstMgr.h"
#include "parser/PtModule.h"
#include "parser/PtUdp.h"
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
const std::vector<AstModule>&
AstMgr::module_list() const
{
  return mModuleList;
}

// @brief 登録されている UDP のリストを返す．
const std::vector<AstUdp>&
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
  const AstBase& ast_obj
) const
{
  auto key = ast_obj.key();
  PtAttrInfo info(key, {});
  if ( mAttrDict.count(info) > 0 ) {
    const auto& attr_info = mAttrDict.find(info);
    return attr_info->attr_list().to_vector();
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

// @brief 内容を JsonValue に変換する．
JsonValue
AstMgr::json_obj() const
{
  auto jobj = JsonValue::object();
  auto module_array = JsonValue::array();
  for ( auto module: module_list() ) {
    auto jobj1 = module.json_obj();
    module_array.add(jobj1);
  }
  jobj.add("module_list", module_array);
  auto udp_array = JsonValue::array();
  for ( auto udp: udp_list() ) {
    auto jobj1 = udp.json_obj();
    udp_array.add(jobj1);
  }
  jobj.add("udp_list", udp_array);
  return jobj;
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
  const AstUdp& udp
)
{
  mUdpList.push_back(udp);
}

// モジュールの登録
void
AstMgr::reg_module(
  const AstModule& module
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
  PtrIntType ptr,
  const AstAttrInstList& ai_list,
  bool def
)
{
  if ( ptr != 0 && ai_list.size() > 0 ) {
    mAttrDict.emplace(PtAttrInfo(ptr, ai_list, def));
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
