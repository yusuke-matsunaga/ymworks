#ifndef YM_UNIONFINDSET_H
#define YM_UNIONFINDSET_H

/// @file ym/UnionFindSet.h
/// @brief UnionFindSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

// クラスの前方参照定義
// ユーザは知る必要はない．
class UFCell;

//////////////////////////////////////////////////////////////////////
/// @class UnionFindSet UnionFindSet.h "ym/UnionFindSet.h"
/// @ingroup YmUtils
/// @brief Union/Find set を実装したクラス
///
/// データ構造とアルゴリズムの教科書でおなじみの Merge/Find-set
/// お互いに素な集合のマージと検索のみを行なう抽象データ型
//////////////////////////////////////////////////////////////////////
class UnionFindSet
{
public:

  /// @brief コンストラクタ
  UnionFindSet(
    SizeType n ///< [in] 確保したい要素の数．
  );

  /// @brief デストラクタ
  ~UnionFindSet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  num() const
  {
    return mCellArray.size();
  }

  /// @brief 代表元の検索 (Find)
  /// @return 要素 x の属する集合の代表元
  /// @exception std::out_of_range 範囲外のアクセス
  SizeType
  find(
    SizeType x ///< [in] 要素番号 ( 0 <= x < num() )
  );

  /// @brief 2つの集合の併合 (Union)
  /// @return 新たな代表元を返す．
  /// @exception std::out_of_range 範囲外のアクセス
  ///
  /// 2つの代表元 x, y の表す集合を併合する．
  ///
  /// union という名前は c/c++ の予約語なので merge という名前にしている．
  ///
  /// 実は x, y が代表元でなくても内部で find(x), find(y)
  /// を呼ぶので処理は行えるが，
  /// 代表元が分かっている場合にはそれを使ったほうが処理は速い．
  SizeType
  merge(
    SizeType x, ///< [in] マージ対象の要素番号1 ( 0 <= x < num() )
    SizeType y  ///< [in] マージ対象の要素番号2 ( 0 <= y < num() )
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 番号 x の要素セルを取ってくる．
  ///
  /// x が範囲外の場合には std::out_of_range 例外を送出する．
  UFCell*
  _get(
    SizeType x
  )
  {
    if ( x >= num() ) {
      throw std::out_of_range{"'x' is out of range"};
    }
    return mCellArray[x];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の配列
  std::vector<UFCell*> mCellArray;

};

END_NAMESPACE_YM

#endif // YM_UNIONFINDSET_H
