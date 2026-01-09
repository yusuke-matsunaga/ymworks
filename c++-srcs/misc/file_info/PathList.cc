
/// @file PathList.cc
/// @brief PathList の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/PathList.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PathList
//////////////////////////////////////////////////////////////////////

// サーチパスを考慮して filename を探す
Path
PathList::search(
  const Path& filename
) const
{
  if ( filename.is_absolute() ) {
    // 絶対パスならそのままファイルの状態を取得する．
    auto status = std::filesystem::status(filename);
    if ( status.type() == std::filesystem::file_type::regular ) {
      return filename;
    }
  }
  else {
    // 相対パスの場合
    if ( mPathList.empty() ) {
      // サーチパスが空の場合，カレントディレクトリからの相対パスを試す．
      auto status = std::filesystem::status(filename);
      if ( status.type() == std::filesystem::file_type::regular ) {
	return std::filesystem::canonical(filename);
      }
    }
    else {
      // サーチパスを頭につけて試す．
      for ( auto path: mPathList ) {
	auto tmp = path / filename;
	auto status = std::filesystem::status(tmp);
	if ( status.type() == std::filesystem::file_type::regular ) {
	  return std::filesystem::canonical(tmp);
	}
      }
    }
  }
  // 見つからなかった
  return std::filesystem::path();
}

// 現在のサーチパスを取り出す．
std::string
PathList::to_string(
  const std::string& separator
) const
{
  std::string ans;
  std::string sep = "";
  for ( auto path: mPathList ) {
    ans += sep;
    ans += path.string();
    sep = separator;
  }
  return ans;
}

// 文字列を PathName のリストに変換する
std::vector<Path>
PathList::to_list(
  const std::string& str
)
{
  std::vector<Path> ans_list;
  std::string::size_type pos1 = 0;
  std::string::size_type pos2;
  for ( ; (pos2 = str.find(':', pos1)) != std::string::npos; pos1 = pos2 + 1) {
    ans_list.push_back(Path(str.substr(pos1, pos2)));
  }
  ans_list.push_back(Path(str.substr(pos1, std::string::npos)));
  return ans_list;
}

END_NAMESPACE_YM
