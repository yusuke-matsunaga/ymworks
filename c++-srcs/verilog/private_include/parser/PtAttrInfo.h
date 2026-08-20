#ifndef PTATTRINFO_H
#define PTATTRINFO_H

/// @file PtAttrInfo.h
/// @brief PtAttrInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/AstBase.h"
#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

class AstAttrInst;

//////////////////////////////////////////////////////////////////////
/// @class PtAttrInfo PtAttrInfo.h "PtAttrInfo.h"
/// @brief AstAttrInst に関する情報を表すクラス
//////////////////////////////////////////////////////////////////////
class PtAttrInfo
{
public:

  /// @brief コンストラクタ
  PtAttrInfo(
    const AstBase* obj,        ///< [in] 元の構文木要素
    PtAttrInstList* attr_list, ///< [in] AstAttrInstのリスト
    bool def = false           ///< [in] 定義側の時 true とするフラグ
  ) : mObj{obj},
      mAttrList{attr_list},
      mDef{def}
  {
  }

  /// @brief デストラクタ
  ~PtAttrInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// 元の構文木要素を返す．
  const AstBase*
  obj() const
  {
    return mObj;
  }

  /// @brief 属性リストを返す．
  PtAttrInstList*
  attr_list() const
  {
    return mAttrList;
  }

  /// @brief 定義側の属性の時 true を返す．
  bool
  def() const
  {
    return mDef;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 元の構文木要素
  const AstBase* mObj;

  // 属性リスト
  PtAttrInstList* mAttrList;

  // 定義側のフラグ
  bool mDef;

};

END_NAMESPACE_YM_VERILOG

#endif // PTATTRINFO_H
