#ifndef ZDDBASE_H
#define ZDDBASE_H

/// @file ZddBase.h
/// @brief ZddBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddMgrHolder.h"


BEGIN_NAMESPACE_YM_DD

class DdEdge;

//////////////////////////////////////////////////////////////////////
/// @class ZddBase ZddBase.h "ym/ZddBase.h"
/// @ingroup DdGroup
/// @brief Zdd, ZddItem, ZddItemSet の基底クラス
//////////////////////////////////////////////////////////////////////
class ZddBase :
  public ZddMgrHolder
{
  friend class ZddMgrHolder;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  ZddBase();

  /// @brief コピーコンストラクタ
  ZddBase(
    const ZddBase& src ///< [in] コピー元のZDD
  );

  /// @brief コピー代入演算子
  ZddBase&
  operator=(
    const ZddBase& src ///< [in] コピー元のZDD
  );

  /// @brief デストラクタ
  ~ZddBase();

  /// @brief ZddBase(の派生クラス)のリストから代表オブジェクトを取り出す．
  ///
  /// - T は ZddBase の派生クラスでなければならない．
  /// - zdd_list が空リストの場合は不正値が返される．
  /// - zdd_list の中の定数ではないオブジェクトを代表オブジェクトとする．
  /// - zdd_list が不正なZDDを含む場合には std::invalid_argument が送出される．
  /// - zdd_list が相異なるマネージャのZDDを含んでいる場合には std::invalid_argument が送出される．
  template<class T>
  static
  T
  rep(
    const std::vector<T>& zdd_list
  )
  {
    T rep;
    for ( auto& zdd: zdd_list ) {
      if ( zdd.is_invalid() ) {
	throw std::invalid_argument{"contains invalid BDD"};
      }
      if ( rep.has_valid_mgr() ) {
	rep._check_mgr(zdd);
      }
      else {
	rep = zdd;
      }
    }
    return rep;
  }


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を取得する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のマネージャを返す．
  ZddMgr
  mgr() const;

  /// @brief Zdd に変換する
  ///
  /// 元の継承クラスの性質は失われる．
  Zdd
  zdd() const;

  /// @brief 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const;

  /// @brief 不正値の時 true を返す．
  bool
  is_invalid() const;

  /// @brief 終端の時 true を返す．
  ///
  /// 具体的には，定数0, 定数1, 不正値 の時 true を返す．
  bool
  is_terminal() const;

  /// @brief 非終端ノードの時 true を返す．
  bool
  is_node() const
  {
    return !is_terminal();
  }

  /// @brief シングルトンの時 true を返す．
  bool
  is_singleton() const;

  /// @brief 根が否定されている時 true を返す．
  bool
  root_inv() const;

  /// @brief 等価比較演算
  ///
  /// 構造が同じで異なるZDDとの等価比較は
  /// is_identical() で行う．
  bool
  operator==(
    const ZddBase& right ///< [in] オペランド2
  ) const;

  /// @brief 非等価比較演算
  bool
  operator!=(
    const ZddBase& right ///< [in] オペランド2
  ) const
  {
    return !operator==(right);
  }

  /// @brief 同じ構造を持つか調べる．
  ///
  /// 同じマネージャに属するZDDなら同じノードだが
  /// マネージャが異なる場合には構造を調べる必要がある．
  /// その場合，変数番号ではなくレベルを参照する．
  static
  bool
  is_identical(
    const ZddBase& left, ///< [in] オペランド1
    const ZddBase& right ///< [in] オペランド2
  );

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
  ZddBase(
    const ZddMgrHolder& mgr,
    DdEdge root
  );

  /// @brief 根の枝を返す．
  DdEdge
  root() const;

  /// @brief 根の枝を変更する．
  void
  _change_root(
    DdEdge new_root ///< [in] 変更する枝
  );

  /// @brief ZddBase(の派生クラス)のリストから枝のリストに変換する．
  template<class T>
  static
  std::vector<DdEdge>
  _conv_to_edgelist(
    const std::vector<T>& zdd_list ///< [in] ZDDのリスト
  )
  {
    std::vector<DdEdge> edge_list;
    edge_list.reserve(zdd_list.size());
    for ( auto& zdd: zdd_list ) {
      auto edge = zdd.root();
      edge_list.push_back(edge);
    }
    return edge_list;
  }

  /// @brief onset() の下請け関数
  DdEdge
  _onset(
    const ZddItem& item ///< [in] 要素
  ) const;

  /// @brief offset() の下請け関数
  DdEdge
  _offset(
    const ZddItem& item ///< [in] 要素
  ) const;

  /// @brief 共通集合演算
  /// @return 結果を返す．
  DdEdge
  _cap(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief ユニオン演算
  /// @return 結果を返す．
  DdEdge
  _cup(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief 集合差演算
  /// @return 結果を返す．
  DdEdge
  _diff(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief 直積演算
  /// @return 結果を返す．
  DdEdge
  _product(
    const Zdd& right ///< [in] オペランド
  ) const;

  /// @brief 補集合演算
  /// @return 結果を返す．
  DdEdge
  _invert() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根の枝
  PtrIntType mRoot{0};

};

END_NAMESPACE_YM_DD

#endif // ZDDBASE_H
