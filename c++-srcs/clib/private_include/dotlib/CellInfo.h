#ifndef CELLINFO_H
#define CELLINFO_H

/// @file CellInfo.h
/// @brief CellInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "dotlib/ElemInfo.h"
#include "dotlib/FFInfo.h"
#include "dotlib/LatchInfo.h"
#include "dotlib/FSMInfo.h"
#include "dotlib/PinInfo.h"
#include "ym/ShString.h"
#include "ym/clib.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class CellInfo CellInfo.h "CellInfo.h"
/// @brief セルのパース情報
//////////////////////////////////////////////////////////////////////
class CellInfo :
  public ElemInfo
{
public:

  /// @brief ピン名からピン番号を得る辞書の型
  using PinMap = std::unordered_map<ShString, SizeType>;

public:

  /// @brief コンストラクタ
  CellInfo(
    LibraryInfo& library_info ///< [in] ライブラリのパース情報
  ) : ElemInfo{library_info},
      mFFInfo{library_info},
      mLatchInfo{library_info},
      mFSMInfo{library_info}
  {
  }

  /// @brief デストラクタ
  ~CellInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(
    const AstValue* cell_val ///< [in] セル情報のパース木
  );

  /// @brief セルを作る．
  void
  add_cell();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積を取り出す．
  ///
  /// 値は mArea にセットされる．
  void
  set_area();

  /// @brief FF グループの情報を取り出す．
  void
  set_FF();

  /// @brief Latch グループの情報を取り出す．
  void
  set_Latch();

  /// @brief FSM グループの情報を取り出す．
  void
  set_FSM();

  /// @brief pin グループの情報を取り出す．
  void
  set_pin();

  /// @brief FF セルを作る．
  CiCell*
  add_ff_cell() const;

  /// @brief ラッチセルを作る．
  CiCell*
  add_latch_cell() const;

  /// @brief FSM セルを作る．
  CiCell*
  add_fsm_cell() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 面積
  ClibArea mArea;

  // FF 関係の情報
  bool mHasFF{false};
  FFInfo mFFInfo;

  // ラッチ関係の情報
  bool mHasLatch{false};
  LatchInfo mLatchInfo;

  // FSM 関係の情報
  bool mHasFSM{false};
  FSMInfo mFSMInfo;

  // ピンの情報
  std::vector<PinInfo> mPinInfoList;

  // 割当済みの入力ピン番号
  SizeType mInputId;

  // 割当済みの出力ピン番号
  SizeType mOutputId;

  // ピン名をキーにして入力ピン番号を格納する辞書
  PinMap mIpinMap;

  // ピン名をキーにして出力ピン番号を格納する辞書
  PinMap mOpinMap;

};

END_NAMESPACE_YM_DOTLIB

#endif // CELLINFO_H
