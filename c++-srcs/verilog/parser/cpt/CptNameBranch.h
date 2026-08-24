#ifndef CPTNAMEBRANCH_H
#define CPTNAMEBRANCH_H

/// @file CptNameBranch.h
/// @brief CptNameBranch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtMisc.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief 階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNameBranch :
  public PtNameBranch
{
public:

  /// @brief コンストラクタ
  CptNameBranch(
    const char* name
  ) : mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptNameBranch() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief インデックスの有無のチェック
  /// @retval true インデックスを持っている時
  /// @retval false インデックスを持っていない時
  bool
  has_index() const override;

  /// @brief インデックスの取得
  /// @return インデックスの値
  int
  index() const override;

  /// @brief 次の要素の取得
  const AstNameBranch*
  link() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief link を設定する．
  void
  set_link(
    PtNameBranch* link
  ) override;

  /// @brief PtNameBranch* 型のリンクを返す．
  PtNameBranch*
  _link() const override;

  /// @brief リンクトリストを逆順にする．
  /// @return 新しい先頭を返す．
  PtNameBranch*
  reverse() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 次の要素を指すリンク
  PtNameBranch* mLink{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @brief インデックスつきの階層名を表すクラス
//////////////////////////////////////////////////////////////////////
class CptNameBranchI :
  public CptNameBranch
{
public:

  /// @brief コンストラクタ
  CptNameBranchI(
    const char* name,
    int index
  ) : CptNameBranch(name),
      mIndex{index}
  {
  }

  /// @brief デストラクタ
  ~CptNameBranchI() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスを持っている時 true を返す．
  bool
  has_index() const override;

  /// @brief インデックスを取り出す．
  int
  index() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス
  int mIndex;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTNAMEBRANCH_H
