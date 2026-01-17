#ifndef ZDDITEM_H
#define ZDDITEM_H

/// @file ZddItem.h
/// @brief ZddItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddBase.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class ZddItem ZddItem.h "ZddItem.h"
/// @ingroup DdGroup
/// @brief ZDD の要素を表すクラス
///
/// - 実際にはシングルトンを表す ZDD
//////////////////////////////////////////////////////////////////////
class ZddItem :
  public ZddBase
{
  friend class ZddMgrHolder;

private:

  /// @brief 内容を指定したコンストラクタ
  ///
  /// root はシングルトンでなければならない．
  ZddItem(
    const ZddMgrHolder& holder, ///< [in] マネージャ
    DdEdge root                 ///< [in] 根の枝
  );


public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  ZddItem() = default;

  // コピーコンストラクタとコピー代入演算子はデフォルト実装で OK

  /// @brief デストラクタ
  ~ZddItem() = default;

  /// @brief 不正値を作るクラスメソッド
  static
  ZddItem
  invalid()
  {
    return ZddItem{};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素番号を返す．
  SizeType
  id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief レベルを返す．
  SizeType
  level() const;

};


/// @brief ストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,
  const ZddItem& item
)
{
  s << "I" << item.id();
  return s;
}

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// ZddItem をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::ZddItem>
{
  SizeType
  operator()(
    const YM_NAMESPACE::ZddItem& item
  ) const
  {
    return item.hash();
  }
};

END_NAMESPACE_STD

#endif // ZDDITEM_H
