#ifndef CPTITEM_TASKFUNC_H
#define CPTITEM_TASKFUNC_H

/// @file CptItem_TaskFunc.h
/// @brief CptItem_TaskFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtIOHead.h"
#include "parser/PtIOItem.h"
#include "ym/vl/AstIOItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptItem_TaskFunc CptItem_TaskFunc.h "CptItem_TaskFunc.h"
/// @brief task と function に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class CptItem_TaskFunc :
  public CptItem
{
protected:

  /// @brief コンストラクタ
  CptItem_TaskFunc(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    const PtIOHead* iohead_top,
    const PtDeclHead* declhead_top,
    const PtStmt* stmt
  ) : mFileRegion{file_region},
      mName{name},
      mAutomatic{automatic},
      mIOHeadTop{iohead_top},
      mDeclHeadTop{declhead_top},
      mBody{stmt}
  {
    int n = 0;
    for ( auto head = iohead_top; head != nullptr; head = head->link() ) {
      n += AstIOItemList(AstIOItem(head->item_top())).size();
    }
    mIOItemNum = n;
  }

  /// @brief デストラクタ
  ~CptItem_TaskFunc() {}


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

  /// @brief IO宣言ヘッダリストの先頭の取得
  const PtIOHead*
  iohead_top() const override;

  /// @brief IO宣言の要素数の取得
  SizeType
  ioitem_num() const override;

  /// @brief 宣言ヘッダリストの先頭の取得
  const PtDeclHead*
  declhead_top() const override;

  /// @brief 本体を取り出す．
  const PtStmt*
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
  const PtIOHead* mIOHeadTop;

  // その他の宣言の先頭
  const PtDeclHead* mDeclHeadTop;

  // 本体
  const PtStmt* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_Task CptItem_TaskFunc.h "CptItem_TaskFunc.h"
/// @brief task を表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_Task :
  public CptItem_TaskFunc
{
public:

  /// @brief コンストラクタ
  CptItem_Task(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    const PtIOHead* iohead_top,
    const PtDeclHead* declhead_top,
    const PtStmt* stmt
  ) : CptItem_TaskFunc(file_region, name, automatic,
	    iohead_top, declhead_top,
	    stmt)
  {
  }

  /// @brief デストラクタ
  ~CptItem_Task() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_Function CptItem_TaskFunc.h "CptItem_TaskFunc.h"
/// @brief function を表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_Function :
  public CptItem_TaskFunc
{
public:

  /// @brief コンストラクタ
  CptItem_Function(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    bool sign,
    const PtIOHead* iohead_top,
    const PtDeclHead* declhead_top,
    const PtStmt* stmt
  ) : CptItem_TaskFunc(file_region, name, automatic,
	    iohead_top, declhead_top,
	    stmt),
      mSigned{sign}
  {
    mInUse = false;
  }

  /// @brief デストラクタ
  ~CptItem_Function() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  AstItem::Type
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 符号の有無
  bool
  is_signed() const override;

  /// @brief 範囲の取得
  /// @return 範囲
  const PtRange*
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
/// @class CptItem_SizedFunc CptItem_TaskFunc.h "CptItem_TaskFunc.h"
/// @brief ビットベクタタイプの function を表すノード
//////////////////////////////////////////////////////////////////////
class CptItem_SizedFunc :
  public CptItem_Function
{
public:

  /// @brief コンストラクタ
  CptItem_SizedFunc(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    bool sign,
    const PtRange* range,
    const PtIOHead* iohead_top,
    const PtDeclHead* declhead_top,
    const PtStmt* stmt
  ) : CptItem_Function(file_region, name, automatic, sign,
		       iohead_top, declhead_top,
		       stmt),
      mRange{range}
  {
  }

  /// @brief デストラクタ
  ~CptItem_SizedFunc() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtTf の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲を得る．
  const PtRange*
  range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const PtRange* mRange;

};


//////////////////////////////////////////////////////////////////////
/// @class CptItem_TypedFunc CptItem_TaskFunc.h "CptItem_TaskFunc.h"
/// @brief 組み込み型の function を表すノード
//////////////////////////////////////////////////////////////////////
class CptItem_TypedFunc :
  public CptItem_Function
{
public:

  /// @brief コンストラクタ
  CptItem_TypedFunc(
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    bool sign,
    VpiVarType data_type,
    const PtIOHead* iohead_top,
    const PtDeclHead* declhead_top,
    const PtStmt* stmt
  ) : CptItem_Function(file_region, name, automatic, sign,
		       iohead_top, declhead_top,
		       stmt),
      mDataType{data_type}
  {
  }

  /// @brief デストラクタ
  ~CptItem_TypedFunc() {}


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

#endif // CPTITEM_TASKFUNC_H
