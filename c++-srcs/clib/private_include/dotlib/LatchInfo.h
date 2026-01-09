#ifndef LATCHINFO_H
#define LATCHINFO_H

/// @file LatchInfo.h
/// @brief LatchInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "dotlib/FLInfo.h"
#include "ym/ClibSeqAttr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LatchInfo LatchInfo.h "LatchInfo.h"
/// @brief ラッチグループの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class LatchInfo :
  public FLInfo
{
public:

  /// @brief コンストラクタ
  LatchInfo(
    LibraryInfo& library_info
  ) : FLInfo{library_info}
  {
  }

  /// @brief デストラクタ
  ~LatchInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 情報をセットする．
  void
  set(
    const AstValue* ast_val
  );

  /// @brief enable_on の値を返す．
  const AstExpr*
  enable_on() const
  {
    return mEnableOn;
  }

  /// @brief enable_on_also の値を返す．
  const AstExpr*
  enable_on_also() const
  {
    return mEnableOnAlso;
  }

  /// @brief data_in の値を返す．
  const AstExpr*
  data_in() const
  {
    return mDataIn;
  }

  /// @brief 順序セルの属性を返す．
  ClibSeqAttr
  seq_attr() const
  {
    bool has_slave_clock = mEnableOnAlso != nullptr;
    bool has_clear = clear() != nullptr;
    bool has_preset = preset() != nullptr;
    if ( has_clear && has_preset ) {
      return ClibSeqAttr{has_slave_clock,
			 clear_preset_var1(),
			 clear_preset_var2()};
    }
    else {
      return ClibSeqAttr{has_slave_clock,
			 has_clear,
			 has_preset};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // enable_on の値
  const AstExpr* mEnableOn{nullptr};

  // enable_on_also の値
  const AstExpr* mEnableOnAlso{nullptr};

  // data_in の値
  const AstExpr* mDataIn{nullptr};

};

END_NAMESPACE_YM_DOTLIB

#endif // LATCHINFO_H
