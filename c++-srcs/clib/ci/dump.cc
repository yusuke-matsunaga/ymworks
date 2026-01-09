
/// @file CiCellLibrary_dump.cc
/// @brief CiCellLibrary の実装ファイル(dump()関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCellLibrary.h"
#include "ci/CiCellClass.h"
#include "ci/CiCellGroup.h"
#include "ci/CiCell.h"
#include "ci/CiBusType.h"
#include "ci/CiLutTemplate.h"
#include "ci/CiLut.h"
#include "ci/CiTiming.h"
#include "ci/CiPatGraph.h"
#include "ci/Serializer.h"
#include "ym/ClibIOMap.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiCellLibrary
//////////////////////////////////////////////////////////////////////

// @brief 内容をバイナリダンプする．
void
CiCellLibrary::dump(
  std::ostream& os
) const
{
  Serializer s{os};

  // 要素のシリアライズを行う．
  serialize(s);

  // シリアライズで登録された要素の内容をダンプする．
  s.dump_obj();

  // 名前
  s.dump(name());

  // テクノロジ
  s.dump(technology());

  // 遅延モデル
  s.dump(delay_model());

  // バス命名規則
  s.dump(bus_naming_style());

  // 日付情報
  s.dump(date());

  // リビジョン情報
  s.dump(revision());

  // コメント
  s.dump(comment());

  // 時間単位
  s.dump(time_unit());

  // 電圧単位
  s.dump(voltage_unit());

  // 電流単位
  s.dump(current_unit());

  // 抵抗単位
  s.dump(pulling_resistance_unit());

  // 容量単位
  s.dump(capacitive_load_unit());

  // 容量単位の文字列
  s.dump(capacitive_load_unit_str());

  // 電力単位
  s.dump(leakage_power_unit());

  // バスタイプのリスト
  s.dump(mBusTypeList);

  // 遅延テーブルのテンプレートのリスト
  s.dump(mLutTemplateList);

  // セルのリスト
  s.dump(mCellList);

  // セルグループ情報のリスト
  s.dump(mCellGroupList);

  // セルクラス情報のリスト
  s.dump(mCellClassList);

  // 組み込み型の情報のダンプ
  for ( auto g: mLogicGroup ) {
    s.dump(g);
  }
  s.dump(mFFClassDict);
  s.dump(mLatchClassDict);

  // パタングラフの情報のダンプ
  mPatMgr.dump(s);
}

// @brief 要素をシリアライズする．
void
CiCellLibrary::serialize(
  Serializer& s
) const
{
  for ( auto& bustype: mBusTypeList ) {
    bustype->serialize(s);
  }
  for ( auto& templ: mLutTemplateList ) {
    templ->serialize(s);
  }
  for ( auto& cell: mCellList ) {
    cell->serialize(s);
  }
  for ( auto& cgroup: mCellGroupList ) {
    cgroup->serialize(s);
  }
  for ( auto& cclass: mCellClassList ) {
    cclass->serialize(s);
  }
}

END_NAMESPACE_YM_CLIB
