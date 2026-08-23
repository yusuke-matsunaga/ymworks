#ifndef PTARRAY_H
#define PTARRAY_H

/// @file PtArray.h
/// @brief PtArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"
#include "alloc/Alloc.h"
#include "parser/PtFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtArray PtArray.h PtArray.h
/// @brief ポインタ配列のテンプレートクラス
///
/// このクラスの目的はメモリを Alloc を用いて確保するということ．
/// パース中のエラーで中断しても安全にメモリを開放できる．
//////////////////////////////////////////////////////////////////////
template <typename T, typename T2>
class PtArray
{
public:

  // 反復子の型
  using iterator = T*;

public:

  /// @brief 空のコンストラクタ
  ///
  /// 要素を持たない配列を表す．
  PtArray(
  ) : mNum{0},
      mArray{nullptr}
  {
  }

  /// @brief 要素数を指定したコンストラクタ
  ///
  /// 内容は nullptr のまま
  PtArray(
    Alloc& alloc, ///< [in] アロケータ
    SizeType size ///< [in] 要素数
  ) : mNum{size},
      mArray{alloc.get_array<T>(mNum)}
  {
    for ( SizeType i = 0; i < mNum; ++ i ) {
      mArray[i] = nullptr;
    }
  }

  /// @brief 内容を指定したコンストラクタ
  PtArray(
    Alloc& alloc,             ///< [in] メモリアロケータ
    const std::vector<T>& src ///< [in] ソース
  ) : mNum{src.size()},
      mArray{alloc.get_array<T>(mNum)}
  {
    for ( SizeType i = 0; i < mNum; ++ i ) {
      mArray[i] = src[i];
    }
  }

  /// @brief 内容を指定したコンストラクタ
  PtArray(
    Alloc& alloc,              ///< [in] メモリアロケータ
    const std::vector<T2>& src ///< [in] ソース
  ) : mNum{src.size()},
      mArray{alloc.get_array<T>(mNum)}
  {
    for ( SizeType i = 0; i < mNum; ++ i ) {
      mArray[i] = src[i];
    }
  }

  /// @brief 1つの要素からなるコンストラクタ
  PtArray(
    Alloc& alloc, ///< [in] メモリアロケータ
    T elem1       ///< [in] 要素1
  ) : mNum{1},
      mArray{alloc.get_array<T>(1)}
  {
    mArray[0] = elem1;
  }

  /// @brief 2つの要素からなるコンストラクタ
  PtArray(
    Alloc& alloc, ///< [in] メモリアロケータ
    T elem1,      ///< [in] 要素1
    T elem2       ///< [in] 要素2
  ) : mNum{2},
      mArray{alloc.get_array<T>(2)}
  {
    mArray[0] = elem1;
    mArray[1] = elem2;
  }

  /// @brief 3つの要素からなるコンストラクタ
  PtArray(
    Alloc& alloc, ///< [in] メモリアロケータ
    T elem1,      ///< [in] 要素1
    T elem2,      ///< [in] 要素2
    T elem3       ///< [in] 要素3
  ) : mNum{3},
      mArray{alloc.get_array<T>(3)}
  {
    mArray[0] = elem1;
    mArray[1] = elem2;
    mArray[2] = elem3;
  }

  /// @brief 4つの要素からなるコンストラクタ
  PtArray(
    Alloc& alloc, ///< [in] メモリアロケータ
    T elem1,      ///< [in] 要素1
    T elem2,      ///< [in] 要素2
    T elem3,      ///< [in] 要素3
    T elem4       ///< [in] 要素4
  ) : mNum{4},
      mArray{alloc.get_array<T>(4)}
  {
    mArray[0] = elem1;
    mArray[1] = elem2;
    mArray[2] = elem3;
    mArray[3] = elem4;
  }

  /// コピーコンストラクタもどき
  PtArray(
    Alloc& alloc,      ///< [in] メモリアロケータ
    const PtArray& src ///< [in] ソース
  ) : mNum{src.mNum},
      mArray{alloc.get_array<T>(mNum)}
  {
    for ( SizeType i = 0; i < mNum; ++ i ) {
      mArray[i] = src.mArray[i];
    }
  }

