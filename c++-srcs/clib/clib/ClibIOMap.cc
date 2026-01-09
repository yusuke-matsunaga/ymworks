
/// @file ClibIOMap.cc
/// @brief ClibIOMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibIOMap.h"


BEGIN_NAMESPACE_YM_CLIB

BEGIN_NONAMESPACE

void
dump_map_list(
  BinEnc& s,
  const std::vector<ClibPinMap>& map_list
)
{
  std::uint8_t n = map_list.size();
  s << n;
  for ( auto map: map_list ) {
    map.dump(s);
  }
}

std::vector<ClibPinMap>
restore_map_list(
  BinDec& s
)
{
  std::uint8_t n;
  s >> n;
  std::vector<ClibPinMap> tmp_list;
  for ( SizeType i = 0; i < n; ++ i ) {
    ClibPinMap map;
    map.restore(s);
    tmp_list.push_back(map);
  }
  return tmp_list;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス ClibIOMap
//////////////////////////////////////////////////////////////////////

// @brief 合成演算
ClibIOMap
ClibIOMap::operator*(
  const ClibIOMap& right
) const
{
  return ClibIOMap{*this}.operator*=(right);
}

BEGIN_NONAMESPACE

inline
bool
compose_inv(
  bool inv1,
  bool inv2
)
{
  return inv1 != inv2;
}

END_NONAMESPACE

// @brief 合成演算(intern演算)
ClibIOMap&
ClibIOMap::operator*=(
  const ClibIOMap& right
)
{
  SizeType ni = input_num();
  SizeType no = output_num();
  SizeType nb = inout_num();
  if ( right.input_num() != ni ) {
    throw std::invalid_argument{"input_num() mismatch"};
  }
  if ( right.output_num() != no ) {
    throw std::invalid_argument{"output_num() mismatch"};
  }
  if ( right.inout_num() != nb ) {
    throw std::invalid_argument{"inout_num() mismatch"};
  }
  std::vector<ClibPinMap> input_map(ni);
  std::vector<ClibPinMap> output_map(no);
  std::vector<ClibPinMap> inout_map(nb);
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto pinmap1 = mInputMap[i];
    const auto& pinmap2 = right.input_map(pinmap1.id());
    bool inv = compose_inv(pinmap1.inv(), pinmap2.inv());
    mInputMap[i] = ClibPinMap{pinmap2.id(), inv};
  }
  for ( SizeType i = 0; i < no; ++ i ) {
    auto pinmap1 = mOutputMap[i];
    const auto& pinmap2 = right.output_map(pinmap1.id());
    bool inv = compose_inv(pinmap1.inv(), pinmap2.inv());
    mOutputMap[i] = ClibPinMap{pinmap2.id(), inv};
  }
  for ( SizeType i = 0; i < nb; ++ i ) {
    auto pinmap1 = mInoutMap[i];
    const auto& pinmap2 = right.inout_map(pinmap1.id());
    bool inv = compose_inv(pinmap1.inv(), pinmap2.inv());
    mInoutMap[i] = ClibPinMap{pinmap2.id(), inv};
  }
  return *this;
}

// @brief 逆写像演算
ClibIOMap
ClibIOMap::inverse() const
{
  SizeType ni = input_num();
  SizeType no = output_num();
  SizeType nb = inout_num();
  std::vector<ClibPinMap> input_map(ni);
  std::vector<ClibPinMap> output_map(no);
  std::vector<ClibPinMap> inout_map(nb);
  for ( SizeType i = 0; i < ni; ++ i ) {
    const auto& pinmap = mInputMap[i];
    input_map[pinmap.id()] = ClibPinMap{i, pinmap.inv()};
  }
  for ( SizeType i = 0; i < no; ++ i ) {
    const auto& pinmap = mOutputMap[i];
    output_map[pinmap.id()] = ClibPinMap{i, pinmap.inv()};
  }
  for ( SizeType i = 0; i < nb; ++ i ) {
    const auto& pinmap = mInoutMap[i];
    inout_map[pinmap.id()] = ClibPinMap{i, pinmap.inv()};
  }
  return ClibIOMap{input_map, output_map, inout_map};
}

// @brief 等価比較演算
bool
ClibIOMap::operator==(
  const ClibIOMap& right
) const
{
  if ( input_num() != right.input_num() ||
       output_num() != right.output_num() ||
       inout_num() != right.inout_num() ) {
    return false;
  }
  for ( SizeType i = 0; i < input_num(); ++ i ) {
    if ( input_map(i) != right.input_map(i) ) {
      return false;
    }
  }
  for ( SizeType i = 0; i < output_num(); ++ i ) {
    if ( output_map(i) != right.output_map(i) ) {
      return false;
    }
  }
  for ( SizeType i = 0; i < inout_num(); ++ i ) {
    if ( inout_map(i) != right.inout_map(i) ) {
      return false;
    }
  }
  return true;
}

// @brief バイナリダンプを行う．
void
ClibIOMap::dump(
  BinEnc& bos
) const
{
  dump_map_list(bos, input_map_list());
  dump_map_list(bos, output_map_list());
  dump_map_list(bos, inout_map_list());
}

// @brief バイナリダンプされた内容を読み込む．
void
ClibIOMap::restore(
  BinDec& bis
)
{
  mInputMap = restore_map_list(bis);
  mOutputMap = restore_map_list(bis);
  mInoutMap = restore_map_list(bis);
}

/// @relates ClibIOMap
std::ostream&
operator<<(
  std::ostream& s,
  const ClibIOMap& iomap
)
{
  s << "Input[";
  for ( auto pinmap: iomap.input_map_list() ) {
    s << " " << pinmap;
  }
  s << "]";
  if ( iomap.output_num() > 0 ) {
    s << ", Output[";
    for ( auto pinmap: iomap.output_map_list() ) {
      s << " " << pinmap;
    }
    s << "]";
  }
  if ( iomap.inout_num() > 0 ) {
    s << ", Inout[";
    for ( auto pinmap: iomap.inout_map_list() ) {
      s << " " << pinmap;
    }
    s << "]";
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス ClibPinMap
//////////////////////////////////////////////////////////////////////

// @brief バイナリダンプを行う．
void
ClibPinMap::dump(
  BinEnc& bos ///< [in] 出力先のストリーム
) const
{
  bos << mBody;
}

// @brief バイナリダンプされた内容を読み込む．
void
ClibPinMap::restore(
  BinDec& bis ///< [in] 入力元のストリーム
)
{
  bis >> mBody;
}

END_NAMESPACE_YM_CLIB
