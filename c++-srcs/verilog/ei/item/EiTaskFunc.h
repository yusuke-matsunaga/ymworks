#ifndef EITASKFUNC_H
#define EITASKFUNC_H

/// @file EiTaskFunc.h
/// @brief EiTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiTask EiTaskFunc.h "EiTaskFunc.h"
/// @brief ElbTaskFunc の実装クラス
//////////////////////////////////////////////////////////////////////
class EiTaskFunc :
  public ElbTaskFunc
{
public:

  /// @brief コンストラクタ
  EiTaskFunc(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstItem& ast_item, ///< [in] パース木の定義
    SizeType io_num          ///< [in] IOの数
  ) : mParent{parent},
      mAstItem{ast_item}
  {
    mIODeclList.reserve(io_num);
  }

  /// @brief デストラクタ
  ~EiTaskFunc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 名前の取得
  std::string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlTaskFunc の仮想関数(タスク/関数共通)
  //////////////////////////////////////////////////////////////////////

  /// @brief automatic 宣言されていたら true を返す．
  bool
  automatic() const override;

  /// @brief 入出力数を得る．
  SizeType
  io_num() const override;

  /// @brief 入出力の取得
  const VlIODecl*
  io(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < io_num() )
  ) const override;

  /// @brief 入出力のリストの取得
  std::vector<const VlIODecl*>
  io_list() const override;

  /// @brief 本体のステートメントを得る．
  const VlStmt*
  stmt() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbTaskFunc の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入出力の初期設定を行う．
  void
  add_iodecl(
    const VlIODecl* io_decl ///< [in] 入出力宣言要素
  ) override;

  /// @brief 本体のステートメントをセットする．
  void
  set_stmt(
    const VlStmt* stmt
  ) override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AstItem を取り出す．
  AstItem
  ast_item() const
  {
    return mAstItem;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

  // パース木のタスク/関数定義
  AstItem mAstItem;

  // 入出力用ヘッダのリスト
  std::vector<ElbIOHead*> mIOHeadList;

  // 入出力のリスト
  std::vector<std::unique_ptr<const VlIODecl>> mIODeclList;

  // 本体のステートメント
  const VlStmt* mStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EITASKFUNC_H
