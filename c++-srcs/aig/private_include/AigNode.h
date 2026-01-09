#ifndef AIGNODE_H
#define AIGNODE_H

/// @file AigNode.h
/// @brief AigNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigEdge.h"
#include "FoInfo.h"

#define DEBUG_AIGNODE 0
#define DOUT std::cout


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigNode AigNode.h "AigNode.h"
/// @brief AIG のノードを表すクラス
///
/// 以下の情報を持つ．
/// - ID番号
/// - 参照回数
/// - 入力ノードかANDノードかのフラグ
/// - 入力ノード: 入力番号
/// - ANDノード: 2つのファンインの枝
///
/// 出力やDFFの入力は AigTerminal で表す．
/// AigNode ではない．
//////////////////////////////////////////////////////////////////////
class AigNode
{
  friend class AigMgrImpl; // _inc_ref()/_dec_ref()

public:

  /// @brief 入力ノード用のコンストラクタ
  AigNode(
    SizeType id,       ///< [in] ノード番号
    SizeType input_id, ///< [in] 入力番号
    SizeType type = 0  ///< [in] 種類(0: 外部入力, 1: DFF)
  ) : mId{id},
      mInput{true},
      mFanins{reinterpret_cast<PtrIntType>(input_id), type}
  {
  }

  /// @brief ANDノード用のコンストラクタ
  AigNode(
    SizeType id,    ///< [in] ノード番号
    AigEdge fanin0, ///< [in] ファンイン0
    AigEdge fanin1  ///< [in] ファンイン1
  ) : mId{id},
      mInput{false},
      mFanins{fanin0.mPackedData, fanin1.mPackedData}
  {
  }

  /// @brief デストラクタ
  ~AigNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 基本情報
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 入力ノードの時，true を返す．
  bool
  is_input() const
  {
    return mInput;
  }

  /// @brief 外部入力ノードの時 true を返す．
  bool
  is_primary_input() const
  {
    return is_input() && mFanins[1] == 0;
  }

  /// @brief DFFの出力ノードの時 true を返す．
  bool
  is_dff_output() const
  {
    return is_input() && mFanins[1] == 1;
  }

  /// @brief ANDノードの時，true を返す．
  bool
  is_and() const
  {
    return !is_input();
  }

  /// @brief 参照回数を返す．
  SizeType
  ref_count() const
  {
    return mRefCount;
  }

