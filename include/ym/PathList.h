#ifndef YM_PATHLIST_H
#define YM_PATHLIST_H

/// @file ym/PathList.h
/// @brief PathList の定義ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <filesystem>


BEGIN_NAMESPACE_YM

using Path = std::filesystem::path;

//////////////////////////////////////////////////////////////////////
/// @class PathList PathList.h "ym/PathList.h"
/// @ingroup YmUtils
/// @brief パスのリストを表すクラス
//////////////////////////////////////////////////////////////////////
class PathList
{
public:

  /// @brief コンストラクタ
  ///
  /// 空のリストを作る．
  PathList() = default;

  /// @brief パスのリストからの変換コンストラクタ
  PathList(
    const std::vector<Path>& path_list
  ) : mPathList{path_list.begin(), path_list.end()}
  {
  }

  /// @brief 文字列からの変換コンストラクタ
  ///
  /// 文字列は ':' で区切られた文字列で '.'
  /// はカレントディレクトリを表す．
  /// また，末尾が '/' で終わっている場合にはそのサブディレクトリ
  /// も探索する．
  PathList(
    const std::string& src_str ///< [in] サーチパスを表す文字列
  ) : PathList(to_list(src_str))
  {
  }

  /// @brief コピーコンストラクタ
  PathList(
    const PathList& src ///< [in] 代入元のオブジェクト
  ) = default;

  /// @brief 代入演算子
  PathList&
  operator=(
    const PathList& src ///< [in] 代入元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~PathList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 以前の内容をクリアして文字列をセットする．
  ///
  /// サーチパス文字列は ':' で区切られた文字列で '.'
  /// はカレントディレクトリを表す．
  /// また，末尾が '/' で終わっている場合にはそのサブディレクトリ
  /// も探索する．
  void
  set_from_string(
    const std::string& str ///< [in] セットするサーチパスを表す文字列
  )
  {
    auto vec = to_list(str);
    mPathList.clear();
    mPathList.insert(mPathList.end(), vec.begin(), vec.end());
  }

  /// @brief パスの先頭にを追加する．
  void
  add_top(
    const Path& path ///< [in] 追加するパス
  )
  {
    mPathList.insert(mPathList.begin(), path);
  }

  /// @brief パスの戦闘に追加する．
  void
  add_top(
    const PathList& path_list ///< [in] 追加するパスのリスト
  )
  {
    mPathList.insert(mPathList.begin(),
		     path_list.mPathList.begin(), path_list.mPathList.end());
  }

  /// @brief パスの末尾にを追加する．
  void
  add_end(
    const Path& path ///< [in] 追加するパス
  )
  {
    mPathList.push_back(path);
  }

  /// @brief パスの末尾にを追加する．
  void
  add_end(
    const PathList& path_list ///< [in] 追加するパスのリスト
  )
  {
    mPathList.insert(mPathList.end(),
		     path_list.mPathList.begin(), path_list.mPathList.end());
  }

  /// @brief パスリストを考慮して filename を探す
  /// @retval 最初に見つかったファイルの full-path
  /// @retval 空のパス名 見つからない場合
  ///
  /// サーチパスが空ならカレントディレクトリで filename を探す．
  Path
  search(
    const std::string& filename ///< [in] ファイル名
  ) const
  {
    return search(Path{filename});
  }

  /// @brief パスリストを考慮して filename を探す
  /// @retval 最初に見つかったファイルの full-path
  /// @retval 空のパス名 見つからない場合
  ///
  /// サーチパスが空ならカレントディレクトリで filename を探す．
  Path
  search(
    const Path& filename ///< [in] ファイル名
  ) const;

  /// @brief 現在のサーチパスを取り出す．
  /// @return サーチパスを文字列に変換したもの
  std::string
  to_string(
    const std::string& separator = ":" ///< [in] 区切り文字
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 文字列を PathName のリストに変換する
  static
  std::vector<Path>
  to_list(
    const std::string& str
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のサーチパスのリスト
  std::deque<Path> mPathList;

};

END_NAMESPACE_YM

#endif // YM_PATHLIST_H
