#ifndef AIGLITERAL_H
#define AIGLITERAL_H

/// @file AigLiteral.h
/// @brief AigLiteral のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigLiteral AigLiteral.h "ym/AigLiteral.h"
/// @brief AIG のノードを指す枝を表すクラス
/// @ingroup AigGroup
///
/// - 実際にはノード番号と極性をパックしたもの．
/// - ノード番号は AigMgr::cleanup() で変わる可能性があるので注意
//////////////////////////////////////////////////////////////////////
class AigLiteral
{
  friend class AigMgr;

public:

  /// @brief 空のコンストラクタ
  ///
  /// - 不正値となる．
  AigLiteral() = default;

  /// @brief デストラクタ
  ~AigLiteral() = default;

  /// @brief 不正値を返す．
  static
  AigLiteral
  invalid()
  {
    return AigLiteral(BAD_DATA);
  }

  /// @brief 定数0を返す．
  static
  AigLiteral
  zero()
  {
    return AigLiteral(0);
  }

  /// @brief 定数1を返す．
  static
  AigLiteral
  one()
  {
    return AigLiteral(1);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return !is_invalid();
  }

  /// @brief 適正な値を持っていない時 true を返す．
  bool
  is_invalid() const
  {
    return mData == BAD_DATA;
  }

  /// @brief 反転属性を返す．
  bool
  inv() const
  {
    return static_cast<bool>(mData & 1);
  }

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const
  {
    return mData == 0;
  }

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const
  {
    return mData == 1;
  }

  /// @brief 定数の時 true を返す．
  bool
  is_const() const
  {
    return mData < 2;
  }

  /// @brief ノード番号を取り出す．
  SizeType
  node_id() const
  {
    if ( is_invalid() || is_const() ) {
      throw std::logic_error{"not a node type"};
    }
    return (mData >> 1) - 1;
  }

  /// @brief 配列のインデックスとして使用可能なユニークな値を返す．
  SizeType
  index() const
  {
    // 実は mData そのもの
    return mData;
  }

  /// @brief 反転したリテラルを返す．
  AigLiteral
  operator~() const
  {
    return AigLiteral(mData ^ inv_mask());
  }

  /// @brief 自身を反転する．
  AigLiteral&
  invert()
  {
    mData ^= inv_mask();
    return *this;
  }

  /// @brief 反転属性をかけ合わせる．
  AigLiteral
  operator*(
    bool inv
  ) const
  {
    if ( inv ) {
      return operator~();
    }
    return *this;
  }

  /// @brief 反転属性を自身に掛け合わせる．
  AigLiteral&
  operator*=(
    bool inv
  )
  {
    if ( inv ) {
      invert();
    }
    return *this;
  }

  /// @brief 等価比較演算
  bool
  operator==(
    const AigLiteral& right
  ) const
  {
    return mData == right.mData;
  }

  /// @brief 非等価比較演算
  bool
  operator!=(
    const AigLiteral& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を指定したコンストラクタ
  AigLiteral(
    SizeType data
  ) : mData{data}
  {
  }

  /// @brief ノード番号を反転属性を指定したコンストラクタ
  AigLiteral(
    SizeType node_id,
    bool inv
  ) : mData{(node_id + 1) * 2 + static_cast<SizeType>(inv)}
  {
  }

  /// @brief 反転用のマスクを返す．
  SizeType
  inv_mask() const
  {
    if ( is_valid() ) {
      return 1;
    }
    return 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  static const SizeType BAD_DATA = ~0UL;

  // ノード番号と反転属性をパックしたもの
  // 正確には (ノード番号 + 1) * 2 + 反転属性
  SizeType mData{BAD_DATA};

};

END_NAMESPACE_YM_AIG

BEGIN_NAMESPACE_STD

// AigLiteral をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::nsAig::AigLiteral>
{
  SizeType
  operator()(
    const YM_NAMESPACE::nsAig::AigLiteral& lit
  ) const
  {
    return lit.index();
  }
};

END_NAMESPACE_STD

#endif // AIGLITERAL_H
