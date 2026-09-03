#ifndef CPTITEM_GATEH_H
#define CPTITEM_GATEH_H

/// @file CptItem_GateH.h
/// @brief CptItem_GateH のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptItem_GateH CptItem_GateH.h "CptItem_GateH.h"
/// @brief gate header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptItem_GateH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptItem_GateH(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const PtInst* inst_top
  ) : mFileRegion{file_region},
      mPrimType{prim_type},
      mInstTop{inst_top}
  {
  }

  /// @brief デストラクタ
  ~CptItem_GateH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

  /// @brief strength の取得
  /// @return 信号強度
  const PtStrength*
  strength() const override;

  /// @brief delay の取得
  /// @return 遅延
  const PtDelay*
  delay() const override;

  /// @brief プリミティブタイプを返す．
  VpiPrimType
  prim_type() const override;

  /// @brief module/UDP/gate instance リストの先頭の取得
  const PtInst*
  inst_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // プリミティブタイプ
  VpiPrimType mPrimType;

  // 要素の先頭
  const PtInst* mInstTop;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_GateHS CptItem_GateH.h "CptItem_GateH.h"
/// @brief strength をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_GateHS :
  public CptItem_GateH
{
public:

  /// @brief コンストラクタ
  CptItem_GateHS(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const PtStrength* strength,
    const PtInst* inst_top
  ) : CptItem_GateH(file_region, prim_type, inst_top),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptItem_GateHS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const PtStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_GateHD CptItem_GateH.h "CptItem_GateH.h"
/// @brief delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_GateHD :
  public CptItem_GateH
{
public:

  /// @brief コンストラクタ
  CptItem_GateHD(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const PtDelay* delay,
    const PtInst* inst_top
  ) : CptItem_GateH(file_region, prim_type, inst_top),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptItem_GateHD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_GateHSD CptItem_GateH.h "CptItem_GateH.h"
/// @brief strength と delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_GateHSD :
  public CptItem_GateH
{
public:

  /// @brief コンストラクタ
  CptItem_GateHSD(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const PtStrength* strength,
    const PtDelay* delay,
    const PtInst* inst_top
  ) : CptItem_GateH(file_region, prim_type, inst_top),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptItem_GateHSD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_GATEH_H
