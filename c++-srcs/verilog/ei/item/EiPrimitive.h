#ifndef EIPRIMITIVE_H
#define EIPRIMITIVE_H

/// @file EiPrimitive.h
/// @brief EiPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimitive EiPrimitive.h "EiPrimitive.h"
/// @brief 単一要素の EiPrimitive
//////////////////////////////////////////////////////////////////////
class EiPrimitive :
  public EiPrimBase
{
public:

  /// @brief コンストラクタ
  EiPrimitive(
    ElbPrimHead* head,    ///< [in] ヘッダ
    const AstInst& ast_inst ///< [in] インスタンス定義
  ) : mHead{head},
      mAstInst{ast_inst}
  {
    init_port(ast_inst.port_list().size());
  }

  /// @brief コンストラクタ
  EiPrimitive(
    ElbPrimHead* head,      ///< [in] ヘッダ
    const ClibCell& cell,   ///< [in] セル
    const AstInst& ast_inst ///< [in] インスタンス定義
  ) : mHead{head},
      mAstInst{ast_inst}
  {
    init_port(ast_inst.port_list().size(), cell);
  }

  /// @brief デストラクタ
  ~EiPrimitive() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを得る．
  ElbPrimHead*
  head() const override;

  /// @brief パース木のインスタンス定義を得る．
  AstInst
  ast_inst() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbPrimHead* mHead;

  // パース木のインスタンス定義
  AstInst mAstInst;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMITIVE_H
