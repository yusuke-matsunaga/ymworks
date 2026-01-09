
/// @file LexModulePlugin.cc
/// @brief LexModulePlugin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "LexModulePlugin.h"
#include "LexModuleState.h"
#include "common.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// クラス LexModulePlugin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LexModulePlugin::LexModulePlugin(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexPlugin(lex, name),
    mState{state}
{
}

// @brief デストラクタ
LexModulePlugin::~LexModulePlugin()
{
}

// @brief LexModuleState を返す．
LexModuleState*
LexModulePlugin::module_state()
{
  return mState;
}


//////////////////////////////////////////////////////////////////////
// @class LpCellDefine
// @ingroup VlParser
// @brief `celldefine 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpCellDefine::LpCellDefine(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexModulePlugin(lex, name, state)
{
}

// @brief デストラクタ
LpCellDefine::~LpCellDefine()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpCellDefine::parse()
{
  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Warning,
		    "LEX",
		    "`celldefine in module definition.");
  }

  auto loc = cur_token_loc();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after `celldefine.");
    return false;
  }

  module_state()->set_cell_define(true, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpEndCellDefine
// @ingroup VlParser
// @brief `endcelldefine 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpEndCellDefine::LpEndCellDefine(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexModulePlugin(lex, name, state)
{
}

// @brief デストラクタ
LpEndCellDefine::~LpEndCellDefine()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpEndCellDefine::parse()
{
  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Warning,
		    "LEX",
		    "`endcelldefine in module definition.");
  }

  auto loc = cur_token_loc();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after `endcelldefine.");
    return false;
  }

  module_state()->set_cell_define(false, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpNetType
// @ingroup VlParser
// @brief `default_nettype 用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpNetType::LpNetType(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexModulePlugin(lex, name, state)
{
}

// @brief デストラクタ
LpNetType::~LpNetType()
{
}


// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpNetType::parse()
{
  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "`default_nettype in module definition.");
    return false;
  }

  auto file_region = cur_token_loc();

  int id = get_nospace_token();
  VpiNetType val = VpiNetType::None;
  switch ( id ) {
  case WIRE:   val = VpiNetType::Wire; break;
  case TRI:    val = VpiNetType::Tri; break;
  case TRI0:   val = VpiNetType::Tri0; break;
  case TRI1:   val = VpiNetType::Tri1; break;
  case WAND:   val = VpiNetType::Wand; break;
  case TRIAND: val = VpiNetType::TriAnd; break;
  case WOR:    val = VpiNetType::Wor; break;
  case TRIOR:  val = VpiNetType::TriOr; break;
  case TRIREG: val = VpiNetType::TriReg; break;
  case IDENTIFIER:
    // "none" は Verilog-HDL の予約語ではない．
    // `default_nettype の後に来たときだけ特別な意味を持つ．
    if ( cur_string() == "none" ) {
      val = VpiNetType::None;
      break;
    }
    // わざと default に続く
  default:
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting net-type value after `default_net_type.");
    return false;
  }

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "new-line is expected.");
    return false;
  }

  module_state()->set_default_nettype(val, file_region);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpTimeScale
// @ingroup VlParser
// @brief `timescale のパース
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpTimeScale::LpTimeScale(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexModulePlugin(lex, name, state)
{
}

// @brief デストラクタ
LpTimeScale::~LpTimeScale()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpTimeScale::parse()
{
  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "`timescale in module definition.");
    return false;
  }

  auto file_region1 = cur_token_loc();
  int unit = -1;
  if ( !parse_unit(unit) ) {
    // エラーメッセージは parse_unit() 内でセットされている．
    return false;
  }

  if ( !expect('/') ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting '/' after time-unit value.");
    return false;
  }

  auto file_region2 = cur_token_loc();
  int precision = -1;
  if ( !parse_unit(precision) ) {
    // エラーメッセージは parse_unit() 内でセットされている．
    return false;
  }

  if ( unit < precision ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "time precision is coarser than time unit.");
    return false;
  }

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after time-precision value.");
    return false;
  }

  module_state()->set_time_unit(unit, file_region1);
  module_state()->set_time_precision(precision, file_region2);

  return true;
}

