#ifndef AIGVALARRAY_H
#define AIGVALARRAY_H

/// @file AigValArray.h
/// @brief AigValArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigEdge.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigValArray AigValArray.h "AigValArray.h"
/// @brief AIG の値を保持する配列
///
/// AIG のノード番号をキーとして値(AigBitVect)を保持する配列
/// 中身は std::vector<AigBitVect> だが，AigEdge に対する値を返す
/// インターフェイスを持つ．
///
/// 値の設定は friend class の AigMgrImpl が行う．
//////////////////////////////////////////////////////////////////////
class AigValArray
{
  friend class AigMgrImpl;

public:

  /// @brief コンストラクタ
  AigValArray() = default;

  /// @brief コピーコンストラクタ
  AigValArray(
    const AigValArray& src
  ) = default;

  /// @brief ムーブコンストラクタ
  AigValArray(
    AigValArray&& src
  ) = default;

  /// @brief デストラクタ
  ~AigValArray() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝に対応する値を返す．
  AigBitVect
  get_val(
    AigEdge edge ///< [in] 枝
  ) const
  {
    if ( edge.is_zero() ) {
      return 0U;
    }
    else if ( edge.is_one() ) {
      return ~0U;
    }
    auto node = edge.node();
    auto val = mValArray[node->id()];
    if ( edge.inv() ) {
      val = ~val;
    }
    return val;
  }

  /// @brief get_val() の別名
  AigBitVect
  operator[](
    AigEdge edge ///< [in] 枝
  ) const
  {
    return get_val(edge);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値の配列
  std::vector<AigBitVect> mValArray;

};

END_NAMESPACE_YM_AIG

#endif // AIGVALARRAY_H
