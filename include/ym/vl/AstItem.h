#ifndef YM_VL_ASTITEM_H
#define YM_VL_ASTITEM_H

/// @file ym/vl/AstItem.h
/// @brief AstItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtItem;

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
/// - declhead_list()
///   * Task
///   * Func
///   * Generate/GenBlock
///   * GenFor
///
/// - item_list()
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
/// - caseitem_list()
///   * GenCase
///
/// - loop_var(), init_expr(), next_expr()
///   * GenFor
//////////////////////////////////////////////////////////////////////
class AstItem :
  public AstNamedBaseWithPtr<const PtItem>
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

  /// @brief コンストラクタ
  explicit
  AstItem(
    const PtItem* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstNamedBaseWithPtr<const PtItem>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstItem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  /// @return 型
  Type
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 様々な型で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式の取得
  /// @return 条件式
  AstExpr
  cond_expr() const;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  AstStmt
  body() const;

  /// @brief strength の取得
  /// @return 信号強度
  AstStrength
  strength() const;

  /// @brief delay の取得
  /// @return 遅延
  AstDelay
  delay() const;


public:
  //////////////////////////////////////////////////////////////////////
  // DefParam 型の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief defparam リストの取得
  AstDefParamList
  defparam_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ContAssign の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief continuous assign リストの取得
  AstContAssignList
  contassign_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // GateInst 型の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  VpiPrimType
  prim_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // MuInst の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ割り当てのリストの取得
  AstConnectionList
  paramassign_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // GateInst/MuInst の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief module/UDP/gate instance リストの取得
  AstInstList
  inst_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部に宣言要素を持つタイプの関数
  // Task/Function/Generate系
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言ヘッダリストの取得
  AstDeclHeadList
  declhead_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部に要素を持つタイプの関数
  // Generate系
  //////////////////////////////////////////////////////////////////////

  /// @brief item リストの取得
  AstItemList
  item_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // Task/Func の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  bool
  automatic() const;

  /// @brief IO宣言ヘッダリストの取得
  AstIOHeadList
  iohead_list() const;

  /// @brief IO要素の数
  SizeType
  ioitem_num() const;


public:
  //////////////////////////////////////////////////////////////////////
  // Function の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const;

  /// @brief 範囲の取得
  /// @return 範囲
  AstRange
  range() const;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  VpiVarType
  data_type() const;

  /// @brief constant function の展開中の印をつける．
  void
  set_in_use() const;

  /// @brief constant function の展開中の印を消す．
  void
  clear_in_use() const;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  bool
  is_in_use() const;


public:
  //////////////////////////////////////////////////////////////////////
  // SpecItem の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  VpiSpecItemType
  specitem_type() const;

  /// @brief ターミナルリストの取得
  AstExprList
  terminal_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // SpecPath の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  VpiSpecPathType
  specpath_type() const;

  /// @brief パス記述の取得
  /// @return パス記述
  AstPathDecl
  path_decl() const;


public:
  //////////////////////////////////////////////////////////////////////
  // GenIf の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
  AstDeclHeadList
  then_declhead_list() const;

  /// @brief 条件が成り立った時に生成される要素リストの取得
  AstItemList
  then_item_list() const;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  AstDeclHeadList
  else_declhead_list() const;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  AstItemList
  else_item_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // GenCase の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief case item リストの取得
  AstGenCaseItemList
  caseitem_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // GenFor の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  const char*
  loop_var() const;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  AstExpr
  init_expr() const;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  AstExpr
  next_expr() const;


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


public:
  //////////////////////////////////////////////////////////////////////
  // AstNamedBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  const char*
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstItem
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief json_obj() の下請け関数
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const override;

  /// @brief DefParam 用の json_sub()
  void
  json_sub_defparam(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief ContAssign 用の json_sub()
  void
  json_sub_contassign(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief Initial/Process 用の json_sub()
  void
  json_sub_process(
    JsonValue& jobj,  ///< [in] 対象の JSON オブジェクト
    const char* label ///< [in] ラベル
  ) const;

  /// @brief Task 用の json_sub()
  void
  json_sub_task(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief Func 用の json_sub()
  void
  json_sub_func(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief GateInst 用の json_sub()
  void
  json_sub_gateinst(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief MuInst 用の json_sub()
  void
  json_sub_muinst(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief SpecItem 用の json_sub()
  void
  json_sub_specitem(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief SpecPath 用の json_sub()
  void
  json_sub_specpath(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief Generate 用の json_sub()
  void
  json_sub_generate(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief GenBlock 用の json_sub()
  void
  json_sub_genblock(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief GenIf 用の json_sub()
  void
  json_sub_genif(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief GenCase 用の json_sub()
  void
  json_sub_gencase(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief GenFor 用の json_sub()
  void
  json_sub_genfor(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

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

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTITEM_H
