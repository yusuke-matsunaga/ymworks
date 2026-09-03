#ifndef CPTATTRINST_H
#define CPTATTRINST_H

/// @file CptMisc.h
/// @brief CptMisc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtAttrInst.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptAttrInst CptAttrInst.h "CptAttrInst.h"
/// @brief attribute_instance を表すクラス
//////////////////////////////////////////////////////////////////////
class CptAttrInst :
  public PtAttrInst
{
public:

  /// @brief コンストラクタ
  CptAttrInst(
    const FileRegion& file_region,
    const PtAttrSpec* as_top
  ) : mFileRegion{file_region},
      mAttrSpecTop{as_top}
  {
  }

  /// @brief デストラクタ
  ~CptAttrInst() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtAttrInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 要素のリストの取得
  const PtAttrSpec*
  attrspec_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mFileRegion;

  // attr spec の先頭
  const PtAttrSpec* mAttrSpecTop;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTATTRINST_H
