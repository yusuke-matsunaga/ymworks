#ifndef CPTTASKFUNC_H
#define CPTTASKFUNC_H

/// @file CptTaskFunc.h
/// @brief CptTaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptTf CptTaskFunc.h "CptTaskFunc.h"
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
    const AstIOHead* iohead_top,
    const AstDeclHead* declhead_top,
    const AstStmt* stmt
  ) : mFileRegion{file_region},
      mName{name},
      mAutomatic{automatic},
      mIOHeadTop{iohead_top},
      mDeclHeadTop{declhead_top},
      mBody{stmt}
  {
    int n = 0;
    for ( auto head: AstIOHeadList(mIOHeadTop) ) {
      n += head->item_list().size();
    }
    mIOItemNum = n;
  }

  /// @brief デストラクタ
  ~CptTf() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
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

  /// @brief IO宣言ヘッダリストの取得
  AstIOHeadList
  iohead_list() const override;

  /// @brief IO宣言の要素数の取得
  SizeType
  ioitem_num() const override;

  /// @brief 宣言ヘッダリストの取得
  AstDeclHeadList
  declhead_list() const override;

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

  // IO宣言の先頭
  const AstIOHead* mIOHeadTop;

  // その他の宣言の先頭
  const AstDeclHead* mDeclHeadTop;

  // 本体
  const AstStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class CptTask CptTaskFunc.h "CptTaskFunc.h"
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
    const AstIOHead* iohead_top,
    const AstDeclHead* declhead_top,
    const AstStmt* stmt
  ) : CptTf(file_region, name, automatic,
	    iohead_top, declhead_top,
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
/// @class CptFunction CptTaskFunc.h "CptTaskFunc.h"
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
    const AstIOHead* iohead_top,
    const AstDeclHead* declhead_top,
    const AstStmt* stmt
  ) : CptTf(file_region, name, automatic,
	    iohead_top, declhead_top,
	    stmt),
      mSigned{sign}
  {
    mInUse = false;
  }

  /// @brief デストラクタ
  ~CptFunction() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無
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
/// @class CptSizedFunc CptTaskFunc.h "CptTaskFunc.h"
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
    const AstIOHead* iohead_top,
    const AstDeclHead* declhead_top,
    const AstStmt* stmt
  ) : CptFunction(file_region, name, automatic, sign,
		  iohead_top, declhead_top,
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
/// @class CptTypedFunc CptTaskFunc.h "CptTaskFunc.h"
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
    const AstIOHead* iohead_top,
    const AstDeclHead* declhead_top,
    const AstStmt* stmt
  ) : CptFunction(file_region, name, automatic, sign,
		  iohead_top, declhead_top,
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

#endif // CPTTASKFUNC_H
