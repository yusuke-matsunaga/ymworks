#ifndef BNDFF_H
#define BNDFF_H

/// @file BnDff.h
/// @brief BnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"


BEGIN_NAMESPACE_YM_BN

class DffImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnDff BnDff.h "BnDff.h"
/// @brief BnModel のDFFを表すクラス
///
/// 以下の情報を持つ．
/// - ID番号
/// - 出力のノード
/// - 入力のノード
/// - リセット値
///
/// 空のコンストラクタで作られたインスタンスは不正値となる．
/// その場合，is_valid() は false を返し， is_invalid() は true を返す．
/// それ以外のメンバ関数の呼び出しは std::logic_error 例外を送出する．
///
/// 公開されているメンバ関数はすべて const であり，内容を変更することは
/// できない．
//////////////////////////////////////////////////////////////////////
class BnDff
{
  friend class BnModel;
  friend class BnNode;
  friend class BnDffIter;
  friend class BnDffIter2;
  friend class BnDffList;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnDff() = default;

  /// @brief コピーコンストラクタ
  BnDff(
    const BnDff& src ///< [in] コピー元のオブジェクト
  );

  /// @brief コピー代入演算子
  BnDff&
  operator=(
    const BnDff& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~BnDff();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持つ時 true を返す．
  bool
  is_valid() const
  {
    return mPtr != nullptr;
  }

  /// @brief 適正な値を持たないとき true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief ノード番号を返す．
  SizeType
  id() const;

  /// @brief 出力ノードを返す．
  BnNode
  output() const;

  /// @brief 入力ノードを返す．
  BnNode
  input() const;

  /// @brief リセット値
  char
  reset_val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  bool
  operator==(
    const BnDff& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BnDff& right ///< [in] 比較対象のオブジェクト
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  BnDff(
    const DffImpl* ptr ///< [in] 実装オブジェクトのポインタ
  );

  /// @brief DFFの実体を返す．
  const DffImpl&
  _dff_impl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装オブジェクトのポインタ
  const DffImpl* mPtr{nullptr};

};

END_NAMESPACE_YM_BN

#endif // BNDFF_H
