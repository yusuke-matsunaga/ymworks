
/// @file CiFFCell.cc
/// @brief CiFFCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "CiFFCell.h"
#include "cgmgr/CgSignature.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"
#include "ym/ClibSeqAttr.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiCell
//////////////////////////////////////////////////////////////////////

// @brief master/slave型のFFセルを生成するクラスメソッド
std::unique_ptr<CiCell>
CiCell::new_FF(
  const ShString& name,
  ClibArea area,
  const ShString& var1,
  const ShString& var2,
  const Expr& clocked_on,
  const Expr& clocked_on_also,
  const Expr& next_state,
  const Expr& clear,
  const Expr& preset,
  ClibSeqAttr seq_attr
)
{
  CiCell* ptr = nullptr;
  if ( seq_attr.has_slave_clock() ) {
    ptr = new CiFF2Cell{
      name, area,
      var1, var2,
      clocked_on,
      clocked_on_also,
      next_state,
      clear, preset,
      seq_attr
    };
  }
  else {
    ptr = new CiFFCell{
      name, area,
      var1, var2,
      clocked_on,
      next_state,
      clear, preset,
      seq_attr
    };
  }
  return std::unique_ptr<CiCell>{ptr};
}


//////////////////////////////////////////////////////////////////////
// クラス CiFFCell
//////////////////////////////////////////////////////////////////////

// @brief セルの種類を返す．
ClibCellType
CiFFCell::type() const
{
  return ClibCellType::FF;
}

// @brief 組み合わせ論理タイプの時 true を返す．
bool
CiFFCell::is_logic() const
{
  return false;
}

// @brief FFセルの時に true を返す．
bool
CiFFCell::is_ff() const
{
  return true;
}

// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
Expr
CiFFCell::clock_expr() const
{
  return mClock;
}

// @brief FFセルの場合に次状態関数を表す論理式を返す．
Expr
CiFFCell::next_state_expr() const
{
  return mNextState;
}

// @brief シグネチャを返す．
CgSignature
CiFFCell::make_signature() const
{
  SizeType ni = input_num();
  SizeType no = output_num();
  SizeType nb = inout_num();
  SizeType ni2 = ni + nb;;
  SizeType no2 = no + nb;
  SizeType xni = ni2 + 2; // iq, ixq
  std::vector<TvFunc> logic_list(no2);
  std::vector<TvFunc> tristate_list(no2);
  for ( SizeType i = 0; i < no2; ++ i ) {
    auto opin = output(i);
    logic_list[i] = opin->function().tvfunc(xni);
    tristate_list[i] = opin->tristate().tvfunc(xni);
  }
  auto clock = clock_expr().tvfunc(xni);
  auto clock2 = clock2_expr().tvfunc(xni);
  auto next = next_state_expr().tvfunc(xni);
  auto clear = clear_expr().tvfunc(xni);
  auto preset = preset_expr().tvfunc(xni);
  auto sig = CgSignature::make_ff_sig(ni, no, nb,
				      logic_list, tristate_list,
				      clock, clock2, next,
				      clear, preset,
				      seq_attr());
  return sig;
}

// @brief 内容をバイナリダンプする．
void
CiFFCell::dump(
  Serializer& s
) const
{
  s.dump(static_cast<std::uint8_t>(1));
  dump_FF(s);
}

// @brief 内容をバイナリダンプする．
void
CiFFCell::dump_FF(
  Serializer& s
) const
{
  dump_FL(s);
  s.dump(mClock);
  s.dump(mNextState);
}

// @brief 内容を復元する．
void
CiFFCell::_restore(
  Deserializer& s
)
{
  restore_FF(s);
}

// @brief 内容を復元する．
void
CiFFCell::restore_FF(
  Deserializer& s
)
{
  restore_FL(s);
  s.restore(mClock);
  s.restore(mNextState);
}


//////////////////////////////////////////////////////////////////////
// クラス ClibFF2Cell
//////////////////////////////////////////////////////////////////////

// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
Expr
CiFF2Cell::clock2_expr() const
{
  return mClock2;
}

// @brief 内容をバイナリダンプする．
void
CiFF2Cell::dump(
  Serializer& s
) const
{
  s.dump(static_cast<std::uint8_t>(2));
  dump_FF(s);
  s.dump(mClock2);
}

// @brief 内容を復元する．
void
CiFF2Cell::_restore(
  Deserializer& s
)
{
  restore_FF(s);
  s.restore(mClock2);
}

END_NAMESPACE_YM_CLIB
