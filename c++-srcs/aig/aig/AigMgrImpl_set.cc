
/// @file AigMgrImpl_set.cc
/// @brief AigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief 要素数を指定して配列のサイズを設定する．
void
AigMgrImpl::_initialize(
  SizeType node_num,   ///< [in] ノード数
  SizeType input_num,  ///< [in] 入力数
  SizeType output_num, ///< [in] 出力数
  SizeType dff_num     ///< [in] DFF数
)
{
  // ノード関連の初期化
  mNodeArray.clear();
  mAndTable.clear();
  mNodeArray.resize(node_num);
  for ( SizeType id = 0; id < node_num; ++ id ) {
    auto node = new AigNode(id, AigEdge::zero(), AigEdge::zero());
    mNodeArray[id] = std::unique_ptr<AigNode>{node};
  }

  // 入力リストの初期化
  mInputList.clear();
  mInputList.resize(input_num);

  // 出力リストの初期化
  // 空の枝を持つ出力を作る．
  mOutputList.clear();
  mOutputList.reserve(output_num);
  for ( SizeType oid = 0; oid < output_num; ++ oid ) {
    auto output = AigTerminal::output(oid);
    mOutputList.push_back({std::unique_ptr<AigTerminal>{output}, {}});
  }

  // DFFリストの初期化
  // 未接続のDFFを作る．
  mDffList.clear();
  mDffList.reserve(dff_num);
  for ( SizeType dff_id = 0; dff_id < dff_num; ++ dff_id ) {
    auto input = AigTerminal::dff_input(dff_id);
    mDffList.push_back(
      {nullptr, std::unique_ptr<AigTerminal>{input}, {}}
    );
  }
}

// @brief 参照回数を適正化する．
void
AigMgrImpl::_finalize()
{
  std::vector<std::string> err_list;
  if ( !_sanity_check(err_list) ) {
    for ( auto& err_msg: err_list ) {
      DOUT << err_msg << std::endl;
    }
    throw std::logic_error{"insane"};
  }

  // 参照回数を0に初期化する．
  for ( auto& node: mNodeArray ) {
    node->mRefCount = 0;
  }

  // 出力側から参照回数の計算を行う．
  for ( auto& output: mOutputList ) {
    _inc_node_ref(output.terminal.get());
  }
  for ( auto& dff: mDffList ) {
    _inc_node_ref(dff.input.get());
  }

  _fo_check();
}

END_NAMESPACE_YM_AIG
