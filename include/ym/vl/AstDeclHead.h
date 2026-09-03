#ifndef YM_VL_ASTDECLHEAD_H
#define YM_VL_ASTDECLHEAD_H

/// @file ym/vl/AstDeclHead.h
/// @brief AstDeclHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtDeclHead;

//////////////////////////////////////////////////////////////////////
/// @class AstDeclHead AstDeclHead.h "ym/vl/AstDeclHead.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////
class AstDeclHead :
  public AstBaseWithPtr<const PtDeclHead>
{
public:

  /// @brief AstDeclH の派生クラスを区別するための列挙型
  enum Type : std::uint8_t {
    Param,      ///< @brief parameter
    LocalParam, ///< @brief localparam
    Reg,        ///< @brief reg
    Var,        ///< @brief variables
    Genvar,     ///< @brief genvar
    Net,        ///< @brief net
    Event,      ///< @brief named event
    SpecParam   ///< @brief specparam
  };


public:

  /// @brief コンストラクタ
  explicit
  AstDeclHead(
    const PtDeclHead* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtDeclHead>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstDeclHead() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素の型の取得
  /// @return 宣言要素の型
  Type
  type() const;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const;

  /// @brief 範囲Bの取得
  ///
  /// 範囲を持たないときは無効なオブエジェクトが返される．
  AstRange
  range() const;

  /// @brief データ型の取得
  /// @retval データ型 type() = Param, LocalParam, Var の場合
  /// @retval VpiVarType::None 上記以外
  VpiVarType
  data_type() const;

  /// @brief ネット型の取得
  /// @retval ネット型 type() = Net の場合
  /// @retval VpiNetType::None ネット型でない場合
  VpiNetType
  net_type() const;

  /// @brief vectored|scalared 属性の取得
  /// @retval None vectored|scalared 指定なし
  /// @retval Vectored vectored 指定あり
  /// @retval Scalared scalared 指定あり
  VpiVsType
  vs_type() const;

  /// @brief strength の取得
  /// @retval strength
  /// @retval nullptr strength の指定なし
  AstStrength
  strength() const;

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  AstDelay
  delay() const;

  /// @brief 要素のリストを返す．
  AstDeclItemList
  item_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const override;

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 比較用のユニークなキーを返す．
  PtrIntType
  key() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstDeclHead
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief json_obj() の下請け関数
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const override;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTDECLHEAD_H
