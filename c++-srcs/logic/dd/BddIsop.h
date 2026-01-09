#ifndef BDDISOP_H
#define BDDISOP_H

/// @file BddIsop.h
/// @brief BddIsop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/logic.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_DD

struct IsopKey {
  Bdd key1;
  Bdd key2;

  bool
  operator==(
    const IsopKey& right
  ) const
  {
    return key1 == right.key1 && key2 == right.key2;
  }

};

struct IsopHash {
  SizeType
  operator()(
    const IsopKey& key
  ) const
  {
    auto h1 = key.key1.hash();
    auto h2 = key.key2.hash();
    return (h1 + 13) * h2;
  }
};


//////////////////////////////////////////////////////////////////////
/// @class BddIsop BddIsop.h "BddIsop.h"
/// @brief ISOP を行うクラス
//////////////////////////////////////////////////////////////////////
class BddIsop
{
  using CubeList = std::vector<std::vector<Literal>>;

public:

  /// @brief コンストラクタ
  BddIsop(
    const BddMgr& mgr ///< [in] マネージャ
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~BddIsop() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 簡単化を行う．
  CubeList
  isop_step(
    const Bdd& on,
    const Bdd& dc
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief キューブのリストを BDD に変換する．
  Bdd
  cube2bdd(
    const CubeList& cube_list
  );

  /// @brief キューブのリストをマージする．
  static
  void
  merge_list(
    SizeType top,
    CubeList& cube_list,
    const CubeList& cube0_list,
    const CubeList& cube1_list
  );

  /// @brief キューブのリストを出力する(デバッグ用)
  static
  void
  print_cover(
    std::ostream& s,
    const CubeList& cube_list
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  BddMgr mMgr;

  // 演算結果テーブル
  std::unordered_map<IsopKey, CubeList, IsopHash> mTable;

};

END_NAMESPACE_YM_DD

#endif // BDDISOP_H