  /// @brief コピー代入演算子は禁止
  PtArray&
  operator=(
    const PtArray& src
  ) = delete;

  /// @brief ムーブコンストラクタ
  PtArray(
    PtArray&& src ///< [in] ムーブ元
  ) : mNum{src.mNum},
      mArray{src.mArray}
  {
    src.mNum = 0;
    src.mArray = nullptr;
  }

  /// @brief ムーブ代入演算子
  PtArray&
  operator=(
    PtArray&& src ///< [in] ムーブ元
  )
  {
    mNum = src.mNum;
    mArray = src.mArray;
    src.mNum = 0;
    src.mArray = nullptr;
    return *this;
  }

  /// @brief デストラクタ
  ~PtArray() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数の取得
  /// @return 要素数
  SizeType
  size() const
  {
    return mNum;
  }

  /// @brief 要素の取得
  ///
  /// - pos >= size() の時 std::out_of_range 例外を送出する．
  T
  operator[](
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < size() )
  ) const
  {
    if ( pos >= size() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mArray[pos];
  }

  /// @brief 要素の設定
  ///
  /// - pos >= size() の時 std::out_of_range 例外を送出する．
  void
  set(
    SizeType pos,
    T elem
  )
  {
    if ( pos >= size() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    mArray[pos] = elem;
  }

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const
  {
    return &mArray[0];
  }

  /// @brief 末尾の反復子を返す．
  iterator
  end() const
  {
    return &mArray[mNum];
  }

  /// @brief std:vector<T> に変換する．
  std::vector<T>
  to_vector() const
  {
    std::vector<T> vec(mNum);
    for ( SizeType i = 0; i < mNum; ++ i ) {
      vec[i] = mArray[i];
    }
    return vec;
  }

  /// @brief 配列本体の先頭アドレスを返す．
  T*
  _body() const
  {
    return mArray;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mNum;

  // ポインタ配列本体
  T* mArray;

};

using PtAttrInstArray    = PtArray<const AstAttrInst*,    PtAttrInst*>;
using PtAttrSpecArray    = PtArray<const AstAttrSpec*,    PtAttrSpec*>;
using PtCaseItemArray    = PtArray<const AstCaseItem*,    PtCaseItem*>;
using PtConnectionArray  = PtArray<const AstConnection*,  PtConnection*>;
using PtContAssignArray  = PtArray<const AstContAssign*,  PtContAssign*>;
using PtDeclHeadArray    = PtArray<const AstDeclHead*,    PtDeclHead*>;
using PtDeclItemArray    = PtArray<const AstDeclItem*,    PtDeclItem*>;
using PtDefParamArray    = PtArray<const AstDefParam*,    PtDefParam*>;
using PtExprArray        = PtArray<const AstExpr*,        PtExpr*>;
using PtGenCaseItemArray = PtArray<const AstGenCaseItem*, PtGenCaseItem*>;
using PtInstArray        = PtArray<const AstInst*,        PtInst*>;
using PtIOHeadArray      = PtArray<const AstIOHead*,      PtIOHead*>;
using PtIOItemArray      = PtArray<const AstIOItem*,      PtIOItem*>;
using PtItemArray        = PtArray<const AstItem*,        PtItem*>;
using PtNameBranchArray  = PtArray<const AstNameBranch*,  PtNameBranch*>;
using PtRangeArray       = PtArray<const AstRange*,       PtRange*>;
using PtPortArray        = PtArray<const AstPort*,        PtPort*>;
using PtStmtArray        = PtArray<const AstStmt*,        PtStmt*>;
using PtUdpEntryArray    = PtArray<const AstUdpEntry*,    PtUdpEntry*>;
using PtUdpValueArray    = PtArray<const AstUdpValue*,    PtUdpValue*>;

END_NAMESPACE_YM_VERILOG

#endif // PTARRAY_H
