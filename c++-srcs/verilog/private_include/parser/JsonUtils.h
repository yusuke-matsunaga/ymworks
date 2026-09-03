#ifndef JSONUTILS_H
#define JSONUTILS_H

/// @file JsonUtils.h
/// @brief JsonUtils のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class JsonUtils JsonUtils.h "JsonUtils.h"
/// @brief AstBase::json_obj() で用いられるヘルパクラス
///
/// 実際のは関数の集合
//////////////////////////////////////////////////////////////////////
class JsonUtils
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief VpiNetType の値を jobj に追加する．
  ///
  /// None は追加しない．
  static
  void
  add(
    JsonValue& jobj,
    const char* label,
    VpiNetType net_type
  )
  {
    const char* type_str = nullptr;
    switch ( net_type ) {
    case VpiNetType::Wire:    type_str = "Wire"; break;
    case VpiNetType::Wand:    type_str = "Wand"; break;
    case VpiNetType::Wor:     type_str = "Wor"; break;
    case VpiNetType::Tri:     type_str = "Tri"; break;
    case VpiNetType::Tri0:    type_str = "Tri0"; break;
    case VpiNetType::Tri1:    type_str = "Tri1"; break;
    case VpiNetType::TriReg:  type_str = "TriReg"; break;
    case VpiNetType::TriAnd:  type_str = "TriAnd"; break;
    case VpiNetType::TriOr:   type_str = "TriOr"; break;
    case VpiNetType::Supply1: type_str = "Supply1"; break;
    case VpiNetType::Supply0: type_str = "Supply0"; break;
    case VpiNetType::None:    type_str = nullptr; break;
    }
    add(jobj, label, type_str);
  }

  /// @brief VpiVarType の値を jobj に追加する．
  ///
  /// None は追加しない．
  static
  void
  add(
    JsonValue& jobj,
    const char* label,
    VpiVarType var_type
  )
  {
    const char* type_str = nullptr;
    switch ( var_type ) {
    case VpiVarType::None:     type_str = nullptr; break;
    case VpiVarType::Integer:  type_str = "Integer"; break;
    case VpiVarType::Real:     type_str = "Real"; break;
    case VpiVarType::Time:     type_str = "Time"; break;
    case VpiVarType::Realtime: type_str = "Realtime"; break;
    }
    add(jobj, label, type_str);
  }

  /// @brief VpiPrimType の値を jobj に追加する．
  static
  void
  add(
    JsonValue& jobj,
    const char* label,
    VpiPrimType prim_type
  )
  {
    const char* type_str = nullptr;
    switch ( prim_type ) {
    case VpiPrimType::And:      type_str = "And"; break;
    case VpiPrimType::Nand:     type_str = "Nand"; break;
    case VpiPrimType::Nor:      type_str = "Nor"; break;
    case VpiPrimType::Or:       type_str = "Or"; break;
    case VpiPrimType::Xor:      type_str = "Xor"; break;
    case VpiPrimType::Xnor:     type_str = "Xnor"; break;
    case VpiPrimType::Buf:      type_str = "Buf"; break;
    case VpiPrimType::Not:      type_str = "Not"; break;
    case VpiPrimType::Bufif0:   type_str = "Bufif0"; break;
    case VpiPrimType::Bufif1:   type_str = "bufif1"; break;
    case VpiPrimType::Notif0:   type_str = "Notif0"; break;
    case VpiPrimType::Notif1:   type_str = "Notif1"; break;
    case VpiPrimType::Nmos:     type_str = "Nmos"; break;
    case VpiPrimType::Pmos:     type_str = "Pmos"; break;
    case VpiPrimType::Cmos:     type_str = "Cmos"; break;
    case VpiPrimType::Rnmos:    type_str = "Rnmos"; break;
    case VpiPrimType::Rpmos:    type_str = "Rpmos"; break;
    case VpiPrimType::Rcmos:    type_str = "Rcmos"; break;
    case VpiPrimType::Rtran:    type_str = "Rtran"; break;
    case VpiPrimType::Rtranif0: type_str = "Rtranif0"; break;
    case VpiPrimType::Rtranif1: type_str = "Rtranif1"; break;
    case VpiPrimType::Tran:     type_str = "Tran"; break;
    case VpiPrimType::Tranif0:  type_str = "Tranif0"; break;
    case VpiPrimType::Tranif1:  type_str = "Tranif1"; break;
    case VpiPrimType::Pullup:   type_str = "Pullup"; break;
    case VpiPrimType::Pulldown: type_str = "Pulldown"; break;
    case VpiPrimType::Seq:      type_str = "Seq"; break;
    case VpiPrimType::Comb:     type_str = "Comb"; break;
    case VpiPrimType::Cell:     type_str = "Cell"; break;
    }
    add(jobj, label, type_str);
  }

  /// @brief VpiStrength を jobj に追加する．
  ///
  /// NoStrength ならなにもしない．
  static
  void
  add(
    JsonValue& jobj,
    const char* label,
    VpiStrength strength
  )
  {
    const char* type_str = nullptr;
    switch ( strength ) {
    case VpiStrength::NoStrength:   type_str = nullptr; break;
    case VpiStrength::SupplyDrive:  type_str = "SupplyDrive"; break;
    case VpiStrength::StrongDrive:  type_str = "StrongDrive"; break;
    case VpiStrength::PullDrive:    type_str = "PullDriver"; break;
    case VpiStrength::WeakDrive:    type_str = "WeakDriver"; break;
    case VpiStrength::LargeCharge:  type_str = "LargeCharge"; break;
    case VpiStrength::MediumCharge: type_str = "MediumCharge"; break;
    case VpiStrength::SmallCharge:  type_str = "SmallCharge"; break;
    case VpiStrength::HiZ:          type_str = "HiZ"; break;
    }
    add(jobj, label, type_str);
  }

  /// @brief 文字列を jobj に追加する．
  ///
  /// 空文字列ならなにもしない．
  static
  void
  add(
    JsonValue& jobj,
    const char* label,
    const char* name
  )
  {
    if ( name != nullptr ) {
      jobj.add(label, JsonValue(name));
    }
  }

  /// @brief T の値を jobj に追加する．
  ///
  /// obj が無効な値ならなにもしない．
  template <typename T>
  static
  void
  add(
    JsonValue& jobj,
    const char* label,
    const T& obj
  )
  {
    if ( obj.is_valid() ) {
      jobj.add(label, obj.json_obj());
    }
  }

  /// @brief T の値を jobj に追加する(リスト型)．
  ///
  /// obj_list が空リストならなにもしない．
  template <typename T>
  static
  void
  add_list(
    JsonValue& jobj,
    const char* label,
    const T& obj_list
  )
  {
    if ( !obj_list.empty() ) {
      jobj.add(label, obj_list.json_obj());
    }
  }

};

END_NAMESPACE_YM_VERILOG

#endif // JSONUTILS_H
