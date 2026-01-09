#ifndef ELEMINFO_H
#define ELEMINFO_H

/// @file ElemInfo.h
/// @brief ElemInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupInfo.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_DOTLIB

class LibraryInfo;

//////////////////////////////////////////////////////////////////////
/// @class ElemInfo ElemInfo.h "ElemInfo.h"
/// @brief グループ構造を表すクラス
//////////////////////////////////////////////////////////////////////
class ElemInfo :
  public GroupInfo
{
public:

  /// @brief コンストラクタ
  ElemInfo(
    LibraryInfo& library_info ///< [in] ライブラリのパース情報
  ) : mLibraryInfo{library_info}
  {
  }

  /// @brief デストラクタ
  ~ElemInfo() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ライブラリを取り出す．
  CiCellLibrary*
  library() const;

  /// @brief ライブラリの情報を返す．
  LibraryInfo&
  library_info() const
  {
    return mLibraryInfo;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ライブラリのパース情報
  LibraryInfo& mLibraryInfo;

};

END_NAMESPACE_YM_DOTLIB

#endif // ELEMINFO_H