  /// @brief 内容を表す文字列("Node#ddd")を返す．
  std::string
  string() const
  {
    std::ostringstream buf;
    buf << "Node#" << id() << ": ";
    if ( is_primary_input() ) {
      buf << "Input#" << input_id();
    }
    else if ( is_dff_output() ) {
      buf << "DFF#" << dff_id();
    }
    else { // node->is_and()
      buf << "And("
	  << fanin0()
	  << ", "
	  << fanin1()
	  << ")";
    }
    return buf.str();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ノード用の情報取得関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力番号を返す．
  SizeType
  input_id() const
  {
    _check_primary_input("input_id");
    return static_cast<SizeType>(mFanins[0]);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // DFFの出力ノード用の情報取得関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DFF番号を返す．
  SizeType
  dff_id() const
  {
    _check_dff_output("dff_id");
    return static_cast<SizeType>(mFanins[0]);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // ANDノード用の情報取得関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンインのノードを返す．
  AigNode*
  fanin_node(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    _check_and("fanin_node");
    // 安全のため pos の範囲を補正しておく．
    pos &= 1;
    return fanin(pos).node();
  }

  /// @brief ファンイン0のノードを返す．
  AigNode*
  fanin0_node() const
  {
    _check_and("fanin0_node");
    return fanin0().node();
  }

  /// @brief ファンイン1のノードを返す．
  AigNode*
  fanin1_node() const
  {
    _check_and("fanin1_node");
    return fanin1().node();
  }

  /// @brief ファンインの反転属性を返す．
  bool
  fanin_inv(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    _check_and("fanin_inv");
    // 安全のため pos の範囲を補正しておく．
    pos &= 1;
    return fanin(pos).inv();
  }

  /// @brief ファンイン0の反転属性を返す．
  bool
  fanin0_inv() const
  {
    _check_and("fanin0_inv");
    return fanin0().inv();
  }

  /// @brief ファンイン1の反転属性を返す．
  bool
  fanin1_inv() const
  {
    _check_and("fanin1_inv");
    return fanin1().inv();
  }

  /// @brief ファンインの枝を返す．
  AigEdge
  fanin(
    SizeType pos ///< [in] 位置 ( 0 or 1 )
  ) const
  {
    _check_and("fanin");
    // 安全のため pos の範囲を補正しておく．
    pos &= 1;
    return AigEdge(mFanins[pos]);
  }

  /// @brief ファンイン0の枝を返す．
  AigEdge
  fanin0() const
  {
    _check_and("fanin0");
    return AigEdge(mFanins[0]);
  }

  /// @brief ファンイン0のハンドルを返す．
  AigEdge
  fanin1() const
  {
    _check_and("fanin1");
    return AigEdge(mFanins[1]);
  }

  /// @brief ファンアウト情報のリストを返す．
  const std::vector<FoInfo>&
  fo_info_list() const
  {
    return mFoList;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 参照回数を増やす
  bool
  _inc_ref()
  {
    ++ mRefCount;
    return mRefCount == 1;
  }

  /// @brief 参照回数を減らす
  bool
  _dec_ref()
  {
    -- mRefCount;
    return mRefCount == 0;
  }

  /// @brief 仮死状態にする．
  void
  _deactivate()
  {
    mRefCount = 0;
  }

  /// @brief 入力専用の関数の例外発生
  void
  _check_input(
    const char* name ///< [in] 関数名
  ) const
  {
    if ( !is_input() ) {
      std::ostringstream buf;
      buf << "AigNode::" << name << "() shuold not be used unless is_input() == true";
      throw std::invalid_argument{buf.str()};
    }
  }

  /// @brief 外部入力専用の関数の例外発生
  void
  _check_primary_input(
    const char* name ///< [in] 関数名
  ) const
  {
    if ( !is_primary_input() ) {
      std::ostringstream buf;
      buf << "AigNode::" << name << "() should not be used unless is_primary_input() == true";
      throw std::invalid_argument{buf.str()};
    }
  }

  /// @brief DFF出力専用の関数の例外発生
  void
  _check_dff_output(
    const char* name ///< [in] 関数名
  ) const
  {
    if ( !is_dff_output() ) {
      std::ostringstream buf;
      buf << "AigNode::" << name << "() shold not be used unless is_dff_output() == true";
      throw std::invalid_argument{buf.str()};
    }
  }

  /// @brief AND 専用の関数の例外発生
  void
  _check_and(
    const char* name ///< [in] 関数名
  ) const
  {
    if ( !is_and() ) {
      std::ostringstream buf;
      buf << "AigNode::" << name << "() should not be used unless is_and() == true";
      throw std::invalid_argument{buf.str()};
    }
  }

  /// @brief 入力ノードの内容を設定する．
  void
  _set_input(
    SizeType input_id ///< [in] 入力番号
  )
  {
    mInput = true;
    mFanins[0] = input_id;
    mFanins[1] = 0;
  }

  /// @brief DFFの出力ノードの内容を設定する．
  void
  _set_dff_output(
    SizeType dff_id ///< [in] DFF番号
  )
  {
    mInput = true;
    mFanins[0] = dff_id;
    mFanins[1] = 1;
  }

  /// @brief ANDゲートの内容を設定する．
  void
  _set_and(
    AigEdge fanin0, ///< [in] ファンイン0
    AigEdge fanin1  ///< [in] ファンイン1
  )
  {
    mInput = false;
    mFanins[0] = fanin0.mPackedData;
    mFanins[1] = fanin1.mPackedData;
  }

  /// @brief ファンアウト先の情報を追加する．
  void
  add_fanout(
    AigNode* node
  )
  {
    mFoList.push_back(FoInfo(node));
  }

  /// @brief ファンアウト先の情報を追加する．
  void
  add_fanout(
    AigTerminal* terminal
  )
  {
    mFoList.push_back(FoInfo(terminal));
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId: 62;

  // 入力/ANDの区別
  bool mInput: 1;

  // 参照回数
  SizeType mRefCount{0};

  // ファンインの枝
  PtrIntType mFanins[2];

  // ファンアウト情報のリスト
  std::vector<FoInfo> mFoList;

};

END_NAMESPACE_YM_AIG

#endif // AIGNODE_H
