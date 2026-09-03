#ifndef PARSER_PTNAMEBRANCH_H
#define PARSER_PTNAMEBRANCH_H

/// @file PtNameBranch.h
/// @brief PtNameBranch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstNameBranch.h"
#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtNameBranch PtMisc.h "parser/PtMisc.h"
/// @brief AstNameBranch の実装クラス
//////////////////////////////////////////////////////////////////////
class PtNameBranch
{
public:
  //////////////////////////////////////////////////////////////////////
  // PtNameBranch の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief インデックスの有無のチェック
  /// @retval true インデックスを持っている時
  /// @retval false インデックスを持っていない時
  virtual
  bool
  has_index() const = 0;

  /// @brief インデックスの取得
  /// @return インデックスの値
  ///
  /// has_index() == false の時は std::logic_error 例外を送出する．
  virtual
  int
  index() const = 0;

  /// @brief 次の要素を返す．
  PtNameBranch*
  link() const
  {
    return mLink;
  }

  /// @brief link を設定する．
  void
  set_link(
    PtNameBranch* link
  )
  {
    mLink = link;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素
  PtNameBranch* mLink{nullptr};

};

/// @brief PtNameBranch のリスト
using PtNameBranchList = PtList<PtNameBranch>;

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTNAMEBRANCH_H
