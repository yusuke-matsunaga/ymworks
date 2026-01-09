#ifndef DESERIALIZER_H
#define DESERIALIZER_H

/// @file Deserializer.h
/// @brief Deserializer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/BinDec.h"
#include "ym/logic.h"
#include "ym/ClibArea.h"
#include "ym/ClibCapacitance.h"
#include "ym/ClibResistance.h"
#include "ym/ClibSeqAttr.h"
#include "ym/ClibTime.h"
#include "ci/CiBusType.h"
#include "ci/CiLutTemplate.h"
#include "ci/CiLut.h"
#include "ci/CiStLut.h"
#include "ci/CiPin.h"
#include "ci/CiBus.h"
#include "ci/CiBundle.h"
#include "ci/CiTiming.h"
#include "ci/CiCell.h"
#include "ci/CiCellGroup.h"
#include "ci/CiCellClass.h"


BEGIN_NAMESPACE_YM_CLIB

class CiCellLibrary;

//////////////////////////////////////////////////////////////////////
/// @class Deserializer Deserializer.h "Deserializer.h"
/// @brief restore 用の Deserializer
///
/// BinDec からデータを読み込み，各要素を復元して mXXXList と mXXXTable
/// に格納する．
/// 各要素の0番目は nullptr として予約されている．
/// mXXXList は unique_ptr となっていて実際の unique_ptr に所有権を奪われるまで
/// は仮の所有者となる．
/// mXXXTable は参照のみを持つ．
/// このように２種類のリストを持つ理由は mXXXList は途中で所有権を奪われて無効化
/// されることがあるため．
//////////////////////////////////////////////////////////////////////
class Deserializer
{
public:

  /// @brief コンストラクタ
  Deserializer(
    std::istream& s ///< [in] 入力ストリーム
  ) : mS{s}
  {
  }

  /// @brief デストラクタ
  ~Deserializer() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のデシリアライズを行う．
  void
  deserialize()
  {
    mBusTypeList.restore(*this);
    mLutTemplateList.restore(*this);
    mLutList.restore(*this);
    mStLutList.restore(*this);
    mPinList.restore(*this);
    mBusList.restore(*this);
    mBundleList.restore(*this);
    mTimingList.restore(*this);
    mCellList.restore(*this);
    mCellGroupList.restore(*this);
    mCellClassList.restore(*this);
  }

  /// @brief 文字列の読み込み
  void
  restore(
    std::string& dst
  )
  {
    in() >> dst;
  }

  /// @brief ShStringの読み込み
  void
  restore(
    ShString& dst
  )
  {
    std::string tmp;
    in() >> tmp;
    dst = tmp;
  }

  /// @brief std::uint8_t の読み込み
  void
  restore(
    std::uint8_t& dst
  )
  {
    dst = in().read_8();
  }

  /// @brief SizeType の読み込み
  void
  restore(
    SizeType& dst
  )
  {
    dst = in().read_64();
  }

  /// @brief doubleの読み込み
  void
  restore(
    double& dst
  )
  {
    in() >> dst;
  }

  /// @brief ClibTechnology の読み込み
  void
  restore(
    ClibTechnology& dst
  )
  {
    dst = static_cast<ClibTechnology>(in().read_8());
  }

  /// @brief ClibDelayModel の読み込み
  void
  restore(
    ClibDelayModel& dst
  )
  {
    dst = static_cast<ClibDelayModel>(in().read_8());
  }

  /// @brief ClibSeqAttr の読み込み
  void
  restore(
    ClibSeqAttr& dst
  )
  {
    dst = ClibSeqAttr{static_cast<SizeType>(in().read_8())};
  }

  /// @brief ClibTimingSense の読み込み
  void
  restore(
    ClibTimingSense& dst
  )
  {
    dst = static_cast<ClibTimingSense>(in().read_8());
  }

  /// @brief ClibTimingType の読み込み
  void
  restore(
    ClibTimingType& dst
  )
  {
    dst = static_cast<ClibTimingType>(in().read_8());
  }

