#ifndef EIMODULEARRAY_H
#define EIMODULEARRAY_H

/// @file EiModuleArray.h
/// @brief EiModuleArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiModuleBase.h"
#include "EiModuleHead.h"
#include "elaborator/ElbModuleArray.h"
#include "elaborator/RangeVal.h"


BEGIN_NAMESPACE_YM_VERILOG

class EiModuleArray;

//////////////////////////////////////////////////////////////////////
/// @class EiModuleElem EiModuleArray.h "EiModuleArray.h"
/// @brief 配列要素の EiModule
//////////////////////////////////////////////////////////////////////
class EiModuleElem :
  public EiModuleBase
{
public:

  /// @brief コンストラクタ
  EiModuleElem() = default;

  /// @brief デストラクタ
  ~EiModuleElem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期設定を行う．
  void
  init(
    SizeType port_num,           ///< [in] ポート配列
    SizeType io_num,             ///< [in] IO 配列
    EiModuleArray* module_array, ///< [in] 親の配列
    int index                    ///< [in] 配列中のインデックス
  );


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列要素の時 true を返す．
  bool
  is_array() const override;

  /// @brief インデックスの値を返す．
  int
  index() const override;

  /// @brief 親の配列を返す．
  const VlModuleArray*
  module_array() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiModule の派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  const EiModuleHead&
  head() const override;

  /// @brief ヘッダ情報を返す．
  EiModuleHead&
  head() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の配列
  EiModuleArray* mModuleArray;

  // インデックス
  int mIndex;

  // 名前
  std::string mName;

};


//////////////////////////////////////////////////////////////////////
/// @class EiModuleArray EiModuleArray.h "EiModuleArray.h"
/// @brief VlModuleArray の実装クラス
//////////////////////////////////////////////////////////////////////
class EiModuleArray :
  public ElbModuleArray
{
public:

  /// @brief コンストラクタ
  EiModuleArray(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstModule& ast_module, ///< [in] モジュールテンプレート
    const AstItem& ast_head,     ///< [in] パース木のヘッダ定義
    const AstInst& ast_inst,     ///< [in] パース木のインスタンス定義
    const AstRange& ast_range,   ///< [in] パース木の範囲定義
    const RangeVal& range        ///< [in] 範囲の値
  ) : mHead(parent, ast_module, ast_head, ast_inst),
      mRange(ast_range, range),
      mArray(mRange.calc_size())
  {
    auto port_num = ast_module.port_list().size();
    auto io_num = ast_module.iohead_list().size();
    auto n = mArray.size();
    for ( SizeType i = 0; i < n; ++ i ) {
      int index = range.index(i);
      mArray[i].init(port_num, io_num, this, index);
    }
  }

  /// @brief デストラクタ
  ~EiModuleArray() = default;


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
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このオブジェクトの属しているスコープを返す．
  const VlScope*
  parent_scope() const override;

  /// @brief 名前の取得
  std::string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModuleArray の仮想関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief 要素を返す．
  const VlModule*
  elem_by_offset(
    SizeType offset ///< [in] 位置番号 (0 <= offset < elem_num())
  ) const override;

  /// @brief 要素を返す．
  const VlModule*
  elem_by_index(
    int index ///< [in] インデックス
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbModuleArray の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を取り出す．
  ElbModule*
  elem(
    SizeType index ///< [in] モジュール番号
  ) override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiModuleArray の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダを返す．
  const EiModuleHead&
  head() const;

  /// @brief ヘッダを返す．
  EiModuleHead&
  head();

  /// @brief 範囲を返す．
  const RangeVal&
  range() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  EiModuleHead mHead;

  // 範囲の情報
  RangeInfo mRange;

  // 要素の配列
  std::vector<EiModuleElem> mArray;

};

END_NAMESPACE_YM_VERILOG

#endif // EIMODULEARRAY_H
