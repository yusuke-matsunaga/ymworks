#ifndef CPTCONTROL_H
#define CPTCONTROL_H

/// @file CptControl.h
/// @brief CptControl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtMisc.h"
#include "ym/FileRegion.h"
#include "parser/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief delay control/event control/repeat control を表すベースクラス
//////////////////////////////////////////////////////////////////////
class CptControl :
  public PtControl
{
protected:

  /// @brief コンストラクタ
  CptControl() {}

  /// @brief デストラクタ
  ~CptControl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 遅延式の取得
  /// @retval 遅延を表す式 delay control の場合
  /// @retval nullptr 上記以外
  const AstExpr*
  delay() const override;

  /// @brief イベントリストの要素数の取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  SizeType
  event_num() const override;

  /// @brief イベントの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  const AstExpr*
  event(
    SizeType index ///< [in] インデックス ( 0 <= index < event_num() )
  ) const override;

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  AstExprVec
  event_list() const override;

  /// @brief 繰り返し数の取得
  /// @retval 繰り返し数を表す式 repeat control の場合
  /// @retval nullptr 上記以外
  const AstExpr*
  rep_expr() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay control を表すクラス
//////////////////////////////////////////////////////////////////////
class CptDelayControl :
  public CptControl
{
public:

  /// @brief コンストラクタ
  CptDelayControl(
    const FileRegion& file_region,
    const AstExpr* value
  ) : mTopLoc{file_region.start_loc()},
      mDelay{value}
  {
    if ( value == nullptr ) {
      throw std::logic_error{"value = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptDelayControl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief 遅延式を返す．
  const AstExpr*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  FileLoc mTopLoc;

  // 遅延を表す式
  const AstExpr* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief event control の基底クラス
///
/// イベントリストを持たない．
//////////////////////////////////////////////////////////////////////
class CptEventControl :
  public CptControl
{
public:

  /// @brief コンストラクタ
  CptEventControl(
    const FileRegion& file_region
  ) : mFileRegion{file_region}
  {
  }

  /// @brief デストラクタ
  ~CptEventControl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief イベントリストの要素数の取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  SizeType
  event_num() const override;

  /// @brief イベントの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  const AstExpr*
  event(
    SizeType index ///< [in] インデックス ( 0 <= index < event_num() )
  ) const override;

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  AstExprVec
  event_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
/// @brief 1つのイベントを持つ EventControl
//////////////////////////////////////////////////////////////////////
class CptEventControl1 :
  public CptEventControl
{
public:

  /// @brief コンストラクタ
  CptEventControl1(
    const FileRegion& file_region,
    const AstExpr* event
  ) : CptEventControl(file_region),
      mEvent{event}
  {
  }

  /// @brief デストラクタ
  ~CptEventControl1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベントリストの要素数の取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  SizeType
  event_num() const override;

  /// @brief イベントの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  const AstExpr*
  event(
    SizeType index ///< [in] インデックス ( 0 <= index < event_num() )
  ) const override;

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  AstExprVec
  event_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベント
  const AstExpr* mEvent;

};


//////////////////////////////////////////////////////////////////////
/// @brief イベントリストを持つ EventControl
//////////////////////////////////////////////////////////////////////
class CptEventControl2 :
  public CptEventControl
{
public:

  /// @brief コンストラクタ
  CptEventControl2(
    const FileRegion& file_region,
    const AstExprList* event_list
  ) : CptEventControl(file_region),
      mEventList{event_list}
  {
  }

  /// @brief デストラクタ
  ~CptEventControl2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベントリストの要素数の取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  SizeType
  event_num() const override;

  /// @brief イベントの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  const AstExpr*
  event(
    SizeType index ///< [in] インデックス ( 0 <= index < event_num() )
  ) const override;

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  AstExprVec
  event_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントのリスト
  const AstExprList* mEventList;

};


//////////////////////////////////////////////////////////////////////
/// @brief repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRepeatControl :
  public CptEventControl
{
public:

  /// @brief コンストラクタ
  CptRepeatControl(
    const FileRegion& file_region,
    const AstExpr* rep
  ) : CptEventControl(file_region),
      mRepExpr{rep}
  {
    if ( rep == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptRepeatControl() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief 繰り返し数を得る．
  const AstExpr*
  rep_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数を表す式
  const AstExpr* mRepExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRepeatControl1 :
  public CptEventControl1
{
public:

  /// @brief コンストラクタ
  CptRepeatControl1(
    const FileRegion& file_region,
    const AstExpr* rep,
    const AstExpr* event
  ) : CptEventControl1(file_region, event),
      mRepExpr{rep}
  {
    if ( rep == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptRepeatControl1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief 繰り返し数を得る．
  const AstExpr*
  rep_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数を表す式
  const AstExpr* mRepExpr;

};


//////////////////////////////////////////////////////////////////////
/// @brief repeat 形式の event を表すクラス
//////////////////////////////////////////////////////////////////////
class CptRepeatControl2 :
  public CptEventControl2
{
public:

  /// @brief コンストラクタ
  CptRepeatControl2(
    const FileRegion& file_region,
    const AstExpr* rep,
    const AstExprList* event_list
  ) : CptEventControl2(file_region, event_list),
      mRepExpr{rep}
  {
    if ( rep == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptRepeatControl2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief 繰り返し数を得る．
  const AstExpr*
  rep_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数を表す式
  const AstExpr* mRepExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTCONTROL_H