  /// @brief ClibVarType の読み込み
  void
  restore(
    ClibVarType& dst
  )
  {
    dst = static_cast<ClibVarType>(in().read_8());
  }

  /// @brief ClibCPV の読み込み
  void
  restore(
    ClibCPV& dst
  )
  {
    dst = static_cast<ClibCPV>(in().read_8());
  }

  /// @brief Expr の読み込み
  void
  restore(
    Expr& dst
  )
  {
    in() >> dst;
  }

  /// @brief ClibArea の読み込み
  void
  restore(
    ClibArea& dst
  )
  {
    in() >> dst;
  }

  /// @brief ClibCapacitance の読み込み
  void
  restore(
    ClibCapacitance& dst
  )
  {
    in() >> dst;
  }

  /// @brief ClibResistance の読み込み
  void
  restore(
    ClibResistance& dst
  )
  {
    in() >> dst;
  }

  /// @brief ClibTime の読み込み
  void
  restore(
    ClibTime& dst
  )
  {
    in() >> dst;
  }

  /// @brief ClibIOMap の読み込み
  void
  restore(
    ClibIOMap& dst
  )
  {
    dst.restore(in());
  }

  /// @brief バスタイプの読み込み
  void
  restore(
    CiBusType*& dst
  )
  {
    dst = mBusTypeList.restore_ref(*this);
  }

  /// @brief バスタイプの読み込み
  void
  restore(
    std::unique_ptr<CiBusType>& dst
  )
  {
    std::swap(dst, mBusTypeList.restore_ptr(*this));
  }

  /// @brief ピンの読み込み
  void
  restore(
    CiPin*& dst
  )
  {
    dst = mPinList.restore_ref(*this);
  }

  /// @brief ピンの読み込み
  void
  restore(
    std::unique_ptr<CiPin>& dst
  )
  {
    std::swap(dst, mPinList.restore_ptr(*this));
  }

  /// @brief バスの読み込み
  void
  restore(
    CiBus*& dst
  )
  {
    dst = mBusList.restore_ref(*this);
  }

  /// @brief バスの読み込み
  void
  restore(
    std::unique_ptr<CiBus>& dst
  )
  {
    std::swap(dst, mBusList.restore_ptr(*this));
  }

  /// @brief バンドルの読み込み
  void
  restore(
    CiBundle*& dst
  )
  {
    dst = mBundleList.restore_ref(*this);
  }

  /// @brief バンドルの読み込み
  void
  restore(
    std::unique_ptr<CiBundle>& dst
  )
  {
    std::swap(dst, mBundleList.restore_ptr(*this));
  }

  /// @brief タイミングの読み込み
  void
  restore(
    CiTiming*& dst
  )
  {
    dst = mTimingList.restore_ref(*this);
  }

  /// @brief タイミングの読み込み
  void
  restore(
    std::unique_ptr<CiTiming>& dst
  )
  {
    std::swap(dst, mTimingList.restore_ptr(*this));
  }

  /// @brief LUTテンプレートの読み込み
  void
  restore(
    CiLutTemplate*& dst
  )
  {
    dst = mLutTemplateList.restore_ref(*this);
  }

  /// @brief LUTテンプレートの読み込み
  void
  restore(
    std::unique_ptr<CiLutTemplate>& dst
  )
  {
    std::swap(dst, mLutTemplateList.restore_ptr(*this));
  }

  /// @brief LUTの読み込み
  void
  restore(
    CiLut*& dst
  )
  {
    dst = mLutList.restore_ref(*this);
  }

  /// @brief LUTの読み込み
  void
  restore(
    std::unique_ptr<CiLut>& dst
  )
  {
    std::swap(dst, mLutList.restore_ptr(*this));
  }

  /// @brief StLutの読み込み
  void
  restore(
    CiStLut*& dst
  )
  {
    dst = mStLutList.restore_ref(*this);
  }

  /// @brief StLutの読み込み
  void
  restore(
    std::unique_ptr<CiStLut>& dst
  )
  {
    std::swap(dst, mStLutList.restore_ptr(*this));
  }

