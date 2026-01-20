#ifndef CIPATMGR_H
#define CIPATMGR_H

/// @file CiPatMgr.h
/// @brief CiPatMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellClass;
class CiPatGraph;
class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiPatMgr CiPatMgr.h "CiPatMgr.h"
/// @brief パタングラフを管理するクラス
///
/// このクラスが持っている情報は
/// - パタングラフ
/// - 各々のパタングラフに対応する論理関数
/// - その論理関数とNPN同値な関数の集合
/// で，最後の関数がセルの論理関数と対応づけられる．
///
/// 情報の設定は専用形式のバイナリファイルを読み込むことでのみ行える．
/// バイナリファイルの生成は patgen/PatGen, pg_dump を参照のこと．
//////////////////////////////////////////////////////////////////////
class CiPatMgr
{
public:

  /// @brief コンストラクタ
  CiPatMgr();

  /// @brief デストラクタ
  ~CiPatMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // パタングラフ関係の情報取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンの最大の入力数を得る．
  SizeType
  max_input() const;

  /// @brief 総ノード数を返す．
  SizeType
  node_num() const
  {
    return mNodeTypeArray.size();
  }

  /// @brief ノードの種類を返す．
  ClibPatType
  node_type(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const
  {
    _check_node_id(node_id);
    return static_cast<ClibPatType>(mNodeTypeArray[node_id] & 3U);
  }

  /// @brief ノードが入力ノードの時に入力番号を返す．
  ///
  /// 入力ノードでない場合の返り値は不定
  SizeType
  input_id(
    SizeType node_id ///< [in] ノード番号 ( 0 <= node_id < node_num() )
  ) const
  {
    _check_node_id(node_id);
    return (mNodeTypeArray[node_id] >> 2);
  }

  /// @brief 入力のノード番号を返す．
  /// @return input_id の入力に対応するノードのノード番号
  SizeType
  input_node(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < max_input() )
  ) const
  {
    if ( input_id >= max_input() ) {
      throw std::out_of_range{"'input_id' is out of range"};
    }
    // 実はノード番号と同一
    return input_id;
  }

  /// @brief 総枝数を返す．
  SizeType
  edge_num() const
  {
    return mEdgeArray.size();
  }

  /// @brief 枝のファンイン元のノード番号を返す．
  SizeType
  edge_from(
    SizeType edge_id ///< [in] 枝番号 ( 0 <= edge_id < edge_num() )
  ) const
  {
    _check_edge_id(edge_id);
    return (mEdgeArray[edge_id] >> 1);
  }

  /// @brief 枝のファンアウト先のノード番号を返す．
  SizeType
  edge_to(
    SizeType edge_id ///< [in] 枝番号 ( 0 <= edge_id < edge_num() )
  ) const
  {
    _check_edge_id(edge_id);
    return (edge_id / 2);
  }

  /// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
  SizeType
  edge_pos(
    SizeType edge_id ///< [in] 枝番号 ( 0 <= edge_id < node_num() * 2 )
  ) const
  {
    _check_edge_id(edge_id);
    return (edge_id & 1U);
  }

  /// @brief 枝の反転属性を返す．
  bool
  edge_inv(
    SizeType edge_id ///< [in] 枝番号 ( 0 <= edge_id < node_num() * 2 )
  ) const
  {
    _check_edge_id(edge_id);
    return static_cast<bool>(mEdgeArray[edge_id] & 1U);
  }

  /// @brief 総パタン数を返す．
  SizeType
  pat_num() const;

  /// @brief パタンを返す．
  const CiPatGraph&
  pat(
    SizeType pat_id ///< [in] パタン番号 ( 0 <= pat_id < pat_num() )
  ) const;

  /// @brief バイナリダンプを行う．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を設定する．
  void
  set_node_num(
    SizeType nn ///< [in] ノード数
  );

  /// @brief 入力ノードの情報を設定する．
  void
  set_node_info(
    SizeType pos,     ///< [in] 位置番号
    SizeType id       ///< [in] 入力番号
  );

  /// @brief 論理ノードの情報を設定する．
  void
  set_node_info(
    SizeType pos,     ///< [in] 位置番号
    ClibPatType type, ///< [in] パタンの種類
    SizeType iid1,    ///< [in] 入力1のID番号
    bool iinv1,       ///< [in] 入力1の反転属性
    SizeType iid2,    ///< [in] 入力2のID番号
    bool iinv2        ///< [in] 入力2の反転属性
  );

  /// @brief パタン数を設定する．
  void
  set_pat_num(
    SizeType np ///< [in] パタン数
  );

  /// @brief パタンの情報を設定する．
  void
  set_pat_info(
    SizeType pos,                          ///< [in] パタン番号
    const CiCellClass* rep_class,          ///< [in] 代表クラス
    SizeType input_num,                    ///< [in] 入力数
    const std::vector<SizeType>& edge_list ///< [in] 枝情報のリスト
  );

  /// @brief データを読み込んでセットする．
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  restore(
    Deserializer& s ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号のチェック
  void
  _check_node_id(
    SizeType node_id ///< [in] ノード番号
  ) const
  {
    if ( node_id >= node_num() ) {
      throw std::out_of_range{"'node_id' is out of range"};
    }
  }

  /// @brief 枝番号のチェック
  void
  _check_edge_id(
    SizeType edge_id ///< [in] 枝番号
  ) const
  {
    if ( edge_id >= edge_num() ) {
      throw std::out_of_range{"'edge_id' is out of range"};
    }
  }

  /// @brief パタン番号のチェック
  void
  _check_pat_id(
    SizeType pat_id ///< [in] パタン番号
  ) const
  {
    if ( pat_id >= pat_num() ) {
      throw std::out_of_range{"'pat_id' is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの種類+入力番号を納めた配列
  // サイズはノード数
  std::vector<SizeType> mNodeTypeArray{};

  // ファンインのノード番号＋反転属性を納めた配列
  // サイズはノード数 x 2
  std::vector<SizeType> mEdgeArray{};

  // パタンの配列
  // サイズはパタン数
  std::vector<CiPatGraph> mPatArray;

};

END_NAMESPACE_YM_CLIB

#endif // CIPATMGR_H
