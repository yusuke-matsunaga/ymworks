
/// @file XformInfo_test.cc
/// @brief XformInfo_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "XformInfo.h"
#include "ym/TruthTable.h"


BEGIN_NAMESPACE_YM_EXSYN

class XformInfoTest :
  public ::testing::Test
{
public:

  void
  do_test(
    const TruthTable& tt
  )
  {
    auto xf = XformInfo(tt);
    //std::cout << tt.bin_str() << " => " << xf << std::endl;

    EXPECT_EQ( tt.input_num(), xf.input_num() );
    EXPECT_EQ( tt.output_num(), xf.output_num() );

    SizeType ni = tt.input_num();
    SizeType ni_exp = 1 << ni;
    SizeType no = tt.output_num();

    // 各出力の期待値を計算する．
    std::vector<XformInfo::OutputInfo> exp_output_info(no);
    std::vector<SizeType> exp_output_literal(no);
    std::vector<SizeType> exp_output_dom(no);
    std::vector<bool> output_done(no, false);
    for ( SizeType o = 0; o < no; ++ o ) {
      // 定数のチェック
      bool all_0 = true;
      bool all_1 = true;
      for ( SizeType b = 0; b < ni_exp; ++ b ) {
	auto val = tt.get_value(b, o);
	switch ( val ) {
	case Val3::ZERO: all_1 = false; break;
	case Val3::ONE:  all_0 = false; break;
	case Val3::X:                   break;
	}
      }
      // すべてドントケアの場合，定数0でも定数1でもよいが
      // ここでは定数0を優先とする．
      if ( all_0 ) {
	exp_output_info[o] = XformInfo::ZERO;
	output_done[o] = true;
	continue;
      }
      if ( all_1 ) {
	exp_output_info[o] = XformInfo::ONE;
	output_done[o] = true;
	continue;
      }

      // リテラル関数のチェック
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto mask = 1 << (ni - i - 1);
	bool all_0 = true;
	bool all_1 = true;
	for ( SizeType b = 0; b < ni_exp; ++ b ) {
	  auto val = tt.get_value(b, o);
	  if ( b & mask ) {
	    val = ~val;
	  }
	  switch ( val ) {
	  case Val3::ZERO: all_1 = false; break;
	  case Val3::ONE:  all_0 = false; break;
	  case Val3::X:                   break;
	  }
	}
	if ( all_1 ) {
	  exp_output_info[o] = XformInfo::NEGA_LIT;
	  exp_output_literal[o] = i;
	  output_done[o] = true;
	  break;
	}
	if ( all_0 ) {
	  exp_output_info[o] = XformInfo::POSI_LIT;
	  exp_output_literal[o] = i;
	  output_done[o] = true;
	  break;
	}
      }
    }

    // 残った出力をドントケアの少ない順にならべる．
    std::vector<SizeType> dc_count(no);
    std::vector<SizeType> o_list;
    o_list.reserve(no);
    for ( SizeType o = 0; o < no; ++ o ) {
      if ( output_done[o] ) {
	continue;
      }
      SizeType c = 0;
      for ( SizeType b = 0; b < ni_exp; ++ b ) {
	auto val = tt.get_value(b, o);
	if ( val == Val3::X ) {
	  ++ c;
	}
      }
      dc_count[o] = c;
      o_list.push_back(o);
    }
    std::sort(o_list.begin(), o_list.end(),
	      [&](SizeType a, SizeType b) -> bool {
		return dc_count[a] < dc_count[b];
	      });
    // 支配関係のチェックを行う．
    SizeType no2 = o_list.size();
    if ( no2 > 0 ) {
      for ( SizeType i1 = 0; i1 < no2 - 1; ++ i1 ) {
	auto o1 = o_list[i1];
	if ( output_done[o1] ) {
	  continue;
	}
	for ( SizeType i2 = i1 + 1; i2 < no2; ++ i2 ) {
	  auto o2 = o_list[i2];
	  if ( output_done[o2] ) {
	    continue;
	  }
	  bool posi_dom = true;
	  bool nega_dom = true;
	  for ( SizeType b = 0; b < ni_exp; ++ b ) {
	    auto val1 = tt.get_value(b, o1);
	    auto val2 = tt.get_value(b, o2);
	    if ( val2 != Val3::X ) {
	      if ( val1 == Val3::X ) {
		posi_dom = false;
		nega_dom = false;
		break;
	      }
	      if ( val2 != val1 ) {
		posi_dom = false;
		if ( !nega_dom ) {
		  break;
		}
	      }
	      else {
		nega_dom = false;
		if ( !posi_dom ) {
		  break;
		}
	      }
	    }
	  }
	  if ( posi_dom ) {
	    exp_output_info[o2] = XformInfo::NOINV_DOM;
	    exp_output_dom[o2] = o1;
	    output_done[o2] = true;
	  }
	  else if ( nega_dom ) {
	    exp_output_info[o2] = XformInfo::INV_DOM;
	    exp_output_dom[o2] = o1;
	    output_done[o2] = true;
	  }
	}
      }
    }
    // 残った出力を求める．
    std::vector<SizeType> o_list2;
    o_list2.reserve(no2);
    for ( auto o: o_list ) {
      if ( !output_done[o] ) {
	o_list2.push_back(o);
      }
    }

    // サポートのチェック
    std::vector<XformInfo::InputInfo> exp_input_info(ni);
    std::vector<bool> input_done(ni, false);
    for ( SizeType i = 0; i < ni; ++ i ) {
      bool support = false;
      for ( auto o: o_list2 ) {
	std::vector<Val3> tt0;
	std::vector<Val3> tt1;
	for ( SizeType b = 0; b < ni_exp; ++ b ) {
	  auto val = tt.get_value(b, o);
	  if ( b & (1 << (ni - i - 1)) ) {
	    tt1.push_back(val);
	  }
	  else {
	    tt0.push_back(val);
	  }
	}
	for ( SizeType j = 0; j < (ni_exp / 2); ++ j ) {
	  if ( tt0[j] != Val3::X &&
	       tt1[j] != Val3::X &&
	       tt0[j] != tt1[j] ) {
	    support = true;
	    break;
	  }
	}
	if ( support ) {
	  break;
	}
      }
      if ( !support ) {
	exp_input_info[i] = XformInfo::UNUSED;
	input_done[i] = true;
      }
    }

    // ピボットのチェック
    SizeType min_dc = no + 1;
    SizeType min_b;
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      SizeType dc = 0;
      for ( auto o: o_list2 ) {
	auto val = tt.get_value(b, o);
	if ( val == Val3::X ) {
	  ++ dc;
	}
      }
      if ( min_dc > dc ) {
	min_dc = dc;
	min_b = b;
	if ( min_dc == 0 ) {
	  break;
	}
      }
    }
    auto pivot = tt.decode_index(min_b);
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( input_done[i] ) {
	continue;
      }
      if ( pivot[i] ) {
	exp_input_info[i] = XformInfo::INV;
      }
      else {
	exp_input_info[i] = XformInfo::NOINV;
      }
    }

    // ピボットに基づいて出力の極性を決める．
    for ( auto o: o_list2 ) {
      auto val = tt.get_value(min_b, o);
      if ( val == Val3::ONE ) {
	exp_output_info[o] = XformInfo::INV_OP;
      }
      else {
	// Val3::X のときも含む
	exp_output_info[o] = XformInfo::NOINV_OP;
      }
    }

    // 期待値とチェック．
    for ( SizeType i = 0; i < ni; ++ i ) {
      EXPECT_EQ( exp_input_info[i], xf.input_info(i) );
    }
    for ( SizeType o = 0; o < no; ++ o ) {
      EXPECT_EQ( exp_output_info[o], xf.output_info(o) );
    }
  }

