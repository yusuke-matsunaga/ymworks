
/// @file LexPluginDict.cc
/// @brief LexPlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "LexPluginDict.h"
#include "LexPlugin.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

// ハッシュ関数
inline
SizeType
hash_func(
  const std::string& str
)
{
  SizeType h = 0;
  for ( auto c: str ) {
    h = h * 37 + c;
  }
  return h;
}

END_NONAMESPACE


// @brief プラグインを登録できるか調べる．
bool
LexPluginDict::check_name(
  const char* name
)
{
  auto p = find_plugin(name);
  if ( p == nullptr || p->is_macro() ) {
    return true;
  }
  return false;
}

// @brief プラグインを登録する．
void
LexPluginDict::reg_plugin(
  LexPlugin* plugin
)
{
  auto name = plugin->name();
  auto hash_value = hash_func(name);
  auto pos = hash_value % mTableSize;
  LexPlugin* p;
  for ( LexPlugin** prev = &mHashTable[pos]; (p = *prev); ) {
    if ( p->name() == name ) {
      if ( p->is_macro() ) {
	// 古いプラグイン(マクロ)を削除する．
	*prev = plugin;
	plugin->mLink = p->mLink;
	delete p;
	return;
      }
      else {
	// コンパイラディレクティブは上書きできない．
	ASSERT_NOT_REACHED;
      }
    }
    prev = &p->mLink;
  }

  if ( mNum >= mNextLimit ) {
    LexPlugin** old_table = mHashTable;
    auto old_size = mTableSize;
    new_table(mTableSize << 1);
    for ( SizeType i = 0; i < old_size; ++ i) {
      for ( LexPlugin* p = old_table[i]; p; ) {
	LexPlugin* next = p->mLink;
	auto pos1 = hash_func(p->name()) % mTableSize;
	p->mLink = mHashTable[pos1];
	mHashTable[pos1] = p;
	p = next;
      }
    }
    delete [] old_table;
    pos = hash_value % mTableSize;
  }

  plugin->mLink = mHashTable[pos];
  mHashTable[pos] = plugin;
  ++ mNum;
  return;
}

// @brief プラグインの登録を解除する．
bool
LexPluginDict::unreg_plugin(
  const char* name
)
{
  auto hash_value = hash_func(name);
  auto pos = hash_value % mTableSize;
  LexPlugin** prev = &mHashTable[pos];
  for (LexPlugin* p = *prev; p; ) {
    if ( p->name() == name ) {
      *prev = p->mLink;
      delete p;
      -- mNum;
      return true;
    }
    prev = &p->mLink;
  }
  return false;
}

// @brief プラグインを検索する．
LexPlugin*
LexPluginDict::find_plugin(
  const char* name
) const
{
  auto hash_value = hash_func(name);
  auto pos = hash_value & mHashMask;
  for (LexPlugin* p = mHashTable[pos]; p; p = p->mLink) {
    if ( p->name() == name ) {
      return p;
    }
  }
  return nullptr;
}

// @brief ハッシュ表を確保する．
void
LexPluginDict::new_table(
  SizeType size
)
{
  mTableSize = size;
  mHashMask = mTableSize - 1;
  mNextLimit = static_cast<SizeType>(mTableSize * 1.8);
  mHashTable = new LexPlugin*[mTableSize];
  for ( SizeType i = 0; i < mTableSize; ++ i) {
    mHashTable[i] = nullptr;
  }
}

END_NAMESPACE_YM_VERILOG
