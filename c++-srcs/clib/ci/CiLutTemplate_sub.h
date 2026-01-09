#ifndef CILUTTEMPLATE_SUB_H
#define CILUTTEMPLATE_SUB_H

/// @file　CiLutTemplate_sub.h
/// @brief CiLutTemplate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiLutTemplate.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class CiLutTemplate1D CiLutTemplate.h "CiLutTemplate.h"
/// @brief 1次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CiLutTemplate1D :
  public CiLutTemplate
{
public:

  /// @brief 空のコンストラクタ
  CiLutTemplate1D() = default;

  /// @brief コンストラクタ
  CiLutTemplate1D(
    ClibVarType var_type,
    const std::vector<double>& index_array
  ) : mVarType{var_type},
      mIndexArray{index_array}
  {
  }

  /// @brief デストラクタ
  ~CiLutTemplate1D() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数の取得
  SizeType
  dimension() const override;

  /// @brief 変数型の取得
  ClibVarType
  variable_type(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;

  /// @brief インデックス数の取得
  SizeType
  index_num(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;

  /// @brief インデックス配列の取得
  const std::vector<double>&
  index_array(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;

protected:

  /// @brief restore() の本体
  void
  _restore(
    Deserializer& s
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  ClibVarType mVarType;

  // デフォルトインデックス値の配列
  std::vector<double> mIndexArray;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLutTemplate2D CiLutTemplate.h "CiLutTemplate.h"
/// @brief 2次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CiLutTemplate2D :
  public CiLutTemplate
{
public:

  /// @brief 空のコンストラクタ
  CiLutTemplate2D() = default;

  /// @brief コンストラクタ
  CiLutTemplate2D(
    ClibVarType var1,
    const std::vector<double>& index_array1,
    ClibVarType var2,
    const std::vector<double>& index_array2
  ) : mVarType{var1, var2},
      mIndexArray{index_array1, index_array2}
  {
  }

  /// @brief デストラクタ
  ~CiLutTemplate2D() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数の取得
  SizeType
  dimension() const override;

  /// @brief 変数型の取得
  ClibVarType
  variable_type(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;

  /// @brief インデックス数の取得
  SizeType
  index_num(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;

  /// @brief インデックス配列の取得
  const std::vector<double>&
  index_array(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;

protected:

  /// @brief restore() の本体
  void
  _restore(
    Deserializer& s
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  ClibVarType mVarType[2];

  // デフォルトインデックス値の配列
  std::vector<double> mIndexArray[2];

};


//////////////////////////////////////////////////////////////////////
/// @class CiLutTemplate3D CiLutTemplate.h "CiLutTemplate.h"
/// @brief 3次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CiLutTemplate3D :
  public CiLutTemplate
{
public:

  /// @brief 空のコンストラクタ
  CiLutTemplate3D() = default;

  /// @brief コンストラクタ
  CiLutTemplate3D(
    ClibVarType var1,
    const std::vector<double>& index_array1,
    ClibVarType var2,
    const std::vector<double>& index_array2,
    ClibVarType var3,
    const std::vector<double>& index_array3
  ) : mVarType{var1, var2, var3},
      mIndexArray{index_array1, index_array2, index_array3}
  {
  }

  /// @brief デストラクタ
  ~CiLutTemplate3D() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数の取得
  SizeType
  dimension() const override;

  /// @brief 変数型の取得
  ClibVarType
  variable_type(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;

  /// @brief インデックス数の取得
  SizeType
  index_num(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;

  /// @brief インデックス配列の取得
  const std::vector<double>&
  index_array(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;

protected:

  /// @brief restore() の本体
  void
  _restore(
    Deserializer& s
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  ClibVarType mVarType[3];

  // デフォルトインデックス値の配列
  std::vector<double> mIndexArray[3];

};

END_NAMESPACE_YM_CLIB

#endif // CILUTTEMPLATE_H
