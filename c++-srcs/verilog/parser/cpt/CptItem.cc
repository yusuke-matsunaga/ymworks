
/// @file CptItem.cc
/// @brief CptItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtFactory.h"
#include "parser/PtHierName.h"
#include "parser/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// item の基底クラス
//////////////////////////////////////////////////////////////////////

// @brief プリミティブタイプの取得
VpiPrimType
CptItem::prim_type() const
{
  throw std::logic_error{"Not an INSTANCE type"};
}

// @brief strength の取得
const AstStrength*
CptItem::strength() const
{
  return nullptr;
}

// @brief delay の取得
const AstDelay*
CptItem::delay() const
{
  return nullptr;
}

// @brief パラメータ割り当て数の取得
SizeType
CptItem::paramassign_num() const
{
  return 0;
}

// @brief パラメータ割り当ての取得
const AstConnection*
CptItem::paramassign(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief defparam の要素数の取得
SizeType
CptItem::defparam_num() const
{
  return 0;
}

// @brief defparam の取得
const AstDefParam*
CptItem::defparam(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief continuous assign の要素数の取得
SizeType
CptItem::contassign_num() const
{
  return 0;
}

// @brief continuous assign の取得
const AstContAssign*
CptItem::contassign(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
CptItem::inst_num() const
{
  return 0;
}

// @brief module/UDP/gate instance の取得
const AstInst*
CptItem::inst(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 名前の取得
const char*
CptItem::name() const
{
  return nullptr;
}

// @brief automatic 情報の取得
bool
CptItem::automatic() const
{
  return false;
}

// @brief IO宣言の要素数の取得
SizeType
CptItem::ioitem_num() const
{
  return 0;
}

// @brief IO宣言ヘッダリストの要素数の取得
SizeType
CptItem::iohead_num() const
{
  return 0;
}

// @brief IO宣言ヘッダの取得
const AstIOHead*
CptItem::iohead(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptItem::declhead_num() const
{
  return 0;
}

// @brief 宣言ヘッダの取得
const AstDeclHead*
CptItem::declhead(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief item リストの要素数の取得
SizeType
CptItem::item_num() const
{
  return 0;
}

// @brief item の取得
const AstItem*
CptItem::item(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 本体のステートメントの取得
const AstStmt*
CptItem::body() const
{
  return nullptr;
}

// @brief 符号の取得
bool
CptItem::is_signed() const
{
  return false;
}

// @brief 範囲の取得
const AstRange*
CptItem::range() const
{
  return nullptr;
}

// @brief 戻値のデータ型の取得
VpiVarType
CptItem::data_type() const
{
  return VpiVarType::None;
}

// @brief constant function の展開中の印をつける．
void
CptItem::set_in_use() const
{
}

// @brief constant function の展開中の印を消す．
void
CptItem::clear_in_use() const
{
}

// @brief 使用中(constant function として展開中)のチェック
bool
CptItem::is_in_use() const
{
  return false;
}

// @brief specify block item の種類の取得
VpiSpecItemType
CptItem::specitem_type() const
{
  throw std::logic_error{"Not a SPECITEM type"};
}

// @brief specify block path の種類の取得
VpiSpecPathType
CptItem::specpath_type() const
{
  throw std::logic_error{"Not a SPECPATH type"};
}

// @brief ターミナルの要素数の取得
SizeType
CptItem::terminal_num() const
{
  return 0;
}

// @brief ターミナルの取得
const AstExpr*
CptItem::terminal(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief パス記述の取得
const AstPathDecl*
CptItem::path_decl() const
{
  return nullptr;
}

// @brief 条件式の取得
const AstExpr*
CptItem::expr() const
{
  return nullptr;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の要素数の取得
SizeType
CptItem::then_declhead_num() const
{
  return 0;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
const AstDeclHead*
CptItem::then_declhead(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 条件が成り立ったときに生成される item 配列の要素数の取得
SizeType
CptItem::then_item_num() const
{
  return 0;
}

// @brief 条件が成り立ったときに生成される item の取得
const AstItem*
CptItem::then_item(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
SizeType
CptItem::else_declhead_num() const
{
  return 0;
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
const AstDeclHead*
CptItem::else_declhead(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 条件が成り立たなかったときに生成される item 配列の要素数の取得
SizeType
CptItem::else_item_num() const
{
  return 0;
}

// @brief 条件が成り立たなかったときに生成される item の取得
const AstItem*
CptItem::else_item(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief case item のリストの要素数の取得
SizeType
CptItem::caseitem_num() const
{
  return 0;
}

// @brief case item の取得
const AstGenCaseItem*
CptItem::caseitem(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// @brief 繰り返し制御用の変数名の取得
const char*
CptItem::loop_var() const
{
  return nullptr;
}

// @brief 初期化文の右辺の取得
const AstExpr*
CptItem::init_expr() const
{
  return nullptr;
}

// @brief 増加文の右辺の取得
const AstExpr*
CptItem::next_expr() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptDefParamH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptDefParamH::type() const
{
  return DefParam;
}

// @brief defparam の要素数の取得
SizeType
CptDefParamH::defparam_num() const
{
  return mArray.size();
}

// @brief defparam の取得
const AstDefParam*
CptDefParamH::defparam(
  SizeType pos
) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CptDefparam
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptDefParam::file_region() const
{
  return FileRegion{mTopLoc, mExpr->file_region().end_loc()};
}

// @brief 階層ブランチの要素数の取得
SizeType
CptDefParam::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const AstNameBranch*
CptDefParam::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}

// 末尾の名前を返す．
const char*
CptDefParam::name() const
{
  return mName;
}

// 値を返す．
const AstExpr*
CptDefParam::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptContAssignH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptContAssignH::type() const
{
  return ContAssign;
}

// @brief continuous assign の要素数の取得
SizeType
CptContAssignH::contassign_num() const
{
  return mArray.size();
}

// @brief continuous assign の取得
const AstContAssign*
CptContAssignH::contassign(
  SizeType pos
) const
{
  return mArray[pos];
}


//////////////////////////////////////////////////////////////////////
// strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptContAssignHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// delay を返す．
const AstDelay*
CptContAssignHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptContAssignHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const AstDelay*
CptContAssignHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// continuous assign文 のベース実装クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を取り出す．
FileRegion
CptContAssign::file_region() const
{
  return FileRegion{lhs()->file_region(), rhs()->file_region()};
}

// 左辺式を取り出す．
const AstExpr*
CptContAssign::lhs() const
{
  return mLhs;
}

// 右辺式を取り出す．
const AstExpr*
CptContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// initial文/always文に共通なクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptProcess::file_region() const
{
  return FileRegion{mTopLoc, mBody->file_region().end_loc()};
}

// 本体のステートメントを返す．
const AstStmt*
CptProcess::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// initial文 を表すクラス
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptInitial::type() const
{
  return Initial;
}


//////////////////////////////////////////////////////////////////////
// always文 を表すクラス
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptAlways::type() const
{
  return Always;
}


//////////////////////////////////////////////////////////////////////
// function と task の共通の親クラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CptTf::CptTf(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  PtIOHeadArray&& iohead_array,
  PtDeclHeadArray&& declhead_array,
  const AstStmt* stmt
) : mFileRegion{file_region},
    mName{name},
    mAutomatic{automatic},
    mIOHeadArray{std::move(iohead_array)},
    mDeclHeadArray{std::move(declhead_array)},
    mBody{stmt}
{
  int n = 0;
  for ( auto head: mIOHeadArray ) {
    n += head->item_num();
  }
  mIOItemNum = n;
}

// ファイル位置を返す．
FileRegion
CptTf::file_region() const
{
  return mFileRegion;
}

// 名前を返す．
const char*
CptTf::name() const
{
  return mName;
}

// automatic なら true
bool
CptTf::automatic() const
{
  return mAutomatic;
}

// @brief IO宣言の要素数の取得
SizeType
CptTf::ioitem_num() const
{
  return mIOItemNum;
}

// @brief IO宣言ヘッダリストの要素数の取得
SizeType
CptTf::iohead_num() const
{
  return mIOHeadArray.size();
}

// @brief IO宣言ヘッダの取得
const AstIOHead*
CptTf::iohead(
  SizeType pos
) const
{
  return mIOHeadArray[pos];
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptTf::declhead_num() const
{
  return mDeclHeadArray.size();
}

// @brief 宣言ヘッダの取得
const AstDeclHead*
CptTf::declhead(
  SizeType pos
) const
{
  return mDeclHeadArray[pos];
}

// 本体を取り出す．
const AstStmt*
CptTf::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// task を表すノード
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptTask::type() const
{
  return Task;
}


//////////////////////////////////////////////////////////////////////
// function を表すノード
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptFunction::type() const
{
  return Func;
}

// 符号の有無を返す．
bool
CptFunction::is_signed() const
{
  return mSigned;
}

// constant function の展開中の印をつける．
void
CptFunction::set_in_use() const
{
  mInUse = true;
}

// constant function の展開中の印を消す．
void
CptFunction::clear_in_use() const
{
  mInUse = false;
}

// 使用中(constant function として展開中)の時 true を返す．
bool
CptFunction::is_in_use() const
{
  return mInUse;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタタイプの function を表すノード
//////////////////////////////////////////////////////////////////////

// 範囲を得る．
const AstRange*
CptSizedFunc::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
/// 組み込み型の function を表すノード
//////////////////////////////////////////////////////////////////////

// 戻値の型を返す．
VpiVarType
CptTypedFunc::data_type() const
{
  return mDataType;
}


//////////////////////////////////////////////////////////////////////
// PtFactory
//////////////////////////////////////////////////////////////////////

// defparam 文のヘッダを生成する．
PtItem*
PtFactory::new_DefParamH(
  const FileRegion& file_region,
  PtDefParamArray&& elem_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptDefParamH));
  return new (p) CptDefParamH(file_region, std::move(elem_array));
}

// defparam 文の要素を生成する．
PtDefParam*
PtFactory::new_DefParam(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* value
)
{
  void* p = mAlloc.get_memory(sizeof(CptDefParam));
  return new (p) CptDefParam(file_region,
			     PtNameBranchArray(),
			     name, value);
}

PtDefParam*
PtFactory::new_DefParam(
  const FileRegion& file_region,
  PtHierName* hname,
  const AstExpr* value
)
{
  void* p = mAlloc.get_memory(sizeof(CptDefParam));
  auto tail_name = hname->tail_name();
  return new (p) CptDefParam(file_region,
			     hname->name_branch_to_array(mAlloc),
			     tail_name, value);
}

// continuous assign 文のヘッダを生成する．
PtItem*
PtFactory::new_ContAssignH(
  const FileRegion& file_region,
  const AstStrength* strength,
  const AstDelay* delay,
  PtContAssignArray&& elem_array
)
{
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptContAssignH));
      return new (p) CptContAssignH(file_region,
				    std::move(elem_array));
    }
    void* p = mAlloc.get_memory(sizeof(CptContAssignHD));
    return new (p) CptContAssignHD(file_region, delay,
				   std::move(elem_array));
  }
  if ( delay == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptContAssignHS));
    return new (p) CptContAssignHS(file_region, strength,
				   std::move(elem_array));
  }
  void* p = mAlloc.get_memory(sizeof(CptContAssignHSD));
  return new (p) CptContAssignHSD(file_region, strength, delay,
				  std::move(elem_array));
}

// continuous assign 文の要素を生成する．
PtContAssign*
PtFactory::new_ContAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  // 実は file_region は不要
  void* p = mAlloc.get_memory(sizeof(CptContAssign));
  return new (p) CptContAssign(lhs, rhs);
}

// initial 文を生成する．
PtItem*
PtFactory::new_Initial(
  const FileRegion& file_region,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptInitial));
  return new (p) CptInitial(file_region, body);
}

// always 文を生成する．
PtItem*
PtFactory::new_Always(
  const FileRegion& file_region,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptAlways));
  return new (p) CptAlways(file_region, body);
}

// task 文を生成する．
PtItem*
PtFactory::new_Task(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  PtIOHeadArray&& iohead_array,
  PtDeclHeadArray&& declhead_array,
  const AstStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptTask));
  return new (p) CptTask(file_region, name, automatic,
			 std::move(iohead_array),
			 std::move(declhead_array),
			 stmt);
}

// function 文を生成する．
PtItem*
PtFactory::new_Function(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  bool sign,
  PtIOHeadArray&& iohead_array,
  PtDeclHeadArray&& declhead_array,
  const AstStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptFunction));
  return new (p) CptFunction(file_region, name, automatic, sign,
			     std::move(iohead_array),
			     std::move(declhead_array),
			     stmt);
}

// ビットベクタ型 function の生成
PtItem*
PtFactory::new_SizedFunc(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  bool sign,
  const AstRange* range,
  PtIOHeadArray&& iohead_array,
  PtDeclHeadArray&& declhead_array,
  const AstStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptSizedFunc));
  return new (p) CptSizedFunc(file_region,
			      name, automatic,
			      sign, range,
			      std::move(iohead_array),
			      std::move(declhead_array),
			      stmt);
}

// 組み込み型 function の生成
PtItem*
PtFactory::new_TypedFunc(
  const FileRegion& file_region,
  const char* name,
  bool automatic,
  bool sign,
  VpiVarType func_type,
  PtIOHeadArray&& iohead_array,
  PtDeclHeadArray&& declhead_array,
  const AstStmt* stmt
)
{
  void* p = mAlloc.get_memory(sizeof(CptTypedFunc));
  return new (p) CptTypedFunc(file_region, name,
			      automatic, sign,
			      func_type,
			      std::move(iohead_array),
			      std::move(declhead_array),
			      stmt);
}

END_NAMESPACE_YM_VERILOG
