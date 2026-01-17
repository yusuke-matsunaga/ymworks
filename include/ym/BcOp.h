#ifndef BCOP_H
#define BCOP_H

/// @file BcGraph.h
/// @brief BcGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bc.h"
#include "ym/BcEdge.h"


BEGIN_NAMESPACE_YM_BC

//////////////////////////////////////////////////////////////////////
/// @class BcOp BcOp.h "ym/BcOp.h"
/// @ingroup BcGroup
/// @brief Bool chain の演算を表すクラス
///
/// type() が BcOpType::TV の時は真理値表ベクタを持つ．
//////////////////////////////////////////////////////////////////////
class BcOp
{
  friend class BcGraph;

public:

  /// @brief 空のコンストラクタ
  BcOp() = default;

  /// @brief コンストラクタ
  BcOp(
    SizeType id,                         ///< [in] 演算番号
    BcOpType type,                       ///< [in] 演算の種類
    const std::vector<BcBvType>& tv,     ///< [in] 真理値表ベクタ
    const std::vector<BcEdge>& opr_list, ///< [in] オペランドのリスト
    SizeType level                       ///< [in] レベル
  ) : mId{id},
      mType{type},
      mTv{tv},
      mOprList{opr_list},
      mLevel{level}
  {
  }

  /// @brief デストラクタ
  ~BcOp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算番号を返す．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief 演算の種類を返す．
  BcOpType
  type() const
  {
    return mType;
  }

  /// @brief 演算タイプが BcOpType::TV の場合に真理値表ベクタを返す．
  const std::vector<BcBvType>&
  tv() const
  {
    return mTv;
  }

  /// @brief tv() のビットを得る．
  bool
  tv_bit(
    SizeType index
  ) const
  {
    auto blk = index / 64;
    auto sft = index % 64;
    return static_cast<bool>((mTv[blk] >> sft) & 1);
  }

  /// @brief オペランド数を返す．
  SizeType
  operand_num() const
  {
    return mOprList.size();
  }

  /// @brief オペランドを返す．
  BcEdge
  operand(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < operand_num() )
  ) const
  {
    if ( pos >= operand_num() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mOprList[pos];
  }

  /// @brief レベルを返す．
  SizeType
  level() const
  {
    return mLevel;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const BcOp& right
  ) const
  {
    if ( type() != right.type() ) {
      return false;
    }
    if ( type() == BcOpType::TV ) {
      auto n = mTv.size();
      if ( right.mTv.size() != n ) {
	return false;
      }
      for ( SizeType i = 0; i < n; ++ i ) {
	if ( mTv[i] != right.mTv[i] ) {
	  return false;
	}
      }
    }
    if ( operand_num() != right.operand_num() ) {
      return false;
    }
    for ( SizeType i = 0; i < operand_num(); ++ i ) {
      if ( operand(i) != right.operand(i) ) {
	return false;
      }
    }
    return true;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const BcOp& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 内容を表す文字列を返す．
  std::string
  to_string() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝のレベルを返す．
  SizeType
  level(
    BcEdge edge ///< [in] 枝
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 演算番号
  SizeType mId{static_cast<SizeType>(-1)};

  // 演算の種類
  BcOpType mType{BcOpType::NONE};

  // 真理値表ベクタ
  std::vector<BcBvType> mTv;

  // オペランドのリスト
  std::vector<BcEdge> mOprList;

  // レベル
  SizeType mLevel{0};

};

END_NAMESPACE_YM_BC

#endif // BCOP_H
