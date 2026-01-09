#ifndef YM_CLIBCELLLIBRARY_H
#define YM_CLIBCELLLIBRARY_H

/// @file ym/ClibCellLibrary.h
/// @brief ClibCellLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/logic.h"
#include "ym/ClibLibraryPtr.h"
#include "ym/ClibList.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellLibrary;

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibCellLibrary ClibCellLibrary.h "ym/ClibCellLibrary.h"
/// @brief セルライブラリを表すクラス
///
/// 実際には実体の CiCellLibrary へのスマートポインタとなっている．
/// アプリケーション側では CiCellLibrary のメモリ管理について気にする
/// 必要はない．
//////////////////////////////////////////////////////////////////////
class ClibCellLibrary
{
public:

  /// @brief コンストラクタ
  ///
  /// 空の状態で初期化される．
  ClibCellLibrary() = default;

  /// @brief 実体のポインタを引数にしたコンストラクタ
  ClibCellLibrary(
    ClibLibraryPtr ptr
  ) : mImpl{ptr}
  {
  }

  /// @brief コピーコンストラクタ
  ///
  /// '浅い'コピーを行う．
  ClibCellLibrary(
    const ClibCellLibrary& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief 代入演算子
  /// @return 代入後の自身への参照を返す．
  ClibCellLibrary&
  operator=(
    const ClibCellLibrary& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~ClibCellLibrary() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を設定する関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief mislib 形式のファイルを読み込む．
  /// @return 生成したライブラリを返す．
  ///
  /// 読み込みが失敗した場合は std::invalid_argument 例外を送出する．
  static
  ClibCellLibrary
  read_mislib(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief liberty 形式のファイルを読み込む．
  /// @return 生成したライブラリを返す．
  ///
  /// 読み込みが失敗した場合は std::invalid_argumnet 例外を送出する．
  static
  ClibCellLibrary
  read_liberty(
    const std::string& filename ///< [in] ファイル名
  );

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 属性の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を持っているときに true を返す．
  bool
  is_valid() const
  {
    return mImpl != nullptr;
  }

  /// @brief 名前の取得
  std::string
  name() const;

  /// @brief テクノロジの取得
  ///
  /// 返り値は
  /// - cmos
  /// - fpga
  /// のどちらか
  ClibTechnology
  technology() const;

  /// @brief 遅延モデルの取得
  ///
  /// 返り値は
  /// - generic_cmos
  /// - table_lookup
  /// - piecewise_cmos
  /// - cmos2
  /// - dcm
  /// のいずれか
  ClibDelayModel
  delay_model() const;

  /// @brief 区間のタイプの取得
  ///
  /// 返り値は
  /// - output_net_length ( piece_length )
  /// - output_pin_cap ( pice_pin_cap )
  /// - output_wire_cap ( piece_wire_cap )
  /// - total_output_net_capacitance ( piece_total_cap )
  /// - none
  /// のいずれか．
  /// それ以外の値は用いられない．
  ///
  /// delay_model() = ClibDelayModel::piecewise_cmos
  /// の時のみ意味を持つ．
  ClibVarType
  piece_type() const;

  /// @brief 区間のリストの取得
  ///
  /// delay_model() = ClibDelayModel::piecewise_cmos
  /// の時のみ意味を持つ．
  std::vector<double>
  piece_define() const;

  /// @brief バス命名規則の取得
  std::string
  bus_naming_style() const;

  /// @brief 日付情報の取得
  std::string
  date() const;

  /// @brief リビジョン情報の取得
  std::string
  revision() const;

  /// @brief コメント情報の取得
  std::string
  comment() const;

  /// @brief 時間単位の取得
  std::string
  time_unit() const;

  /// @brief 電圧単位の取得
  std::string
  voltage_unit() const;

  /// @brief 電流単位の取得
  std::string
  current_unit() const;

  /// @brief 抵抗単位の取得
  std::string
  pulling_resistance_unit() const;

  /// @brief 容量単位の取得
  ///
  /// なぜかここだけインターフェイスが異なる．
  double
  capacitive_load_unit() const;

  /// @brief 容量単位文字列の取得
  ///
  /// なぜかここだけインターフェイスが異なる．
  std::string
  capacitive_load_unit_str() const;

  /// @brief 電力単位の取得
  std::string
  leakage_power_unit() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name セル情報の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief このライブラリの持つセル数の取得
  SizeType
  cell_num() const;

  /// @brief セル情報の取得
  /// @return 該当するセル情報を返す．
  ClibCell
  cell(
    SizeType cell_id ///< [in] セル番号 ( 0 <= cell_id < cell_num() )
  ) const;

  /// @brief 名前からのセルの取得
  /// @return セルを返す．
  ///
  /// なければ不正値を返す．
  ClibCell
  cell(
    const std::string& name ///< [in] セル名
  ) const;

  /// @brief 全セルのリストの取得
  ClibCellList
  cell_list() const;

  /// @brief セルグループ数の取得
  SizeType
  cell_group_num() const;

  /// @brief セルグループの取得
  ClibCellGroup
  cell_group(
    SizeType id ///< [in] グループ番号 ( 0 <= id < cell_group_num() )
  ) const;

  /// @brief セルグループのリストの取得
  ClibCellGroupList
  cell_group_list() const;

  /// @brief NPN同値クラス数の取得
  SizeType
  npn_class_num() const;

  /// @brief NPN同値クラスの取得
  ClibCellClass
  npn_class(
    SizeType id ///< [in] 同値クラス番号 ( 0 <= id < npn_class_num() )
  ) const;

  /// @brief NPN同値クラスのリストの取得
  ClibCellClassList
  npn_class_list() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理セルの情報の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0セルのグループを返す．
  ClibCellGroup
  const0_func() const;

  /// @brief 定数1セルのグループを返す．
  ClibCellGroup
  const1_func() const;

  /// @brief バッファセルのグループを返す．
  ClibCellGroup
  buf_func() const;

  /// @brief インバータセルのグループを返す．
  ClibCellGroup
  inv_func() const;

  /// @brief ANDセルのグループを返す．
  ClibCellGroup
  and_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const;

  /// @brief NANDセルのグループを返す．
  ClibCellGroup
  nand_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const;

  /// @brief ORセルのグループを返す．
  ClibCellGroup
  or_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const;

  /// @brief NORセルのグループを返す．
  ClibCellGroup
  nor_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const;

  /// @brief XORセルのグループを返す．
  ClibCellGroup
  xor_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const;

  /// @brief XNORセルのグループを返す．
  ClibCellGroup
  xnor_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const;

  /// @brief MUX2セルのグループを返す．
  ClibCellGroup
  mux2_func() const;

  /// @brief MUX4セルのグループを返す．
  ClibCellGroup
  mux4_func() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // FFセルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 指定されたFFタイプのセルクラスを返す．
  ClibCellClassList
  find_ff_class(
    ClibSeqAttr attr ///< [in] FFタイプの属性
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ラッチセルの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 指定されたラッチタイプのセルクラスを返す．
  ClibCellClassList
  find_latch_class(
    ClibSeqAttr attr ///< [in] FFタイプの属性
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name パタングラフ関係の情報の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 総パタン数を返す．
  SizeType
  pg_pat_num() const;

  /// @brief パタンを返す．
  ClibPatGraph
  pg_pat(
    SizeType id ///< [in] パタン番号 ( 0 <= id < pg_pat_num() )
  ) const;

  /// @brief パタンの最大の入力数を得る．
  SizeType
  pg_max_input() const;

  /// @brief 総ノード数を返す．
  SizeType
  pg_node_num() const;

  /// @brief ノードの種類を返す．
  ClibPatType
  pg_node_type(
    SizeType id ///< [in] ノード番号 ( 0 <= id < pg_node_num() )
  ) const;

  /// @brief ノードが入力ノードの時に入力番号を返す．
  ///
  /// 入力ノードでない場合の返り値は不定
  SizeType
  pg_input_id(
    SizeType id ///< [in] ノード番号 ( 0 <= id < pg_node_num() )
  ) const;

  /// @brief 入力のノード番号を返す．
  /// @return input_id の入力に対応するノードのノード番号
  SizeType
  pg_input_node(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < pg_input_num() )
  ) const;

  /// @brief 総枝数を返す．
  SizeType
  pg_edge_num() const;

  /// @brief 枝のファンイン元のノード番号を返す．
  SizeType
  pg_edge_from(
    SizeType id ///< [in] 枝番号 ( 0 <= id < edge_num() )
  ) const;

  /// @brief 枝のファンアウト先のノード番号を返す．
  SizeType
  pg_edge_to(
    SizeType id ///< [in] 枝番号 ( 0 <= id < edge_num() )
  ) const;

  /// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
  SizeType
  pg_edge_pos(
    SizeType id ///< [in] 枝番号 ( 0 <= id < edge_num() )
  ) const;

  /// @brief 枝の反転属性を返す．
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  pg_edge_inv(
    SizeType id ///< [in] 枝番号 ( 0 <= id < edge_num() )
  ) const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ダンプ/リストア関係の関数
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    const std::string& filename ///< [in] ファイル名
  ) const;

  /// @brief バイナリダンプされた内容を読み込む．
  static
  ClibCellLibrary
  restore(
    std::istream& s ///< [in] 入力元のストリーム
  );

  /// @brief バイナリダンプされた内容を読み込む．
  static
  ClibCellLibrary
  restore(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief 内容を出力する(デバッグ用)．
  void
  display(
    std::ostream& s ///< [in] 出力先のストリーム
  ) const;

  /// @brief 内容を表す文字列を返す．
  std::string
  to_string() const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 等価比較
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較
  bool
  operator==(
    ClibCellLibrary right ///< [in] オペランド
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    ClibCellLibrary right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラス
  ClibLibraryPtr mImpl;

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBCELLLIBRARY_H
