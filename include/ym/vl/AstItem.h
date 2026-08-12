#ifndef YM_VL_ASTITEM_H
#define YM_VL_ASTITEM_H

/// @file ym/vl/AstItem.h
/// @brief AstItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstItem AstItem.h "ym/vl/AstItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief ブロック内要素の共通の親クラス
///
/// 個々の型ごとに専用の関数を持つ．
/// それ以外の型では std::logic_error 例外を送出する．
//////////////////////////////////////////////////////////////////////
class AstItem :
  public AstNamedBase
{
public:

  /// @brief AstItem の派生クラスの型を表す列挙型
  enum Type : std::uint8_t {
    DefParam,
    ContAssign,
    Initial,
    Always,
    Task,
    Func,
    GateInst,
    MuInst,
    SpecItem,
    SpecPath,
    Generate,
    GenBlock,
    GenIf,
    GenCase,
    GenFor
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  virtual
  Type
  type() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // GateInst 型の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  virtual
  VpiPrimType
  prim_type() const = 0;

  /// @brief strength の取得
  /// @return 信号強度
  virtual
  const AstStrength*
  strength() const = 0;

  /// @brief delay の取得
  /// @return 遅延
  virtual
  const AstDelay*
  delay() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // DefParam 型の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ割り当て数の取得
  virtual
  SizeType
  paramassign_num() const = 0;

  /// @brief パラメータ割り当ての取得
  ///
  /// - pos >= paramassign_num() の時 std::logic_error 例外を送出する．
  virtual
  const AstConnection*
  paramassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < paramassign_num() )
  ) const = 0;

  /// @brief パラメータ割り当てのリストの取得
  std::vector<const AstConnection*>
  paramassign_list() const
  {
    auto n = paramassign_num();
    std::vector<const AstConnection*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(paramassign(i));
    }
    return vec;
  }

  /// @brief defparam の要素数の取得
  virtual
  SizeType
  defparam_num() const = 0;

  /// @brief defparam の取得
  ///
  /// - pos >= defparam_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstDefParam*
  defparam(
    SizeType pos ///< [in] 位置 ( 0 <= pos < defparam_num() )
  ) const = 0;

  /// @brief defparam リストの取得
  std::vector<const AstDefParam*>
  defparam_list() const
  {
    auto n = defparam_num();
    std::vector<const AstDefParam*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(defparam(i));
    }
    return vec;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // ContAssign の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief continuous assign の要素数の取得
  virtual
  SizeType
  contassign_num() const = 0;

  /// @brief continuous assign の取得
  ///
  /// - pos >= contassign_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstContAssign*
  contassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < contassign_num() )
  ) const = 0;

  /// @brief continuous assign リストの取得
  std::vector<const AstContAssign*>
  contassign_list() const
  {
    auto n = contassign_num();
    std::vector<const AstContAssign*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(contassign(i));
    }
    return vec;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // MuInst の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module/UDP/gate instance の要素数の取得
  virtual
  SizeType
  inst_num() const = 0;

  /// @brief module/UDP/gate instance の取得
  ///
  /// - pos >= inst_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstInst*
  inst(
    SizeType pos ///< [in] 位置 ( 0 <= pos < inst_num() )
  ) const = 0;

  /// @brief module/UDP/gate instance リストの取得
  std::vector<const AstInst*>
  inst_list() const
  {
    auto n = inst_num();
    std::vector<const AstInst*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(inst(i));
    }
    return vec;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // Task/Func の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  virtual
  bool
  automatic() const = 0;

  /// @brief IO宣言の要素数の取得
  virtual
  SizeType
  ioitem_num() const = 0;

  /// @brief IO宣言ヘッダリストの要素数の取得
  virtual
  SizeType
  iohead_num() const = 0;

  /// @brief IO宣言ヘッダの取得
  ///
  /// - pos >= iohead_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstIOHead*
  iohead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const = 0;

  /// @brief IO宣言ヘッダリストの取得
  std::vector<const AstIOHead*>
  iohead_list() const
  {
    auto n = iohead_num();
    std::vector<const AstIOHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(iohead(i));
    }
    return vec;
  }

  /// @brief 宣言ヘッダの要素数の取得
  virtual
  SizeType
  declhead_num() const = 0;

  /// @brief 宣言ヘッダの取得
  ///
  /// - pos >= declhead_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const = 0;

  /// @brief 宣言ヘッダリストの取得
  std::vector<const AstDeclHead*>
  declhead_list() const
  {
    auto n = declhead_num();
    std::vector<const AstDeclHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(declhead(i));
    }
    return vec;
  }

  /// @brief item リストの要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief item の取得
  ///
  /// - pos >= item_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const = 0;

  /// @brief item リストの取得
  std::vector<const AstItem*>
  item_list() const
  {
    auto n = item_num();
    std::vector<const AstItem*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(item(i));
    }
    return vec;
  }

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const AstStmt*
  body() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Func の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  virtual
  bool
  is_signed() const = 0;

  /// @brief 範囲の取得
  /// @return 範囲
  virtual
  const AstRange*
  range() const = 0;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  virtual
  VpiVarType
  data_type() const = 0;

  /// @brief constant function の展開中の印をつける．
  virtual
  void
  set_in_use() const = 0;

  /// @brief constant function の展開中の印を消す．
  virtual
  void
  clear_in_use() const = 0;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  virtual
  bool
  is_in_use() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // SpecItem の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  virtual
  VpiSpecItemType
  specitem_type() const = 0;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  virtual
  VpiSpecPathType
  specpath_type() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // SpecPath の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ターミナルの要素数の取得
  virtual
  SizeType
  terminal_num() const = 0;

  /// @brief ターミナルの取得
  ///
  /// - pos >= terminal_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstExpr*
  terminal(
    SizeType pos ///< [in] 位置 ( 0 <= pos < terminal_num() )
  ) const = 0;

  /// @brief ターミナルリストの取得
  std::vector<const AstExpr*>
  terminal_list() const
  {
    auto n = terminal_num();
    std::vector<const AstExpr*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(terminal(i));
    }
    return vec;
  }

  /// @brief パス記述の取得
  /// @return パス記述
  virtual
  const AstPathDecl*
  path_decl() const = 0;

  /// @brief 条件式の取得
  /// @return 条件式
  virtual
  const AstExpr*
  expr() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // GenIf の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  then_declhead_num() const = 0;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
  ///
  /// - pos >= then_declhead_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstDeclHead*
  then_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < then_declhead_num() )
  ) const = 0;

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
  std::vector<const AstDeclHead*>
  then_declhead_list() const
  {
    auto n = then_declhead_num();
    std::vector<const AstDeclHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(then_declhead(i));
    }
    return vec;
  }

  /// @brief 条件が成り立ったときに生成される item 配列の要素数の取得
  virtual
  SizeType
  then_item_num() const = 0;

  /// @brief 条件が成り立ったときに生成される item の取得
  ///
  /// - pos >= then_item_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstItem*
  then_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < then_item_num() )
  ) const = 0;

  /// @brief 条件が成り立った時に生成されるitemリストの取得
  std::vector<const AstItem*>
  then_item_list() const
  {
    auto n = then_item_num();
    std::vector<const AstItem*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(then_item(i));
    }
    return vec;
  }

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
  virtual
  SizeType
  else_declhead_num() const = 0;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
  ///
  /// - pos >= else_declhead_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstDeclHead*
  else_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < else_declhead_num() )
  ) const = 0;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  std::vector<const AstDeclHead*>
  else_declhead_list() const
  {
    auto n = else_declhead_num();
    std::vector<const AstDeclHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(else_declhead(i));
    }
    return vec;
  }

  /// @brief 条件が成り立たなかったときに生成される item 配列の要素数の取得
  virtual
  SizeType
  else_item_num() const = 0;

  /// @brief 条件が成り立たなかったときに生成される item の取得
  ///
  /// - pos >= else_item_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstItem*
  else_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < else_item_num() )
  ) const = 0;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  std::vector<const AstItem*>
  else_item_list() const
  {
    auto n = then_item_num();
    std::vector<const AstItem*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(else_item(i));
    }
    return vec;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // GenCase の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief case item のリストの要素数の取得
  virtual
  SizeType
  caseitem_num() const = 0;

  /// @brief case item の取得
  ///
  /// - pose >= caseitem_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstGenCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const = 0;

  /// @brief case item リストの取得
  std::vector<const AstGenCaseItem*>
  caseitem_list() const
  {
    auto n = caseitem_num();
    std::vector<const AstGenCaseItem*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(caseitem(i));
    }
    return vec;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // GenFor の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  virtual
  const char*
  loop_var() const = 0;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  virtual
  const AstExpr*
  init_expr() const = 0;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  virtual
  const AstExpr*
  next_expr() const = 0;

};

