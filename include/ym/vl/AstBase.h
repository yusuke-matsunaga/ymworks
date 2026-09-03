#ifndef YM_VL_ASTBASE_H
#define YM_VL_ASTBASE_H

/// @file ym/vl/AstBase.h
/// @brief Ast の基底クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"
#include "ym/JsonValue.h"
#include "ym/vl/AstFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 全ての Ast クラスの基底クラス
///
/// と言っても共通するインターフェイスは FileRegion を持つことのみ
//////////////////////////////////////////////////////////////////////
class AstBase
{
public:

  /// @brief デストラクタ
  virtual
  ~AstBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  virtual
  bool
  is_valid() const = 0;

  /// @brief 適切な値を持っていない時 true を返す．
  bool
  is_invalid() const
  {
    return !is_valid();
  }

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

  /// @brief 内容を表す JSON オブジェクトを返す．
  JsonValue
  json_obj() const;

  /// @brief 比較用のユニークなキーを返す．
  virtual
  PtrIntType
  key() const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 有効な値を持っているか調べる．
  void
  _check_ptr() const
  {
    if ( is_invalid() ) {
      throw std::logic_error{"is_invalid()"};
    }
  }

  /// @brief json_obj() の下請け関数
  virtual
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

};


//////////////////////////////////////////////////////////////////////
/// @class AstBaseWithPtr AstBase.h "ym/vl/AstBase.h"
/// @brief 実装クラスのポインタを持った AstBase
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstBaseWithPtr :
  public AstBase
{
public:

  /// @brief コンストラクタ
  AstBaseWithPtr(
    T* ptr = nullptr ///< [in] 実体のポインタ
  ) : mPtr{ptr}
  {
  }

  /// @brief デストラクタ
  ~AstBaseWithPtr() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較用のユニークなキーを返す．
  PtrIntType
  _key() const
  {
    return reinterpret_cast<PtrIntType>(mPtr);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  bool
  operator==(
    const AstBaseWithPtr<T>& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AstBaseWithPtr<T>& right
  ) const
  {
    return !operator==(right);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体のポインタ
  T* mPtr{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class AstNamedBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 名前を持つ AstBase
//////////////////////////////////////////////////////////////////////
class AstNamedBase :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief json_obj() の下請け関数
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class AstNamedBaseWithPtr AstBase.h "ym/vl/AstBase.h"
/// @brief 実装クラスのポインタを持った AstNamedBase
///
/// ナチュラルに考えたら AstNamedBase と AstBaseWithPtr の他重継承
/// だけど，いろいろめんどくさい．
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstNamedBaseWithPtr :
  public AstNamedBase
{
public:

  /// @brief コンストラクタ
  AstNamedBaseWithPtr(
    T* ptr = nullptr ///< [in] 実体のポインタ
  ) : mPtr{ptr}
  {
  }

  /// @brief デストラクタ
  ~AstNamedBaseWithPtr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較用のユニークなキーを返す．
  PtrIntType
  _key() const
  {
    return reinterpret_cast<PtrIntType>(mPtr);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  bool
  operator==(
    const AstNamedBaseWithPtr<T>& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AstNamedBaseWithPtr<T>& right
  ) const
  {
    return !operator==(right);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体のポインタ
  T* mPtr{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class AstHierNamedBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 階層付き名前を持つ AstBase
//////////////////////////////////////////////////////////////////////
class AstHierNamedBase :
  public AstNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  virtual
  AstNameBranchList
  namebranch_list() const = 0;

  /// @brief 階層名を展開する．
  std::string
  decompile_name() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief json_obj() の下請け関数
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class AstHierNamedBaseWithPtr AstBase.h "ym/vl/AstBase.h"
/// @brief 実装クラスのポインタを持った AstHierNamedBase
///
/// ナチュラルに考えたら AstHierNamedBase と AstBaseWithPtr の他重継承
/// だけど，いろいろめんどくさい．
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstHierNamedBaseWithPtr :
  public AstHierNamedBase
{
public:

  /// @brief コンストラクタ
  AstHierNamedBaseWithPtr(
    T* ptr = nullptr ///< [in] 実体のポインタ
  ) : mPtr{ptr}
  {
  }

  /// @brief デストラクタ
  ~AstHierNamedBaseWithPtr() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較用のユニークなキーを返す．
  PtrIntType
  _key() const
  {
    return reinterpret_cast<PtrIntType>(mPtr);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較演算子
  bool
  operator==(
    const AstHierNamedBaseWithPtr<T>& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const AstHierNamedBaseWithPtr<T>& right
  ) const
  {
    return !operator==(right);
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実体のポインタ
  T* mPtr{nullptr};

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTBASE_H
