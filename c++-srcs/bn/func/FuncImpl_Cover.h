#ifndef FUNCIMPL_COVER_H
#define FUNCIMPL_COVER_H

/// @file FuncImpl_Cover.h
/// @brief FuncImpl_Cover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "ym/SopCover.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncImpl_Cover FuncImpl_Cover.h "FuncImpl_Cover.h"
/// @brief カバー型の関数情報を表すクラス
//////////////////////////////////////////////////////////////////////
class FuncImpl_Cover :
  public FuncImpl
{
public:

  /// @brief コンストラクタ
  FuncImpl_Cover(
    const SopCover& input_cover, ///< [in] 入力カバー
    bool output_inv              ///< [in] 出力の反転属性
  );

  /// @brief デストラクタ
  ~FuncImpl_Cover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の種類を返す．
  BnFunc::Type
  type() const override;

  /// @brief カバー型の時 true を返す．
  bool
  is_cover() const override;

  /// @brief 入力数を返す．
  SizeType
  input_num() const override;

  /// @brief 入力カバーを返す．
  const SopCover&
  input_cover() const override;

  /// @brief 出力の反転属性を返す．
  bool
  output_inv() const override;

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

  // 入力カバー
  SopCover mInputCover;

  // 出力の反転属性
  bool mOutputInv;

};

END_NAMESPACE_YM_BN

#endif // FUNCIMPL_COVER_H
