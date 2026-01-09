#ifndef FUNCIMPL_BDD_H
#define FUNCIMPL_BDD_H

/// @file FuncImpl_Bdd.h
/// @brief FuncImpl_Bdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncImpl_Bdd FuncImpl_Bdd.h "FuncImpl_Bdd.h"
/// @brief BDD型の関数情報を表すクラス
//////////////////////////////////////////////////////////////////////
class FuncImpl_Bdd :
  public FuncImpl
{
public:

  /// @brief コンストラクタ
  FuncImpl_Bdd(
    Bdd bdd ///< [in] BDD
  );

  /// @brief デストラクタ
  ~FuncImpl_Bdd() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の種類を返す．
  BnFunc::Type
  type() const override;

  /// @brief BDD型の時 true を返す．
  bool
  is_bdd() const override;

  /// @brief 入力数を返す．
  SizeType
  input_num() const override;

  /// @brief BDDを返す．
  ///
  /// - is_bdd() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  Bdd
  bdd() const override;

  /// @brief コピーを作る．
  std::unique_ptr<FuncImpl>
  copy(
    BddMgr& bdd_mgr ///< [in] BddMgr
  ) const override;

  /// @brief ハッシュ用のユニークな文字列を返す．
  std::string
  signature() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDD
  Bdd mBdd;

};

END_NAMESPACE_YM_BN

#endif // FUNCIMPL_BDD_H
