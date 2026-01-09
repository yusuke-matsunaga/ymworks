#ifndef EXPRLUGINDICT_H
#define EXPRLUGINDICT_H

/// @file LexPluginDict.h
/// @brief LexPlugin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "LexPlugin.h"

BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class LexPluginDict LexPluginDict.h "LexPluginDict.h"
/// @brief LexPlugin の辞書
///
/// 登録されたプラグインの所有権はこのオブジェクトとなる．
//////////////////////////////////////////////////////////////////////
class LexPluginDict
{
public:

  /// @brief コンストラクタ
  LexPluginDict() = default;

  /// @brief デストラクタ
  ///
  /// 登録されているプラグインはすべて削除される．
  ~LexPluginDict() = default;


public:

  /// @brief プラグインを登録できるか調べる．
  /// @return true 登録可能
  /// @return false 同名の定義済みプラグインがあり，登録不可能
  bool
  check_name(
    const std::string& name ///< [in] プラグイン名
  ) const
  {
    if ( mDict.count(name) == 0 ) {
      return true;
    }
    auto& plugin = mDict.at(name);
    if ( plugin->is_macro() ) {
      // マクロなら再定義可能
      return true;
    }
    return false;
  }

  /// @brief プラグインを登録する．
  void
  reg_plugin(
    const std::string& name, ///< [in] 名前
    LexPlugin* plugin        ///< [in] プラグイン
  )
  {
    if ( mDict.count(name) == 0 ) {
      mDict.emplace(name, std::unique_ptr<LexPlugin>{plugin});
    }
    auto& old_plugin = mDict.at(name);
    if ( old_plugin->is_macro() ) {
      // 以前のプラグインを削除して上書きする．
      mDict.erase(name);
      mDict.emplace(name, std::unique_ptr<LexPlugin>{plugin});
    }
    std::ostringstream buf;
    buf << name << " cannot be overwritten";
    throw std::invalid_argument{buf.str()};
  }

  /// @brief プラグインの登録を解除する．
  /// @retval true 指定された名前のプラグインの登録解除が成功した．
  /// @retval false 指定された名前のプラグインは登録されていなかった．
  bool
  unreg_plugin(
    const std::string& name ///< [in] 名前
  )
  {
    if ( mDict.count(name) > 0 ) {
      mDict.erase(name);
      return true;
    }
    return false;
  }

  /// @brief プラグインを検索する．
  /// @return name という名前で登録されているプラグインを返す．
  LexPlugin*
  find_plugin(
    const std::string& name ///< [in] 名前
  ) const
  {
    if ( mDict.count(name) > 0 ) {
      return mDict.at(name).get();
    }
    return nullptr;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ディレクティブ名をキーにしてプラグインを保持する辞書
  std::unordered_map<std::string, std::unique_ptr<LexPlugin>> mDict;

};


END_NAMESPACE_YM_VERILOG

#endif // EXPRLUGINDICT_H
