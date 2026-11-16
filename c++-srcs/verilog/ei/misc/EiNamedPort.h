#ifndef EINAMEDPORT_H
#define EINAMEDPORT_H

/// @file EiNamedPort.h
/// @brief EiNamedPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiNamedPort EiNamedPort.h "EiNamedPort.h"
/// @brief 名前を持つ EiPort
//////////////////////////////////////////////////////////////////////
class EiNamedPort :
  public EiPort
{
public:

  /// @brief コンストラクタ
  EiNamedPort(
    const FileRegion& loc,  ///< [in] ファイル位置
    const VlModule* parent, ///< [in] 親のモジュール
    const char* name,       ///< [in] 名前
    SizeType index,         ///< [in] ポート番号
    const VlExpr* low_conn, ///< [in] 下位の接続
    VpiDir dir              ///< [in] 向き
  ) : EiPort(loc, parent, index, low_conn, dir),
      mName{name}
  {
  }

  /// @brief デストラクタ
  ~EiNamedPort() {}


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

#endif // EINAMEDPORT_H
