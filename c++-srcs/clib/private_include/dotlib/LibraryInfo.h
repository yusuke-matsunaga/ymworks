#ifndef LIBRARYINFO_H
#define LIBRARYINFO_H

/// @file LibraryInfo.h
/// @brief LibraryInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupInfo.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LibraryInfo LibraryInfo.h "LibraryInfo.h"
/// @brief ライブラリのパース情報
//////////////////////////////////////////////////////////////////////
class LibraryInfo :
  public GroupInfo
{
public:

  /// @brief コンストラクタ
  LibraryInfo(
    CiCellLibrary* library ///< [in] 対象のライブラリ
  ) : mLibrary{library}
  {
  }

  /// @brief デストラクタ
  ~LibraryInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(
    const AstValue& lib_val ///< [in] ライブラリ情報のパース木
  );

  /// @brief ライブラリを取り出す．
  CiCellLibrary*
  library()
  {
    return mLibrary;
  }

  /// @brief LUTテンプレートを取り出す．
  const CiLutTemplate*
  find_lut(
    const ShString& name ///< [in] テンプレート名
  )
  {
    if ( mLutDict.count(name) > 0 ) {
      return mLutDict.at(name);
    }
    return nullptr;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief technology の属性をセットする．
  void
  set_technology();

  /// @brief delay_model の属性をセットする．
  void
  set_delay_model();

  /// @brief pice_type, piece_define の属性をセットする．
  void
  set_piece_params();

  /// @brief 文字列型の属性をセットする．
  void
  set_str_attr(
    const char* keyword ///< [in] キーワード
  );

  /// @brief capacitive_load_unit の属性をセットする．
  void
  set_capacitive_load_unit();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ライブラリ
  CiCellLibrary* mLibrary{nullptr};

  // 名前をキーにしてLUTテンプレートを格納する辞書
  std::unordered_map<ShString, const CiLutTemplate*> mLutDict;

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBRARYINFO_H
