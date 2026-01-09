
/// @file CgLogicSig_test.cc
/// @brief CgLogicSig_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "cgmgr/CgSignature.h"
#include "ym/PermGen.h"
#include "ym/ClibIOMap.h"


BEGIN_NAMESPACE_YM_CLIB

// 関数とそのシグネチャの期待値
struct FuncSpec
{
  TvFunc mFunc;
  TvFunc mTristate;
};

class FuncTest:
  public ::testing::TestWithParam<FuncSpec>
{
public:

  void
  check();

};


BEGIN_NONAMESPACE

// 真理値表を取り出す．
std::vector<bool>
extract_table(
  const TvFunc& func
)
{
  if ( func.is_invalid() ) {
    return std::vector<bool>{};
  }

  SizeType ni_exp = 1 << func.input_num();
  std::vector<bool> ans(ni_exp);
  for ( SizeType i = 0; i < ni_exp; ++ i ) {
    ans[i] = func.value(i);
  }
  return ans;
}

void
dump_hex(
  std::ostream& s,
  SizeType val
)
{
  if ( val < 10 ) {
    s << val;
  }
  else {
    s << static_cast<char>('A' + val - 10);
  }
}

// 結果のHEX文字列を作る．
std::string
to_hex(
  const std::vector<bool>& table ///< [in] 真理値表
)
{
  SizeType n = table.size();
  std::ostringstream buf;
  SizeType val = 0;
  SizeType count = 0;
  for ( auto v: table ) {
    val <<= 1;
    if ( v ) {
      val |= 1;
    }
    ++ count;
    if ( count == 4 ) {
      dump_hex(buf, val);
      val = 0;
      count = 0;
    }
  }
  if ( count > 0 ) {
    dump_hex(buf, val);
  }
  return buf.str();
}

// CgSignature::str() の期待値を作る．
std::string
gen_str(
  SizeType ni,
  const std::vector<bool>& func_table,
  const std::vector<bool>& tristate_table
)
{
  std::ostringstream buf;
  buf << "C:" << ni << ":1:0:" << to_hex(func_table)
      << ":" << to_hex(tristate_table);
  return buf.str();
}

// 真理値表を変換する．
std::vector<bool>
xform_table(
  const std::vector<bool>& table,
  bool oinv,
  const std::vector<bool>& iinv,
  const PermGen& pg
)
{
  auto ni_exp = table.size();
  if ( ni_exp == 0 ) {
    return std::vector<bool>{};
  }

  SizeType ni = 0;
  while ( (1 << ni) < ni_exp ) {
    ++ ni;
  }
  std::vector<bool> ans(ni_exp, false);
  for ( SizeType bits = 0; bits < ni_exp; ++ bits ) {
    std::vector<bool> ival(ni, false);
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( bits & (1 << i) ) {
	ival[i] = true;
      }
    }
    SizeType xbits = 0;
    for ( SizeType i = 0; i < ni; ++ i ) {
      SizeType j = pg(i);
      if ( ival[i] ^ iinv[i] ) {
	xbits |= 1 << j;
      }
    }
    if ( table[xbits] ^ oinv ) {
      ans[bits] = true;
    }
  }
  return ans;
}

END_NONAMESPACE

