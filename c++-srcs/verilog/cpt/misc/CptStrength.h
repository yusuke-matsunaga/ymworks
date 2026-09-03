#ifndef CPTSTRENGTH_H
#define CPTSTRENGTH_H

/// @file CptStrength.h
/// @brief CptStrength のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtStrength.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptStrength CptStrength.h "CptStrength.h"
/// @brief strength を表す基底クラス
//////////////////////////////////////////////////////////////////////
class CptStrength :
  public PtStrength
{
public:

  /// @brief コンストラクタ
  CptStrength(
    const FileRegion& file_region
  ) : mFileRegion{file_region}
  {
  }

  /// @brief デストラクタ
  ~CptStrength() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取出す．
  FileRegion
  file_region() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStrength1 CptStrength.h "CptStrength.h"
/// @brief drive strength を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStrength1 :
  public CptStrength
{
public:

  /// @brief drive strength を表すコンストラクタ
  CptStrength1(
    const FileRegion& file_region,
    VpiStrength value1,
    VpiStrength value2
  ) : CptStrength(file_region),
      mValue{value1, value2}
  {
  }

  /// @brief デストラクタ
  ~CptStrength1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief drive strength0 を返す．
  VpiStrength
  drive0() const override;

  /// @brief drive strength1 を返す．
  VpiStrength
  drive1() const override;

  /// @brief charge strength を返す．
  VpiStrength
  charge() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値の配列
  VpiStrength mValue[2];

};


//////////////////////////////////////////////////////////////////////
/// @class CptStrength2 CptStrength.h "CptStrength.h"
/// @brief charge strength を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStrength2 :
  public CptStrength
{
public:

  /// @brief コンストラクタ
  CptStrength2(
    const FileRegion& file_region,
    VpiStrength value
  ) : CptStrength(file_region),
      mValue{value}
  {
  }

  /// @brief デストラクタ
  ~CptStrength2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStrength の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief drive strength0 を返す．
  VpiStrength
  drive0() const override;

  /// @brief drive strength1 を返す．
  VpiStrength
  drive1() const override;

  /// @brief charge strength を返す．
  VpiStrength
  charge() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  VpiStrength mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTSTRENGTH_H
