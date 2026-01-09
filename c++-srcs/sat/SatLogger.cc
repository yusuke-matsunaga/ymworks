
/// @file SatLogger.cc
/// @brief SatLogger の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "SatLogger.h"
#include "ym/SatLiteral.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// クラス SatLogger
//////////////////////////////////////////////////////////////////////

// @brief 正常な状態の時 true を返す．
bool
SatLogger::valid()
{
  return true;
}

// @brief 変数を追加する．
void
SatLogger::new_variable(
  SatLiteral lit,
  bool decision
)
{
  put_line("N");
  std::ostringstream buf;
  buf << "# var = " << lit;
  if ( decision ) {
    buf << "[*]";
  }
  put_line(buf.str());
}

// @brief 節を追加する．
void
SatLogger::add_clause(
  const std::vector<SatLiteral>& lits
)
{
  std::ostringstream buf;
  buf << "A";
  for ( auto l: lits ) {
    put_lit(buf, l);
  }
  put_line(buf.str());
}

// @brief assumption 付きの SAT 問題を解く．
void
SatLogger::solve(
  const std::vector<SatLiteral>& assumptions
)
{
  std::ostringstream buf;
  buf << "S";
  for ( auto l: assumptions ) {
    put_lit(buf, l);
  }
  put_line(buf.str());
}

// @brief solve() の結果を出力する．
void
SatLogger::solve_result(
  SatBool3 res
)
{
  std::ostringstream buf;
  buf << "# -> ";
  switch ( res ) {
  case SatBool3::False: buf << "UNSAT"; break;
  case SatBool3::True:  buf << "SAT";   break;
  case SatBool3::X:     buf << "ABORT"; break;
  }
  put_line(buf.str());
}

// @brief 一行文の出力を行う．
void
SatLogger::put_line(
  const std::string& str ///< [in] 出力する文字列
)
{
  // デフォルトではなにもしない．
}

// @brief リテラルを出力する．
void
SatLogger::put_lit(
  std::ostream& s,
  SatLiteral lit
)
{
  s << " " << lit.varid();
  if ( lit.is_positive() ) {
    s << "P";
  }
  else {
    s << "N";
  }
}


//////////////////////////////////////////////////////////////////////
// クラス SatLogger_file
//////////////////////////////////////////////////////////////////////

// @brief 正常な状態の時 true を返す．
bool
SatLogger_file::valid()
{
  return static_cast<bool>(mS);
}

// @brief コンストラクタ
SatLogger_file::SatLogger_file(
  const std::string& filename
) : mS{filename}
{
  if ( !mS ) {
    std::ostringstream buf;
    buf << filename << ": Could not create file";
    throw std::invalid_argument{buf.str()};
  }
}

// @brief 一行文の出力を行う．
void
SatLogger_file::put_line(
  const std::string& str
)
{
  if ( mS ) {
    mS << str << std::endl;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス SatLogger_cout
//////////////////////////////////////////////////////////////////////

// @brief 一行文の出力を行う．
void
SatLogger_cout::put_line(
  const std::string& str ///< [in] 出力する文字列
)
{
  std::cout << str << std::endl;
}


//////////////////////////////////////////////////////////////////////
// クラス SatLogger_cerr
//////////////////////////////////////////////////////////////////////

// @brief 一行文の出力を行う．
void
SatLogger_cerr::put_line(
  const std::string& str ///< [in] 出力する文字列
)
{
  std::cerr << str << std::endl;
}

END_NAMESPACE_YM_SAT
