#ifndef TVDECEDGE_H
#define TVDECEDGE_H

/// @file TvDecEdge.h
/// @brief TvDecEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "TvDecNode.h"


BEGIN_NAMESPACE_YM_DJDEC

//////////////////////////////////////////////////////////////////////
/// @class TvDecEdge TvDecEdge.h "TvDecEdge.h"
/// @brief TvDecNode を指す枝を表すクラス
//////////////////////////////////////////////////////////////////////
class TvDecEdge
{
public:

  /// @brief 空のコンストラクタ
  TvDecEdge() = default;

  /// @brief ノードと反転フラグを指定したコンストラクタ
  explicit
  TvDecEdge(
    const TvDecNode* node, ///< [in] ノード
    bool inv = false       ///< [in] 反転フラグ
  ) : mBody{reinterpret_cast<PtrIntType>(node) | static_cast<PtrIntType>(inv)}
  {
  }

  /// @brief 定数0の枝を返す．
  static
  TvDecEdge
  zero()
  {
    return TvDecEdge{0UL};
  }

  /// @brief 定数1の枝を返す．
  static
  TvDecEdge
  one()
  {
    return TvDecEdge{1UL};
  }

  /// @brief デストラクタ
  ~TvDecEdge() = default;


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
  const TvDecNode*
  node() const
  {
    return reinterpret_cast<const TvDecNode*>(mBody & ~1UL);
  }

  /// @brief 反転フラグを返す．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1U);
  }

  /// @brief 反転していない枝を返す．
  TvDecEdge
  normal_edge() const
  {
    return TvDecEdge{mBody & ~1UL};
  }

  /// @brief グローバル関数を返す．
  TvFunc
  global_func(
    SizeType input_num
  ) const
  {
    if ( is_zero() ) {
      return TvFunc::zero(input_num);
    }
    if ( is_one() ) {
      return TvFunc::one(input_num);
    }
    auto f = node()->global_func(input_num);
    return f * inv();
  }

  /// @brief サポートを返す．
  std::vector<SizeType>
  support() const
  {
    if ( is_const() ) {
      return {};
    }
    return node()->support();
  }

  /// @brief ローカル関数を求める．
  TvFunc
  local_func() const
  {
    return node()->local_func() * inv();
  }

  /// @brief ローカル関数を求める．
  ///
  /// child_list の枝(ノード)は推移的ファンインのカットである必要がある．
  TvFunc
  local_func(
    const std::vector<TvDecEdge>& child_list ///< [in] 子供のリスト
  ) const
  {
    return node()->local_func(child_list) * inv();
  }

  /// @brief 反転した枝を返す．
  TvDecEdge
  operator~() const
  {
    return TvDecEdge{mBody ^ 1UL};
  }

  /// @brief 自身を反転させる．
  /// @return 自分自身を返す．
  TvDecEdge&
  negate()
  {
    mBody ^= 1UL;
    return *this;
  }

  /// @brief 反転フラグをかける．
  TvDecEdge
  operator*(
    bool inv ///< [in] 反転フラグ
  ) const
  {
    return TvDecEdge{mBody ^ static_cast<PtrIntType>(inv)};
  }

  /// @brief 反転フラグをかけて代入する．
  TvDecEdge&
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
    const TvDecEdge& right
  ) const
  {
    return mBody == right.mBody;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const TvDecEdge& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 2つの枝が極性違いのときに true を返す．
  static
  bool
  check_complement(
    const TvDecEdge& left,
    const TvDecEdge& right
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
  TvDecEdge(
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

inline
std::ostream&
operator<<(
  std::ostream& s,
  const TvDecEdge& e
)
{
  e.print(s);
  return s;
}

END_NAMESPACE_YM_DJDEC

BEGIN_NAMESPACE_STD

// TvDecEdge をキーにしたハッシュ関数クラスの定義
template <>
struct hash<DJDEC_NAMESPACE::TvDecEdge>
{
  SizeType
  operator()(
    const DJDEC_NAMESPACE::TvDecEdge& dgedge
  ) const
  {
    return dgedge.hash();
  }
};

// std::vector<TvDecEdge> をキーにしたハッシュ関数クラスの定義
template<>
struct hash<vector<DJDEC_NAMESPACE::TvDecEdge>>
{
  SizeType
  operator()(
    const vector<DJDEC_NAMESPACE::TvDecEdge>& dgedge_list
  ) const
  {
    SizeType ans = 1;
    for ( auto edge: dgedge_list ) {
      ans *= edge.hash();
    }
    return ans;
  }
};

END_NAMESPACE_STD

#endif // TVDECEDGE_H
