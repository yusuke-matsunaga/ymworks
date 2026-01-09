#ifndef ASTOUTPUTVOLTAGE_H
#define ASTOUTPUTVOLTAGE_H

/// @file AstOutputVoltage.h
/// @brief AstOutputVoltage のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "AstNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstOutputVoltage AstOutputVoltage.h "AstOutputVoltage.h"
/// @brief output voltage を表すクラス
//////////////////////////////////////////////////////////////////////
class AstOutputVoltage :
  public AstNode
{
public:

  /// @brief コンストラクタ
  AstOutputVoltage(const FileRegion& attr_loc,         ///< [in] 属性のファイル上の位置
		   const StrHandler& header,	       ///< [in] ヘッダを読み込んだハンドラ
		   const OutputVoltageHandler& group); ///< [in] グループ本体を読み込んだハンドラ

  /// @brief デストラクタ
  ~AstOutputVoltage();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const AstString*
  name() const
  {
    return mName;
  }

  /// @brief 'vol' を返す．
  const AstExpr*
  vol() const
  {
    return mVol;
  }

  /// @brief 'voh' を返す．
  const AstExpr*
  voh() const
  {
    return mVoh;
  }

  /// @brief 'vomin' を返す．
  const AstExpr*
  vomin() const
  {
    return mVoMin;
  }

  /// @brief 'vomax' を返す．
  const AstExpr*
  vomax() const
  {
    return mVoMax;
  }

  /// @brief 内容をストリーム出力する．
  void
  dump(ostream& s,     ///< [in] 出力先のストリーム
       int indent = 0) ///< [in] インデント量
    const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const AstString* mName;

  // vol
  const AstExpr* mVol;

  // voh
  const AstExpr* mVoh;

  // vomin
  const AstExpr* mVoMin;

  // vomax
  const AstExpr* mVoMax;

};

END_NAMESPACE_YM_DOTLIB

#endif // ASTOUTPUTVOLTAGE_H