void
FuncTest::check()
{
  const auto& spec = GetParam();

  auto sig = CgSignature::make_logic_sig(spec.mFunc, spec.mTristate);

  SizeType ni = spec.mFunc.input_num();
  SizeType no = 1;
  SizeType nb = 0;

  SizeType ni_exp = 1 << ni;

  // mFunc の真理値表を取り出す．
  auto func_table = extract_table(spec.mFunc);
  // mTristate の真理値表を取り出す．
  auto tristate_table = extract_table(spec.mTristate);

  // str() の期待値を作る．
  auto exp_str = gen_str(ni, func_table, tristate_table);
  EXPECT_EQ( exp_str, sig.str() );

  // 代表シグネチャを求める．
  auto iomap = sig.rep_map();
  auto rep_sig = sig.xform(iomap);
  auto rep_str = rep_sig.str();

  // 同位体変換のチェック
  auto idmap_list = sig.idmap_list();
  for ( const auto& idmap: idmap_list ) {
    auto sig1 = sig.xform(idmap);
    auto sig1_str = sig1.str();
    EXPECT_EQ( exp_str, sig1_str );
  }

  for ( bool oinv: {false, true} ) {
    std::vector<bool> iinv(ni, false);
    for ( SizeType bits = 0; bits < ni_exp; ++ bits ) {
      for ( SizeType i = 0; i < ni; ++ i ) {
	if ( bits & (1 << i) ) {
	  iinv[i] = true;
	}
      }
      for ( PermGen pg(ni, ni); !pg.is_end(); ++ pg ) {
	std::vector<ClibPinMap> ipin_map(ni);
	for ( SizeType i = 0; i < ni; ++ i ) {
	  ipin_map[pg(i)] = ClibPinMap(i, iinv[i]);
	}
	auto xform = ClibIOMap{ipin_map, oinv};
	auto xsig = sig.xform(xform);

	// func_table, tristate_table を変換する．
	auto xfunc_table = xform_table(func_table, oinv, iinv, pg);
	auto xtristate_table =xform_table(tristate_table, false, iinv, pg);
	// str() の期待値を作る．
	auto exp_str = gen_str(ni, xfunc_table, xtristate_table);
	EXPECT_EQ( exp_str, xsig.str() );
	if ( exp_str != xsig.str() ) {
	  std::cout << "func: " << spec.mFunc
		    << std::endl
		    << "tristate: " << spec.mTristate
		    << std::endl
		    << "oinv: " << oinv
		    << std::endl
		    << "iinv: ";
	  for ( SizeType i = 0; i < ni; ++ i ) {
	    std::cout << " " << iinv[i];
	  }
	  std::cout << std::endl
		    << "imap: ";
	  for ( SizeType i = 0; i < ni; ++ i ) {
	    std::cout << " " << pg(i);
	  }
	  std::cout << std::endl
		    << "xfunc: " << to_hex(xfunc_table)
		    << std::endl
		    << "xtristate: " << to_hex(xtristate_table)
		    << std::endl;
	}
	// 代表シグネチャを求める．
	auto xiomap = xsig.rep_map();
	auto xrep_sig = xsig.xform(xiomap);
	auto xrep_str = xrep_sig.str();
	EXPECT_EQ( rep_str, xrep_str );
	if ( rep_str != xrep_str ) {
	  std::cout << "func: " << spec.mFunc
		    << std::endl
		    << "tristate: " << spec.mTristate
		    << std::endl
		    << "oinv: " << oinv
		    << std::endl
		    << "iinv: ";
	  for ( SizeType i = 0; i < ni; ++ i ) {
	    std::cout << " " << iinv[i];
	  }
	  std::cout
	    << std::endl
	    << "imap: ";
	  for ( SizeType i = 0; i < ni; ++ i ) {
	    std::cout << " " << pg(i);
	  }
	  std::cout
	    << std::endl
	    << "xfunc: " << to_hex(xfunc_table)
	    << std::endl
	    << "xtristate: " << to_hex(xtristate_table)
	    << std::endl;
	}
      }
    }
  }
}

TEST_P(FuncTest, xform)
{
  check();
}

// 0入力関数のテスト
INSTANTIATE_TEST_SUITE_P(CgLogicSig0_test,
			 FuncTest,
			 ::testing::Values(
			   FuncSpec{TvFunc::invalid(),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::zero(0),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::one(0),
				    TvFunc::invalid()}
			 ));

// 1入力関数のテスト
INSTANTIATE_TEST_SUITE_P(CgLogicSig1_test,
			 FuncTest,
			 ::testing::Values(
			   FuncSpec{TvFunc::zero(1),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::one(1),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(1, 0),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::negative_literal(1, 0),
				    TvFunc::invalid()}
			 ));


// 2入力関数のテスト
INSTANTIATE_TEST_SUITE_P(CgLogicSig2_test,
			 FuncTest,
			 ::testing::Values(
			   FuncSpec{TvFunc::zero(2),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::one(2),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(2, 0),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(2, 1),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(2, 0) &
				    TvFunc::positive_literal(2, 1),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(2, 0) &
				    TvFunc::negative_literal(2, 1),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(2, 0) ^
				    TvFunc::positive_literal(2, 1),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(2, 0),
				    TvFunc::positive_literal(2, 1)}
			 ));

// 3入力関数のテスト
INSTANTIATE_TEST_SUITE_P(CgLogicSig3_test,
			 FuncTest,
			 ::testing::Values(
			   FuncSpec{TvFunc::zero(3),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::one(3),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(3, 0) |
				    (TvFunc::positive_literal(3, 1) &
				     TvFunc::negative_literal(3, 2)),
				    TvFunc::invalid()}
			 ));

// 4入力関数のテスト
INSTANTIATE_TEST_SUITE_P(CgLogicSig4_test,
			 FuncTest,
			 ::testing::Values(
			   FuncSpec{TvFunc::zero(4),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::one(4),
				    TvFunc::invalid()},
			   FuncSpec{TvFunc::positive_literal(4, 0) |
				    (TvFunc::positive_literal(4, 1) &
				     TvFunc::negative_literal(4, 2)),
				    TvFunc::positive_literal(4, 3)}
			 ));

END_NAMESPACE_YM_CLIB