  /// @brief セルの読み込み
  void
  restore(
    CiCell*& dst
  )
  {
    dst = mCellList.restore_ref(*this);
  }

  /// @brief セルの読み込み
  void
  restore(
    std::unique_ptr<CiCell>& dst
  )
  {
    std::swap(dst, mCellList.restore_ptr(*this));
  }

  /// @brief セルグループの読み込み
  void
  restore(
    CiCellGroup*& dst
  )
  {
    dst = mCellGroupList.restore_ref(*this);
  }

  /// @brief セルグループの読み込み
  void
  restore(
    std::unique_ptr<CiCellGroup>& dst
  )
  {
    std::swap(dst, mCellGroupList.restore_ptr(*this));
  }

  /// @brief セルクラスの読み込み
  void
  restore(
    CiCellClass*& dst
  )
  {
    dst = mCellClassList.restore_ref(*this);
  }

  /// @brief セルクラスの読み込み
  void
  restore(
    std::unique_ptr<CiCellClass>& dst
  )
  {
    std::swap(dst, mCellClassList.restore_ptr(*this));
  }

  /// @brief const 型の要素の読み込み
  template<class T>
  void
  restore(
    const T*& dst
  )
  {
    T* tmp_dst;
    restore(tmp_dst);
    dst = tmp_dst;
  }

  /// @brief 要素のリストの読み込み
  template<class T>
  void
  restore(
    std::vector<T>& dst
  )
  {
    SizeType n = in().read_64();
    dst.resize(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      restore(dst[i]);
    }
  }

  /// @brief 辞書の読み込み
  template<class T>
  void
  restore(
    std::unordered_map<SizeType, T>& dst
  )
  {
    dst.clear();
    SizeType n = in().read_64();
    for ( SizeType i = 0; i < n; ++ i ) {
      SizeType key;
      restore(key);
      T val;
      restore(val);
      dst.emplace(key, std::move(val));
    }
  }


private:

  /// @brief バイナリデコーダの取得
  BinDec&
  in()
  {
    return mS;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  template<class T>
  struct ObjList
  {
    std::vector<std::unique_ptr<T>> mObjList;
    std::vector<T*> mObjRefList;

    // 内容を復元する．
    void
    restore(
      Deserializer& s
    )
    {
      SizeType n;
      s.restore(n);
      mObjList.resize(n + 1);
      mObjRefList.resize(n + 1);
      // 先頭に nullptr を追加しておく．
      mObjList[0] = {};
      mObjRefList[0] = nullptr;
      for ( SizeType i = 0; i < n; ++ i ) {
	auto ptr = T::restore(s);
	mObjRefList[i + 1] = ptr.get();
	mObjList[i + 1] = std::move(ptr);
      }
    }

    // 参照を復元する．
    T*
    restore_ref(
      Deserializer& s
    )
    {
      SizeType id;
      s.restore(id);
      return mObjRefList[id];
    }

    // ポインタを復元する．
    std::unique_ptr<T>&
    restore_ptr(
      Deserializer& s
    )
    {
      SizeType id;
      s.restore(id);
      return mObjList[id];
    }
  };

  // バイナリデコーダ
  BinDec mS;

  // バスタイプのリスト
  ObjList<CiBusType> mBusTypeList;

  // LUTテンプレートのリスト
  ObjList<CiLutTemplate> mLutTemplateList;

  // LUTのリスト
  ObjList<CiLut> mLutList;

  // StLutのリスト
  ObjList<CiStLut> mStLutList;

  // ピンのリスト
  ObjList<CiPin> mPinList;

  // バスのリスト
  ObjList<CiBus> mBusList;

  // バンドルのリスト
  ObjList<CiBundle> mBundleList;

  // タイミングのリスト
  ObjList<CiTiming> mTimingList;

  // セルのリスト
  ObjList<CiCell> mCellList;

  // セルグループのリスト
  ObjList<CiCellGroup> mCellGroupList;

  // セルクラスのリスト
  ObjList<CiCellClass> mCellClassList;

};

END_NAMESPACE_YM_CLIB

#endif // DESERIALIZER_H
