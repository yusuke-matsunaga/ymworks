#ifndef TVDECNODE_H
#define TVDECNODE_H

/// @file TvDecNode.h
/// @brief TvDecNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_DJDEC

class TvDecEdge;

//////////////////////////////////////////////////////////////////////
/// @class TvDecNode TvDecNode.h "TvDecNode.h"
/// @brief Disjoint Graph のノードを表すクラス
///
/// このクラスはインターフェイス定義のみの純粋仮想基底クラス
//////////////////////////////////////////////////////////////////////
class TvDecNode
{
public:

  /// @brief デストラクタ
  virtual
  ~TvDecNode() = default;

  /// @brief LITタイプのオブジェクトを生成する．
  static
  TvDecNode*
  new_lit(
    SizeType id, ///< [in] ID番号
    SizeType var ///< [in] 変数番号
  );

  /// @brief ORタイプのオブジェクトを生成する．
  static
  TvDecNode*
  new_or(
    SizeType id,                             ///< [in] ID番号
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  );

  /// @brief XORタイプのオブジェクトを生成する．
  static
  TvDecNode*
  new_xor(
    SizeType id,                             ///< [in] ID番号
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  );

  /// @brief CPLXタイプのオブジェクトを生成する．
  static
  TvDecNode*
  new_cplx(
    SizeType id,                             ///< [in] ID番号
    const TvFunc& local_func,                ///< [in] ローカル関数
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  virtual
  SizeType
  id() const = 0;

  /// @brief LITタイプの時に true を返す．
  virtual
  bool
  is_lit() const = 0;

  /// @brief ORタイプの時に true を返す．
  virtual
  bool
  is_or() const = 0;

  /// @brief XORタイプの時に true を返す．
  virtual
  bool
  is_xor() const = 0;

  /// @brief CPLXタイプの時に true を返す．
  virtual
  bool
  is_cplx() const = 0;

  /// @brief グローバル関数を返す．
  virtual
  TvFunc
  global_func(
    SizeType input_num ///< [in] 入力数
  ) const = 0;

  /// @brief 先頭の変数を返す．
  virtual
  SizeType
  top_var() const = 0;

  /// @brief サポートを返す．
  virtual
  const std::vector<SizeType>&
  support() const = 0;

  /// @brief ローカル関数を求める．
  virtual
  TvFunc
  local_func() const = 0;

  /// @brief ローカル関数を求める．
  ///
  /// child_list の枝(ノード)は推移的ファンインのカットである必要がある．
  TvFunc
  local_func(
    const std::vector<TvDecEdge>& child_list ///< [in] 子供のリスト
  ) const;

  /// @brief subfunction の数を得る．
  virtual
  SizeType
  child_num() const = 0;

  /// @brief subfunction を表す枝を得る．
  virtual
  TvDecEdge
  child(
    SizeType pos ///< [in] 位置 ( 0 <= pos < child_num() )
  ) const = 0;

  /// @brief 内容を出力する．
  virtual
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const = 0;

};

END_NAMESPACE_YM_DJDEC

#endif // TVDECNODE_H
