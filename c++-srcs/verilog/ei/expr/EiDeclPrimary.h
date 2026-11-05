#ifndef EIDECLPRIMARY_H
#define EIDECLPRIMARY_H

/// @file EiDeclPrimary.h
/// @brief EiDeclPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclPrimary EiDeclPrimary.h "EiDeclPrimary.h"
/// @brief VlDecl を持つプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPrimary :
  public EiPrimaryExprBase
{
public:

  /// @brief コンストラクタ
  EiDeclPrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj        ///< [in] 本体のオブジェクト
  ) : EiPrimaryExprBase(ast_expr),
      mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiDeclPrimary() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
  const VlDecl*
  decl_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlDecl* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclSelect EiDeclPrimary.h "EiDeclPrimary.h"
/// @brief Decl の部分選択を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiDeclSelect :
  public EiDeclPrimary
{
public:

  /// @brief コンストラクタ
  EiDeclSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclPrimary(ast_expr, obj),
      mWidth{width}
  {
  }

  /// @brief デストラクタ
  ~EiDeclSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するビット/部分選択を持つ時 true を返す．
  ///
  /// - expr_type() が PRIMARY かつ primary_type() が DECL,
  ///   CONST_DECL_ARRAY, VAR_DECL_ARRAY の場合に意味を持つ．
  bool
  has_bit_select() const override;

  /// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
  bool
  has_const_bit_select() const override;

  /// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  /// - 可変ビット選択の場合も true を返す．
  bool
  has_plus_bit_select() const override;

  /// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  bool
  has_minus_bit_select() const override;

  /// @brief 宣言要素に対する部分選択のビット幅を返す．
  ///
  /// - has_bit_select() == true の時のみ意味を持つ．
  /// - ビット選択の場合は 1 を返す．
  SizeType
  select_width() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 選択するビット幅
  SizeType mWidth;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclConstSelect EiDeclPrimary.h "EiDeclPrimary.h"
/// @brief 固定インデックスのビット/部分指定を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclConstSelect :
  public EiDeclSelect
{
public:

  /// @brief コンストラクタ
  EiDeclConstSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    SizeType bit_offset,     ///< [in] ビット位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclSelect(ast_expr, obj, width),
      mBitOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiDeclConstSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
  bool
  has_const_bit_select() const override;

  /// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
  ///
  /// - has_const_bit_select() == true の場合のみ意味を持つ．
  SizeType
  bit_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビット位置のオフセット
  SizeType mBitOffset;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclVarSelect EiDeclPrimary.h "EiDeclPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclVarSelect :
  public EiDeclSelect
{
public:

  /// @brief コンストラクタ
  EiDeclVarSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclSelect(ast_expr, obj, width),
      mBitIndex{bit_index}
  {
  }

  /// @brief デストラクタ
  ~EiDeclVarSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対する可変ビット/部分選択のインデックス式を返す．
  ///
  /// - has_plus_bit_select() か has_minus_bit_select() が true
  ///   の場合のみ意味を持つ．
  const VlExpr*
  bit_index() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビットインデックスの式
  const VlExpr* mBitIndex;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclPlusSelect EiDeclPrimary.h "EiDeclPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPlusSelect :
  public EiDeclVarSelect
{
public:

  /// @brief コンストラクタ
  EiDeclPlusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclVarSelect(ast_expr, obj, bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiDeclPlusSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  /// - 可変ビット選択の場合も true を返す．
  bool
  has_plus_bit_select() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclMinusSelect EiDeclPrimary.h "EiDeclPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclMinusSelect :
  public EiDeclVarSelect
{
public:

  /// @brief コンストラクタ
  EiDeclMinusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclVarSelect(ast_expr, obj, bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiDeclMinusSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  /// - 可変ビット選択の場合も true を返す．
  bool
  has_minus_bit_select() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLPRIMARY_H
