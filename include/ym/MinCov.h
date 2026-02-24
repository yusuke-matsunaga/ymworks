#ifndef YM_MINCOV_H
#define YM_MINCOV_H

/// @file ym/MinCov.h
/// @brief MinCov のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/combopt.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class MinCov MinCov.h "ym/MinCov.h"
/// @brief 最小被覆問題を表すクラス
///
/// 集合 $ = { s_1, s_2, ..., s_n } が与えられた時．
/// S の部分集合 S1 = { s_k1, s_k2, ..., s_kn }
/// に対して S1 の要素 s_ki は S1 を「被覆」する，と言う．
/// 与えられた部分集合族 C = { Si } の全てを被覆するような
/// S の部分集合のうち要素数最小のものを C の最小被覆と呼ぶ．
///
/// 場合によっては各集合要素にコストを持たせ，要素数ではなく，
/// 選ばれた要素のコストの総和を最小化する問題もある．
/// もしも負か0のコストを持つ要素があった場合，そのような要素は
/// 必ず最小解に含まれるので簡単に処理できる．
/// そこでコストは1以上と仮定する．
///
/// ここではこの問題を，集合要素を「列」に，被覆条件の部分集合
/// を「行」に対応させた「行列被覆問題」として表している．
//////////////////////////////////////////////////////////////////////
class MinCov
{
public:

  /// @brief 初期化用の要素を表す型
  struct ElemType {
    SizeType row_pos; ///< 行番号
    SizeType col_pos; ///< 列番号
  };

  /// @brief コストの辞書
  using CostDict = std::unordered_map<SizeType, SizeType>;


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  MinCov() = default;

  /// @brief デストラクタ
  ~MinCov() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear()
  {
    mMaxRow = 0;
    mMaxCol = 0;
    mColCostDict.clear();
    mElemList.clear();
  }

  /// @brief 列のコストを設定する
  void
  set_col_cost(
    SizeType col_pos, ///< [in] 列番号 ( 0 <= col_pos < col_size() )
    SizeType cost     ///< [in] コスト
  )
  {
    mColCostDict.emplace(col_pos, cost);
  }

  /// @brief 要素を追加する．
  ///
  /// * 重複した要素の追加についてはチェックしない．
  void
  insert_elem(
    SizeType row_pos, ///< [in] 追加する要素の行番号
    SizeType col_pos  ///< [in] 追加する要素の列番号
  )
  {
    mMaxRow = std::max(mMaxRow, row_pos);
    mMaxCol = std::max(mMaxCol, col_pos);
    mElemList.push_back(ElemType{row_pos, col_pos});
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 行数を得る．
  SizeType
  row_size() const
  {
    return mMaxRow + 1;
  }

  /// @brief 列数を得る．
  SizeType
  col_size() const
  {
    return mMaxCol + 1;
  }

  /// @brief 列のコストを得る．
  SizeType
  col_cost(
    SizeType col_pos ///< [in] 列番号 ( 0 <= col_pos < col_size )
  ) const
  {
    if ( mColCostDict.count(col_pos) == 0 ) {
      return 1;
    }
    return mColCostDict.at(col_pos);
  }

  /// @brief 列のコストの辞書を得る．
  const CostDict&
  col_cost_dict() const
  {
    return mColCostDict;
  }

  /// @brief 要素のリストを得る．
  const std::vector<ElemType>&
  elem_list() const
  {
    return mElemList;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 問題を解く関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @return 解のコスト
  /// @sa JsonValue
  SizeType
  solve(
    std::vector<SizeType>& solution, ///< [out] 選ばれた列集合
    const JsonValue& option = {}     ///< [in] オプションを表すJSONオブジェクト
  ) const;

  /// @brief 解を検証する．
  /// @retval true 正しい解だった．
  /// @retval false 被覆されていない行があった．
  bool
  verify_solution(
    const std::vector<SizeType>& solution, ///< [in] 解のリスト
    std::vector<SizeType>& uncov_row_list  ///< [in] 被覆されていない行のリスト
  ) const;

  /// @brief 解を検証する．
  /// @retval true 正しい解だった．
  /// @retval false 被覆されていない行があった．
  bool
  verify_solution(
    const std::vector<SizeType>& solution ///< [in] 解のリスト
  ) const
  {
    std::vector<SizeType> dummy;
    return verify_solution(solution, dummy);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号の最大値
  SizeType mMaxRow{0};

  // 列番号を最大値
  SizeType mMaxCol{0};

  // 列のコスト辞書
  // 存在しない場合は1と仮定する．
  CostDict mColCostDict;

  // 要素のリスト
  std::vector<ElemType> mElemList;

};

END_NAMESPACE_YM_MINCOV

#endif // YM_MINCOV_H
