#ifndef CPTITEM_MUH_H
#define CPTITEM_MUH_H

/// @file CptItem_MuH.h
/// @brief CptItem_MuH のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
//#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptItem_MuH CptItem_MuH.h  "CptItem_MuH.h"
/// @brief module/udp header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptItem_MuH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptItem_MuH(
    const FileRegion& file_region,
    const char* def_name,
    const PtInst* inst_top
  ) : mFileRegion{file_region},
      mName{def_name},
      mInstTop{inst_top}
  {
  }

  /// @brief デストラクタ
  ~CptItem_MuH() {}


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

  /// @brief 定義名を返す．
  const char*
  name() const override;

  /// @brief パラメータ割り当てのリストの先頭の取得
  const PtConnection*
  paramassign_top() const override;

  /// @brief strength を返す．
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  const PtDelay*
  delay() const override;

  /// @brief module/UDP/gate instance リストの先頭の取得
  const PtInst*
  inst_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 定義名
  const char* mName;

  // 要素の先頭
  const PtInst* mInstTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief パラメータ割り当てつきの CptItem_MuH
//////////////////////////////////////////////////////////////////////
class CptItem_MuHP :
  public CptItem_MuH
{
public:

  /// @brief コンストラクタ
  CptItem_MuHP(
    const FileRegion& file_region,
    const char* def_name,
    const PtConnection* con_top,
    const PtInst* inst_top
  ) : CptItem_MuH(file_region, def_name, inst_top),
      mParamAssignTop{con_top}
  {
  }

  /// @brief デストラクタ
  ~CptItem_MuHP() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ割り当てのリストの先頭の取得
  const PtConnection*
  paramassign_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ割り当ての先頭
  const PtConnection* mParamAssignTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptItem_MuHS :
  public CptItem_MuH
{
public:

  /// @brief コンストラクタ
  CptItem_MuHS(
    const FileRegion& file_region,
    const char* def_name,
    const PtStrength* strength,
    const PtInst* inst_top
  ) : CptItem_MuH(file_region, def_name, inst_top),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptItem_MuHS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
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
/// @brief delay を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptItem_MuHD :
  public CptItem_MuH
{
public:

  /// @brief コンストラクタ
  CptItem_MuHD(
    const FileRegion& file_region,
    const char* def_name,
    const PtDelay* delay,
    const PtInst* inst_top
  ) : CptItem_MuH(file_region, def_name, inst_top),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptItem_MuHD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
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
/// @brief strength と delay を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptItem_MuHSD :
  public CptItem_MuH
{
public:

  /// @brief コンストラクタ
  CptItem_MuHSD(
    const FileRegion& file_region,
    const char* def_name,
    const PtStrength* strength,
    const PtDelay* delay,
    const PtInst* inst_top
  ) : CptItem_MuH(file_region, def_name, inst_top),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptItem_MuHSD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
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

#endif // CPTITEM_MUH_H