#if 0
  // サポートのチェック
  void
  check_support(
    const TruthTable& tt,
    const XformInfo& xf
  )
  {
    SizeType ni = tt.input_num();
    SizeType ni_exp = 1 << ni;
    SizeType no = tt.output_num();

    for ( SizeType i = 0; i < ni; ++ i ) {
      bool support = false;
      for ( SizeType o = 0; o < no; ++ o ) {
	auto oinfo = xf.output_info(o);
	if ( oinfo != XformInfo::NOINV_OP &&
	     oinfo != XformInfo::INV_OP ) {
	  continue;
	}
	std::vector<Val3> tt0;
	std::vector<Val3> tt1;
	for ( SizeType b = 0; b < ni_exp; ++ b ) {
	  auto val = tt.get_value(b, o);
	  if ( b & (1 << (ni - i - 1)) ) {
	    tt1.push_back(val);
	  }
	  else {
	    tt0.push_back(val);
	  }
	}
	for ( SizeType j = 0; j < (ni_exp / 2); ++ j ) {
	  if ( tt0[j] != Val3::X &&
	       tt1[j] != Val3::X &&
	       tt0[j] != tt1[j] ) {
	    support = true;
	    break;
	  }
	}
	if ( support ) {
	  break;
	}
      }
      EXPECT_EQ( support, xf.input_info(i) != XformInfo::UNUSED );
    }
  }

  // ピボットのチェック
  void
  check_pibot(
    const TruthTable& tt,
    const XformInfo& xf
  )
  {
    SizeType ni = tt.input_num();
    SizeType ni_exp = 1 << ni;
    SizeType no = tt.output_num();

    SizeType min_dc = no + 1;
    SizeType min_b;
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      SizeType dc = 0;
      for ( SizeType o = 0; o < no; ++ o ) {
	auto oinfo = xf.output_info(o);
	if ( oinfo != XformInfo::NOINV_OP &&
	     oinfo != XformInfo::INV_OP ) {
	  continue;
	}
	auto val = tt.get_value(b, o);
	if ( val == Val3::X ) {
	  ++ dc;
	}
      }
      if ( min_dc > dc ) {
	min_dc = dc;
	min_b = b;
      }
    }

    auto pivot = tt.decode_index(min_b);
    for ( SizeType i = 0; i < ni; ++ i ) {
      switch ( xf.input_info(i) ) {
      case XformInfo::NOINV:  EXPECT_FALSE( pivot[i] ); break;
      case XformInfo::INV:    EXPECT_TRUE ( pivot[i] ); break;
      case XformInfo::UNUSED: break;
      }
    }
  }

  // 出力のチェック
  void
  check_output(
    const TruthTable& tt,
    const XformInfo& xf,
    SizeType o
  )
  {
    SizeType ni = tt.input_num();
    SizeType ni_exp = 1 << ni;
    XformInfo::OutputInfo exp_val;
    SizeType exp_input = ni;

    // 定数のチェック
    bool all_0 = true;
    bool all_1 = true;
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      auto val = tt.get_value(b, o);
      switch ( val ) {
      case Val3::ZERO: all_1 = false; break;
      case Val3::ONE:  all_0 = false; break;
      case Val3::X:                   break;
      }
    }
    if ( all_0 ) {
      exp_val = XformInfo::ZERO;
    }
    else if ( all_1 ) {
      exp_val = XformInfo::ONE;
    }
    else {
      // 入力のチェック
      bool found = false;
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto mask = 1 << (ni - i - 1);
	bool all_0 = true;
	bool all_1 = true;
	for ( SizeType b = 0; b < ni_exp; ++ b ) {
	  auto val = tt.get_value(b, o);
	  if ( b & mask ) {
	    val = ~val;
	  }
	  switch ( val ) {
	  case Val3::ZERO: all_1 = false; break;
	  case Val3::ONE:  all_0 = false; break;
	  case Val3::X:                   break;
	  }
	}
	if ( all_1 ) {
	  exp_val = XformInfo::NEGA_LIT;
	  exp_input = i;
	  found = true;
	  break;
	}
	if ( all_0 ) {
	  exp_val = XformInfo::POSI_LIT;
	  exp_input = i;
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	// ピボット
	std::vector<bool> pivot(ni, false);
	for ( SizeType i = 0; i < ni; ++ i ) {
	  if ( xf.input_info(i) == XformInfo::INV ) {
	    pivot[i] = true;
	  }
	}
	auto pivot_index = tt.encode_index(pivot);

	// 極性のチェック
	auto val0 = tt.get_value(pivot_index, o);
	switch ( val0 ) {
	case Val3::ZERO: exp_val = XformInfo::NOINV_OP; break;
	case Val3::ONE:  exp_val = XformInfo::INV_OP;   break;
	default: break;
	}
      }
    }

    EXPECT_EQ( exp_val, xf.output_info(o) );
    if ( exp_val == XformInfo::POSI_LIT ||
	 exp_val == XformInfo::NEGA_LIT ) {
      EXPECT_EQ( exp_input, xf.output_literal(o) );
    }
  }
