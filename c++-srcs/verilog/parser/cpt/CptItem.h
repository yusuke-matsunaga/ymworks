#ifndef CPTITEM_H
#define CPTITEM_H

/// @file CptItem.h
/// @brief CptItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtItem.h"
#include "parser/PtArray.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// item の基底クラス
//////////////////////////////////////////////////////////////////////
class CptItem :
  public PtItem
{
protected:

  /// @brief コンストラクタ
  CptItem() {}

  /// @brief デストラクタ
  ~CptItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief プリミティブタイプの取得
  /// @return プリミティブタイプ
  VpiPrimType
  prim_type() const override;

  /// @brief strength の取得
  /// @return 信号強度
  const AstStrength*
  strength() const override;

  /// @brief delay の取得
  /// @return 遅延
  const AstDelay*
  delay() const override;

  /// @brief パラメータ割り当て数の取得
  SizeType
  paramassign_num() const override;

  /// @brief パラメータ割り当ての取得
  const AstConnection*
  paramassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < paramassign_num() )
  ) const override;

  /// @brief defparam の要素数の取得
  SizeType
  defparam_num() const override;

  /// @brief defparam の取得
  const AstDefParam*
  defparam(
    SizeType pos ///< [in] 位置 ( 0 <= pos < defparam_num() )
  ) const override;

  /// @brief continuous assign の要素数の取得
  SizeType
  contassign_num() const override;

  /// @brief continuous assign の取得
  const AstContAssign*
  contassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < contassign_num() )
  ) const override;

  /// @brief module/UDP/gate instance の要素数の取得
  SizeType
  inst_num() const override;

  /// @brief module/UDP/gate instance の取得
  const AstInst*
  inst(
    SizeType pos ///< [in] 位置 ( 0 <= pos < inst_num() )
  ) const override;

  /// @brief automatic 情報の取得
  /// @retval true automatic 宣言された task/function
  /// @retval false 上記以外
  bool
  automatic() const override;

  /// @brief 名前の取得
  /// @return 名前
  const char*
  name() const override;

  /// @brief IO宣言の要素数の取得
  SizeType
  ioitem_num() const override;

  /// @brief IO宣言ヘッダリストの要素数の取得
  SizeType
  iohead_num() const override;

  /// @brief IO宣言ヘッダの取得
  const AstIOHead*
  iohead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const AstDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const AstItem*
  item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < item_num() )
  ) const override;

  /// @brief 本体のステートメントの取得
  /// @return 本体のステートメント
  const AstStmt*
  body() const override;

  /// @brief 符号の取得
  /// @retval true 符号つき
  /// @retval false 符号なし
  bool
  is_signed() const override;

  /// @brief 範囲の取得
  /// @return 範囲
  const AstRange*
  range() const override;

  /// @brief 戻値のデータ型の取得
  /// @return 戻値のデータ型
  VpiVarType
  data_type() const override;

  /// @brief constant function の展開中の印をつける．
  void
  set_in_use() const override;

  /// @brief constant function の展開中の印を消す．
  void
  clear_in_use() const override;

  /// @brief 使用中(constant function として展開中)のチェック
  /// @return 使用中の時 true を返す．
  bool
  is_in_use() const override;

  /// @brief specify block item の種類の取得
  /// @return specify block item の種類
  VpiSpecItemType
  specitem_type() const override;

  /// @brief specify block path の種類の取得
  /// @return specify block path の種類
  VpiSpecPathType
  specpath_type() const override;

  /// @brief ターミナルの要素数の取得
  SizeType
  terminal_num() const override;

  /// @brief ターミナルの取得
  const AstExpr*
  terminal(
    SizeType pos ///< [in] 位置 ( 0 <= pos < terminal_num() )
  ) const override;

  /// @brief パス記述の取得
  /// @return パス記述
  const AstPathDecl*
  path_decl() const override;

  /// @brief 条件式の取得
  /// @return 条件式
  const AstExpr*
  expr() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダ配列の要素数の取得
  SizeType
  then_declhead_num() const override;

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
  const AstDeclHead*
  then_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < then_declhead_num() )
  ) const override;

  /// @brief 条件が成り立ったときに生成される item 配列の要素数の取得
  SizeType
  then_item_num() const override;

  /// @brief 条件が成り立ったときに生成される item の取得
  const AstItem*
  then_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < then_item_num() )
  ) const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
  SizeType
  else_declhead_num() const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
  const AstDeclHead*
  else_declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < else_declhead_num() )
  ) const override;

  /// @brief 条件が成り立たなかったときに生成される item 配列の要素数の取得
  SizeType
  else_item_num() const override;

  /// @brief 条件が成り立たなかったときに生成される item の取得
  const AstItem*
  else_item(
    SizeType pos ///< [in] 位置 ( 0 <= pos < else_item_num() )
  ) const override;

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  const AstGenCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置 ( 0 <= pos < caseitem_num() )
  ) const override;

  /// @brief 繰り返し制御用の変数名の取得
  /// @return 繰り返し制御用の変数名
  const char*
  loop_var() const override;

  /// @brief 初期化文の右辺の取得
  /// @return 初期化文の右辺
  const AstExpr*
  init_expr() const override;

  /// @brief 増加文の右辺の取得
  /// @return 増加文の右辺
  const AstExpr*
  next_expr() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief defparam 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptDefParamH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptDefParamH(
    const FileRegion& file_region,
    PtDefParamArray&& dp_array
  ) : mFileRegion{file_region},
      mArray{std::move(dp_array)}
  {
  }

  /// @brief デストラクタ
  ~CptDefParamH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief defparam の要素数の取得
  SizeType
  defparam_num() const override;

  /// @brief defparam の取得
  const AstDefParam*
  defparam(
    SizeType pos ///< [in] 位置 ( 0 <= pos < defparam_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の配列
  PtDefParamArray mArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief defparam 文を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDefParam :
  public PtDefParam
{
public:

  /// @brief コンストラクタ
  CptDefParam(
    const FileRegion& file_region,
    PtNameBranchArray&& nb_array,
    const char* tail_name,
    const AstExpr* value
  ) : mTopLoc{file_region.start_loc()},
      mNbArray{std::move(nb_array)},
      mName{tail_name},
      mExpr{value}
  {
    if ( value == nullptr ) {
      throw std::logic_error{"value = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptDefParam() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 階層ブランチの要素数の取得
  SizeType
  namebranch_num() const override;

  /// @brief 階層ブランチの取得
  const AstNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const override;

  /// @brief 末尾の名前の取得
  /// @return 末尾の名前
  const char*
  name() const override;

  /// @brief 値の取得
  /// @return 値を表す式
  const AstExpr*
  expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  FileLoc mTopLoc;

  // 階層ブランチのリスト
  PtNameBranchArray mNbArray;

  // 末尾の名前
  const char* mName;

  // 値
  const AstExpr* mExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptContAssignH(
    const FileRegion& file_region,
    PtContAssignArray&& ca_array
  ) : mFileRegion{file_region},
      mArray{std::move(ca_array)}
  {
  }

  /// @brief デストラクタ
  ~CptContAssignH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief continuous assign の要素数の取得
  SizeType
  contassign_num() const override;

  /// @brief continuous assign の取得
  const AstContAssign*
  contassign(
    SizeType pos ///< [in] 位置 ( 0 <= pos < contassign_num() )
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 要素の配列
  PtContAssignArray mArray;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHS :
  public CptContAssignH
{
public:

  /// @brief コンストラクタ
  CptContAssignHS(
    const FileRegion& file_region,
    const AstStrength* strength,
    PtContAssignArray&& ca_array
  ) : CptContAssignH(file_region, std::move(ca_array)),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptContAssignHS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const AstStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const AstStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHD :
  public CptContAssignH
{
public:

  /// @brief コンストラクタ
  CptContAssignHD(
    const FileRegion& file_region,
    const AstDelay* delay,
    PtContAssignArray&& ca_array
  ) : CptContAssignH(file_region, std::move(ca_array)),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptContAssignHD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  const AstDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const AstDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////
class CptContAssignHSD :
  public CptContAssignH
{
public:

  /// @brief コンストラクタ
  CptContAssignHSD(
    const FileRegion& file_region,
    const AstStrength* strength,
    const AstDelay* delay,
    PtContAssignArray&& ca_array
  ) : CptContAssignH(file_region, std::move(ca_array)),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptContAssignHSD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssignH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const AstStrength*
  strength() const override;

  /// @brief delay を返す．
  const AstDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const AstStrength* mStrength;

  // delay
  const AstDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief continuous assign 文
//////////////////////////////////////////////////////////////////////
class CptContAssign :
  public PtContAssign
{
public:

  /// @brief コンストラクタ
  CptContAssign(
    const AstExpr* lhs,
    const AstExpr* rhs
  ) : mLhs{lhs},
      mRhs{rhs}
  {
  }

  /// @brief デストラクタ
  ~CptContAssign() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  FileRegion
  file_region() const override;

  /// @brief 左辺式を取り出す．
  const AstExpr*
  lhs() const override;

  /// @brief 右辺式を取り出す．
  const AstExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺式
  const AstExpr* mLhs;

  // 右辺式
  const AstExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @brief inititial 文/always 文に共通なクラス
//////////////////////////////////////////////////////////////////////
class CptProcess :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptProcess(
    const FileRegion& file_region,
    const AstStmt* body
  ) : mTopLoc{file_region.start_loc()},
      mBody{body}
  {
    if ( body == nullptr ) {
      throw std::logic_error{"body = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptProcess() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を取り出す．
  FileRegion
  file_region() const override;

  /// @brief 本体のステートメントを返す．
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭のファイル位置
  FileLoc mTopLoc;

  // 本体
  const AstStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief inititial 文
//////////////////////////////////////////////////////////////////////
class CptInitial :
  public CptProcess
{
public:

  /// @brief コンストラクタ
  CptInitial(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptProcess(file_region, body)
  {
  }

  /// @brief デストラクタ
  ~CptInitial() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief always 文
//////////////////////////////////////////////////////////////////////
class CptAlways :
  public CptProcess
{
public:

  /// @brief コンストラクタ
  CptAlways(
    const FileRegion& file_region,
    const AstStmt* body
  ) : CptProcess(file_region, body)
  {
  }

  /// @brief デストラクタ
  ~CptAlways() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief task と function に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class CptTf :
  public CptItem
{
protected:

  /// @brief コンストラクタ
  CptTf(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    PtIOHeadArray&& iohead_array,
    PtDeclHeadArray&& declhead_array,
    const AstStmt* stmt
  );

  /// @brief デストラクタ
  ~CptTf() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief automatic なら true
  bool
  automatic() const override;

  /// @brief IO宣言の要素数の取得
  SizeType
  ioitem_num() const override;

  /// @brief IO宣言ヘッダリストの要素数の取得
  SizeType
  iohead_num() const override;

  /// @brief IO宣言ヘッダの取得
  const AstIOHead*
  iohead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < iohead_num() )
  ) const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const AstDeclHead*
  declhead(
    SizeType pos ///< [in] 位置 ( 0 <= pos < declhead_num() )
  ) const override;

  /// @brief 本体を取り出す．
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 名前
  const char* mName;

  // automatic 属性
  bool mAutomatic;

  // IO宣言の要素数
  int mIOItemNum;

  // IO宣言の配列
  PtIOHeadArray mIOHeadArray;

  // その他の宣言の配列
  PtDeclHeadArray mDeclHeadArray;

  // 本体
  const AstStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief task を表すクラス
//////////////////////////////////////////////////////////////////////
class CptTask :
  public CptTf
{
public:

  /// @brief コンストラクタ
  CptTask(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    PtIOHeadArray&& iohead_array,
    PtDeclHeadArray&& declhead_array,
    const AstStmt* stmt
  ) : CptTf(file_region,
	    name, automatic,
	    std::move(iohead_array),
	    std::move(declhead_array),
	    stmt)
  {
  }

  /// @brief デストラクタ
  ~CptTask() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief function を表すクラス
//////////////////////////////////////////////////////////////////////
class CptFunction :
  public CptTf
{
public:

  /// @brief コンストラクタ
  CptFunction(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    bool sign,
    PtIOHeadArray&& iohead_array,
    PtDeclHeadArray&& declhead_array,
    const AstStmt* stmt
  ) : CptTf(file_region,
	    name, automatic,
	    std::move(iohead_array),
	    std::move(declhead_array),
	    stmt),
      mSigned{sign}
  {
    mInUse = false;
  }

  /// @brief デストラクタ
  ~CptFunction() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtTf の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無
  bool
  is_signed() const override;

  /// @brief constant function の展開中の印をつける．
  void
  set_in_use() const override;

  /// @brief constant function の展開中の印を消す．
  void
  clear_in_use() const override;

  /// @brief 使用中(constant function として展開中)の時 true を返す．
  bool
  is_in_use() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 符号の有無
  bool mSigned;

  // 使用中かどうかを示すフラグ
  mutable bool mInUse;

};


//////////////////////////////////////////////////////////////////////
/// @brief ビットベクタタイプの function を表すノード
//////////////////////////////////////////////////////////////////////
class CptSizedFunc :
  public CptFunction
{
public:

  /// @brief コンストラクタ
  CptSizedFunc(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    bool sign,
    const AstRange* range,
    PtIOHeadArray&& iohead_array,
    PtDeclHeadArray&& declhead_array,
    const AstStmt* stmt
  ) : CptFunction(file_region,
		  name, automatic, sign,
		  std::move(iohead_array),
		  std::move(declhead_array),
		  stmt),
      mRange{range}
  {
  }

  /// @brief デストラクタ
  ~CptSizedFunc() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtTf の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲を得る．
  const AstRange*
  range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const AstRange* mRange;

};


//////////////////////////////////////////////////////////////////////
/// @brief 組み込み型の function を表すノード
//////////////////////////////////////////////////////////////////////
class CptTypedFunc :
  public CptFunction
{
public:

  /// @brief コンストラクタ
  CptTypedFunc(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    bool sign,
    VpiVarType data_type,
    PtIOHeadArray&& iohead_array,
    PtDeclHeadArray&& declhead_array,
    const AstStmt* stmt
  ) : CptFunction(file_region,
		  name, automatic, sign,
		  std::move(iohead_array),
		  std::move(declhead_array),
		  stmt),
      mDataType{data_type}
  {
  }

  /// @brief デストラクタ
  ~CptTypedFunc() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 戻値の型を返す．
  VpiVarType
  data_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 戻値の型
  VpiVarType mDataType;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_H
