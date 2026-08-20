#ifndef CPTPROCESS_H
#define CPTPROCESS_H

/// @file CptItem.h
/// @brief CptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptProcess CptProcess.h "CptProcess.h"
/// @brief inititial 文/always 文に共通なクラス
//////////////////////////////////////////////////////////////////////
class CptProcess :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptProcess(
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
  ~CptProcess() {}


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
/// @class CptInitial CptProcess.h "CptProcess.h"
/// @brief inititial 文
//////////////////////////////////////////////////////////////////////
class CptInitial :
  public CptProcess
{
public:

  /// @brief コンストラクタ
  CptInitial(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptProcess(file_region, body)
  {
  }

  /// @brief デストラクタ
  ~CptInitial() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptAlways CptProcess.h "CptProcess.h"
/// @brief always 文
//////////////////////////////////////////////////////////////////////
class CptAlways :
  public CptProcess
{
public:

  /// @brief コンストラクタ
  CptAlways(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptProcess(file_region, body)
  {
  }

  /// @brief デストラクタ
  ~CptAlways() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTPROCESS_H
