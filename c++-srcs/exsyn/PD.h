#ifndef PD_H
#define PD_H

/// @file PD.h
/// @brief PD のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class PD PD.h "PD.h"
/// @brief Partial DAG を表すクラス
///
/// 基本的には各ノードのオペランド番号の組のリストの情報を表す．
/// 対称性の考慮のためのノードのグループの情報も構築する．
//////////////////////////////////////////////////////////////////////
class PD
{
public:

  /// @brief コンストラクタ
  explicit
  PD(
    const std::vector<std::vector<int>>& opr_list
  ) : mOprList{opr_list}
  {
    // グループを作る．
    std::vector<SizeType> tmp_list2_1;
    std::vector<SizeType> tmp_list3_1;
    std::vector<SizeType> tmp_list3_2;
    auto last_opr = std::vector<int>{};
    for ( SizeType op_id = 0; op_id < mOprList.size(); ++ op_id ) {
      auto& opr = mOprList[op_id];
      auto nop = opr.size();
      if ( nop == 2 ) {
	int opr0 = opr[0];
	int opr1 = opr[1];
	if ( opr1 == -1 ) {
	  // opr0 == -1
	  mGroup2_2.push_back(op_id);
	}
	else if ( opr0 == -1 ) {
	  if ( tmp_list2_1.empty() ) {
	    tmp_list2_1.push_back(op_id);
	  }
	  else {
	    if ( last_opr.size() == 2 &&
		 last_opr[0] == -1 &&
		 last_opr[1] == opr1 ) {
	      tmp_list2_1.push_back(op_id);
	    }
	    else {
	      mGroup2_1.push_back(tmp_list2_1);
	      tmp_list2_1.clear();
	      tmp_list2_1.push_back(op_id);
	    }
	  }
	}
	else {
	  mGroup2_0.push_back(op_id);
	}
      }
      else if ( nop == 3 ) {
	int opr0 = opr[0];
	int opr1 = opr[1];
	int opr2 = opr[2];
	if ( opr2 == -1 ) {
	  // opr0 == -1
	  // opr1 == -1
	  mGroup3_3.push_back(op_id);
	}
	else if ( opr1 == -1 ) {
	  if ( tmp_list3_2.empty() ) {
	    tmp_list3_2.push_back(op_id);
	  }
	  else {
	    if ( last_opr.size() == 3 &&
		 last_opr[0] == -1 &&
		 last_opr[1] == -1 &&
		 last_opr[2] == opr2 ) {
	      tmp_list3_2.push_back(op_id);
	    }
	    else {
	      mGroup3_2.push_back(tmp_list3_2);
	      tmp_list3_2.clear();
	      tmp_list3_2.push_back(op_id);
	    }
	  }
	}
	else if ( opr0 == -1 ) {
	  if ( tmp_list3_1.empty() ) {
	    tmp_list3_1.push_back(op_id);
	  }
	  else {
	    if ( last_opr.size() == 3&&
		 last_opr[0] == -1 &&
		 last_opr[1] == opr1 &&
		 last_opr[2] == opr2 ) {
	      tmp_list3_1.push_back(op_id);
	    }
	    else {
	      mGroup3_1.push_back(tmp_list3_1);
	      tmp_list3_1.clear();
	      tmp_list3_1.push_back(op_id);
	    }
	  }
	}
	else {
	  mGroup3_0.push_back(op_id);
	}
      }
      last_opr = opr;
    }
    if ( !tmp_list2_1.empty() ) {
      mGroup2_1.push_back(tmp_list2_1);
    }
    if ( !tmp_list3_1.empty() ) {
      mGroup3_1.push_back(tmp_list3_1);
    }
    if ( !tmp_list3_2.empty() ) {
      mGroup3_2.push_back(tmp_list3_2);
    }
  }

  /// @brief デストラクタ
  ~PD() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算ノード数を返す．
  SizeType
  op_num() const
  {
    return mOprList.size();
  }

  /// @brief オペランドの情報を返す．
  const std::vector<int>&
  opr(
    SizeType op_id ///< [in] ノード番号 ( 0 <= op_id < op_num() )
  ) const
  {
    if ( op_id >= op_num() ) {
      throw std::out_of_range{"op_id is out of range"};
    }
    return mOprList[op_id];
  }

  /// @brief 2_0 のグループを返す．
  const std::vector<SizeType>&
  group2_0() const
  {
    return mGroup2_0;
  }

  /// @brief 2_1 の等価グループのリストを返す．
  const std::vector<std::vector<SizeType>>&
  group2_1_list() const
  {
    return mGroup2_1;
  }

  /// @brief 2_2 の等価グループを返す．
  const std::vector<SizeType>&
  group2_2() const
  {
    return mGroup2_2;
  }

  /// @brief 3_0 のグループを返す．
  const std::vector<SizeType>&
  group3_0() const
  {
    return mGroup3_0;
  }

  /// @brief 3_1 の等価グループのリストを返す．
  const std::vector<std::vector<SizeType>>&
  group3_1_list() const
  {
    return mGroup3_1;
  }

  /// @brief 3_2 の等価グループのリストを返す．
  const std::vector<std::vector<SizeType>>&
  group3_2_list() const
  {
    return mGroup3_2;
  }

  /// @brief 3_3 の等価グループを返す．
  const std::vector<SizeType>&
  group3_3() const
  {
    return mGroup3_3;
  }

  /// @brief 内容を表す文字列を返す．
  std::string
  to_string() const
  {
    std::ostringstream buf;
    const char* sep0 = "";
    buf << "(";
    for ( auto& opr: mOprList ) {
      buf << sep0 << "(";
      sep0 = ", ";
      const char* sep1 = "";
      for ( auto x: opr ) {
	buf << sep1 << x;
	sep1 = ", ";
      }
      buf << ")";
    }
    buf << ")";
    return buf.str();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド情報のリスト
  std::vector<std::vector<int>> mOprList;

  // 2_0 のグループ
  std::vector<SizeType> mGroup2_0;

  // 2_1 の等価グループのリスト
  std::vector<std::vector<SizeType>> mGroup2_1;

  // 2_2 の等価グループ
  std::vector<SizeType> mGroup2_2;

  // 3_0 のグループ
  std::vector<SizeType> mGroup3_0;

  // 3_1 の等価グループのリスト
  std::vector<std::vector<SizeType>> mGroup3_1;

  // 3_2 の等価グループのリスト
  std::vector<std::vector<SizeType>> mGroup3_2;

  // 3_3 の等価グループ
  std::vector<SizeType> mGroup3_3;

};

inline
std::ostream&
operator<<(
  std::ostream& s,
  const PD& pd
)
{
  s << pd.to_string();
  return s;
}

END_NAMESPACE_YM_EXSYN

#endif // PD_H
