#ifndef EIDECLELEMPRIMARY_H
#define EIDECLELEMPRIMARY_H

/// @file EiDeclElemPrimary.h
/// @brief EiDeclElemPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclElemPrimary EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclElemPrimary :
  public EiPrimaryExprBase
{
public:

  /// @brief コンストラクタ
  EiDeclElemPrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj   ///< [in] 本体のオブジェクト
  ) : EiPrimaryExprBase(ast_expr),
    mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiDeclElemPrimary() = default;


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
  const VlDeclArray*
  declarray_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlDeclArray* mObj;

};


//////////////////////////////////////////////////////////////////////
/// @class EiConstDeclElemPrimary EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 固定インデックスの配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemPrimary :
  public EiDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemPrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset    ///< [in] 配列上の位置のオフセット
  ) : EiDeclElemPrimary(ast_expr, obj),
      mOffset{array_offset}
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemPrimary() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列型宣言要素への参照のオフセットを返す．
  SizeType
  declarray_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オフセット
  SizeType mOffset;

};


//////////////////////////////////////////////////////////////////////
/// @class EiConstDeclElemSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief DeclArray の固定インデックス要素の部分選択を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemSelect :
  public EiConstDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemPrimary(ast_expr, obj, array_offset),
      mWidth{width}
  {
  }


  /// @brief デストラクタ
  ~EiConstDeclElemSelect() = default;


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
/// @class EiConstDeclElemConstSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief EiConstDeclSelect の固定インデックスのビット/部分指定を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemConstSelect :
  public EiConstDeclElemSelect
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemConstSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    SizeType bit_offset,     ///< [in] ビット位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemSelect(ast_expr, obj, array_offset, width),
      mBitOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemConstSelect() = default;


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
/// @class EiConstDeclElemVarSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemVarSelect :
  public EiConstDeclElemSelect
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemVarSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemSelect(ast_expr, obj, array_offset, width),
      mBitIndex{bit_index}
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemVarSelect() = default;


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
/// @class EiConstDeclElemPlusSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemPlusSelect :
  public EiConstDeclElemVarSelect
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemPlusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemVarSelect(ast_expr, obj, array_offset,
			       bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemPlusSelect() = default;


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
/// @class EiConstDeclElemMinusSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemMinusSelect :
  public EiConstDeclElemVarSelect
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemMinusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemVarSelect(ast_expr, obj, array_offset,
			       bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemMinusSelect() = default;


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


//////////////////////////////////////////////////////////////////////
/// @class EiVarDeclElemPrimary EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 可変インデックスの配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemPrimary :
  public EiDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemPrimary(
    const AstExpr& ast_expr,                     ///< [in] パース木の定義要素
    const VlDeclArray* obj,                      ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list ///< [in] インデックスのリスト
  ) : EiDeclElemPrimary(ast_expr, obj),
      mIndexList{index_list}
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemPrimary() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスリストを返す．
  ///
  /// - primary_type() が CONST_DECL_ELEM, VAR_DECL_ELEM の場合のみ意味を持つ．
  std::vector<const VlExpr*>
  declarray_index_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックスのリスト
  std::vector<const VlExpr*> mIndexList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiVarDeclElemSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief Decl の部分選択を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemSelect :
  public EiVarDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemPrimary(ast_expr, obj, index_list),
      mWidth{width}
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemSelect() = default;


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
/// @class EiVarDeclElemConstSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 固定インデックスのビット/部分指定を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemConstSelect :
  public EiVarDeclElemSelect
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemConstSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    SizeType bit_offset,     ///< [in] ビット位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemSelect(ast_expr, obj, index_list, width),
      mBitOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemConstSelect() = default;


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
/// @class EiVarDeclElemVarSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemVarSelect :
  public EiVarDeclElemSelect
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemVarSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemSelect(ast_expr, obj, index_list, width),
      mBitIndex{bit_index}
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemVarSelect() = default;


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
/// @class EiVarDeclElemPlusSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemPlusSelect :
  public EiVarDeclElemVarSelect
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemPlusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemVarSelect(ast_expr, obj, index_list,
			     bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemPlusSelect() = default;


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
/// @class EiVarDeclElemMinusSelect EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemMinusSelect :
  public EiVarDeclElemVarSelect
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemMinusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemVarSelect(ast_expr, obj, index_list,
			     bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemMinusSelect() = default;


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

#endif // EIDECLELEMPRIMARY_H
