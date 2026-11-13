#ifndef ELBSELECTER_H
#define ELBSELECTER_H

/// @file ElbSelecter.h
/// @brief ElbSelecter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/VlFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbSelecter ElbSelecter.h "ElbSelecter.h"
/// @brief ビット/部分選択の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class ElbSelecter
{
public:

  /// @brief 選択なしタイプのオブジェクトを作る．
  static
  ElbSelecter
  new_NONE(
    SizeType width ///< [in] ビット幅
  )
  {
    return ElbSelecter(VlSelectType::NONE, width, 0, nullptr);
  }

  /// @brief 固定選択のオブジェクトを作る．
  static
  ElbSelecter
  new_CONST(
    SizeType width, ///< [in] ビット幅
    SizeType offset ///< [in] オフセット
  )
  {
    return ElbSelecter(VlSelectType::CONST, width, offset, nullptr);
  }

  /// @brief 可変プラス選択のオブジェクトを作る．
  static
  ElbSelecter
  new_PLUS(
    SizeType width,     ///< [in] ビット幅
    const VlExpr* index ///< [in] インデックス式
  )
  {
    return ElbSelecter(VlSelectType::PLUS, width, 0, index);
  }

  /// @brief 可変マイナス選択のオブジェクトを作る．
  static
  ElbSelecter
  new_MINUS(
    SizeType width,     ///< [in] ビット幅
    const VlExpr* index ///< [in] インデックス式
  )
  {
    return ElbSelecter(VlSelectType::MINUS, width, 0, index);
  }

  /// @brief デストラクタ
  ~ElbSelecter() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief タイプを返す．
  VlSelectType
  type() const
  {
    return mType;
  }

  /// @brief 選択範囲のビット幅を返す．
  SizeType
  select_width() const
  {
    return mWidth;
  }

  /// @brief ビット位置のオフセットを返す．
  SizeType
  bit_offset() const
  {
    return mOffset;
  }

  /// @brief ビット位置のインデックス式を返す．
  const VlExpr*
  bit_index_expr() const
  {
    return mIndex;
  }

  /// @brief ビット/部分選択した結果を返す．
  ///
  /// もともとのタイプが NONE/CONST の時のみ有効
  ElbSelecter
  slice(
    SizeType width, ///< [in] 選択する幅
    SizeType offset ///< [in] 開始位置のオフセット
  ) const
  {
    switch ( mType ) {
    case VlSelectType::NONE:
      return ElbSelecter::new_CONST(width, offset);
    case VlSelectType::CONST:
      if ( width < mWidth ) {
	return ElbSelecter::new_CONST(width, mOffset + offset);
      }
      if ( width == mWidth && offset == 0 ) {
	return *this;
      }
      throw std::logic_error{"ElbSelecter::slice(): range overflow"};
    default:
      throw std::logic_error{"ElbSelecter::slice(): illegal select_type"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  ElbSelecter(
    VlSelectType type,
    SizeType width,
    SizeType offset,
    const VlExpr* index
  ) : mType{type},
      mWidth{width},
      mOffset{offset},
      mIndex{index}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タイプ
  VlSelectType mType{VlSelectType::NONE};

  // ビット幅
  SizeType mWidth{0};

  // オフセット
  SizeType mOffset{0};

  // インデックス式
  const VlExpr* mIndex{nullptr};

};

END_NAMESPACE_YM_VERILOG

#endif // ELBSELECTER_H
