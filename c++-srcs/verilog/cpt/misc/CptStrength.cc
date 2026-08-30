
/// @file CptStrength.cc
/// @brief CptStrength の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStrength.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptStrength
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置を取出す．
FileRegion
CptStrength::file_region() const
{
  return mFileRegion;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStrength1
//////////////////////////////////////////////////////////////////////

// drive strength0 を返す．
VpiStrength
CptStrength1::drive0() const
{
  return mValue[0];
}

// drive strength1 を返す．
VpiStrength
CptStrength1::drive1() const
{
  return mValue[1];
}

// charge strength を返す．
VpiStrength
CptStrength1::charge() const
{
  return VpiStrength::NoStrength;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStrength2
//////////////////////////////////////////////////////////////////////

// drive strength0 を返す．
VpiStrength
CptStrength2::drive0() const
{
  return VpiStrength::NoStrength;
}

// drive strength1 を返す．
VpiStrength
CptStrength2::drive1() const
{
  return VpiStrength::NoStrength;
}

// charge strength を返す．
VpiStrength
CptStrength2::charge() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// drive strength を生成する．
PtStrength*
PtFactory::new_Strength(
  const FileRegion& file_region,
  VpiStrength value1,
  VpiStrength value2
)
{
  void* p = mAlloc.get_memory(sizeof(CptStrength1));
  return new (p) CptStrength1(file_region, value1, value2);
}

// charge strength を生成する．
PtStrength*
PtFactory::new_Strength(
  const FileRegion& file_region,
  VpiStrength value1
)
{
  void* p = mAlloc.get_memory(sizeof(CptStrength2));
  return new (p) CptStrength2(file_region, value1);
}

END_NAMESPACE_YM_VERILOG
