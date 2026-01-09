#ifndef FOINFO_H
#define FOINFO_H

/// @file FoInfo.h
/// @brief FoInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"


BEGIN_NAMESPACE_YM_AIG

class AigNode;
class AigTerminal;

//////////////////////////////////////////////////////////////////////
/// @class FoInfo FoInfo.h "FoInfo.h"
/// @brief ファンアウト先の情報を表すクラス
///
/// ファンアウト先は AigNode のファンインの場合と AigTerminal
/// の場合がある．
/// これらを区別するためにポインタの下位1ビットを用いる．
/// なお，AigNode のファンインは場合によって入れ替わるのでどちらの
/// ファンインに接続しているからは記録しない．
//////////////////////////////////////////////////////////////////////
class FoInfo
{
public:

  /// @brief 空のコンストラクタ
  FoInfo() = default;

  /// @brief AigNode のファンインを表すコンストラクタ
  FoInfo(
    AigNode* node
  ) : mPtr{reinterpret_cast<PtrIntType>(node)}
  {
  }

  /// @brief AigTerminal のファンインを表すコンストラクタ
  FoInfo(
    AigTerminal* terminal
  ) : mPtr{reinterpret_cast<PtrIntType>(terminal) | 1}
  {
  }

  /// @brief デストラクタ
  ~FoInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief AigNode のファンインの時 true を返す．
  bool
  is_node() const
  {
    return static_cast<bool>((mPtr & 1) == 0);
  }

  /// @brief AigTerminal のファンインの時 true を返す．
  bool
  is_terminal() const
  {
    return !is_node();
  }

  /// @brief ファンアウト先の AigNode を返す．
  AigNode*
  node() const
  {
    if ( !is_node() ) {
      throw std::invalid_argument{"not AigNode type"};
    }
    return reinterpret_cast<AigNode*>(mPtr & ~1UL);
  }

  /// @brief ファンアウト先の AigTerminal を返す．
  AigTerminal*
  terminal() const
  {
    if ( !is_terminal() ) {
      throw std::invalid_argument{"not AigTerminal type"};
    }
    return reinterpret_cast<AigTerminal*>(mPtr& ~1UL);
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const FoInfo& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const FoInfo& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタとフラグを組み合わせたもの．
  PtrIntType mPtr{0};

};

END_NAMESPACE_YM_AIG

#endif // FOINFO_H
