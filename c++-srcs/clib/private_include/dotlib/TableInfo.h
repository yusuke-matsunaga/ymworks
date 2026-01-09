#ifndef TABLEINFO_H
#define TABLEINFO_H

/// @file TableInfo.h
/// @brief TableInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/ElemInfo.h"
#include "ci/CiLut.h"
#include "ci/CiStLut.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class TableInfo TableInfo.h "TableInfo.h"
/// @brief テーブルグループのパース情報を表すクラス
//////////////////////////////////////////////////////////////////////
class TableInfo :
  public ElemInfo
{
public:

  /// @brief コンストラクタ
  TableInfo(
    LibraryInfo& library_info ///< [in] ライブラリのパース情報
  ) : ElemInfo{library_info}
  {
  }

  /// @brief デストラクタ
  ~TableInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をセットする．
  void
  set(
    const AstValue* val
  );

  /// @brief 標準タイプの時 true を返す．
  bool
  is_standard() const;

  /// @brief CiLut を生成する．
  std::unique_ptr<CiLut>
  gen_lut() const;

  /// @brief CiStLut を生成する．
  std::unique_ptr<CiStLut>
  gen_stlut() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CiLutTemplate* mTemplate;

  // index_1 の値
  std::vector<double> mIndex1;

  // index_2 の値
  std::vector<double> mIndex2;

  // index_3 の値
  std::vector<double> mIndex3;

  // values の値
  std::vector<double> mValues;

  // domain の値
  const AstValue* mDomain{nullptr};

};

END_NAMESPACE_YM_DOTLIB

#endif // TABLEINFO_H
