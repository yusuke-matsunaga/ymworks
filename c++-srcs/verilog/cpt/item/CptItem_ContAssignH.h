#ifndef CPTITEM_CONTASSIGNH_H
#define CPTITEM_CONTASSIGNH_H

/// @file CptItem_ContAssignH.h
/// @brief CptItem_ContAssignH のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtContAssign.h"


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
    const PtContAssign* ca_top
  ) : mFileRegion{file_region},
      mTop{ca_top}
  {
  }

  /// @brief デストラクタ
  ~CptItem_ContAssignH() {}


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

  /// @brief continuous assign リストの取得
  const PtContAssign*
  contassign_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の先頭
  const PtContAssign* mTop;

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
    const PtStrength* strength,
    const PtContAssign* ca_top
  ) : CptItem_ContAssignH(file_region, ca_top),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptItem_ContAssignHS() {}


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
    const PtDelay* delay,
    const PtContAssign* ca_top
  ) : CptItem_ContAssignH(file_region, ca_top),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptItem_ContAssignHD() {}


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
    const PtStrength* strength,
    const PtDelay* delay,
    const PtContAssign* ca_top
  ) : CptItem_ContAssignH(file_region, ca_top),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptItem_ContAssignHSD() {}


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
    const PtExpr* lhs,
    const PtExpr* rhs
  ) : mLhs{lhs},
      mRhs{rhs}
  {
  }

  /// @brief デストラクタ
  ~CptContAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  FileRegion
  file_region() const override;

  /// @brief 左辺式を取り出す．
  const PtExpr*
  lhs() const override;

  /// @brief 右辺式を取り出す．
  const PtExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺式
  const PtExpr* mLhs;

  // 右辺式
  const PtExpr* mRhs;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_CONTASSIGNH_H
