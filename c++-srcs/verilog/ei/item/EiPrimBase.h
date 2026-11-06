#ifndef EIPRIMBASE_H
#define EIPRIMBASE_H

/// @file EiPrimBase.h
/// @brief EiPrimBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbPrimitive.h"
#include "ym/ClibCell.h"
#include "ym/vl/AstInst.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiPrimTerm;

//////////////////////////////////////////////////////////////////////
/// @class EiPrimBase EiPrimBase.h "EiPrimBase.h"
/// @brief ElbPrimitive の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimBase :
  public ElbPrimitive
{
public:

  /// @brief コンストラクタ
  EiPrimBase() = default;

  /// @brief デストラクタ
  ~EiPrimBase() = default;


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
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  std::string
  def_name() const override;

  /// @brief UPD 定義を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;

  /// @brief 0 の強さを得る．
  VpiStrength
  drive0() const override;

  /// @brief 1 の強さを得る．
  VpiStrength
  drive1() const override;

  /// @brief 遅延式を得る．
  const VlDelay*
  delay() const override;

  /// @brief ポート数を得る．
  SizeType
  port_num() const override;

  /// @brief ポート端子を得る．
  const VlPrimTerm*
  prim_term(
    SizeType pos ///< [in] 位置番号 (0 <= pos < port_num())
  ) const override;

  /// @brief ポート端子のリストを得る．
  std::vector<const VlPrimTerm*>
  prim_term_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 接続する．
  void
  connect(
    SizeType pos,      ///< [in] ポート番号 (0 から始まる)
    const VlExpr* expr ///< [in] 接続する式
  ) override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 初期化用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ポート配列を初期化する．
  void
  init_port(
    SizeType port_num ///< [in] ポート数
  );

  /// @brief ポート配列を初期化する．
  void
  init_port(
    SizeType port_num,   ///< [in] ポート数
    const ClibCell& cell ///< [in] セル
  );


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パース木のインスタンス定義を得る．
  virtual
  AstInst
  ast_inst() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポートの実体の配列
  std::vector<EiPrimTerm> mPortArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMBASE_H
