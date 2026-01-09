#ifndef ASTCELL_H
#define ASTCELL_H

#error "don't include me"

/// @file AstCell.h
/// @brief AstCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib_nsdef.h"
#include "AstNameNode.h"
#include "AstArray.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstCell AstCell.h "AstCell.h"
/// @brief セルの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class AstCell :
  public AstNameNode
{
public:

  /// @brief コンストラクタ
  AstCell(const FileRegion& attr_loc, ///< [in] 属性のファイル上の位置
	  const StrHandler& header,   ///< [in] ヘッダを読み込んだハンドラ
	  const CellHandler& group);  ///< [in] グループ本体を読み込んだハンドラ

  /// @brief デストラクタ
  ~AstCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積を返す．
  const AstFloat*
  area() const;

  /// @brief bus_naming_style を返す．
  const AstString*
  bus_nameing_style() const;

  /// @brief ピングループのリストを返す．
  AstArray<const AstPin*>
  pin_list() const;

  /// @brief バスグループのリストを返す．
  AstArray<const AstBus*>
  bus_list() const;

  /// @brief バンドルグループのリストを返す．
  AstArray<const AstBundle*>
  bundle_list() const;

  /// @brief ff グループを返す．
  const AstFF*
  ff() const;

  /// @brief latch グループを返す．
  const AstLatch*
  latch() const;

  /// @brief statetable グループを返す．
  const AstStateTable*
  state_table() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  void
  dump(ostream& s,
       int indent = 0) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 面積
  const AstFloat* mArea;

  // "bus_naming_style"
  const AstString* mBusNamingStyle;

  // ピングループのリスト
  AstArray<const AstPin*> mPinList;

  // バスグループのリスト
  AstArray<const AstBus*> mBusList;

  // バンドルグループのリスト
  AstArray<const AstBundle*> mBundleList;

  // ff グループ
  const AstFF* mFF;

  // latch グループ
  const AstLatch* mLatch;

  // statetable グループ
  const AstStateTable* mStateTable;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 面積を返す．
inline
const AstFloat*
AstCell::area() const
{
  return mArea;
}

// @brief bus_naming_style を返す．
inline
const AstString*
AstCell::bus_nameing_style() const
{
  return mBusNamingStyle;
}

// @brief ピングループのリストを返す．
inline
AstArray<const AstPin*>
AstCell::pin_list() const
{
  return mPinList;
}

// @brief バスグループのリストの要素を返す．
inline
AstArray<const AstBus*>
AstCell::bus_list() const
{
  return mBusList;
}

// @brief バンドルグループのリストの要素を返す．
inline
AstArray<const AstBundle*>
AstCell::bundle_list() const
{
  return mBundleList;
}

// @brief ff グループを返す．
inline
const AstFF*
AstCell::ff() const
{
  return mFF;
}

// @brief latch グループを返す．
inline
const AstLatch*
AstCell::latch() const
{
  return mLatch;
}

// @brief statetable グループを返す．
inline
const AstStateTable*
AstCell::state_table() const
{
  return mStateTable;
}

END_NAMESPACE_YM_DOTLIB

#endif // ASTCELL_H
