#ifndef IMPLBASE_H
#define IMPLBASE_H

/// @file ImplBase.h
/// @brief ImplBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"


BEGIN_NAMESPACE_YM_BN

class ModelImpl;

//////////////////////////////////////////////////////////////////////
/// @class ImplBase ImplBase.h "ImplBase.h"
/// @brief 実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class ImplBase
{
public:

#if 0
  /// @brief コンストラクタ
  ImplBase(
    const ModelImpl* model ///< [in] 親のモデル
  ) : mModel{model}
  {
  }
#else
  /// @brief コンストラクタ
  ImplBase(
    const ModelImpl* model ///< [in] 親のモデル
  );
#endif

  /// @brief デストラクタ
  ~ImplBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモデルを返す．
  const ModelImpl*
  _model() const
  {
    return mModel;
  }

  /// @brief 参照を増やす
  void
  inc_ref() const;

  /// @brief 参照を減らす
  void
  dec_ref() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモデル
  const ModelImpl* mModel{nullptr};

};

END_NAMESPACE_YM_BN

#endif // IMPLBASE_H
