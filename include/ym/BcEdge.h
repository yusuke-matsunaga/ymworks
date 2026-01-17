#ifndef BCEDGE_H
#define BCEDGE_H

/// @file BcEdge.h
/// @brief BcEDge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bc.h"


BEGIN_NAMESPACE_YM_BC

//////////////////////////////////////////////////////////////////////
/// @class BcEdge BcEdge.h "ym/BcEdge.h"
/// @ingroup BcGroup
/// @brief BcGraph の枝を表すクラス
///
/// - 入力
/// - 演算ノード
/// - 入力の否定
/// - 演算ノードの否定
/// - 定数0
/// - 定数1
/// を表す．
/// ただし，定数0，定数1 は出力の枝の場合のみ
//////////////////////////////////////////////////////////////////////
class BcEdge
{
  friend class BcGraph;

public:

  /// @brief 空のコンストラクタ
  BcEdge() = default;

  /// @brief コピーコンストラクタ
  BcEdge(
    const BcEdge& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief コピー代入演算
  BcEdge&
  operator=(
    const BcEdge& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief 定数0を表す枝を返す．
  static
  BcEdge
  zero()
  {
    return BcEdge(0);
  }

  /// @brief 定数1を表す枝を返す．
  static
  BcEdge
  one()
  {
    return BcEdge(1);
  }

  /// @brief 入力を指す枝を返す．
  static
  BcEdge
  input(
    SizeType input_id, ///< [in] 入力番号
    bool inv = false   ///< [in] 反転属性
  )
  {
    return BcEdge((input_id << 3) | 2 | static_cast<std::uint64_t>(inv));
  }

  /// @brief 演算を指す枝を返す．
  static
  BcEdge
  op(
    SizeType op_id,  ///< [in] 演算番号
    bool inv = false ///< [in] 反転属性
  )
  {
    return BcEdge((op_id << 3) | 4 | static_cast<std::uint64_t>(inv));
  }

  /// @brief デストラクタ
  ~BcEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力を指す時 true を返す．
  bool
  is_input() const
  {
    return (mBody & 6) == 2;
  }

  /// @brief 演算ノードを指す時 true を返す．
  bool
  is_op() const
  {
    return (mBody & 6) == 4;
  }

  /// @brief 定数0を指す時 true を返す．
  bool
  is_zero() const
  {
    return mBody == 0;
  }

  /// @brief 定数1を指す時 true を返す．
  bool
  is_one() const
  {
    return mBody == 1;
  }

  /// @brief 定数を指す時 true を返す．
  bool
  is_const() const
  {
    return (mBody & 6) == 0;
  }

  /// @brief 入力番号を返す．
  SizeType
  input_id() const
  {
    if ( !is_input() ) {
      throw std::invalid_argument{"not an input"};
    }
    return mBody >> 3;
  }

  /// @brief 演算番号を返す．
  SizeType
  op_id() const
  {
    if ( !is_op() ) {
      throw std::invalid_argument{"not an operator"};
    }
    return mBody >> 3;
  }

  /// @brief 否定属性を持つ時 true を返す．
  ///
  /// 定数の場合には否定属性を持たない．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1);
  }

  /// @brief 反転した枝を返す．
  BcEdge
  operator~() const
  {
    return BcEdge(mBody ^ 1);
  }

  /// @brief 自身の値を反転する．
  /// @return 自身の参照を返す．
  BcEdge&
  invert()
  {
    mBody ^= 1;
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BcEdge& right
  ) const
  {
    return mBody == right.mBody;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BcEdge& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 内容を表す文字列を返す．
  std::string
  to_string() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BcEdge(
    std::uint64_t body
  ) : mBody{body}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容を1ワードにパックしたもの
  // 0 bit:   反転フラグ
  // 1-2 bit: 00: 定数(0が反転なし)
  //          01: 入力
  //          10: 演算
  std::uint64_t mBody;

};

END_NAMESPACE_YM_BC

#endif // BCEDGE_H
