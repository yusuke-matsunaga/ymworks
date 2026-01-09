#ifndef VLDUMPER_H
#define VLDUMPER_H

/// @file VlDumper.h
/// @brief VlDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

class VlMgr;
class VlDumperImpl;

//////////////////////////////////////////////////////////////////////
/// @class VlDumper VlDumper.h <ym/vl/VlDumper.h>
/// @ingroup Vlgroup
/// @brief VlObj の出力用クラス
//////////////////////////////////////////////////////////////////////
class VlDumper
{
public:

  /// @brief コンストラクタ
  VlDumper(
    std::ostream& s ///< [in] 出力ストリーム
  );

  /// @brief デストラクタ
  ~VlDumper();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を出力する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief VlMgr の持っている内容を出力する
  void
  operator()(
    const VlMgr& vlmgr ///< [in] マネージャ
  );


public:
  //////////////////////////////////////////////////////////////////////
  // オプションの設定/取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置情報を出力するモードにする
  void
  enable_file_loc_mode();

  /// @brief ファイル位置情報の出力を抑制するモードにする．
  void
  disable_file_loc_mode();

  /// @brief ファイル位置情報を出力するモードのとき true を返す．
  bool
  file_loc_mode() const;

  /// @brief nullptr ポインタの出力を抑止する．
  void
  enable_nullptr_suppress_mode();

  /// @brief nullptr ポインタの出力を行う．
  void
  disable_nullptr_suppress_mode();

  /// @brief nullptr ポインタの出力抑止モードの状態を得る．
  bool
  nullptr_suppress_mode() const;

  /// @brief bit 展開した内容を出力する．
  void
  enable_bitexpand_mode();

  /// @brief bit 展開しない．
  void
  disable_bitexpand_mode();

  /// @brief bit 展開モードの時 true を返す．
  bool
  bitexpand_mode() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の実装クラス
  std::unique_ptr<VlDumperImpl> mImpl;

};


END_NAMESPACE_YM_VERILOG

#endif // VLDUMPER_H
