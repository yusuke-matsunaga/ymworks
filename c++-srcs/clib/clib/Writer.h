#ifndef WRITER_H
#define WRITER_H

/// @file Writer.h
/// @brief Writer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellLibrary.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class Writer Writer.h "Writer.h"
/// @brief ClibCellLibrary の内容を出力するためのクラス
///
/// 意味的にはただの関数だが，セルクラスやセルグループと番号の対応関係を表す
/// 辞書などを持つ．
/// これらのオブジェクトはコンストラクタ中で初期化され以後は変更されない．
//////////////////////////////////////////////////////////////////////
class Writer
{
public:

  /// @brief コンストラクタ
  Writer(
    const ClibCellLibrary& library ///< [in] セルライブラリ
  );

  /// @brief デストラクタ
  ~Writer() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief セルライブラリの内容を出力する．
  void
  run(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルクラスの情報を出力する．
  void
  display_class(
    std::ostream& s,
    const ClibCellClass& cclass
  ) const;

  /// @brief セルグループの情報を出力する．
  void
  display_group(
    std::ostream& s,
    const ClibCellGroup& group
  ) const;

  /// @brief セルの情報を出力する．
  void
  display_cell(
    std::ostream& s,
    const ClibCell& cell
  ) const;

  /// @brief タイミング情報を出力する．
  void
  display_timing(
    std::ostream& s,
    const ClibCell& cell,
    SizeType ipos,
    SizeType opos,
    ClibTimingSense sense
  ) const;

  /// @brief LUT の情報を出力する．
  void
  display_lut(
    std::ostream& s,
    const std::string& label,
    const ClibLut& lut
  ) const;

  /// @brief パタングラフの情報を出力する．
  void
  display_patgraph(
    std::ostream& s
  ) const;

  /// @brief DFF のセルクラスに名前をつける．
  void
  reg_dff_class();

  /// @brief ラッチのセルクラスに名前をつける．
  void
  reg_latch_class();

  /// @brief セルグループ名を登録する．
  void
  reg_group_name(
    const std::string& name,
    const ClibCellGroup& cgroup
  );

  /// @brief セルクラス名を登録する．
  void
  reg_class_name(
    const std::string& name,
    const ClibCellGroup& cgroup
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  ClibCellLibrary mLibrary;

  // セルクラス番号の辞書
  // キーは ClibCellClass::key()
  std::unordered_map<SizeType, SizeType> mClassDict;

  // セルグループ番号の辞書
  // キーは ClibCellGroup::key()
  std::unordered_map<SizeType, SizeType> mGroupDict;

  // セル番号の辞書
  // キーは ClibCell::key()
  std::unordered_map<SizeType, SizeType> mCellDict;

  // 特定のクラスの名前を保持する辞書
  std::unordered_map<SizeType, std::string> mClassNameDict;

  // 特定のグループの名前を保持する辞書
  std::unordered_map<SizeType, std::string> mGroupNameDict;

};

END_NAMESPACE_YM_CLIB

#endif // WRITER_H
