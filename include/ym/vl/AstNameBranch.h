#ifndef YM_VL_ASTNAMEBRANCH_H
#define YM_VL_ASTNAMEBRANCH_H

/// @file ym/vl/AstNameBranch.h
/// @brief AstNameBranch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtNameBranch;

//////////////////////////////////////////////////////////////////////
/// @class AstNameBranch AstNameBranch.h "ym/vl/AstNameBranch.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 階層名を表すクラス
///
/// 通常はただの文字列だが，場合によっては '[' '1' ']' のようなインデックスを含む．
///
/// AstBase の継承クラスではない．
//////////////////////////////////////////////////////////////////////
class AstNameBranch
{
public:

  /// @brief コンストラクタ
  explicit
  AstNameBranch(
    const PtNameBranch* ptr = nullptr ///< [in] 実体のポインタ
  ) : mPtr{ptr}
  {
  }

  /// @brief デストラクタ
  ~AstNameBranch() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mPtr != nullptr;
  }

  /// @brief 適切な値を持っていない時 true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 名前の取得
  /// @return 名前
  const char*
  name() const;

  /// @brief インデックスの有無のチェック
  /// @retval true インデックスを持っている時
  /// @retval false インデックスを持っていない時
  bool
  has_index() const;

  /// @brief インデックスの取得
  /// @return インデックスの値
  ///
  /// has_index() == false の時は std::logic_error 例外を送出する．
  int
  index() const;

  /// @brief インデックスを含めた名前を返す．
  std::string
  decompile() const;

  /// @brief 内容を JsonValue に変換する．
  JsonValue
  json_obj() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素に移動する．
  AstNameBranch
  next() const;

  /// @brief 等価比較演算子
  bool
  operator==(
    const AstNameBranch& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AstNameBranch& right
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 有効な値を持っているか調べる．
  void
  _check_ptr() const
  {
    if ( is_invalid() ) {
      throw std::logic_error{"is_invalid()"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体のポインタ
  const PtNameBranch* mPtr{nullptr};

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTNAMEBRANCH_H
