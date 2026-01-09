#ifndef YM_CLIBPATGRAPH_H
#define YM_CLIBPATGRAPH_H

/// @file ym/ClibPatGraph.h
/// @brief ClibPatGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ClibLibraryPtr.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellLibrary;

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibPatGraph ClibPatGraph.h "ym/ClibPatGraph.h"
/// @brief パタングラフを表すクラス
///
/// このクラスは実装を隠すためにインターフェイスの定義のみとなっている．
//////////////////////////////////////////////////////////////////////
class ClibPatGraph
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正値となる．
  ClibPatGraph() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibPatGraph(
    ClibLibraryPtr lib, ///< [in] 親のライブラリ
    SizeType id         ///< [in] ID番号
  ) : mLibrary{lib},
      mId{id}
  {
  }

  /// @brief デストラクタ
  ~ClibPatGraph() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 代表クラスを返す．
  ClibCellClass
  rep_class() const;

  /// @brief 根のノード番号を返す．
  SizeType
  root_id() const;

  /// @brief 根の反転属性を返す．
  bool
  root_inv() const;

  /// @brief 入力数を返す．
  SizeType
  input_num() const;

  /// @brief 枝数を返す．
  SizeType
  edge_num() const;

  /// @brief 枝(の番号)を返す．
  SizeType
  edge(
    SizeType pos ///< [in] 位置 ( 0 <= pos < edge_num() )
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // mImpl に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mLibrary != nullptr;
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
    const ClibPatGraph& right
  ) const
  {
    return mLibrary == right.mLibrary && mId == right.mId;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    const ClibPatGraph& right
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のライブラリ
  ClibLibraryPtr mLibrary;

  // ID番号
  SizeType mId{CLIB_NULLID};

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBPATGRAPH_H
