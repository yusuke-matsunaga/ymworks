#ifndef DFFIMPL_H
#define DFFIMPL_H

/// @file DffImpl.h
/// @brief DffImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ImplBase.h"


BEGIN_NAMESPACE_YM_BN

class DffImpl;
class NodeImpl;

//////////////////////////////////////////////////////////////////////
/// @class DffImpl ModelImpl.h "ModelImpl.h"
/// @brief DFFの情報を表す構造体
//////////////////////////////////////////////////////////////////////
class DffImpl :
  public ImplBase
{
  friend class ModelImpl;

public:

  /// @brief コンストラクタ
  DffImpl(
    const ModelImpl* model, ///< [in] 親のモデル
    SizeType id,            ///< [in] ID番号
    std::string name,       ///< [in] 名前
    const NodeImpl* output, ///< [in] 出力のノード番号
    const NodeImpl* src,    ///< [in] 入力のノード番号
    char reset_val          ///< [in] リセット値 ('X', '0', '1')
  ) : ImplBase(model),
      mId{id},
      mName{name},
      mOutput{output},
      mSrc{src},
      mResetVal{reset_val}
  {
  }

  /// @brief デストラクタ
  ~DffImpl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 名前を返す．
  const std::string&
  name() const
  {
    return mName;
  }

  /// @brief 出力のノードを返す．
  const NodeImpl*
  output() const
  {
    return mOutput;
  }

  /// @brief 入力のノードを返す．
  const NodeImpl*
  src() const
  {
    return mSrc;
  }

  /// @brief リセット値を返す．
  char
  reset_val() const
  {
    return mResetVal;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // 名前
  std::string mName;

  // 出力のノード
  const NodeImpl* mOutput;

  // 入力のノード
  const NodeImpl* mSrc;

  // リセット値
  char mResetVal;

};


END_NAMESPACE_YM_BN

#endif // DFFIMPL_H
