#ifndef EIPRIMARRAY_H
#define EIPRIMARRAY_H

/// @file EiPrimArray.h
/// @brief EiPrimArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimBase.h"
#include "elaborator/RangeVal.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiPrimArray;

//////////////////////////////////////////////////////////////////////
/// @class EiPrimElem EiPrimArray.h "EiPrimArray.h"
/// @brief 配列要素の EiPrimitive
//////////////////////////////////////////////////////////////////////
class EiPrimElem :
  public EiPrimBase
{
public:

  /// @brief コンストラクタ
  EiPrimElem() = default;

  /// @brief デストラクタ
  ~EiPrimElem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期設定を行う．
  void
  init(
    EiPrimArray* prim_array, ///< [in] 親の配列
    SizeType index,          ///< [in] インデックス番号
    SizeType port_num        ///< [in] 端子数
  );

  /// @brief 初期設定を行う．
  void
  init(
    EiPrimArray* prim_array, ///< [in] 親の配列
    SizeType index,          ///< [in] インデックス番号
    SizeType port_num,       ///< [in] 端子数
    const ClibCell& cell     ///< [in] セル
  );


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiPrimitive の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを得る．
  ElbPrimHead*
  head() const override;

  /// @brief パース木のインスタンス定義を得る．
  AstInst
  ast_inst() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の配列
  EiPrimArray* mPrimArray;

  // インデックス番号
  SizeType mIndex;

};


//////////////////////////////////////////////////////////////////////
/// @class EiPrimArray EiPrimArray.h "EiPrimArray.h"
/// @brief ElbPrimArray の実装クラス
//////////////////////////////////////////////////////////////////////
class EiPrimArray :
  public ElbPrimArray
{
public:

  /// @brief コンストラクタ
  EiPrimArray(
    ElbPrimHead* head,         ///< [in] ヘッダ
    const AstInst& ast_inst,   ///< [in] インスタンス定義
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  ) : mHead{head},
      mAstInst{ast_inst},
      mRange(ast_range, range),
      mArray(mRange.calc_size())
  {
    SizeType n = mRange.calc_size();
    SizeType port_num = ast_inst.port_list().size();
    for ( SizeType i = 0; i < n; ++ i ) {
      SizeType index = mRange.index(i);
      mArray[i].init(this, index, port_num);
    }
  }

  /// @brief コンストラクタ
  EiPrimArray(
    ElbPrimHead* head,         ///< [in] ヘッダ
    const ClibCell& cell,      ///< [in] セル
    const AstInst& ast_inst,   ///< [in] インスタンス定義
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  ) : mHead{head},
      mAstInst{ast_inst},
      mRange(ast_range, range),
      mArray(mRange.calc_size())
  {
    SizeType n = mRange.calc_size();
    SizeType port_num = ast_inst.port_list().size();
    for ( SizeType i = 0; i < n; ++ i ) {
      SizeType index = mRange.index(i);
      mArray[i].init(this, index, port_num);
    }
  }

  /// @brief デストラクタ
  ~EiPrimArray() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 名前の取得
  std::string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlPrimArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primitive type を返す．
  VpiPrimType
  prim_type() const override;

  /// @brief プリミティブの定義名を返す．
  std::string
  def_name() const override;

  /// @brief UDP 定義を返す．
  const VlUdpDefn*
  udp_defn() const override;

  /// @brief セルを返す．
  ClibCell
  cell() const override;

  /// @brief 0 の強さを得る．
  VpiStrength
  drive0() const override;

  /// @brief 1 の強さを得る．
  VpiStrength
  drive1() const override;

  /// @brief 遅延式を得る．
  const VlDelay*
  delay() const override;

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

  /// @brief 要素数を返す．
  SizeType
  elem_num() const override;

  /// @brief 要素のプリミティブを返す．
  const VlPrimitive*
  elem_by_offset(
    SizeType offset ///< [in] 位置番号 ( 0 <= offset < elem_num() )
  ) const override;

  /// @brief 要素を返す．
  const VlPrimitive*
  elem_by_index(
    int index ///< [in] インデックス
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のプリミティブを取り出す．
  ElbPrimitive*
  _primitive_by_offset(
    SizeType offset
  ) override;

  /// @brief 要素のプリミティブを取り出す．
  ElbPrimitive*
  _primitive_by_index(
    int index
  ) override;

  /// @brief ヘッダを得る．
  ElbPrimHead*
  head() const override;

  /// @brief パース木のインスタンス定義を得る．
  AstInst
  ast_inst() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  ElbPrimHead* mHead;

  // パース木のインスタンス定義
  AstInst mAstInst;

  // 範囲の情報
  RangeInfo mRange;

  // 要素の配列
  std::vector<EiPrimElem> mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMARRAY_H
