#ifndef BDDDECEDGE_H
#define BDDDECEDGE_H

/// @file BddDecEdge.h
/// @brief BddDecEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "BddDecNode.h"


BEGIN_NAMESPACE_YM_DJDEC

//////////////////////////////////////////////////////////////////////
/// @class BddDecEdge BddDecNode.h "ym/BddDecNode.h"
/// @brief BddDecNode を指す枝を表すクラス
//////////////////////////////////////////////////////////////////////
class BddDecEdge
{
public:

  /// @brief 空のコンストラクタ
  BddDecEdge() = default;

  /// @brief ノードと反転フラグを指定したコンストラクタ
  explicit
  BddDecEdge(
    const BddDecNode* node, ///< [in] ノード
    bool inv = false        ///< [in] 反転フラグ
  ) : mBody{reinterpret_cast<PtrIntType>(node) | static_cast<PtrIntType>(inv)}
  {
  }

  /// @brief 定数0の枝を返す．
  static
  BddDecEdge
  zero()
  {
    return BddDecEdge{0UL};
  }

  /// @brief 定数1の枝を返す．
  static
  BddDecEdge
  one()
  {
    return BddDecEdge{1UL};
  }

  /// @brief デストラクタ
  ~BddDecEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0の時に true を返す．
  bool
  is_zero() const
  {
    return mBody == 0UL;
  }

  /// @brief 定数1の時に true を返す．
  bool
  is_one() const
  {
    return mBody == 1UL;
  }

  /// @brief 定数ノードの時に true を返す．
  bool
  is_const() const
  {
    return (mBody & ~1UL) == 0UL;
  }

  /// @brief ノードを返す．
  const BddDecNode*
  node() const
  {
    return reinterpret_cast<BddDecNode*>(mBody & ~1UL);
  }

  /// @brief 反転フラグを返す．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1U);
  }

  /// @brief 反転していない枝を返す．
  BddDecEdge
  normal_edge() const
  {
    return BddDecEdge{mBody & ~1UL};
  }

  /// @brief グローバル関数を返す．
  Bdd
  global_func() const
  {
    if ( is_zero() ) {
      return Bdd::zero();
    }
    if ( is_one() ) {
      return Bdd::one();
    }
    auto f = node()->global_func();
    return f * inv();
  }

  /// @brief グローバル関数が 1 となる入力割当を返す．
  BddCube
  pat_1() const
  {
    if ( inv() ) {
      return node()->pat_0();
    }
    else {
      return node()->pat_1();
    }
  }

  /// @brief グローバル関数が 0 となる入力割当を返す．
  BddCube
  pat_0() const
  {
    if ( inv() ) {
      return node()->pat_1();
    }
    else {
      return node()->pat_0();
    }
  }

  /// @brief 反転した枝を返す．
  BddDecEdge
  operator~() const
  {
    return BddDecEdge{mBody ^ 1UL};
  }

  /// @brief 自身を反転させる．
  /// @return 自分自身を返す．
  BddDecEdge&
  negate()
  {
    mBody ^= 1UL;
    return *this;
  }

  /// @brief 反転フラグをかける．
  BddDecEdge
  operator*(
    bool inv ///< [in] 反転フラグ
  ) const
  {
    return BddDecEdge{mBody ^ static_cast<PtrIntType>(inv)};
  }

  /// @brief 反転フラグをかけて代入する．
  BddDecEdge&
  operator*=(
    bool inv ///< [in] 反転フラグ
  )
  {
    mBody ^= static_cast<PtrIntType>(inv);
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BddDecEdge& right
  ) const
  {
    return mBody == right.mBody;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BddDecEdge& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 2つの枝が極性違いのときに true を返す．
  static
  bool
  check_complement(
    const BddDecEdge& left,
    const BddDecEdge& right
  )
  {
    return (left.mBody ^ right.mBody) == 1UL;
  }

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const
  {
    return (mBody * mBody) >> 20;
  }

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s
  ) const
  {
    if ( is_zero() ) {
      s << "ZERO";
    }
    else if ( is_one() ) {
      s << "ONE";
    }
    else {
      if ( inv() ) {
	s << "~";
      }
      s << "#" << node()->id();
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BddDecEdge(
    PtrIntType body
  ) : mBody{body}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  PtrIntType mBody{0UL};

};

END_NAMESPACE_YM_DJDEC

BEGIN_NAMESPACE_STD

// BddDecEdge をキーにしたハッシュ関数クラスの定義
template <>
struct hash<DJDEC_NAMESPACE::BddDecEdge>
{
  SizeType
  operator()(
    const DJDEC_NAMESPACE::BddDecEdge& dgedge
  ) const
  {
    return dgedge.hash();
  }
};

END_NAMESPACE_STD

#endif // BDDDECEDGE_H
