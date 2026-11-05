#ifndef EIGENBLOCKSCOPE_H
#define EIGENBLOCKSCOPE_H

/// @file EiGenBlockScope.h
/// @brief EiGenBlockScope のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiScope.h"
#include "ym/vl/AstItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiGenBlockScope EiGenBlockScope.h "ei/EiGenBlockScope.h"
/// @brief generate block に対応するスコープ
//////////////////////////////////////////////////////////////////////
class EiGenBlockScope :
  public EiScope
{
public:

  /// @brief コンストラクタ
  EiGenBlockScope(
    const VlScope* parent,  ///< [in] 親のスコープ環境
    const AstItem& ast_item ///< [in] 対応するパース木の要素
  ) : EiScope(parent),
      mAstItem{ast_item}
  {
  }

  /// @brief デストラクタ
  ~EiGenBlockScope() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlScope の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応するパース木の要素
  AstItem mAstItem;

};


//////////////////////////////////////////////////////////////////////
/// @class EiGfBlockScope EiGenBlockScope.h "EiGenBlockScope.h"
/// @brief generate for 文に対応するスコープ
//////////////////////////////////////////////////////////////////////
class EiGfBlockScope :
  public EiGenBlockScope
{
public:

  /// @brief コンストラクタ
  EiGfBlockScope(
    const VlScope* parent,   ///< [in] 親のスコープ環境
    const AstItem& ast_item, ///< [in] 対応するパース木の要素
    int index                ///< [in] インデックス
  ) : EiGenBlockScope(parent, ast_item),
      mIndex{index}
  {
  }

  /// @brief デストラクタ
  ~EiGfBlockScope() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  int mIndex;

};

END_NAMESPACE_YM_VERILOG

#endif // EIGENBLOCKSCOPE_H
