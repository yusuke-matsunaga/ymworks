#ifndef BNDFF_H
#define BNDFF_H

/// @file BnDff.h
/// @brief BnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/BnBase.h"


BEGIN_NAMESPACE_YM_BN

class DffImpl;

//////////////////////////////////////////////////////////////////////
/// @class BnDff BnDff.h "BnDff.h"
/// @brief BnModel のDFFを表すクラス
///
/// 以下の情報を持つ．
/// - ID番号
/// - 名前
/// - 出力のノード
/// - 入力のノード
/// - リセット値
///
/// 空のコンストラクタで作られたインスタンスは不正値となる．
/// その場合，is_valid() は false を返し，id() は BAD_ID を返すが，
/// それ以外のメンバ関数の呼び出しは std::invalid_argument 例外を送出する．
///
/// 公開されているメンバ関数はすべて const であり，内容を変更することは
/// できない．
///
/// 通常は BnModel のみが生成/設定を行う．
//////////////////////////////////////////////////////////////////////
class BnDff :
  public BnBase
{
  friend class BnBase;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 不正な値となる．
  BnDff() = default;

  /// @brief デストラクタ
  ~BnDff();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 名前を返す．
  const std::string&
  name() const;

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
    return BnBase::operator==(right) && mId == right.mId;
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
  ///
  /// これは BnBase のみが使用する．
  BnDff(
    const std::shared_ptr<ModelImpl>& model, ///< [in] 親のモデル．
    SizeType id                              ///< [in] ノード番号
  );

  /// @brief DFFの実体を返す．
  const DffImpl&
  _dff_impl() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  SizeType mId{BAD_ID};

};

END_NAMESPACE_YM_BN

#endif // BNDFF_H
