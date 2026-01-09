#ifndef YM_CLIBCELLGROUP_H
#define YM_CLIBCELLGROUP_H

/// @file ym/ClibCellGroup.h
/// @brief ClibCellGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibHandle.h"
#include "ym/ClibList.h"
#include "ym/logic.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellGroup;

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibCellGroup ClibCellGroup.h "ym/ClibCellGroup.h"
/// @brief 論理的に等価なセルのグループを表すクラス
///
/// - 組み合わせ論理タイプ
///   各出力の論理関数と tristate 条件を表す論理関数を持つ．
///
/// - FFタイプ
///   single-stage, master-slave の区別，
///   非同期セット・クリアの有無，
///   リセット状態(セット・リセットの両方がアクティブ)
///   で区別されるFFコアの周囲の論理関数を持つ．
///
/// - latchタイプ
///   single-stage, master-slave の区別，
///   非同期セット・クリアの有無，
///   リセット状態(セット・リセットの両方がアクティブ)
///   で区別されるlatchコアの周囲の論理関数を持つ．
///
/// - fsmタイプ
///   statetableをコアとして，周囲の論理関数を持つ．
///
/// すべてのタイプで用いられる論理関数は入力ピンと入出力ピンをサポート
/// とする．FFセルの場合には内部ノードも使用可能
//////////////////////////////////////////////////////////////////////
class ClibCellGroup
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正値となる．
  ClibCellGroup() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibCellGroup(
    const CiCellGroup* impl ///< [in] 本体
  );

  /// @brief デストラクタ
  ~ClibCellGroup();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 一般的な情報を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 代表クラスを返す．
  ClibCellClass
  rep_class() const;

  /// @brief 代表クラスに対する変換マップを返す．
  const ClibIOMap&
  iomap() const;

  /// @brief セルの種類を返す．
  ClibCellType
  cell_type() const;

  /// @brief 順序セルタイプの場合の属性を返す．
  ClibSeqAttr
  seq_attr() const;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name このグループに属しているセルの情報を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief セル数を返す．
  SizeType
  cell_num() const;

  /// @brief セルを返す．
  ClibCell
  cell(
    SizeType pos ///< [in] インデックス ( 0 <= pos < cell_num() )
  ) const;

  /// @brief セルのリストを返す．
  ClibCellList
  cell_list() const;

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
    const ClibCellGroup& right
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    const ClibCellGroup& right
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
  const CiCellGroup* mImpl;

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCELLGROUP_H
