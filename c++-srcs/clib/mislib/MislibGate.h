#ifndef MISLIBGATE_H
#define MISLIBGATE_H

/// @file MislibGate.h
/// @brief MislibGate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "MislibNode.h"
#include "MislibExpr.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibGate MislibGate.h "MislibGate.h"
/// @brief ゲート(セル)を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibGate :
  public MislibNode
{
public:

  /// @brief コンストラクタ
  MislibGate(
    const FileRegion& loc,                ///< [in] 位置情報
    MislibStrPtr&& name,                  ///< [in] 名前を表すノード
    MislibNumPtr&& area,                  ///< [in] 面積を表すノード
    MislibStrPtr&& opin_name,             ///< [in] 出力品名を表すノード
    MislibExprPtr&& opin_expr,            ///< [in] 出力の論理式を表すノード
    std::vector<MislibPinPtr>&& ipin_list ///< [in] 入力ピンを表すノードのリスト
  ) : MislibNode{loc},
      mName{std::move(name)},
      mArea{std::move(area)},
      mOpinName{std::move(opin_name)},
      mOpinExpr{std::move(opin_expr)},
      mIpinList{std::move(ipin_list)}
  {
  }

  /// @brief デストラクタ
  ~MislibGate() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲート名を表すオブジェクトを取り出す．
  const MislibStr*
  name() const { return mName.get(); }

  /// @brief 面積を表すオブジェクトを返す．
  const MislibNum*
  area() const { return mArea.get(); }

  /// @brief 出力ピン名を表すオブジェクトを返す．
  const MislibStr*
  opin_name() const { return mOpinName.get(); }

  /// @brief 出力の論理式を表すオブジェクトを返す．
  const MislibExpr*
  opin_expr() const { return mOpinExpr.get(); }

  /// @brief 入力ピン数を返す．
  int
  ipin_num() const { return mIpinList.size(); }

  /// @brief 入力ピンを返す．
  const MislibPin*
  ipin(
    int pos ///< [in] 位置 ( 0 <= pos < ipin_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < ipin_num() );
    return mIpinList[pos].get();
  }

  /// @brief 内容を出力する(デバッグ用)．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  MislibStrPtr mName;

  // 面積
  MislibNumPtr mArea;

  // 出力ピン名
  MislibStrPtr mOpinName;

  // 出力の論理式
  MislibExprPtr mOpinExpr;

  // 入力ピンのリスト
  std::vector<MislibPinPtr> mIpinList;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibPin MislibPin.h "MislibPin.h"
/// @brief ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPin :
  public MislibNode
{
public:

  /// @brief コンストラクタ
  MislibPin(
    const FileRegion& loc,            ///< [in] 位置情報
    MislibStrPtr&& name,              ///< [in] ピン名
    MislibPhasePtr&& phase,           ///< [in] 極性
    MislibNumPtr&& input_load,        ///< [in] 入力負荷
    MislibNumPtr&& max_load,          ///< [in] 最大駆動負荷
    MislibNumPtr&& rise_block_delay,  ///< [in] 立ち上がり固定遅延
    MislibNumPtr&& rise_fanout_delay, ///< [in] 立ち上がり負荷依存遅延
    MislibNumPtr&& fall_block_delay,  ///< [in] 立ち下がり固定遅延
    MislibNumPtr&& fall_fanout_delay  ///< [in] 立ち下がり負荷依存遅延
  ) : MislibNode{loc},
      mName{std::move(name)},
      mPhase{std::move(phase)},
      mInputLoad{std::move(input_load)},
      mMaxLoad{std::move(max_load)},
      mRiseBlockDelay{std::move(rise_block_delay)},
      mRiseFanoutDelay{std::move(rise_fanout_delay)},
      mFallBlockDelay{std::move(fall_block_delay)},
      mFallFanoutDelay{std::move(fall_fanout_delay)}
  {
  }

  /// @brief デストラクタ
  ~MislibPin() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名を表すオブジェクトを取り出す．
  const MislibStr*
  name() const { return mName.get(); }

  /// @brief 極性情報を表すオブジェクトを取り出す．
  const MislibPhase*
  phase() const { return mPhase.get(); }

  /// @brief 入力負荷を表すオブジェクトを取り出す．
  const MislibNum*
  input_load() const { return mInputLoad.get(); }

  /// @brief 最大駆動負荷を表すオブジェクトを取り出す．
  const MislibNum*
  max_load() const { return mMaxLoad.get(); }

  /// @brief 立ち上がり固定遅延を表すオブジェクトを取り出す．
  const MislibNum*
  rise_block_delay() const { return mRiseBlockDelay.get(); }

  /// @brief 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
  const MislibNum*
  rise_fanout_delay() const { return mRiseFanoutDelay.get(); }

  /// @brief 立ち下がり固定遅延を表すオブジェクトを取り出す．
  const MislibNum*
  fall_block_delay() const { return mFallBlockDelay.get(); }

  /// @brief 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
  const MislibNum*
  fall_fanout_delay() const { return mFallFanoutDelay.get(); }

  /// @brief 内容を出力する(デバッグ用)．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン名
  MislibStrPtr mName;

  // 極性情報
  MislibPhasePtr mPhase;

  // 入力負荷
  MislibNumPtr mInputLoad;

  // 最大駆動負荷
  MislibNumPtr mMaxLoad;

  // 立ち上がり固有遅延
  MislibNumPtr mRiseBlockDelay;

  // 立ち上がり負荷依存遅延係数
  MislibNumPtr mRiseFanoutDelay;

  // 立ち下がり固有遅延
  MislibNumPtr mFallBlockDelay;

  // 立ち下がり負荷依存遅延係数
  MislibNumPtr mFallFanoutDelay;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBGATE_H
