#ifndef BD_H
#define BD_H

/// @file BD.h
/// @brief BD のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class BD BD.h "BD.h"
/// @brief Bound DAG を表すクラス
///
/// Partial DAG の -1 の部分に入力番号を割り当てたもの
/// 大小比較が自然に行えるようにノード番号に入力数分の下駄を履かせる．
/// そのため，入力数のメンバを持つ．
//////////////////////////////////////////////////////////////////////
class BD
{
public:

  /// @brief コンストラクタ
  BD(
    SizeType input_num,                           ///< [in] 入力数
    const std::vector<std::vector<int>>& opr_list ///< [in] 演算ノードのオペランドの情報
  ) : mInputNum{input_num},
      mOprList{opr_list}
  {
  }

  /// @brief デストラクタ
  ~BD() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

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
	buf << sep1;
	sep1 = ", ";
	if ( x >= mInputNum ) {
	  buf << "OP#" << (x - mInputNum);
	}
	else {
	  buf << "I#" << x;
	}
      }
      buf << ")";
    }
    buf << ")";
    return buf.str();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum;

  // オペランド情報のリスト
  std::vector<std::vector<int>> mOprList;

};

inline
std::ostream&
operator<<(
  std::ostream& s,
  const BD& bd
)
{
  s << bd.to_string();
  return s;
}

END_NAMESPACE_YM_EXSYN

#endif // BD_H
