#ifndef EIPRIMTERM_H
#define EIPRIMTERM_H

/// @file EiPrimTerm.h
/// @brief EiPrimTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlPrimitive.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimTerm EiPrimTerm.h "EiPrimTerm.h"
/// @brief ElbPrimTerm の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimTerm :
  public VlPrimTerm
{
public:

  /// @brief コンストラクタ
  EiPrimTerm() = default;

  /// @brief デストラクタ
  ~EiPrimTerm() = default;


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
  // VlPrimTerm の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のプリミティブを返す．
  const VlPrimitive*
  primitive() const override;

  /// @brief 入出力の種類を返す．
  VpiDir
  direction() const override;

  /// @brief 端子番号を返す．
  SizeType
  term_index() const override;

  /// @brief 接続しているネットを表す式を返す．
  const VlExpr*
  expr() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(
    const VlPrimitive* primitive,
    SizeType index,
    VpiDir dir
  );

  /// @brief 接続している式を設定する．
  void
  set_expr(
    const VlExpr* expr
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のプリミティブ
  const VlPrimitive* mPrimitive;

  // インデックス + 方向(3bit)
  int mIndexDir;

  // 接続している式
  const VlExpr* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMTERM_H
