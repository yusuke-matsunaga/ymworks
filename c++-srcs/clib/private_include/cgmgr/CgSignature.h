#ifndef CGSIGNATURE_H
#define CGSIGNATURE_H

/// @file CgSignature.h
/// @brief CgSignature のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_CLIB

class CgSigRep;

//////////////////////////////////////////////////////////////////////
/// @class CgSignature CgSignature.h "CgSignature.h"
/// @brief CgMgr 用のシグネチャを表すクラス
//////////////////////////////////////////////////////////////////////
class CgSignature
{
public:

  /// @brief 空のコンストラクタ
  CgSignature();

  /// @brief 内容を指定したコンストラクタ
  explicit
  CgSignature(
    std::unique_ptr<const CgSigRep>&& ptr ///< [in] ムーブされる CgSigRep のポインタ
  );

  /// @brief ムーブコンストラクタ
  CgSignature(
    CgSignature&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief ムーブ代入演算子
  CgSignature&
  operator=(
    CgSignature&& src ///< [in] ムーブ元のオブジェクト
  );

  /// @brief デストラクタ
  ~CgSignature();


public:
  //////////////////////////////////////////////////////////////////////
  // 具体的なシグネチャを作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 1出力論理関数用のシグネチャを作る．
  static
  CgSignature
  make_logic_sig(
    SizeType ni,        ///< [in] 入力数
    const Expr& expr    ///< [in] 対象の論理式
  );

  /// @brief 1出力tristate関数用のシグネチャを作る．
  static
  CgSignature
  make_logic_sig(
    const TvFunc& func,    ///< [in] 対象の論理関数
    const TvFunc& tristate ///< [in] 対象のtristate条件
  )
  {
    auto ni = func.input_num();
    return make_logic_sig(ni, 1, 0, {func}, {tristate});
  }

  /// @brief 一般的な組み合わせ回路用のシグネチャを作る．
  ///
  /// - func_list, tristate_list のサイズは no + nb
  /// - func_list, tristate_list の要素の論理関数の入力サイズは ni + nb
  static
  CgSignature
  make_logic_sig(
    SizeType ni,                             ///< [in] 入力数
    SizeType no,                             ///< [in] 出力数
    SizeType nb,                             ///< [in] 入出力数
    const std::vector<TvFunc>& func_list,    ///< [in] 対象の論理関数のリスト
    const std::vector<TvFunc>& tristate_list ///< [in] 対象のtristate条件のリスト
  );

  /// @brief FF用のシグネチャを作る．
  ///
  /// - func_list, tristate_list のサイズは no + nb
  /// - func_list, tristate_list の要素の論理関数の入力サイズは ni + nb + 2
  static
  CgSignature
  make_ff_sig(
    SizeType ni,                              ///< [in] 入力数
    SizeType no,                              ///< [in] 出力数
    SizeType nb,                              ///< [in] 入出力数
    const std::vector<TvFunc>& func_list,     ///< [in] 対象の論理関数のリスト
    const std::vector<TvFunc>& tristate_list, ///< [in] 対象のtristate条件のリスト
    const TvFunc& clocked_on,                 ///< [in] マスタークロック
    const TvFunc& clocked_on_also,            ///< [in] スレーブクロック
    const TvFunc& next_state,                 ///< [in] 次状態関数
    const TvFunc& clear,                      ///< [in] クリア条件
    const TvFunc& preset,                     ///< [in] プリセット条件
    ClibSeqAttr seq_attr                      ///< [in] 順序セルの属性
  );

  /// @brief ラッチ用のシグネチャを作る．
  ///
  /// - func_list, tristate_list のサイズは no + nb
  /// - func_list, tristate_list の要素の論理関数の入力サイズは ni + nb + 2
  static
  CgSignature
  make_latch_sig(
    SizeType ni,                              ///< [in] 入力数
    SizeType no,                              ///< [in] 出力数
    SizeType nb,                              ///< [in] 入出力数
    const std::vector<TvFunc>& func_list,     ///< [in] 対象の論理関数のリスト
    const std::vector<TvFunc>& tristate_list, ///< [in] 対象のtristate条件のリスト
    const TvFunc& enable_on,                  ///< [in] マスターenable
    const TvFunc& enable_on_also,             ///< [in] スレーenable
    const TvFunc& data_in,                    ///< [in] データ入力
    const TvFunc& clear,                      ///< [in] クリア条件
    const TvFunc& preset,                     ///< [in] プリセット条件
    ClibSeqAttr seq_attr                      ///< [in] 順序セルの属性
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 意味のある値を持つ時 true を返す．
  bool
  is_valid() const
  {
    return mRepPtr != nullptr;
  }

  /// @brief シグネチャ文字列を返す．
  std::string
  str() const;

  /// @brief セルの種類を返す．
  ClibCellType
  cell_type() const;

  /// @brief 順序セルの属性を返す．
  ClibSeqAttr
  seq_attr() const;

  /// @brief 単一の論理式を持つ時その式を返す．
  Expr
  expr() const;

  /// @brief 変換を施したシグネチャを返す．
  CgSignature
  xform(
    const ClibIOMap& iomap ///< [in] 変換マップ
  ) const;

  /// @brief 代表シグネチャに対する変換を求める．
  ClibIOMap
  rep_map() const;

  /// @brief 同位体変換のリストを求める．
  std::vector<ClibIOMap>
  idmap_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  std::unique_ptr<const CgSigRep> mRepPtr{nullptr};

};

END_NAMESPACE_YM_CLIB

#endif // CGSIGNATURE_H
