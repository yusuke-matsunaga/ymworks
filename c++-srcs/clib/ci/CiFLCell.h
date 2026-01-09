#ifndef CIFLCELL_H
#define CIFLCELL_H

/// @file CiFLCell.h
/// @brief CiLFCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiCell.h"
#include "ym/ClibSeqAttr.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class CiFLCell CiFLCell.h "CiFLCell.h"
/// @brief FFとラッチに共通なクラス
//////////////////////////////////////////////////////////////////////
class CiFLCell :
  public CiCell
{
public:

  /// @brief 空のコンストラクタ
  CiFLCell() = default;

  /// @brief コンストラクタ
  CiFLCell(
    const ShString& name,      ///< [in] 名前
    ClibArea area,             ///< [in] 面積
    const ShString& var1,      ///< [in] 内部変数1の名前
    const ShString& var2,      ///< [in] 内部変数2の名前
    const Expr& clear,         ///< [in] "clear" 関数の式
    const Expr& preset,        ///< [in] "preset" 関数の式
    ClibSeqAttr seq_attr       ///< [in] 順序セルの属性
  ) : CiCell{name, area},
      mVar1{var1},
      mVar2{var2},
      mClear{clear},
      mPreset{preset},
      mSeqAttr{seq_attr}
  {
  }

  /// @brief デストラクタ
  ~CiFLCell() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部変数1の名前を返す．
  std::string
  qvar1() const override;

  /// @brief 内部変数1の名前を返す．
  std::string
  qvar2() const override;

  /// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
  ///
  /// クリア端子がない場合の返り値は不定
  Expr
  clear_expr() const override;

  /// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
  ///
  /// プリセット端子がない場合の返り値は不定
  Expr
  preset_expr() const override;

  /// @brief 順序セルの属性を返す．
  ClibSeqAttr
  seq_attr() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump_FL(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief restore() の下請け関数
  void
  restore_FL(
    Deserializer& s ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内部変数1
  ShString mVar1;

  // 内部変数2
  ShString mVar2;

  // clear 論理式
  Expr mClear;

  // preset 論理式
  Expr mPreset;

  // 順序セルの属性
  ClibSeqAttr mSeqAttr;

};

END_NAMESPACE_YM_CLIB

#endif // CIFLCELL_H
