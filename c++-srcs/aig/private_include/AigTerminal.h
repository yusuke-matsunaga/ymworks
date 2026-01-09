#ifndef AIGTERMINAL_H
#define AIGTERMINAL_H

/// @file AigTerminal.h
/// @brief AigTerminal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigEdge.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigTerminal AigTerminal.h "AigTerminal.h"
/// @brief 端子を表すクラス
///
/// - 出力端子
/// - DFFの入力端子
/// の2種類の端子を表す．
///
/// 実体は AigEdgeだが 以下の補助情報も持つ．
/// - 出力番号(外部出力の場合)
/// - DFF番号(DFFの入力端子の場合)
//////////////////////////////////////////////////////////////////////
class AigTerminal
{
  friend class AigMgrImpl;

public:

  /// @brief コンストラクタ
  ///
  /// 不正な値となる．
  AigTerminal() = default;

  /// @brief 外部出力を作るクラスメソッド
  static
  AigTerminal*
  output(
    SizeType output_id,            ///< [in] 出力番号
    AigEdge edge = AigEdge::zero() ///< [in] 枝
  )
  {
    return new AigTerminal(output_id, edge, false);
  }

  /// @brief DFFのデータ入力を作るクラスメソッド
  static
  AigTerminal*
  dff_input(
    SizeType dff_id,		   ///< [in] DFF番号
    AigEdge edge = AigEdge::zero() ///< [in] 枝
  )
  {
    return new AigTerminal(dff_id, edge, true);
  }

  /// @brief デストラクタ
  ~AigTerminal() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝を返す．
  AigEdge
  edge() const
  {
    return mEdge;
  }

  /// @brief 外部出力の時 true を返す．
  bool
  is_output() const
  {
    return !is_dff_input();
  }

  /// @brief DFFのデータ入力の時 true を返す．
  bool
  is_dff_input() const
  {
    return mDff;
  }

  /// @brief 内容を表す文字列を返す．
  std::string
  string() const
  {
    std::ostringstream buf;
    if ( is_output() ) {
      buf << "Output#" << output_id();
    }
    else {
      buf << "DFF#" << dff_id();
    }
    return buf.str();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部出力用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力番号を返す．
  ///
  /// is_output() == false のときはエラーとなる．
  SizeType
  output_id() const
  {
    if ( !is_output() ) {
      throw std::invalid_argument{"output_id() is valid only if is_output() == true"};
    }
    return mId;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // DFFの入力用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DFF 番号を返す．
  ///
  /// is_dff_input() == false のときはエラーとなる．
  SizeType
  dff_id() const
  {
    if ( !is_dff_input() ) {
      throw std::invalid_argument{"dff_id() is valid only if is_dff_input() == true"};
    }
    return mId;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  AigTerminal(
    SizeType id,
    AigEdge edge,
    bool dff
  ) : mId{id},
      mDff{dff},
      mEdge{edge}
  {
  }

  /// @brief 枝をセットする．
  void
  _set_edge(
    AigEdge edge
  )
  {
    mEdge = edge;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 番号
  SizeType mId : 63;

  // 出力/DFF入力の区別
  bool mDff;

  // 枝
  AigEdge mEdge;

};

END_NAMESPACE_YM_AIG

#endif // AIGTERMINAL_H
