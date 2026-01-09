#ifndef SERIALIZER_H
#define SERIALIZER_H

/// @file Serializer.h
/// @brief Serializer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/BinEnc.h"
#include "ym/logic.h"
#include "ym/ClibArea.h"
#include "ym/ClibCapacitance.h"
#include "ym/ClibResistance.h"
#include "ym/ClibSeqAttr.h"
#include "ym/ClibTime.h"
#include "ym/ClibIOMap.h"
#include "ci/CiCell.h"


BEGIN_NAMESPACE_YM_CLIB

class CiBusType;
class CiPin;
class CiBus;
class CiBundle;
class CiTiming;
class CiLutTemplate;
class CiLut;
class CiStLut;
class CiCell;
class CiCellGroup;
class CiCellClass;

//////////////////////////////////////////////////////////////////////
/// @class Serializer Serializer.h "Serializer.h"
/// @brief dump 用の serializer
//////////////////////////////////////////////////////////////////////
class Serializer
{
public:

  /// @brief コンストラクタ
  Serializer(
    std::ostream& s ///< [in] 出力ストリーム
  ) : mS{s}
  {
  }

  /// @brief デストラクタ
  ~Serializer() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief バスタイプを追加する．
  void
  reg_obj(
    const CiBusType* obj
  )
  {
    mBusTypeList.put(obj);
  }

  /// @brief ピンを追加する．
  void
  reg_obj(
    const CiPin* obj
  )
  {
    mPinList.put(obj);
  }

  /// @brief バスを追加する．
  void
  reg_obj(
    const CiBus* obj
  )
  {
    mBusList.put(obj);
  }

  /// @brief バンドルを追加する．
  void
  reg_obj(
    const CiBundle* obj
  )
  {
    mBundleList.put(obj);
  }

  /// @brief タイミングを追加する．
  void
  reg_obj(
    const CiTiming* obj
  )
  {
    mTimingList.put(obj);
  }

  /// @brief LUTテンプレートを追加する．
  void
  reg_obj(
    const CiLutTemplate* obj
  )
  {
    mLutTemplateList.put(obj);
  }

  /// @brief LUTを追加する．
  void
  reg_obj(
    const CiLut* obj
  )
  {
    mLutList.put(obj);
  }

  /// @brief LUT2Dを追加する．
  void
  reg_obj(
    const CiStLut* obj
  )
  {
    mStLutList.put(obj);
  }

  /// @brief セルを追加する．
  void
  reg_obj(
    const CiCell* obj
  )
  {
    mCellList.put(obj);
    if ( obj != nullptr ) {
      auto id = mCellList.get_id(obj);
    }
  }

  /// @brief セルグループを追加する．
  void
  reg_obj(
    const CiCellGroup* obj
  )
  {
    mCellGroupList.put(obj);
  }

  /// @brief セルクラスを追加する．
  void
  reg_obj(
    const CiCellClass* obj
  )
  {
    mCellClassList.put(obj);
  }

  /// @brief 登録された要素の内容をダンプする．
  void
  dump_obj()
  {
    mBusTypeList.dump(*this);
    mLutTemplateList.dump(*this);
    mLutList.dump(*this);
    mStLutList.dump(*this);
    mPinList.dump(*this);
    mBusList.dump(*this);
    mBundleList.dump(*this);
    mTimingList.dump(*this);
    mCellList.dump(*this);
    mCellGroupList.dump(*this);
    mCellClassList.dump(*this);
  }

  /// @brief 文字列をダンプする．
  void
  dump(
    const std::string& val
  )
  {
    out() << val;
  }

  /// @brief ShStringをダンプする．
  void
  dump(
    const ShString& val
  )
  {
    out() << val;
  }

  /// @brief std::uint8_tをダンプする．
  void
  dump(
    std::uint8_t val
  )
  {
    out().write_8(val);
  }

  /// @brief SizeType をダンプする．
  void
  dump(
    SizeType val
  )
  {
    out().write_64(val);
  }

