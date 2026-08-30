#ifndef CPTITEM_CONTASSIGNH_H
#define CPTITEM_CONTASSIGNH_H

/// @file CptItem_ContAssignH.h
/// @brief CptItem_ContAssignH のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptItem_ContAssignH CptItem_ContAssignH.h "CptItem_ContAssignH.h"
/// @brief continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptItem_ContAssignH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptItem_ContAssignH(
    const FileRegion& file_region,
    const AstContAssign* ca_top
  ) : mFileRegion{file_region},
      mTop{ca_top}
  {
  }

  /// @brief デストラクタ
  ~CptItem_ContAssignH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief strength の取得
  /// @return 信号強度
  const AstStrength*
  strength() const override;

  /// @brief delay の取得
  /// @return 遅延
  const AstDelay*
  delay() const override;

  /// @brief continuous assign リストの取得
  AstContAssignList
  contassign_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の先頭
  const AstContAssign* mTop;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_ContAssignHS CptItem_ContAssignH.h "CptItem_ContAssignH.h"
/// @brief strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptItem_ContAssignHS :
  public CptItem_ContAssignH
{
public:

  /// @brief コンストラクタ
  CptItem_ContAssignHS(
    const FileRegion& file_region,
    const AstStrength* strength,
    const AstContAssign* ca_top
  ) : CptItem_ContAssignH(file_region, ca_top),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptItem_ContAssignHS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
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
/// @class CptItem_ContAssignHD CptItem_ContAssignH.h "ContItem_AssignH.h"
/// @brief delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptItem_ContAssignHD :
  public CptItem_ContAssignH
{
public:

  /// @brief コンストラクタ
  CptItem_ContAssignHD(
    const FileRegion& file_region,
    const AstDelay* delay,
    const AstContAssign* ca_top
  ) : CptItem_ContAssignH(file_region, ca_top),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptItem_ContAssignHD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
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
/// @class CptItem_ContAssignHSD CptItem_ContAssignH.h "CptItem_ContAssignH.h"
/// @brief strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptItem_ContAssignHSD :
  public CptItem_ContAssignH
{
public:

  /// @brief コンストラクタ
  CptItem_ContAssignHSD(
    const FileRegion& file_region,
    const AstStrength* strength,
    const AstDelay* delay,
    const AstContAssign* ca_top
  ) : CptItem_ContAssignH(file_region, ca_top),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptItem_ContAssignHSD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
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


//////////////////////////////////////////////////////////////////////
/// @class CptContAssign CptItem_ContAssignH.h "CptItem_ContAssignH.h"
/// @brief continuous assign 文
//////////////////////////////////////////////////////////////////////
class CptContAssign :
  public PtContAssign
{
public:

  /// @brief コンストラクタ
  CptContAssign(
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : mLhs{lhs},
      mRhs{rhs}
  {
  }

  /// @brief デストラクタ
  ~CptContAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  FileRegion
  file_region() const override;

  /// @brief 左辺式を取り出す．
  const AstExpr*
  lhs() const override;

  /// @brief 右辺式を取り出す．
  const AstExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺式
  const AstExpr* mLhs;

  // 右辺式
  const AstExpr* mRhs;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_CONTASSIGNH_H
