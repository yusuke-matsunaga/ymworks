
/// @file CiCellLibrary_restore.cc
/// @brief CiCellLibrary の実装ファイル(restore()関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCellLibrary.h"
#include "ci/CiCellClass.h"
#include "ci/CiCellGroup.h"
#include "ci/CiCell.h"
#include "ci/CiPin.h"
#include "ci/CiBus.h"
#include "ci/CiBusType.h"
#include "ci/CiBundle.h"
#include "ci/CiTiming.h"
#include "ci/CiLutTemplate.h"
#include "ci/CiLut.h"
#include "ci/CiPatMgr.h"
#include "ci/CiPatGraph.h"
#include "ci/Deserializer.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiCellLibrary
//////////////////////////////////////////////////////////////////////

CiCellLibrary*
CiCellLibrary::restore(
  std::istream& is
)
{
  Deserializer s{is};

  // 要素を復元する．
  s.deserialize();

  auto lib = new CiCellLibrary;
  lib->_restore(s);
  return lib;
}

void
CiCellLibrary::_restore(
  Deserializer& s
)
{
  // 名前
  s.restore(mName);

  // テクノロジ
  s.restore(mTechnology);

  // 遅延モデル
  s.restore(mDelayModel);

  // バス命名規則
  s.restore(mBusNamingStyle);

  // 日付情報
  s.restore(mDate);

  // リビジョン情報
  s.restore(mRevision);

  // コメント
  s.restore(mComment);

  // 時間単位
  s.restore(mTimeUnit);

  // 電圧単位
  s.restore(mVoltageUnit);

  // 電流単位
  s.restore(mCurrentUnit);

  // 抵抗単位
  s.restore(mPullingResistanceUnit);

  // 容量単位
  s.restore(mCapacitiveLoadUnit);

  // 容量単位の文字列
  s.restore(mCapacitiveLoadUnitStr);

  // 電力単位
  s.restore(mLeakagePowerUnit);

  // バスタイプのリスト
  s.restore(mBusTypeList);

  // LUTテンプレートのリスト
  s.restore(mLutTemplateList);

  // セルのリスト
  s.restore(mCellList);

  // セルグループのリスト
  s.restore(mCellGroupList);

  // セルクラスのリスト
  s.restore(mCellClassList);
  for ( auto& cclass: mCellClassList ) {
    cclass->set_library(this);
  }

  // 組み込み型の読み込み
  for ( auto id: Range(24) ) {
    s.restore(mLogicGroup[id]);
  }
  s.restore(mFFClassDict);
  s.restore(mLatchClassDict);

  // パタングラフの情報の設定
  mPatMgr.restore(s);

  // 最終処理
  wrap_up();
}

END_NAMESPACE_YM_CLIB
