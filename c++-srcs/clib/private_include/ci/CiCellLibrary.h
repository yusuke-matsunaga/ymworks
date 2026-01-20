#ifndef CICELLLIBRARY_H
#define CICELLLIBRARY_H

/// @file CiCellLibrary.h
/// @brief CiCellLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibArea.h"
#include "ym/ClibTime.h"
#include "ym/ClibCapacitance.h"
#include "ym/ClibResistance.h"
#include "ym/ShString.h"
#include "ym/logic.h"
#include "ci/CiPatMgr.h"
#include "ci/CiCellNameHash.h"
#include "ci/CiPin.h"
#include "ci/CiBus.h"
#include "ci/CiBundle.h"
#include "ci/conv_list.h"


BEGIN_NAMESPACE_YM_CLIB

class CiBusType;
class CiCell;
class CiCellClass;
class CiCellGroup;
class CiInternalPin;
class CiLutTemplate;
class CiTiming;
class Serializer;

//////////////////////////////////////////////////////////////////////
/// @class CiCellLibrary CiCellLibrary.h "CiCellLibrary.h"
/// @brief ClibCellLibrary の実装クラス
///
/// 本来はライブラリ -> セル -> ピン という風に階層構造を持つが，
/// 個々のオブジェクトの所有権的にはすべてライブラリに属していると考える．
/// 逆にすべての要素に対する参照は親のライブラリに対する参照とみなす．
//////////////////////////////////////////////////////////////////////
class CiCellLibrary
{
public:

  /// @brief コンストラクタ
  CiCellLibrary();

