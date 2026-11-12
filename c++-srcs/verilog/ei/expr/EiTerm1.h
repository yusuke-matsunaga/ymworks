#ifndef EITERM1_H
#define EITERM1_H

/// @file EiTerm1.h
/// @brief EiTerm1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlTerm.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiTerm1 EiTerm1.h "EiTerm1.h"
/// @brief 単一要素の EiTerm
//////////////////////////////////////////////////////////////////////
class EiTerm1 :
  public VlTerm
{
public:

  /// @brief コンストラクタ
  EiTerm1(
    const VlExpr* primary    ///< [in] プライマリ
  ) : mPrimary{primary}
  {
  }

  /// @brief デストラクタ
  ~EiTerm1() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTerm の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Verilog-HDL の文字列を得る．
  std::string
  decompile() const override;

  /// @brief 式の値のタイプを返す．
  ///
  /// このタイプで評価する．
  VlValueType
  value_type() const override;

  /// @brief 式のビット幅を返す．
  ///
  /// value_type().size() を同じ
  SizeType
  bit_size() const override;

  /// @brief 要素数を返す．
  SizeType
  elem_num() const override;

  /// @brief 要素を返す．
  const VlExpr*
  elem(
    SizeType index ///< [in] インデックス ( 0 <= index < elem_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // プライマリ
  const VlExpr* mPrimary;

};

END_NAMESPACE_YM_VERILOG

#endif // EITERM1_H
