#ifndef IDPOOL_H
#define IDPOOL_H

/// @file IdPool.h
/// @brief IdPool のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <mutex>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class IdPool IdPool.h "ym/IdPool.h"
/// @brief マルチスレッド環境でユニークがIDを得るための機構
//////////////////////////////////////////////////////////////////////
class IdPool
{
public:

  /// @brief コンストラクタ
  IdPool(
    SizeType id_num ///< [in] IDの要素数
  ) : mIdNum{id_num},
      mCurId{0}
  {
  }

  /// @brief デストラクタ
  ~IdPool() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief データが取り出す．
  /// @return データがある時 true を返す．
  bool
  get(
    SizeType& id ///< [out] 取り出したIDを格納する変数
  )
  {
    std::unique_lock lock{mMtx};
    if ( mCurId >= mIdNum ) {
      return false;
    }
    id = mCurId;
    ++ mCurId;
    return true;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mIdNum;

  // 現在のID番号
  SizeType mCurId;

  // 排他制御用のミューテックス
  std::mutex mMtx;

};

END_NAMESPACE_YM

#endif // IDPOOL_H
