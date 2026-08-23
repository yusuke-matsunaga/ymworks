#ifndef CPTCONTASSIGN_H
#define CPTCONTASSIGN_H

/// @file CptItem.h
/// @brief CptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptContAssignH CptContAssign.h "CptContAssign.h"
/// @brief continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptContAssignH(
    const FileRegion& file_region,
    PtContAssignArray&& ca_list
  ) : mFileRegion{file_region},
      mList{std::move(ca_list)}
  {
  }

  /// @brief デストラクタ
  ~CptContAssignH() {}


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

  /// @brief continuous assign の要素数の取得
  SizeType
  contassign_num() const override;

  /// @brief continuous assign の取得
  const AstContAssign*
  contassign(
    SizeType index ///< [in] インデックス ( 0 <= index < contassign_num() )
  ) const override;

  /// @brief continuous assign リストの取得
  AstContAssignVec
  contassign_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素のリスト
  PtContAssignArray mList;

};


//////////////////////////////////////////////////////////////////////
/// @class CptContAssignHS CptContAssign.h "CptContAssign.h"
/// @brief strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHS :
  public CptContAssignH
{
public:

  /// @brief コンストラクタ
  CptContAssignHS(
    const FileRegion& file_region,
    const AstStrength* strength,
    PtContAssignArray&& ca_list
  ) : CptContAssignH(file_region, std::move(ca_list)),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptContAssignHS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
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
/// @class CptContAssignHD CptContAssign.h "ContAssign.h"
/// @brief delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHD :
  public CptContAssignH
{
public:

  /// @brief コンストラクタ
  CptContAssignHD(
    const FileRegion& file_region,
    const AstDelay* delay,
    PtContAssignArray&& ca_list
  ) : CptContAssignH(file_region, std::move(ca_list)),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptContAssignHD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
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
/// @class CptContAssignHSD CptContAssign.h "CptContAssign.h"
/// @brief strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHSD :
  public CptContAssignH
{
public:

  /// @brief コンストラクタ
  CptContAssignHSD(
    const FileRegion& file_region,
    const AstStrength* strength,
    const AstDelay* delay,
    PtContAssignArray&& ca_list
  ) : CptContAssignH(file_region, std::move(ca_list)),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptContAssignHSD() {}


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
/// @class CptContAssign CptContAssign.h "CptContAssign.h"
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
  // PtContAssign の仮想関数
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

#endif // CPTCONTASSIGN_H