#endif
};


TEST_F(XformInfoTest, test1)
{
  // 2入力の完全指定関数
  for ( SizeType bits = 0; bits < 16; ++ bits ) {
    TruthTable tt(2, 1);
    for ( SizeType b = 0; b < 4; ++ b ) {
      if ( bits & (1 << (3 - b)) ) {
	tt.set_value(b, 0, Val3::ONE);
      }
      else {
	tt.set_value(b, 0, Val3::ZERO);
      }
    }
    do_test(tt);
  }
}

TEST_F(XformInfoTest, test2)
{
  // 2入力の不完全指定関数
  std::vector<int> val_array(4, 0);
  for ( auto go_on = true; go_on; ) {
    TruthTable tt(2, 1);
    for ( SizeType b = 0; b < 4; ++ b ) {
      switch ( val_array[3 - b] ) {
      case 0: tt.set_value(b, 0, Val3::ZERO); break;
      case 1: tt.set_value(b, 0, Val3::ONE);  break;
      case 2: tt.set_value(b, 0, Val3::X);    break;
      }
    }
    do_test(tt);

    // val_array を次の値にする．
    for ( SizeType i = 0; i < 4; ++ i ) {
      ++ val_array[i];
      if ( val_array[i] == 3 ) {
	val_array[i] = 0;
	if ( i == 3 ) {
	  go_on = false;
	}
      }
      else {
	break;
      }
    }
  }
}

TEST_F(XformInfoTest, test3)
{
  // 2入力2出力の不完全指定関数
  std::vector<int> val_array(8, 0);
  for ( auto go_on = true; go_on; ) {
    TruthTable tt(2, 2);
    for ( SizeType o = 0; o < 2; ++ o ) {
      for ( SizeType b = 0; b < 4; ++ b ) {
	switch ( val_array[o * 4 + 3 - b] ) {
	case 0: tt.set_value(b, o, Val3::ZERO); break;
	case 1: tt.set_value(b, o, Val3::ONE);  break;
	case 2: tt.set_value(b, o, Val3::X);    break;
	}
      }
    }
    do_test(tt);

    // val_array を次の値にする．
    for ( SizeType i = 0; i < 8; ++ i ) {
      ++ val_array[i];
      if ( val_array[i] == 3 ) {
	val_array[i] = 0;
	if ( i == 7 ) {
	  go_on = false;
	}
      }
      else {
	break;
      }
    }
  }
}

END_NAMESPACE_YM_EXSYN
