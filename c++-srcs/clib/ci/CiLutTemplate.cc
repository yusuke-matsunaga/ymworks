
/// @file CiLutTemplate.cc
/// @brief CiLutTemplate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiLutTemplate.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"
#include "CiLutTemplate_sub.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplate
//////////////////////////////////////////////////////////////////////

// @brief 1次元のテンプレートを生成するクラスメソッド
std::unique_ptr<CiLutTemplate>
CiLutTemplate::new_1D(
  ClibVarType var_type,
  const std::vector<double>& index_array
)
{
  auto ptr = new CiLutTemplate1D{
    var_type, index_array
  };
  return std::unique_ptr<CiLutTemplate>{ptr};
}

// @brief 2次元のテンプレートを生成するクラスメソッド
std::unique_ptr<CiLutTemplate>
CiLutTemplate::new_2D(
  ClibVarType var1,
  const std::vector<double>& index_array1,
  ClibVarType var2,
  const std::vector<double>& index_array2
)
{
  auto ptr = new CiLutTemplate2D{
    var1, index_array1,
    var2, index_array2
  };
  return std::unique_ptr<CiLutTemplate>{ptr};
}

// @brief 3次元のテンプレートを生成するクラスメソッド
std::unique_ptr<CiLutTemplate>
CiLutTemplate::new_3D(
  ClibVarType var1,
  const std::vector<double>& index_array1,
  ClibVarType var2,
  const std::vector<double>& index_array2,
  ClibVarType var3,
  const std::vector<double>& index_array3
)
{
  auto ptr = new CiLutTemplate3D{
    var1, index_array1,
    var2, index_array2,
    var3, index_array3
  };
  return std::unique_ptr<CiLutTemplate>{ptr};
}

// @brief 内容をシリアライズする．
void
CiLutTemplate::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
}

// @brief 1つの変数の情報をバイナリダンプする．
void
CiLutTemplate::dump_var(
  Serializer& s,
  ClibVarType var_type,
  const std::vector<double>& index_array
)
{
  s.dump(var_type);
  s.dump(index_array);
}


// @brief 内容を復元する．
std::unique_ptr<CiLutTemplate>
CiLutTemplate::restore(
  Deserializer& s
)
{
  std::uint8_t d;
  s.restore(d);
  CiLutTemplate* tmpl = nullptr;
  switch ( d ) {
  case 1: tmpl = new CiLutTemplate1D; break;
  case 2: tmpl = new CiLutTemplate2D; break;
  case 3: tmpl = new CiLutTemplate3D; break;
  default:
    ASSERT_NOT_REACHED;
  }
  auto ptr = std::unique_ptr<CiLutTemplate>{tmpl};
  ptr->_restore(s);
  return ptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplate1D
//////////////////////////////////////////////////////////////////////

// @brief 次元数の取得
SizeType
CiLutTemplate1D::dimension() const
{
  return 1;
}

// @brief 変数型の取得
ClibVarType
CiLutTemplate1D::variable_type(
  SizeType var
) const
{
  if ( var < 0 || 1 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mVarType;
}

// @brief インデックス数の取得
SizeType
CiLutTemplate1D::index_num(
  SizeType var
) const
{
  if ( var < 0 || 1 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mIndexArray.size();
}

// @brief インデックス配列の取得
const std::vector<double>&
CiLutTemplate1D::index_array(
  SizeType var
) const
{
  if ( var < 0 || 1 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mIndexArray;
}

// @brief 内容をバイナリダンプする．
void
CiLutTemplate1D::dump(
  Serializer& s
) const
{
  s.dump(static_cast<std::uint8_t>(1));
  dump_var(s, mVarType, mIndexArray);
}

// @brief restore() の本体
void
CiLutTemplate1D::_restore(
  Deserializer& s
)
{
  s.restore(mVarType);
  s.restore(mIndexArray);
}


//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplate2D
//////////////////////////////////////////////////////////////////////

// @brief 次元数の取得
SizeType
CiLutTemplate2D::dimension() const
{
  return 2;
}

// @brief 変数型の取得
ClibVarType
CiLutTemplate2D::variable_type(
  SizeType var
) const
{
  if ( var < 0 || 2 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mVarType[var];
}

// @brief インデックス数の取得
SizeType
CiLutTemplate2D::index_num(
  SizeType var
) const
{
  if ( var < 0 || 2 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mIndexArray[var].size();
}

// @brief インデックス配列の取得
const std::vector<double>&
CiLutTemplate2D::index_array(
  SizeType var
) const
{
  if ( var < 0 || 2 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mIndexArray[var];
}

// @brief 内容をバイナリダンプする．
void
CiLutTemplate2D::dump(
  Serializer& s
) const
{
  s.dump(static_cast<std::uint8_t>(2));
  for ( SizeType i: Range(2) ) {
    dump_var(s, mVarType[i], mIndexArray[i]);
  }
}

// @brief restore() の本体
void
CiLutTemplate2D::_restore(
  Deserializer& s
)
{
  for ( SizeType i: Range(2) ) {
    s.restore(mVarType[i]);
    s.restore(mIndexArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplate3D
//////////////////////////////////////////////////////////////////////

// @brief 次元数の取得
SizeType
CiLutTemplate3D::dimension() const
{
  return 3;
}

// @brief 変数型の取得
ClibVarType
CiLutTemplate3D::variable_type(
  SizeType var
) const
{
  if ( var < 0 || 3 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mVarType[var];
}

// @brief インデックス数の取得
SizeType
CiLutTemplate3D::index_num(
  SizeType var
) const
{
  if ( var < 0 || 3 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mIndexArray[var].size();
}

// @brief インデックス配列の取得
const std::vector<double>&
CiLutTemplate3D::index_array(
  SizeType var
) const
{
  if ( var < 0 || 3 <= var ) {
    throw std::out_of_range{"var is out of range"};
  }
  return mIndexArray[var];
}

// @brief 内容をバイナリダンプする．
void
CiLutTemplate3D::dump(
  Serializer& s
) const
{
  s.dump(static_cast<std::uint8_t>(3));
  for ( SizeType i: Range(2) ) {
    dump_var(s, mVarType[i], mIndexArray[i]);
  }
}

// @brief restore() の本体
void
CiLutTemplate3D::_restore(
  Deserializer& s
)
{
  for ( SizeType i: Range(3) ) {
    s.restore(mVarType[i]);
    s.restore(mIndexArray[i]);
  }
}

END_NAMESPACE_YM_CLIB