// @brief `timescale 内の時間単位を読む関数
bool
LpTimeScale::parse_unit(
  int& unit
)
{
  int mag = 0;
  int uni = 0;

  // 実際にはループを回ることはないが break を使って
  // 実行制御を行うために for-loop を使っている．
  // ここでは成功した場合のみ break で抜けている．
  for ( ; ; ) {
    if ( expect(UNUM_INT) ) {
      auto number = cur_string();
      if ( number == "1" ) {
	mag = 0;
	break;
      }
      if ( number == "10" ) {
	mag = 1;
	break;
      }
      if ( number == "100" ) {
	mag = 2;
	break;
      }
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting 1/10/100.");
    return false;
  }

  // 上と同様の疑似ループ
  for ( ; ; ) {
    if ( expect(IDENTIFIER) ) {
      auto unit_str = cur_string();
      if ( unit_str == "s" ) {
	uni = 0;
	break;
      }
      if ( unit_str == "ms" ) {
	uni = -3;
	break;
      }
      if ( unit_str == "us" ) {
	uni = -6;
	break;
      }
      if ( unit_str == "ns" ) {
	uni = -9;
	break;
      }
      if ( unit_str == "ps" ) {
	uni = -12;
	break;
      }
      if ( unit_str == "fs" ) {
	uni = -15;
	break;
      }
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting s/ms/us/ns/ps/fs.");
    return false;
  }

  unit = uni + mag;

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpUnconnDrive
// @ingroup VlParser
// @brief unconnected_drive のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpUnconnDrive::LpUnconnDrive(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexModulePlugin(lex, name, state)
{
}

// @brief デストラクタ
LpUnconnDrive::~LpUnconnDrive()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpUnconnDrive::parse()
{
  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "`unconnected_drive in module definition.");
    return false;
  }

  auto loc = cur_token_loc();
  auto ud = VpiUnconnDrive::HighZ;
  switch ( get_nospace_token() ) {
  case PULL0: ud = VpiUnconnDrive::Pull0; break;
  case PULL1: ud = VpiUnconnDrive::Pull1; break;
  default:
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting \"pull0\" or \"pull1\""
		    " after `unconneccted_drive.");
    return false;
  }

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line.");
    return false;
  }

  module_state()->set_unconnected_drive(ud, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpNonconnDrive
// @ingroup VlParser
// @brief unconnected_drive のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpNounconnDrive::LpNounconnDrive(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexModulePlugin(lex, name, state)
{
}

// @brief デストラクタ
LpNounconnDrive::~LpNounconnDrive()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpNounconnDrive::parse()
{
  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "`nounconnected_drive in module definition.");
    return false;
  }

  auto loc = cur_token_loc();

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line after `nounconnected_drive.");
    return false;
  }

  module_state()->set_unconnected_drive(VpiUnconnDrive::HighZ, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpDecayTime
// @ingroup VlParser
// @brief `decay_time のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpDecayTime::LpDecayTime(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexModulePlugin(lex, name, state)
{
}

// @brief デストラクタ
LpDecayTime::~LpDecayTime()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpDecayTime::parse()
{
  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "`default_decay_time in module definition.");
    return false;
  }

  int val = 0;
  FileRegion loc;
  for ( ; ; ) {
    // 別にループにする必要はないけど
    // これが一番簡単

    int id = get_nospace_token();
    loc = cur_token_loc();
    if ( id == UNUM_INT ) {
      val = std::stoi(cur_string());
      break;
    }
    if ( id == IDENTIFIER ) {
      if ( cur_string() == "infinite" ) {
	val = -1;
	break;
      }
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting an integer or \"infinite\""
		    " after `default_decay_time.");
    return false;
  }

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "new-line is expected.");
    return false;
  }

  module_state()->set_default_decay_time(val, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpTriregStrength
// @ingroup VlParser
// @brief `trireg_strength のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpTriregStrength::LpTriregStrength(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state
) : LexModulePlugin(lex, name, state)
{
}

// @brief デストラクタ
LpTriregStrength::~LpTriregStrength()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
// @return エラーが起きたら false を返す．
bool
LpTriregStrength::parse()
{
  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "`default_trireg_strength in module definition.");
    return false;
  }

  int val;
  FileRegion loc;
  for ( ; ; ) {
    if ( expect(UNUM_INT) ) {
      val = std::stoi(cur_string());
      loc = cur_token_loc();
      if ( val >= 0 && val <= 250 ) {
	break;
      }
    }
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting an integer between 0 and 250 "
		    "after `default_trireg_strength.");
    return false;
  }

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line.");
    return false;
  }

  module_state()->set_default_trireg_strength(val, loc);

  return true;
}


//////////////////////////////////////////////////////////////////////
// @class LpDelayMode
// @ingroup VlParser
// @brief `delay_mode のパース用プラグイン
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LpDelayMode::LpDelayMode(
  RawLex& lex,
  const std::string& name,
  LexModuleState* state,
  VpiDefDelayMode mode
) : LexModulePlugin(lex, name, state),
  mMode(mode)
{
}

// @brief デストラクタ
LpDelayMode::~LpDelayMode()
{
}

// @brief 該当するコンパイラディレクティブが読み込まれた時に呼ばれる関数
bool
LpDelayMode::parse()
{
  auto loc = cur_token_loc();

  if ( is_in_module() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    MsgType::Error,
		    "LEX",
		    "`delay_mode in module definition.");
    return false;
  }

  if ( !expect_nl() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_token_loc(),
		    MsgType::Error,
		    "LEX",
		    "Syntax error: "
		    "expecting new-line.");
    return false;
  }

  module_state()->set_delay_mode(mMode, loc);

  return true;
}

END_NAMESPACE_YM_VERILOG
