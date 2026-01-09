#ifndef YM_CLIBBUSTYPE_H
#define YM_CLIBBUSTYPE_H

/// @file ym/ClibBusType.h
/// @brief ClibBusType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibHandle.h"


BEGIN_NAMESPACE_YM_CLIB

class CiBusType;

//////////////////////////////////////////////////////////////////////
/// @class ClibBusType ClibBusType.h "ym/ClibBusType.h"
/// @brief バスタイプを表すクラス
//////////////////////////////////////////////////////////////////////
class ClibBusType
{
public:

  /// @brief base_type を表す型
  enum BaseType {
    // 今はこれだけ
    ArrayType
  };

  /// @brief data_type を表す型
  enum DataType {
    // 今はこれだけ
    BitType
  };


public:

  /// @brief 空のコンストラクタ
  ClibBusType() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibBusType(
    const CiBusType* impl ///< [in] 本体のオブジェクト
  );

  /// @brief デストラクタ
  ~ClibBusType();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const;

  /// @brief base_type の取得
  BaseType
  base_type() const;

  /// @brief data_type の取得
  DataType
  data_type() const;

  /// @brief ビット幅の取得
  SizeType
  bit_width() const;

  /// @brief 開始ビットの取得
  SizeType
  bit_from() const;

  /// @brief 終了ビットの取得
  SizeType
  bit_to() const;

  /// @brief 向きの取得
  ///
  /// true の時，降順を表す．
  bool
  downto() const;


public:
  //////////////////////////////////////////////////////////////////////
  // mImpl に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適正な値を持っている時 true を返す．
  bool
  is_valid() const
  {
    return mImpl != nullptr;
  }

  /// @brief 不正値の時 true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief 等価比較
  bool
  operator==(
    const ClibBusType& right
  ) const
  {
    return mImpl == right.mImpl;
  }

  /// @brief 非等価比較
  bool
  operator!=(
    const ClibBusType& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 適正な値を持っているかチェックする．
  void
  _check_valid() const
  {
    if ( !is_valid() ) {
      throw std::invalid_argument{"not having a valid data"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const CiBusType* mImpl{nullptr};

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBBUS_H
