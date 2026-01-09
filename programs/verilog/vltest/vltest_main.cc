
/// @file vltest_main.cc
/// @brief Verilog-HDL フロントエンドのテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <unistd.h>
#include <libgen.h>
#include "ym/verilog.h"
#include "ym/ClibCellLibrary.h"
#include "ym/PoptMainApp.h"


BEGIN_NAMESPACE_YM_VERILOG

void
parse_mode(
  const std::vector<std::string>& filename_list,
  const std::string& spath,
  int watch_line,
  bool verbose,
  bool profile,
  int loop,
  bool dump_pt
);

void
elaborate_mode(
  const std::vector<std::string>& filename_list,
  bool all_msg,
  const std::string& spath,
  const ClibCellLibrary& cell_library,
  int watch_line,
  bool verbose,
  bool profile,
  int loop,
  bool dump_vpi
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
  bool use_cpt = false;
  bool profile = false;
  std::string liberty_name;
  std::string mislib_name;

  int ch;
  while ( (ch = getopt(argc, argv, "v34dap:l:w:q")) != -1 ) {
    switch (ch) {
    case 'v': verbose = true; break;
    case '3': mode = 3; break;
    case '4': mode = 4; break;
    case 'd': dump = true; break;
    case 'a': all_msg = true; break;
    case 'p': spath = std::string{optarg}; break;
    case 'l': loop = atoi(optarg); break;
    case 'w': watch_line = atoi(optarg); break;
    case 'q': profile = true; break;
    default:
      std::cerr << "USAGE: " << basename(argv[0])
		<< " [-v,-3,-4,-d,-a,-q] [-p <search-path>]"
		<< " [-l <loop-count>] [-w <watch-line>]"
		<< std::endl;
      return -1;
    }
  }
  argc -= optind;
  argv += optind;

#if 0
  PoptMainApp popt;

  PoptNone popt_verbose("verbose", 'v', "enable verbose mode");
  PoptNone popt_yacc("yacc", '3', "enable yacc mode");
  PoptNone popt_elab("elaborate", '4', "enable elaborate mode");
  PoptNone popt_dump("dump", 'd', "set dump-flag");
  PoptNone popt_allmsg("all-msg", 'a', "display all kind of messages");
  PoptStr popt_path("search-path", 'p', "set search path", "\"path list \"");
  PoptInt popt_loop("loop", 'l', "loop test", "loop count");
  PoptInt popt_watch("watch-line", 'w', "enable line watcher", "line number");
  PoptNone popt_prof("profile", 'q', "show memory profile");
  PoptStr popt_dotlib("liberty", 0, "specify liberty library", "\"file name\"");
  PoptStr popt_mislib("mislib", 0, "specify mislib library", "\"file name\"");

  popt.add_option(&popt_verbose);
  popt.add_option(&popt_yacc);
  popt.add_option(&popt_elab);
  popt.add_option(&popt_dump);
  popt.add_option(&popt_allmsg);
  popt.add_option(&popt_path);
  popt.add_option(&popt_loop);
  popt.add_option(&popt_watch);
  popt.add_option(&popt_prof);
  popt.add_option(&popt_dotlib);
  popt.add_option(&popt_mislib);

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
  if ( popt_yacc.is_specified() ) {
    mode = 3;
  }
  if ( popt_elab.is_specified() ) {
    mode = 4;
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
  if ( popt_dotlib.is_specified() ) {
    liberty_name = popt_dotlib.val().c_str();
  }
  if ( popt_mislib.is_specified() ) {
    mislib_name = popt_mislib.val().c_str();
  }
#endif

  ClibCellLibrary cell_library;
  std::vector<std::string> filename_list;
  for ( int i = 1; i < argc; ++ i ) {
    auto arg = argv[i];
    if ( strcmp(arg, "--liberty") == 0 ) {
      if ( i + 1 >= argc ) {
	std::cerr << "'--liberty' requires FILENAME" << std::endl;
	return -2;
      }
      std::string filename = argv[i + 1];
      cell_library = ClibCellLibrary::read_liberty(filename);
      if ( !cell_library.is_valid() ) {
	std::cerr << filename << ": read failed" << std::endl;
	return -2;
      }
    }
    else if ( strcmp(arg, "--mislib") == 0 ) {
      if ( i + 1 >= argc ) {
	std::cerr << "'--mislib' requires FILENAME" << std::endl;
	return -2;
      }
      std::string filename = argv[i + 1];
      cell_library = ClibCellLibrary::read_mislib(filename);
      if ( !cell_library.is_valid() ) {
	std::cerr << filename << ": read failed" << std::endl;
	return -2;
      }
    }
    else {
      filename_list.push_back(arg);
    }
  }

  switch ( mode ) {
  case 3:
    parse_mode(filename_list,
	       spath,
	       watch_line,
	       verbose,
	       profile,
	       loop,
	       dump);
    break;

  case 4:
    elaborate_mode(filename_list,
		   all_msg,
		   spath,
		   cell_library,
		   watch_line,
		   verbose,
		   profile,
		   loop,
		   dump);
    break;
  }
  return 0;
}
