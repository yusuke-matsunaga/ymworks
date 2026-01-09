
/// @file vltest_main.cc
/// @brief Verilog-HDL フロントエンドのテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <unistd.h>
#include <libgen.h>
#include "ym/verilog.h"
#include "ym/PoptMainApp.h"


BEGIN_NAMESPACE_YM_VERILOG

void
rawlex_mode(
  const std::vector<std::string>& filename_list,
  const std::string& spath,
  int watch_line,
  int loop,
  bool verbose,
  bool dump_token
);

void
lex_mode(
  const std::vector<std::string>& filename_list,
  const std::string& spath,
  int watch_line,
  int loop,
  bool verbose,
  bool dump_token
);

END_NAMESPACE_YM_VERILOG


int
main(
  int argc,
  char** argv
)
{
  using namespace nsYm;
  using namespace nsYm::nsVerilog;

  int mode = 4;
  bool verbose = false;
  bool dump = false;
  int bit_expand = 0;
  bool all_msg = false;
  std::string spath;
  int watch_line = 0;
  int loop = 0;
  bool profile = false;

  int ch;
  while ( (ch = getopt(argc, argv, "v12dap:l:w:q")) != -1 ) {
    switch ( ch ) {
    case 'v':
      verbose = true;
      break;
    case '1':
      mode = 1;
      break;
    case '2':
      mode = 2;
      break;
    case 'd':
      dump = true;
      break;
    case 'a':
      all_msg = true;
      break;
    case 'p':
      spath = std::string{optarg};
      break;
    case 'l':
      loop = atoi(optarg);
      break;
    case 'w':
      watch_line = atoi(optarg);
      break;
    case'q':
      profile = true;
      break;
    default:
      std::cerr << "Usage: " << basename(argv[0])
		<< " [-v,-1,-2,-d,-a,-p] [-p <search-path>] [-l <loop-number>] [-w <watch-line>]" << std::endl;
      return -1;
    }
  }
  argc -= optind;
  argv += optind;

#if 0
  PoptMainApp popt;

  PoptNone popt_verbose("verbose", 'v', "enable verbose mode");
  PoptNone popt_rawlex("rawlex", '1', "enable rawlex mode");
  PoptNone popt_lex("lex", '2', "enable lex mode");
  PoptNone popt_dump("dump", 'd', "set dump-flag");
  PoptNone popt_allmsg("all-msg", 'a', "display all kind of messages");
  PoptStr popt_path("search-path", 'p', "set search path", "\"path list \"");
  PoptInt popt_loop("loop", 'l', "loop test", "loop count");
  PoptInt popt_watch("watch-line", 'w', "enable line watcher", "line number");
  PoptNone popt_prof("profile", 'q', "show memory profile");

  popt.add_option(&popt_verbose);
  popt.add_option(&popt_rawlex);
  popt.add_option(&popt_lex);
  popt.add_option(&popt_dump);
  popt.add_option(&popt_allmsg);
  popt.add_option(&popt_path);
  popt.add_option(&popt_loop);
  popt.add_option(&popt_watch);
  popt.add_option(&popt_prof);

  popt.set_other_option_help("[OPTIONS]* <file-name> ...");

  auto popt_stat = popt.parse_options(argc, argv, 0);
  if ( popt_stat == PoptStat::Abort ) {
    return -1;
  }

  // 残りの引数はすべてファイル名と見なす
  std::vector<std::string> filename_list;
  auto n_files = popt.get_args(filename_list);

  if ( n_files == 0 ) {
    popt.print_usage(stderr, 0);
    return 1;
  }

  if ( popt_verbose.is_specified() ) {
    verbose = true;
  }
  if ( popt_rawlex.is_specified() ) {
    mode = 1;
  }
  if ( popt_lex.is_specified() ) {
    mode = 2;
  }
  if ( popt_dump.is_specified() ) {
    dump = 1;
  }
  if ( popt_allmsg.is_specified() ) {
    all_msg = 1;
  }
  if ( popt_path.is_specified() ) {
    spath = popt_path.val().c_str();
  }
  if ( popt_loop.is_specified() ) {
    loop = popt_loop.val();
  }
  if ( popt_watch.is_specified() ) {
    watch_line = popt_watch.val();
  }
  if ( popt_prof.is_specified() ) {
    profile = 1;
  }
#endif

  // オプション解析なし
  std::vector<std::string> filename_list;
  for (int i = 1; i < argc; ++ i) {
    filename_list.push_back(argv[i]);
  }

  switch ( mode ) {
  case 1:
    rawlex_mode(filename_list,
		spath,
		watch_line,
		loop,
		verbose,
		dump);
    break;

  case 2:
    lex_mode(filename_list,
	     spath,
	     watch_line,
	     loop,
	     verbose,
	     dump);
    break;
  }

  return 0;
}
