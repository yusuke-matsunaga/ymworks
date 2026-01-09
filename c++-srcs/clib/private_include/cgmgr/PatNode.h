#ifndef PATNODE_H
#define PATNODE_H

/// @file PatNode.h
/// @brief PatNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

class PatMgr;

//////////////////////////////////////////////////////////////////////
/// @class PatNode PatNode.h "PatNode.h"
/// @brief パタン生成用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class PatNode
{
  friend class PatMgr;

public:

  /// @brief コンストラクタ
  explicit
  PatNode(
    SizeType id ///< [in] ID番号
  ) : mId{id},
      mType{0U},
      mFanin{nullptr, nullptr},
      mLink{nullptr}
  {
  }

  /// @brief デストラクタ
  ~PatNode() = default;


public:

  /// @brief ノード番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 入力の時 true を返す．
  bool
  is_input() const
  {
    return ((mType & 3U) == INPUT);
  }

  /// @brief AND の時 true を返す．
  bool
  is_and() const
  {
    return ((mType & 3U) == AND);
  }

  /// @brief XOR の時 true を返す．
  bool
  is_xor() const
  {
    return ((mType & 3U) == XOR);
  }

  /// @brief 入力の時に入力番号を返す．
  SizeType
  input_id() const
  {
    return mType >> 2;
  }

  /// @brief AND/XOR の時にファンインのノードを返す．
  const PatNode&
  fanin(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    ASSERT_COND( pos == 0 || pos == 1 );
    return *mFanin[pos];
  }

  /// @brief AND/XOR の時にファンイン0のノードを返す．
  const PatNode&
  fanin0() const
  {
    return *mFanin[0];
  }

  /// @brief AND/XOR の時にファンイン1のノードを返す．
  const PatNode&
  fanin1() const
  {
    return *mFanin[1];
  }

  /// @brief AND/XOR の時にファンインの極性を返す．
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    return static_cast<bool>((mType >> (pos + 2)) & 1U);
  }

  /// @brief AND/XOR の時にファンイン0の極性を返す．
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv0() const
  {
    return static_cast<bool>((mType >> 2) & 1U);
  }

  /// @brief AND/XOR の時にファンイン1の極性を返す．
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv1() const
  {
    return static_cast<bool>((mType >> 3) & 1U);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる定数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類
  enum Type {
    INPUT = 0,
    AND   = 1,
    XOR   = 2
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  SizeType mId;

  // ノードの種類 ( 2bit )
  //  + 入力番号 ( 30bit)
  // or
  //  + ファンインの極性 ( 1bit x 2)
  SizeType mType;

  // ファンインのノード
  const PatNode* mFanin[2];

  // ハッシュ表中のリンク
  PatNode* mLink;

};

END_NAMESPACE_YM_CLIB

#endif // PATNODE_H
