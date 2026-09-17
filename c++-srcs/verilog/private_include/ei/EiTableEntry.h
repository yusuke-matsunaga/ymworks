#ifndef EITABLEENTRY_H
#define EITABLEENTRY_H

/// @file EiTableEntry.h
/// @brief EiTableEntry のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

// UDP定義を表すクラス
// IEEE Std 1364-2001 26.6.14 UDP

#include "elaborator/ElbTableEntry.h"
#include "elaborator/ElbUdpDefn.h"
#include "ym/vl/AstUdpEntry.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiTableEntry ElbTableEntry.h "ElbTableEntry.h"
/// @brief UDP の table entry を表すクラス
//////////////////////////////////////////////////////////////////////
class EiTableEntry :
  public ElbTableEntry
{
public:

  /// @brief コンストラクタ
  EiTableEntry() = default;

  /// @brief 内容を指定したコンストラクタ
  EiTableEntry(
    ElbUdpDefn* udp,
    const AstUdpEntry& ast_entry,
    const std::vector<VlUdpVal>& vals
  ) : mUdp{udp},
      mAstUdpEntry{ast_entry},
      mValArray{vals}
  {
  }

  /// @brief デストラクタ
  ~EiTableEntry() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTableEntry の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行の要素数を返す．
  SizeType
  size() const override;

  /// @brief pos 番目の位置の値を返す．
  VlUdpVal
  val(
    SizeType pos
  ) const override;

  /// @brief 一行文の内容を表す文字列をつくる．
  std::string
  str() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の UDP
  ElbUdpDefn* mUdp;

  // パース木の定義
  AstUdpEntry mAstUdpEntry;

  // 値の配列
  std::vector<VlUdpVal> mValArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EITABLEENTRY_H
