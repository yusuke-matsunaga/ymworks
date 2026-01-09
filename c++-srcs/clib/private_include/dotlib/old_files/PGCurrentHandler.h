#ifndef PGCURRENTHANDLER_H
#define PGCURRENTHANDLER_H

/// @file PGCurrentHandler.h
/// @brief PGCurrentHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PGCurrentHandler PGCurrentHandler.h "PGCurrentHandler.h"
/// @brief 'pg_current' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
///
/// * このマニュアル読んだだけではなんのことかさっぱりわかりません．
/// * Group Statement
///   - vector () { ... }
//////////////////////////////////////////////////////////////////////
class PGCurrentHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  PGCurrentHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~PGCurrentHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif pg_current Group Statement の記述をパースする．
  /// @param[in] dst 読み込んだ値を格納する変数
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(const AstPGCurrent*& dst);


protected:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ記述の始まり
  void
  begin_group() override;

  /// @brief グループ記述の終わり
  /// @param[in] group_loc グループ全体のファイル上の位置
  /// @retval true 正常にパーズした．
  /// @retval false パーズ中にエラーが起こった．
  bool
  end_group(const FileRegion& group_loc) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_DOTLIB

#endif // PGCURRENTHANDLER_H
