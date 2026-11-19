
/// @file FuncEval.cc
/// @brief FuncEval の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncEval.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlIODecl.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/VlExpr.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
FuncEval::FuncEval(
  LogMgr& log_mgr,
  const VlTaskFunc* function
) : mLogMgr{log_mgr},
    mFunction{function}
{
}

// @brief デストラクタ
FuncEval::~FuncEval()
{
}

// @brief 関数を評価する．
VlValue
FuncEval::operator()(
  const std::vector<VlValue>& arg_list
)
{
  // 入力変数の値をセットする．
  auto io_num = mFunction->io_num();
  if ( io_num != arg_list.size() ) {
    throw std::logic_error{"io_num != arg_list.size()"};
  }

  for ( SizeType index = 0; index < io_num; ++ index ) {
    auto io = mFunction->io(index);
    if ( io->direction() != VpiDir::Input ) {
      throw std::logic_error{"io->direction() != VpiDir::Input"};
    }
    auto decl = io->decl();
    auto key = decl->decl_id();
    reg_val(key, arg_list[index]);
  }

  // 本体のステートメントを実行する．
  auto body = mFunction->stmt();
  evaluate_stmt(body);

  // 出力結果を得る．
  auto ovar = mFunction->ovar();
  auto ovar_id = ovar->decl_id();
  auto val = get_val(ovar_id);

  return val;
}

// ステートメントを実行する．
const VlScope*
FuncEval::evaluate_stmt(
  const VlStmt* stmt
)
{
  // disable が実行された場合の対象のスコープ
  auto break_scope = (const VlScope*)nullptr;

  // ここでは個々のステートメント用の関数にディスパッチするだけ．
  switch ( stmt->type() ) {
  case VpiObjType::Begin:
    break_scope = evaluate_seqblock(stmt);
    break;

  case VpiObjType::NamedBegin:
    break_scope = evaluate_namedseqblock(stmt);
    break;

  case VpiObjType::NullStmt:
    // なにもしない．
    break;

  case VpiObjType::Assignment:
    break_scope = evaluate_assign(stmt);
    break;

  case VpiObjType::While:
    break_scope = evaluate_while(stmt);
    break;

  case VpiObjType::Repeat:
    break_scope = evaluate_repeat(stmt);
    break;

  case VpiObjType::For:
    break_scope = evaluate_for(stmt);
    break;

  case VpiObjType::Forever:
    break_scope = evaluate_forever(stmt);
    break;

  case VpiObjType::If:
    break_scope = evaluate_if(stmt);
    break;

  case VpiObjType::IfElse:
    break_scope = evaluate_ifelse(stmt);
    break;

  case VpiObjType::Case:
    break_scope = evaluate_case(stmt);
    break;

  case VpiObjType::Disable:
    break_scope = evaluate_disable(stmt);
    break;

  case VpiObjType::SysTaskCall:
    // constant expression の評価時には無視される．
    break;

  default:
    // 上記以外はエラー
    throw std::logic_error{"FuncEval::evaluate_stmt(): wrong ast_stmt.type()"};
    break;
  }

  return break_scope;
}

