#ifndef EIDECLHEAD_H
#define EIDECLHEAD_H

/// @file EiDeclHead.h
/// @brief EiDeclHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbDecl.h"
#include "ei/EiRange.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclHead.h EiDeclHead.h "EiDeclHead.h"
/// @brief 宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclHead :
  public ElbDeclHead
{
public:

  /// @brief コンストラクタ
  EiDeclHead(
    const VlScope* parent ///< [in] 親のスコープ
  );

  /// @brief デストラクタ
  ~EiDeclHead();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbDeclHead の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlScope* mParent;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclHeadAst.h EiDeclHead.h "EiDeclHead.h"
/// @brief AstDeclH を用いた宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclHeadAst :
  public EiDeclHead
{
public:

  /// @brief コンストラクタ
  EiDeclHeadAst(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstDeclHead* ast_head ///< [in] パース木の宣言ヘッダ
  );

  /// @brief デストラクタ
  ~EiDeclHeadAst();


public:
  //////////////////////////////////////////////////////////////////////
  // EiDeclHead の派生クラスに共通の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] インデックスに対するオフセット値
  ) const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  VpiNetType
  net_type() const override;

  /// @brief vectored|scalared 属性の取得
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  VpiVsType
  vs_type() const override;

  /// @brief drive0 strength の取得
  /// @retval 0 の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  drive0() const override;

  /// @brief drive1 strength の取得
  /// @retval 1 の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  drive1() const override;

  /// @brief charge strength の取得
  /// @retval 電荷の強度
  /// @retval kVpiNoStrength strength の指定なし
  VpiStrength
  charge() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の宣言ヘッダ
  const AstDeclHead* mAstHead;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadAstD EiDeclHead.h "EiDeclHead.h"
/// @brief delay 付きの EiDeclHeadAst
//////////////////////////////////////////////////////////////////////
class EiDeclHeadAstD :
  public EiDeclHeadAst
{
public:

  /// @brief コンストラクタ
  EiDeclHeadAstD(
    const VlScope* parent,    ///< [in] 親のスコープ
    const AstDeclHead* ast_head ///< [in] パース木の宣言ヘッダ
  );

  /// @brief デストラクタ
  ~EiDeclHeadAstD();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  const VlDelay*
  delay() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の設定
  void
  set_delay(
    const VlDelay* delay ///< [in] 遅延式
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延式
  const VlDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadAstV EiDeclHead.h "EiDeclHead.h"
/// @brief 範囲付きの EiDeclHeadAst
//////////////////////////////////////////////////////////////////////
class EiDeclHeadAstV :
  public EiDeclHeadAst
{
public:

  /// @brief コンストラクタ
  EiDeclHeadAstV(
    const VlScope* parent,     ///< [in] ヘッダ
    const AstDeclHead* ast_head, ///< [in] パース木の宣言ヘッダ
    const AstRange* ast_range,   ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  );

  /// @brief デストラクタ
  ~EiDeclHeadAstV();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] インデックスに対するオフセット値
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲の情報
  RangeInfo mRange;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadAstVD EiDeclHead.h "EiDeclHead.h"
/// @brief 範囲付き，delay 付きの EiDeclHeadAst
//////////////////////////////////////////////////////////////////////
class EiDeclHeadAstVD :
  public EiDeclHeadAstV
{
public:

  /// @brief コンストラクタ
  EiDeclHeadAstVD(
    const VlScope* parent,     ///< [in] ヘッダ
    const AstDeclHead* ast_head, ///< [in] パース木の宣言ヘッダ
    const AstRange* ast_range,   ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  );

  /// @brief デストラクタ
  ~EiDeclHeadAstVD();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay の取得
  /// @retval delay
  /// @retval nullptr delay の指定なし
  const VlDelay*
  delay() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の設定
  void
  set_delay(
    const VlDelay* delay ///< [in] 遅延式
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 遅延式
  const VlDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclHeadAst2.h EiDeclHead.h "EiDeclHead.h"
/// @brief AstIOH を用いた宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclHeadAst2 :
  public EiDeclHead
{
public:

  /// @brief コンストラクタ
  EiDeclHeadAst2(
    const VlScope* parent,   ///< [in] 親のスコープ
    const AstIOHead* ast_head, ///< [in] パース木のIO宣言ヘッダ
    VpiAuxType aux_type      ///< [in] 補助的なデータ型
  );

  /// @brief デストラクタ
  ~EiDeclHeadAst2();


public:
  //////////////////////////////////////////////////////////////////////
  // EiDeclHead の派生クラスに共通の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return vpi_user.h で定義された型 (vpiModule など)
  VpiObjType
  type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] インデックスに対するオフセット値
  ) const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  VpiNetType
  net_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木のIO宣言ヘッダ
  const AstIOHead* mAstHead;

  // 補助的なデータ型
  VpiAuxType mAuxType;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadAst2V EiDeclHead.h "EiDeclHead.h"
/// @brief 範囲付きの EiDeclHeadAst2
//////////////////////////////////////////////////////////////////////
class EiDeclHeadAst2V :
  public EiDeclHeadAst2
{
public:

  /// @brief コンストラクタ
  EiDeclHeadAst2V(
    const VlScope* parent,   ///< [in] ヘッダ
    const AstIOHead* ast_head, ///< [in] パース木のIO宣言ヘッダ
    VpiAuxType aux_type,     ///< [in] 補助的なデータ型
    const AstRange* ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range    ///< [in] 範囲の値
  );

  /// @brief デストラクタ
  ~EiDeclHeadAst2V();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] インデックスに対するオフセット値
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲の情報
  RangeInfo mRange;

};


//////////////////////////////////////////////////////////////////////
/// @class EiDeclHeadAst3.h EiDeclHead.h "EiDeclHead.h"
/// @brief AstItem を用いた宣言要素のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclHeadAst3 :
  public EiDeclHead
{
public:

  /// @brief コンストラクタ
  EiDeclHeadAst3(
    const VlScope* parent, ///< [in] 親のスコープ
    const AstItem* ast_item  ///< [in] パース木の宣言要素
  );

  /// @brief デストラクタ
  ~EiDeclHeadAst3();


public:
  //////////////////////////////////////////////////////////////////////
  // EiDeclHead の派生クラスに共通の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return vpi_user.h で定義された型 (vpiModule など)
  VpiObjType
  type() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] インデックスに対するオフセット値
  ) const override;

  /// @brief データ型の取得
  /// @retval データ型 kParam, kLocalParam, kVar の場合
  /// @retval kVpiVarNone 上記以外
  VpiVarType
  data_type() const override;

  /// @brief net 型の取得
  /// @retval net 型 net 型の要素の場合
  /// @retval kVpiNone net 型の要素でない場合
  ///
  /// このクラスでは kVpiNone を返す．
  VpiNetType
  net_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の関数定義
  const AstItem* mAstItem;

};


//////////////////////////////////////////////////////////////////////
/// @clacc EiDeclHeadAst3V EiDeclHead.h "EiDeclHead.h"
/// @brief 範囲付きの EiDeclHeadAst3
//////////////////////////////////////////////////////////////////////
class EiDeclHeadAst3V :
  public EiDeclHeadAst3
{
public:

  /// @brief コンストラクタ
  EiDeclHeadAst3V(
    const VlScope* parent,   ///< [in] ヘッダ
    const AstItem* ast_item,   ///< [in] パース木の宣言ヘッダ
    const AstRange* ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range    ///< [in] 範囲の値
  );

  /// @brief デストラクタ
  ~EiDeclHeadAst3V();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲指定を持つとき true を返す．
  bool
  has_range() const override;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const override;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const override;

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_range_string() const override;

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_range_string() const override;

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const override;

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const override;

  /// @brief ビット幅を返す．
  SizeType
  bit_size() const override;

  /// @brief オフセット値の取得
  /// @retval true インデックスが範囲内に入っている時
  /// @retval false インデックスが範囲外の時
  bool
  calc_bit_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] インデックスに対するオフセット値
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲の情報
  RangeInfo mRange;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLHEAD_H
