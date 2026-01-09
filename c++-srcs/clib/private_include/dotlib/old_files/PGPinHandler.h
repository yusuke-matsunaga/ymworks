#ifndef PGPINHANDLER_H
#define PGPINHANDLER_H

/// @file PGPinHandler.h
/// @brief PGPinHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Str1GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PGPinHandler PGPinHandler.h "PGPinHandler.h"
/// @brief 'pg_pin' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダ名はピン名を表す．
/// * Simple Attributes
///   - voltage_name : string ;
///   - pg_type : primaY_power | primary_ground | backup_power |
///               backup_ground | internal_power, internal_ground
//////////////////////////////////////////////////////////////////////
class PGPinHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  PGPinHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~PGPinHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif 'pg_pin' Group Statement の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstPGPin*>& dst_list);


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

  // voltage_name
  const AstString* mVoltageName;

  // pg_type
  const AstString* mPgType;

};

END_NAMESPACE_YM_DOTLIB

#endif // PGPINHANDLER_H
