#ifndef ASTCPTYPE_H
#define ASTCPTYPE_H

/// @file AstCPType.h
/// @brief AstNode の継承クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "AstNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstCellPinDirection AstCPType.h "AstCPType.h"
/// @brief clear_preset_var1, clear_preset_var2 のを表すクラス
//////////////////////////////////////////////////////////////////////
class AstCPType :
  public AstNode
{
public:

  // 値
  enum Type { L, H, N, T, X };


public:

  /// @brief コンストラクタ
  AstCPType(const FileRegion& loc, ///< [in] ファイル上の位置
	    Type value);           ///< [in] 値

  /// @brief デストラクタ
  ~AstCPType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を返す．
  Type
  value() const
  {
    return mValue;
  }

  /// @brief 内容をストリーム出力する．
  void
  dump(ostream& s,     ///< [in] 出力先のストリーム
       int indent = 0) ///< [in] インデント量
    const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  Type mValue;

};

END_NAMESPACE_YM_DOTLIB

#endif // ASTCPTYPE_H
