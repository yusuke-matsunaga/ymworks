#ifndef YM_VL_ASTEXPR_H
#define YM_VL_ASTEXPR_H

/// @file ym/vl/AstExpr.h
/// @brief AstExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtExpr;

//////////////////////////////////////////////////////////////////////
/// @class AstExpr AstExpr.h "ym/vl/AstExpr.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief expression を表すクラスの共通の親クラス
///
/// 大まかには以下の種類(Type)に分類される．
/// - Opr:         演算子
/// - FuncCall:    関数呼び出し
/// - SysFuncCall: システム関数呼び出し
/// - Primary:     プライマリ
/// - Const:       定数
///
/// 各グループごとに関係するメソッドが用意されている．
/// それ以外のグループの場合には std::logic_error 例外を送出する．
//////////////////////////////////////////////////////////////////////
class AstExpr :
  public AstHierNamedBaseWithPtr<const PtExpr>
{
public:

  /// @brief AstExpr の派生クラスを識別するための列挙型
  enum Type : std::uint8_t {
    Opr,         ///< 演算子
    FuncCall,    ///< 関数呼び出し
    SysFuncCall, ///< システム関数呼び出し
    Primary,     ///< プライマリ
    Const        ///< 定数
  };


public:

  /// @brief コンストラクタ
  explicit
  AstExpr(
    const PtExpr* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstHierNamedBaseWithPtr<const PtExpr>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstExpr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 全グループに共通の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式の型の取得
  /// @return 式の型
  Type
  type() const;

  /// @brief インデックスとして使える式のチェック
  /// @retval true 階層名の添字として使える式
  /// @retval false 使えない式
  bool
  is_index_expr() const;

  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  ///
  /// - is_index_expr() が false の時は std::logic_error 例外を送出する．
  int
  index_value() const;

  /// @brief 階層名を持っている時 true を返す．
  bool
  has_hierarchical_name() const;

  /// @brief 式を表す文字列表現の取得
  std::string
  decompile() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 演算子グループの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子の種類の取得
  /// @return 演算子の種類
  ///
  /// - type() != Opr の時 std::logi_error 例外を送出する．
  VpiOpType
  op_type() const;

  /// @brief オペランド数を返す．
  ///
  /// 可変型の場合は 0 を返す．
  SizeType
  operand_num() const;

  /// @brief 0番目のオペランドの取得
  ///
  /// - operand_num() < 1 の時 std::logic_error 例外を送出する．
  AstExpr
  operand0() const;

  /// @brief 1番目のオペランドの取得
  ///
  /// - operand_num() < 2 の時 std::logic_error 例外を送出する．
  AstExpr
  operand1() const;

  /// @brief 2番目のオペランドの取得
  ///
  /// - operand_num() < 3 の時 std::logic_error 例外を送出する．
  AstExpr
  operand2() const;

  /// @brief オペランドのリストの取得
  ///
  /// - Concat/MultiConcat 演算以外は
  ///   std::logi_error 例外を送出する．
  AstExprList
  operand_list() const;

  /// @brief multi-concat の繰り返し数
  ///
  /// - type() != Opr および op_type() != MultiConcat の時
  ///   std::logic 例外を送出する．
  AstExpr
  rep() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 関数呼び出しグループの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数リストの取得
  ///
  /// - FuncCall/SysFuncCall 演算以外は std::logi_error 例外を送出する．
  AstExprList
  arg_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // プライマリグループの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数インデックスのチェック
  /// @retval true インデックスもしくは範囲が定数にならなければならないとき
  /// @retval false 上記以外
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  bool
  is_const_index() const;

  /// @brief インデックスリストの取得
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  AstExprList
  index_list() const;

  /// @brief 範囲指定を表す構文木を返す．
  /// @return 範囲指定
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  AstPart
  part() const;

  /// @brief simple primary のチェック
  /// @retval true index_list も part も持たないとき
  /// @retval false 上記以外
  ///
  /// - op_type() != Primary の時 std::logic_error 例外を送出する．
  bool
  is_simple() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 定数グループの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数の種類の取得
  /// @return 定数の種類
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  VpiConstType
  const_type() const;

  /// @brief 整数型の定数のサイズの取得
  /// @return サイズ
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  /// - サイズ指定の無い場合には 0 を返す．
  SizeType
  const_size() const;

  /// @brief ビットベクタ型の値の取得
  /// @return 値
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  BitVector
  const_bitvect() const;

  /// @brief 整数型および文字列型の定数の文字列表現の取得
  /// @return 値の文字列表現
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  /// - 整数型時のサイズと基数は含まない．
  /// - 整数型で文字列表現を持たない場合には nullptr を返す．
  const char*
  const_str() const;

  /// @brief 実数型の値の取得
  /// @return 値
  ///
  /// - op_type() != Const の時 std::logic_error 例外を送出する．
  double
  const_real() const;


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
  // AstHierNamedBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチのリストを返す．
  AstNameBranchList
  namebranch_list() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstExpr
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

  /// @brief Opr 用の json_sub()
  void
  json_sub_opr(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief FuncCall 用の json_sub()
  void
  json_sub_funccall(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief SysFuncCall 用の json_sub()
  void
  json_sub_sysfunccall(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief Primary 用の json_sub()
  void
  json_sub_primary(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

  /// @brief Const 用の json_sub()
  void
  json_sub_const(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const;

};

/// @brief AstExpr::Type のストリーム出力演算子
inline
std::ostream&
operator<<(
  std::ostream& s,  ///< [in] 出力ストリーム
  AstExpr::Type val ///< [in] 値
)
{
  switch ( val ) {
  case AstExpr::Opr:         s << "Opr"; break;
  case AstExpr::FuncCall:    s << "FuncCall"; break;
  case AstExpr::SysFuncCall: s << "SysFuncCall"; break;
  case AstExpr::Primary:     s << "Primary"; break;
  case AstExpr::Const:       s << "Concat"; break;
  }
  return s;
}

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTEXPR_H
