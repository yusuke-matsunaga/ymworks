#ifndef CPTPART_H
#define CPTPART_H

/// @file CptPart.h
/// @brief CptPart のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtPart.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptPart CptPart.h "CptPart.h"
/// @brief PtPart の実装クラス
//////////////////////////////////////////////////////////////////////
class CptPart :
  public PtPart
{
public:

  /// @brief コンストラクタ
  CptPart(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr1, ///< [in] 1番目の式
    const PtExpr* expr2  ///< [in] 2番目の式
  ) : mFileRegion{fr},
      mLeft{expr1},
      mRight{expr2}
  {
  }

  /// @brief デストラクタ
  ~CptPart() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 1番目の式を取り出す．
  const PtExpr*
  left() const override;

  /// @brief 2番めの式を取り出す．
  const PtExpr*
  right() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 1番目の式
  const PtExpr* mLeft;

  // 2番目の式
  const PtExpr* mRight;

};


//////////////////////////////////////////////////////////////////////
/// @class CptPartC CptPart.h "CptPart.h"
/// @brief Const モードの CptPart
//////////////////////////////////////////////////////////////////////
class CptPartC :
  public CptPart
{
public:

  /// @brief コンストラクタ
  CptPartC(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr1, ///< [in] 1番目の式
    const PtExpr* expr2  ///< [in] 2番目の式
  ) : CptPart(fr, expr1, expr2)
  {
  }

  /// @brief デストラクタ
  ~CptPartC() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  mode() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptPartP CptPart.h "CptPart.h"
/// @brief Plus モードの CptPart
//////////////////////////////////////////////////////////////////////
class CptPartP :
  public CptPart
{
public:

  /// @brief コンストラクタ
  CptPartP(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr1, ///< [in] 1番目の式
    const PtExpr* expr2  ///< [in] 2番目の式
  ) : CptPart(fr, expr1, expr2)
  {
  }

  /// @brief デストラクタ
  ~CptPartP() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  mode() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptPartM CptPart.h "CptPart.h"
/// @brief Minus モードの CptPart
//////////////////////////////////////////////////////////////////////
class CptPartM :
  public CptPart
{
public:

  /// @brief コンストラクタ
  CptPartM(
    const FileRegion& fr, ///< [in] ファイル位置の情報
    const PtExpr* expr1, ///< [in] 1番目の式
    const PtExpr* expr2  ///< [in] 2番目の式
  ) : CptPart(fr, expr1, expr2)
  {
  }

  /// @brief デストラクタ
  ~CptPartM() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定のモードを返す．
  VpiRangeMode
  mode() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTPART_H
