#ifndef FENCE_H
#define FENCE_H

/// @file Fence.h
/// @brief Fence のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class Fence Fence.h "Fence.h"
/// @brief fench の情報を表すクラス
///
/// fench は各レベルのノード数を持つ．
/// それ以外に各レベルの先頭のノード番号も持つ．
/// 本当は入力がレベル0で最初のノードのレベルは1だが
/// 入力のレベルは不要なので最初のノードのレベルを0としている．
//////////////////////////////////////////////////////////////////////
class Fence
{
public:

  /// @brief 空のコンストラクタ
  Fence() = default;

  /// @brief コンストラクタ
  Fence(
    const std::vector<SizeType>& num_array ///< [in] 各レベルのノード数の配列
  ) : mNumArray{num_array}
  {
    mTopArray.reserve(mNumArray.size() + 1);
    SizeType acc_num = 0;
    for ( auto num: mNumArray ) {
      mTopArray.push_back(acc_num);
      acc_num += num;
    }
    mTopArray.push_back(acc_num);
    mLevelArray.reserve(acc_num);
    for ( SizeType l = 0; l < mNumArray.size(); ++ l ) {
      auto num = mNumArray[l];
      auto top = mTopArray[l];
      for ( SizeType i = 0; i < num; ++ i ) {
	mLevelArray.push_back(l);
      }
    }
  }

  /// @brief デストラクタ
  ~Fence() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大レベルを返す．
  SizeType
  max_level() const
  {
    return mNumArray.size();
  }

  /// @brief 全ノード数を返す．
  SizeType
  total_num() const
  {
    return mTopArray.back();
  }

  /// @brief level のノード数を返す．
  SizeType
  num(
    SizeType level ///< [in] レベル
  ) const
  {
    if ( level >= max_level() ) {
      throw std::out_of_range{"level is out of range"};
    }
    return mNumArray[level];
  }

  /// @brief level の先頭のノード番号を返す．
  SizeType
  top(
    SizeType level ///< [in] レベル
  ) const
  {
    if ( level > max_level() ) {
      throw std::out_of_range{"level is out of range"};
    }
    return mTopArray[level];
  }

  /// @brief node_id のノードのレベルを返す．
  SizeType
  level(
    SizeType node_id ///< [in] ノード番号
  ) const
  {
    if ( node_id >= total_num() ) {
      throw std::out_of_range{"node_id is out of range"};
    }
    return mLevelArray[node_id];
  }

  /// @brief 内容を表す文字列を返す．
  std::string
  to_string() const
  {
    std::ostringstream buf;
    const char* sep = "";
    buf << "(";
    for ( auto num: mNumArray ) {
      buf << sep << num;
      sep = ", ";
    }
    buf << ")";
    return buf.str();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各レベルのノード数
  std::vector<SizeType> mNumArray;

  // 各レベルの先頭のノード番号
  // 終端を表すために要素数はレベル数+1
  std::vector<SizeType> mTopArray;

  // 各ノードのレベル
  std::vector<SizeType> mLevelArray;

};

inline
std::ostream&
operator<<(
  std::ostream& s,
  const Fence& fence
)
{
  s << fence.to_string();
  return s;
}

END_NAMESPACE_YM_EXSYN

#endif // FENCE_H
