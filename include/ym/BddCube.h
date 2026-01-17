#ifndef BddCube_H
#define BddCube_H

/// @file BddCube.h
/// @brief BddCube のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddBase.h"
#include "ym/BddLit.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
/// @class BddCube BddCube.h "BddCube.h"
/// @ingroup DdGroup
/// @brief キューブを表す BDD のクラス
///
/// - ブール論理の世界においてはリテラルとは変数とその否定のみ
/// - キューブはリテラルの集合(論理積)
/// - 実際にはキューブを表すBDDを用いている．
/// - 空集合はユニバーサルキューブを表す(is_universal_cube() == true)．
/// - それ以外にヌルキューブの状態がある(is_invalid() == true)．
///   これは相反するリテラルをかけ合わせた結果に生じる．
//////////////////////////////////////////////////////////////////////
class BddCube :
  public BddBase
{
  friend class BddMgrHolder;

public:

  /// @brief BddLit からの変換コンストラクタ
  explicit
  BddCube(
    const BddLit& lit ///< [in] リテラル
  );

  /// @brief 要素のリストを指定したコンストラクタ
  ///
  /// lit_set を省略した場合には空のキューブとなる．
  explicit
  BddCube(
    const std::vector<BddLit>& lit_list ///< [in] リテラルのりスト
    = std::vector<BddLit>{}
  );

  /// @brief デストラクタ
  ~BddCube() = default;

  /// @brief 空のキューブを返す．
  static
  BddCube
  universal_cube();

  /// @brief 不正値を返す．
  static
  BddCube
  invalid();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のキューブの時に true を返す．
  bool
  is_universal_cube() const;

  /// @brief リテラル数を返す．
  SizeType
  size() const;

  /// @brief リテラルのリストに変換する．
  std::vector<BddLit>
  lit_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラルとの論理積を返す．
  ///
  /// 相反するリテラルとの積はヌルキューブとなる．
  BddCube
  operator&(
    const BddLit& right
  ) const
  {
    return _cube_and(*this, right);
  }

  /// @brief リテラルとの論理積を代入する．
  ///
  /// 相反するリテラルとの積はヌルキューブとなる．
  BddCube&
  operator&=(
    const BddLit& right
  )
  {
    *this = _cube_and(*this, right);
    return *this;
  }

  /// @brief キューブとの論理積を返す．
  ///
  /// 相反するリテラルとの積はヌルキューブとなる．
  BddCube
  operator&(
    const BddCube& right
  ) const
  {
    return _cube_and(*this, right);
  }

  /// @brief キューブとの論理積を代入する．
  ///
  /// 相反するリテラルとの積はヌルキューブとなる．
  BddCube&
  operator&=(
    const BddCube& right
  )
  {
    *this = _cube_and(*this, right);
    return *this;
  }

  /// @brief リテラルを取り除いたものを返す．
  ///
  /// 含んでいないリテラルは無視する．
  BddCube
  operator-(
    const BddLit& right
  ) const
  {
    return _cube_diff(*this, right);
  }

  /// @brief 自身からリテラルを取り除く
  BddCube&
  operator-=(
    const BddLit& right
  )
  {
    *this = _cube_diff(*this, right);
    return *this;
  }

  /// @brief キューブを取り除いたものを返す．
  ///
  /// 含んでいないリテラルは無視する．
  BddCube
  operator-(
    const BddCube& right
  ) const
  {
    return _cube_diff(*this, right);
  }

  /// @brief 自身からキューブを取り除く
  ///
  /// 含んでいないリテラルは無視する．
  BddCube&
  operator-=(
    const BddCube& right
  )
  {
    *this = _cube_diff(*this, right);
    return *this;
  }

  /// @brief 相反するリテラルを持つ時 true を返す．
  bool
  check_conflict(
    const BddLit& right
  ) const
  {
    return _cube_conflict(*this, right);
  }

  /// @brief 相反するリテラルを持つ時 true を返す．
  bool
  check_conflict(
    const BddCube& right
  ) const
  {
    return _cube_conflict(*this, right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // BddMgrHolder から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BddCube(
    const BddMgrHolder& holder,
    DdEdge root
  );

};

/// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  const BddCube& cube
);

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_STD

// BddCube をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::BddCube>
{
  SizeType
  operator()(
    const YM_NAMESPACE::BddCube& cube
  ) const
  {
    return cube.hash();
  }
};

END_NAMESPACE_STD

#endif // BddCube_H
