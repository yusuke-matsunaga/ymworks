#ifndef CPTITEM_PROCESS_H
#define CPTITEM_PROCESS_H

/// @file CptItem_Process.h
/// @brief CptItem_Process のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptItem_Process CptItem_Process.h "CptItem_Process.h"
/// @brief inititial 文/always 文に共通なクラス
//////////////////////////////////////////////////////////////////////
class CptItem_Process :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptItem_Process(
    const FileRegion& file_region,
    const AstStmt* body
  ) : mTopLoc{file_region.start_loc()},
      mBody{body}
  {
    if ( body == nullptr ) {
      throw std::logic_error{"body = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptItem_Process() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  FileRegion
  file_region() const override;

  /// @brief 本体のステートメントを返す．
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭のファイル位置
  FileLoc mTopLoc;

  // 本体
  const AstStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_Initial CptItem_Process.h "CptItem_Process.h"
/// @brief inititial 文
//////////////////////////////////////////////////////////////////////
class CptItem_Initial :
  public CptItem_Process
{
public:

  /// @brief コンストラクタ
  CptItem_Initial(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptItem_Process(file_region, body)
  {
  }

  /// @brief デストラクタ
  ~CptItem_Initial() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_Always CptItem_Process.h "CptItem_Process.h"
/// @brief always 文
//////////////////////////////////////////////////////////////////////
class CptItem_Always :
  public CptItem_Process
{
public:

  /// @brief コンストラクタ
  CptItem_Always(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptItem_Process(file_region, body)
  {
  }

  /// @brief デストラクタ
  ~CptItem_Always() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_PROCESS_H
