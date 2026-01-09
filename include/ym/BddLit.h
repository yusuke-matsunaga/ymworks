#ifndef BDDLIT_H
#define BDDLIT_H

/// @file BddLit.h
/// @brief BddLit のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddBase.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddLit BddLit.h "BddLit.h"
/// @brief BDD のリテラルを表すクラス
///
/// - ブール論理の世界においてはリテラルとは変数とその否定のみ
/// - 実際にはリテラル関数を表すBDDを用いている．
//////////////////////////////////////////////////////////////////////
class BddLit :
  public BddBase
{
  friend class BddMgrHolder;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddLit() = default;

  /// @brief マネージャと変数番号を指定したコンストラクタ
  ///
  /// - mgr が nullptr の時は std::invalid_argument 例外が送出される．
  explicit
  BddLit(
    BddMgrHolder& mgr, ///< [in] マネージャ
    SizeType varid,    ///< [in] 変数番号
    bool inv = false   ///< [in] 反転属性
                       ///<      - false: 肯定(反転なし)
                       ///<      - true:  否定(反転あり)
  );

  /// @brief BddVar と反転属性を引数にとるコンストラクタ
  ///
  /// - var が不正値の時は std::invalid_argument 例外が送出される．
  explicit
  BddLit(
    const BddVar& var, ///< [in] 変数
    bool inv = false   ///< [in] 反転属性
                       ///<      - false: 肯定(反転なし)
                       ///<      - true:  否定(反転あり)
  );

  /// @brief デストラクタ
  ~BddLit() = default;

  /// @brief 不正な値を作るクラスメソッド
  static
  BddLit
  invalid()
  {
    return BddLit{};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を取り出す．
  BddVar
  var() const;

  /// @brief 肯定のリテラルのとき true を返す．
  bool
  is_positive() const
  {
    return !is_negative();
  }

  /// @brief 否定のリテラルのとき true を返す．
  bool
  is_negative() const
  {
    return root_inv();
  }

  /// @brief 同じ変数の肯定のリテラルを返す．
  BddLit
  positive_literal() const;

  /// @brief 同じ変数の否定のリテラルを返す．
  BddLit
  negative_literal() const;

  /// @brief 極性を反転したリテラルを返す．
  BddLit
  complement() const;

  /// @brief 反転演算子(complementの別名)
  BddLit
  operator~() const
  {
    return complement();
  }

  /// @brief 反転属性との掛け算
  BddLit
  operator*(
    bool inv ///< [in] 反転属性(true で反転)
  ) const;

  /// @brief 反転属性との掛け算付き代入
  BddLit&
  operator*=(
    bool inv ///< [in] 反転属性(true で反転)
  );

  /// @brief 論理積を計算する．
  ///
  /// 結果は BddCube となる．
  BddCube
  operator&(
    const BddLit& right
  ) const;

  /// @brief キューブとの論理積
  BddCube
  operator&(
    const BddCube& right
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // BddMgrHolder から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ///
  /// - mgr が nullptr の時は std::invalid_argument 例外が送出される．
  /// - root がリテラル関数でない時は std::invalid_argument 例外が送出される．
  BddLit(
    const BddMgrHolder& mgr, ///< [in] マネージャ
    DdEdge root              ///< [in] 根の枝
  );

};


/// @brief ストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,  ///< [in] 出力ストリーム
  const BddLit& lit ///< [in] リテラル
)
{
  if ( lit.is_positive() ) {
    s << "p";
  }
  else {
    s << "n";
  }
  s << lit.var().varid();
  return s;
}

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// BddLit をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::BddLit>
{
  SizeType
  operator()(
    const YM_NAMESPACE::BddLit& lit
  ) const
  {
    return lit.hash();
  }
};

END_NAMESPACE_STD

#endif // BDDLIT_H
