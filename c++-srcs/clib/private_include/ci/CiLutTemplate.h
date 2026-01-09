#ifndef CILUTTEMPLATE_H
#define CILUTTEMPLATE_H

/// @file CiLutTemplate.h
/// @brief CiLutTemplate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_CLIB

class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiLutTemplate CiLutTemplate.h "CiLutTemplate.h"
/// @brief CiLutTemplateXXX の(擬似)基底クラス
//////////////////////////////////////////////////////////////////////
class CiLutTemplate
{
protected:

  /// @brief コンストラクタ
  CiLutTemplate() = default;


public:

  /// @brief 1次元のテンプレートを生成するクラスメソッド
  static
  std::unique_ptr<CiLutTemplate>
  new_1D(
    ClibVarType var_type,
    const std::vector<double>& index_array
  );

  /// @brief 2次元のテンプレートを生成するクラスメソッド
  static
  std::unique_ptr<CiLutTemplate>
  new_2D(
    ClibVarType var1,
    const std::vector<double>& index_array1,
    ClibVarType var2,
    const std::vector<double>& index_array2
  );

  /// @brief 3次元のテンプレートを生成するクラスメソッド
  static
  std::unique_ptr<CiLutTemplate>
  new_3D(
    ClibVarType var1,
    const std::vector<double>& index_array1,
    ClibVarType var2,
    const std::vector<double>& index_array2,
    ClibVarType var3,
    const std::vector<double>& index_array3
  );

  /// @brief デストラクタ
  virtual
  ~CiLutTemplate() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 標準タイプの時 true を返す．
  ///
  /// 標準タイプは以下の条件
  /// - dimension() == 2
  /// - variable_type(0) == ClibVarType::total_output_net_capacitance
  /// - variable_type(1) == ClibVarType::input_net_transition
  bool
  is_standard_type() const
  {
    if ( dimension() == 2 &&
	 variable_type(0) == ClibVarType::total_output_net_capacitance &&
	 variable_type(1) == ClibVarType::input_net_transition ) {
      return true;
    }
    return false;
  }

  /// @brief 次元数の取得
  virtual
  SizeType
  dimension() const = 0;

  /// @brief 変数型の取得
  virtual
  ClibVarType
  variable_type(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const = 0;

  /// @brief インデックス数の取得
  virtual
  SizeType
  index_num(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const = 0;

  /// @brief インデックス配列の取得
  virtual
  const std::vector<double>&
  index_array(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をシリアライズする．
  void
  serialize(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容をバイナリダンプする．
  virtual
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const = 0;

  /// @brief 内容を復元する．
  static
  std::unique_ptr<CiLutTemplate>
  restore(
    Deserializer& s ///< [in] デシリアライザ
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1つの変数の情報をバイナリダンプする．
  static
  void
  dump_var(
    Serializer& s,                         ///< [in] シリアライザ
    ClibVarType var_type,                  ///< [in] 変数のタイプ
    const std::vector<double>& index_array ///< [in] インデックスの配列
  );

  /// @brief restore() の本体
  virtual
  void
  _restore(
    Deserializer& s
  ) = 0;

};

END_NAMESPACE_YM_CLIB

#endif // CILUTTEMPLATE_H
