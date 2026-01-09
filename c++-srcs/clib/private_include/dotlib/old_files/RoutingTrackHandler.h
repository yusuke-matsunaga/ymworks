#ifndef ROUTINGTRACKHANDLER_H
#define ROUTINGTRACKHANDLER_H

/// @file RoutingTrackHandler.h
/// @brief RoutingTrackHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Str1GroupHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class RoutingTrackHandler RoutingTrackHandler.h "RoutingTrackHandler.h"
/// @brief 'routing_track' Group Statement 用のハンドラ
///
/// 参考文献 : Library Compiler Reference Manual, Section 2
///           "cell and model Group Description and Syntax"
/// * ヘッダ名は routing_layer_name を表す．
/// * Simple Attributes
///   - tracks : integer ;
///   - total_track_area : float ;
/// * Complex Attributes
///   - short ? // 今は無視
//////////////////////////////////////////////////////////////////////
class RoutingTrackHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  RoutingTrackHandler(DotlibParser& parser);

  /// @brief デストラクタ
  ~RoutingTrackHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @breif 'routing_track' Group Statement の記述をパースする．
  /// @param[in] dst_list 読み込んだ値を格納するリスト
  /// @retval true 正しく読み込んだ．
  /// @retval false エラーが起きた．
  bool
  parse_value(vector<const AstRoutingTrack*>& dst_list);


protected:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ記述の始まり
  void
  begin_group() override;

  /// @brief グループ記述の終わり
  /// @param[in] group_loc グループ全体のファイル上の位置
  /// @retval true 正常にパーズした．
  /// @retval false パーズ中にエラーが起こった．
  bool
  end_group(const FileRegion& group_loc) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // tracks
  const AstInt* mTracks;

  // total_track_area
  const AstFloag* mTotalTrackArea;

};

END_NAMESPACE_YM_DOTLIB

#endif // ROUTINGTRACKHANDLER_H
