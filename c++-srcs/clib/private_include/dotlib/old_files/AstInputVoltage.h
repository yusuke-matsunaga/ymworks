#ifndef ASTINPUTvolTAGE_H
#define ASTINPUTvolTAGE_H

/// @file AstInputVoltage.h
/// @brief AstInputVoltage のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "AstNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstInputVoltage AstInputVoltage.h "AstInputVoltage.h"
/// @brief input voltage を表すクラス
//////////////////////////////////////////////////////////////////////
class AstInputVoltage :
  public AstNode
{
public:

  /// @brief コンストラクタ
  AstInputVoltage(const FileRegion& attr_loc,        ///< [in] 属性のファイル上の位置
		  const StrHandler& header,          ///< [in] ヘッダを読み込んだハンドラ
		  const InputVoltageHandler& group); ///< [in] グループ本体を読み込んだハンドラ

  /// @brief デストラクタ
  ~AstInputVoltage();


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

  /// @brief 'vil' を返す．
  const AstExpr*
  vil() const
  {
    return mVil;
  }

  /// @brief 'vih' を返す．
  const AstExpr*
  vih() const
  {
    return mVih;
  }

  /// @brief 'vimin' を返す．
  const AstExpr*
  vimin() const
  {
    return mViMin;
  }

  /// @brief 'vimax' を返す．
  const AstExpr*
  vimax() const
  {
    return mViMax;
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

  // vil
  const AstExpr* mVil;

  // vih
  const AstExpr* mVih;

  // vimin
  const AstExpr* mViMin;

  // vimax
  const AstExpr* mViMax;

};

END_NAMESPACE_YM_DOTLIB

#endif // ASTINPUTvolTAGE_H
