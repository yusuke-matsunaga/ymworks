#ifndef CPTCONNECTION_H
#define CPTCONNECTION_H

/// @file CptConnection.h
/// @brief CptConnection のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtConnection.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptConnection CptConnection.h "CptConnection.h"
/// @brief ordered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptConnection :
  public PtConnection
{
protected:

  /// @brief コンストラクタ
  CptConnection(
    const FileRegion& file_region,
    const PtExpr* expr
  ) : mFileRegion{file_region},
      mExpr{expr}
  {
  }

  /// @brief デストラクタ
  ~CptConnection() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取出す．
  FileRegion
  file_region() const override;

  /// @brief 名前の取得
  /// @retval 名前 named connection の場合
  /// @retval "" ordered connection の場合
  const char*
  name() const override;

  /// @brief 式を取り出す．
  const PtExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 接続を表す式
  const PtExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief orered_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOrderedCon :
  public CptConnection
{
public:

  /// @brief コンストラクタ
  CptOrderedCon(
    const FileRegion& file_region,
    const PtExpr* expr
  ) : CptConnection(file_region, expr)
  {
  }

  /// @brief デストラクタ
  ~CptOrderedCon() {}

};


//////////////////////////////////////////////////////////////////////
/// @brief named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNamedCon :
  public CptConnection
{
public:

  /// @brief コンストラクタ
  CptNamedCon(
    const FileRegion& file_region,
    const char* name,
    const PtExpr* expr
  ) : CptConnection(file_region, expr),
      mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptNamedCon() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtConnection の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ名/ポート名
  const char* mName;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTCONNECTION_H
