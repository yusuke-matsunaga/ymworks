#ifndef BDDBASE_H
#define BDDBASE_H

/// @file BddBase.h
/// @brief BddBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/dd.h"
#include "ym/BddMgrHolder.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class BddBase BddBase.h "ym/BddBase.h"
/// @brief Bdd, BddVar, BddLit の基底クラス
///
/// - 基本的に個々の Bdd は一つの BddMgr に属す．
/// - 例外は invalid な Bdd で関連する BddMgr を持たない．
/// - Bdd 間の演算は同じ BddMgr に属するもののみ可とする．
/// - 異なる BddMgr に属する Bdd の演算は std::invalid_argument 例外を
///   送出する．
//////////////////////////////////////////////////////////////////////
class BddBase :
  public BddMgrHolder
{
  friend class BddMgrHolder;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BddBase();

  /// @brief コピーコンストラクタ
  BddBase(
    const BddBase& src ///< [in] コピー元のBDD
  );

  /// @brief コピー代入演算子
  BddBase&
  operator=(
    const BddBase& src ///< [in] コピー元のBDD
  );

  /// @brief デストラクタ
  ~BddBase();

  /// @brief BddBase(の派生クラス)のリストから代表オブジェクトを取り出す．
  ///
  /// - T は BddBase の派生クラスでなければならない．
  /// - bdd_list が空リストの場合は不正値が返される．
  /// - bdd_list の中の定数ではないオブジェクトを代表オブジェクトとする．
  /// - bdd_list が不正なBDDを含む場合には std::invalid_argument が送出される．
  /// - bdd_list が相異なるマネージャのBDDを含んでいる場合には
  ///   std::invalid_argument が送出される．
  template<class T>
  static
  T
  rep(
    const std::vector<T>& bdd_list
  )
  {
    T rep;
    for ( auto& bdd: bdd_list ) {
      if ( bdd.is_invalid() ) {
	throw std::invalid_argument{"contains invalid BDD"};
      }
      if ( rep._has_valid_mgr() ) {
	_check_mgr(rep, bdd);
      }
      else {
	rep = bdd;
      }
    }
    return rep;
  }


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 不正値の時 true を返す．
  bool
  is_invalid() const;

  /// @brief 親のマネージャを返す．
  ///
  /// 定数と不正値の時はマネージャを持たない．
  BddMgr
  mgr() const;

  /// @brief Bdd に変換する．
  ///
  /// 元の継承クラスの性質は失われる．
  Bdd
  bdd() const;

  /// @brief 根が否定されている時 true を返す．
  bool
  root_inv() const;

  /// @brief 等価比較演算
  ///
  /// 構造が同じで異なるBDDとの等価比較は
  /// is_identical() で行う．
  bool
  operator==(
    const BddBase& right ///< [in] オペランド
  ) const;

  /// @brief 非等価比較演算
  bool
  operator!=(
    const BddBase& right ///< [in] オペランド2
  ) const
  {
    return !operator==(right);
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を返す．
  SizeType
  size() const;

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BddBase(
    const BddMgrHolder& holder,
    DdEdge root
  );

  /// @brief 終端の時 true を返す．
  ///
  /// 具体的には，定数0, 定数1, 不正値 の時 true を返す．
  bool
  is_terminal() const;

  /// @brief 非終端ノードの時 true を返す．
  bool
  is_node() const;

  /// @brief 変数の時 true を返す．
  bool
  is_variable() const;

  /// @brief リテラルの時 true を返す．
  bool
  is_literal() const;

  /// @brief 肯定のリテラルの時 true を返す．
  ///
  /// - is_variable() と等価
  bool
  is_positive_literal() const;

  /// @brief 否定のリテラルの時 true を返す．
  bool
  is_negative_literal() const;

  /// @brief 積項の時 true を返す．
  bool
  is_cube() const;

  /// @brief 正リテラルの積項の時 true を返す．
  bool
  is_posicube() const;

  /// @brief 根の枝を返す．
  DdEdge
  root() const;

  /// @brief 根のノードのレベルを得る．
  SizeType
  root_level() const;

  /// @brief 根の枝を変更する．
  void
  _change_root(
    DdEdge new_root ///< [in] 変更する枝
  );

  /// @brief BddBase(の派生クラス)のリストから枝のリストに変換する．
  template<class T>
  static
  std::vector<DdEdge>
  _conv_to_edgelist(
    const std::vector<T>& bdd_list ///< [in] BDDのリスト
  )
  {
    std::vector<DdEdge> edge_list;
    edge_list.reserve(bdd_list.size());
    for ( auto& bdd: bdd_list ) {
      auto edge = bdd.root();
      edge_list.push_back(edge);
    }
    return edge_list;
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  /// @name 正常検査用の関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラルリストを表すBDDか調べる．
  ///
  /// is_cube() でない時は std::invalid_argument 例外を送出する．
  void
  _check_cube() const
  {
    if ( !is_cube() ) {
      throw std::invalid_argument{"not a cube"};
    }
  }

  /// @brief 変数リストを表すBDDか調べる．
  ///
  /// is_posicube() でない時は std::invalid_argument 例外を送出する．
  void
  _check_posicube() const
  {
    if ( !is_posicube() ) {
      throw std::invalid_argument{"not a positive cube"};
    }
  }

  /// @brief 適正な値を持っているかチェックする．
  void
  _check_valid() const
  {
    if ( is_invalid() ) {
      throw std::invalid_argument{"invalid BDD"};
    }
  }

  /// @brief 適正な値を持っているかチェックする(二項演算用)．
  static
  void
  _check_valid(
    const BddBase& left,
    const BddBase& right
  )
  {
    left._check_valid();
    right._check_valid();
    if ( left.is_node() &&
	 right.is_node() ) {
      _check_mgr(left, right);
    }
  }

  /// @brief 適正な値を持っているかチェックする(三項演算用)．
  static
  void
  _check_valid(
    const BddBase& op0,
    const BddBase& op1,
    const BddBase& op2
  )
  {
    op0._check_valid();
    op1._check_valid();
    op2._check_valid();
    if ( op0.is_node() &&
	 op1.is_node() &&
	 op2.is_node() ) {
      _check_mgr(op0, op1);
      _check_mgr(op0, op2);
    }
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  // BddVarSet 用の下請け関数
  // 引数が BddVar と BddVarSet があるので共通の BddBase の関数にしている．
  //////////////////////////////////////////////////////////////////////

  /// @brief サポート集合のユニオンを計算する．
  static
  BddVarSet
  _varset_cup(
    const BddBase& left, ///< [in] 第1オペランド
    const BddBase& right ///< [in] 第2オペランド
  );

  /// @brief サポート集合のインターセクションを計算する．
  static
  BddVarSet
  _varset_cap(
    const BddBase& left, ///< [in] 第1オペランド
    const BddBase& right ///< [in] 第2オペランド
  );

  /// @brief サポート集合の差を計算する．
  static
  BddVarSet
  _varset_diff(
    const BddBase& left, ///< [in] 第1オペランド
    const BddBase& right ///< [in] 第2オペランド
  );

  /// @brief 共通部分を持つかチェックする．
  static
  bool
  _varset_check(
    const BddBase& left, ///< [in] 第1オペランド
    const BddBase& right ///< [in] 第2オペランド
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // BddCube 用の下請け関数
  // 引数が BddLit と BddCube があるので共通の BddBase の関数にしている．
  //////////////////////////////////////////////////////////////////////

  static
  BddCube
  _cube_and(
    const BddBase& left,
    const BddBase& right
  );

  /// @brief 集合差を計算する下請け関数
  static
  BddCube
  _cube_diff(
    const BddBase& left,
    const BddBase& right
  );

  /// @brief 相反するリテラルを持つ時 true を返す．
  static
  bool
  _cube_conflict(
    const BddBase& left,
    const BddBase& right
  );

  friend
  bool
  is_identical(
    const BddBase& left, ///< [in] オペランド1
    const BddBase& right ///< [in] オペランド2
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根の枝(ポインタ+反転属性)
  PtrIntType mRoot{0};

};


/// @brief 同じ構造を持つか調べる．
///
/// 同じマネージャに属するBDDなら同じノードだが
/// マネージャが異なる場合には構造を調べる必要がある．
/// その場合，変数番号ではなくレベルを参照する．
bool
is_identical(
  const BddBase& left, ///< [in] オペランド1
  const BddBase& right ///< [in] オペランド2
);

END_NAMESPACE_YM_DD

BEGIN_NAMESPACE_YM

using nsDd::is_identical;

END_NAMESPACE_YM

#endif // BDD_H
