#ifndef ELBDECLREF_H
#define ELBDECLREF_H

/// @file ElbDeclRef.h
/// @brief ElbDeclRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbDeclRef ElbDeclRef.h "ElbDeclRef.h"
/// @brief 宣言要素に対する参照を表すクラス
///
/// ElbMgr::new_Primary() の引数として用いるだけのクラス
/// 以下のタイプがある．
/// - 単独の宣言要素
/// - 配列型の宣言要素の固定インデックス
/// - 配列型の宣言要素の可変インデックス
//////////////////////////////////////////////////////////////////////
class ElbDeclRef
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正値となる．
  ElbDeclRef() = default;

  /// @brief 単独の宣言要素のコンストラクタ
  ElbDeclRef(
    const VlDecl* decl
  ) : mDecl{decl},
      mDeclBase{decl}
  {
  }

  /// @brief 固定インデックスの配列型宣言要素のコンストラクタ
  ElbDeclRef(
    const VlDeclArray* declarray,
    SizeType offset
  ) : mDeclArray{declarray},
      mDeclBase{declarray},
      mOffset{offset}
  {
  }

  /// @brief 可変インデックスの配列型宣言要素のコンストラクタ
  ElbDeclRef(
    const VlDeclArray* declarray,
    const std::vector<const VlExpr*>& index_list
  ) : mDeclArray{declarray},
      mDeclBase{declarray},
      mIndexList{index_list}
  {
  }

  /// @brief デストラクタ
  ~ElbDeclRef() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 不正値の時 true を返す．
  bool
  is_invalid() const
  {
    return mDecl == nullptr && mDeclArray == nullptr;
  }

  /// @brief 単独の宣言要素の時 true を返す．
  bool
  is_decl() const
  {
    return mDecl != nullptr;
  }

  /// @brief 配列型の宣言要素の時 true を返す．
  bool
  is_declarray() const
  {
    return mDeclArray != nullptr;
  }

  /// @brief 固定インデックスの配列型の宣言要素の時 true を返す．
  bool
  is_const_elem() const
  {
    return is_declarray() && mIndexList.empty();
  }

  /// @brief 可変インデックスの配列がの宣言要素の時 true を返す．
  bool
  is_var_elem() const
  {
    return is_declarray() && !mIndexList.empty();
  }

  /// @brief 単独の宣言要素を返す．
  const VlDecl*
  decl() const
  {
    return mDecl;
  }

  /// @brief 配列型の宣言要素を返す．
  const VlDeclArray*
  declarray() const
  {
    return mDeclArray;
  }

  /// @brief 宣言要素の基底クラスを返す．
  const VlDeclBase*
  declbase() const
  {
    return mDeclBase;
  }

  /// @brief 固定インデックスのオフセットを返す．
  ///
  /// - is_const_elem() == true の時のみ意味を持つ．
  SizeType
  offset() const
  {
    return mOffset;
  }

  /// @brief 可変インデックスのインデックス式を返す．
  ///
  /// - is_var_elem() == true の時のみ意味を持つ．
  const std::vector<const VlExpr*>&
  index_list() const
  {
    return mIndexList;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 単独の宣言要素
  const VlDecl* mDecl{nullptr};

  // 配列型の宣言要素
  const VlDeclArray* mDeclArray{nullptr};

  // 宣言要素の基底クラス(mDecl or mDeclArray)
  const VlDeclBase* mDeclBase{nullptr};

  // 固定オフセット
  SizeType mOffset;

  // インデックス式のリスト
  std::vector<const VlExpr*> mIndexList;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBDECLREF_H
