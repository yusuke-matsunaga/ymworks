#ifndef ASTATTR_H
#define ASTATTR_H

/// @file AstAttr.h
/// @brief AstAttr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014, 2018, 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "dotlib/AstValue.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstAttr AstAttr.h "AstAttr.h"
/// @brief 属性を表すクラス
///
/// 属性は
/// - 属性名
/// - 属性名のファイル上の位置
/// - 値 (AttrValue)
/// の組で表されるが，値には様々な種類がある．
//////////////////////////////////////////////////////////////////////
class AstAttr
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 無効な値となる．
  AstAttr() = default;

  /// @brief コンストラクタ
  AstAttr(
    const std::string& kwd,    ///< [in] 属性名
    const FileRegion& kwd_loc, ///< [in] 属性名の位置
    AstValuePtr&& value        ///< [in] 値
  ) : mKwd{kwd},
      mKwdLoc{kwd_loc},
      mValue{std::move(value)}
  {
  }

  /// @brief デストラクタ
  ~AstAttr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 有効な値を表す時 true を返す．
  bool
  is_valid() const { return !mKwd.empty(); }

  /// @brief 属性名を返す．
  std::string
  kwd() const { return mKwd; }

  /// @brief 属性名の位置
  FileRegion
  kwd_loc() const { return mKwdLoc; }

  /// @brief 属性の値を得る．
  const AstValue&
  value() const { return *mValue; }

  /// @brief 内容をストリーム出力する．
  void
  print(
    std::ostream& s, ///< [in] 出力先のストリーム
    int ilevel = 0   ///< [in] インデントレベル
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性名
  std::string mKwd;

  // 属性名の位置
  FileRegion mKwdLoc;

  // 値
  AstValuePtr mValue;

};

END_NAMESPACE_YM_DOTLIB

#endif // ASTATTR_H
