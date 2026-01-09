#ifndef GROUPINFO_H
#define GROUPINFO_H

/// @file GroupInfo.h
/// @brief GroupInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "ym/ShString.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class GroupInfo GroupInfo.h "GroupInfo.h"
/// @brief グループ構造を表すクラス
//////////////////////////////////////////////////////////////////////
class GroupInfo
{
public:

  /// @brief コンストラクタ
  GroupInfo() = default;

  /// @brief デストラクタ
  ~GroupInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(
    const AstValue* val ///< [in] パース木の値
  );

  /// @brief ファイル上の位置を返す．
  const FileRegion&
  loc() const
  {
    return mLoc;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の辞書を返す．
  const std::unordered_map<std::string, std::vector<const AstValue*>>&
  elem_dict() const
  {
    return mElemDict;
  }

  /// @brief キーワードの値を返す．
  ///
  /// 値を持たない場合は nullptr を返す．
  /// 複数の値を持つ場合には invalid_argment 例外が送出される．
  const AstValue*
  get_value(
    const char* keyword ///< [in] キーワード
  ) const;

  /// @brief string の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 複数回定義されている．
  /// - 対象が文字列でなかった．
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_string(
    const char* keyword, ///< [in] キーワード
    ShString& val        ///< [out] 値を格納する変数
  ) const;

  /// @brief area の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 複数回定義されている．
  /// - 対象が数値でなかった．
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_area(
    const char* keyword, ///< [in] キーワード
    ClibArea& val        ///< [out] 値を格納する変数
  ) const;

  /// @brief capacitance の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 複数回定義されている．
  /// - 対象が数値でなかった．
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_capacitance(
    const char* keyword, ///< [in] キーワード
    ClibCapacitance& val ///< [out] 値を格納する変数
  ) const;

  /// @brief resistance の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 複数回定義されている．
  /// - 対象が数値でなかった．
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_resistance(
    const char* keyword, ///< [in] キーワード
    ClibResistance& val  ///< [out] 値を格納する変数
  ) const;

  /// @brief time の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 複数回定義されている．
  /// - 対象が数値でなかった．
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_time(
    const char* keyword, ///< [in] キーワード
    ClibTime& val	 ///< [out] 値を格納する変数
  ) const;

  /// @brief ClibCPV の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 2回以上指定されている．
  /// - 値が 'L', 'H', 'N', 'T', 'X' 以外
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_cpv(
    const char* keyword, ///< [in] キーワード
    ClibCPV& val	 ///< [out] 値を格納する変数
  ) const;

  /// @brief ClibVarType の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 2回以上指定されている．
  /// - 値が ClibVarType 以外
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_variable_type(
    const char* keyword, ///< [in] キーワード
    ClibVarType& val     ///< [out] 値を格納する変数
  ) const;

  /// @brief direction の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 2回以上指定されている．
  /// - 値が ClibDirection 以外
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_direction(
    const char* keyword, ///< [in] キーワード
    ClibDirection& val   ///< [out] 値を格納する変数
  ) const;

  /// @brief timing_type の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 2回以上指定されている．
  /// - 値が ClibTimingType 以外
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_timing_type(
    const char* keyword,  ///< [in] キーワード
    ClibTimingType& val   ///< [out] 値を格納する変数
  ) const;

  /// @brief timing_sense の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 2回以上指定されている．
  /// - 値が ClibTimingSense 以外
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_timing_sense(
    const char* keyword,  ///< [in] キーワード
    ClibTimingSense& val  ///< [out] 値を格納する変数
  ) const;

  /// @brief float_vector の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 2回以上指定されている．
  /// - 値が数値のリストでなかった．
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_float_vector(
    const char* keyword,     ///< [in] キーワード
    std::vector<double>& val ///< [out] 値を格納する変数
  ) const;

  /// @brief expr の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 2回以上指定されている．
  /// - 値が論理式でなかった．
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_expr(
    const char* keyword, ///< [in] キーワード
    const AstExpr*& val  ///< [out] 値を格納する変数
  ) const;

  /// @brief complex 形式の float_vector の値を取り出す．
  /// @retval true 成功
  /// @retval false 定義されていなかった
  ///
  /// 以下の場合にエラーとなる．
  /// - 2回以上指定されている．
  /// - 値が数値のリストでなかった．
  /// エラーの場合には invalid_argment 例外が送出される．
  /// true 以外の場合には val の値は変更されない．
  bool
  get_complex_float_vector(
    const char* keyword,     ///< [in] キーワード
    std::vector<double>& val ///< [out] 値を格納する変数
  ) const;

  /// @brief エラー処理を行う．
  void
  parse_error(
    const std::string& err_msg ///< [in] エラーメッセージ
  ) const;

  /// @brief 警告処理を行う．
  void
  warning(
    const std::string& warn_msg ///< [in] 警告メッセージ
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のグループの位置
  FileRegion mLoc;

  // 要素の辞書
  std::unordered_map<std::string, std::vector<const AstValue*>> mElemDict;

};

END_NAMESPACE_YM_DOTLIB

#endif // GROUPINFO_H
