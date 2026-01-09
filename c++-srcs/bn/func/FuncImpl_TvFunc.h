#ifndef FUNCIMPL_TVFUNC_H
#define FUNCIMPL_TVFUNC_H

/// @file FuncImpl_TvFunc.h
/// @brief FuncImpl_TvFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncImpl_TvFunc FuncImpl_TvFunc.h "FuncImpl_TvFunc.h"
/// @brief 真理値表型の関数情報を表すクラス
//////////////////////////////////////////////////////////////////////
class FuncImpl_TvFunc :
  public FuncImpl
{
public:

  /// @brief コンストラクタ
  FuncImpl_TvFunc(
    const TvFunc& func ///< [in] 真理値表
  );

  /// @brief デストラクタ
  ~FuncImpl_TvFunc() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の種類を返す．
  BnFunc::Type
  type() const override;

  /// @brief 真理値表型の時 true を返す．
  bool
  is_tvfunc() const override;

  /// @brief 入力数を返す．
  SizeType
  input_num() const override;

  /// @brief 真理値表を返す．
  ///
  /// - is_tvfunc() が true の時のみ意味を持つ．
  /// - それ以外の時は std::invalid_argument 例外を送出する．
  const TvFunc&
  tvfunc() const override;

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

  // 真理値表
  TvFunc mTvFunc;

};

END_NAMESPACE_YM_BN

#endif // FUNCIMPL_TVFUNC_H
