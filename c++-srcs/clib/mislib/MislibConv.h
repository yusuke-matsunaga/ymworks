#ifndef MISLIBCONV_H
#define MISLIBCONV_H

/// @file MislibConv.h
/// @brief MislibConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "mislib_nsdef.h"
#include "ci/CiCellLibrary.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibConv MislibConv.h "MislibConv.h"
/// @brief mislib(genlib) を ClibCellLibrary に変換するクラス
//////////////////////////////////////////////////////////////////////
class MislibConv
{
public:

  /// @brief コンストラクタ
  MislibConv() = default;

  /// @brief デストラクタ
  ~MislibConv() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief mislib 形式のファイルを読み込んでセルライブラリを作る．
  std::unique_ptr<CiCellLibrary>
  convert(
    const std::string& filename ///< [in] ファイル名
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルを作る．
  void
  new_gate(
    const MislibGate* gate ///< [in] ゲート記述
  );

  /// @brief タイミング情報を作る．
  const CiTiming*
  add_timing(
    CiCell* cell,        ///< [in] セル
    const MislibPin* pin ///< [in] ピン記述
  );

  /// @brief タイミング情報をセットする．
  void
  set_timing(
    const MislibPin* pin,      // パース木のピン情報
    const TvFunc& tv_function, // 出力の関数
    SizeType ipos,             // 入力ピン番号
    SizeType opos,             // 出力ピン番号(常に0だけど念の為)
    CiCell* cell,              // 対象のセル
    const CiTiming* timing     // タイミング
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ライブラリ
  std::unique_ptr<CiCellLibrary> mLibrary;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBCONV_H
