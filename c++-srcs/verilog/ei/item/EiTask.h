#ifndef EITASK_H
#define EITASK_H

/// @file EiTaskFunc.h
/// @brief EiTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief タスクを表す EiTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTask :
  public EiTaskFunc
{
public:

  /// @brief コンストラクタ
  EiTask(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem& ast_item, ///< [in] パース木の定義
    SizeType io_num          ///< [in] IOの数
  ) : EiTaskFunc(parent, ast_item, io_num)
  {
  }

  /// @brief デストラクタ
  ~EiTask() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTaskFunc の仮想関数(関数の時のみ意味を持つもの)
  //////////////////////////////////////////////////////////////////////

  /// @brief function type を返す．
  VpiFuncType
  func_type() const override;

  /// @brief 符号付きの時 true を返す．
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief 出力のビット幅を返す．
  SizeType
  bit_size() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  // ただし Function の時しか意味を持たない．
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力変数をセットする．
  void
  set_ovar(
    ElbDecl* ovar ///< [in] 出力変数
  ) override;

  /// @brief constant function の時に true を返す．
  bool
  is_constant_function() const override;

  /// @brief 出力変数を返す．
  ///
  /// 出力変数とは関数名と同名の変数
  VlDecl*
  ovar() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EITASK_H
