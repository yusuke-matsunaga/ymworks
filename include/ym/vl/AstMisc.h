#ifndef YM_VL_ASTMISC_H
#define YM_VL_ASTMISC_H

/// @file ym/vl/AstMisc.h
/// @brief その他のの部品クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstControl AstMisc.h "ym/vl/AstMisc.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief delay control/event control/repeat control を表すクラス
//////////////////////////////////////////////////////////////////////
class AstControl :
  public AstBase
{
public:

  /// @brief コントロールの型を区別するための列挙型
  enum Type : std::uint8_t {
    Delay, ///< [in] delay control
    Event, ///< [in] event control
    Repeat ///< [in] repeat control
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  Type
  type() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // delay control の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の取得
  /// @return 遅延を表す式
  ///
  /// - type() != Delay の時 std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  delay() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // event control/repeat control の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベントリストの要素数の取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  virtual
  SizeType
  event_num() const = 0;

  /// @brief イベントの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  event(
    SizeType index ///< [in] インデックス ( 0 <= index < event_num() )
  ) const = 0;

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  virtual
  AstExprVec
  event_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // repeat control の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 繰り返し数の取得
  /// @return 繰り返し数を表す式
  ///
  /// - type() != Repeat の時 std::logic_error 例外を送出する．
  virtual
  const AstExpr*
  rep_expr() const = 0;

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


//////////////////////////////////////////////////////////////////////
/// @class AstConnection AstMisc.h "ym/vl/AstMisc.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief orered_connection/named_connection を表すクラス
//////////////////////////////////////////////////////////////////////
class AstConnection :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstConnection の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @retval 名前 named connection の場合
  /// @retval "" ordered connection の場合
  virtual
  const char*
  name() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const AstExpr*
  expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstStrength AstMisc.h "ym/vl/AstMisc.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief strength を表すクラス
///
/// drive strength を表す場合と charge strength を表す場合の２通りがある．
/// drive strength の場合の charge() は NoStrength を返す．
/// charge strength の場合の drive0() と driver1() は NoStrength を返す．
//////////////////////////////////////////////////////////////////////
class AstStrength :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstStrength の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief drive strength0 の取得
  /// @return 0 の強度
  virtual
  VpiStrength
  drive0() const = 0;

  /// @brief drive strength1 の取得
  /// @return 1 の強度
  virtual
  VpiStrength
  drive1() const = 0;

  /// @brief charge strength の取得
  /// @return 電荷の強度
  virtual
  VpiStrength
  charge() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstDelay AstMisc.h "ym/vl/AstMisc.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief delay を表すクラス
///
/// 最大で3つの値（式）を持つ．
/// 該当する値がない時は nullptr を返す．
//////////////////////////////////////////////////////////////////////
class AstDelay :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstDelay の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  virtual
  const AstExpr*
  value0() const = 0;

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  virtual
  const AstExpr*
  value1() const = 0;

  /// @brief 値の取得
  ///
  /// 該当する要素がなければ nullptr を返す．
  virtual
  const AstExpr*
  value2() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstNameBranch AstMisc.h "ym/vl/AstMisc.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 階層名を表すクラス
///
/// 通常はただの文字列だが，場合によっては '[' '1' ']' のようなインデックスを含む．
//////////////////////////////////////////////////////////////////////
class AstNameBranch
{
public:

  /// @brief デストラクタ
  virtual
  ~AstNameBranch() = default;


public:

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief インデックスの有無のチェック
  /// @retval true インデックスを持っている時
  /// @retval false インデックスを持っていない時
  virtual
  bool
  has_index() const = 0;

  /// @brief インデックスの取得
  /// @return インデックスの値
  ///
  /// has_index() == false の時は std::logic_error 例外を送出する．
  virtual
  int
  index() const = 0;

  /// @brief 次の要素の取得
  virtual
  const AstNameBranch*
  link() const = 0;

  /// @brief インデックスを含めた名前を返す．
  std::string
  decompile() const
  {
    std::ostringstream buf;
    buf << name();
    if ( has_index() ) {
      buf << "[" << index() << "]";
    }
    return buf.str();
  }

};


//////////////////////////////////////////////////////////////////////
/// @class AstAttrInst AstMisc.h "ym/vl/AstMisc.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief attribute_instance を表すクラス
///
/// 実体は AstAttrSpec のリスト
/// ただしファイル領域としては先頭と末尾の '(*' と '*)' を含んだ領域となる．
//////////////////////////////////////////////////////////////////////
class AstAttrInst :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstAttrInst の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のリストの取得
  virtual
  AstAttrSpecList
  attrspec_list() const = 0;

  /// @brief 次の要素の取得
  virtual
  const AstAttrInst*
  link() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstAttrSpec AstMisc.h "ym/vl/AstMisc.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief attr_spec を表すクラス
///
/// 属性名(name)と値(expr)を持つ．
/// 場合によっては値が nullptr のこともある．
//////////////////////////////////////////////////////////////////////
class AstAttrSpec :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstAttrSpec の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

  /// @brief 式の取得
  /// @return 式
  virtual
  const AstExpr*
  expr() const = 0;

  /// @brief 次の要素の取得
  virtual
  const AstAttrSpec*
  link() const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTMISC_H
