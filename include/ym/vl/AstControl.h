#ifndef YM_VL_ASTCONTROL_H
#define YM_VL_ASTCONTROL_H

/// @file ym/vl/AstControl.h
/// @brief AstControl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtControl;

//////////////////////////////////////////////////////////////////////
/// @class AstControl AstControl.h "ym/vl/AstControl.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////
class AstControl :
  public AstBaseWithPtr<const PtControl>
{
public:

  /// @brief コントロールの型を区別するための列挙型
  enum Type : std::uint8_t {
    Delay, ///< [in] delay control
    Event, ///< [in] event control
    Repeat ///< [in] repeat control
  };


public:

  /// @brief コンストラクタ
  explicit
  AstControl(
    const PtControl* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtControl>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstControl() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  Type
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // delay control の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の取得
  /// @return 遅延を表す式
  ///
  /// - type() != Delay の時 std::logic_error 例外を送出する．
  AstExpr
  delay() const;


public:
  //////////////////////////////////////////////////////////////////////
  // event control/repeat control の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  AstExprList
  event_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // repeat control の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 繰り返し数の取得
  /// @return 繰り返し数を表す式
  ///
  /// - type() != Repeat の時 std::logic_error 例外を送出する．
  AstExpr
  rep_expr() const;

public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const override;

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 比較用のユニークなキーを返す．
  PtrIntType
  key() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief json_obj() の下請け関数
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const override;

};

/// @brief AstControl::Type のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,     ///< [in] 出力ストリーム
  AstControl::Type val ///< [in] 値
)
{
  switch ( val ) {
  case AstControl::Delay:  s << "delay control"; break;
  case AstControl::Event:  s << "event control"; break;
  case AstControl::Repeat: s << "repeat control"; break;
  }
  return s;
}

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTCONTROL_H
