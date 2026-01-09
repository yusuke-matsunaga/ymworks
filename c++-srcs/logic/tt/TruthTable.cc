
/// @file TruthTable.cc
/// @brief TruthTable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TruthTable.h"
#include "ym/split.h"


BEGIN_NAMESPACE_YM

// @brief 入力インデックスを作る．
SizeType
TruthTable::encode_index(
  const std::vector<bool> input_vals
) const
{
  SizeType index = 0;
  SizeType ni = input_num();
  for ( SizeType i = 0; i < ni; ++ i ) {
    if ( input_vals[i] ) {
      index += (1 << (ni - i - 1));
    }
  }
  return index;
}

// @brief 入力インデックスから入力値ベクタを作る．
std::vector<bool>
TruthTable::decode_index(
  SizeType i_index
) const
{
  SizeType ni = input_num();
  std::vector<bool> input_vals(ni, false);
  for ( SizeType i = 0; i < ni; ++ i ) {
    if ( i_index & (1 << (ni - i - 1)) ) {
      input_vals[i] = true;
    }
  }
  return input_vals;
}

// @brief ファイルを読み込む．
TruthTable
TruthTable::read_file(
  const std::string& filename
)
{
  std::ifstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": No such file";
    throw std::invalid_argument{buf.str()};
  }

  bool first = true;
  SizeType ni = 0;
  SizeType no = 0;
  TruthTable tt;
  std::string line;
  std::unordered_set<std::string> addr_mark;
  while ( getline(s, line) ) {
    std::istringstream buf{line};
    std::string addr_str;
    std::string val_str;
    buf >> addr_str >> val_str;
    if ( first ) {
      ni = addr_str.size();
      no = val_str.size();
      tt = TruthTable(ni, no);
      first = false;
    }
    else {
      if ( addr_str.size() != ni ) {
	throw std::invalid_argument{"addr_str.size() != input_num"};
      }
      if ( val_str.size() != no ) {
	throw std::invalid_argument{"val_str.size() != otput_num"};
      }
    }
    if ( addr_mark.count(addr_str) > 0 ) {
      std::ostringstream buf;
      buf << addr_str << " appears more than once";
      throw std::invalid_argument{buf.str()};
    }
    SizeType index = 0;
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( addr_str[i] == '1' ) {
	index |= (1 << (ni - i - 1));
      }
    }
    addr_mark.emplace(addr_str);
    SizeType offset = index * no;
    for ( SizeType j = 0; j < no; ++ j ) {
      if ( val_str[j] == '1' ) {
	tt.mValue0[offset + j] = false;
      }
      else if ( val_str[j] == '0' ) {
	tt.mValue1[offset + j] = false;
      }
    }
  }
  return tt;
}

BEGIN_NONAMESPACE

inline
Val3
decode_bin(
  char c
)
{
  switch ( c ) {
  case '0': return Val3::ZERO;
  case '1': return Val3::ONE;
  case 'x': return Val3::X;
  case 'X': return Val3::X;
  case 'd': return Val3::X;
  case 'D': return Val3::X;
  case '-': return Val3::X;
  case '*': return Val3::X;
  default:
    throw std::invalid_argument{"invalid character in BIN string"};
  }
}

END_NONAMESPACE

// @brief BIN 形式の文字列から変換する．
TruthTable
TruthTable::from_bin(
  const std::string& str
)
{
  auto str_list = split(str, ":");
  if ( str_list.size() != 3 ) {
    throw std::invalid_argument{"invalid string format"};
  }
  SizeType ni = std::stoi(str_list[0]);
  SizeType no = std::stoi(str_list[1]);
  auto bin_str = str_list[2];
  auto n = bin_str.size();
  SizeType ni_exp = 1 << ni;
  if ( ni_exp * no != n ) {
    throw std::invalid_argument{"invalid string size"};
  }
  auto tt = TruthTable(ni, no);
  for ( SizeType o = 0; o < no; ++ o ) {
    auto str1 = bin_str.substr(o * ni_exp, ni_exp);
    for ( SizeType i = 0; i < ni_exp; ++ i ) {
      // 2進文字列の左端が MSB と見なす．
      SizeType index = ni_exp - i - 1;
      auto val = decode_bin(str1[i]);
      tt.set_value(index, o, val);
    }
  }
  return tt;
}

BEGIN_NONAMESPACE

inline
SizeType
decode_hex(
  char c
)
{
  if ( '0' <= c && c <= '9' ) {
    return static_cast<SizeType>(c) - static_cast<SizeType>('0');
  }
  if ( 'a' <= c && c <= 'f' ) {
    return static_cast<SizeType>(c) - static_cast<SizeType>('a') + 10;
  }
  if ( 'A' <= c && c <= 'F' ) {
    return static_cast<SizeType>(c) - static_cast<SizeType>('A') + 10;
  }
  std::ostringstream buf;
  buf << c << ": invalid character";
  throw std::invalid_argument{buf.str()};
}

