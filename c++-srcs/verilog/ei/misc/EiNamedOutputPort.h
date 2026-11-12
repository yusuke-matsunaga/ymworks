#ifndef EINAMEDOUTPUTPORT_H
#define EINAMEDOUTPUTPORT_H

/// @file EiNamedOutputPort.h
/// @brief EiNamedOutputPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiOutputPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiNamedOutputPort EiNamedOutputPort.h "EiNamedOutputPort.h"
/// @brief 名前を持つ出力ポート
//////////////////////////////////////////////////////////////////////
class EiNamedOutputPort :
  public EiOutputPort
{
public:

  /// @brief コンストラクタ
  EiNamedOutputPort(
    const FileRegion& loc,  ///< [in] ファイル位置
    const VlModule* parent, ///< [in] 親のモジュール
    const char* name,       ///< [in] 名前
    SizeType index,         ///< [in] ポート番号
    const VlTerm* low_conn, ///< [in] 下位の接続
    VpiDir dir              ///< [in] 向き
  ) : EiOutputPort(loc, parent, index, low_conn, dir),
      mName{name}
  {
  }

  /// @brief デストラクタ
  ~EiNamedOutputPort() {}


public:
  //////////////////////////////////////////////////////////////////////
  // VlPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 明示的に名前がついているとき true を返す．
  bool
  has_explicit_name() const override;

  /// @brief 名前を返す．
  std::string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  // この文字列領域は AstMgr が管理しているはず
  const char* mName;

};

END_NAMESPACE_YM_VERILOG

#endif // EINAMEDOUTPUTPORT_H
