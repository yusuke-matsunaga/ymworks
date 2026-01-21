#ifndef YM_CLIBCELLCLASS_H
#define YM_CLIBCELLCLASS_H

/// @file ym/ClibCellClass.h
/// @brief ClibCellClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibHandle.h"
#include "ym/ClibList.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellClass;

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibCellClass ClibCellClass.h "ym/ClibCellClass.h"
/// @brief NPN同値なセルグループの集合を表すクラス
//////////////////////////////////////////////////////////////////////
class ClibCellClass
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// - 不正値となる．
  ClibCellClass() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibCellClass(
    const CiCellClass* impl ///< [in] 本体
  );

  /// @brief デストラクタ
  ~ClibCellClass();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 一般的な情報を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 同位体変換の個数を得る．
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - 恒等変換は含まない．
  SizeType
  idmap_num() const;

  /// @brief 同位体変換を得る．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibIOMap
  const ClibIOMap&
  idmap(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < idmap_num() )
  ) const;

  /// @brief 同位体変換のリストを得る．
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibIOMap
  const std::vector<ClibIOMap>&
  idmap_list() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name このクラスに属しているセルグループの情報を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数を返す．
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  cell_group_num() const;

  /// @brief グループを返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibCellGroup
  ClibCellGroup
  cell_group(
    SizeType pos ///< [in] インデックス ( 0 <= pos < cell_group_num() )
  ) const;

  /// @brief グループのリストを返す．
  /// @exception std::logic_error is_valid() = false の場合
  /// @sa ClibCellGroupList
  ClibCellGroupList
  cell_group_list() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // mImpl に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mImpl != nullptr;
  }

  /// @brief 不正値の時 true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 等価比較
  bool
  operator==(
    const ClibCellClass& right
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    const ClibCellClass& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 適正な値を持っているかチェックする．
  void
  _check_valid() const
  {
    if ( !is_valid() ) {
      throw std::invalid_argument{"not having a valid data"};
    }
  }

  /// @brief ハッシュ用のキー
  SizeType
  key() const
  {
    return reinterpret_cast<SizeType>(mImpl);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装
  const CiCellClass* mImpl;

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCELLCLASS_H