END_NONAMESPACE

// @brief HEX 形式の文字列から変換する．
TruthTable
TruthTable::from_hex(
  const std::string& str
)
{
  auto str_list = split(str, ":");
  if ( str_list.size() != 3 ) {
    throw std::invalid_argument{"invalid string format"};
  }
  SizeType ni = std::stoi(str_list[0]);
  SizeType no = std::stoi(str_list[1]);
  auto hex_str = str_list[2];
  auto n = hex_str.size();
  SizeType ni_exp = 1 << ni;
  SizeType n1 = ni_exp / 4;
  if ( n1 * no != n ) {
    throw std::invalid_argument{"invalid string size"};
  }
  auto tt = TruthTable(ni, no);
  for ( SizeType o = 0; o < no; ++ o ) {
    auto str1 = hex_str.substr(o * n1, n1);
    for ( SizeType i = 0; i < n1; ++ i ) {
      auto val = decode_hex(str1[i]);
      for ( SizeType j = 0; j < 4; ++ j ) {
	SizeType index = (n1 - i - 1) * 4 + j;
	if ( (val & (1 << j)) != 0 ) {
	  tt.set_value(index, o, Val3::ONE);
	}
	else {
	  tt.set_value(index, o, Val3::ZERO);
	}
      }
    }
  }
  return tt;
}

// @brief 内容を出力する．
void
TruthTable::print(
  std::ostream& s
) const
{
  s << input_num() << " "
    << output_num() << std::endl;
  SizeType index_size = (1 << input_num());
  for ( SizeType i_index = 0; i_index < index_size; ++ i_index ) {
    for ( SizeType o_index = 0; o_index < output_num(); ++ o_index ) {
      auto val = get_value(i_index, o_index);
      s << val;
    }
    s << std::endl;
  }
}

BEGIN_NONAMESPACE

inline
char
encode_bin(
  Val3 val
)
{
  switch ( val ) {
  case Val3::ZERO: return '0';
  case Val3::ONE:  return '1';
  case Val3::X:    return '-';
  default:
    throw std::invalid_argument{"invalid value for Val3"};
  }
}

END_NONAMESPACE

// @brief 内容を表すBIN文字列を返す．
std::string
TruthTable::bin_str() const
{
  std::ostringstream buf;
  buf << input_num() << ":"
      << output_num() << ":";
  SizeType ni_exp = 1 << input_num();
  for ( SizeType o = 0; o < output_num(); ++ o ) {
    for ( SizeType i = 0; i < ni_exp; ++ i ) {
      SizeType index = ni_exp - i - 1;
      auto val = get_value(index, o);
      auto c = encode_bin(val);
      buf << c;
    }
  }
  return buf.str();
}

BEGIN_NONAMESPACE

inline
char
encode_hex(
  SizeType v
)
{
  if ( 0 <= v && v <= 9 ) {
    return static_cast<char>('0' + v);
  }
  if ( 10 <= v && v <= 15 ) {
    return static_cast<char>('A' + v - 10);
  }
  std::ostringstream buf;
  buf << v << ": out of range";
  throw std::invalid_argument{buf.str()};
}

END_NONAMESPACE

// @brief 内容を表すHEX文字列を返す．
std::string
TruthTable::hex_str() const
{
  std::ostringstream buf;
  buf << input_num() << ":"
      << output_num() << ":";
  SizeType ni_exp = 1 << input_num();
  SizeType n1 = ni_exp / 4;
  for ( SizeType o = 0; o < output_num(); ++ o ) {
    for ( SizeType i1 = 0; i1 < n1; ++ i1 ) {
      SizeType index0 = (n1 - i1 - 1) * 4;
      SizeType v = 0;
      for ( SizeType i2 = 0; i2 < 4; ++ i2 ) {
	SizeType index = index0 + i2;
	auto val = get_value(index, o);
	if ( val == Val3::ONE ) {
	  v |= (1 << i2);
	}
      }
      auto c = encode_hex(v);
      buf << c;
    }
  }
  return buf.str();
}

// @brief サポートを得る．
std::vector<bool>
TruthTable::get_support(
  SizeType o_index
) const
{
  SizeType ni = input_num();
  SizeType ni_exp = 1 << ni;
  std::vector<bool> support(ni, false);
  for ( SizeType i_index = 0; i_index < ni_exp; ++ i_index ) {
    auto val0 = get_value(i_index, o_index);
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto index1 = i_index ^ (1 << (ni - i - 1));
      auto val1 = get_value(index1, o_index);
      if ( val0 != Val3::X && val1 != Val3::X && val0 != val1 ) {
	support[i] = true;
      }
    }
  }
  return support;
}

END_NAMESPACE_YM