  /// @brief デストラクタ
  ~CiCellLibrary();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mislib 形式のファイルを読み込む．
  /// @return 生成したライブラリを返す．
  static
  CiCellLibrary*
  read_mislib(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief liberty 形式のファイルを読み込む．
  /// @return 生成したライブラリを返す．
  static
  CiCellLibrary*
  read_liberty(
    const std::string& filename ///< [in] ファイル名
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const
  {
    return mName;
  }

  /// @brief テクノロジの取得
  ///
  /// このクラスは常に ClibTechnology::cmos を返す．
  ClibTechnology
  technology() const
  {
    return mTechnology;
  }

  /// @brief 遅延モデルの取得
  /// 返り値は
  /// - ClibDelayModel::generic_cmos
  /// - ClibDelayModel::table_lookup
  /// - ClibDelayModel::piecewise_cmos
  /// - ClibDelayModel::cmos2
  /// - ClibDelayModel::dcm
  /// のいずれか
  ClibDelayModel
  delay_model() const
  {
    return mDelayModel;
  }

  /// @brief 区間インデックスのタイプの取得
  ClibVarType
  piece_type() const
  {
    return mPieceType;
  }

  /// @brief 区間のリストの取得
  std::vector<double>
  piece_define() const
  {
    return mPieceDefine;
  }

  /// @brief バス命名規則の取得
  std::string
  bus_naming_style() const
  {
    return mBusNamingStyle;
  }

  /// @brief 日付情報の取得
  std::string
  date() const
  {
    return mDate;
  }

  /// @brief リビジョン情報の取得
  std::string
  revision() const
  {
    return mRevision;
  }

  /// @brief コメント情報の取得
  std::string
  comment() const
  {
    return mComment;
  }

  /// @brief 時間単位の取得
  std::string
  time_unit() const
  {
    return mTimeUnit;
  }

  /// @brief 電圧単位の取得
  std::string
  voltage_unit() const
  {
    return mVoltageUnit;
  }

  /// @brief 電流単位の取得
  std::string
  current_unit() const
  {
    return mCurrentUnit;
  }

  /// @brief 抵抗単位の取得
  std::string
  pulling_resistance_unit() const
  {
    return mPullingResistanceUnit;
  }

  /// @brief 容量単位の取得
  double
  capacitive_load_unit() const
  {
    return mCapacitiveLoadUnit;
  }

  /// @brief 容量単位文字列の取得
  std::string
  capacitive_load_unit_str() const
  {
    return mCapacitiveLoadUnitStr;
  }

  /// @brief 電力単位の取得
  std::string
  leakage_power_unit() const
  {
    return mLeakagePowerUnit;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // セル情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief このライブラリの持つセル数の取得
  SizeType
  cell_num() const
  {
    return mCellList.size();
  }

  /// @brief 位置からセルの取得
  const CiCell*
  cell(
    SizeType id ///< [in] 位置番号 ( 0 <= id < cell_num() )
  ) const
  {
    return _cell(id);
  }

  /// @brief 名前からのセルの取得
  const CiCell*
  cell(
    const std::string& name
  ) const
  {
    return cell(ShString{name});
  }

  /// @brief 名前からのセルの取得
  const CiCell*
  cell(
    const ShString& name
  ) const
  {
    if ( mCellDict.count(name) == 0 ) {
      std::ostringstream buf;
      buf << name << ": not found";
      throw std::out_of_range{buf.str()};
    }
    return mCellDict.at(name);
  }

  /// @brief セルのリストの取得
  std::vector<const CiCell*>
  cell_list() const
  {
    return conv_list(mCellList);
  }

  /// @brief セルグループ数の取得
  SizeType
  cell_group_num() const
  {
    return mCellGroupList.size();
  }

  /// @brief セルグループの取得
  const CiCellGroup*
  cell_group(
    SizeType id ///< [in] 位置 ( 0 <= id < cell_group_num() )
  ) const
  {
    return _cell_group(id);
  }

  /// @brief セルグループのリストの取得
  std::vector<const CiCellGroup*>
  cell_group_list() const
  {
    return conv_list(mCellGroupList);
  }

  /// @brief NPN同値クラス数の取得
  SizeType
  npn_class_num() const
  {
    return mCellClassList.size();
  }

  /// @brief NPN同値クラスの取得
  const CiCellClass*
  npn_class(
    SizeType id ///< [in] 位置 ( 0 <= pos < id_class_num() )
  ) const
  {
    return _cell_class(id);
  }

  /// @brief NPN同値クラス番号のリストの取得
  std::vector<const CiCellClass*>
  npn_class_list() const
  {
    return conv_list(mCellClassList);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 論理セルグループの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0セルのグループを返す．
  const CiCellGroup*
  const0_func() const
  {
    return mLogicGroup[C0_BASE];
  }

  /// @brief 定数1セルのグループを返す．
  const CiCellGroup*
  const1_func() const
  {
    return mLogicGroup[C1_BASE];
  }

  /// @brief バッファセルのグループを返す．
  const CiCellGroup*
  buf_func() const
  {
    return mLogicGroup[BUF_BASE];
  }

  /// @brief インバータセルのグループを返す．
  const CiCellGroup*
  inv_func() const
  {
    return mLogicGroup[INV_BASE];
  }

  /// @brief ANDセルのグループを返す．
  const CiCellGroup*
  and_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const
  {
    if ( ni < 2 || 4 < ni ) {
      throw std::out_of_range{"'ni' is out of range"};
    }
    auto index = ni - 2;
    return mLogicGroup[AND_BASE + index];
  }

  /// @brief NANDセルのグループを返す．
  const CiCellGroup*
  nand_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const
  {
    if ( ni < 2 || 4 < ni ) {
      throw std::out_of_range{"'ni' is out of range"};
    }
    auto index = ni - 2;
    return mLogicGroup[NAND_BASE + index];
  }

  /// @brief ORセルのグループを返す．
  const CiCellGroup*
  or_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const
  {
    if ( ni < 2 || 4 < ni ) {
      throw std::out_of_range{"'ni' is out of range"};
    }
    auto index = ni - 2;
    return mLogicGroup[OR_BASE + index];
  }

  /// @brief NORセルのグループを返す．
  const CiCellGroup*
  nor_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const
  {
    if ( ni < 2 || 4 < ni ) {
      throw std::out_of_range{"'ni' is out of range"};
    }
    auto index = ni - 2;
    return mLogicGroup[NOR_BASE + index];
  }

  /// @brief XORセルのグループを返す．
  const CiCellGroup*
  xor_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const
  {
    if ( ni < 2 || 4 < ni ) {
      throw std::out_of_range{"'ni' is out of range"};
    }
    auto index = ni - 2;
    return mLogicGroup[XOR_BASE + index];
  }

  /// @brief XNORセルのグループを返す．
  const CiCellGroup*
  xnor_func(
    SizeType ni ///< [in] 入力数 ( 2 <= ni <= 4 )
  ) const
  {
    if ( ni < 2 || 4 < ni ) {
      throw std::out_of_range{"'ni' is out of range"};
    }
    auto index = ni - 2;
    return mLogicGroup[XNOR_BASE + index];
  }

  /// @brief MUX2セルのグループを返す．
  const CiCellGroup*
  mux2_func() const
  {
    return mLogicGroup[MUX2_BASE];
  }

  /// @brief MUX4セルのグループを返す．
  const CiCellGroup*
  mux4_func() const
  {
    return mLogicGroup[MUX4_BASE];
  }


public:
  //////////////////////////////////////////////////////////////////////
  // FFセルクラスの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 指定された順序セルの属性を持つFFクラスを返す．
  std::vector<const CiCellClass*>
  find_ff_class(
    ClibSeqAttr seq_attr ///< [in] 順序セルの属性
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ラッチセルクラスの情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 指定された順序セルの属性を持つラッチクラスを返す．
  std::vector<const CiCellClass*>
  find_latch_class(
    ClibSeqAttr seq_attr ///< [in] 順序セルの属性
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // パタングラフ関係の情報の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 総パタン数を返す．
  SizeType
  pg_pat_num() const
  {
    return mPatMgr.pat_num();
  }

  /// @brief パタンの最大の入力数を得る．
  SizeType
  pg_max_input() const
  {
    return mPatMgr.max_input();
  }

  /// @brief 総ノード数を返す．
  SizeType
  pg_node_num() const
  {
    return mPatMgr.node_num();
  }

  /// @brief ノードの種類を返す．
  ClibPatType
  pg_node_type(
    SizeType id ///< [in] ノード番号 ( 0 <= id < pg_node_num() )
  ) const
  {
    return mPatMgr.node_type(id);
  }

  /// @brief ノードが入力ノードの時に入力番号を返す．
  ///
  /// 入力ノードでない場合の返り値は不定
  SizeType
  pg_input_id(
    SizeType id ///< [in] ノード番号 ( 0 <= id < pg_node_num() )
  ) const
  {
    return mPatMgr.input_id(id);
  }

  /// @brief 入力のノード番号を返す．
  SizeType
  pg_input_node(
    SizeType input_id ///< [in] 入力番号 ( 0 <= input_id < pg_max_input() )
  ) const
  {
    return mPatMgr.input_node(input_id);
  }

  /// @brief 総枝数を返す．
  SizeType
  pg_edge_num() const
  {
    return mPatMgr.edge_num();
  }

  /// @brief 枝のファンイン元のノード番号を返す．
  SizeType
  pg_edge_from(
    SizeType id ///< [in] 枝番号 ( 0 <= id < pg_edge_num() )
  ) const
  {
    return mPatMgr.edge_from(id);
  }

  /// @brief 枝のファンアウト先のノード番号を返す．
  SizeType
  pg_edge_to(
    SizeType id ///< [in] 枝番号 ( 0 <= id < pg_edge_num() )
  ) const
  {
    return mPatMgr.edge_to(id);
  }

  /// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
  SizeType
  pg_edge_pos(
    SizeType id ///< [in] 枝番号 ( 0 <= id < pg_edge_num() )
  ) const
  {
    return mPatMgr.edge_pos(id);
  }

  /// @brief 枝の反転属性を返す．
  bool
  pg_edge_inv(
    SizeType id ///< [in] 枝番号 ( 0 <= id < pg_edge_num() )
  ) const
  {
    return mPatMgr.edge_inv(id);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // ダンプ/リストア関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief バイナリダンプされた内容を読み込む．
  static
  CiCellLibrary*
  restore(
    std::istream& s ///< [in] 入力ストリーム
  );

  /// @brief restore() の本体
  void
  _restore(
    Deserializer& s
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を設定する．
  void
  set_name(
    const std::string& name ///< [in] 名前
  )
  {
    mName = name;
  }

  /// @brief 'technology' を設定する．
  void
  set_technology(
    ClibTechnology technology ///< [in] テクノロジ
  )
  {
    mTechnology = technology;
  }

  /// @brief 遅延モデルを設定する．
  void
  set_delay_model(
    ClibDelayModel delay_model ///< [in] 遅延モデル
  )
  {
    mDelayModel = delay_model;
  }

  /// @brief 区間関係のパラメータを設定する．
  void
  set_piece_params(
    ClibVarType piece_type,               ///< [in] 区間のタイプ
    const std::vector<double>& piece_list ///< [in] 区間のリスト
  )
  {
    mPieceType = piece_type;
    mPieceDefine = piece_list;
  }

  /// @brief 'capacitive_load_unit' を設定する．
  void
  set_capacitive_load_unit(
    double unit,            ///< [in] 単位
    const std::string& ustr ///< [in] 単位を表す文字列
  )
  {
    mCapacitiveLoadUnit = unit;
    mCapacitiveLoadUnitStr = ustr;
  }

  /// @brief 属性を設定する(浮動小数点型)
  void
  set_attr(
    const std::string& attr_name, ///< [in] 属性名
    double value                  ///< [in] 値
  );

  /// @brief 属性を設定する(文字列型)．
  void
  set_attr(
    const std::string& attr_name, ///< [in] 属性名
    const std::string& value      ///< [in] 値
  );

  /// @brief 設定後の最終処理を行う．
  void
  wrap_up();

  /// @brief セルグループ/セルクラスの設定を行なう．
  void
  compile();


public:
  //////////////////////////////////////////////////////////////////////
  // CiXXX の生成関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1次元の LUT のテンプレートを作る．
  CiLutTemplate*
  add_lut_template1(
    ClibVarType var_type1,
    const std::vector<double>& index_list1
  );

  /// @brief 2次元の LUT のテンプレートを作る．
  CiLutTemplate*
  add_lut_template2(
    ClibVarType var_type1,
    const std::vector<double>& index_list1,
    ClibVarType var_type2,
    const std::vector<double>& index_list2
  );

  /// @brief 3次元の LUT のテンプレートを作る．
  CiLutTemplate*
  add_lut_template3(
    ClibVarType var_type1,
    const std::vector<double>& index_list1,
    ClibVarType var_type2,
    const std::vector<double>& index_list2,
    ClibVarType var_type3,
    const std::vector<double>& index_list3
  );

  /// @brief セルクラスを作る．
  CiCellClass*
  add_cell_class(
    ClibCellType cell_type,                  ///< [in] セルの種類
    ClibSeqAttr seq_attr,                    ///< [in] 順序セルの属性
    const std::vector<ClibIOMap>& idmap_list ///< [in] 同位体変換のリスト
  );

  /// @brief セルグループを作る．
  CiCellGroup*
  add_cell_group(
    const CiCellClass* rep_class, ///< [in] 親のセルクラス
    const ClibIOMap& iomap        ///< [in] 変換マップ
  );

  /// @brief 論理セルを追加する．
  CiCell*
  add_logic_cell(
    const ShString& name,     ///< [in] 名前
    ClibArea area             ///< [in] 面積
  );

  /// @brief FFセルを追加する．
  CiCell*
  add_ff_cell(
    const ShString& name,      ///< [in] 名前
    ClibArea area,             ///< [in] 面積
    const ShString& var1,      ///< [in] 内部変数1の名前
    const ShString& var2,      ///< [in] 内部変数2の名前
    const Expr& clock,         ///< [in] マスタークロックの論理式
    const Expr& clock2,        ///< [in] スレーブクロックの論理式
    const Expr& next_state,    ///< [in] 次状態の論理式
    const Expr& clear,         ///< [in] クリア条件の論理式
    const Expr& preset,        ///< [in] プリセット条件の論理式
    ClibSeqAttr seq_attr       ///< [in] 順序セルの属性
  );

  /// @brief ラッチセルを追加する．
  CiCell*
  add_latch_cell(
    const ShString& name,      ///< [in] 名前
    ClibArea area,             ///< [in] 面積
    const ShString& var1,      ///< [in] 内部変数1の名前
    const ShString& var2,      ///< [in] 内部変数2の名前
    const Expr& enable,        ///< [in] マスターイネーブルの論理式
    const Expr& enable2,       ///< [in] スレーブイネーブルの論理式
    const Expr& data_in,       ///< [in] データ入力の論理式
    const Expr& clear,         ///< [in] クリア条件の論理式
    const Expr& preset,        ///< [in] プリセット条件の論理式
    ClibSeqAttr seq_attr       ///< [in] 順序セルの属性
  );

  /// @brief FSMセルを追加する．
  CiCell*
  add_fsm_cell(
    const ShString& name,      ///< [in] 名前
    ClibArea area              ///< [in] 面積
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 参照回数に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす．
  void
  inc_ref() const;

  /// @brief 参照回数を減らす．
  void
  dec_ref() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CiXXX の要素にアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief LUT テンプレートを得る．
  CiLutTemplate*
  _lut_template(
    SizeType id ///< [in] ID番号
  ) const
  {
    return mLutTemplateList[id].get();
  }

  /// @brief セルクラスを得る．
  CiCellClass*
  _cell_class(
    SizeType id ///< [in] ID番号
  ) const
  {
    if ( id >= npn_class_num() ) {
      throw std::out_of_range{"'id' is out of range"};
    }
   return mCellClassList[id].get();
  }

  /// @brief セルグループを得る．
  CiCellGroup*
  _cell_group(
    SizeType id ///< [in] ID番号
  ) const
  {
    if ( id >= cell_group_num() ) {
      throw std::out_of_range{"'id' is out of range"};
    }
    return mCellGroupList[id].get();
  }

  /// @brief セルを得る．
  CiCell*
  _cell(
    SizeType id ///< [in] ID番号
  ) const
  {
    if ( id >= cell_num() ) {
      throw std::out_of_range{"'id' is out of range"};
    }
    return mCellList[id].get();
  }

  /// @brief パタンを得る．
  const CiPatGraph*
  _pat_graph(
    SizeType id ///< [in] ID番号
  ) const
  {
    return &mPatMgr.pat(id);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // dump() で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素をシリアライズする．
  void
  serialize(
    Serializer& s ///< [in] シリアライザ
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // restore で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 'capacitive_load_unit' を読み込む．
  void
  restore_capacitive_load_unit(
    Deserializer& s ///< [in] デシリアライザ
  );


public:
  //////////////////////////////////////////////////////////////////////
  // ピンハッシュ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名からピンを取り出す．
  ///
  /// 見つからない場合は std::out_of_range 例外を送出する．
  const CiPin*
  find_pin(
    const CiCell* cell, ///< [in] セル
    ShString name       ///< [in] ピン名
  ) const
  {
    return mPinDict.get(cell, name);
  }

  /// @brief バス名からバスを取り出す．
  ///
  /// 見つからない場合は std::out_of_range 例外を送出する．
  const CiBus*
  find_bus(
    const CiCell* cell, ///< [in] セル
    ShString name       ///< [in] バス名
  ) const
  {
    return mBusDict.get(cell, name);
  }

  /// @brief バンドル名からバンドル番号を取り出す．
  ///
  /// 見つからない場合は std::out_of_range 例外を送出する．
  const CiBundle*
  find_bundle(
    const CiCell* cell, ///< [in] セル番号
    ShString name       ///< [in] バンドル名
  ) const
  {
    return mBundleDict.get(cell, name);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief セルを登録する．
  /// @return セルを返す．
  CiCell*
  reg_cell(
    std::unique_ptr<CiCell>& ptr ///< [in] セルへのポインタ
  );

  /// @brief FF/ラッチの属性をエンコードする．
  static
  void
  encode_attr(
    bool master_slave, ///< [in] master/slave 型の時 true
    bool has_clear,    ///< [in] clear 端子を持つ時 true
    bool has_preset,   ///< [in] preset 端子を持つ時 true
    ClibCPV cpv1,      ///< [in] clear_preset_var1 の値
    ClibCPV cpv2,      ///< [in] clear_preset_var2 の値
    SizeType& idx,     ///< [out] メインインデックス
    SizeType& sub_idx  ///< [out] サブインデックス
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  mutable
  SizeType mRefCount{0};

  // 名前
  std::string mName;

  // テクノロジ
  ClibTechnology mTechnology{ClibTechnology::cmos};

  // バス命名規則
  std::string mBusNamingStyle;

  // 日付
  std::string mDate;

  // リビジョン
  std::string mRevision;

  // コメント
  std::string mComment;

  // 時間単位
  std::string mTimeUnit;

  // 電圧単位
  std::string mVoltageUnit;

  // 電流単位
  std::string mCurrentUnit;

  // 抵抗単位
  std::string mPullingResistanceUnit;

  // 容量単位
  double mCapacitiveLoadUnit{0.0};

  // 容量単位の文字列
  std::string mCapacitiveLoadUnitStr;

  // 電力単位
  std::string mLeakagePowerUnit;

  // 遅延モデル
  ClibDelayModel mDelayModel{ClibDelayModel::generic_cmos};

  // 区間のタイプ
  ClibVarType mPieceType;

  // 区間(の下限)のリスト
  std::vector<double> mPieceDefine;

  // バスタイプのリスト
  std::vector<std::unique_ptr<CiBusType>> mBusTypeList;

  // 遅延テンプレートの実体のリスト
  std::vector<std::unique_ptr<CiLutTemplate>> mLutTemplateList;

  // セルの所有権管理用のリスト
  std::vector<std::unique_ptr<CiCell>> mCellList;

  // 名前をキーにしたセルの辞書
  std::unordered_map<ShString, const CiCell*> mCellDict;

  // セルとピン名をキーにしたピンの辞書
  CiCellNameHash<CiPin> mPinDict;

  // セルとバス名をキーにしたバスの辞書
  CiCellNameHash<CiBus> mBusDict;

  // セルとバンドル名をキーにしたバンドルの辞書
  CiCellNameHash<CiBundle> mBundleDict;

  // セルグループの所有権管理用のリスト
  std::vector<std::unique_ptr<CiCellGroup>> mCellGroupList;

  // NPN同値クラスの所有権管理用のリスト
  std::vector<std::unique_ptr<CiCellClass>> mCellClassList;

  const SizeType C0_BASE = 0;
  const SizeType C1_BASE = 1;
  const SizeType BUF_BASE = 2;
  const SizeType INV_BASE = 3;
  const SizeType AND_BASE = 4;
  const SizeType NAND_BASE = 7;
  const SizeType OR_BASE = 10;
  const SizeType NOR_BASE = 13;
  const SizeType XOR_BASE = 16;
  const SizeType XNOR_BASE = 19;
  const SizeType MUX2_BASE = 22;
  const SizeType MUX4_BASE = 23;

  // 論理セルグループの情報
  const CiCellGroup* mLogicGroup[24];

  // FFセルクラスのリストを持つ辞書
  // キーは ClibSeqAttr::index()
  std::unordered_map<SizeType, std::vector<const CiCellClass*>> mFFClassDict;

  // ラッチセルクラスのリストを持つ辞書
  // キーは ClibSeqAttr::index()
  std::unordered_map<SizeType, std::vector<const CiCellClass*>> mLatchClassDict;

  // パタングラフを管理するオブジェクト
  CiPatMgr mPatMgr;

};

END_NAMESPACE_YM_CLIB

#endif // CICELLLIBRARY_H
