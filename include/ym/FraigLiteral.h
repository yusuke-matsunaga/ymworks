#ifndef FRAIGLITERAL_H
#define FRAIGLITERAL_H

/// @file FraigLiteral.h
/// @brief FraigLiteral のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/fraig.h"


BEGIN_NAMESPACE_YM_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class FraigLiteral FraigLiteral.h "ym/FraigLiteral.h"
/// @brief Fraig の枝を表すクラス
///
/// 具体的にはノード番号と極性を表す1ビットの情報をまとめたもの
/// ノード番号0番は特殊なノードで定数0を表す
///
/// ただし，このクラスの内容は FraigMgrImpl 以外は読み出すことが
/// できない．
/// また，定数値以外の値は FraigMgr の関数の戻り値として返された値をそのまま
/// もちいることしかできない．
//////////////////////////////////////////////////////////////////////
class FraigLiteral
{
  friend class FraigMgrImpl;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタと生成/内容の設定
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  FraigLiteral() = default;

  /// @brief 定数０のハンドルを返す．
  static
  FraigLiteral
  zero()
  {
    return FraigLiteral{0};
  }

  /// @brief 定数1のハンドルを返す．
  static
  FraigLiteral
  one()
  {
    return FraigLiteral{1};
  }

  /// @brief デストラクタ
  ~FraigLiteral() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部の情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0を指しているとき true を返す．
  bool
  is_zero() const
  {
    return mPackedData == 0UL;
  }

  /// @brief 定数1を指しているとき true を返す．
  bool
  is_one() const
  {
    return mPackedData == 1UL;
  }

  /// @brief 定数を指しているとき true を返す．
  ///
  /// = is_zero() || is_one()
  bool
  is_const() const
  {
    return (mPackedData & ~1UL) == 0UL;
  }

  /// @brief ANDノードを指している時 true を返す．
  ///
  /// 実際には入力ノードの場合も含む．
  bool
  is_and() const
  {
    return !is_const();
  }

  /// @brief 否定の枝を返す．
  FraigLiteral
  operator~() const
  {
    return FraigLiteral{mPackedData ^ 1UL};
  }

  /// @brief 極性をかけ合わせる．
  FraigLiteral
  operator*(
    bool inv ///< [in] 極性
  ) const
  {
    return FraigLiteral{mPackedData ^ static_cast<SizeType>(inv)};
  }

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const
  {
    return static_cast<SizeType>((mPackedData * mPackedData) >> 20);
  }

  /// @brief 等価比較関数
  bool
  operator==(
    FraigLiteral src2 ///< [in] オペランド
  ) const
  {
    return mPackedData == src2.mPackedData;
  }

  /// @brief 非等価比較演算
  bool
  operator!=(
    FraigLiteral src2  ///< [in] オペランド2
  )
  {
    return !operator==(src2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を直接指定したコンストラクタ
  explicit
  FraigLiteral(
    SizeType data ///< [in] 値
  ) : mPackedData{data}
  {
  }

  /// @brief 内容を直接指定したコンストラクタ
  explicit
  FraigLiteral(
    SizeType node_id, ///< [in] ノード番号
    bool inv          ///< [in] 反転属性
  ) : mPackedData{(node_id + 1) * 2 + static_cast<SizeType>(inv)}
  {
  }

  /// @brief ノード番号を得る．
  SizeType
  node_id() const
  {
    if ( is_const() ) {
      throw std::invalid_argument{"not a FraigNode"};
    }
    return (mPackedData >> 1) - 1;
  }

  /// @brief 極性を得る．
  /// @return 反転しているとき true を返す．
  bool
  inv() const
  {
    return static_cast<bool>(mPackedData & 1UL);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号＋極性
  SizeType mPackedData{0};

};

END_NAMESPACE_YM_FRAIG

#endif // FRAIGLITERAL_H
