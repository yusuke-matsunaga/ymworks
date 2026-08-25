#ifndef YM_VL_ASTITEM_H
#define YM_VL_ASTITEM_H

/// @file ym/vl/AstItem.h
/// @brief AstItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"
#include "ym/vl/AstDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstItem AstItem.h "ym/vl/AstItem.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief ブロック内要素の共通の親クラス
///
/// 個々の型ごとに専用の関数を持つ．
/// それ以外の型では std::logic_error 例外を送出する．
/// AstItem は特に種類が多いので注意が必要．
///
/// - 共通(AstNamedBase)
///   * file_region()
///   * name()
///   * type()
///
/// - cond_expr()
///   * SpecPath
///   * GenIf
///   * GenCase
///
/// - body()
///   * Initial
///   * Always
///   * Task
///   * Func
///
/// - strength(), delay()
///   * ContAssign
///   * GateInst
///
/// - defparam_list()
///   * DefParam
///
/// - contassign_list()
///   * ContAssign
///
/// - prim_type()
///   * GateInst
///
/// - paramassign_lsit()
///   * MuInst

/// - inst_list()
///   * GateInst
///   * MuInst
///
/// - declhead_list(), item_list()
///   * Task
///   * Func
///   * Generate/GenBlock
///   * GenFor
///
/// - automatic(), iohead_list()
///   * Task
///   * Func
///
/// - is_signed(), range(), data_type()
///   * Func
///
/// - specitem_type(), terminal_list()
///   * SpecItem
///
/// - specpath_type(), path_decl()
///   * SpecPath
///
/// - then_declhead_list(), then_item_list(),
///   else_declhead_list(), else_item_list()
///   * GenIf
///
/// - loop_var(), init_expr(), next_expr()
///   * GenFor
//////////////////////////////////////////////////////////////////////
class AstItem :
  public AstNamedBase
{
  friend class AstList<const AstItem>;
  friend class AstListIter<const AstItem>;

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
  // 様々な型で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式の取得
  /// @return 条件式
  virtual
  const AstExpr*
  cond_expr() const = 0;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  virtual
  const AstStmt*
  body() const = 0;

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

  /// @brief defparam リストの取得
  virtual
  AstDefParamList
  defparam_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ContAssign の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief continuous assign リストの取得
  virtual
  AstContAssignList
  contassign_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // GateInst 型の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  virtual
  VpiPrimType
  prim_type() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // MuInst の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ割り当てのリストの取得
  virtual
  AstConnectionList
  paramassign_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // GateInst/MuInst の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module/UDP/gate instance リストの取得
  virtual
  AstInstList
  inst_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部に要素を持つタイプの関数
  // Task/Function/Generate系
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言ヘッダリストの取得
  virtual
  AstDeclHeadList
  declhead_list() const = 0;

  /// @brief item リストの取得
  virtual
  AstItemList
  item_list() const = 0;


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

  /// @brief IO宣言ヘッダリストの取得
  virtual
  AstIOHeadList
  iohead_list() const = 0;

  /// @brief IO要素の数
  virtual
  SizeType
  ioitem_num() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // Function の関数
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

  /// @brief ターミナルリストの取得
  virtual
  AstExprList
  terminal_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // SpecPath の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  virtual
  VpiSpecPathType
  specpath_type() const = 0;

  /// @brief パス記述の取得
  /// @return パス記述
  virtual
  const AstPathDecl*
  path_decl() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // GenIf の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
  virtual
  AstDeclHeadList
  then_declhead_list() const = 0;

  /// @brief 条件が成り立った時に生成される要素リストの取得
  virtual
  AstItemList
  then_item_list() const = 0;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  virtual
  AstDeclHeadList
  else_declhead_list() const = 0;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  virtual
  AstItemList
  else_item_list() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // GenCase の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief case item リストの取得
  virtual
  AstGenCaseItemList
  caseitem_list() const = 0;


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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstItem*
  link() const = 0;

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
  friend class AstList<const AstDefParam>;
  friend class AstListIter<const AstDefParam>;

public:
  //////////////////////////////////////////////////////////////////////
  // AstDefParam の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の取得
  /// @return 値を表す式
  virtual
  const AstExpr*
  expr() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstDefParam*
  link() const = 0;

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
  friend class AstList<const AstContAssign>;
  friend class AstListIter<const AstContAssign>;

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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstContAssign*
  link() const = 0;

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
  friend class AstList<const AstInst>;
  friend class AstListIter<const AstInst>;

public:
  //////////////////////////////////////////////////////////////////////
  // AstInst の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲の取得
  /// @return 範囲
  virtual
  const AstRange*
  range() const = 0;

  /// @brief ポートリストの取得
  virtual
  AstConnectionList
  port_list() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstInst*
  link() const = 0;

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
  friend class AstList<const AstGenCaseItem>;
  friend class AstListIter<const AstGenCaseItem>;

public:
  //////////////////////////////////////////////////////////////////////
  // AstGenCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルリストの取得
  virtual
  AstExprList
  label_list() const = 0;

  /// @brief 宣言リストの取得
  virtual
  AstDeclHeadList
  declhead_list() const = 0;

  /// @brief item リストの取得
  virtual
  AstItemList
  item_list() const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素の取得
  virtual
  const AstGenCaseItem*
  link() const = 0;

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
  /// @return edge_descriptor
  ///
  /// 0の場合もある．
  virtual
  int
  edge() const = 0;

  /// @brief 入力のリストの取得
  virtual
  AstExprList
  input_list() const = 0;

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

  /// @brief 出力リストの取得
  virtual
  AstExprList
  output_list() const = 0;

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
