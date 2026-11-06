#ifndef EIFUNCTION_H
#define EIFUNCTION_H

/// @file EiFunction.h
/// @brief EiFunction のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiTaskFunc.h"
#include "elaborator/RangeVal.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiFunction EiFunction.h "EiFunction.h"
/// @brief 関数を表す EiFunction の実装クラス
//////////////////////////////////////////////////////////////////////
class EiFunction :
  public EiTaskFunc
{
public:

  /// @brief コンストラクタ
  EiFunction(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem& ast_item, ///< [in] パース木の定義
    SizeType io_num,         ///< [in] IOの数
    bool const_func          ///< [in] constant function の時 true にする．
  ) : EiTaskFunc(parent, ast_item, io_num),
      mConstFunc{const_func}
  {
  }

  /// @brief デストラクタ
  ~EiFunction() = default;


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

  /// @brief constant function の時に true を返す．
  bool
  is_constant_function() const override;

  /// @brief 出力変数を返す．
  ///
  /// 出力変数とは関数名と同名の変数
  VlDecl*
  ovar() const override;


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力変数
  ElbDecl* mOvar;

  // 定数関数フラグ
  bool mConstFunc;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFunctionV EiaskFunc.h "EiTaskFunc.h"
/// @brief ベクタ型の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiFunctionV :
  public EiFunction
{
public:

  /// @brief コンストラクタ
  EiFunctionV(
    const VlScope* parent,     ///< [in] 親のスコープ
    const AstItem& ast_item,   ///< [in] パース木の定義
    SizeType io_num,           ///< [in] IOの数
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range,     ///< [in] 範囲の値
    bool const_func            ///< [in] 定数関数フラグ
  ) : EiFunction(parent, ast_item, io_num, const_func),
      mRange(ast_range, range)
  {
  }

  /// @brief デストラクタ
  ~EiFunctionV() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlFunction の仮想関数
  //////////////////////////////////////////////////////////////////////

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲の情報
  RangeInfo mRange;

};

END_NAMESPACE_YM_VERILOG

#endif // EIFUNCTION_H
