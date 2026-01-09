#ifndef CGMGR_H
#define CGMGR_H

/// @file CgMgr.h
/// @brief CgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/Expr.h"
#include "cgmgr/PatMgr.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellLibrary;
class CiCellClass;
class CiCellGroup;
class CiCell;
class CiSeqInfo;
class CgSignature;

//////////////////////////////////////////////////////////////////////
/// @class CgMgr CgMgr.h "CgMgr.h"
/// @brief セルグループを管理するクラス
///
/// ClibCellLibrary の構築中に一時的に生成されるインスタンスなので
/// あまりメモリ効率は考えていない．
//////////////////////////////////////////////////////////////////////
class CgMgr
{
public:

  /// @brief コンストラクタ
  explicit
  CgMgr(
    CiCellLibrary& library ///< [in] 対象のセルライブラリ
  );

  /// @brief デストラクタ
  ~CgMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理セルグループを得る．
  const CiCellGroup*
  logic_group(
    SizeType type ///< [in] グループの種類 ( 0 <= type < 24 )
  ) const
  {
    if ( type < 0 || 24 <= type ) {
      throw std::invalid_argument{"'type' is out-of-range"};
    }
    return mLogicGroup[type];
  }

  /// @brief FFクラスを得る．
  std::vector<const CiCellClass*>
  find_ff_class(
    ClibSeqAttr seq_attr ///< [in] 順序セルの属性
  ) const;

  /// @brief FFクラスの辞書を得る．
  const std::unordered_map<SizeType, std::vector<const CiCellClass*>>&
  ff_class_dict() const
  {
    return mFFClassDict;
  }

  /// @brief ラッチクラスを得る．
  std::vector<const CiCellClass*>
  find_latch_class(
    ClibSeqAttr seq_attr ///< [in] 順序セルの属性
  ) const;

  /// @brief ラッチクラスの辞書を得る．
  const std::unordered_map<SizeType, std::vector<const CiCellClass*>>&
  latch_class_dict() const
  {
    return mLatchClassDict;
  }

  /// @brief シグネチャに一致するグループを探す．
  /// @return グループを返す．
  ///
  /// なければ作る．
  CiCellGroup*
  find_group(
    const CgSignature& sig ///< [in] シグネチャ
  );

  /// @breif パタングラフを生成する．
  void
  gen_pat();

  /// @brief 全ノード数を返す．
  SizeType
  pat_node_num() const;

  /// @brief ノードを返す．
  const PatNode&
  pat_node(
    SizeType pos ///< [in] ノード番号 ( 0 <= pos < node_num() )
  ) const;

  /// @brief パタン数を返す．
  SizeType
  pat_num() const;

  /// @brief パタンのノードリストを返す．
  void
  get_pat_info(
    SizeType id,                     ///< [in] パタン番号 ( 0 <= id < pat_num() )
    const CiCellClass*& rep_class,   ///< [out] パタンの表す代表関数番号
    SizeType& input_num,             ///< [out] パタンの入力数
    std::vector<SizeType>& node_list ///< [out] パタンを DFS 順でたどった時のノード番号のリスト
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理セルグループの初期化を行なう．
  void
  logic_init();

  /// @brief 代表クラスを得る．
  CiCellClass*
  _find_class(
    const CgSignature& sig ///< [in] シグネチャ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルクラスと関係する論理式のリスト
  struct ClassExprList
  {
    CiCellClass* mClass;
    std::vector<Expr> mExprList;
  };

  // ライブラリ
  CiCellLibrary& mLibrary;

  // シグネチャ文字列をキーにしてグループを保持する辞書
  std::unordered_map<std::string, CiCellGroup*> mGroupDict;

  // シグネチャ文字列をキーにしてセルクラスを保持する辞書
  std::unordered_map<std::string, CiCellClass*> mClassDict;

  // セルクラスをキーにして論理式のリストを保持する辞書
  std::unordered_map<const CiCellClass*, std::vector<Expr>> mExprListDict;

  const SizeType C0_BASE = 0;
  const SizeType C1_BASE = 1;
  const SizeType BUF_BASE = 2;
  const SizeType INV_BASE = 3;
  const SizeType AND_BASE = 4;
  const SizeType NAND_BASE = 7;
  const SizeType OR_BASE = 10;
  const SizeType NOR_BASE = 13;
  const SizeType XOR_BASE = 16;
  const SizeType XNOR_BASE = 19;
  const SizeType MUX2_BASE = 22;
  const SizeType MUX4_BASE = 23;

  // 論理セルグループのリスト
  const CiCellGroup* mLogicGroup[24];

  // FFセルクラスのリストを持つ辞書
  // キーは ClibSeqAttr::index()
  std::unordered_map<SizeType, std::vector<const CiCellClass*>> mFFClassDict;

  // ラッチセルクラスのリストを持つ辞書
  // キーは ClibSeqAttr::index()
  std::unordered_map<SizeType, std::vector<const CiCellClass*>> mLatchClassDict;

  // 一時的にパタンを保持するクラス
  PatMgr mPatMgr;

};

END_NAMESPACE_YM_CLIB

#endif // CGMGR_H