  /// @brief double をダンプする．
  void
  dump(
    double val
  )
  {
    out() << val;
  }

  /// @brief ClibTechnology をダンプする．
  void
  dump(
    ClibTechnology val
  )
  {
    out().write_8(static_cast<std::uint8_t>(val));
  }

  /// @brief ClibDelayModel をダンプする．
  void
  dump(
    ClibDelayModel val
  )
  {
    out().write_8(static_cast<std::uint8_t>(val));
  }

  /// @brief ClibSeqAttr をダンプする．
  void
  dump(
    ClibSeqAttr val
  )
  {
    out().write_8(static_cast<std::uint8_t>(val.index()));
  }

  /// @brief ClibTimingSense をダンプする．
  void
  dump(
    ClibTimingSense val
  )
  {
    out().write_8(static_cast<std::uint8_t>(val));
  }

  /// @brief ClibTimingType をダンプする．
  void
  dump(
    ClibTimingType val
  )
  {
    out().write_8(static_cast<std::uint8_t>(val));
  }

  /// @brief ClibVarType をダンプする．
  void
  dump(
    ClibVarType val
  )
  {
    out().write_8(static_cast<std::uint8_t>(val));
  }

  /// @brief ClibCPV をダンプする．
  void
  dump(
    ClibCPV val
  )
  {
    out().write_8(static_cast<std::uint8_t>(val));
  }

  /// @brief Expr をダンプする．
  void
  dump(
    const Expr& val
  )
  {
    out() << val;
  }

  /// @brief ClibArea をダンプする．
  void
  dump(
    const ClibArea& val
  )
  {
    out() << val;
  }

  /// @brief ClibCapacitance をダンプする．
  void
  dump(
    const ClibCapacitance& val
  )
  {
    out() << val;
  }

  /// @brief ClibResistance をダンプする．
  void
  dump(
    const ClibResistance& val
  )
  {
    out() << val;
  }

  /// @brief ClibTime をダンプする．
  void
  dump(
    const ClibTime& val
  )
  {
    out() << val;
  }

  /// @brief ClibIOMap をダンプする．
  void
  dump(
    const ClibIOMap& val
  )
  {
    val.dump(out());
  }

  /// @brief バスタイプ番号を出力する．
  void
  dump(
    const CiBusType* obj
  )
  {
    SizeType id = mBusTypeList.get_id(obj);
    dump(id);
  }

  /// @brief ピン番号を出力する．
  void
  dump(
    const CiPin* obj
  )
  {
    SizeType id = mPinList.get_id(obj);
    dump(id);
  }

  /// @brief バス番号を出力する．
  void
  dump(
    const CiBus* obj
  )
  {
    SizeType id = mBusList.get_id(obj);
    dump(id);
  }

  /// @brief バンドル番号を出力する．
  void
  dump(
    const CiBundle* obj
  )
  {
    SizeType id = mBundleList.get_id(obj);
    dump(id);
  }

  /// @brief タイミング番号を出力する．
  void
  dump(
    const CiTiming* obj
  )
  {
    SizeType id = mTimingList.get_id(obj);
    dump(id);
  }

  /// @brief LUTテンプレート番号を出力する．
  void
  dump(
    const CiLutTemplate* obj
  )
  {
    SizeType id = mLutTemplateList.get_id(obj);
    dump(id);
  }

  /// @brief LUT番号を出力する．
  void
  dump(
    const CiLut* obj
  )
  {
    SizeType id = mLutList.get_id(obj);
    dump(id);
  }

  /// @brief LUT番号を出力する．
  void
  dump(
    const CiStLut* obj
  )
  {
    SizeType id = mStLutList.get_id(obj);
    dump(id);
  }

  /// @brief セル番号を出力する．
  void
  dump(
    const CiCell* obj
  )
  {
    SizeType id = mCellList.get_id(obj);
    dump(id);
  }

