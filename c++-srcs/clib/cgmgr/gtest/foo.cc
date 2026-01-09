
/// @file CgLogicSig_test.cc
/// @brief CgLogicSig_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

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

class FuncTest
{
public:

  void
  check(const FuncSpec& spec);

};


BEGIN_NONAMESPACE

// 真理値表を取り出す．
vector<bool>
extract_table(
  const TvFunc& func
)
{
  if ( func.is_invalid() ) {
    return vector<bool>{};
  }

  SizeType ni_exp = 1 << func.input_num();
  vector<bool> ans(ni_exp);
  for ( SizeType i = 0; i < ni_exp; ++ i ) {
    ans[i] = func.value(i);
  }
  return ans;
}

void
dump_hex(
  ostream& s,
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
string
to_hex(
  const vector<bool>& table ///< [in] 真理値表
)
{
  SizeType n = table.size();
  ostringstream buf;
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
string
gen_str(
  SizeType ni,
  const vector<bool>& func_table,
  const vector<bool>& tristate_table
)
{
  ostringstream buf;
  buf << "C:" << ni << ":1:0:" << to_hex(func_table)
      << ":" << to_hex(tristate_table);
  return buf.str();
}

// 真理値表を変換する．
vector<bool>
xform_table(
  const vector<bool>& table,
  bool oinv,
  const vector<bool>& iinv,
  const PermGen& pg
)
{
  SizeType ni_exp = table.size();
  if ( ni_exp == 0 ) {
    return vector<bool>{};
  }

  SizeType ni = 0;
  while ( (1 << ni) < ni_exp ) {
    ++ ni;
  }
  vector<bool> ans(ni_exp, false);
  for ( SizeType bits = 0; bits < ni_exp; ++ bits ) {
    vector<bool> ival(ni, false);
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
FuncTest::check(
  const FuncSpec& spec
)
{
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
  ASSERT_COND( exp_str == sig.str() );

  // 代表シグネチャを求める．
  auto iomap = sig.rep_map();
  auto rep_sig = sig.xform(iomap);
  auto rep_str = rep_sig.str();

  for ( bool oinv: {false, true} ) {
    vector<bool> iinv(ni, false);
    for ( SizeType bits = 0; bits < ni_exp; ++ bits ) {
      for ( SizeType i = 0; i < ni; ++ i ) {
	if ( bits & (1 << i) ) {
	  iinv[i] = true;
	}
      }
      for ( PermGen pg(ni, ni); !pg.is_end(); ++ pg ) {
	vector<ClibPinMap> ipin_map(ni);
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
	ASSERT_COND( exp_str == xsig.str() );
	if ( exp_str != xsig.str() ) {
	  cout << "func: " << spec.mFunc << endl
	       << "tristate: " << spec.mTristate << endl
	       << "oinv: " << oinv << endl
	       << "iinv: ";
	  for ( SizeType i = 0; i < ni; ++ i ) {
	    cout << " " << iinv[i];
	  }
	  cout << endl;
	  cout << "imap: ";
	  for ( SizeType i = 0; i < ni; ++ i ) {
	    cout << " " << pg(i);
	  }
	  cout << endl;
	  cout << "xfunc: " << to_hex(xfunc_table) << endl
	       << "xtristate: " << to_hex(xtristate_table) << endl;
	}
	// 代表シグネチャを求める．
	auto xiomap = xsig.rep_map();
	auto xrep_sig = xsig.xform(xiomap);
	auto xrep_str = xrep_sig.str();
	if ( rep_str != xrep_str ) {
	  cout << "Error" << endl
	       << "func: " << spec.mFunc << endl
	       << "tristate: " << spec.mTristate << endl
	       << "oinv: " << oinv << endl
	       << "iinv: ";
	  for ( SizeType i = 0; i < ni; ++ i ) {
	    cout << " " << iinv[i];
	  }
	  cout << endl;
	  cout << "imap: ";
	  for ( SizeType i = 0; i < ni; ++ i ) {
	    cout << " " << pg(i);
	  }
	  cout << endl;
	  cout << "xfunc: " << to_hex(xfunc_table) << endl
	       << "xtristate: " << to_hex(xtristate_table) << endl
	       << "rep_str: " << rep_str << endl
	       << "xrep_str: " << xrep_str << endl;
	}
      }
    }
  }
}

int
foo_test(
  int argc,
  char** argv
)
{
#if 0
  FuncSpec spec{TvFunc::zero(1),
		TvFunc::invalid()};
#else
  FuncSpec spec{TvFunc::posi_literal(2, 0),
                TvFunc::posi_literal(2, 1)};
#endif

  FuncTest test;

  test.check(spec);

  return 0;
}

END_NAMESPACE_YM_CLIB


int
main(
  int argc,
  char** argv
)
{
  return nsYm::nsClib::foo_test(argc, argv);
}
