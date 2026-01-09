#ifndef LUTEMPLINFO_H
#define LUTEMPLINFO_H

/// @file LuTemplInfo.h
/// @brief LuTemplInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/ElemInfo.h"
#include "ym/ShString.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LuTemplInfo LuTemplInfo.h "LuTemplInfo.h"
/// @brief lu_template のパース情報を表すクラス
//////////////////////////////////////////////////////////////////////
class LuTemplInfo :
  public ElemInfo
{
public:

  /// @brief コンストラクタ
  LuTemplInfo(
    LibraryInfo& library_info
  ) : ElemInfo{library_info}
  {
  }

  /// @brief デストラクタ
  ~LuTemplInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(
    const AstValue* lut_val ///< [in] テンプレート情報のパース木
  );

  /// @brief 名前を得る．
  ShString
  name() const
  {
    return mName;
  }

  /// @brief テンプレートを作る．
  const CiLutTemplate*
  add_lu_template();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 次元
  SizeType mDimension;

  // variable_1
  ClibVarType mVar1{ClibVarType::none};

  // variable_2
  ClibVarType mVar2{ClibVarType::none};

  // variable_3
  ClibVarType mVar3{ClibVarType::none};

  // index_1
  std::vector<double> mIndex1;

  // index_2
  std::vector<double> mIndex2;

  // index_3
  std::vector<double> mIndex3;

};

END_NAMESPACE_YM_DOTLIB

#endif // LUTEMPLINFO_H