/// @brief AstItem::Type のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,  ///< [in] 出力ストリーム
  AstItem::Type val ///< [in] 値
)
{
  switch ( val ) {
  case AstItem::DefParam:   s << "DefParam"; break;
  case AstItem::ContAssign: s << "ContAssign"; break;
  case AstItem::Initial:    s << "Initial"; break;
  case AstItem::Always:     s << "Always"; break;
  case AstItem::Task:       s << "Task"; break;
  case AstItem::Func:       s << "Func"; break;
  case AstItem::GateInst:   s << "GateInst"; break;
  case AstItem::MuInst:     s << "MuInst"; break;
  case AstItem::SpecItem:   s << "SpecItem"; break;
  case AstItem::SpecPath:   s << "SpecPath"; break;
  case AstItem::Generate:   s << "Generate"; break;
  case AstItem::GenBlock:   s << "GenBlock"; break;
  case AstItem::GenIf:      s << "GenIf"; break;
  case AstItem::GenCase:    s << "GenCase"; break;
  case AstItem::GenFor:     s << "GenFor"; break;
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
/// @class AstDefParam AstItem.h "ym/vl/AstItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class AstDefParam :
  public AstHierNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstDefParam の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  /// @return 値を表す式
  virtual
  const AstExpr*
  expr() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstContAssign AstItem.h "ym/vl/AstItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief continuous assign 文
//////////////////////////////////////////////////////////////////////
class AstContAssign :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstContAssign の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式の取得
  /// @return 左辺式
  virtual
  const AstExpr*
  lhs() const = 0;

  /// @brief 右辺式の取得
  /// @return 右辺式
  virtual
  const AstExpr*
  rhs() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstInst AstItem.h "ym/vl/AstItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief module instance/UDP/gate instance を表すクラス
//////////////////////////////////////////////////////////////////////
class AstInst :
  public AstNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstInst の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の取得
  /// @return 範囲
  virtual
  const AstRange*
  range() const = 0;

  /// @brief ポートの要素数の取得
  virtual
  SizeType
  port_num() const = 0;

  /// @brief ポートの取得
  ///
  /// - pos >= port_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstConnection*
  port(
    SizeType pos ///< [in] 位置 ( 0 <= pos < port_num() )
  ) const = 0;

  /// @brief ポートリストの取得
  std::vector<const AstConnection*>
  port_list() const
  {
    auto n = port_num();
    std::vector<const AstConnection*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(port(i));
    }
    return vec;
  }

};


