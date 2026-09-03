#ifndef CPTCONTROL_H
#define CPTCONTROL_H

/// @file CptControl.h
/// @brief CptControl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtControl.h"
#include "parser/PtExpr.h"
#include "ym/FileRegion.h"


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
  const PtExpr*
  delay() const override;

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  const PtExpr*
  event_top() const override;

  /// @brief 繰り返し数の取得
  /// @retval 繰り返し数を表す式 repeat control の場合
  /// @retval nullptr 上記以外
  const PtExpr*
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
    const PtExpr* value
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
  AstControl::Type
  type() const override;

  /// @brief 遅延式を返す．
  const PtExpr*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の位置
  FileLoc mTopLoc;

  // 遅延を表す式
  const PtExpr* mDelay;

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
  AstControl::Type
  type() const override;

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  const PtExpr*
  event_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
/// @brief イベントリストを持つ EventControl
//////////////////////////////////////////////////////////////////////
class CptEventControl1 :
  public CptEventControl
{
public:

  /// @brief コンストラクタ
  CptEventControl1(
    const FileRegion& file_region,
    const PtExpr* event_top
  ) : CptEventControl(file_region),
      mEventTop{event_top}
  {
  }

  /// @brief デストラクタ
  ~CptEventControl1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtControl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イベントリストの取得
  ///
  /// - type() == Delay の時 std::logic_error 例外を送出する．
  const PtExpr*
  event_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントの先頭
  const PtExpr* mEventTop;

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
    const PtExpr* rep
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
  AstControl::Type
  type() const override;

  /// @brief 繰り返し数を得る．
  const PtExpr*
  rep_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数を表す式
  const PtExpr* mRepExpr;

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
    const PtExpr* rep,
    const PtExpr* event_top
  ) : CptEventControl1(file_region, event_top),
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
  AstControl::Type
  type() const override;

  /// @brief 繰り返し数を得る．
  const PtExpr*
  rep_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 繰り返し数を表す式
  const PtExpr* mRepExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTCONTROL_H
