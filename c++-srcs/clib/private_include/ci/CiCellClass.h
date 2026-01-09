#ifndef CICELLCLASS_H
#define CICELLCLASS_H

/// @file CiCellClass.h
/// @brief CiCellClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ClibIOMap.h"
#include "ym/ClibSeqAttr.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellLibrary;
class CiCellGroup;
class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiCellClass CiCellClass.h "CiCellClass.h"
/// @brief ClibCellClass の実装クラス
//////////////////////////////////////////////////////////////////////
class CiCellClass
{
public:

  /// @brief 空のコンストラクタ
  CiCellClass() = default;

  /// @brief コンストラクタ
  CiCellClass(
    CiCellLibrary* lib,                      ///< [in] 親のセルライブラリ
    ClibCellType cell_type,                  ///< [in] セルの種類
    ClibSeqAttr seq_attr,                    ///< [in] 順序セルの属性
    const std::vector<ClibIOMap>& idmap_list ///< [in] 同位体変換リスト
  ): mLibrary{lib},
     mCellType{cell_type},
     mSeqAttr{seq_attr},
     mIdMapList{idmap_list}
  {
  }

  /// @brief デストラクタ
  ~CiCellClass() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 一般的な情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のライブラリオブジェクトを返す．
  const CiCellLibrary*
  library() const
  {
    return mLibrary;
  }

  /// @brief セルの種類を返す．
  ClibCellType
  cell_type() const
  {
    return mCellType;
  }

  /// @brief 順序セルタイプの場合の属性を返す．
  ClibSeqAttr
  seq_attr() const
  {
    return mSeqAttr;
  }

  /// @brief 同位体変換の個数を得る．
  /// @note 恒等変換は含まない．
  SizeType
  idmap_num() const
  {
    return mIdMapList.size();
  }

  /// @brief 同位体変換を得る．
  const ClibIOMap&
  idmap(
    SizeType pos
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < idmap_num() );
    return mIdMapList[pos];
  }

  /// @brief 同位体変換のリストを得る．
  const std::vector<ClibIOMap>&
  idmap_list() const
  {
    return mIdMapList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに属しているセルグループの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数を返す．
  SizeType
  cell_group_num() const
  {
    return mGroupList.size();
  }

  /// @brief グループを返す．
  const CiCellGroup*
  cell_group(
    SizeType pos ///< [in] インデックス ( 0 <= pos < cell_group_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < cell_group_num() );
    return mGroupList[pos];
  }

  /// @brief グループのリストを返す．
  const std::vector<const CiCellGroup*>&
  cell_group_list() const
  {
    return mGroupList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をシリアライズする．
  void
  serialize(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容を読み込む．
  static
  std::unique_ptr<CiCellClass>
  restore(
    Deserializer& s ///< [in] デシリアライザ
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のライブラリオブジェクトを設定する．
  void
  set_library(
    CiCellLibrary* lib
  )
  {
    mLibrary = lib;
  }

  /// @brief このクラスに属しているセルグループを追加する．
  void
  add_group(
    const CiCellGroup* group
  )
  {
    mGroupList.push_back(group);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief restore() の本体
  void
  _restore(
    Deserializer& s
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のライブラリオブジェクト
  CiCellLibrary* mLibrary;

  // セルの種類
  ClibCellType mCellType;

  // 順序セルの属性
  ClibSeqAttr mSeqAttr;

  // 同位体変換のリスト
  std::vector<ClibIOMap> mIdMapList;

  // セルグループのリスト
  std::vector<const CiCellGroup*> mGroupList;

};

END_NAMESPACE_YM_CLIB

#endif // CICELLCLASS_H
