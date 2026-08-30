#ifndef CPTITEM_MUH_H
#define CPTITEM_MUH_H

/// @file CptItem_MuH.h
/// @brief CptItem_MuH のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtMisc.h"


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
    PtInst* inst_top
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
  Type
  type() const override;

  /// @brief 定義名を返す．
  const char*
  name() const override;

  /// @brief パラメータ割り当てのリストの取得
  AstConnectionList
  paramassign_list() const override;

  /// @brief strength を返す．
  const AstStrength*
  strength() const override;

  /// @brief delay を返す．
  const AstDelay*
  delay() const override;

  /// @brief module/UDP/gate instance リストの取得
  AstInstList
  inst_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 定義名
  const char* mName;

  // 要素の先頭
  const AstInst* mInstTop;

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
    PtConnection* con_top,
    PtInst* inst_top
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

  /// @brief パラメータ割り当てのリストの取得
  AstConnectionList
  paramassign_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ割り当ての先頭
  PtConnection* mParamAssignTop;

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
    const AstStrength* strength,
    PtInst* inst_top
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
  const AstStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const AstStrength* mStrength;

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
    const AstDelay* delay,
    PtInst* inst_top
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
  const AstDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const AstDelay* mDelay;

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
    const AstStrength* strength,
    const AstDelay* delay,
    PtInst* inst_top
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
  const AstStrength*
  strength() const override;

  /// @brief delay を返す．
  const AstDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const AstStrength* mStrength;

  // delay
  const AstDelay* mDelay;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_MUH_H
