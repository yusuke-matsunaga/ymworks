#ifndef CICELLNAMEHASH_H
#define CICELLNAMEHASH_H

/// @file CiNameHash.h
/// @brief CiNameHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCell;

//////////////////////////////////////////////////////////////////////
/// @class CiNameHash CiNameHash.h "CiNameHash.h"
/// @brief セルと要素名をキーにしたハッシュ表
///
/// 要素はピン，バス，バンドルを仮定している．
//////////////////////////////////////////////////////////////////////
template<class T>
class CiCellNameHash
{
public:

  /// @brief コンストラクタ
  CiCellNameHash() = default;

  /// @brief デストラクタ
  ~CiCellNameHash() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear()
  {
    mDict.clear();
  }

  /// @brief 要素を追加する．
  void
  add(
    const CiCell* cell, ///< [in] 親のセル
    ShString name,      ///< [in] 名前
    const T* elem       ///< [in] 登録する要素
  )
  {
    Key key{cell, name};
    mDict.emplace(key, elem);
  }

  /// @brief 要素を取り出す．
  /// @return cell の name という要素を返す．
  ///
  /// なければ nullptr を返す．
  const T*
  get(
    const CiCell* cell, ///< [in] ピンの親のセル
    ShString name       ///< [in] 名前
  ) const
  {
    Key key{cell, name};
    if ( mDict.count(key) > 0 ) {
      return mDict.at(key);
    }
    else {
      return nullptr;
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  struct Key
  {
    const CiCell* mCell;
    ShString mName;

    bool
    operator==(const Key& right) const
    {
      return mCell == right.mCell && mName == right.mName;
    }
  };


  struct Hash
  {
    SizeType
    operator()(
      const Key& key
    ) const
    {
      return reinterpret_cast<SizeType>(key.mCell) + key.mName.hash() * 97;
    }
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  std::unordered_map<Key, const T*, Hash> mDict;

};

END_NAMESPACE_YM_CLIB

#endif // CICELLNAMEHASH_H
