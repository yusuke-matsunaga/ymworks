#ifndef ATTRDICT_H
#define ATTRDICT_H

/// @file AttrDict.h
/// @brief AttrDict のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class AstAttrInst;

//////////////////////////////////////////////////////////////////////
/// @class AttrDict AttrDict.h "AttrDict.h"
/// @brief AstAttrInst をキーとして ElbAttrList を格納するハッシュ表
//////////////////////////////////////////////////////////////////////
class AttrDict
{
public:

  /// @brief コンストラクタ
  AttrDict() = default;

  /// @brief デストラクタ
  ~AttrDict() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // AttrDict の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録する．
  void
  add(
    const AstAttrInst* ast_attr,                       ///< [in] パース木の属性定義
    const std::vector<const VlAttribute*>& attr_list ///< [in] 登録する属性のリスト
  )
  {
    mHash.emplace(ast_attr, attr_list);
  }

  /// @brief 属性リストを取り出す．
  std::vector<const VlAttribute*>
  find(
    const AstAttrInst* ast_attr ///< [in] パース木の属性定義
  ) const
  {
    if ( mHash.count(ast_attr) > 0 ) {
      return mHash.at(ast_attr);
    }
    return {};
  }

  /// @brief 内容をクリアする．
  void
  clear()
  {
    mHash.clear();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  std::unordered_map<const AstAttrInst*, std::vector<const VlAttribute*>> mHash;

};

END_NAMESPACE_YM_VERILOG

#endif // ATTRDICT_H