//////////////////////////////////////////////////////////////////////
/// @class AstGenCaseItem AstItem.h "ym/vl/AstItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class AstGenCaseItem :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstGenCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルの要素数の取得
  virtual
  SizeType
  label_num() const = 0;

  /// @brief ラベルの取得
  ///
  /// - pos >= label_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstExpr*
  label(
    SizeType pos ///< [in] 位置 ( 0 <= pos < label_num() )
  ) const = 0;

  /// @brief ラベルリストの取得
  std::vector<const AstExpr*>
  label_list() const
  {
    auto n = label_num();
    std::vector<const AstExpr*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(label(i));
    }
    return vec;
  }

  /// @brief 宣言の要素数の取得
  virtual
  SizeType
  declhead_num() const = 0;

  /// @brief 宣言の取得
  ///
  /// - pos >= declhead_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const = 0;

  /// @brief 宣言リストの取得
  std::vector<const AstDeclHead*>
  declhead_list() const
  {
    auto n = declhead_num();
    std::vector<const AstDeclHead*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(declhead(i));
    }
    return vec;
  }

  /// @brief item の要素数の取得
  virtual
  SizeType
  item_num() const = 0;

  /// @brief item の取得
  ///
  /// - pos >= item_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const = 0;

  /// @brief item リストの取得
  std::vector<const AstItem*>
  item_list() const
  {
    auto n = item_num();
    std::vector<const AstItem*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(item(i));
    }
    return vec;
  }

};


//////////////////////////////////////////////////////////////////////
/// @class AstPathDecl AstItem.h "ym/vl/AstItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief path_delay_declaration を表すクラス
//////////////////////////////////////////////////////////////////////
class AstPathDecl :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstPathDecl の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief edge_descriptor の取得
  /// @return edge_descriptor\n
  /// 0の場合もある．
  virtual
  int
  edge() const = 0;

  /// @brief 入力のリストの要素数の取得
  virtual
  SizeType
  input_num() const = 0;

  /// @brief 入力の取得
  ///
  /// - pos >= input_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstExpr*
  input(
    SizeType pos ///< [in] 位置 ( 0 <= pos < input_num() )
  ) const = 0;

  /// @brief 入力のリストの取得
  std::vector<const AstExpr*>
  input_list() const
  {
    auto n = input_num();
    std::vector<const AstExpr*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(input(i));
    }
    return vec;
  }

  /// @brief 入力の極性の取得
  /// @return 入力の極性\n
  /// 0の場合もありうる．
  virtual
  int
  input_pol() const = 0;

  /// @brief パス記述子(?)の取得
  /// @return vpiParallel か vpiFull
  virtual
  VpiPathType
  op() const = 0;

  /// @brief 出力のリストの要素数の取得
  virtual
  SizeType
  output_num() const = 0;

  /// @brief 出力の取得
  ///
  /// - pos >= output_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstExpr*
  output(
    SizeType pos ///< [in] 位置 ( 0 <= pos < output_num() )
  ) const = 0;

  /// @brief 出力リストの取得
  std::vector<const AstExpr*>
  output_list() const
  {
    auto n = output_num();
    std::vector<const AstExpr*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      vec.push_back(output(i));
    }
    return vec;
  }

  /// @brief 出力の極性の取得
  /// @return 出力の極性\n
  /// 0の場合もありうる．
  virtual
  int
  output_pol() const = 0;

  /// @brief 式の取得
  /// @return 式\n
  /// nullptr の場合もありうる．
  virtual
  const AstExpr*
  expr() const = 0;

  /// @brief path_delay_value の取得
  /// @return path_delay_value
  virtual
  const AstPathDelay*
  path_delay() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstPathDelay AstItem.h "ym/vl/AstItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////
class AstPathDelay :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstPathDecl の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を取り出す．
  /// 0の場合もある．
  virtual
  const AstExpr*
  value(
    SizeType pos ///< [in] 位置 ( 0 <= pos < 3 )
  ) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTITEM_H
