#ifndef PARSER_PTHIERNAME_H
#define PARSER_PTHIERNAME_H

/// @file parser/PtHierName.h
/// @brief PtHierName のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtList.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtHierName PtHierName.h "parser/PtHierName.h"
/// @ingroup VlParser
/// @brief 階層つき名を表すクラス
///
/// 中身は階層部分を表す PtNameBranch のリストと末尾の名前を表す
/// 文字列から成る．
///
/// コンストラクタでは最下層の名前の設定のみを行う．
/// 階層の追加は add(const char*), add(int index, const char*)
/// で行う．これは内部で PtNameBranch を生成していることに因る．
//////////////////////////////////////////////////////////////////////
class PtHierName
{
public:

  /// @brief コンストラクタ
  PtHierName(
    Alloc& alloc,            ///< [in] アロケータ
    const AstNameBranch* nb, ///< [in] 階層ブランチ
    const char* name         ///< [in] 名前
  ) : mTailName{name}
  {
    void* p = alloc.get_memory(sizeof(PtNameBranchList));
    mNbList = new (p) PtNameBranchList;
    mNbList->push_back(alloc, nb);
  }

  /// @brief デストラクタ
  ~PtHierName() = default;


public:

  /// @brief 階層を追加する．
  void
  add(
    Alloc& alloc,            ///< [in] アロケータ
    const AstNameBranch* nb, ///< [in] 追加する階層ブランチ
    const char* tail_name    ///< [in] 追加する最下層の名前
  )
  {
    mNbList->push_back(alloc, nb);
    mTailName = tail_name;
  }


public:

  /// @brief 階層ブランチのリストを返す．
  PtNameBranchList*
  nb_list() const
  {
    return mNbList;
  }

#if 0
  /// @brief 階層ブランチのリストを PtNameBranchArray の形で取り出す．
  ///
  /// この関数を呼ぶと mNbList は破壊される．
  PtNameBranchArray
  nb_listh_to_array(
    Alloc& alloc
  )
  {
    auto ans = mNbList->to_array(alloc);
    mNbList = nullptr;
    return ans;
  }
#endif

  /// @brief 最下層の名前を取り出す．
  /// @return 最下層の名前
  const char*
  tail_name() const
  {
    return mTailName;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 階層ブランチのリスト
  PtNameBranchList* mNbList;

  // 最下層の名前
  const char* mTailName;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_PTHIERNAME_H
