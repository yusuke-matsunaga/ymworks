#ifndef BDDVAR_H
#define BDDVAR_H

/// @file BddVar.h
/// @brief BddVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddBase.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class BddVar BddVar.h "BddVar.h"
/// @brief BDD の変数を表すクラス
///
/// - 実際には正のリテラル関数を表す BDD
//////////////////////////////////////////////////////////////////////
class BddVar :
  public BddBase
{
  friend class BddMgrHolder;

public:
  //////////////////////////////////////////////////////////////////////
  // 通常のコンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddVar() = default;

  // コピーコンストラクタとコピー代入演算子はデフォルト実装でOK

  /// @brief デストラクタ
  ~BddVar() = default;

  /// @brief 不正な値を作るクラスメソッド
  static
  BddVar
  invalid()
  {
    return BddVar{};
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号を返す．
  SizeType
  varid() const;

  /// @brief 肯定のリテラルを返す．
  BddLit
  positive_literal() const;

  /// @brief 否定のリテラルを返す．
  BddLit
  negative_literal() const;

  /// @brief 他の変数との集合を作る．
  ///
  /// 結果は BddVarSet となる．
  BddVarSet
  operator+(
    const BddVar& right
  ) const;

  /// @brief BddVarSet に追加する．
  ///
  /// 結果は BddVarSet となる．
  BddVarSet
  operator+(
    const BddVarSet& right
  ) const;

  /// @brief 比較演算子
  ///
  /// 変数番号ではなくレベルで比較する．
  bool
  operator<(
    const BddVar& right
  ) const;

  /// @brief 比較演算子
  ///
  /// 変数番号ではなくレベルで比較する．
  bool
  operator<=(
    const BddVar& right
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // BddMgrHolder から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ///
  /// - mgr が nullptr の時は std::invalid_argument 例外が送出される．
  /// - root が正のリテラル関数でない時は std::invalid_argument 例外が送出される．
  BddVar(
    const BddMgrHolder& mgr, ///< [in] マネージャ
    DdEdge root              ///< [in] 根の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief レベルを返す．
  SizeType
  level() const;

};

/// @brief 比較演算子
inline
bool
operator>(
  const BddVar& left,
  const BddVar& right
)
{
  return right.operator<(left);
}

/// @brief 比較演算子
inline
bool
operator>=(
  const BddVar& left,
  const BddVar& right
)
{
  return right.operator<=(left);
}

/// @brief ストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,  ///< [in] 出力ストリーム
  const BddVar& var ///< [in] 変数
)
{
  s << "v" << var.varid();
  return s;
}

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// BddVar をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::BddVar>
{
  SizeType
  operator()(
    const YM_NAMESPACE::BddVar& var
  ) const
  {
    return var.hash();
  }
};

END_NAMESPACE_STD

#endif // BDDVAR_H
