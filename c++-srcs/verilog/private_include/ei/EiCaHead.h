#ifndef EICAHEAD_H
#define EICAHEAD_H

/// @file EiCaHead.h
/// @brief EiCaHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbCaHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiCaHead EiCaHead.h "ei/EiCaHead.h"
/// @brief 継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class EiCaHead :
  public ElbCaHead
{
public:

  /// @brief コンストラクタ
  EiCaHead(
    const VlModule* module, ///< [in] 親のモジュール
    const AstItem& ast_head ///< [in] パース木のヘッダ定義
  );

  /// @brief デストラクタ
  ~EiCaHead();


public:
  //////////////////////////////////////////////////////////////////////
  // EiCaHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  module() const override;

  /// @brief 0の強さを返す．
  VpiStrength
  drive0() const override;

  /// @brief 1の強さを返す．
  VpiStrength
  drive1() const override;

  /// @brief 遅延を表す式を返す．
  /// @note このクラスでは nullptr を返す．
  const VlDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

  // パース木のヘッダ定義
  AstItem mAstHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCaHeadD EiCaHead.h "ei/EiCaHead.h"
/// @brief 遅延付きの継続的代入文のヘッダ
//////////////////////////////////////////////////////////////////////
class EiCaHeadD :
  public EiCaHead
{
public:

  /// @brief コンストラクタ
  EiCaHeadD(
    const VlModule* module,  ///< [in] 親のモジュール
    const AstItem& ast_head, ///< [in] パース木のヘッダ定義
    const VlDelay* delay     ///< [in] 遅延値
  );

  /// @brief デストラクタ
  ~EiCaHeadD();


public:
  //////////////////////////////////////////////////////////////////////
  // EiCaHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延を表す式を返す．
  const VlDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延値
  const VlDelay* mDelay;

};

END_NAMESPACE_YM_VERILOG

#endif // EICAHEAD_H