  /// @brief セルグループ番号を出力する．
  void
  dump(
    const CiCellGroup* obj
  )
  {
    SizeType id = mCellGroupList.get_id(obj);
    dump(id);
  }

  /// @brief セルクラス番号を出力する．
  void
  dump(
    const CiCellClass* obj
  )
  {
    SizeType id = mCellClassList.get_id(obj);
    dump(id);
  }

  template<class T>
  void
  dump(
    const std::vector<T>& obj_list
  )
  {
    SizeType n = obj_list.size();
    dump(n);
    for ( auto obj: obj_list ) {
      dump(obj);
    }
  }

  template<class T>
  void
  dump(
    const std::vector<const T*>& obj_list
  )
  {
    SizeType n = obj_list.size();
    dump(n);
    for ( auto obj: obj_list ) {
      dump(obj);
    }
  }

  template<class T>
  void
  dump(
    const std::vector<std::unique_ptr<T>>& obj_list
  )
  {
    SizeType n = obj_list.size();
    dump(n);
    for ( auto& obj: obj_list ) {
      dump(obj.get());
    }
  }

  template<class T>
  void
  dump(
    const std::unordered_map<SizeType, T>& obj_dict
  )
  {
    SizeType n = obj_dict.size();
    dump(n);
    for ( auto& p: obj_dict ) {
      auto key = p.first;
      auto& val = p.second;
      dump(key);
      dump(val);
    }
  }


private:

  /// @brief バイナリエンコーダを返す．
  BinEnc&
  out()
  {
    return mS;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリストと辞書
  template<class T>
  class ListMap
  {
  public:

    /// @brief コンストラクタ
    ListMap()
    {
      // nullptr を 0 番目の要素とする．
      // ただし，実際には mList には入れない
      mIdMap.emplace(nullptr, 0);
    }

    /// @brief デストラクタ
    ~ListMap() = default;

    /// @brief 要素を追加する．
    void
    put(
      const T* obj
    )
    {
      if ( mIdMap.count(obj) == 0 ) {
	// nullptr の分を足しておく．
	auto id = mList.size() + 1;
	mList.push_back(obj);
	mIdMap.emplace(obj, id);
      }
    }

    /// @brief ID番号を得る．
    SizeType
    get_id(
      const T* obj
    ) const
    {
      if ( mIdMap.count(obj) == 0 ) {
	abort();
      }
      return mIdMap.at(obj);
    }

    /// @brief 要素のリストを得る．
    const std::vector<const T*>&
    obj_list() const
    {
      return mList;
    }

    /// @brief 内容をバイナリダンプする．
    void
    dump(
      Serializer& s
    ) const
    {
      s.dump(mList.size());
      for ( auto obj: mList ) {
	obj->dump(s);
      }
    }


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 要素のリスト
    std::vector<const T*> mList;

    // 要素番号の辞書
    std::unordered_map<const T*, SizeType> mIdMap;

  };

  // バイナリエンコーダ
  BinEnc mS;

  // バスタイプのリスト
  ListMap<CiBusType> mBusTypeList;

  // テンプレートのリスト
  ListMap<CiLutTemplate> mLutTemplateList;

  // LUTのリスト
  ListMap<CiLut> mLutList;

  // StLUTのリスト
  ListMap<CiStLut> mStLutList;

  // ピンのリスト
  ListMap<CiPin> mPinList;

  // バスのリスト
  ListMap<CiBus> mBusList;

  // バンドルのリスト
  ListMap<CiBundle> mBundleList;

  // タイミングのリスト
  ListMap<CiTiming> mTimingList;

  // セルクラスのリスト
  ListMap<CiCellClass> mCellClassList;

  // セルグループのリスト
  ListMap<CiCellGroup> mCellGroupList;

  // セルのリスト
  ListMap<CiCell> mCellList;

};

END_NAMESPACE_YM_CLIB

#endif // SERIALIZER_H