// @brief begin-end ブロックの実行を行う．
const VlScope*
FuncEval::evaluate_seqblock(
  const VlStmt* stmt
)
{
  auto  n = stmt->child_stmt_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto break_scope = evaluate_stmt(stmt->child_stmt(i));
    if ( break_scope != nullptr ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief 名前付き begin-end ブロックの実行を行う．
const VlScope*
FuncEval::evaluate_namedseqblock(
  const VlStmt* stmt
)
{
  auto n = stmt->child_stmt_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto break_scope = evaluate_stmt(stmt->child_stmt(i));
    if ( break_scope != nullptr ) {
      if ( break_scope == stmt->scope() ) {
	// 自分自身のスコープの実行を終了する．
	// 上のスコープには通常の終了として通知する．
	break_scope = nullptr;
      }
      return break_scope;
    }
  }
  return nullptr;
}

// @brief 代入文の実行を行う．
const VlScope*
FuncEval::evaluate_assign(
  const VlStmt* stmt
)
{
  if ( stmt->control() != nullptr ) {
    throw std::logic_error{"stmt->control() != nullptr"};
  }
  if ( !stmt->is_blocking() ) {
    throw std::logic_error{"!stmt->is_blocking()"};
  }

  auto rhs = stmt->rhs();
  auto val = evaluate_expr(rhs);

  // lhs に val を代入する．
  auto lhs = stmt->lhs();
  if ( lhs->is_primary() ) {
    assign_primary(lhs, val);
  }
  else {
    // 左辺が連結式の場合
    auto nl = lhs->lhs_elem_num();
    auto bv = val.bitvector_value();
    SizeType base{0};
    for ( SizeType i = 0; i < nl; ++ i ) {
      auto primary = lhs->lhs_elem(i);
      auto w = primary->bit_size();
      auto part_val = bv.part_select_op(base + w - 1, base);
      assign_primary(primary, VlValue(part_val));
      base += w;
    }
  }

  return nullptr;
}

// @brief プライマリに値を代入する．
void
FuncEval::assign_primary(
  const VlExpr* expr,
  const VlValue& val
)
{
  // 対象が
  // - 単独の要素
  // - 配列要素
  // の２通り．
  // 代入範囲が
  // - 要素全体
  // - ビット選択
  // - 範囲選択
  // の3通りがある．

  SizeType decl_id = get_decl_id(expr);
  auto stype = expr->select_type();
  if ( stype ==VlSelectType::NONE ) {
    reg_val(decl_id, val);
  }
  else {
    // ビット/部分選択あり
    auto decl = expr->decl_base();
    auto width = expr->bit_size();
    // 元の値を取り出す．
    // 初期化が必要なので get_val() は使えない．
    BitVector bv0;
    if ( mValMap.count(decl_id) == 0 ) {
      // 未定義なので X で初期化する．
      // decl が real type ならエラー
#warning "TODO: エラー処理"
      bv0 = BitVector(VlScalarVal::x(), decl->bit_size());
    }
    else {
      auto val0 = mValMap.at(decl_id);
      if ( !val0.is_bitvector_compat() ) {
#warning "TODO: エラー処理"
      }
      bv0 = val0.bitvector_value();
    }
    int lsb, msb;
    if ( stype == VlSelectType::CONST ) {
      // 固定インデックス
      auto offset = expr->bit_offset();
      lsb = offset;
      msb = lsb + width - 1;
    }
    else {
      // 可変インデックス
      auto decl = expr->decl_base();
      auto index = expr->bit_index();
      auto index_val = evaluate_int(index);
      SizeType offset1;
      if ( !decl->calc_bit_offset(index_val, offset1) ) {
#warning "TODO: エラー処理"
      }
      if ( stype == VlSelectType::PLUS ) {
	index_val += width - 1;
      }
      else { // stype == VlSelectType::MINUS
	index_val -= width - 1;
      }
      SizeType offset2;
      if ( !decl->calc_bit_offset(index_val, offset2) ) {
#warning "TODO: エラー処理"
      }
      if ( offset1 < offset2 ) {
	lsb = offset1;
	msb = offset2;
      }
      else {
	lsb = offset2;
	msb = offset1;
      }
    }
    bv0.part_select_op(msb, lsb, val.bitvector_value());
    auto val1 = VlValue(bv0);
    reg_val(decl_id, val1);
  }
}

// @brief while 文の実行を行う．
const VlScope*
FuncEval::evaluate_while(
  const VlStmt* stmt
)
{
  auto cond_expr = stmt->expr();
  auto body_stmt = stmt->body_stmt();
  while ( evaluate_bool(cond_expr) ) {
    auto break_scope = evaluate_stmt(body_stmt);
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief repeat 文の実行を行う．
const VlScope*
FuncEval::evaluate_repeat(
  const VlStmt* stmt
)
{
  auto rep_expr = stmt->expr();
  int rep_num = evaluate_int(rep_expr);
  auto body_stmt = stmt->body_stmt();
  for ( int i: Range(rep_num) ) {
    auto break_scope = evaluate_stmt(body_stmt);
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief for 文の実行を行う．
const VlScope*
FuncEval::evaluate_for(
  const VlStmt* stmt
)
{
  auto cond_expr = stmt->expr();
  auto init_stmt = stmt->init_stmt();
  auto body_stmt = stmt->body_stmt();
  auto next_stmt = stmt->inc_stmt();
  auto break_scope = evaluate_stmt(init_stmt);
  if ( break_scope ) {
    return break_scope;
  }
  while ( evaluate_bool(cond_expr) ) {
    break_scope = evaluate_stmt(body_stmt);
    if ( break_scope ) {
      return break_scope;
    }
    break_scope = evaluate_stmt(next_stmt);
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief forever 文の実行を行う．
const VlScope*
FuncEval::evaluate_forever(
  const VlStmt* stmt
)
{
  auto body_stmt = stmt->body_stmt();
  for ( ; ; ) {
    auto break_scope = evaluate_stmt(body_stmt);
    if ( break_scope ) {
      return break_scope;
    }
  }
  return nullptr;
}

// @brief if 文の実行を行う．
const VlScope*
FuncEval::evaluate_if(
  const VlStmt* stmt
)
{
  auto cond_expr = stmt->expr();
  if ( evaluate_bool(cond_expr) ) {
    auto then_stmt = stmt->body_stmt();
    auto break_scope = evaluate_stmt(then_stmt);
    return break_scope;
  }
  return nullptr;
}

// @brief if-else 文の実行を行う．
const VlScope*
FuncEval::evaluate_ifelse(
  const VlStmt* stmt
)
{
  auto cond_expr = stmt->expr();
  if ( evaluate_bool(cond_expr) ) {
    auto then_stmt = stmt->body_stmt();
    auto break_scope = evaluate_stmt(then_stmt);
    return break_scope;
  }
  else {
    auto else_stmt = stmt->else_stmt();
    auto break_scope = evaluate_stmt(else_stmt);
    return break_scope;
  }
}

// @brief case 文の実行を行う．
const VlScope*
FuncEval::evaluate_case(
  const VlStmt* stmt
)
{
  auto case_type = stmt->case_type();
  auto expr = stmt->expr();
  auto switch_val = evaluate_expr(expr);
  SizeType n = stmt->caseitem_num();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto caseitem{stmt->caseitem(i)};
    if ( match(case_type, switch_val, caseitem) ) {
      auto body = caseitem->body_stmt();
      auto break_scope = evaluate_stmt(body);
      return break_scope;
    }
  }
  return nullptr;
}

// @brief caseitem のラベルと一致するか調べる．
bool
FuncEval::match(
  VpiCaseType case_type,
  const VlValue& val,
  const VlCaseItem* caseitem
)
{
  SizeType nexp = caseitem->expr_num();
  if ( nexp == 0 ) {
    // default なのでマッチする．
    return true;
  }

  for ( SizeType i = 0; i < nexp; ++ i ) {
    auto label_expr = caseitem->expr(i);
    auto label_val = evaluate_expr(label_expr);
    VlValue eq_val;
    if ( case_type == VpiCaseType::Exact ) {
      eq_val = eq(val, label_val);
    }
    else if ( case_type == VpiCaseType::X ) {
      eq_val = eq_with_x(val, label_val);
    }
    else if ( case_type == VpiCaseType::Z ) {
      eq_val = eq_with_xz(val, label_val);
    }
    else {
      throw std::logic_error{"FuncEval::match(): wrong case_type"};
    }
    return eq_val.logic_value().to_bool();
  }
  return false;
}

// @brief disable 文の実行を行う．
const VlScope*
FuncEval::evaluate_disable(
  const VlStmt* stmt
)
{
  return stmt->target_scope();
}

// @brief 式の評価を行う．
VlValue
FuncEval::evaluate_expr(
  const VlExpr* expr
)
{
  switch ( expr->expr_type() ) {
  case VlExprType::CONST:       return evaluate_const(expr);
  case VlExprType::PRIMARY:     return evaluate_primary(expr);
  case VlExprType::OPERATION:   return evaluate_opr(expr);
  case VlExprType::FUNCCALL:    return evaluate_funccall(expr);
  case VlExprType::SYSFUNCCALL: return VlValue(); // 定数式の中では無視
  default: break; // NONE と LHS
  }
  throw std::logic_error{"FuncEval::evalueate_expr(): wrong expr_type"};
}

// @brief 演算子に対して式の値を評価する．
VlValue
FuncEval::evaluate_opr(
  const VlExpr* expr
)
{
  SizeType n_op = expr->operand_num();
  // オペランドの値を評価する．
  std::vector<VlValue> opr_vals(n_op);
  for ( SizeType i = 0; i < n_op; ++ i ) {
    opr_vals[i] = evaluate_expr(expr->operand(i));
  }

  // ほとんど VlValue の該当の関数を呼び出すだけ．
  switch ( expr->op_type() ) {
  case VpiOpType::Minus:
    return - opr_vals[0];
  case VpiOpType::Plus:
    return opr_vals[0];
  case VpiOpType::Not:
    return log_not(opr_vals[0]);
  case VpiOpType::BitNeg:
    return bit_negate(opr_vals[0]);
  case VpiOpType::UnaryAnd:
    return reduction_and(opr_vals[0]);
  case VpiOpType::UnaryNand:
    return reduction_nand(opr_vals[0]);
  case VpiOpType::UnaryOr:
    return reduction_or(opr_vals[0]);
  case VpiOpType::UnaryNor:
    return reduction_nor(opr_vals[0]);
  case VpiOpType::UnaryXor:
    return reduction_xor(opr_vals[0]);
  case VpiOpType::UnaryXNor:
    return reduction_xnor(opr_vals[0]);
  case VpiOpType::Sub:
    return opr_vals[0] - opr_vals[1];
  case VpiOpType::Div:
    return opr_vals[0] / opr_vals[1];
  case VpiOpType::Mod:
    return opr_vals[0] % opr_vals[1];
  case VpiOpType::Eq:
    return eq(opr_vals[0], opr_vals[1]);
  case VpiOpType::Neq:
    return ne(opr_vals[0], opr_vals[1]);
  case VpiOpType::CaseEq:
    return eq_with_x(opr_vals[0], opr_vals[1]);
  case VpiOpType::CaseNeq:
    return log_not(eq_with_x(opr_vals[0], opr_vals[1]));
  case VpiOpType::Gt:
    return gt(opr_vals[0], opr_vals[1]);
  case VpiOpType::Ge:
    return ge(opr_vals[0], opr_vals[1]);
  case VpiOpType::Lt:
    return lt(opr_vals[0], opr_vals[1]);
  case VpiOpType::Le:
    return le(opr_vals[0], opr_vals[1]);
  case VpiOpType::LShift:
    return opr_vals[0] << opr_vals[1];
  case VpiOpType::RShift:
    return opr_vals[0] >> opr_vals[1];
  case VpiOpType::Add:
    return opr_vals[0] + opr_vals[1];
  case VpiOpType::Mult:
    return opr_vals[0] * opr_vals[1];
  case VpiOpType::LogAnd:
    return log_and(opr_vals[0], opr_vals[1]);
  case VpiOpType::LogOr:
    return log_or(opr_vals[0], opr_vals[1]);
  case VpiOpType::BitAnd:
    return bit_and(opr_vals[0], opr_vals[1]);
  case VpiOpType::BitOr:
    return bit_or(opr_vals[0], opr_vals[1]);
  case VpiOpType::BitXor:
    return bit_xor(opr_vals[0], opr_vals[1]);
  case VpiOpType::BitXNor:
    return bit_xnor(opr_vals[0], opr_vals[1]);
  case VpiOpType::Condition:
    return ite(opr_vals[0], opr_vals[1], opr_vals[2]);
  case VpiOpType::Concat:
    return concat(opr_vals);
  case VpiOpType::MultiConcat:
    return multi_concat(opr_vals);
  case VpiOpType::ArithLShift:
    // 算術左シフトは普通の左シフトと同じ
    return opr_vals[0] << opr_vals[1];
  case VpiOpType::ArithRShift:
    return arshift(opr_vals[0], opr_vals[1]);
  case VpiOpType::Power:
    return power(opr_vals[0], opr_vals[1]);

  case VpiOpType::EventOr:
  case VpiOpType::Null:
  case VpiOpType::List:
  case VpiOpType::MinTypMax:
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    // これらは使えない．
    throw std::logic_error{"FuncEval::evaluate_opr(): wrong expr->op_type()"};
  }
}

// @brief 定数に対して式の値を評価する．
VlValue
FuncEval::evaluate_const(
  const VlExpr* expr
)
{
  return expr->constant_value();
}

// @brief プライマリに対して式の値を評価する．
VlValue
FuncEval::evaluate_primary(
  const VlExpr* expr
)
{
  auto decl_id = get_decl_id(expr);
  // 宣言要素の値を求める．
  auto base_value = get_val(decl_id);
  auto stype = expr->select_type();
  if ( stype == VlSelectType::NONE ) {
    // そのまま
    return base_value;
  }

  // ビット/部分選択の場合はビットベクタ型に変換する．
  if ( !base_value.is_bitvector_compat() ) {
    throw std::logic_error{"src_value.is_bitvector_compat() == false"};
  }
  auto decl = expr->decl_base(); // オフセット変換用
  auto bv = base_value.bitvector_value();
  auto width = expr->bit_size();
  int msb, lsb;
  if ( stype == VlSelectType::CONST ) {
    auto offset = expr->bit_offset();
    lsb = offset;
    msb = lsb + width - 1;
  }
  else {
    auto index = expr->bit_index();
    auto index_val = evaluate_int(index);
    SizeType offset1;
    if ( !decl->calc_bit_offset(index_val, offset1) ) {
      throw std::out_of_range{"index out of range"};
    }
    if ( stype == VlSelectType::PLUS ) {
      index_val += width - 1;
    }
    else { // stype == VlSelectType::MINUS
      index_val -= width - 1;
    }
    SizeType offset2;
    if ( !decl->calc_bit_offset(index_val, offset2) ) {
      throw std::out_of_range{"index out of range"};
    }
    if ( offset1 < offset2 ) {
      lsb = offset1;
      msb = offset2;
    }
    else {
      lsb = offset2;
      msb = offset1;
    }
  }
  auto bv1 = bv.part_select_op(msb, lsb);
  return VlValue(bv1);
}

// @brief 関数呼び出しに対して式の値を評価する．
VlValue
FuncEval::evaluate_funccall(
  const VlExpr* expr
)
{
  auto func = expr->function();

  // 引数の値を評価する．
  SizeType n_io = expr->argument_num();
  if ( n_io != func->io_num() ) {
    throw std::logic_error{"n_io != func->io_num()"};
  }
  std::vector<VlValue> arg_list(n_io);
  for ( SizeType i = 0; i < n_io; ++ i ) {
    arg_list[i] = evaluate_expr(expr->argument(i));
  }

  FuncEval eval(mLogMgr, func);
  return eval(arg_list);
}

// @brief 式を評価して整数値を返す．
int
FuncEval::evaluate_int(
  const VlExpr* expr
)
{
  auto val = evaluate_expr(expr);
  if ( !val.is_int_compat() ) {
    mLogMgr.error_int_required(__FILE__, __LINE__, expr);
  }
  return val.int_value();
}

// @brief 式を評価して整数値を返す．
bool
FuncEval::evaluate_bool(
  const VlExpr* expr
)
{
  auto val = evaluate_expr(expr);
  return val.logic_value().to_bool();
}

// @brief インデックス式を評価する．
std::vector<int>
FuncEval::evaluate_index(
  const std::vector<const VlExpr*>& index_list
)
{
  auto n = index_list.size();
  std::vector<int> ans_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto index = index_list[i];
    auto val = evaluate_int(index);
    ans_list[i] = val;
  }
  return ans_list;
}

// @brief プライマリ式から宣言要素の番号を取り出す．
SizeType
FuncEval::get_decl_id(
  const VlExpr* expr
)
{
  auto ptype = expr->primary_type();
  if ( ptype == VlPrimaryType::DECL ) {
    auto decl = expr->decl_obj();
    return decl->decl_id();
  }
  if ( ptype == VlPrimaryType::CONST_DECL_ELEM ) {
    auto declarray = expr->declarray_obj();
    auto offset = expr->declarray_offset();
    return declarray->decl_id(offset);
  }
  if ( ptype == VlPrimaryType::VAR_DECL_ELEM ) {
    auto declarray = expr->declarray_obj();
    auto index_expr_list = expr->declarray_index_list();
    auto index_list = evaluate_index(index_expr_list);
    SizeType offset = 0;
    auto r = declarray->calc_array_offset(index_list, offset);
    if ( !r ) {
      std::out_of_range{"FuncEval::get_decl_id(): index is out of range"};
    }
    return declarray->decl_id(offset);
  }

  throw std::logic_error{"FuncEval::get_decl_id(): illiegal type"};
}

// @brief 値を登録する．
void
FuncEval::reg_val(
  SizeType key,
  const VlValue& val
)
{
  mValMap.emplace(key, val);
}

// @brief 値を取り出す．
VlValue
FuncEval::get_val(
  SizeType key
) const
{
  if ( mValMap.count(key) == 0 ) {
    throw std::logic_error{"mValMap.count(key) == 0"};
  }
  return mValMap.at(key);
}

END_NAMESPACE_YM_VERILOG
