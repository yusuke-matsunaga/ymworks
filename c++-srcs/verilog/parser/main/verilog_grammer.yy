%{

/// @file verilog_parser.yy
/// @brief Verilog-HDL (IEEE1364-2001) パーサ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MsgMgr.h"
#include "ym/FileRegion.h"
#include "parser/Parser.h"


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = fr_merge(Rhs, N);


BEGIN_NAMESPACE_YM_VERILOG

// yacc/bison が生成したヘッダファイル
#include "verilog_grammer.hh"


// 字句解析関数
extern
int
yylex(
  YYSTYPE*,
  YYLTYPE*,
  Parser&
);

// エラー報告関数
extern
int
yyerror(
  YYLTYPE*,
  Parser&,
  const char*
);


// fr_array 全体のファイル領域を求める．
// 直感的には FileRegion(fr_array[1], fr_array[n])
// だが(先頭が 0 でなく 1 であることに注意),
// 場合によっては空のトークンで位置がないばあいがあるので
// それをスキップしなければならない．
inline
FileRegion
fr_merge(
  const FileRegion fr_array[],
  size_t n
)
{
  if ( n == 0 ) {
    // なんでこんなことがあるのか不明
    return FileRegion();
  }

  // 真の先頭を求める．
  size_t i;
  for (i = 1; i <= n && !fr_array[i].is_valid(); ++ i) ;
  const FileRegion& first = fr_array[i];

  // 真の末尾を求める．
  size_t j;
  for (j = n; j >= i && !fr_array[j].is_valid(); -- j) ;
  const FileRegion& last = fr_array[j];

  return FileRegion(first, last);
}

%}


// "pure" parser にする．
%define api.pure

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {Parser& parser}

// yylex の引数
%lex-param {Parser& parser}


// トークンの型
%union {
  // 組み込み型
  int inttype;
  double dbltype;
  unsigned long uinttype;
  const char* strtype;

  // 列挙型
  VpiConstType consttype;
  VpiNetType nettype;
  VpiPrimType primtype;
  VpiStrength strengthtype;
  VpiVarType vartype;
  VpiVsType vstype;
  char udpsymbol;

  // PtPort.h
  PtPort* port;
  PtPortList portlist;

  // PtDecl.h
  PtIOHead* iohead;
  PtIOItem* ioitem;
  PtIOItemList ioitemlist;
  PtIODList iodlist;
  PtDeclHead* declhead;
  PtDeclItem* declitem;
  PtDeclItemList declitemlist;
  PtDeclDList decldlist;
  PtRange* range;
  PtRangeList rangelist;

  // PtItem.h
  PtItem* item;
  PtDefParam* defparam;
  PtDefParamList defparamlist;
  PtContAssign* contassign;
  PtContAssignList contassignlist;
  PtInst* inst;
  PtInstList instlist;
  PtGenCaseItem* gencaseitem;
  PtGenCaseItemList gencaseitemlist;
  PtPathDecl* pathdecl;
  PtPathDelay* pathdelay;

  // PtStmt.h
  PtStmt* stmt;
  PtStmtList stmtlist;
  PtCaseItem* caseitem;
  PtCaseItemList caseitemlist;

  // PtExpr.h
  PtExpr* expr;
  PtExprList exprlist;
  PtPart* part;

  // PtMisc.h
  PtStrength* strength;
  PtDelay* delay;
  PtControl* control;
  PtConnection* connection;
  PtConnectionList connectionlist;
  PtAttrInst* attrinst;
  PtAttrInstList attrinstlist;
  PtAttrSpec* attrspec;
  PtAttrSpecList attrspeclist;

  // PtUdp.h
  PtUdpEntry* udpentry;
  PtUdpEntryList udpentrylist;
  PtUdpValue* udpvalue;
  PtUdpValueList udpvaluelist;

  // それ以外の構造体型
  PtHierName hiername;
  PtHeadList headlist;

}

// Lex 内部のみで用いられるトークン
%token ERROR                // エラーを表す．
%token SPACE                // ' ', '\t', eof
%token NL                   // '\r', '\n' or "\r\n"
%token CD_SYMBOL            // `ではじまる文字列
%token COMMENT1             // "//" 〜 改行
%token COMMENT2             // "/*" 〜 "*/"

// 値を持つ終端記号
%token <strtype> IDENTIFIER          // 識別子
%token <strtype> SYS_IDENTIFIER      // '$'で始まる文字列
%token <strtype> STRING              // 文字列
%token <strtype> UNUMBER             // unsigned number (in string)
%token <strtype> UNUM_BIG            // big unsigned number (in string)
%token <uinttype> UNUM_INT           // unsigned number
%token <dbltype> RNUMBER             // real number

// 値を持たない終端記号
%token BASE_B               // 'b
%token BASE_SB              // 'sb
%token BASE_O               // 'o
%token BASE_SO              // 'so
%token BASE_D               // 'd
%token BASE_SD              // 'sd
%token BASE_H               // 'h
%token BASE_SH              // 'sh
%token TILDEAND             // ~&
%token TILDEOR              // ~|
%token TILDEXOR             // ~^
%token LTLT                 // <<
%token LTLTLT               // <<<
%token GTGT                 // >>
%token GTGTGT               // >>>
%token LTEQ                 // <=
%token GTEQ                 // >=
%token EQEQ                 // ==
%token NOTEQ                // !=
%token EQEQEQ               // ===
%token NOTEQEQ              // !==
%token ANDAND               // &&
%token ANDANDAND            // &&&
%token OROR                 // ||
%token STARSTAR             // **
%token EQGT                 // =>
%token STARGT               // *>
%token MINUSGT              // ->
%token PRSTAR               // (*
%token STARPR               // *)
%token PLUSCOLON            // +:
%token MINUSCOLON           // -:
%token ';'
%token ':'
%token '['
%token ']'
%token '{'
%token '}'
%token '('
%token ')'
%token ','
%token '.'
%token '@'
%token '#'
%token '%'
%token '?'
%token '+'
%token '-'
%token '*'
%token '/'
%token '^'
%token '!'
%token '&'
%token '|'
%token '~'
%token '<'
%token '>'
%token '='
%token '$'
%token '0'
%token '1'
%token 'x'
%token 'b'
%token 'r'
%token 'f'
%token 'n'
%token 'p'

// 予約語
%token ALWAYS               // "always"
%token AND                  // "and"
%token ASSIGN               // "assign"
%token AUTOMATIC            // "automatic"
%token BEGIN                // "begin"
%token BUF                  // "buf"
%token BUFIF0               // "bufif0"
%token BUFIF1               // "bufif1"
%token CASE                 // "case"
%token CASEX                // "casex"
%token CASEZ                // "casez"
%token CMOS                 // "cmos"
%token DEASSIGN             // "deassign"
%token DEFAULT              // "default"
%token DEFPARAM             // "defparam"
%token DISABLE              // "disable"
%token EDGE                 // "edge"
%token ELSE                 // "else"
%token END                  // "end"
%token ENDCASE              // "endcase"
%token ENDFUNCTION          // "endfunction"
%token ENDGENERATE          // "endgenerate"
%token ENDMODULE            // "endmodule"
%token ENDPRIMITIVE         // "endprimitive"
%token ENDSPECIFY           // "endspecify"
%token ENDTABLE             // "endtable"
%token ENDTASK              // "endtask"
%token EVENT                // "event"
%token FOR                  // "for"
%token FORCE                // "force"
%token FOREVER              // "forever"
%token FORK                 // "fork"
%token FUNCTION             // "function"
%token GENERATE             // "generate"
%token GENVAR               // "genvar"
%token HIGHZ0               // "highz0"
%token HIGHZ1               // "highz1"
%token IF                   // "if"
%token INITIAL              // "initial"
%token IFNONE               // "ifnone"
%token INOUT                // "inout"
%token INPUT                // "input"
%token INTEGER              // "integer"
%token JOIN                 // "join"
%token LARGE                // "large"
%token LOCALPARAM           // "localparam"
%token MACROMODULE          // "macromodule"
%token MEDIUM               // "medium"
%token MODULE               // "module"
%token NAND                 // "nand"
%token NEGEDGE              // "negedge"
%token NMOS                 // "nmos"
%token NONE                 // "none" `default_nettype のみで用いる．
%token NOR                  // "nor"
%token NOSHOWCANCELLED      // "noshowcancelled"
%token NOT                  // "not"
%token NOTIF0               // "notif0"
%token NOTIF1               // "notif1"
%token OR                   // "or"
%token OUTPUT               // "output"
%token PARAMETER            // "parameter"
%token PMOS                 // "pmos"
%token POSEDGE              // "posedge"
%token PRIMITIVE            // "primitive"
%token PULL0                // "pull0"
%token PULL1                // "pull1"
%token PULLUP               // "pullup"
%token PULLDOWN             // "pulldown"
%token PULSESTYLE_ONEVENT   // "pulsestyle_onevent"
%token PULSESTYLE_ONDETECT  // "pulsestyle_ondetect"
%token RCMOS                // "rcmos"
%token REAL                 // "real"
%token REALTIME             // "realtime"
%token REG                  // "reg"
%token RELEASE              // "release"
%token REPEAT               // "repeat"
%token RNMOS                // "rnmos"
%token RPMOS                // "rpmos"
%token RTRAN                // "rtran"
%token RTRANIF0             // "rtranif0"
%token RTRANIF1             // "rtranif1"
%token SCALARED             // "scalared"
%token SHOWCANCELLED        // "showcancelled"
%token SIGNED               // "signed"
%token SMALL                // "small"
%token SPECIFY              // "specify"
%token SPECPARAM            // "specparam"
%token STRONG0              // "strong0"
%token STRONG1              // "strong1"
%token SUPPLY0              // "supply0"
%token SUPPLY1              // "supply1"
%token TABLE                // "table"
%token TASK                 // "task"
%token TIME                 // "time"
%token TRAN                 // "tran"
%token TRANIF0              // "tranif0"
%token TRANIF1              // "tranif1"
%token TRI                  // "tri"
%token TRI0                 // "tri0"
%token TRI1                 // "tri1"
%token TRIAND               // "triand"
%token TRIOR                // "trior"
%token TRIREG               // "trireg"
%token UNSIGNED             // "unsigned"
%token VECTORED             // "vectored"
%token WAIT                 // "wait"
%token WAND                 // "wand"
%token WEAK0                // "weak0"
%token WEAK1                // "weak1"
%token WHILE                // "while"
%token WIRE                 // "wire"
%token WOR                  // "wor"
%token XOR                  // "xor"
%token XNOR                 // "xnor"

%token SETUP                // "$setup"
%token HOLD                 // "$hold"
%token SETUPHOLD            // "$setuphold"
%token RECOVERY             // "$recovery"
%token REMOVAL              // "$removal"
%token RECREM               // "$recrem"
%token SKEW                 // "$skew"
%token TIMESKEW             // "$timeskew"
%token FULLSKEW             // "$fullskew"
%token PERIOD               // "$period"
%token WIDTH                // "$width"
%token NOCHANGE             // "$nochange"

%token PATHPULSE            // "PATHPULSE$"

// 演算子の定義
%left  COND                         // ? :
%left  OROR                         // ||
%left  ANDAND                       // &&
%left  '|' TILDEOR                  // |, ~|
%left  '&' TILDEAND '^' TILDEXOR    // &, ~&, ^, ~^(^~)
%left  EQEQ NOTEQ EQEQEQ NOTEQEQ    // ==, !=, ===, !==
%left  '<' LTEQ '>' GTEQ            // <, <=, >, >=
%left  LTLT GTGT LTLTLT GTGTGT      // <<, >>, <<<, >>>
%left  '+' '-'                      // +, -
%left  '*' '/' '%'                  // *, /, %
%right STARSTAR                     // **
%right '!' '~' UOP                  // !, ~, (単項演算子)

// dangling else 解消のための優先順位
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

// 非終端ノードの型定義

%type <inttype> module_keyword
%type <inttype> opt_auto
%type <inttype> sign
%type <inttype> pol_op pol_colon
%type <inttype> edge

%type <consttype> numbase

%type <nettype> net_type

%type <primtype> cmos_switchtype enable_gatetype mos_switchtype
%type <primtype> n_input_gatetype n_output_gatetype pass_en_switchtype
%type <primtype> pass_switchtype

%type <strengthtype> strength0 strength1

%type <vartype> task_port_type
%type <vartype> data_type

%type <vstype> vstype

%type <udpsymbol> next_state output_symbol level_symbol edge_symbol

%type <port> port
%type <port> nonnull_port
%type <port> udp_port

%type <portlist> list_of_ports
%type <portlist> udp_port_list

%type <iohead> portdecl_head
%type <iohead> udp_output_port_declaration

%type <iohead> io_declaration
%type <iohead> inout_declaration
%type <iohead> input_declaration
%type <iohead> output_declaration

%type <iohead> tf_inout_declhead
%type <iohead> tf_input_declhead
%type <iohead> tf_output_declhead

%type <iohead> tf_io_declaration
%type <iohead> tf_input_declaration
%type <iohead> tf_inout_declaration
%type <iohead> tf_output_declaration

%type <iohead> udp_input_declaration
%type <iohead> udp_output_declaration

%type <iodlist> udp_declaration_port_list
%type <iodlist> module_portdecl_list
%type <iodlist> function_port_list
%type <iodlist> task_port_list
%type <iodlist> list_of_port_declarations

%type <ioitem> port_identifier_item
%type <ioitem> variable_port_identifier_item

%type <ioitemlist> list_of_port_identifiers
%type <ioitemlist> list_of_variable_port_identifiers

%type <declhead> udp_reg_declaration
%type <declhead> paramport_head
%type <declhead> module_or_generate_decl
%type <declhead> module_decl
%type <declhead> bitem_decl_body
%type <declhead> event_declaration
%type <declhead> genvar_declaration
%type <declhead> integer_declaration
%type <declhead> real_declaration
%type <declhead> realtime_declaration
%type <declhead> time_declaration
%type <declhead> net_declaration
%type <declhead> reg_declaration
%type <declhead> block_reg_declaration
%type <declhead> parameter_declaration
%type <declhead> specparam_declaration
%type <declhead> block_item_declaration

%type <decldlist> module_parameter_port_list
%type <decldlist> list_of_paramport_decl

%type <declitem> paramport_assignment
%type <declitem> param_assignment
%type <declitem> specparam_assignment
%type <declitem> net_decl_assignment
%type <declitem> identifier_with_range
%type <declitem> variable_type
%type <declitem> block_variable_type
%type <declitem> real_type

%type <declitemlist> list_of_param_assignments
%type <declitemlist> list_of_specparam_assignments
%type <declitemlist> list_of_event_identifiers
%type <declitemlist> list_of_genvar_identifiers
%type <declitemlist> list_of_variable_identifiers
%type <declitemlist> list_of_real_identifiers
%type <declitemlist> list_of_net_decls
%type <declitemlist> list_of_net_identifiers
%type <declitemlist> list_of_net_decl_assignments
%type <declitemlist> list_of_block_variable_identifiers

%type <range> range

%type <rangelist> nzlist_of_dimensions

%type <item> module_item
%type <item> module_or_generate_item
%type <item> generated_instantiation
%type <item> specify_block
%type <item> task_declaration
%type <item> function_declaration
%type <item> parameter_override
%type <item> continuous_assign
%type <item> gate_instantiation
%type <item> module_instantiation
%type <item> generate_item
%type <item> initial_construct
%type <item> always_construct
 //%type <item> specify_item

%type <defparam> defparam_assignment

%type <defparamlist> nzlist_of_defparam_assignment

%type <contassign> net_assignment

%type <contassignlist> list_of_net_assignments

%type <inst> cmos_switch_instance
%type <inst> enable_gate_instance
%type <inst> mos_switch_instance
%type <inst> n_input_gate_instance
%type <inst> n_output_gate_instance
%type <inst> pass_enable_switch_instance
%type <inst> pass_switch_instance
%type <inst> pull_gate_instance
%type <inst> mu_instance

%type <instlist> nzlist_of_cmos_switch_inst
%type <instlist> nzlist_of_enable_gate_inst
%type <instlist> nzlist_of_mos_switch_inst
%type <instlist> nzlist_of_n_input_gate_inst
%type <instlist> nzlist_of_n_output_gate_inst
%type <instlist> nzlist_of_pass_en_switch_inst
%type <instlist> nzlist_of_pass_switch_inst
%type <instlist> nzlist_of_pull_inst
%type <instlist> nzlist_of_mu_inst

%type <gencaseitem> genvar_case_item

%type <gencaseitemlist> list_of_gencaseitem

%type <pathdecl> path_declaration
%type <pathdecl> simple_path_declaration
%type <pathdecl> edge_sensitive_path_declaration

%type <pathdelay> path_delay_value
%type <pathdelay> list_of_path_delay_expressions

%type <stmt> blocking_assignment
%type <stmt> nonblocking_assignment
%type <stmt> procedural_continuous_assignments
%type <stmt> variable_assignment
%type <stmt> par_block seq_block
%type <stmt> statement statement_or_null
%type <stmt> pure_statement
%type <stmt> disable_statement
%type <stmt> event_trigger
%type <stmt> procedural_timing_control_statement
%type <stmt> wait_statement conditional_statement
%type <stmt> case_statement
%type <stmt> loop_statement
%type <stmt> system_task_enable
%type <stmt> task_enable

%type <stmtlist> nzlist_of_stmt

%type <caseitem> case_item

%type <caseitemlist> list_of_case_items

%type <expr> port_reference
%type <expr> concatenation
%type <expr> multiple_concatenation
%type <expr> function_call
%type <expr> system_function_call
%type <expr> expression
%type <expr> expr1
%type <expr> primary
%type <expr> module_path_expression
%type <expr> net_lvalue
%type <expr> variable_lvalue
%type <expr> lvalue
%type <expr> number
%type <expr> unumber
%type <expr> rnumber
%type <expr> argument
%type <expr> index
%type <expr> delay_value delay_value_x delay_value_y
%type <expr> path_delay_expression
%type <expr> mintypmax_expression
%type <expr> init_val
%type <expr> event_primary
%type <expr> specify_terminal

%type <exprlist> nzlist_of_expressions
%type <exprlist> nzlist_of_lvalues
%type <exprlist> nzlist_of_terminals
%type <exprlist> nzlist_of_arguments
%type <exprlist> nzlist_of_port_references
%type <exprlist> nzlist_of_index
%type <exprlist> case_item_label
%type <exprlist> eve_list
%type <exprlist> event_expression
%type <exprlist> genvar_case_head

%type <part> part

%type <strength> drive_strength
%type <strength> charge_strength
%type <strength> pulldown_strength
%type <strength> pullup_strength

%type <delay> delay3 delay2

%type <control> delay_control
%type <control> event_control
%type <control> repeat_control

%type <connection> ordered_port_connection
%type <connection> named_port_connection
%type <connection> named_parameter_assignment

%type <connectionlist> n_input_gate_terminals
%type <connectionlist> n_output_gate_terminals
%type <connectionlist> list_of_ordered_param_assign
%type <connectionlist> list_of_named_param_assign
%type <connectionlist> list_of_ordered_port_connections
%type <connectionlist> list_of_named_port_connections

%type <attrinst> ai_list
%type <attrinst> attr_inst

%type <attrinstlist> nz_ai_list

%type <attrspec> attr_spec

%type <attrspeclist> nzlist_of_attr_spec

%type <udpentry> udp_entry

%type <udpentrylist> list_of_udp_entries

%type <udpvalue> level_symbol_value
%type <udpvalue> edge_indicator

%type <udpvaluelist> seq_input_list

%type <hiername> hierarchical_identifier

%type <headlist> list_of_module_items
%type <headlist> list_of_module_items2
%type <headlist> nzlist_of_fitem_decl
%type <headlist> list_of_titem_decl
%type <headlist> list_of_generate_items
%type <headlist> generate_item_or_null
%type <headlist> nzlist_of_uport_decl
%type <headlist> list_of_bitem_decl

/* まだできていない */
%type <inttype> system_timing_check
%type <inttype> timing_check_options
%type <inttype> skew_timing_check_options
%type <inttype> opt_notify_reg
%type <inttype> delayed_data
%type <inttype> timing_check_event
%type <inttype> controlled_timing_check_event
%type <inttype> opt_timing_check_event_control
%type <inttype> timing_check_event_control
%type <inttype> edge_descriptor
%type <inttype> opt_timing_check_condition
%type <inttype> timing_check_condition
/* end-of まだできていない */

%%


//////////////////////////////////////////////////////////////////////
// A.1.3 Module and primitive source text
//////////////////////////////////////////////////////////////////////

// [SPEC] source_text ::= { description }
source_text
: // 空もありうる．
| source_text description
;

// [SPEC] description ::=
//            module_declaration
//           |udp_declaration
description
: module_declaration
| udp_declaration
;

// [SPEC] module_declaration ::=
//            { attribute_instance } module_keyword module_identifier
//                [ module_parameter_port_list ] [ list_of_ports ] ';'
//                { module_item }
//            "endmodule"
//           |{ attribute_instance } module_keyword module_identifier
//                [ module_parameter_port_list ]
//                [ list_of_port_declarations ] ';'
//                { non_port_module_item }
//            "endmodule"
// [SPEC] module_identifier ::= identifier
// 問題は list_of_ports と list_of_port_declarations が共に空の時に
// 区別がつかなくなってしまうということ．
// そのため，list_of_ports も list_of_port_declarations も空ではなくし，
// 代わりに第3番目の形式を追加している．
// もともと port は空もありうるので nzlist_of_ports が名前とは裏腹に
// 空のリストも表している．ただし括弧は含まれる．
// 3番目の形式は括弧も含まない．
// non_port_module_item は module_item でパーズしておいて
// 中でチェックした方がエラーメッセージが分かりやすい．
// yacc の構文にしてしまうとただの syntax error となってしまう．
module_declaration
: ai_list module_keyword IDENTIFIER
  module_parameter_port_list
  '(' list_of_ports ')' ';'
  list_of_module_items
  ENDMODULE
{
  auto m = parser.new_Module1995(@$, $2, $3,
				 $4.head_list.top,
				 $6.top,
				 $9.iohead_list.top,
				 $9.declhead_list.top,
				 $9.item_list.top);
  parser.reg_attrinst(m, $1);
  parser.reg_module(m);
}
| ai_list module_keyword IDENTIFIER
  module_parameter_port_list module_portdecl_list ';'
  list_of_module_items2
  ENDMODULE
{
  auto m = parser.new_Module2001(@$, $2, $3,
				 $4.head_list.top,
				 $5.head_list.top,
				 $7.declhead_list.top,
				 $7.item_list.top);
  parser.reg_attrinst(m, $1);
  parser.reg_module(m);
}
| ai_list module_keyword IDENTIFIER
  module_parameter_port_list
  '(' ')' ';'
  list_of_module_items
  ENDMODULE
{ // list_of_ports が空の場合
  auto m = parser.new_Module1995(@$, $2, $3,
				 $4.head_list.top,
				 nullptr,
				 $8.iohead_list.top,
				 $8.declhead_list.top,
				 $8.item_list.top);
  parser.reg_attrinst(m, $1);
  parser.reg_module(m);
}
| ai_list module_keyword error ENDMODULE
{
  yyerrok;
}
;

// [SPEC] module_keyword ::= "module" | "macromodule"
module_keyword
: MODULE
{
  // ちょっとした hack!
  $$ = false;
}
| MACROMODULE
{
  // ちょっとした hack!
  $$ = true;
}
;

module_portdecl_list
: // 空もあり
{
  $$ = PtIODList();
  $$.init();
}
| '(' list_of_port_declarations ')'
{
  $$ = $2;
}
;


//////////////////////////////////////////////////////////////////////
// A.1.4 Module parameters and ports
//////////////////////////////////////////////////////////////////////

// [SPEC] module_parameter_port_list ::=
//            '#' '(' parameter_declaration {',' parameter_declaration} ')'
// ほとんどそのままの翻訳．ただし [ module_parameter_port_list ]
// という形で使われるので空の場合も含めている．
// しかしこれ仕様書通りだと parameter_declaration の末尾はセミコロン
// なんですけど,,,,,
module_parameter_port_list
: // 空
{
  $$ = PtDeclDList();
  $$.init();
}
| '#' '(' list_of_paramport_decl ')'
{
  $$ = $3;
}
;

// [SPEC*] list_of_param_decl ::=
//           parameter_declaration { ',' parameter_declaration }
list_of_paramport_decl
: ai_list paramport_head
{
  $$ = PtDeclDList();
  $$.init($2);
  parser.reg_attrinst($2, $1);
}
| list_of_paramport_decl ',' ai_list paramport_head
{
  $$ = $1;
  $$.add_head($4);
  parser.reg_attrinst($4, $3);
}
| list_of_paramport_decl ',' paramport_assignment
{
  $$ = $1;
  $1.add_item($3);
}
;

// head と言っているが，実際にはヘッダと最初の要素を含んでいる．
paramport_head
: PARAMETER paramport_assignment
{
  $$ = parser.factory().new_ParamH(@$, $2);
}
| PARAMETER sign range paramport_assignment
{
  $$ = parser.factory().new_ParamH(@$, $2, $3, $4);
}
| PARAMETER data_type paramport_assignment
{
  $$ = parser.factory().new_ParamH(@$, $2, $3);
}
;

// [SPEC] param_assignment ::= parameter_identifier '=' constant_expression
// [SPEC] parameter_identifier ::= identifier
paramport_assignment
: IDENTIFIER '=' expression
{
  $$ = parser.factory().new_DeclItem(@$, $1, $3);
}
;

// [SPEC] list_of_ports ::= '(' port {',' port} ')'
//
// 実は port は空にもなりうるが，空ポートが一つのときは
// '(' ')' となってしまった空リストと区別ができない．
// '(' ')' は別に処理するので空でないリストのみを対象とする．
// そのため nonnull_port というトークンを作る．
// 要素数が２つ以上の時には空ポートがあってもよい．
list_of_ports
: nonnull_port
{
  $$ = PtPortList();
  $$.init($1);
}
| list_of_ports ',' port
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] port ::=
//             [port_expression]
//            |'.' port_identifier '(' [port_expression] ')'
// [SPEC] port_identifier ::= identifier
// [SPEC] port_expression ::=
//            port_reference
//           |'{' port_reference { ',' port_reference } '}'
port
: // 空
{
  // でも nullptr を返さない．
  $$ = parser.factory().new_Port(FileRegion());
}
| nonnull_port
{
  $$ = $1;
}
;

// 空でない port
// port_expression を port_reference と nzlist_of_port_reference
// に展開する．
nonnull_port
: port_reference
{
  // 内側の式のみ指定するタイプ
  $$ = parser.factory().new_Port(@$, nullptr, $1);
}
| '{' nzlist_of_port_references '}'
{
  // 内側の式のみ指定するタイプ
  $$ = parser.factory().new_Port(@$, nullptr, $2.top);
}
| '.' IDENTIFIER '('                 ')'
{
  // 外側の名前のみ指定するタイプ
  $$ = parser.factory().new_Port(@$, $2);
}
| '.' IDENTIFIER '(' port_reference ')'
{
  // 外側の名前と内側の式を指定するタイプ
  $$ = parser.factory().new_Port(@$, $2, $4);
}
| '.' IDENTIFIER '(' '{' nzlist_of_port_references '}' ')'
{
  // 外側の名前と内側の式を指定するタイプ
  $$ = parser.factory().new_Port(@$, $2, $5.top);
}
;

// [SPEC*] nzlist_of_port_references ::=
//              port_reference { ',' port_reference }
nzlist_of_port_references
: port_reference
{
  $$ = PtExprList();
  $$.init($1);
}
| nzlist_of_port_references ',' port_reference
{
  $$ = $1;
  $$.add($3);
}
;


// [SPEC] port_reference ::= port_identifier
//            port_identifier '[' constant_expression ']'
//           |port_identifier '[' range_expression ']'
// [SPEC] port_identifier ::= identifier
// オリジナルは range_expression だけどこれは
// constant_range_expression の誤りでしょ．
// さらにオリジナルの (constant_)range_expression は (constant_)expression
// を含むので reduce/reduce conflict が生じる．
// 解決策は (constant_)range_expression から (constant_)expression を取り
// 除くこと．この expression と range の話は他の所でも出てくる
// (cf. dimension と index)
// また port_identifier は実はただの identifier
// どうも Verilog の BNF を書いた人は意味解析と構文解析の違いをわかって
// いないようである．構文解析中に識別子テーブルでもつくって別のトークンを
// 返すようにしろとでも言うのだろうか？
// たしかに C言語の構造体名はそういう工夫が必要だが．
port_reference
: IDENTIFIER
{
  $$ = parser.factory().new_Primary(@$, $1);
}
| IDENTIFIER index
{
  $$ = parser.factory().new_Primary(@$, $1, $2);
}
| IDENTIFIER part
{
  $$ = parser.factory().new_Primary(@$, $1, $2);
}
;


// [SPEC] list_of_port_declarations ::=
//            '(' port_declaration {',' port_declaration} ')'
//           |'(' ')'
// [SPEC] port_declaration ::=
//            { attribute_instance } inout_declaration
//           |{ attribute_instance } input_declaration
//           |{ attribute_instance } output_declaration
// input_declaration などの詳細は後に出てくる．
// 上で述べたように () のケースは nzlist_of_ports で特別に扱うこととし,
// list_of_port_declarations は非ゼロ要素のリストとする．
// 2重のリストを同じ ',' で区切っているので IEEE1364 の BNF 通りにすると
// 曖昧な文法となってしまうので1重のリストにして ',' の後に
// INPUT/INOUT/OUTPUT が来るか port_identifier が来るかで区別する様に
// 変更した．何にも考えていない IEEE1364-2001 の拡張の仕方が悪い．
// 詳細は A.2.1.2 Port declarations を参照のこと．
list_of_port_declarations
: ai_list portdecl_head
{
  $$ = PtIODList();
  $$.init($2);
  parser.reg_attrinst($2, $1);
}
| list_of_port_declarations ',' ai_list portdecl_head
{
  $$ = $1;
  $1.add_head($4);
  parser.reg_attrinst($4, $3);
}
| list_of_port_declarations ',' variable_port_identifier_item
{
  $$ = $1;
  $1.add_item($3);
}
;

// [SPEC*] portdecl_head ::=
//            "inout" [nettype] ["signed"] [range]
//           |"input" [nettype] ["signed"] [range]
//           |"output" [nettype] ["signed"] [range]
//           |"output" "reg"  ["signed"][range]
//           |"output" "reg"  ["signed"][range]
//           |"output" output_variable_type
//           |"output" output_variable_type
portdecl_head
: INOUT          sign variable_port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Inout, $3);
}
| INOUT net_type sign variable_port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Inout, $2, $4);
}
| INOUT          sign range variable_port_identifier_item
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Inout, $2, $3, $4);
}
| INOUT net_type sign range variable_port_identifier_item
{
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Inout, $2, $3, $4, $5);
}
| INPUT          sign variable_port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $3);
}
| INPUT net_type sign variable_port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Input, $2, $4);
}
| INPUT          sign range variable_port_identifier_item
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $2, $3, $4);
}
| INPUT net_type sign range variable_port_identifier_item
{
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Input, $2, $3, $4, $5);
}
| OUTPUT          sign variable_port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $3);
}
| OUTPUT net_type sign variable_port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Output, $2, $4);
}
| OUTPUT          sign range variable_port_identifier_item
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $2, $3, $4);
}
| OUTPUT net_type sign range variable_port_identifier_item
{
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Output, $2, $3, $4, $5);
}
| OUTPUT REG sign variable_port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $4);
}
| OUTPUT REG sign range variable_port_identifier_item
{
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $3, $4, $5);
}
| OUTPUT INTEGER variable_port_identifier_item
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Output, VpiVarType::Integer, $3);
}
| OUTPUT TIME variable_port_identifier_item
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Output, VpiVarType::Time, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.1.5 Module items
//////////////////////////////////////////////////////////////////////

// [SPEC] module_item ::=
//             module_or_generate_item
//            |port_declaration ';'
//            |{ attribute_instance } generated_instantiation
//            |{ attribute_instance } local_paramter_declaration
//            |{ attribute_instance } parameter_declaration
//            |{ attribute_instance } specify_block
//            |{ attribute_instance } specparam_declaration
// [SPEC] port_declaration ::=
//             { attribute_instance } inout_declaration
//            |{ attribute_instance } input_declaration
//            |{ attribute_instance } output_declaration
// 実は port_declarations 以外は non_port_module_item と同一なので
// そちらを使うと簡単に書ける．
// [SPEC] non_port_module_item ::=
//             { attribute_instance } generated_instantiation
//            |{ attribute_instance } module_or_generate_item
//            |{ attribute_instance } local_paramter_declaration
//            |{ attribute_instance } parameter_declaration
//            |{ attribute_instance } specify_block
//            |{ attribute_instance } specparam_declaration
// おいおい，module_or_generate_item はその定義の中に
// { attribute_instance }を含んでいるのでここでは不要でしょ．
// 文法規則に一貫性がないからこんな些細なミスが入り込むことになるのだよ．
// ところでここで言う item とは declaration も含んでおり, あとで処理
// を区別したいので item という形で返すことが難しい．
// そこで list_of_module_items という非終端節点をつくって，そこを
// 多重(並列)のリストにして item と declaration を区別している．
// また inout_declaration などはポート宣言で使うためにセミコロンを含まない
// 定義になっているのでここでセミコロンをつけなければいけないのに忘れている．
// とはいえ二重リストのときには inout_declhead という形で処理するので
// inout_declaration の最後にセミコロンを含めておいた方が分かりやすいので
// そうしている．

// [SPEC*] list_of_module_items ::= { module_item }
// 各要素の先頭の { attribute_instance } はここで処理する．
list_of_module_items
: // 空もありうる．
{
  $$ = PtHeadList();
}
| list_of_module_items io_declaration
{
  $$ = $1;
  $1.iohead_list.add($2);
}
| list_of_module_items module_decl
{
  $$ = $1;
  $1.declhead_list.add($2);
}
| list_of_module_items module_item
{
  $$ = $1;
  $1.item_list.add($2);
}
;

// [SPEC*] list_of_module_items ::= { module_item }
// 各要素の先頭の { attribute_instance } はここで処理する．
// こちらは io_declaration を含まない．
list_of_module_items2
: // 空もありうる．
{
  $$ = PtHeadList();
}
| list_of_module_items2 module_decl
{
  $$ = $1;
  $1.declhead_list.add($2);
}
| list_of_module_items2 module_item
{
  $$ = $1;
  $1.item_list.add($2);
}
;

// ai_list の処理を一つにまとめるため module_decl_body という
// 非終端節点を追加している．
module_decl
: ai_list module_or_generate_decl
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
| ai_list parameter_declaration
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
| ai_list specparam_declaration
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
;

io_declaration
: ai_list inout_declaration
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
| ai_list input_declaration
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
| ai_list output_declaration
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
;

module_item
: ai_list module_or_generate_item
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
| ai_list generated_instantiation
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
;

// [SPEC] module_or_generate_item ::=
//             { attribute_instance } module_or_generate_item_declaration
//            |{ attribute_instance } parameter_override
//            |{ attribute_instance } continuous_assign
//            |{ attribute_instance } gate_instantiation
//            |{ attribute_instance } udp_instantiation
//            |{ attribute_instance } module_instantiation
//            |{ attribute_instance } initial_construct
//            |{ attribute_instance } always_construct
// [SPEC] module_or_generate_item_declaration ::=
//             net_declaration
//            |reg_declaration
//            |integer_declaration
//            |real_declaration
//            |time_declaration
//            |realtime_declaration
//            |event_declaration
//            |genvar_declaration
//            |task_declaration
//            |function_declaration
// みての通り module_or_generate_item_declaration という非終端ノードを
// 展開している．
// 先頭の { attribute_instance } は module_or_generate_item の外側で処理する．
// 具体的には list_of_module_items と generate_item
// 展開しておいてから言うのも何だけど PtDeclHead 系と PtItem 系の2つに
// 分類している．ここでいう declaration は上と微妙に異なる．
module_or_generate_decl
: net_declaration
{
  $$ = $1;
}
| reg_declaration
{
  $$ = $1;
}
| integer_declaration
{
  $$ = $1;
}
| real_declaration
{
  $$ = $1;
}
| time_declaration
{
  $$ = $1;
}
| realtime_declaration
{
  $$ = $1;
}
| event_declaration
{
  $$ = $1;
}
| genvar_declaration
{
  $$ = $1;
}
;

module_or_generate_item
: task_declaration
{
  $$ = $1;
}
| function_declaration
{
  $$ = $1;
}
| parameter_override
{
  $$ = $1;
}
| continuous_assign
{
  $$ = $1;
}
| gate_instantiation
{
  $$ = $1;
}
| module_instantiation
{
  $$ = $1;
}
| initial_construct
{
  $$ = $1;
}
| always_construct
{
  $$ = $1;
}
;

// [SPEC] parameter_override ::= "defparam" list_of_param_assignments ';'
// list_of_param_assignments の parameter は階層なしだが defparam 文では
// 階層付きの名前を使ってこそ意味があるのでこれは仕様の明らかな誤り
// そのため defparam_assignment という非終端節点を用意している．
parameter_override
: DEFPARAM nzlist_of_defparam_assignment ';'
{
  $$ = parser.factory().new_DefParamH(@$, $2.top);
}
| DEFPARAM error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC*] nzlist_of_defparam_assignment ::=
//             defparam_assignment { ',' defparam_assignment }
nzlist_of_defparam_assignment
: defparam_assignment
{
  $$ = PtDefParamList();
  $$.init($1);
}
| nzlist_of_defparam_assignment ',' defparam_assignment
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] defparam_assignment ::=
//             hierarchical_identifier '=' constant_expression
defparam_assignment
: hierarchical_identifier '=' expression
{
  $$ = parser.factory().new_DefParam(@$, $1, $3);
}
| IDENTIFIER '=' expression
{
  $$ = parser.factory().new_DefParam(@$, $1, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.2 Declarations
//////////////////////////////////////////////////////////////////////
// A.2.1 Declaration types
//////////////////////////////////////////////////////////////////////
// A.2.1.1 Module parameter declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] local_parameter_declaration ::=
//             "localparam" ["signed"] [range] list_of_param_assignments ';'
//            |"localparam" "integer"          list_of_param_assignments ';'
//            |"localparam" "real"             list_of_param_assignments ';'
//            |"localparam" "realtime"         list_of_param_assignments ';'
//            |"localparam" "time"             list_of_param_assignments ';'
// [SPEC] parameter_declaration ::=
//             "parameter" ["signed"] [range] list_of_param_assignments ';'
//            |"parameter" "integer"          list_of_param_assignments ';'
//            |"parameter" "real"             list_of_param_assignments ';'
//            |"parameter" "realtime"         list_of_param_assignments ';'
//            |"parameter" "time"             list_of_param_assignments ';'
// この2つは区別しない．
parameter_declaration
: PARAMETER            list_of_param_assignments ';'
{
  $$ = parser.factory().new_ParamH(@$, $2.top);
}
| PARAMETER sign range list_of_param_assignments ';'
{
  $$ = parser.factory().new_ParamH(@$, $2, $3, $4.top);
}
| PARAMETER data_type list_of_param_assignments ';'
{
  $$ = parser.factory().new_ParamH(@$, $2, $3.top);
}
| LOCALPARAM          list_of_param_assignments ';'
{
  $$ = parser.factory().new_LocalParamH(@$, $2.top);
}
| LOCALPARAM sign range list_of_param_assignments ';'
{
  $$ = parser.factory().new_LocalParamH(@$, $2, $3, $4.top);
}
| LOCALPARAM data_type list_of_param_assignments ';'
{
  $$ = parser.factory().new_LocalParamH(@$, $2, $3.top);
}
| PARAMETER error ';'
{
  $$ = nullptr;
  yyerrok;
}
| LOCALPARAM error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] specparam_declaration ::= specparam ["range"]
//             list_of_specparam_assignments ';'
specparam_declaration
: SPECPARAM        list_of_specparam_assignments ';'
{
  $$ = parser.factory().new_SpecParamH(@$, $2.top);
}
| SPECPARAM range list_of_specparam_assignments ';'
{
  $$ = parser.factory().new_SpecParamH(@$, $2, $3.top);
}
| SPECPARAM error ';'
{
  $$ = nullptr;
  yyerrok;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.1.2 Port declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] inout_declaration ::=
//            "inout" [nettype] ["signed"] [range] list_of_port_identifiers ';'
// 末尾のセミコロンは IEEE1364-2001 の仕様書にはない．
inout_declaration
: INOUT          sign       list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Inout, $3.top);
}
| INOUT net_type sign       list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Inout, $2, $4.top);
}
| INOUT          sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Inout, $2, $3, $4.top);
}
| INOUT net_type sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Inout, $2, $3, $4, $5.top);
}
| INOUT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] input_declaration ::=
//            "input" [nettype]["signed"][range] list_of_port_identifiers ';'
// 末尾のセミコロンは IEEE1364-2001 の仕様書にはない．
input_declaration
: INPUT          sign       list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $3.top);
}
| INPUT net_type sign       list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Input, $2, $4.top);
}
| INPUT          sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $2, $3, $4.top);
}
| INPUT net_type sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Input, $2, $3, $4, $5.top);
}
| INPUT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] output_declaration ::=
//    "output" [nettype]["signed"][range] list_of_port_identifiers
//   |"output" ["reg"]  ["signed"][range] list_of_port_identifiers
//   |"output" "reg"    ["signed"][range] list_of_variable_port_identifiers
//   |"output" [output_variable_type]     list_of_port_identifiers
//   |"output" output_variable_type       list_of_variable_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
// [SPEC] output_variable_type ::= "integer"|"time"
// [SPEC] list_of_variable_port_identifiers ::=
//    port_identifier ['=' constant_expression]
//    {',' port_identifier ['=' constant_expression]}
// [SPEC] port_identifier ::= identifier
// inout,input と異なりちょっと複雑．
// まず [nettype] ではじまる形式と ["reg"]で始まる形式は省略時の区別が
// つかないので "reg" のほうは省略なしとする．
// 同様に [output_variable_type] も省略なしとする．
// 残りはリストが identifier のみの形式と
// identifier '=' constant_expression の形式の区別だが，
// "reg" および output_variable_type で始まった場合には両方がありうることが
// わかる．そのため，list_of_port_identifiers のみをとる形式と両方をとる形式
// にわける．
output_declaration
: OUTPUT          sign       list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $3.top);
}
| OUTPUT net_type sign       list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Output, $2, $4.top);
}
| OUTPUT          sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $2, $3, $4.top);
}
| OUTPUT net_type sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_NetIOHead(@$, VpiDir::Output, $2, $3, $4, $5.top);
}
| OUTPUT REG sign       list_of_variable_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $4.top);
}
| OUTPUT REG sign range list_of_variable_port_identifiers ';'
{
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $3, $4, $5.top);
}
| OUTPUT INTEGER        list_of_variable_port_identifiers ';'
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Output,
				      VpiVarType::Integer,
				      $3.top);
}
| OUTPUT TIME          list_of_variable_port_identifiers ';'
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Output,
				      VpiVarType::Time,
				      $3.top);
}
| OUTPUT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.1.3 Type declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] event_declaration ::= "event" list_of_event_identifiers ';'
event_declaration
: EVENT list_of_event_identifiers ';'
{
  $$ = parser.factory().new_EventH(@$, $2.top);
}
| EVENT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] genvar_declaration ::= "genvar" list_of_genvar_identifiers ';'
genvar_declaration
: GENVAR list_of_genvar_identifiers ';'
{
  $$ = parser.factory().new_GenvarH(@$, $2.top);
}
| GENVAR error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] integer_declaration ::= "integer" list_of_variable_identifiers ';'
integer_declaration
: INTEGER list_of_variable_identifiers ';'
{
  $$ = parser.factory().new_VarH(@$, VpiVarType::Integer, $2.top);
}
| INTEGER error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] net_declaration ::=
//    net_type                                           ["signed"]
//             [delay3] list_of_net_identifiers ';'
//   |net_type [drive_strength]                          ["signed"]
//             [delay3] list_of_net_decl_assignments ';'
//   |net_type                   ["vectored"|"scalared"] ["signed"] range
//             [delay3] list_of_net_identifiers ';'
//   |net_type [drive_strength]  ["vectored"|"scalared"] ["signed"] range
//             [delay3] list_of_net_decl_assignments ';'
//   |"trireg" [charge_strength]                         ["signed"]
//             [delay3] list_of_net_identifiers ';'
//   |"trireg" [drive_strength]                          ["signed"]
//             [delay3] list_of_net_decl_assignments ';'
//   |"trireg" [charge_strength] ["vectored"|"scalared"] ["signed"] range
//             [delay3] list_of_net_identifiers ';'
//   |"trireg" [drive_strength]  ["vectored"|"scalared"] ["signed"] range
//             [delay3] list_of_net_decl_assignments ';'
// これは難解．まず，最後が list_of_net_identifiers のパタンと
// list_of_net_decl_assignments のパタンに分ける．
// 良く見てみると drive_strength が省略されたパタンは両方にある．
// また charge_strength があるパタンは list_of_net_identifier のみである．
// そこで，list_of_net_identifiers と list_of_net_decl_assignments の両方
// がありうるパタンを net_declaration1 とし，list_of_net_identifiers のみ
// がありうるパタンを net_declaration2 とし，list_of_net_decl_assignments
// のみがありうるパタンを net_declaration3 とする．
net_declaration
: net_type sign                      list_of_net_decls ';'
{
  // net のデフォルトは1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, $1, $3.top);
}
| net_type        sign        delay3 list_of_net_decls ';'
{
  // net のデフォルトは1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, $1, $3, $4.top);
}
| net_type        sign range         list_of_net_decls ';'
{
  $$ = parser.factory().new_NetH(@$, $1, VpiVsType::None,
				 $2, $3, $4.top);
}
| net_type vstype sign range         list_of_net_decls ';'
{
  $$ = parser.factory().new_NetH(@$, $1, $2,
				 $3, $4, $5.top);
}
| net_type        sign range delay3 list_of_net_decls ';'
{
  $$ = parser.factory().new_NetH(@$, $1, VpiVsType::None,
				 $2, $3, $4, $5.top);
}
| net_type vstype sign range delay3 list_of_net_decls ';'
{
  $$ = parser.factory().new_NetH(@$, $1, $2, $3, $4, $5, $6.top);
}
| TRIREG sign                       list_of_net_decls ';'
{
  // 1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $3.top);
}
| TRIREG sign        delay3         list_of_net_decls ';'
{
  // 1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $3, $4.top);
}
| TRIREG        sign range         list_of_net_decls ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 VpiVsType::None,
				 $2, $3, $4.top);
}
| TRIREG vstype sign range         list_of_net_decls ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $3, $4, $5.top);
}
| TRIREG        sign range delay3 list_of_net_decls ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 VpiVsType::None,
				 $2, $3, $4, $5.top);
}
| TRIREG vstype sign range delay3 list_of_net_decls ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $3, $4, $5, $6.top);
}
| TRIREG charge_strength sign                list_of_net_identifiers ';'
{
  // 1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $4.top);
}
| TRIREG charge_strength sign         delay3 list_of_net_identifiers ';'
{
  // 1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $4, $5.top);
}
| TRIREG charge_strength        sign range list_of_net_identifiers ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, VpiVsType::None,
				 $3, $4, $5.top);
}
| TRIREG charge_strength vstype sign range list_of_net_identifiers ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $3, $4, $5, $6.top);
}
| TRIREG charge_strength        sign range delay3 list_of_net_identifiers ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, VpiVsType::None, $3, $4, $5, $6.top);
}
| TRIREG charge_strength vstype sign range delay3 list_of_net_identifiers ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $3, $4, $5, $6, $7.top);
}
| net_type drive_strength sign list_of_net_decl_assignments ';'
{
  // 1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, $1, $2, $4.top);
}
| net_type drive_strength sign        delay3 list_of_net_decl_assignments ';'
{
  // 1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, $1, $2, $4, $5.top);
}
| net_type drive_strength        sign range list_of_net_decl_assignments ';'
{
  $$ = parser.factory().new_NetH(@$, $1, $2,
				 VpiVsType::None, $3, $4, $5.top);
}
| net_type drive_strength vstype sign range list_of_net_decl_assignments ';'
{
  $$ = parser.factory().new_NetH(@$, $1, $2, $3, $4, $5, $6.top);
}
| net_type drive_strength        sign range delay3 list_of_net_decl_assignments ';'
{
  $$ = parser.factory().new_NetH(@$, $1, $2,
				 VpiVsType::None,
				 $3, $4, $5, $6.top);
}
| net_type drive_strength vstype sign range delay3 list_of_net_decl_assignments ';'
{
  $$ = parser.factory().new_NetH(@$, $1, $2, $3, $4, $5, $6, $7.top);
}
| TRIREG drive_strength sign list_of_net_decl_assignments ';'
{
  // 1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $4.top);
}
| TRIREG drive_strength sign        delay3 list_of_net_decl_assignments ';'
{
  // 1ビット
  // sign は無視
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $4, $5.top);
}
| TRIREG drive_strength        sign range list_of_net_decl_assignments ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, VpiVsType::None,
				 $3, $4, $5.top);
}
| TRIREG drive_strength vstype sign range list_of_net_decl_assignments ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $3, $4, $5, $6.top);
}
| TRIREG drive_strength        sign range delay3 list_of_net_decl_assignments ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, VpiVsType::None,
				 $3, $4, $5, $6.top);
}
| TRIREG drive_strength vstype sign range delay3 list_of_net_decl_assignments ';'
{
  $$ = parser.factory().new_NetH(@$, VpiNetType::TriReg,
				 $2, $3, $4, $5, $6, $7.top);
}
| net_type error ';'
{
  $$ = nullptr;
  yyerrok;
}
| TRIREG error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] real_declaration ::= "real" list_of_real_identifiers ';'
real_declaration
: REAL list_of_real_identifiers ';'
{
  $$ = parser.factory().new_VarH(@$, VpiVarType::Real, $2.top);
}
| REAL error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] realtime_declaration ::= "realtime" list_of_real_identifiers ';'
realtime_declaration
: REALTIME list_of_real_identifiers ';'
{
  $$ = parser.factory().new_VarH(@$, VpiVarType::Realtime, $2.top);
}
| REALTIME error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] reg_declaration ::= "reg" ["signed"] [range]
//             list_of_variable_identifiers ';'
reg_declaration
: REG sign list_of_variable_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_RegH(@$, $3.top);
}
| REG sign range list_of_variable_identifiers ';'
{
  $$ = parser.factory().new_RegH(@$, $2, $3, $4.top);
}
| REG error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] time_declaration ::= "time" list_of_variable_identifiers ';'
time_declaration
: TIME list_of_variable_identifiers ';'
{
  $$ = parser.factory().new_VarH(@$, VpiVarType::Time, $2.top);
}
| TIME error ';'
{
  $$ = nullptr;
  yyerrok;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.2 Declaration data types
//////////////////////////////////////////////////////////////////////
// A.2.2.1 Net and variable types
//////////////////////////////////////////////////////////////////////

// [SPEC] net_type ::=
//             "supply0"|"supply1"
//            |"tri"|"triand"|"trior"|"tri0"|"tri1"
//            |"wire"|"wand"|"wor"
net_type
: SUPPLY0
{
  $$ = VpiNetType::Supply0;
}
| SUPPLY1
{
  $$ = VpiNetType::Supply1;
}
| TRI
{
  $$ = VpiNetType::Tri;
}
| TRIAND
{
  $$ = VpiNetType::TriAnd;
}
| TRIOR
{
  $$ = VpiNetType::TriOr;
}
| TRI0
{
  $$ = VpiNetType::Tri0;
}
| TRI1
{
  $$ = VpiNetType::Tri1;
}
| WIRE
{
  $$ = VpiNetType::Wire;
}
| WAND
{
  $$ = VpiNetType::Wand;
}
| WOR
{
  $$ = VpiNetType::Wor;
}
;

// [SPEC*] sign ::= "signed"|null
sign
: SIGNED
{
  $$ = true;
}
| // 空
{
  $$ = false;
}
;

// [SPEC*] vstype ::= "vectored"|"scalared"
vstype
: VECTORED
{
  $$ = VpiVsType::Vectored;
}
| SCALARED
{
  $$ = VpiVsType::Scalared;
}
;

// [SPEC] output_variable_type ::= "integer"|"time"

// [SPEC] real_type ::=
//             real_identifier ['=' constant_expression]
//            |real_identifier dimension { dimension }
// 1番目の形式で '=' 以下が省略されたときが含まれていないように見えるが,
// 実は2番目の形式が dimension が無い場合を含んでいる．この場合,
// identifier のみになるのでこちらでカバーしている．
// [SPEC] real_identifier ::= identifier
real_type
: IDENTIFIER '=' expression
{
  $$ = parser.factory().new_DeclItem(@$, $1, $3);
}
| identifier_with_range
{
  $$ = $1;
}
;

// [SPEC] variable_type ::=
//   variable_identifier ['=' constant_expression]
//  |variable_identifier dimension { dimension }
// [SPEC] variable_identifier ::= identifier
// 1番目の形式で '=' 以下が省略されたときが含まれていないように見えるが,
// 実は2番目の形式が dimension が無い場合を含んでいる．この場合,
// identifier のみになるのでこちらでカバーしている．
variable_type
: IDENTIFIER '=' expression
{
  $$ = parser.factory().new_DeclItem(@$, $1, $3);
}
| identifier_with_range
{
  $$ = $1;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.2.2 Strength
//////////////////////////////////////////////////////////////////////

// [SPEC] drive_strength ::=
//             '(' strength0 ',' strength1 ')'
//            |'(' strength1 ',' strength0 ')'
//            |'(' strength0 ',' "highz1"')'
//            |'(' strength1 ',' "highz0" ')'
//            |'(' "highz1" ',' strength0 ')'
//            |'(' "highz0" ',' strength1 ')'
drive_strength
: '(' strength0 ',' strength1 ')'
{
  $$ = parser.factory().new_Strength(@$, $2, $4);
}
| '(' strength1 ',' strength0 ')'
{
  // 順序を入れ替える．
  $$ = parser.factory().new_Strength(@$, $4, $2);
}
| '(' strength0 ',' HIGHZ1 ')'
{
  $$ = parser.factory().new_Strength(@$, $2, VpiStrength::HiZ);
}
| '(' strength1 ',' HIGHZ0 ')'
{
  // 順序を入れ替える．
  $$ = parser.factory().new_Strength(@$, VpiStrength::HiZ, $2);
}
| '(' HIGHZ1    ',' strength0 ')'
{
  // 順序を入れ替える．
  $$ = parser.factory().new_Strength(@$, $4, VpiStrength::HiZ);
}
| '(' HIGHZ0    ',' strength1 ')'
{
  $$ = parser.factory().new_Strength(@$, VpiStrength::HiZ, $4);
}
;

// [SPEC] strength0 ::= "supply0"|"strong0"|"pull0"|"weak0"
strength0
: SUPPLY0
{
  $$ = VpiStrength::SupplyDrive;
}
| STRONG0
{
  $$ = VpiStrength::StrongDrive;
}
| PULL0
{
  $$ = VpiStrength::PullDrive;
}
| WEAK0
{
  $$ = VpiStrength::WeakDrive;
}
;

// [SPEC] strength1 ::= "supply1"|"strong1"|"pull1"|"weak1"
strength1
: SUPPLY1
{
  $$ = VpiStrength::SupplyDrive;
}
| STRONG1
{
  $$ = VpiStrength::StrongDrive;
}
| PULL1
{
  $$ = VpiStrength::PullDrive;
}
| WEAK1
{
  $$ = VpiStrength::WeakDrive;
}
;

// [SPEC] charge_strength ::= '(' "small" ')'|'(' "medium" ')'|'(' "large" ')'
charge_strength
: '(' SMALL ')'
{
  $$ = parser.factory().new_Strength(@$, VpiStrength::SmallCharge);
}
| '(' MEDIUM ')'
{
  $$ = parser.factory().new_Strength(@$, VpiStrength::MediumCharge);
}
| '(' LARGE ')'
{
  $$ = parser.factory().new_Strength(@$, VpiStrength::LargeCharge);
}
;


//////////////////////////////////////////////////////////////////////
// A.2.2.3 Delays
//////////////////////////////////////////////////////////////////////

// [SPEC] delay3 ::= '#'delay_value
//                  |'#''('delay_value[',' delay_value[',' delay_value]]')'
// [SPEC] delay2 ::= '#'delay_value
//                  |'#''('delay_value[',' delay_value]')'
// ちなみに delay2 と delay3 は共通部分があるので delay2 を使って delay3 を
// 簡潔に表している．
// ひとつの問題は '(' delay_value ')' の形は delay_value(実は expression)
// 自体が括弧を含むので適切に還元できない．
// そこでここでは '(' delay_value ')' の形は取り除いている．
delay3
: delay2
{
  $$ = $1;
}
| '#' '(' delay_value_y ',' delay_value_y ',' delay_value_y ')'
{
  $$ = parser.factory().new_Delay(@$, $3, $5, $7);
}
;

delay2
: '#' delay_value
{
  $$ = parser.factory().new_Delay(@$, $2);
}
| '#' '(' delay_value_y ',' delay_value_y ')'
{
  $$ = parser.factory().new_Delay(@$, $3, $5);
}
;

delay_value_y
: delay_value
{
  $$ = $1;
}
| '(' mintypmax_expression ')'
{
  $$ = $2;
}
;

// [SPEC] delay_value ::=
//             unsigned number
//            |parameter_identifier
//            |specparam_identifier
//            |mintypmax_expression
// [SPEC] parameter_identifier ::= identifier
// [SPEC] specparam_identifier ::= identifier
// 最もやっかいな構文規則の一つ．
// parameter_identifier と specparam_identifier は構文規則上は同じトークン
// なのだがこれくらいは笑って許すとしても, そもそも mintypmax_expression に
// unsigned number や identifier は含まれてしまうので冗長である．
// また，意味的にはこれは constant_mintypmax_expression ではないかと思われる．
// 実際, IEEE1364(2001ではなく)では constant_mintypmax_expression になって
// いた．
// さらにさらに IEEE1364-2001 では constant_function が constant_expression
// に追加されたおかげで，
// gatetype '#' delay_value '(' nzlist_of_expressions ')'
// の delay_value とつぎの '(' の切目が曖昧になってしまった．
// つまり， delay_value の最後が identifier の時に次の '(' を読んで
// constant_function だと思ってシフトするのかそれとも identifier
// で還元して delay_value を終わらせるのかが分からない．
// これは明らかに文法上のミスである．
// とりあえず，括弧無しの形式は UNUMBER/RNUMBER と identifier しか認めない
// 様にした．括弧をつければ全部うまく行く．
delay_value
: delay_value_x
{
  $$ = $1;
}
| delay_value_x ':' delay_value_x ':' delay_value_x
{
  $$ = parser.factory().new_MinTypMax(@$, $1, $3, $5);
}
| '(' expression ')'
{
  $$ = parser.factory().new_Opr(@$, VpiOpType::Null, $2);
}
;
/*
| '(' mintypmax_expression ')'

*/

delay_value_x
: unumber
{
  $$ = $1;
}
| rnumber
{
  $$ = $1;
}
| IDENTIFIER
{
  $$ = parser.factory().new_Primary(@1, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.2.3 Declaration lists
//////////////////////////////////////////////////////////////////////

// [SPEC] list_of_event_identifiers ::=
//             event_identifier [dimension {dimension}]
//                 {',' event_identifier [dimension {dimension}]}
// [SPEC] event_identifier ::= identifier
// event_identifier [dimension {dimension}]
// は identifier_with_range という非終端節点で代用できる．
// しかしなんで event_identifier {dimension} と書かないのかね？
// これも Verilog1995 -> Verilog2001 の修正が不十分な証拠
list_of_event_identifiers
: identifier_with_range
{
  $$ = PtDeclItemList();
  $$.init($1);
}
| list_of_event_identifiers ','identifier_with_range
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] list_of_genvar_identifiers ::=
//             genvar_identifier {',' genvar_identifier}
list_of_genvar_identifiers
: IDENTIFIER
{
  auto item = parser.factory().new_DeclItem(@1, $1);
  $$ = PtDeclItemList();
  $$.init(item);
}
| list_of_genvar_identifiers ',' IDENTIFIER
{
  auto item = parser.factory().new_DeclItem(@3, $3);
  $$ = $1;
  $$.add(item);
}
;

// [SPEC*] list_of_net_decls ::=
//             list_of_net_identifiers|list_of_net_decl_assignments
list_of_net_decls
: list_of_net_identifiers
{
  $$ = $1;
}
| list_of_net_decl_assignments
{
  $$ = $1;
}
;

// [SPEC] list_of_net_decl_assignments ::=
//             net_decl_assignment {',' net_decl_assignment}
list_of_net_decl_assignments
: net_decl_assignment
{
  $$ = PtDeclItemList();
  $$.init($1);
}
| list_of_net_decl_assignments ',' net_decl_assignment
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] list_of_net_identifiers ::=
//             net_identifier [dimension {dimension}]
//                 {',' net_identifier [dimension {dimension}]}
// [SPEC] net_identifier ::= identifier
// net_identifier [dimension {dimension}]
// は identifier_with_range という非終端節点で代用できる．
// しかしなんで net_identifier {dimension} と書かないのかね？
// これも Verilog1995 -> Verilog2001 の修正が不十分な証拠
list_of_net_identifiers
: identifier_with_range
{
  $$ = PtDeclItemList();
  $$.init($1);
}
| list_of_net_identifiers ',' identifier_with_range
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] list_of_param_assignments ::= param_assignment {',' param_assignment}
list_of_param_assignments
: param_assignment
{
  $$ = PtDeclItemList();
  $$.init($1);
}
| list_of_param_assignments ',' param_assignment
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
// [SPEC] port_identifier ::= identifier
list_of_port_identifiers
: port_identifier_item
{
  $$ = PtIOItemList();
  $$.init($1);
}
| list_of_port_identifiers ',' port_identifier_item
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] list_of_real_identifiers ::= real_type {',' real_type}
list_of_real_identifiers
: real_type
{
  $$ = PtDeclItemList();
  $$.init($1);
}
| list_of_real_identifiers ',' real_type
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] list_of_specparam_assignments ::=
//             specparam_assignment {',' specparam_assignment}
list_of_specparam_assignments
: specparam_assignment
{
  $$ = PtDeclItemList();
  $$.init($1);
}
| list_of_specparam_assignments ',' specparam_assignment
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] list_of_variable_identifiers ::=
//             variable_type {',' variable_type}
list_of_variable_identifiers
: variable_type
{
  $$ = PtDeclItemList();
  $$.init($1);
}
| list_of_variable_identifiers ',' variable_type
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] list_of_variable_port_identifiers ::=
//             port_identifier ['=' constant_expression]
//                 {',' port_identifier ['=' constant_expression]}
list_of_variable_port_identifiers
: variable_port_identifier_item
{
  $$ = PtIOItemList();
  $$.init($1);
}
| list_of_variable_port_identifiers ',' variable_port_identifier_item
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] port_identifier_item ::= port_identifier
// [SPEC] port_identifier ::= identifier
// これは構文規則よりもアクションが重要
port_identifier_item
: IDENTIFIER
{
  $$ = parser.factory().new_IOItem(@1, $1);
}
;

// [SPEC*] variable_port_identifier_item ::=
//              port_identifier ['=' constant_expression]
// [SPEC] port_identifier ::= identifier
variable_port_identifier_item
: IDENTIFIER
{
  $$ = parser.factory().new_IOItem(@1, $1);
}
| IDENTIFIER '=' expression
{
  $$ = parser.factory().new_IOItem(@1, $1, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.2.4 Declaration assignments
//////////////////////////////////////////////////////////////////////

// [SPEC] net_decl_assignment ::= net_identifier '=' expression
// [SPEC] net_identifier ::= identifier
net_decl_assignment
: IDENTIFIER '=' expression
{
  $$ = parser.factory().new_DeclItem(@$, $1, $3);
}
;

// [SPEC] param_assignment ::= parameter_identifier '=' constant_expression
// [SPEC] parameter_identifier ::= identifier
param_assignment
: IDENTIFIER '=' expression
{
  $$ = parser.factory().new_DeclItem(@$, $1, $3);
}
;

// [SPEC] specparam_assignment ::=
//             specparam_identifier '=' constant_mintypmax_expression
//            |pulse_control_specparam
// [SPEC] specparam_identifier ::= identifier
// [SPEC] pulse_control_specparam ::=
//    "PATHPULSE$" '=' '(' reject_limit_value [',' error_limit_value] ')'';'
//   |"PATHPULSE$"specify_input_terminal_descriptor
//     '$'specify_output_terminal_descriptor
//     '=' '(' reject_limit_value [','error_limit_value] ')'';'
// [SPEC] error_limit_value ::= limit_value
// [SPEC] reject_limit_value ::= limit_value
// [SPEC] limit_value ::= constant_mintypmax_expression
// あれ？ pulse_control_specparam の末尾のセミコロンは不要じゃないの？
// mintypmax_expression のところで書いてあるように mintypmax_expression
// に expression は含まれないので expression を明示的に加えている．
specparam_assignment
: IDENTIFIER '=' expression
{
  $$ = parser.factory().new_DeclItem(@$, $1, $3);
}
| IDENTIFIER '=' mintypmax_expression
{
  $$ = parser.factory().new_DeclItem(@$, $1, $3);
}
| PATHPULSE '=' '(' mintypmax_expression ')'
{
  // $$ = parser.new_specparam(@$, nullptr, nullptr, $4, nullptr);
  $$ = nullptr;
}
| PATHPULSE '=' '(' mintypmax_expression ','
                    mintypmax_expression ')'
{
  // $$ = parser.new_specparam(@$, nullptr, nullptr, $4, $6);
  $$ = nullptr;
}
| PATHPULSE specify_terminal '$' specify_terminal '='
                '(' mintypmax_expression ')'
{
  // $$ = parser.new_specparam(@$, $2, $4, $7, nullptr);
  $$ = nullptr;
}
| PATHPULSE specify_terminal '$' specify_terminal '='
                '(' mintypmax_expression ','
                    mintypmax_expression ')'
{
  // $$ = parser.new_specparam(@$, $2, $4, $7, $9);
  $$ = nullptr;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.5 Declaration ranges
//////////////////////////////////////////////////////////////////////

// [SPEC] dimension ::= '[' dimension_constant_expression ':'
//                  dimension_constant_expression ']'
// [SPEC] dimension_constant_expression ::= constant_expression
// dimension_constant_expression などという非終端ノードは不必要

// [SPEC*] nzlist_of_dimensions ::= dimension { dimension }
nzlist_of_dimensions
: range
{
  $$ = PtRangeList();
  $$.init($1);
}
| nzlist_of_dimensions range
{
  $$ = $1;
  $$.add($2);
}
;

// [SPEC] range ::= '[' msb_constant_expression ':' lsb_constant_expression ']'
// [SPEC] msb_constant_expression ::= constant_expression
// [SPEC] lsb_constant_expression ::= constant_expression
// この msb_constant_expression/lsb_constant_expression などは構文規則と
// 意味づけを混同している良い(悪い)例である．
range
: '[' expression ':' expression ']'
{
  $$ = parser.factory().new_Range(@$, $2, $4);
}
;

// 範囲指定
part
: '[' expression ':' expression ']'
{
  $$ = parser.factory().new_PartConst(@$, $2, $4);
}
| '[' expression PLUSCOLON expression ']'
{
  $$ = parser.factory().new_PartPlus(@$, $2, $4);
}
| '[' expression MINUSCOLON expression ']'
{
  $$ = parser.factory().new_PartMinus(@$, $2, $4);
}
;

//////////////////////////////////////////////////////////////////////
// A.2.6 Function declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] function_declaration ::=
//             "function" ["automatic"] ["signed"] [range_or_type]
//                 function_identifier ';'
//                 function_item_declaration { function_item_declaration }
//                 function_statement
//             "endfunction"
//            |"function" ["automatic"] ["signed"] [range_or_type]
//                 function_identifier '(' function_port_list ')'';'
//                 block_item_declaration { block_item_declaration }
//                function_statement
//             "endfunction"
// [SPEC] range_or_type ::= range|"integer"|"real"|"realtime"|"time"
// IEEE1364-2001 にはこう書いてあるけど "signed" をつけて意味のあるのは
// range だけなので実際には
// ["signed"] [range] と
// "integer"|"real"|"realtime"|"time"
// の2種類なんじゃないかと思う．
// ここでは忠実に記録しておく．
// function_statement は statement で代用する．
// 読み込んだあとでチェックする．
function_declaration
: FUNCTION opt_auto sign IDENTIFIER ';'
  nzlist_of_fitem_decl
  statement
  ENDFUNCTION
{
  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( parser.check_function_statement($7) ) {
    $$ = parser.factory().new_Function(@$, $4, $2, $3,
				       $6.iohead_list.top,
				       $6.declhead_list.top,
				       $7);
  }
  else {
    $$ = nullptr;
  }
}
| FUNCTION opt_auto sign range IDENTIFIER ';'
  nzlist_of_fitem_decl
  statement
  ENDFUNCTION
{
  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( parser.check_function_statement($8) ) {
    $$ = parser.factory().new_SizedFunc(@$, $5, $2, $3, $4,
					$7.iohead_list.top,
					$7.declhead_list.top,
					$8);
  }
  else {
    $$ = nullptr;
  }
}
| FUNCTION opt_auto sign data_type IDENTIFIER ';'
  nzlist_of_fitem_decl
  statement
  ENDFUNCTION
{
  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( parser.check_function_statement($8) ) {
    $$ = parser.factory().new_TypedFunc(@$, $5, $2, $3, $4,
					$7.iohead_list.top,
					$7.declhead_list.top,
					$8);
  }
  else {
    $$ = nullptr;
  }
}
| FUNCTION opt_auto sign IDENTIFIER '(' function_port_list ')' ';'
  list_of_bitem_decl
  statement
  ENDFUNCTION
{
  // Verilog2001 style
  // list_of_bitem_decl には IOHead は含まれない．

  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( parser.check_function_statement($10) ) {
    $$ = parser.factory().new_Function(@$, $4, $2, $3,
				       $6.head_list.top,
				       $9.declhead_list.top,
				       $10);
  }
  else {
    $$ = nullptr;
  }
}
| FUNCTION opt_auto sign range
  IDENTIFIER '(' function_port_list ')' ';'
  list_of_bitem_decl
  statement
  ENDFUNCTION
{
  // Verilog2001 style
  // list_of_bitem_decl には IOHead は含まれない．

  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( parser.check_function_statement($11) ) {
    $$ = parser.factory().new_SizedFunc(@$, $5, $2, $3, $4,
					$7.head_list.top,
					$10.declhead_list.top,
					$11);
  }
  else {
    $$ = nullptr;
  }
}
| FUNCTION opt_auto sign data_type IDENTIFIER '(' function_port_list ')' ';'
  list_of_bitem_decl
  statement
  ENDFUNCTION
{
  // Verilog2001 style
  // list_of_bitem_decl には IOHead は含まれない．

  // 関数内で使用できないステートメントが含まれていないかチェック
  if ( parser.check_function_statement($11) ) {
    $$ = parser.factory().new_TypedFunc(@$, $5, $2, $3, $4,
					$7.head_list.top,
					$10.declhead_list.top,
					$11);
  }
  else {
    $$ = nullptr;
  }
}
| FUNCTION error ENDFUNCTION
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC*] nzlist_of_fitem_decl ::=
//             ai_list function_item_declaration
//               { ai_list function_item_declaration}
// [SPEC] function_item_declaration ::=
//             block_item_declaration
//            |tf_input_declaration ';'
// たぶん tf_input_declaration の前に { attribute_instance } がつくと思う．
// これ 2001 の拡張なのでどの位置に書くのか統一がとれていない．
// セミコロンは tf_input_declaration 中で処理する．
nzlist_of_fitem_decl
: block_item_declaration
{
  $$ = PtHeadList();
  $$.declhead_list.add($1);
}
| tf_input_declaration
{
  $$ = PtHeadList();
  $$.iohead_list.add($1);
}
| nzlist_of_fitem_decl block_item_declaration
{
  $$ = $1;
  $1.declhead_list.add($2);
}
| nzlist_of_fitem_decl tf_input_declaration
{
  $$ = $1;
  $1.iohead_list.add($2);
}
;

// [SPEC*] list_of_bitem_decl ::= { block_item_declaration }
list_of_bitem_decl
: // 空
{
  $$ = PtHeadList();
}
| list_of_bitem_decl block_item_declaration
{
  $$ = $1;
  $1.declhead_list.add($2);
}
;

// [SPEC] function_item_declaration ::=
//             block_item_declaration
//            |tf_input_declaration ';'
// たぶん tf_input_declaration の前に { attribute_instance } がつくと思う．
// これ 2001 の拡張なのでどの位置に書くのか統一がとれていない．

// [SPEC] function_port_list ::=
//             { attribute_instance } tf_input_declaration
//                 {',' { attribute_instance } tf_input_declaration }
function_port_list
: ai_list tf_input_declhead
{
  $$ = PtIODList();
  $$.init($2);
  parser.reg_attrinst($2, $1);
}
| function_port_list ',' ai_list tf_input_declhead
{
  $$ = $1;
  $$.add_head($4);
  parser.reg_attrinst($4, $3);
}
| function_port_list ',' port_identifier_item
{
  $$ = $1;
  $1.add_item($3);
}
;

tf_input_declhead
: INPUT     sign port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $3);
}
| INPUT REG sign port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Input, $4);
}
| INPUT     sign range port_identifier_item
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $2, $3, $4);
}
| INPUT REG sign range port_identifier_item
{
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Input, $3, $4, $5);
}
| INPUT task_port_type port_identifier_item
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Input, $2, $3);
}
;

// [SPEC*] data_type ::=
//             |"integer"
//             |"real"
//             |"realtime"
//             |"time"
data_type
: INTEGER
{
  $$ = VpiVarType::Integer;
}
| REAL
{
  $$ = VpiVarType::Real;
}
| REALTIME
{
  $$ = VpiVarType::Realtime;
}
| TIME
{
  $$ = VpiVarType::Time;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.7 Task declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] task_declaration ::=
//             "task" ["automatic"] task_identifier ';'
//                 { task_item_declaration }
//                 statement
//             "endtask"
//            |"task" ["automatic"] task_identifier '(' task_port_list ')' ';'
//                 { block_item_declaration }
//                 statement
//             "endtask"
// この statement は statement_or_null でよいそうだ．
// Verilog-XL もそうなっている．
task_declaration
: TASK opt_auto IDENTIFIER ';'
  list_of_titem_decl
  statement_or_null
  ENDTASK
{
  $$ = parser.factory().new_Task(@$, $3, $2,
				 $5.iohead_list.top,
				 $5.declhead_list.top,
				 $6);
}
| TASK opt_auto IDENTIFIER '(' task_port_list ')' ';'
  list_of_bitem_decl
  statement_or_null
  ENDTASK
{
  $$ = parser.factory().new_Task(@$, $3, $2,
				 $5.head_list.top,
				 $8.declhead_list.top,
				 $9);
}
| TASK error ENDTASK
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC*] opt_auto ::=
//            /* 空 */
//           | "automatic"
opt_auto
:
{
  $$ = false;
}
| AUTOMATIC
{
  $$ = true;
}
;

// [SPEC*] list_of_task_item_declarations ::=
//             { task_item_declaration }
// セミコロンは tf_XXX_declaration 中で処理する．
list_of_titem_decl
: // 空
{
  $$ = PtHeadList();
}
| list_of_titem_decl block_item_declaration
{
  $$ = $1;
  $1.declhead_list.add($2);
}
| list_of_titem_decl tf_io_declaration
{
  $$ = $1;
  $1.iohead_list.add($2);
}
;

tf_io_declaration
: tf_input_declaration
{
  $$ = $1;
}
| tf_output_declaration
{
  $$ = $1;
}
| tf_inout_declaration
{
  $$ = $1;
}
;

// [SPEC] task_item_declaration ::=
//             block_item_declaration
//            |{ attribute_instance } tf_input_declaration ';'
//            |{ attribute_instance } tf_output_declaration ';'
//            |{ attribute_instance } tf_inout_declaration ';'
// block_item_declaration の先頭にも { attribute_instance } がつくので
// まとめて list_of_titem_decl で処理する．

// [SPEC] task_port_list ::= task_port_item {',' task_port_item }
// [SPEC] task_port_item ::=
//             { attribute_instance } tf_input_declaration
//            |{ attribute_instance } tf_output_declaration
//            |{ attribute_instance } tf_inout_declaration
task_port_list
: tf_input_declhead
{
  $$ = PtIODList();
  $$.init($1);
}
| tf_output_declhead
{
  $$ = PtIODList();
  $$.init($1);
}
| tf_inout_declhead
{
  $$ = PtIODList();
  $$.init($1);
}
| task_port_list ',' tf_input_declhead
{
  $$ = $1;
  $$.add_head($3);
}
| task_port_list ',' tf_output_declhead
{
  $$ = $1;
  $$.add_head($3);
}
| task_port_list ',' tf_inout_declhead
{
  $$ = $1;
  $$.add_head($3);
}
| task_port_list ',' port_identifier_item
{
  $$ = $1;
  $1.add_item($3);
}
;

// [SPEC] tf_input_declaration ::=
//             "input" ["reg"] ["signed"] [range] list_of_port_identifiers
//            |"input" [task_port_type]           list_of_port_identifiers
tf_input_declaration
: INPUT     sign list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $3.top);
}
| INPUT REG sign list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Input, $4.top);
}
| INPUT     sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $2, $3, $4.top);
}
| INPUT REG sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Input, $3, $4, $5.top);
}
| INPUT task_port_type list_of_port_identifiers ';'
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Input, $2, $3.top);
}
| INPUT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] tf_output_declaration ::=
//             "output" ["reg"] ["signed"] [range] list_of_port_identifiers
//            |"output" [task_port_type]           list_of_port_identifiers
tf_output_declaration
: OUTPUT     sign list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $3.top);
}
| OUTPUT REG sign list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $4.top);
}
| OUTPUT     sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $2, $3, $4.top);
}
| OUTPUT REG sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $3, $4, $5.top);
}
| OUTPUT task_port_type list_of_port_identifiers ';'
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Output, $2, $3.top);
}
| OUTPUT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC*] tf_output_declhead ::=
//              "output" ["reg"] ["signed"] [range]
//             |"output" [task_port_type]
// reg, signed, range が省略された場合と task_port_type が省略された
// 場合の区別がつかないので2番めの形は task_port_type の省略無しとする．
tf_output_declhead
: OUTPUT     sign port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $3);
}
| OUTPUT REG sign port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $4);
}
| OUTPUT     sign range port_identifier_item
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $2, $3, $4);
}
| OUTPUT REG sign range port_identifier_item
{
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $3, $4, $5);
}
| OUTPUT task_port_type port_identifier_item
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Output, $2, $3);
}
;

// [SPEC] tf_inout_declaration ::=
//             inout ["reg"] ["signed"] [range] list_of_port_identifiers
//            |inout [task_port_type]           list_of_port_identifiers
tf_inout_declaration
: INOUT     sign list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Inout, $3.top);
}
| INOUT REG sign list_of_port_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Inout, $4.top);
}
| INOUT     sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Inout, $2, $3, $4.top);
}
| INOUT REG sign range list_of_port_identifiers ';'
{
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Inout, $3, $4, $5.top);
}
| INOUT task_port_type list_of_port_identifiers ';'
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Inout, $2, $3.top);
}
| INOUT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC*] tf_inout_declhead ::=
//              inout ["reg"] ["signed"] [range]
//             |inout [task_port_type]
// reg, signed, range が省略された場合と task_port_type が省略された
// 場合の区別がつかないので2番めの形は task_port_type の省略無しとする．
tf_inout_declhead
: INOUT     sign port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_IOHead(@$, VpiDir::Inout, $3);
}
| INOUT REG sign port_identifier_item
{
  // sign は無視
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Inout, $4);
}
| INOUT     sign range port_identifier_item
{
  $$ = parser.factory().new_IOHead(@$, VpiDir::Inout, $2, $3, $4);
}
| INOUT REG sign range port_identifier_item
{
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Inout, $3, $4, $5);
}
| INOUT task_port_type port_identifier_item
{
  $$ = parser.factory().new_VarIOHead(@$, VpiDir::Inout, $2, $3);
}
;

// [SPEC] task_port_type ::= "time"|"real"|"realtime"|"integer"
// じつは data_type と同じ
task_port_type
: TIME
{
  $$ = VpiVarType::Time;
}
| REAL
{
  $$ = VpiVarType::Real;
}
| REALTIME
{
  $$ = VpiVarType::Realtime;
}
| INTEGER
{
  $$ = VpiVarType::Integer;
}
;


//////////////////////////////////////////////////////////////////////
// A.2.8 Block item declaration
//////////////////////////////////////////////////////////////////////

// [SPEC] block_item_declaration ::=
//             { attribute_instance } block_reg_declaration
//            |{ attribute_instance } event_declaration
//            |{ attribute_instance } integer_declaration
//            |{ attribute_instance } local_parameter_declaration
//            |{ attribute_instance } parameter_declaration
//            |{ attribute_instance } real_declaration
//            |{ attribute_instance } realtime_declaration
//            |{ attribute_instance } time_declaration
// 先頭の { attribute_instance } は block_item_declaration を
// 使っているところで処理する．
// 具体的には nzlist_of_fitem_decl, list_of_bitem_decl, list_of_titem_decl
block_item_declaration
: ai_list bitem_decl_body
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
;

bitem_decl_body
: block_reg_declaration
{
  $$ = $1;
}
| event_declaration
{
  $$ = $1;
}
| integer_declaration
{
  $$ = $1;
}
| parameter_declaration
{
  $$ = $1;
}
| real_declaration
{
  $$ = $1;
}
| realtime_declaration
{
  $$ = $1;
}
| time_declaration
{
  $$ = $1;
}
;

// [SPEC] block_reg_declaration ::= "reg" ["signed"] [range]
//                 list_of_block_variable_identifiers ';'
// reg_declaration との違いは初期値を持てないこと．
block_reg_declaration
: REG sign list_of_block_variable_identifiers ';'
{
  // sign は無視
  $$ = parser.factory().new_RegH(@$, $3.top);
}
| REG sign range list_of_block_variable_identifiers ';'
{
  $$ = parser.factory().new_RegH(@$, $2, $3, $4.top);
}
| REG error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] list_of_block_variable_identifiers ::=
//             block_variable_type {',' block_variable_type }
list_of_block_variable_identifiers
: block_variable_type
{
  $$ = PtDeclItemList();
  $$.init($1);
}
| list_of_block_variable_identifiers ',' block_variable_type
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] block_variable_type ::=
//             variable_identifier
//            |variable_identifier dimension {dimension}
// [SPEC] variable_identifier ::= identifier
// なんども出てきているように identifier_with_range は空も含むので
// 一通りでよい．
block_variable_type
: identifier_with_range
{
  $$ = $1;
}
;

// [SPEC*] identifier_with_range ::= identifier {dimension}
identifier_with_range
: IDENTIFIER
{
  $$ = parser.factory().new_DeclItem(@1, $1);
}
| IDENTIFIER nzlist_of_dimensions
{
  $$ = parser.factory().new_DeclItem(@$, $1, $2.top);
}
;


//////////////////////////////////////////////////////////////////////
// A.3 Primitive instances
// A.3.1 Primitive instantiation and instances
//////////////////////////////////////////////////////////////////////

// [SPEC] gate_instantiation ::=
//             cmos_switchtype [delay3] cmos_switch_instance
//                 {',' cmos_switch_instance } ';'
//            |enable_gatetype [drive_strength] [delay3] enable_gate_instance
//                 {',' enble_gate_instance } ';'
//            |mos_switchtype [delay3] mos_switch_instance
//                 {',' mos_switch_instance } ';'
//            |n_input_gatetype [drive_strength] [delay2] n_input_gate_instance
//                 {',' n_input_gate_instance } ';'
//            |n_output_gatetype [drive_strength] [delay2]
//                 n_output_gate_instance {',' n_output_gate_instance } ';'
//            |pass_en_switchtype [delay3] pass_enable_switch_instance
//                 {',' pass_enable_switch_instance } ';'
//            |pass_switchtype pass_switch_instance
//                 {',' pass_switch_instance } ';'
//            |"pulldown" [pulldown_strength] pull_gate_instance
//                 {',' pull_gate_instance } ';'
//            |"pullup" [pullup_strength] pull_gate_instance
//                 {',' pull_gate_instance } ';'
gate_instantiation
: cmos_switchtype                        nzlist_of_cmos_switch_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2.top);
}
| cmos_switchtype                 delay3 nzlist_of_cmos_switch_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| enable_gatetype                        nzlist_of_enable_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2.top);
}
| enable_gatetype                 delay3 nzlist_of_enable_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| enable_gatetype  drive_strength        nzlist_of_enable_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| enable_gatetype  drive_strength delay3 nzlist_of_enable_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3, $4.top);
}
| mos_switchtype                         nzlist_of_mos_switch_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2.top);
}
| mos_switchtype                  delay3 nzlist_of_mos_switch_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| n_input_gatetype                       nzlist_of_n_input_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2.top);
}
| n_input_gatetype                delay2 nzlist_of_n_input_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| n_input_gatetype drive_strength        nzlist_of_n_input_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| n_input_gatetype drive_strength delay2 nzlist_of_n_input_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3, $4.top);
}
| n_output_gatetype                       nzlist_of_n_output_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2.top);
}
| n_output_gatetype                delay2 nzlist_of_n_output_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| n_output_gatetype drive_strength        nzlist_of_n_output_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| n_output_gatetype drive_strength delay2 nzlist_of_n_output_gate_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3, $4.top);
}
| pass_en_switchtype                      nzlist_of_pass_en_switch_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2.top);
}
| pass_en_switchtype               delay3 nzlist_of_pass_en_switch_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2, $3.top);
}
| pass_switchtype                         nzlist_of_pass_switch_inst ';'
{
  $$ = parser.factory().new_GateH(@$, $1, $2.top);
}
| PULLDOWN                                nzlist_of_pull_inst ';'
{
  $$ = parser.factory().new_GateH(@$, VpiPrimType::Pulldown, $2.top);
}
| PULLDOWN pulldown_strength              nzlist_of_pull_inst ';'
{
  $$ = parser.factory().new_GateH(@$, VpiPrimType::Pulldown, $2, $3.top);
}
| PULLUP                                  nzlist_of_pull_inst ';'
{
  $$ = parser.factory().new_GateH(@$, VpiPrimType::Pullup, $2.top);
}
| PULLUP pullup_strength                  nzlist_of_pull_inst ';'
{
  $$ = parser.factory().new_GateH(@$, VpiPrimType::Pullup, $2, $3.top);
}
;

// [SPEC*] nzlist_of_cmos_switch_inst ::=
//                  cmos_siwtch_instance {',' cmos_switch_instance}
nzlist_of_cmos_switch_inst
: cmos_switch_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_cmos_switch_inst ',' cmos_switch_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] nzlist_of_enable_gate_inst ::=
//                  enable_gate_instance {',' enble_gate_instance }
nzlist_of_enable_gate_inst
: enable_gate_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_enable_gate_inst ',' enable_gate_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] nzlist_of_mos_switch_inst ::=
//                   mos_switch_instance {',' mos_switch_instance }
nzlist_of_mos_switch_inst
: mos_switch_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_mos_switch_inst ',' mos_switch_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] nzlist_of_n_input_gate_inst ::=
//                  n_input_gate_instance {',' n_input_gate_instance }
nzlist_of_n_input_gate_inst
: n_input_gate_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_n_input_gate_inst ',' n_input_gate_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] nzlist_of_n_output_gate_inst ::=
//                   n_output_gate_instance {',' n_output_gate_instance }
nzlist_of_n_output_gate_inst
: n_output_gate_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_n_output_gate_inst ',' n_output_gate_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] nzlist_of_pass_en_switch_inst ::=
//                  pass_enable_switch_instance
//                  {',' pass_enable_switch_instance }
nzlist_of_pass_en_switch_inst
: pass_enable_switch_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_pass_en_switch_inst ',' pass_enable_switch_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] nzlist_of_pass_switch_inst ::=
//                   pass_switch_instance {',' pass_switch_instance }
nzlist_of_pass_switch_inst
: pass_switch_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_pass_switch_inst ',' pass_switch_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] nzlist_of_pull_inst ::=
//                  pull_gate_instance {',' pull_gate_instance }
nzlist_of_pull_inst
: pull_gate_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_pull_inst ',' pull_gate_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] cmos_switch_instance ::= [name_of_gate_instance]
//                  '(' output_terminal ',' input_terminal ','
//                      ncontrol_terminal ',' pcontrol_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] input_terminal ::= expression
// [SPEC] ncontrol_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
// [SPEC] pcontrol_terminal ::= expression
cmos_switch_instance
: '(' net_lvalue ',' expression ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $2, $4, $6, $8);
}
| IDENTIFIER
  '(' net_lvalue ',' expression ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3, $5, $7, $9);
}
| IDENTIFIER range
  '(' net_lvalue ',' expression ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4, $6, $8, $10);
}
;

// [SPEC] enable_gate_instance ::= [name_of_gate_instance]
//                  '(' output_terminal ',' input_terminal ','
//                      enable_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] enable_terminal ::= expression
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
enable_gate_instance
:                  '(' net_lvalue ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $2, $4, $6);
}
| IDENTIFIER       '(' net_lvalue ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3, $5, $7);
}
| IDENTIFIER range
  '(' net_lvalue ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4, $6, $8);
}
;

// [SPEC] mos_switch_instance ::= [name_of_gate_instance]
//                 '(' output_terminal ',' input_terminal ','
//                     enable_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] enable_terminal ::= expression
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
mos_switch_instance
:                  '(' net_lvalue ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $2, $4, $6);
}
| IDENTIFIER       '(' net_lvalue ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3, $5, $7);
}
| IDENTIFIER range
  '(' net_lvalue ',' expression ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4, $6, $8);
}
;

// [SPEC] n_input_gate_instance ::= [name_of_gate_instance]
//                 '(' output_terminal ',' input_terminal
//                     {',' input_terminal} ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
n_input_gate_instance
:                  '(' n_input_gate_terminals ')'
{
  $$ = parser.factory().new_Inst(@$, $2.top);
}
| IDENTIFIER       '(' n_input_gate_terminals ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3.top);
}
| IDENTIFIER range '(' n_input_gate_terminals ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4.top);
}
;

// [SPEC] n_input_gate_terminals ::=
//             output_terminal ',' input_terminal {',' input_terminal}
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
n_input_gate_terminals
: net_lvalue ',' expression
{
  auto con1 = parser.factory().new_OrderedCon($1);
  auto con2 = parser.factory().new_OrderedCon($3);
  $$ = PtConnectionList();
  $$.init(con1);
  $$.add(con2);
}
| n_input_gate_terminals ',' expression
{
  auto con1 = parser.factory().new_OrderedCon($3);
  $$ = $1;
  $$.add(con1);
}
;

// [SPEC] n_output_gate_instance ::=
//             [name_of_gate_instance]
//                 '(' output_terminal {',' output_terminal}
//                   ',' input_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
n_output_gate_instance
:                  '(' n_output_gate_terminals ')'
{
  $$ = parser.factory().new_Inst(@$, $2.top);
}
| IDENTIFIER       '(' n_output_gate_terminals ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3.top);
}
| IDENTIFIER range '(' n_output_gate_terminals ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4.top);
}
;

// [SPEC] n_output_gate_terminals ::=
//             output_terminal {',' output_terminal} ', ' input_terminal
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
// ちょっと手抜きをしてすべて expression のリストと見なす．
n_output_gate_terminals
: expression ',' expression
{
  auto con1 = parser.factory().new_OrderedCon($1);
  auto con2 = parser.factory().new_OrderedCon($3);
  $$ = PtConnectionList();
  $$.init(con1);
  $$.add(con2);
}
| n_output_gate_terminals ',' expression
{
  auto con1 = parser.factory().new_OrderedCon($3);
  $$ = $1;
  $$.add(con1);
}
;

// [SPEC] pass_switch_instance ::=
//            [name_of_gate_instance] '(' inout_terminal ',' inout_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] inout_terminal ::= net_lvalue
pass_switch_instance
:                  '(' net_lvalue ',' net_lvalue ')'
{
  $$ = parser.factory().new_Inst(@$, $2, $4);
}
| IDENTIFIER       '(' net_lvalue ',' net_lvalue ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3, $5);
}
| IDENTIFIER range '(' net_lvalue ',' net_lvalue ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4, $6);
}
;

// [SPEC] pass_enable_switch_instance ::=
//             [name_of_gate_instance] '(' inout_terminal ',' inout_terminal
//                 ',' enable_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] enable_terminal ::= expression
// [SPEC] inout_terminal ::= net_lvalue
pass_enable_switch_instance
:                  '(' net_lvalue ',' net_lvalue ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $2, $4, $6);
}
| IDENTIFIER       '(' net_lvalue ',' net_lvalue ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3, $5, $7);
}
| IDENTIFIER range '(' net_lvalue ',' net_lvalue ',' expression ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4, $6, $8);
}
;

// [SPEC] pull_gate_instance ::=
//             [name_of_gate_instance] '(' output_terminal ')'
// [SPEC] name_of_gate_instance ::= gate_instance_identifier [ range ]
// [SPEC] output_terminal ::= net_lvalue
pull_gate_instance
:                  '(' net_lvalue ')'
{
  $$ = parser.factory().new_Inst(@$, $2);
}
| IDENTIFIER       '(' net_lvalue ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3);
}
| IDENTIFIER range '(' net_lvalue ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4);
}
;


//////////////////////////////////////////////////////////////////////
// A.3.2 Primitive strengths
//////////////////////////////////////////////////////////////////////

// [SPEC] pulldown_strength ::=
//             '(' strength0 ',' strength1 ')'
//            |'(' strength1 ',' strength0 ')'
//            |'(' strength0 ')'
pulldown_strength
: '(' strength0 ',' strength1 ')'
{
  $$ = parser.factory().new_Strength(@$, $2, $4);
}
| '(' strength1 ',' strength0 ')'
{
  $$ = parser.factory().new_Strength(@$, $2, $4);
}
| '(' strength0 ')'
{
  $$ = parser.factory().new_Strength(@$, $2);
}
;

// [SPEC] pullup_strength ::=
//             '(' strength0 ',' strength1 ')'
//            |'(' strength1 ',' strength0 ')'
//            |'(' strength1 ')'
pullup_strength
: '(' strength0 ',' strength1 ')'
{
  $$ = parser.factory().new_Strength(@$, $2, $4);
}
| '(' strength1 ',' strength0 ')'
{
  $$ = parser.factory().new_Strength(@$, $2, $4);
}
| '(' strength1 ')'
{
  $$ = parser.factory().new_Strength(@$, $2);
}
;


//////////////////////////////////////////////////////////////////////
// A.3.3 Primitive terminals
//////////////////////////////////////////////////////////////////////

// [SPEC] enable_terminal ::= expression

// [SPEC] inout_terminal ::= net_lvalue

// [SPEC] input_terminal ::= expression

// [SPEC] ncontrol_terminal ::= expression

// [SPEC] output_terminal ::= net_lvalue

// [SPEC] pcontrol_terminal ::= expression



//////////////////////////////////////////////////////////////////////
// A.3.4 Primitive gate and swith types
//////////////////////////////////////////////////////////////////////

// [SPEC] cmos_switchtype ::= "cmos"|"rcmos"
cmos_switchtype
: CMOS
{
  $$ = VpiPrimType::Cmos;
}
| RCMOS
{
  $$ = VpiPrimType::Rcmos;
}
;

// [SPEC] enable_gatetype ::= "bufif0"|"bufif1"|"notif0"|"notif1"
enable_gatetype
: BUFIF0
{
  $$ = VpiPrimType::Bufif0;
}
| BUFIF1
{
  $$ = VpiPrimType::Bufif1;
}
| NOTIF0
{
  $$ = VpiPrimType::Notif0;
}
| NOTIF1
{
  $$ = VpiPrimType::Notif1;
}
;

// [SPEC] mos_switchtype ::= "nmos"|"pmos"|"rnmos"|"rpmos"
mos_switchtype
: NMOS
{
  $$ = VpiPrimType::Nmos;
}
| PMOS
{
  $$ = VpiPrimType::Pmos;
}
| RNMOS
{
  $$ = VpiPrimType::Rnmos;
}
| RPMOS
{
  $$ = VpiPrimType::Rpmos;
}
;

// [SPEC] n_input_gatetype ::= "and"|"nand"|"or"|"nor"|"xor"|"xnor"
n_input_gatetype
: AND
{
  $$ = VpiPrimType::And;
}
| NAND
{
  $$ = VpiPrimType::Nand;
}
| OR
{
  $$ = VpiPrimType::Or;
}
| NOR
{
  $$ = VpiPrimType::Nor;
}
| XOR
{
  $$ = VpiPrimType::Xor;
}
| XNOR
{
  $$ = VpiPrimType::Xnor;
}
;

// [SPEC] n_output_gatetype ::= "buf"|"not"
n_output_gatetype
: BUF
{
  $$ = VpiPrimType::Buf;
}
| NOT
{
  $$ = VpiPrimType::Not;
}
;

// [SPEC] pass_en_switchtype ::= "tranif0"|"tranif1"|"rtranif0"|"rtranif1"
pass_en_switchtype
: TRANIF0
{
  $$ = VpiPrimType::Tranif0;
}
| TRANIF1
{
  $$ = VpiPrimType::Tranif1;
}
| RTRANIF0
{
  $$ = VpiPrimType::Rtranif0;
}
| RTRANIF1
{
  $$ = VpiPrimType::Rtranif1;
}
;

// [SPEC] pass_switchtype ::= "tran"|"rtran"
pass_switchtype
: TRAN
{
  $$ = VpiPrimType::Tran;
}
| RTRAN
{
  $$ = VpiPrimType::Rtran;
}
;



//////////////////////////////////////////////////////////////////////
// A.4 Module and generated instantiation
//////////////////////////////////////////////////////////////////////
// A.4.1 Module instantiation
//////////////////////////////////////////////////////////////////////

// [SPEC] module_instantiation ::=
//             module_identifier [parameter_value_assignment]
//                 module_instance {',' module_instance } ';'
// [SPEC] udp_instantiation ::= udp_identifier [drive_strength] [delay2]
//             udp_instance {',' udp_instance} ';'
// [SPEC] module_identifier ::= identifier
// [SPEC] udp_identifier ::= identifier
// 最大の頭痛の種
// module_identifier と udp_identifier は区別不能
// なので両方の最大公約数的な仕様にしておく．
// [SPEC*] mu_instantiation ::=
//             identifier [parameter_value_assignment]
//                 mu_instance {',' mu_instance } ';'
//            |identifier [drive_strength] [delay2]
//                 mu_instance {',' mu_instance } ';'
// [SPEC] parameter_value_assignment ::=
//             '#' '(' list_of_parameter_assignments ')'
// [SPEC] list_of_parameter_assignments ::=
//             ordered_parameter_assignment {',' ordered_parameter_assignment }
//            |named_parameter_assignment {',' named_parameter_assignment }
// [SPEC] ordered_parameter_assignment ::= expression
// 最悪なことに delay2 とは '#' '(' expression ',' ,,,
// なので parameter_value_assignment と重なる．
// とりあえず delay2 と ordered_parameter_assignment は区別できない．
module_instantiation
: IDENTIFIER                                          nzlist_of_mu_inst ';'
{
  $$ = parser.factory().new_MuH(@$, $1, $2.top);
  parser.reg_defname($1);
}
| IDENTIFIER '#' '(' list_of_ordered_param_assign ')' nzlist_of_mu_inst ';'
{
  $$ = parser.factory().new_MuH(@$, $1, $4.top, $6.top);
  parser.reg_defname($1);
}
| IDENTIFIER '#' '(' list_of_named_param_assign ')'   nzlist_of_mu_inst ';'
{
  $$ = parser.factory().new_MuH(@$, $1, $4.top, $6.top);
  parser.reg_defname($1);
}
| IDENTIFIER '#' unumber                              nzlist_of_mu_inst ';'
             // これは delay2 の特殊形
{
  auto delay = parser.factory().new_Delay(FileRegion(@2, @3), $3);
  $$ = parser.factory().new_MuH(@$, $1, delay, $4.top);
  parser.reg_defname($1);
}
| IDENTIFIER '#' rnumber                              nzlist_of_mu_inst ';'
             // これは delay2 の特殊形
{
  auto delay = parser.factory().new_Delay(FileRegion(@2, @3), $3);
  $$ = parser.factory().new_MuH(@$, $1, delay, $4.top);
  parser.reg_defname($1);
}
| IDENTIFIER '#' IDENTIFIER                           nzlist_of_mu_inst ';'
             // これは delay2 の特殊形
{
  auto prim = parser.factory().new_Primary(@3, $3);
  auto delay = parser.factory().new_Delay(FileRegion(@2, @3), prim);
  $$ = parser.factory().new_MuH(@$, $1, delay, $4.top);
  parser.reg_defname($1);
}
| IDENTIFIER drive_strength                           nzlist_of_mu_inst ';'
{
  $$ = parser.factory().new_MuH(@$, $1, $2, $3.top);
  parser.reg_defname($1);
}
| IDENTIFIER drive_strength delay2                    nzlist_of_mu_inst ';'
{
  $$ = parser.factory().new_MuH(@$, $1, $2, $3, $4.top);
  parser.reg_defname($1);
}
| IDENTIFIER error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC*] list_of_ordered_param_assign ::=
//             expression { ',' expression }
list_of_ordered_param_assign
: expression
{
  auto con1 = parser.factory().new_OrderedCon($1);
  $$ = PtConnectionList();
  $$.add(con1);
}
| list_of_ordered_param_assign ',' expression
{
  auto con1 = parser.factory().new_OrderedCon($3);
  $$ = $1;
  $$.add(con1);
}
;

// [SPEC*] list_of_named_param_assign ::=
//             named_parameter_assignment { ',' named_parameter_assignment }
list_of_named_param_assign
: named_parameter_assignment
{
  $$ = PtConnectionList();
  $$.add($1);
}
| list_of_named_param_assign ',' named_parameter_assignment
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] named_parameter_assignment ::=
//             '.'parameter_identifier'('[expression]')'
// [SPEC] parameter_identifier ::= identifier
named_parameter_assignment
: '.' IDENTIFIER '(' ')'
{
  $$ = parser.factory().new_NamedCon(@$, $2);
}
| '.' IDENTIFIER '(' expression ')'
{
  $$ = parser.factory().new_NamedCon(@$, $2, $4);
}
;

// [SPEC*] nzlist_of_module_inst ::=
//             module_instance { ',' module_instance }
nzlist_of_mu_inst
: mu_instance
{
  $$ = PtInstList();
  $$.init($1);
}
| nzlist_of_mu_inst ',' mu_instance
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] module_instance ::=
//             name_of_instance '(' [list_of_port_connections] ')'
// [SPEC] name_of_instance ::= module_instance_identifier [ range ]
// [SPEC] list_of_port_connections ::=
//             orderd_port_connection {',' ordered_port_connection }
//            |named_port_connection {',' named_port_connection }
// [SPEC] udp_instance ::= [name_of_udp_instance] '(' output_terminal ','
//             input_terminal {',' input_terminal} ')'
// [SPEC] name_of_udp_instance ::= udp_instance_identifier [range]
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue
//
// 嘘つき！ udp_instance_identifier は arrayed_identifier なので中に
// [range] を含んでいる．この仕様書をみてプログラム作ってない証拠
//
// 微妙に異なるので以下のようにする．
// [SPEC*] mu_instance ::=
//             [name_of_instance] '(' [list_of_port_connections] ')'
// [SPEC*] name_of_instance ::= identifier [range]
// ただし udp_instance の場合には named_port_connection はない．
// これはここでは判定できないのであとで調べる．
// 空のリスト '(' ')' の場合も 1 つの ai_list (ordered_port_connection)
// を要素として持つリストと認識されるのであとで特別に扱う必要がある．
mu_instance
:                  '(' list_of_ordered_port_connections ')'
{
  $$ = parser.factory().new_Inst(@$, $2.top);
}
|                  '(' list_of_named_port_connections ')'
{
  $$ = parser.factory().new_Inst(@$, $2.top);
}
| IDENTIFIER       '(' list_of_ordered_port_connections ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3.top);
}
| IDENTIFIER       '(' list_of_named_port_connections ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $3.top);
}
| IDENTIFIER range '(' list_of_ordered_port_connections ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4.top);
}
| IDENTIFIER range '(' list_of_named_port_connections ')'
{
  $$ = parser.factory().new_Inst(@$, $1, $2, $4.top);
}
;

// [SPEC*] list_of_ordered_port_connection ::=
//            [ ordered_port_connection { ',' ordered_port_connection }]
// ordered_port_connection は空も含める．
list_of_ordered_port_connections
: ordered_port_connection
{
  $$ = PtConnectionList();
  $$.init($1);
}
| list_of_ordered_port_connections ',' ordered_port_connection
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] ordered_port_connection ::= { attribute_instance } [expression]
ordered_port_connection
: // 空
{
  // でも nullptr は返さない．
  $$ = parser.factory().new_OrderedCon();
}
| expression
{
  $$ = parser.factory().new_OrderedCon($1);
}
| nz_ai_list
{
  $$ = parser.factory().new_OrderedCon();
  parser.reg_attrinst($$, $1.top);
}
| nz_ai_list expression
{
  $$ = parser.factory().new_OrderedCon($2);
  parser.reg_attrinst($$, $1.top);
}
;

// [SPEC*] list_of_named_port_connections ::=
//            named_port_connection { ',' named_port_connection }
list_of_named_port_connections
: named_port_connection
{
  $$ = PtConnectionList();
  $$.init($1);
}
| list_of_named_port_connections ',' named_port_connection
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] named_port_connection ::=
//            { attribute_instance } '.' port_identifier '(' [ expression ] ')'
// [SPEC] port_identifier ::= identifier
named_port_connection
: '.' IDENTIFIER '(' ')'
{
  $$ = parser.factory().new_NamedCon(@$, $2);
}
| '.' IDENTIFIER '(' expression ')'
{
  $$ = parser.factory().new_NamedCon(@$, $2, $4);
}
| nz_ai_list '.' IDENTIFIER '(' ')'
{
  $$ = parser.factory().new_NamedCon(@$, $3);
  parser.reg_attrinst($$, $1.top);
}
| nz_ai_list '.' IDENTIFIER '(' expression ')'
{
  $$ = parser.factory().new_NamedCon(@$, $3, $5);
  parser.reg_attrinst($$, $1.top);
}
;


//////////////////////////////////////////////////////////////////////
// A.4.2 Generated instantiation
//////////////////////////////////////////////////////////////////////

// [SPEC] generated_instantiation ::=
//             "generate" { generate_item } "endgenerate"
generated_instantiation
: GENERATE list_of_generate_items ENDGENERATE
{
  $$ = parser.factory().new_Generate(@$,
				     $2.declhead_list.top,
				     $2.item_list.top);
}
| GENERATE error ENDGENERATE
{
  $$ = nullptr;
  // generate 中のエラーは endgenerate まで読み飛ばす
  yyerrok;
}
;

// [SPEC*] list_of_generate_items ::= { generate_item }
list_of_generate_items
: // 空
{
  $$ = PtHeadList();
}
| list_of_generate_items generate_item
{
  $$ = $1;
  $1.item_list.add($2);
}
| list_of_generate_items module_or_generate_decl
{
  $$ = $1;
  $1.declhead_list.add($2);
}
;

// [SPEC] generate_item_or_null ::= [ generate_item ]
generate_item_or_null
: ';'
{
  $$ = PtHeadList();
}
| generate_item
{
  $$ = PtHeadList();
  $$.item_list.add($1);
}
| module_or_generate_decl
{
  $$ = PtHeadList();
  $$.declhead_list.add($1);
}
;

// [SPEC] generate_item ::=
//             generate_conditional_statement
//            |generate_case_statement
//            |generate_loop_statement
//            |generate_block
//            |module_or_generate_item
// [SPEC] generate_conditional_statement ::=
//             "if" '(' constant_expression ')' generate_item_or_null
//                 [ "else" generate_item_or_null ]
// [SPEC] generate_case_statement ::=
//             "case" '(' constant_expression ')'
//                 genvar_case_item { genvar_case_item } "endcase"
// [SPEC] generate_loop_statement ::=
//             "for" '(' genvar_assignment ';' constant_expression ';'
//                 genvar_assignment ')'
//                 "begin" ';' generate_block_identifier { generate_item }
//                 "end"
// [SPEC] generate_block ::=
//             "begin" [ ':' generate_block_identifier ] { generate_item }
//             "end"
// [SPEC] genvar_assignment ::= genvar_identifier '=' constant_expression
// [SPEC] genvar_identifier ::= identifier
// generate_item_or_null の場合実際には module_or_generate_decl の場合が
// あることを忘れてはならない．
generate_item
: IF '(' expression ')' generate_item_or_null %prec LOWER_THAN_ELSE
{
  $$ = parser.factory().new_GenIf(@$, $3,
				  $5.declhead_list.top,
				  $5.item_list.top);
}
| IF '(' expression ')' generate_item_or_null
  ELSE generate_item_or_null
{
  $$ = parser.factory().new_GenIfElse(@$, $3,
				      $5.declhead_list.top,
				      $5.item_list.top,
				      $7.declhead_list.top,
				      $7.item_list.top);
}
| IF error ELSE { yyerrok; } generate_item_or_null
{
  $$ = nullptr;
}
| CASE '(' expression ')' list_of_gencaseitem ENDCASE
{
  $$ = parser.factory().new_GenCase(@$, $3, $5.top);
}
| CASE error ENDCASE
{
  $$ = nullptr;
  yyerrok;
}
| FOR '(' IDENTIFIER '=' expression ';' expression ';'
          IDENTIFIER '=' expression ')'
  BEGIN ':' IDENTIFIER
  list_of_generate_items
  END
{
  if ( parser.check_GenFor(@$, $3, $9) ) {
    $$ = parser.factory().new_GenFor(@$, $3, $5, $7, $11, $15,
				     $16.declhead_list.top,
				     $16.item_list.top);
  }
  else {
    $$ = nullptr;
  }
}
| FOR '(' IDENTIFIER '=' expression ';' expression ';'
          IDENTIFIER '=' expression ')'
  BEGIN error END
{
  $$ = nullptr;
  yyerrok;
}
| FOR error END
{
  $$ = nullptr;
  yyerrok;
}
| BEGIN list_of_generate_items END
{
  $$ = parser.factory().new_GenBlock(@$,
				     $2.declhead_list.top,
				     $2.item_list.top);
}
| BEGIN ':' IDENTIFIER list_of_generate_items END
{
  $$ = parser.factory().new_GenBlock(@$, $3,
				     $4.declhead_list.top,
				     $4.item_list.top);
}
| BEGIN error END
{
  $$ = nullptr;
  yyerrok;
}
| ai_list module_or_generate_item
{
  $$ = $2;
  parser.reg_attrinst($2, $1);
}
;

// [SPEC*] list_of_gencaseitem ::=
//             genvar_caseitem { genvar_caseitem }
list_of_gencaseitem
: genvar_case_item
{
  $$ = PtGenCaseItemList();
  $$.init($1);
}
| list_of_gencaseitem genvar_case_item
{
  $$ = $1;
  $$.add($2);
}
;

// [SPEC] genvar_case_item ::=
//             constant_expression {',' constant_expression } ':'
//                 generate_item_or_null
//            |"default" [':'] generate_item_or_null
genvar_case_item
: genvar_case_head generate_item_or_null
{
  $$ = parser.factory().new_GenCaseItem(@$, $1.top,
					$2.declhead_list.top,
					$2.item_list.top);
}
;

genvar_case_head
: nzlist_of_expressions ':'
{
  $$ = $1;
}
| DEFAULT
{
  // 空リスト
  $$ = PtExprList();
  $$.init();
}
| DEFAULT               ':'
{
  // 空リスト
  $$ = PtExprList();
  $$.init();
}
;


//////////////////////////////////////////////////////////////////////
// A.5.1 UDP declaration
//////////////////////////////////////////////////////////////////////

// [SPEC] udp_declaration ::=
//             { attribute_instance } "primitive" udp_identifier
//                 '(' udp_port_list ')' ';'
//                 udp_port_declaration { udp_port_declaration }
//                 udp_body
//             "endprimitive"
//            |{ attribute_instance } "primitive" udp_identifier
//                 '(' udp_declaration_port_list ')' ';'
//                 udp_body
//             "endprimitive"
// 実はもとの仕様とかなり違う文法で読み込んでいる．
udp_declaration
: ai_list PRIMITIVE IDENTIFIER '(' udp_port_list ')' ';'
  nzlist_of_uport_decl
  TABLE
  list_of_udp_entries
  ENDTABLE
  ENDPRIMITIVE
{
  auto udp = parser.new_Udp1995(@$, $3,
				nullptr, FileRegion(), nullptr,
				$5.top,
				$8.iohead_list.top,
				$8.declhead_list.top,
				$10.top);
  parser.reg_udp(udp);
  parser.reg_attrinst(udp, $1);
}
| ai_list PRIMITIVE IDENTIFIER '(' udp_port_list ')' ';'
  nzlist_of_uport_decl
  INITIAL IDENTIFIER '=' init_val ';'
  TABLE
  list_of_udp_entries
  ENDTABLE
  ENDPRIMITIVE
{
  auto udp = parser.new_Udp1995(@$, $3,
				$10, @10, $12,
				$5.top,
				$8.iohead_list.top,
				$8.declhead_list.top,
				$15.top);
  parser.reg_udp(udp);
  parser.reg_attrinst(udp, $1);
}
| ai_list PRIMITIVE IDENTIFIER '(' udp_declaration_port_list ')' ';'
  TABLE
  list_of_udp_entries
  ENDTABLE
  ENDPRIMITIVE
{
  auto udp = parser.new_Udp2001(@$, $3,
				nullptr, FileRegion(), nullptr,
				$5.head_list.top,
				$9.top);
  parser.reg_udp(udp);
  parser.reg_attrinst(udp, $1);
}
| ai_list PRIMITIVE IDENTIFIER '(' udp_declaration_port_list ')' ';'
  INITIAL IDENTIFIER '=' init_val ';'
  TABLE
  list_of_udp_entries
  ENDTABLE
  ENDPRIMITIVE
{
  auto udp = parser.new_Udp2001(@$, $3,
				$9, @9, $11,
				$5.head_list.top,
				$14.top);
  parser.reg_udp(udp);
  parser.reg_attrinst(udp, $1);
}
| ai_list PRIMITIVE error ENDPRIMITIVE
{
  yyerrok;
}
;


//////////////////////////////////////////////////////////////////////
// A.5.2 UDP ports
//////////////////////////////////////////////////////////////////////

// [SPEC] udp_port_list ::= output_port_identifier ','
//             input_port_identifier {',' input_port_identifier }
// [SPEC] output_port_identifier ::= identifier
// [SPEC] input_port_identifier ::= identifier
// 相変わらずアホな構文規則
// 要は udp_port_list ::= identifier ',' identifier {',' identifier}
// でしょ．
udp_port_list
: udp_port      ',' udp_port // 最低2つはある．
{
  $$ = PtPortList();
  $$.init($1);
  $$.add($3);
}
| udp_port_list ',' udp_port
{
  $$ = $1;
  $$.add($3);
}
;

udp_port
: IDENTIFIER
{
  $$ = parser.factory().new_Port(@1, $1);
}
;

// [SPEC*] nzlist_of_uport_decl ::=
//            udp_port_declaration { udp_port_declaration }
//
// [SPEC] udp_port_declaration ::=
//             udp_output_declaration ';'
//            |udp_input_declaration ';'
//            |udp_reg_declaration ';'
//
// めんどくさいのは PtIOHead と PtDeclHead が混在していること
// しょうがないので PtHeadList を使う．
nzlist_of_uport_decl
: ai_list udp_output_declaration
{
  $$ = PtHeadList();
  $$.init();
  $$.iohead_list.add($2);
  parser.reg_attrinst($2, $1);
}
| ai_list udp_input_declaration
{
  $$ = PtHeadList();
  $$.init();
  $$.iohead_list.add($2);
  parser.reg_attrinst($2, $1);
}
| ai_list udp_reg_declaration
{
  $$ = PtHeadList();
  $$.init();
  $$.declhead_list.add($2);
  parser.reg_attrinst($2, $1);
}
| nzlist_of_uport_decl ai_list udp_output_declaration
{
  $$ = $1;
  $$.iohead_list.add($3);
  parser.reg_attrinst($3, $2);
}
| nzlist_of_uport_decl ai_list udp_input_declaration
{
  $$ = $1;
  $$.iohead_list.add($3);
  parser.reg_attrinst($3, $2);
}
| nzlist_of_uport_decl ai_list udp_reg_declaration
{
  $$ = $1;
  $$.declhead_list.add($3);
  parser.reg_attrinst($3, $2);
}
;

// [SPEC] udp_output_declaration ::=
//             { attribute_instance } "output"       port_identifier
//            |{ attribute_instance } "output" "reg" port_identifier
//                 [ '=' constant_expression]
// 最後の port_identifier [ '=' constant_expression ] は
// variable_port_identifier として処理する．
udp_output_declaration
: OUTPUT port_identifier_item ';'
{
  // UDP は1ビット符号なしのみ
  $$ = parser.factory().new_IOHead(@$, VpiDir::Output, $2);
}
| OUTPUT REG variable_port_identifier_item ';'
{
  // UDP は1ビット符号なしのみ
  $$ = parser.factory().new_RegIOHead(@$, VpiDir::Output, $3);
}
| OUTPUT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] udp_input_declaration ::=
//             { attribute_instance } "input" list_of_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
udp_input_declaration
: INPUT list_of_port_identifiers ';'
{
  // UDP は1ビット符号無しのみ
  $$ = parser.factory().new_IOHead(@$, VpiDir::Input, $2.top);
}
| INPUT error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] udp_reg_declaration ::=
//             { attribute_instance } "reg" variable_identifier
udp_reg_declaration
: REG IDENTIFIER ';'
{
  auto item = parser.factory().new_DeclItem(@2, $2);
  $$ = parser.factory().new_RegH(@$, item);
}
| REG error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] udp_declaration_port_list ::= udp_output_declaration ','
//             udp_input_declaration {',' udp_input_declaration}
// [SPEC] udp_input_declaration ::=
//             { attribute_instance } "input" list_of_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
// もういい加減この2重リストの問題はうんざり．
udp_declaration_port_list
: udp_output_port_declaration ',' ai_list INPUT port_identifier_item
{
  $$ = PtIODList();
  $$.init($1);
  auto head = parser.factory().new_IOHead(@4, VpiDir::Input, $5);
  parser.reg_attrinst(head, $3);
  $$.add_head(head);
}
| udp_declaration_port_list ',' ai_list INPUT port_identifier_item
{
  $$ = $1;
  auto head = parser.factory().new_IOHead(@4, VpiDir::Input, $5);
  parser.reg_attrinst(head, $3);
  $$.add_head(head);
}
| udp_declaration_port_list ',' port_identifier_item
{
  $$ = $1;
  $$.add_item($3);
}
;

// udp_output_declaration の ';' なしバージョン
udp_output_port_declaration
: ai_list OUTPUT port_identifier_item
{
  $$ = parser.factory().new_IOHead(@2, VpiDir::Output, $3);
  parser.reg_attrinst($$, $1);
}
| ai_list OUTPUT REG variable_port_identifier_item
{
  $$ = parser.factory().new_RegIOHead(FileRegion(@2, @3),
				      VpiDir::Output, $4);
  parser.reg_attrinst($$, $1);
}
;


//////////////////////////////////////////////////////////////////////
// A.5.3 UDP body
//////////////////////////////////////////////////////////////////////

// [SPEC] udp_body ::=
//             combinational_body
//            |sequential_body
// [SPEC] combinational_body ::=
//             "table"
//               combinational_entry { combinational_entry }
//             "endtable"
// [SPEC] sequential_body ::=
//             [ udp_initial_statement ]
//             "table"
//               sequential_entry { sequential_entry }
//             "endtable"
// [SPEC] udp_initial_statement ::=
//             "initial" output_port_identifier '=' init_val ';'
// [SPEC] output_port_identifier ::= identifier
// [SPEC] init_val ::= 1'b0|1'b1|1'bx|1'bX|1'B0|1'B1|1'Bx|1'BX|1|0
// 本当は combinational_entry と sequential_entry が混在しているのは
// なしだが，構文的にチェックするのは面倒くさいので，パーズしておいてから
// あとでチェックする．
init_val
: UNUM_INT
{ // 実際には {1|0} のみ
  if ( $1 != 0 && $1 != 1 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    @1,
		    MsgType::Error,
		    "PARSER",
		    "expectiong 1 or 0.");
    YYERROR;
  }
  else {
    $$ = parser.factory().new_IntConst(@1, $1);
  }
}
| UNUM_INT BASE_B UNUMBER
{ // 実際には 1'b{0|1|x} のみ
  if ( $1 != 1 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    @1,
		    MsgType::Error,
		    "PARSER",
		    "UDP's initial constant size should be 1.");
    YYERROR;
  }
  else if ( strcmp($3, "z") == 0 ||
	    strcmp($3, "Z") == 0 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    @3,
		    MsgType::Error,
		    "PARSER",
		    "Z value is not allowd here.");
    YYERROR;
  }
  else {
    $$ = parser.factory().new_IntConst(@$, 1, VpiConstType::Binary, $3);
  }
}
| UNUM_INT wrong_base UNUMBER
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  @1,
		  MsgType::Error,
		  "PARSER",
		  "illegal base: only 'b is allowed here.");
  YYERROR;
}
| UNUM_BIG BASE_B UNUMBER
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  @1,
		  MsgType::Error,
		  "PARS",
		  "UDP's initial constant size should be 1.");
  YYERROR;
}
| numbase UNUMBER
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  @1,
		  MsgType::Error,
		  "PARSER",
		  "UDP's initial constant size should be 1.");
  YYERROR;
}
;

wrong_base
: BASE_SB
| BASE_O
| BASE_SO
| BASE_D
| BASE_SD
| BASE_H
| BASE_SH
;

// [SPEC*] list_of_udp_entries ::=
//             udp_entry { udp_entry }
list_of_udp_entries
: udp_entry
{
  $$ = PtUdpEntryList();
  $$.init($1);
}
| list_of_udp_entries udp_entry
{
  $$ = $1;
  $$.add($2);
}
;

// [SPEC*] udp_entry ::= combinational_entry | sequential_entry
// [SPEC] combinational_entry ::= level_input_list ':' output_symbol ';'
// [SPEC] sequential_entry ::=
//             seq_input_list ':' current_state ':' next_state ';'
// [SPEC] seq_input_list ::= level_input_list | edge_input_list
// [SPEC] current_state ::= level_symbol
udp_entry
: seq_input_list ':' output_symbol ';'
{
  auto output = parser.factory().new_UdpValue(@3, $3);
  $$ = parser.factory().new_UdpEntry(@$, $1.top, output);
}
| seq_input_list ':' level_symbol ':' next_state ';'
{
  auto current = parser.factory().new_UdpValue(@3, $3);
  auto output = parser.factory().new_UdpValue(@5, $5);
  $$ = parser.factory().new_UdpEntry(@$, $1.top, current, output);
}
| error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] seq_input_list ::= level_input_list | edge_input_list
// [SPEC] level_input_list ::= level_symbol { level_symbol }
// [SPEC] edge_input_list ::= { level_symbol } edge_indicator { level_symbol }
// これをこのまま書くとコンフリクトが起きる．
// 良くみてみると, seq_input_list とは
// level_symbol と edge_indicator の空でないリストのうち，
// edge_indicator を2つ以上含まないものだということがわかる．
// edge_indicator をいくつ含んでいるかのチェックはあとで行う．
seq_input_list
: level_symbol_value
{
  $$ = PtUdpValueList();
  $$.init($1);
}
| edge_indicator
{
  $$ = PtUdpValueList();
  $$.init($1);
}
| seq_input_list level_symbol_value
{
  $$ = $1;
  $$.add($2);
}
| seq_input_list edge_indicator
{
  $$ = $1;
  $$.add($2);
}
;

// [SPEC] edge_indicator ::= '(' level_symbol level_symbol ')' | edge_symbol
edge_indicator
: '(' level_symbol level_symbol ')'
{
  $$ = parser.factory().new_UdpValue(@$, $2, $3);
}
| edge_symbol
{
  $$ = parser.factory().new_UdpValue(@1, $1);
}
;

// [SPEC*] level_symbol_value ::= level_symbol
level_symbol_value
: level_symbol
{
  $$ = parser.factory().new_UdpValue(@1, $1);
}
;

// [SPEC] next_state ::= output_symbol | '-'
next_state
: output_symbol
{
  $$ = $1;
}
| '-'
{
  $$ = '-';
}
;

// [SPEC] output_symbol ::= '0' | '1' | 'x' | 'X'
output_symbol
: '0'
{
  $$ = '0';
}
| '1'
{
  $$ = '1';
}
| 'x'
{
  $$ = 'x';
}
;

// [SPEC] level_symbol ::= '0' | '1' | 'x' | 'X' | '?' | 'b' | 'B'
level_symbol
: output_symbol
{
  $$ = $1;
}
| '?'
{
  $$ = '?';
}
| 'b'
{
  $$ = 'b';
}
;

// [SPEC] edge_symbol ::= 'r' | 'R' | 'f' | 'F' | 'p' | 'P' | 'n' | 'N' | '*'
edge_symbol
: 'r'
{
  $$ = 'r';
}
| 'f'
{
  $$ = 'f';
}
| 'p'
{
  $$ = 'p';
}
| 'n'
{
  $$ = 'n';
}
| '*'
{
  $$ = '*';
}
;


//////////////////////////////////////////////////////////////////////
// A.5.4 UDP instantiation
//////////////////////////////////////////////////////////////////////

// 前述の様に UDP instantiation は module instantiation と区別できないので
// いっしょに扱う．

// [SPEC] udp_instantiation ::= udp_identifier [drive_strength] [delay2]
//             udp_instance {',' udp_instance} ';'
// [SPEC] udp_instance ::= [name_of_udp_instance] '(' output_terminal ','
//             input_terminal {',' input_terminal} ')'
// [SPEC] name_of_udp_instance ::= udp_instance_identifier [range]
// [SPEC] input_terminal ::= expression
// [SPEC] output_terminal ::= net_lvalue


//////////////////////////////////////////////////////////////////////
// A.6 Behavioral statements
//////////////////////////////////////////////////////////////////////
// A.6.1 Contiuous assignment statements
//////////////////////////////////////////////////////////////////////

// [SPEC] continuous_assign ::=
//             "assign" [drive_stregth] [delay3] list_of_net_assignments ';'
continuous_assign
: ASSIGN                       list_of_net_assignments ';'
{
  $$ = parser.factory().new_ContAssignH(@$, $2.top);
}
| ASSIGN                delay3 list_of_net_assignments ';'
{
  $$ = parser.factory().new_ContAssignH(@$, $2, $3.top);
}
| ASSIGN drive_strength        list_of_net_assignments ';'
{
  $$ = parser.factory().new_ContAssignH(@$, $2, $3.top);
}
| ASSIGN drive_strength delay3 list_of_net_assignments ';'
{
  $$ = parser.factory().new_ContAssignH(@$, $2, $3, $4.top);
}
| ASSIGN error ';'
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC] list_of_net_assignments ::= net_assignment {',' net_assignments }
list_of_net_assignments
: net_assignment
{
  $$ = PtContAssignList();
  $$.init($1);
}
| list_of_net_assignments ',' net_assignment
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] net_assignment ::= net_lvalue '=' expression
net_assignment
: net_lvalue '=' expression
{
  $$ = parser.factory().new_ContAssign(@$, $1, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.6.2 Procedural blocks and assignments
//////////////////////////////////////////////////////////////////////

// [SPEC] initial_construct ::= "initial" statement
initial_construct
: INITIAL statement
{
  $$ = parser.factory().new_Initial(@$, $2);
}
;

// [SPEC] always_construct ::= "always" statement
always_construct
: ALWAYS statement
{
  $$ = parser.factory().new_Always(@$, $2);
}
;

// [SPEC] blocking_assignment ::=
//             variable_lvalue '=' [ delay_or_event_control ] expression
// delay_control/event_control/repeat_control を区別する．
// 末尾のセミコロンはこちらで処理する．
blocking_assignment
: variable_lvalue '='               expression ';'
{
  $$ = parser.factory().new_Assign(@$, $1, $3);
}
| variable_lvalue '=' delay_control expression ';'
{
  $$ = parser.factory().new_Assign(@$, $1, $4, $3);
}
| variable_lvalue '=' event_control expression ';'
{
  $$ = parser.factory().new_Assign(@$, $1, $4, $3);
}
| variable_lvalue '=' repeat_control expression ';'
{
  $$ = parser.factory().new_Assign(@$, $1, $4, $3);
}
;

// [SPEC] nonblocking_assignment ::=
//             variable_lvalue "<=" [ delay_or_event_control ] expression
// 末尾のセミコロンはこちらで処理する．
nonblocking_assignment
: variable_lvalue LTEQ               expression ';'
{
  $$ = parser.factory().new_NbAssign(@$, $1, $3);
}
| variable_lvalue LTEQ delay_control expression ';'
{
  $$ = parser.factory().new_NbAssign(@$, $1, $4, $3);
}
| variable_lvalue LTEQ event_control expression ';'
{
  $$ = parser.factory().new_NbAssign(@$, $1, $4, $3);
}
| variable_lvalue LTEQ repeat_control expression ';'
{
  $$ = parser.factory().new_NbAssign(@$, $1, $4, $3);
}
;

// [SPEC] procedural_continuous_assignments ::=
//             "assign" variable_assignment
//            |"deassign" variable_lvalue
//            |"force" variable_assignment
//            |"release" variabble_lvalue
//            |"release" net_lvalue
// [SPEC] variable_assignment ::= variable_lvalue '=' expression
// variable_lvalue と net_lvalue は区別できないので最後の形式は除く
// どうでもいいけど何でこの名前が複数形なのか不思議
// 末尾のセミコロンはこちらで処理する．
procedural_continuous_assignments
: ASSIGN variable_lvalue '=' expression ';'
{
  $$ = parser.factory().new_PcAssign(@$, $2, $4);
}
| DEASSIGN variable_lvalue ';'
{
  $$ = parser.factory().new_Deassign(@$, $2);
}
| FORCE variable_lvalue '=' expression ';'
{
  $$ = parser.factory().new_Force(@$, $2, $4);
}
| RELEASE variable_lvalue ';'
{
  $$ = parser.factory().new_Release(@$, $2);
}
;

// [SPEC] function_blocking_assignment ::= variable_lvalue '=' expression

// [SPEC] function_statement_or_null ::=
//             function_statement
//            |{ attribute_innstance } ';'

//////////////////////////////////////////////////////////////////////
// A.6.3 Parallel and sequential blocks
//////////////////////////////////////////////////////////////////////

// [SPEC] function_seq_block ::=
//             "begin" [':' block_identifer { block_item_declaration }]
//                 { function_statement }
//             "end"

// [SPEC] par_block ::=
//             "fork" [':' block_identifier { block_item_declaration }]
//                 { statement }
//             "join"

par_block
: FORK JOIN
{
  $$ = parser.factory().new_ParBlock(@$, nullptr);
}
| FORK nzlist_of_stmt JOIN
{
  $$ = parser.factory().new_ParBlock(@$, $2.top);
}
| FORK ':' IDENTIFIER
    list_of_bitem_decl
  JOIN
{
  $$ = parser.factory().new_NamedParBlock(@$, $3,
					  $4.declhead_list.top,
					  nullptr);
}
| FORK ':' IDENTIFIER
    list_of_bitem_decl
    nzlist_of_stmt
  JOIN
{
  $$ = parser.factory().new_NamedParBlock(@$, $3,
					  $4.declhead_list.top,
					  $5.top);
}
//| FORK error JOIN
//{
//  $$ = parser.factory().new_NullStmt(@$);
//  yyerrok;
//}
;

// [SPEC] seq_block ::=
//             "begin" [':' block_identifier { block_item_declaration }]
//                 { statement }
//             "end"

seq_block
: BEGIN END
{
  $$ = parser.factory().new_SeqBlock(@$, nullptr);
}
| BEGIN nzlist_of_stmt END
{
  $$ = parser.factory().new_SeqBlock(@$, $2.top);
}
| BEGIN ':' IDENTIFIER
    list_of_bitem_decl
  END
{
  $$ = parser.factory().new_NamedSeqBlock(@$, $3,
					  $4.declhead_list.top,
					  nullptr);
}
| BEGIN ':' IDENTIFIER
    list_of_bitem_decl
    nzlist_of_stmt
  END
{
  $$ = parser.factory().new_NamedSeqBlock(@$, $3,
					  $4.declhead_list.top,
					  $5.top);
}
//| BEGIN error END
//{
//  $$ = parser.factory().new_NullStmt(@$);
//  yyerrok;
//}
;

// [SPEC*] list_of_stmt ::= { statement }
nzlist_of_stmt
: statement
{
  $$ = PtStmtList();
  $$.init($1);
}
| nzlist_of_stmt statement
{
  $$ = $1;
  $$.add($2);
}
;

// [SPEC] variable_assignment ::= variable_lvalue '=' expression
// なぜこの定義がここにあるのか不思議
variable_assignment
: variable_lvalue '=' expression
{
  $$ = parser.factory().new_Assign(@$, $1, $3, nullptr);
}
;


//////////////////////////////////////////////////////////////////////
// A.6.5 Statements
//////////////////////////////////////////////////////////////////////

// [SPEC] statement ::=
//             { attribute_instance } blocking_assignment ';'
//            |{ attribute_instance } case_statement
//            |{ attribute_instance } conditional_statement
//            |{ attribute_instance } disable_statement
//            |{ attribute_instance } event_trigger
//            |{ attribute_instance } loop_statement
//            |{ attribute_instance } nonblocking_assignment ';'
//            |{ attribute_instance } par_block
//            |{ attribute_instance } procedural_continuous_assignments ';'
//            |{ attribute_instance } procedural_timing_control_statement
//            |{ attribute_instance } seq_block
//            |{ attribute_instance } system_task_enable
//            |{ attribute_instance } task_enable
//            |{ attribute_instance } wait_statement
// statement の頭の { attribute_instance } は個別のクラスで
// 処理する．
// blocking_assignment/nonblocking_assignment/
// procedural_continuous_assignments の末尾のセミコロンは
// blocking_assignment/nonblocking_assignment/
// procedural_continuous_assignments 側で処理する．
pure_statement
: blocking_assignment
{
  $$ = $1;
}
| case_statement
{
  $$ = $1;
}
| conditional_statement
{
  $$ = $1;
}
| disable_statement
{
  $$ = $1;
}
| event_trigger
{
  $$ = $1;
}
| loop_statement
{
  $$ = $1;
}
| nonblocking_assignment
{
  $$ = $1;
}
| par_block
{
  $$ = $1;
}
| procedural_continuous_assignments
{
  $$ = $1;
}
| procedural_timing_control_statement
{
  $$ = $1;
}
| seq_block
{
  $$ = $1;
}
| system_task_enable
{
  $$ = $1;
}
| task_enable
{
  $$ = $1;
}
| wait_statement
{
  $$ = $1;
}
| error ';'
{
  // ダミー
  $$ = parser.factory().new_NullStmt(@$);
  yyerrok;
}
;

statement
: ai_list pure_statement
{
  $$ = $2;
  parser.reg_attrinst($$, $1);
}
;

// [SPEC] statement_or_null ::=
//             statement
//            |{ attribute_instance } ';'
statement_or_null
: statement
{
  $$ = $1;
}
| ai_list ';'
{
  $$ = parser.factory().new_NullStmt(@$);
  parser.reg_attrinst($$, $1);
}
;

// [SPEC] function_statement ::=
//             { attribute_instance } function_blocking_assignment ';'
//            |{ attribute_instance } function_case_statement
//            |{ attribute_instance } function_conditional_statement
//            |{ attribute_instance } function_loop_statement
//            |{ attribute_instance } function_seq_block
//            |{ attribute_instance } disable_statement
//            |{ attribute_instance } system_task_enable
// function_statement は statement で処理しておいてあとで function
// では用いることの出来ない statement を弾くようにする．
// このほうが単なる parse error よりも分かりやすいエラーメッセージを
// 出力できる．


//////////////////////////////////////////////////////////////////////
// A.6.5 Timing control statements
//////////////////////////////////////////////////////////////////////

// [SPEC] delay_control ::=
//             '#' delay_value
//            |'#' '(' mintypmax_expression ')'
// [SPEC] delay_value ::=
//             unsigned number
//            |parameter_identifier
//            |specparam_identifier
//            |mintypmax_expression
// あれ？ delay_value に mintypmax_expression は含まれているんですけど．
// ということで2番目の形は冗長なので取り除いた．
delay_control
: '#' delay_value
{
  $$ = parser.factory().new_DelayControl(@$, $2);
}
| '#' '(' mintypmax_expression ')'
{
  $$ = parser.factory().new_DelayControl(@$, $3);
}
;

// [SPEC] delay_or_event_control ::=
//             delay_control
//            |event_control
//            |"repeat" '(' expression ')' event_control

// [SPEC*] repeat_control ::=
//            "repeat" '(' expression ')' event_control
repeat_control
: REPEAT '(' expression ')' '@' hierarchical_identifier
{
  auto event = parser.factory().new_Primary(@6, $6);
  $$ = parser.factory().new_RepeatControl(@$, $3, event);
}
| REPEAT '(' expression ')' '@' IDENTIFIER
{
  auto event = parser.factory().new_Primary(@6, $6);
  $$ = parser.factory().new_RepeatControl(@$, $3, event);
}
| REPEAT '(' expression ')' '@' '(' event_expression ')'
{
  $$ = parser.factory().new_RepeatControl(@$, $3, $7.top);
}
| REPEAT '(' expression ')' any_event
{
  $$ = parser.factory().new_RepeatControl(@$, $3);
}
;

// [SPEC] disable_statement ::=
//             "disable" hierarchical_task_identifier ';'
//            |"disable" hierarchical_block_identifier ';'
// [SPEC] hierarchical_task_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_block_identifier ::= hierarchical_identifier
// こういう意味的な区別を構文規則に持ち込むなっちゅうの
disable_statement
: DISABLE hierarchical_identifier ';'
{
  $$ = parser.factory().new_Disable(@$, $2);
}
| DISABLE IDENTIFIER ';'
{
  $$ = parser.factory().new_Disable(@$, $2);
}
;

// [SPEC] event_control ::=
//             '@' event_identifier
//            |'@' '(' event_expression ')'
//            |'@' '*'
//            |'@' '(' '*' ')'
// [SPEC] event_identifier ::= identifier
// なのだが，Verilog-XL では hierarchical_identifier でも通るみたい．
event_control
: '@' hierarchical_identifier
{
  auto event = parser.factory().new_Primary(@2, $2);
  $$ = parser.factory().new_EventControl(@$, event);
}
| '@' IDENTIFIER
{
  auto event = parser.factory().new_Primary(@2, $2);
  $$ = parser.factory().new_EventControl(@$, event);
}
| '@' '(' event_expression ')'
{
  $$ = parser.factory().new_EventControl(@$, $3.top);
}
| any_event
{
  $$ = parser.factory().new_EventControl(@$);
}
;

// [SPEC*] any_event ::= '@' '*' | '@' '(' '*' ')'
// 実際には attribute_instance の (*, *) との混同を防ぐためにもう少し
// 複雑なルールになる．
any_event
: '@' '*'
| '@' '(' '*' ')'
| '@' PRSTAR ')'
| '@' '(' STARPR
;

// [SPEC] event_trigger ::= "->" hierarchical_event_identifer ';'
// [SPEC] hierarchical_event_identifier ::= hierarchical_identifier
// あれ？ イベントって次元をもった配列として定義できるので
// [SPEC*] hierarchical_identifier {'[' expression ']} じゃないの？
event_trigger
: MINUSGT hierarchical_identifier ';'
{
  auto primary = parser.factory().new_Primary(@2, $2);
  $$ = parser.factory().new_EventStmt(@$, primary);
}
| MINUSGT IDENTIFIER ';'
{
  auto primary = parser.factory().new_Primary(@2, $2);
  $$ = parser.factory().new_EventStmt(@$, primary);
}
| MINUSGT hierarchical_identifier nzlist_of_index ';'
{
  auto primary = parser.factory().new_Primary(FileRegion(@2, @3),
					      $2, $3.top);
  $$ = parser.factory().new_EventStmt(@$, primary);
}
| MINUSGT IDENTIFIER nzlist_of_index ';'
{
  auto primary = parser.factory().new_Primary(FileRegion(@2, @3),
					      $2, $3.top);
  $$ = parser.factory().new_EventStmt(@$, primary);
}
;

// [SPEC] event_expression ::=
//             expression
//            |hierarchical_identifier
//            |"posedge" expression
//            |"negedge" expression
//            |event_expression "or" event_expresison
//            |event_expression ',' event_expression
// or などのパーズをうまくやるために "or" と ',' を含まない
// event_primary という非終端ノードを定義する．
// そもそも expression のなかに hierarchical_identifier は含まれてるでしょ．
event_expression
: eve_list
{
  $$ = $1;
}
;

eve_list
: event_primary
{
  $$ = PtExprList();
  $$.init($1);
}
| eve_list OR event_primary
{
  $$ = $1;
  $$.add($3);
}
| eve_list ',' event_primary
{
  $$ = $1;
  $$.add($3);
}
;

event_primary
: expression
{
  $$ = $1;
}
| POSEDGE expression
{
  $$ = parser.factory().new_Opr(@$, VpiOpType::Posedge, $2);
}
| NEGEDGE expression
{
  $$ = parser.factory().new_Opr(@$, VpiOpType::Negedge, $2);
}
;

// [SPEC] procedural_timing_control_statement ::=
//             delay_or_event_control statement_or_null
// [SPEC] delay_or_event_control ::=
//             delay_control
//            |event_control
//            |"repeat" '(' expression ')' event_control
// この最後の形式を含めると loop_statement の repeat 文と区別ができなくなる．
// そもそも procedural_timing_control_statement は statement の一部であり
// loop_statement の repeat 文で
// repeat '(' expression ')' statement
// と書いたときに statement が procedural_timing_control_statement であったら
// repeat '(' expression ')' event_control statement となるので
// 最後の形式はもともと含まれているので不要である．
procedural_timing_control_statement
: delay_control statement_or_null
{
  $$ = parser.factory().new_DcStmt(@$, $1, $2);
}
| event_control statement_or_null
{
  $$ = parser.factory().new_EcStmt(@$, $1, $2);
}
;

// [SPEC] wait_statement ::=
//             "wait" '(' expression ')' statement_or_null
wait_statement
: WAIT '(' expression ')' statement_or_null
{
  $$ = parser.factory().new_Wait(@$, $3, $5);
}
;


//////////////////////////////////////////////////////////////////////
// A.6.6 Conditional statements
//////////////////////////////////////////////////////////////////////

// [SPEC] conditional_statement ::=
//             "if" '(' expression ')'
//                 statement_or_null [ "else" statement_or_null ]
//            |if_else_if_statement
// [SPEC] if_else_if_statement ::=
//             "if" '(' expresison ')' statement_or_null
//                 { "else if" '(' expresison ')' statement_or_null }
//                 [ "else" statement_or_null ]
// if も statement なので else if の記述は不要なのにね
conditional_statement
: IF '(' expression ')' statement_or_null %prec LOWER_THAN_ELSE
{
  $$ = parser.factory().new_If(@$, $3, $5);
}
| IF '(' expression ')' statement_or_null ELSE statement_or_null
{
  $$ = parser.factory().new_IfElse(@$, $3, $5, $7);
}
;

// [SPEC] function_conditional_statement ::=
//             "if" '(' expression ')'
//                 function_statement_or_null
//                 [ "else" function_statement_or_null ]
//            |function_if_else_if_statement
// [SPEC] function_if_else_if_statement ::=
//             "if" '(' expresison ')' function_statement_or_null
//                 { "else if" '(' expresison ')' function_statement_or_null }
//                 [ "else" function_statement_or_null ]


//////////////////////////////////////////////////////////////////////
// A.6.7 Case statements
//////////////////////////////////////////////////////////////////////

// [SPEC] case_statement ::=
//             "case" '(' expression ')'
//                 case_item { case_item } "endcase"
//            |"casez" '(' expression ')'
//                 case_item { case_item } "endcase"
//            |"casex" '(' expression ')'
//                 case_item { case_item } "endcase"
case_statement
: CASE '(' expression ')'
  list_of_case_items
  ENDCASE
{
  if ( parser.check_default_label($5.top) ) {
    $$ = parser.factory().new_Case(@$, $3, $5.top);
  }
  else {
    YYERROR;
  }
}
| CASEX '(' expression ')'
  list_of_case_items
  ENDCASE
{
  if ( parser.check_default_label($5.top) ) {
    $$ = parser.factory().new_CaseX(@$, $3, $5.top);
  }
  else {
    YYERROR;
  }
}
| CASEZ '(' expression ')'
  list_of_case_items
  ENDCASE
{
  if ( parser.check_default_label($5.top) ) {
    $$ = parser.factory().new_CaseZ(@$, $3, $5.top);
  }
  else {
    YYERROR;
  }
}
;

// [SPEC*] list_of_case_items ::= case_item { case_item }
list_of_case_items
: case_item
{
  $$ = PtCaseItemList();
  $$.init($1);
}
| list_of_case_items case_item
{
  $$ = $1;
  $$.add($2);
}
;

// [SPEC] case_item ::=
//             expresison {',' expression} ':' statement_or_null
//            |"default" [':'] statement_or_null
case_item
: case_item_label statement_or_null
{
  $$ = parser.factory().new_CaseItem(@$, $1.top, $2);
}
;

case_item_label
: nzlist_of_expressions ':'
{
  $$ = $1;
}
| DEFAULT
{
  // 空リスト
  $$ = PtExprList();
  $$.init();
}
| DEFAULT               ':'
{
  // 空リスト
  $$ = PtExprList();
  $$.init();
}
;

// [SPEC] function_case_statement ::=
//             "case" '(' expression ')'
//                 function_case_item { function_case_item } "endcase"
//            |"casez" '(' expression ')'
//                 function_case_item { function_case_item } "endcase"
//            |"casex" '(' expression ')'
//                 function_case_item { function_case_item } "endcase"
// [SPEC] function_case_item ::=
//             expression {',' expression} ':' function_statement_or_null
//            |"default" [':'] function_statement_or_null


//////////////////////////////////////////////////////////////////////
// A.6.8 Looping statements
//////////////////////////////////////////////////////////////////////

// [SPEC] function_loop_statement ::=
//             "forever" function_statement
//            |"repeat" '(' expression ')' function_statement
//            |"while" '(' expression ')' function_statement
//            |"for '(' variable_assignment ';' expresson ';'
//                 variable_assignment ')' function_statement

// [SPEC] loop_statement ::=
//             "forever" statement
//            |"repeat" '(' expression ')' statement
//            |"while" '(' expression ')' statement
//            |"for '(' variable_assignment ';' expresson ';'
//                 variable_assignment ')' statement
loop_statement
: FOREVER statement
{
  $$ = parser.factory().new_Forever(@$, $2);
}
| REPEAT '(' expression ')' statement
{
  $$ = parser.factory().new_Repeat(@$, $3, $5);
}
| WHILE '(' expression ')' statement
{
  $$ = parser.factory().new_While(@$, $3, $5);
}
| FOR '(' variable_assignment ';' expression ';'
          variable_assignment ')' statement
{
  $$ = parser.factory().new_For(@$, $3, $5, $7, $9);
}
;


//////////////////////////////////////////////////////////////////////
// A.6.9 Task enable statements
//////////////////////////////////////////////////////////////////////

// [SPEC] system_task_enable ::=
//             system_task_identifier ['(' expression {',' expresison} ')'] ';'
// どうも system function/system task 系は空の引数を許しているようなので
// nzlist_of_arguments というのを作る．
// リスト自体は空でないがが要素は空がありうる．
system_task_enable
: SYS_IDENTIFIER ';'
{
  $$ = parser.factory().new_SysEnable(@$, $1, nullptr);
}
| SYS_IDENTIFIER '(' nzlist_of_arguments ')' ';'
{
  $$ = parser.factory().new_SysEnable(@$, $1, $3.top);
}
;

// [SPEC] task_enable ::=
//             hierarchical_task_identifier
//                 ['(' expression {',' expresison} ')'] ';'
task_enable
: hierarchical_identifier ';'
{
  $$ = parser.factory().new_Enable(@$, $1, nullptr);
}
| IDENTIFIER ';'
{
  $$ = parser.factory().new_Enable(@$, $1, nullptr);
}
| hierarchical_identifier '(' nzlist_of_expressions ')' ';'
{
  $$ = parser.factory().new_Enable(@$, $1, $3.top);
}
| IDENTIFIER '(' nzlist_of_expressions ')' ';'
{
  $$ = parser.factory().new_Enable(@$, $1, $3.top);
}
;


//////////////////////////////////////////////////////////////////////
// A.7 Specify section
//////////////////////////////////////////////////////////////////////
// A.7.1 Specify block declaration
//////////////////////////////////////////////////////////////////////

// [SPEC] specify_block ::= "specify" { specify_item } "endspecify"

specify_block
: SPECIFY specify_items ENDSPECIFY
{
  // 今は構文木構造を作っていない．
  $$ = nullptr;
}
;

// [SPEC*] specify_items ::= { specify_item }
// [SPEC] specify_item ::=
//             specparam_declaration
//            |pulsestyle_declaration
//            |showcancelled_declaration
//            |path_declaration
//            |system_timing_check
// [SPEC] pulsestyle_declaration ::=
//             "pulsestyle_onevent" list_of_path_outputs ';'
//            |"pulsestyle_ondetect" list_of_path_outputs ';'
// [SPEC] showcancelled_declaration ::=
//             "showcancelled" list_of_path_outputs ';'
//            |"noshowcancelled" list_of_path_outputs ';'
// [SPEC] path_declaration ::=
//             simple_path_declaration ';'
//            |edge_sensitive_path_declaration ';'
//            |state_dependent_path_declaration ';'
// [SPEC] state_dependent_path_declaration ::=
//             "if" '(' module_path_expression ')' simple_path_declaration
//            |"if" '(' module_path_expression ')'
//                 edge_sensitive_path_declaration
//            |"ifnone" simple_path_declaration
// [SPEC] list_of_path_outputs ::=
//             specify_output_terminal_descriptor
//                 {',' specify_output_terminal_descriptor }
// 下の式に現れる path_declaration は実際には
// simple_path_declaration + edge_sensitive_path_declaration である．
// state_dependent_path_declaration は含まないので個別に規則を追加している．
specify_items
: // 空もありうる
| specify_items specify_item
;

specify_item
: specparam_declaration
| PULSESTYLE_ONEVENT  nzlist_of_terminals ';'
{
  //$$ = parser.factory().new_SpecItem(@$, VpiSpecItemType::PulsestyleOnEvent, $2);
}
| PULSESTYLE_ONDETECT nzlist_of_terminals ';'
{
  //$$ = parser.factory().new_SpecItem(@$, VpiSpecItemType::PulsestyleOnDetect, $2);
}
| SHOWCANCELLED   nzlist_of_terminals ';'
{
  //$$ = parser.factory().new_SpecItem(@$, VpiSpecItemType::Showcancelled, $2);
}
| NOSHOWCANCELLED nzlist_of_terminals ';'
{
  //$$ = parser.factory().new_SpecItem(@$, VpiSpecItemType::Noshowcancelled, $2);
}
| path_declaration ';'
{
  //$$ = parser.factory().new_SpecPath(@$, VpiSpecPathType::Null, nullptr, $1);
}
| IF '(' module_path_expression ')' path_declaration ';'
{
  //$$ = parser.factory().new_SpecPath(@$, VpiSpecPathType::If, $3, $5);
}
| IFNONE simple_path_declaration ';'
{
  //$$ = parser.factory().new_SpecPath(@$, VpiSpecPathType::Ifnone, nullptr, $2);
}
| system_timing_check
{
  // system_timing_check はまともにパーズしていない．
  //$$ = nullptr;
}
| error ';'
{
  //$$ = nullptr;
  yyerrok;
}
;


//////////////////////////////////////////////////////////////////////
// A.7.2 Specify path declarations
//////////////////////////////////////////////////////////////////////

// [SPEC] path_declaration ::=
//             simple_path_declaration ';'
//            |edge_sensitive_path_declaration ';'
//            |state_dependent_path_declaration ';'
// [SPEC] simple_path_declaration ::=
//             parallel_path_description '=' path_delay_value
//            |full_path_description '=' path_delay_value
// [SPEC] parallel_path_description ::=
//             '(' specify_input_terminal_descriptor [ polarity_operator ]
//                 '=>' specify_output_terminal_descriptor ')'
// [SPEC] full_path_description ::=
//             '(' list_of_path_inputs [polarity_operator ]
//                 '*>' list_of_path_outputs ')'
// [SPEC] list_of_path_inputs ::=
//             specify_input_terminal_descriptor
//                 {',' specify_input_terminal_descriptor }
// [SPEC] list_of_path_outputs ::=
//             specify_output_terminal_descriptor
//                 {',' specify_output_terminal_descriptor }
// [SPEC] polarity_operator ::= '+'|'-'
// path_declaration は似て非なる構文がたくさんあるので面倒くさい．
// simple_path_declaration と edge_sensitive_path_declaration は
// 頭が共通で後ろに ':' expression がつくかどうかだけで判断する場合が
// あるので異なる非終端ノードにして還元することができない．
// また，本当は parallel_path_description の最初は単一の terminal だが
// 要素数が1の list_of_path_inputs(nzlist_of_terminals)と区別がつかず,
// コンフリクトを起してしまうのでとりあえず nzlist_of_terminals でパーズ
// しておいて複数の terminal を含んでいたらエラーとする．
// セミコロンは上で処理している．
path_declaration
: simple_path_declaration
{
  $$ = $1;
}
| edge_sensitive_path_declaration
{
  $$ = $1;
}
;

simple_path_declaration
: '(' nzlist_of_terminals        EQGT   specify_terminal ')'
      '=' path_delay_value
{
  $$ = parser.factory().new_PathDecl(@$, 0,
				     $2.top, 0,
				     VpiPathType::Parallel,
				     $4, 0,
				     nullptr, $7);
}
| '(' nzlist_of_terminals pol_op EQGT   specify_terminal ')'
      '=' path_delay_value
{
  $$ = parser.factory().new_PathDecl(@$, 0,
				     $2.top, $3,
				     VpiPathType::Parallel,
				     $5, 0,
				     nullptr, $8);
}
| '(' nzlist_of_terminals        STARGT nzlist_of_terminals ')'
      '=' path_delay_value
{
  $$ = parser.factory().new_PathDecl(@$, 0,
				     $2.top, 0,
				     VpiPathType::Full,
				     $4.top, 0,
				     nullptr, $7);
}
| '(' nzlist_of_terminals pol_op STARGT nzlist_of_terminals ')'
      '=' path_delay_value
{
  $$ = parser.factory().new_PathDecl(@$, 0,
				     $2.top, $3,
				     VpiPathType::Full,
				     $5.top, 0,
				     nullptr, $8);
}
;

pol_op
: '+'
{
  $$ = '+';
}
| '-'
{
  $$ = '-';
}
;


//////////////////////////////////////////////////////////////////////
// A.7.3 Specify block terminals
//////////////////////////////////////////////////////////////////////

// [SPEC] specify_input_terminal_descriptor ::=
//             input_identifier
//            |input_identifier '[' constant_expression ']'
//            |input_identifier '[' range_expression ']'
// [SPEC] specify_output_terminal_descriptor ::=
//             output_identifier
//            |output_identifier '[' constant_expression ']'
//            |output_identifier '[' range_expression ']'
// [SPEC] input_identifier ::=
//             input_port_identifier
//            |inout_port_identifier
// [SPEC] output_identifier ::=
//             output_port_identifier
//            |inout_port_identifier
// [SPEC] inout_port_identifier ::= identifier
// [SPEC] input_port_identifier ::= identifier
// [SPEC] output_port_identifier ::= identifier
// お定まりの無意味な構文規則．これを整理すると以下のようになる．
//
// [SPEC*] specify_terminal ::=
//              identifier
//             |identifier '[' constant_expression ']'
//             |identifier '[' range_expression ']'
// この3番目の形式の range_expression って constant_range_expression の
// 誤りでしょ．
specify_terminal
: IDENTIFIER
{
  $$ = parser.factory().new_Primary(@1, $1);
}
| IDENTIFIER index
{
  $$ = parser.factory().new_CPrimary(@$, $1, $2);
}
| IDENTIFIER part
{
  $$ = parser.factory().new_CPrimary(@$, $1, $2);
}
;

// [SPEC*] nzlist_of_terminals ::=
//              specify_terminal {',' specify_terminal}
nzlist_of_terminals
: specify_terminal
{
  $$ = PtExprList();
  $$.init($1);
}
| nzlist_of_terminals ',' specify_terminal
{
  $$ = $1;
  $$.add($3);
}
;


//////////////////////////////////////////////////////////////////////
// A.7.4 Specify path delays
//////////////////////////////////////////////////////////////////////

// [SPEC] path_delay_value ::=
//             list_of_path_delay_expressions
//            |'(' list_of_path_delay_expressions ')'
// list_of_path_delay_expressions が実際には path_delay_expression 単体
// の場合には '(' list_of_path_delay_expressions ')' が重複するので
// カッコは複数のpath_delay_expression を持つときのみにする．
// そのため，list_of_path_delay_expressions から path_delay_expression
// 単体の場合を取り除き path_delay_value に直接加える．
path_delay_value
: path_delay_expression
{
  $$ = parser.factory().new_PathDelay(@1, $1);
}
| list_of_path_delay_expressions
{
  $$ = $1;
}
| '(' list_of_path_delay_expressions ')'
{
  $$ = $2;
}
;

// [SPEC] list_of_path_delay_expressions ::=
//             path_delay_expression
//            |path_delay_expression ',' path_delay_expression
//            |path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression
//            |path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression
//            |path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression
//                 path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression ','
//                 path_delay_expression ',' path_delay_expression
list_of_path_delay_expressions
: path_delay_expression ',' path_delay_expression
{
  $$ = parser.factory().new_PathDelay(@$, $1, $3);
}
| path_delay_expression ',' path_delay_expression ','
  path_delay_expression
{
  $$ = parser.factory().new_PathDelay(@$, $1, $3, $5);
}
| path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression
{
  $$ = parser.factory().new_PathDelay(@$,
				      $1, $3, $5,
				      $7, $9, $11);
}
| path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression ','
  path_delay_expression ',' path_delay_expression
{
  $$ = parser.factory().new_PathDelay(@$,
				      $1, $3, $5,
				      $7, $9, $11,
				      $13, $15, $17,
				      $19, $21, $23);
}
;

// [SPEC] path_delay_expression ::= constant_mintypmax_expression
// 前述のように constant_mintypmax_expression は constant_expression
// を含まないので明示的に加える必要がある．
path_delay_expression
: expression
{
  $$ = $1;
}
| mintypmax_expression
{
  $$ = $1;
}
;

// [SPEC] edge_sensitive_path_declaration ::=
//             parallel_edge_sensitive_path_description '=' path_delay_value
//            |full_edge_sensitive_path_description '=' path_delay_value
// [SPEC] parallel_edge_sensitive_path_description ::=
//             '(' [ edge_identifier ] specify_input_terminal_descriptor
//                 '=>' specify_output_terminal_descriptor
//                 [ polarity_operator ] ':' data_source_expression ')'
// [SPEC] full_edge_sensitive_path_description ::=
//             '(' [ edge_identifier ] list_of_path_inputs "*>"
//                 list_of_path_outputs [ polarity_operator ] ':'
//                 data_source_expression ')'
// [SPEC] data_source_expression ::= expression
// [SPEC] edige_identifier ::= "posedge"|"negedge"
// [SPEC] polarity_operator ::= '+'|'-'
// 仰天！ 仕様が違う．
// 実際には specify_output_terminal_descript [polarity_identifier] ':'
//  data_source_expression を囲む '(' ')' が必要みたい．
// Verilog-XL もそうなっている．これなら reduce/reduce conflict も起きない
// ような気がする．
// parallel_edge_sensitive_path_description は実際にはリストではなく
// edge_sensitive_path_declaration で edge_identifier が省略された場合には
// simple_path_declaration と同じになるので省略はなし．
// 要素数は常に1だがパーズの都合上 nzlist_of_terminals にしている．
// あとで要素数が1であることを確認する必要がある．
//
// 2010.08.21追記
// parallel_edge_sensitive_path_description や
// full_edge_sensitive_path_description で '+'|'-'
// の直後に':'が来る場合がある．
// ところが連続した "+:" や "-:" はそれぞれ PLUSCOLON，MINUSCOLON
// というトークンになってしまう．
// そのため，PLUSCOLON, MINUSCOLON を含んだ構文を追加する．
// 具体的には pol_colon という非終端ノードを追加している．
edge_sensitive_path_declaration
: '('      nzlist_of_terminals EQGT
    '(' specify_terminal           expression ')' ')' '=' path_delay_value
{ // parallel_edge_sensitive_path_description
  $$ = parser.factory().new_PathDecl(@$, 0,
				     $2.top, 0,
				     VpiPathType::Parallel,
				     $5, 0,
				     $6, $10);
}
| '('      nzlist_of_terminals EQGT
    '(' specify_terminal pol_colon expression ')' ')' '=' path_delay_value
{ // parallel_edge_sensitive_path_description
  $$ = parser.factory().new_PathDecl(@$, 0,
				     $2.top, 0,
				     VpiPathType::Parallel,
				     $5, $6,
				     $7, $11);
}
| '(' edge nzlist_of_terminals EQGT
    '(' specify_terminal          expression ')' ')' '=' path_delay_value
{ // parallel_edge_sensitive_path_description
  $$ = parser.factory().new_PathDecl(@$, $2,
				     $3.top, 0,
				     VpiPathType::Parallel,
				     $6, 0,
				     $7, $11);
}
| '(' edge nzlist_of_terminals EQGT
    '(' specify_terminal pol_colon expression ')' ')' '=' path_delay_value
{ // parallel_edge_sensitive_path_description
  $$ = parser.factory().new_PathDecl(@$, $2,
				     $3.top, 0,
				     VpiPathType::Parallel,
				     $6, $7,
				     $8, $12);
}
| '('      nzlist_of_terminals STARGT
     '(' nzlist_of_terminals           expression ')' ')' '=' path_delay_value
{ // full_edge_sensitive_path_description
  $$ = parser.factory().new_PathDecl(@$, 0,
				     $2.top, 0,
				     VpiPathType::Full,
				     $5.top, 0,
				     $6, $10);
}
| '('      nzlist_of_terminals STARGT
     '(' nzlist_of_terminals pol_colon expression ')' ')' '=' path_delay_value
{ // full_edge_sensitive_path_description
  $$ = parser.factory().new_PathDecl(@$, 0,
				     $2.top, 0,
				     VpiPathType::Full,
				     $5.top, $6,
				     $7, $11);
}
| '(' edge nzlist_of_terminals STARGT
    '(' nzlist_of_terminals           expression ')' ')' '=' path_delay_value
{ // full_edge_sensitive_path_description
  $$ = parser.factory().new_PathDecl(@$, $2,
				     $3.top, 0,
				     VpiPathType::Full,
				     $6.top, 0,
				     $7, $11);
}
| '(' edge nzlist_of_terminals STARGT
    '(' nzlist_of_terminals pol_colon expression ')' ')' '=' path_delay_value
{ // full_edge_sensitive_path_description
  $$ = parser.factory().new_PathDecl(@$, $2,
				     $3.top, 0,
				     VpiPathType::Full,
				     $6.top, $7,
				     $8, $12);
}
;

pol_colon
: '+' ':'
{
  $$ = '+';
}
| '-' ':'
{
  $$ = '-';
}
| PLUSCOLON
{
  // + と : がくっついていた
  $$ = '+';
}
| MINUSCOLON
{
  // - と : がくっついていた
  $$ = '-';
}
;

edge
: POSEDGE
{
  $$ = vpiPosedgeOp;
}
| NEGEDGE
{
  $$ = vpiNegedgeOp;
}
;


//////////////////////////////////////////////////////////////////////
// A.7.5.1 System timing check commands
//////////////////////////////////////////////////////////////////////

system_timing_check
: SETUP '(' timing_check_event ',' timing_check_event ',' expression
            opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| HOLD '(' timing_check_event ',' timing_check_event ',' expression
           opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| SETUPHOLD '(' timing_check_event ',' timing_check_event ','
  expression ',' expression timing_check_options ')' ';'
{
  $$ = 0; // ダミー
}
| RECOVERY '(' timing_check_event ',' timing_check_event ',' expression
               opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| REMOVAL '(' timing_check_event ',' timing_check_event ',' expression
              opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| RECREM '(' timing_check_event ',' timing_check_event ','
             expression ',' expression timing_check_options ')' ';'
{
  $$ = 0; // ダミー
}
| SKEW '(' timing_check_event ',' timing_check_event ',' expression
           opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| TIMESKEW '(' timing_check_event ',' timing_check_event ','
               expression skew_timing_check_options ')' ';'
{
  $$ = 0; // ダミー
}
| FULLSKEW '(' timing_check_event ',' timing_check_event ','
               expression ',' expression skew_timing_check_options ')' ';'
{
  $$ = 0; // ダミー
}
| PERIOD '(' controlled_timing_check_event ',' expression
             opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| WIDTH '(' controlled_timing_check_event ',' expression ','
            expression opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
| NOCHANGE '(' timing_check_event ',' timing_check_event ','
               mintypmax_expression ','
               mintypmax_expression opt_notify_reg ')' ';'
{
  $$ = 0; // ダミー
}
;

timing_check_options
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| ',' IDENTIFIER
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' mintypmax_expression
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' mintypmax_expression ',' mintypmax_expression
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' mintypmax_expression ',' mintypmax_expression
  ',' delayed_data
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' mintypmax_expression ',' mintypmax_expression
  ',' delayed_data ',' delayed_data
{
  $$ = 0; // ダミー
}
;

skew_timing_check_options
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| ',' IDENTIFIER
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' expression
{
  $$ = 0; // ダミー
}
| ',' IDENTIFIER ',' expression ',' mintypmax_expression
{
  $$ = 0; // ダミー
}
;

opt_notify_reg
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| ',' IDENTIFIER
{
  $$ = 0; // ダミー
}
;


//////////////////////////////////////////////////////////////////////
// A.7.5.2 System timing check command arguments
//////////////////////////////////////////////////////////////////////

// [SPEC] delayed_data ::=
//             terminal_identifier
//            |terminal_identifier [ constant_mintypmax_expression ]
delayed_data
: IDENTIFIER
{
  $$ = 0; // ダミー
}
| IDENTIFIER '[' mintypmax_expression ']'
{
  $$ = 0; // ダミー
}
;
// CHECK_CONST($3)


//////////////////////////////////////////////////////////////////////
// A.7.5.3 System timing check event definitions
//////////////////////////////////////////////////////////////////////

timing_check_event
: opt_timing_check_event_control specify_terminal
  opt_timing_check_condition
{
  $$ = 0; // ダミー
}
;

controlled_timing_check_event
: timing_check_event_control specify_terminal
  opt_timing_check_condition
{
  $$ = 0; // ダミー
}
;

opt_timing_check_event_control
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| timing_check_event_control
{
  $$ = 0; // ダミー
}
;

timing_check_event_control
: POSEDGE
{
  $$ = 0; // ダミー
}
| NEGEDGE
{
  $$ = 0; // ダミー
}
| EDGE '[' edge_descriptor ']'
{
  $$ = 0; // ダミー
}
| EDGE '[' edge_descriptor ',' edge_descriptor ']'
{
  $$ = 0; // ダミー
}
;

edge_descriptor
: UNUMBER // 01, 10, z0, Z0, z1, Z1, x0, X0, x1, X1, 0z, 0Z, 1z, 1Z,
          // 0x, 0X, 1x, 1X のみが有効
{
  $$ = 0; // ダミー
}
;

opt_timing_check_condition
: // 空
{
  $$ = 0; // ダミー
  @$ = FileRegion();
}
| ANDANDAND timing_check_condition
{
  $$ = 0; // ダミー
}
;

timing_check_condition
: expression
{
  $$ = 0; // ダミー
}
;

/*
timing_check_condition
: scalar_timing_check_condition
| '(' scalar_timing_check_condition ')'
;

scalar_timing_check_condition
: expression
| '~' expression
| expression EQEQ scalar_constant
| expression EQEQEQ scalar_constant
| expression NOTEQ scalar_constant
| expression NOTEQEQ scalar_constant
;

scalar_constant
: UNUMBER // 1ビットの01のみ
;
*/


//////////////////////////////////////////////////////////////////////
// A.8 Expressions
//////////////////////////////////////////////////////////////////////
// A.8.1 Concatenations
//////////////////////////////////////////////////////////////////////

// [SPEC] concatenation ::= '{' expression {',' expression } '}'
concatenation
: '{' nzlist_of_expressions '}'
{
  $$ = parser.factory().new_Concat(@$, $2.top);
}
;

// [SPEC] constant_concatenation ::=
//             '{' constant_expression {',' constant_expression } '}'

// [SPEC] constant_multiple_concatenation ::=
//             '{' constant_expression constant_concatenation '}'

// [SPEC*] nzlist_of_constant_expressions ::=
//              constant_expression {',' constant_expression }

// [SPEC] module_path_concatenation ::=
//             '{' module_path_expression {',' module_path_expression } '}'
// [SPEC] module_path_multiple_concatenation ::=
//             '{' constant_expression module_path_concatenation '}'

// [SPEC] multiple_concatenation ::=
//             '{' constante_expression concatenation '}'
// 本当は $2 は constant_expression だが
// そうすると大量の reduce/reduce conflict が起きる
// 構文解析では expression と甘くしておいて
// あとで constant_expression かどうかチェックするしかない
multiple_concatenation
: '{' expression '{' nzlist_of_expressions '}' '}'
{
  $$ = parser.factory().new_MultiConcat(@$, $2, $4.top);
}
;

// [SPEC*] nzlist_of_expression ::=
//              expression {',' expression }
nzlist_of_expressions
: expression
{
  $$ = PtExprList();
  $$.init($1);
}
| nzlist_of_expressions ',' expression
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] net_concatenation ::=
//             '{' net_concatenation_value {',' net_concatenation_value } '}'
// [SPEC] net_concatenation_value ::=
//             hierarchical_net_identifier
//            |hierarchical_net_identifier
//                 '[' expression ']' {'[' expression ']'}
//            |hierarchical_net_identifier
//                 '[' expression ']' {'[' expression ']'} [ range_expression ]
//            |hierarchical_net_identifier [ range_expression ]
//            |net_concatenation
// オリジナルでは net_concatenation_value という非終端記号を定義しているが
// これは net_lvalue と同一のものであるので置き換えた
// [SPEC] variable_concatenation ::=
//    '{' variable_concatenation_value {',' variable_concatenation_value } '}'
// [SPEC] variable_concatenation_value ::=
//    hierarchical_variable_identifier
//   |hierarchical_variable_identifier '[' expression ']' {'[' expression ']'}
//   |hierarchical_variable_identifier '[' expression ']' {'[' expression ']'}
//      [ range_expression ]
//   |hierarchical_variable_identifier [ range_expression ]
//   |variable_concatenation
// オリジナルでは variable_concatenation_value という非終端記号を定義して
// いるがこれは variable_lvalue と同一のものであるので置き換えた
// さらに net_concatenation と variable_concatenation は区別がほとんどつか
// ないので一つにした．
// しかし良く考えてみるとこの net_concatenation_value の中の expression
// は constant_expression の間違いだよね．どっちにしろ同じだけど．


//////////////////////////////////////////////////////////////////////
// A.8.2 Function calls
//////////////////////////////////////////////////////////////////////

// [SPEC] constant_function_call ::=
//             function_identifier { attribute_instance }
//                 '(' constant_expression {',' constant_expression} ')'
// [SPEC] function_identifier ::= identifier

// [SPEC] function_call ::=
//             hierarchical_function_identifier { attribute_instance }
//                '(' expression {',' expression} ')'
// [SPEC] hierarchical_function_identifier ::= hierarchical_identifier
// 仕様によれば空の引数はないはずだが iverilog のテストスートに存在する．
// でもこれは iverilog がバカなのか？ <<== そうみたい．
// Verilog-XL でもシンタックスエラーになるので無視
function_call
: hierarchical_identifier ai_list '(' nzlist_of_expressions ')'
{
  $$ = parser.factory().new_FuncCall(@$, $1, $4.top);
  parser.reg_attrinst($$, $2);
}
| IDENTIFIER ai_list '(' nzlist_of_expressions ')'
{
  $$ = parser.factory().new_FuncCall(@$, $1, $4.top);
  parser.reg_attrinst($$, $2);
}
;

// [SPEC] genvar_function_call ::=
//             genvar_function_identifier { attribute_instance }
//                  '(' constant_expression {',' constant_expression} ')'

// [SPEC] system_function_call ::=
//             system_function_identifier
//                 ['(' expression {',' expression } ')']
// [SPEC] system_function_identifier ::= $[a-zA-Z0-9_$]{[a-zA-Z0-9_$]}
// どうも空の引数を許しているようなので nzlist_of_arguments というのを作る．
system_function_call
: SYS_IDENTIFIER
{
  $$ = parser.factory().new_SysFuncCall(@$, $1);
}
| SYS_IDENTIFIER '(' nzlist_of_arguments ')'
{
  $$ = parser.factory().new_SysFuncCall(@$, $1, $3.top);
}
;

// [SPEC*] nzlist_of_arguments ::= argument {',' argument }
nzlist_of_arguments
: argument
{
  $$ = PtExprList();
  $$.init($1);
}
| nzlist_of_arguments ',' argument
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC*] argument ::= /* 空 */ | expression
argument
: /* 空 */
{
  $$ = nullptr;
  @$ = FileRegion();
}
| expression
{
  $$ = $1;
}
;


//////////////////////////////////////////////////////////////////////
// A.8.3 Expressions
//////////////////////////////////////////////////////////////////////

// expression 関係は元の構文のままではとても使えないので大幅に変えている．
// そのためオリジナルの構文規則をまとめて書いておく．
// [SPEC] base_expression ::= expression
// [SPEC] conditional_expression ::= expression1 '?' { attribute_instance }
//                 expression2 ':' expression3
// アホか！ なんで expression1, expression2, expression3 が必要なわけ？
// [SPEC] constant_base_expression ::= constant_expression
// [SPEC] constant_expression ::=
//             constant_primary
//            |unary_operator { attribute_instance } constant_primay
//            |constant_expression '?' { attribute_instance }
//                 constant_expression ':' constant_expression
//            |string
// [SPEC] constant_mintypmax_expression ::=
//             constant_expression
//            |constant_expression ':' constant_expression ':'
//                 constant_expression
// [SPEC] constant_range_expression ::=
//             constant_expression
//            |msb_constant_expression ':' lsb_constant_expression
//            |constant_base_expression "+:" width_constant_expression
//            |constant_base_expression "-:" width_constant_expression
// [SPEC] dimension_constant_expression ::= constant_expression
// [SPEC] expression1 ::= expression
// [SPEC] expression2 ::= expression
// [SPEC] expression3 ::= expression
// [SPEC] expression ::=
//             primay
//            |unary_operator { attribute_instance } primary
//            |expression binary_operator { attribute_instance } expression
//            |conditional_expression
//            |string
// [SPEC] lsb_constant_expression ::= constant_expression
// [SPEC] mintypmax_expression ::=
//             expression
//            |expression ':' expression ':' expression
// [SPEC] module_path_conditional_expression ::=
//             module_path_expression '?' { attribute_instance }
//                 module_path_expression ':' module_path_expression
// [SPEC] module_path_expression ::=
//             module_path_primary
//            |unary_module_path_operator { attribute_instance }
//                 module_path_parimary
//            |module_path_expression binary_module_path_operator
//                 module_path_expression
//            |module_path_conditional_expression
// [SPEC] module_path_mintypmax_expression ::=
//             module_path_expression
//            |module_path_expression ':' module_path_expression
//             module_path_expression
// [SPEC] msb_constant_expression ::= constant_expression
// [SPEC] range_expression ::=
//             expression
//            |msb_constant_expression ':' lsb_constant_expression
//            |base_expression "+:" width_constant_expression
//            |base_expression "-:" width_constant_expression
// [SPEC] width_constant_expression ::= constant_expression

// 要するに conditional_expression の第1項めと第2項めには expression
// が書けない．なぜかというとそこに conditional_expression があると
// どこに還元して良いのか分からなくなるので．
// 書きたければ '(' ')' でくくれば primary と見なされるので正しい文法となる．
expression
: expr1 %prec ANDAND
{
  $$ = $1;
}
| expr1 '?' ai_list expr1 ':' expression %prec COND
{
  $$ = parser.new_Opr(@$, VpiOpType::Condition, $1, $4, $6, $3);
}
;

expr1
: primary
{
  $$ = $1;
}
| '+' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::Plus, $3, $2);
}
| '-' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::Minus, $3, $2);
}
| '!' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::Not, $3, $2);
}
| '~' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::BitNeg, $3, $2);
}
| '&' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::UnaryAnd, $3, $2);
}
| '|' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::UnaryOr, $3, $2);
}
| '^' ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::UnaryXor, $3, $2);
}
| TILDEAND ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::UnaryNand, $3, $2);
}
| TILDEOR ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::UnaryNor, $3, $2);
}
| TILDEXOR ai_list primary %prec UOP
{
  $$ = parser.new_Opr(@$, VpiOpType::UnaryXNor, $3, $2);
}
| expr1 STARSTAR ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Power, $1, $4, $3);
}
| expr1 '+' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Add, $1, $4, $3);
}
| expr1 '-' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Sub, $1, $4, $3);
}
| expr1 '*' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Mult, $1, $4, $3);
}
| expr1 '/' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Div, $1, $4, $3);
}
| expr1 '%' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Mod, $1, $4, $3);
}
| expr1 EQEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Eq, $1, $4, $3);
}
| expr1 NOTEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Neq, $1, $4, $3);
}
| expr1 EQEQEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::CaseEq, $1, $4, $3);
}
| expr1 NOTEQEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::CaseNeq, $1, $4, $3);
}
| expr1 ANDAND ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::LogAnd, $1, $4, $3);
}
| expr1 OROR ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::LogOr, $1, $4, $3);
}
| expr1 '<' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Lt, $1, $4, $3);
}
| expr1 '>' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Gt, $1, $4, $3);
}
| expr1 LTEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Le, $1, $4, $3);
}
| expr1 GTEQ ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::Ge, $1, $4, $3);
}
| expr1 '&' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::BitAnd, $1, $4, $3);
}
| expr1 '|' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::BitOr, $1, $4, $3);
}
| expr1 '^' ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::BitXor, $1, $4, $3);
}
| expr1 TILDEXOR ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::BitXNor, $1, $4, $3);
}
| expr1 LTLT ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::LShift, $1, $4, $3);
}
| expr1 GTGT ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::RShift, $1, $4, $3);
}
| expr1 LTLTLT ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::ArithLShift, $1, $4, $3);
}
| expr1 GTGTGT ai_list expr1
{
  $$ = parser.new_Opr(@$, VpiOpType::ArithRShift, $1, $4, $3);
}
| STRING
{
  $$ = parser.factory().new_StringConst(@$, $1);
}
;

module_path_expression
: expression
{
  $$ = $1;
}
;

// [SPEC] constant_mintypmax_expression ::=
//             constant_expression
//            |constant_expression ':' constant_expression ':'
//                 constant_expression
// この最初の形式を認めてしまうと通常の constant_expression にカッコを
// つけたものが constant_mintypmax_expression になってしまうので認めない．
// そのかわり constant_mintypmax_expression を用いている箇所で
// constant_expression も認めるようにする．

// [SPEC] mintypmax_expression ::=
//             expression
//            |expression ':' expression ':' expression
// この最初の形式を認めてしまうと通常の expression にカッコをつけたものが
// mintypmax_expression になってしまうので認めない．
// そのかわり mintypmax_expression を用いている箇所で
// expression も認めるようにする．
mintypmax_expression
:  expression ':' expression ':' expression
{
  $$ = parser.factory().new_MinTypMax(@$, $1, $3, $5);
}
;

// [SPEC] range_expression ::=
//             expression
//            |msb_constant_expression ':' lsb_constant_expression
//            |base_expression "+:" width_constant_expression
//            |base_expression "-:" width_constant_expression

// [SPEC] constant_range_expression ::=
//             constant_expression
//            |msb_constant_expression ':' lsb_constant_expression
//            |constant_base_expression "+:" width_constant_expression
//            |constant_base_expression "-:" width_constant_expression

// IEEE1364-2001 には expression も含まれるが
// index で '[' expression ']' を扱っており,
// それ以外では specify_terminal に
// '[' range_expression ']' が現れるだけなので
// 個別に対応して range_expression には expression を含めない．
// 本当は $1 も constant expression だが大量の reduce/reduce conflict
// が起きるので expression にしている．
// あとで constant_expression かどうかのチェックを行う．
// 先頭と末尾に '[' と ']' を追加している．


//////////////////////////////////////////////////////////////////////
// A.8.4 Primaries
//////////////////////////////////////////////////////////////////////

// [SPEC] constant_primary ::=
//             constant_concatenation
//            |constant_function_call
//            |'(' constant_mintypmax_expression ')'
//            |constant_multiple_concatenation
//            |genvar_identifier
//            |number
//            |parameter_identifier
//            |specparam_identifier
// [SPEC] parameter_identifier ::= identifier
// [SPEC] specparam_identifier ::= identifier
// genvar_identifier, parameter_identifier, specparam_identifier
// は全部 identifier なので一つにまとめる．

// [SPEC] primary ::=
//             number
//            |hierarchical_identifier
//            |hierarchical_identifier '[' expression ']' {'[' expression ']'}
//            |hierarchical_identifier '[' expression ']' {'[' expression ']'}
//                 '[' range_expression ']'
//            |hierarchical_identifier '[' range_expression ']'
//            |concatenation
//            |multiple_concatenation
//            |function_call
//            |system_function_call
//            |constant_function_call
//            |'(' mintypmax_expression ')'
// constant_function_call は function_call に含まれるので省略する．
// mintypmax_expression から expression は取り除いてあるのでここで明示的に
// 追加する．
primary
: number
{
  $$ = $1;
}
| hierarchical_identifier
{
  $$ = parser.factory().new_Primary(@1, $1);
}
| IDENTIFIER
{
  $$ = parser.factory().new_Primary(@1, $1);
}
| hierarchical_identifier nzlist_of_index
{
  $$ = parser.factory().new_Primary(@$, $1, $2.top);
}
| IDENTIFIER nzlist_of_index
{
  $$ = parser.factory().new_Primary(@$, $1, $2.top);
}
| IDENTIFIER                 part
{
  $$ = parser.factory().new_Primary(@$, $1, $2);
}
| hierarchical_identifier    part
{
  $$ = parser.factory().new_Primary(@$, $1, $2);
}
| hierarchical_identifier nzlist_of_index part
{
  $$ = parser.factory().new_Primary(@$, $1, $2.top, $3);
}
| IDENTIFIER nzlist_of_index part
{
  $$ = parser.factory().new_Primary(@$, $1, $2.top, $3);
}
| concatenation
{
  $$ = $1;
}
| multiple_concatenation
{
  $$ = $1;
}
| function_call
{
  $$ = $1;
}
| system_function_call
{
  $$ = $1;
}
| '(' expression ')'
{
  // 括弧の位置を保持するためのトリック
  $$ = parser.new_Opr(@$, VpiOpType::Null, $2, nullptr);
}
| '(' mintypmax_expression ')'
{
  // 括弧の位置を保持するためのトリック
  $$ = parser.new_Opr(@$, VpiOpType::Null, $2, nullptr);
}
;


//////////////////////////////////////////////////////////////////////
// A.8.5 Expression left-side values
//////////////////////////////////////////////////////////////////////

// [SPEC] net_concatenation ::=
//             '{' net_concatenation_value {',' net_concatenation_value } '}'
// [SPEC] net_concatenation_value ::=
//             hierarchical_net_identifier
//            |hierarchical_net_identifier
//                 '[' expression ']' {'[' expression ']'}
//            |hierarchical_net_identifier
//                 '[' expression ']' {'[' expression ']'} [ range_expression ]
//            |hierarchical_net_identifier [ range_expression ]
//            |net_concatenation
// オリジナルでは net_concatenation_value という非終端記号を定義しているが
// これは net_lvalue と同一のものであるので置き換えた
// [SPEC] variable_concatenation ::=
//    '{' variable_concatenation_value {',' variable_concatenation_value } '}'
// [SPEC] variable_concatenation_value ::=
//    hierarchical_variable_identifier
//   |hierarchical_variable_identifier '[' expression ']' {'[' expression ']'}
//   |hierarchical_variable_identifier '[' expression ']' {'[' expression ']'}
//      [ range_expression ]
//   |hierarchical_variable_identifier [ range_expression ]
//   |variable_concatenation
// オリジナルでは variable_concatenation_value という非終端記号を定義して
// いるがこれは variable_lvalue と同一のものであるので置き換えた
// さらに net_concatenation と variable_concatenation は区別がほとんどつか
// ないので一つにした．
// しかし良く考えてみるとこの net_concatenation_value の中の expression
// は constant_expression の間違いだよね．どっちにしろ同じだけど．

// [SPEC] net_lvalue ::=
//    hierarchical_net_identifier
//   |hierarchical_net_identifier '[' constant_expression ']'
//      {'[' constant_expression ']'}
//   |hierarchical_net_identifier '[' constant_expression ']'
//      {'[' constant_expression ']'} '[' constant_range_expression ']'
//   |hierarchical_net_identifier '[' constant_range_expression ']'
//   |net_concatenation
// [SPEC] variable_lvalue ::=
//    hierarchical_variable_identifier
//   |hierarchical_variable_identifier '[' expression ']'
//      {'[' expression ']'}
//   |hierarchical_variable_identifier '[' expression ']'
//      {'[' expression ']'} '[' range_expression ']'
//   |hierarchical_variable_identifier '[' range_expression ']'
//   |variable_concatenation
// [SPEC] hierarchical_net_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_variable_identifier ::= hierarchical_identifier
// net_lvalue と variable_lvalue の違いは添字などが const か否かだが
// syntax 的に切り分けるのは難しいので両方とも const はつけていない．
// あとでチェックする必要がある．
net_lvalue
: lvalue
{
  $$ = $1;
}
;

variable_lvalue
: lvalue
{
  $$ = $1;
}
;

lvalue
: hierarchical_identifier
{
  $$ = parser.factory().new_Primary(@1, $1);
}
| IDENTIFIER
{
  $$ = parser.factory().new_Primary(@1, $1);
}
| hierarchical_identifier nzlist_of_index
{
  $$ = parser.factory().new_Primary(@$, $1, $2.top);
}
| IDENTIFIER nzlist_of_index
{
  $$ = parser.factory().new_Primary(@$, $1, $2.top);
}
| hierarchical_identifier part
{
  $$ = parser.factory().new_Primary(@$, $1, $2);
}
| IDENTIFIER part
{
  $$ = parser.factory().new_Primary(@$, $1, $2);
}
| hierarchical_identifier nzlist_of_index part
{
  $$ = parser.factory().new_Primary(@$, $1, $2.top, $3);
}
| IDENTIFIER nzlist_of_index part
{
  $$ = parser.factory().new_Primary(@$, $1, $2.top, $3);
}
| '{' nzlist_of_lvalues '}'
{
  $$ = parser.factory().new_Concat(@$, $2.top);
}
;

// [SPEC*] nzlist_of_index ::= index { index }
//
// この定義はシフト/還元エラーを起こすので他の
// ExprList 系と同様の構文規則は使えない．
nzlist_of_index
: index
{
  $$ = PtExprList();
  $$.init($1);
}
| nzlist_of_index index
{
  $$ = $1;
  $$.add($2);
}
;

// [SPEC*] index ::= '[' expression ']'
index
: '[' expression ']'
{
  $$ = $2;
}
;

// [SPEC*] nzlist_of_lvalues ::= lvalue {',' lvalue }
nzlist_of_lvalues
: lvalue
{
  $$ = PtExprList();
  $$.init($1);
}
| nzlist_of_lvalues ',' lvalue
{
  $$ = $1;
  $$.add($3);
}
;


//////////////////////////////////////////////////////////////////////
// A.8.6 Operators
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// A.8.7 Numbers
//////////////////////////////////////////////////////////////////////

number
: unumber
{
  $$ = $1;
}
| numbase UNUMBER
{
  // この時の UNUMBER は numbase に対して適切な文字から構成されている
  // はず．
  $$ = parser.factory().new_IntConst(@$, $1, $2);
}
| UNUM_INT numbase UNUMBER
{
  // 2番目の UNUMBER は numbase に対して適切な文字から構成されている
  // はず．
  $$ = parser.factory().new_IntConst(@$, $1, $2, $3);
}
| UNUM_BIG numbase UNUMBER
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  @1,
		  MsgType::Error,
		  "PAR",
		  "size is too big.");
  YYERROR;
}
| rnumber
{
  $$ = $1;
}
;

unumber
: UNUM_INT
{
  $$ = parser.factory().new_IntConst(@$, $1);
}
| UNUM_BIG
{
  $$ = parser.factory().new_IntConst(@$, $1);
}
;

rnumber
: RNUMBER
{
  $$ = parser.factory().new_RealConst(@$, $1);
}
;

// 基数と符号の有無
numbase
: BASE_B
{
  $$ = VpiConstType::Binary;
}
| BASE_SB
{
  $$ = VpiConstType::SignedBinary;
}
| BASE_O
{
  $$ = VpiConstType::Oct;
}
| BASE_SO
{
  $$ = VpiConstType::SignedOct;
}
| BASE_D
{
  $$ = VpiConstType::Dec;
}
| BASE_SD
{
  $$ = VpiConstType::SignedDec;
}
| BASE_H
{
  $$ = VpiConstType::Hex;
}
| BASE_SH
{
  $$ = VpiConstType::SignedHex;
}
;


//////////////////////////////////////////////////////////////////////
// A.8.8 Strings
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// A.9.1 Attributes
//////////////////////////////////////////////////////////////////////

// [SPEC*] ai_list ::= { attribute_instance }
// [SPEC] attribute_instance ::= "(*" attr_spec {',' attr_spec} "*)"
ai_list
: // 空もありうる
{
  $$ = nullptr;
}
| nz_ai_list
{
  $$ = $1.top;
}
;

// [SPEC*] nz_ai_list ::= attribute_instance { attribute_instance }
nz_ai_list
: attr_inst
{
  $$ = PtAttrInstList();
  $$.init($1);
}
| nz_ai_list attr_inst
{
  $$ = $1;
  $$.add($2);
}
;

attr_inst
: PRSTAR nzlist_of_attr_spec STARPR
{
  $$ = parser.factory().new_AttrInst(@$, $2.top);
}
| PRSTAR error STARPR
{
  $$ = nullptr;
  yyerrok;
}
;

// [SPEC*] nzlist_of_attr_spec ::= attr_sprc {',' attr_spec}
nzlist_of_attr_spec
: attr_spec
{
  $$ = PtAttrSpecList();
  $$.init($1);
}
| nzlist_of_attr_spec ',' attr_spec
{
  $$ = $1;
  $$.add($3);
}
;

// [SPEC] attr_spec ::=
//    attr_name '=' constant_expression
//   |attr_name
// [SPEC] attr_name ::= identifier
attr_spec
: IDENTIFIER
{
  $$ = parser.factory().new_AttrSpec(@1, $1);
}
| IDENTIFIER '=' expression
{
  $$ = parser.factory().new_AttrSpec(@$, $1, $3);
}
;


//////////////////////////////////////////////////////////////////////
// A.9.2 Comments
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// A.9.3 Identifiers
//////////////////////////////////////////////////////////////////////

// [SPEC] arrayed_identifier ::=
//             simple_arrayed_identifier
//            |escaped_arrayed_identifier
// [SPEC] block_identifier ::= identifier
// [SPEC] cell_identifier ::= identifier
// [SPEC] config_identifier ::= identifier
// [SPEC] escaped_arrayed_identifier ::= escaped_identifier [ range ]
// [SPEC] escaped_hierarchical_identifier ::=
//             escaped_hierarchical_branch
//                 { '.' simple_hierarchical_branch |
//                   '.' escaped_hierarchical_branch }
// [SPEC] escaped_identifier ::=
//             \{Any_ASCII_character_except_white_space} white_space
// [SPEC] event_identifier ::= identifier
// [SPEC] function_identifier ::= identifier
// [SPEC] gate_instance_identifier ::= arrayed_identifier
// [SPEC] generate_block_identifier ::= identifier
// [SPEC] genvar_function_identifier ::= identifier /* Hierarchy disallowd */
// [SPEC] genvar_identifier ::= identifier
// [SPEC] hierarchical_block_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_event_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_function_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_identifier ::=
//             simple_hierarchical_identifier
//            |escaped_hierarchical_identifier
// [SPEC] hierarchical_net_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_variable_identifier ::= hierarchical_identifier
// [SPEC] hierarchical_task_identifier ::= hierarchical_identifier
// [SPEC] identifier ::=
//             simple_identifier
//            |escaped_identifier
// [SPEC] inout_port_identifier ::= identifier
// [SPEC] input_port_identifier ::= identifier
// [SPEC] instance_identifier ::= identifier
// [SPEC] library_identifier ::= identifier
// [SPEC] memory_identifier ::= identifier
// [SPEC] module_identifier ::= identifier
// [SPEC] module_instance_identifier ::= arrayed_identifier
// [SPEC] net_identifier ::= identifier
// [SPEC] output_port_identifier ::= identifier
// [SPEC] parameter_identifier ::= identifier
// [SPEC] port_identifier ::= identifier
// [SPEC] real_identifier ::= identifier
// [SPEC] simple_arrayed_identifier ::= simple_identifier [ range ]
// [SPEC] simple_hierarchical_indentifier ::=
//            simple_hierarchical_branch [ '.' escaped_identifier ]
// [SPEC] simple_identifier ::= [a-zA-Z_]{[a-zA-Z0-9_$]}
// [SPEC] specparam_identifier ::= identifier
// [SPEC] system_function_identifier ::= $[a-zA-Z0-9_$]{[a-zA-Z0-9_$]}
// [SPEC] system_task_identifier ::= $[a-zA-Z0-9_$]{[a-zA-Z0-9_$]}
// [SPEC] task_identifier ::= identifier
// [SPEC] terminal_identifier ::= identifier
// [SPEC] text_maxro_identifier ::= simple_identifier
// [SPEC] topmodule_identifier ::= identifier
// [SPEC] udp_identifier ::= identifier
// [SPEC] udp_instance_identifier ::= arrayed_identifier
// [SPEC] variable_identifier ::= identifier
//
// まず simple と escaped の区別をなくし hierarchical_branch を
// 展開している．そうしないと還元還元矛盾を引き起こす．
// また '[' ']' の中を UNUMBER とするとシフト還元矛盾を引き起こすので
// expression として読んでおいてあとでチェックする．
// 実際, 意味的には整数型の定数値になればいいはずである．
//
// ***注意***
// もともと hierarchical_identifier は階層のない identifier を含んでいるが
// ここでは最低でも1階層以上の階層をもつものを hierarchical_identifier と
// している．hierarchical_identifier を使うところでは IDENTIFIER も併せて
// 定義しておかないといけない．
//
// [SPEC*] hierarchical_identifier ::=
//    identifier
//  | hierarchical_identifier ['[' expression ']'] '.' identifier
hierarchical_identifier
: IDENTIFIER '.' IDENTIFIER
{
  $$ = parser.new_HierName($1, $3);
}
| IDENTIFIER '[' expression ']' '.' IDENTIFIER
{
  if ( !$3->is_index_expr() ) {
    // エラー: 添字に使える式ではなかった．
    MsgMgr::put_msg(__FILE__, __LINE__,
		    $3->file_region(),
		    MsgType::Error,
		    "PARS",
		    "illegal expression in hierarchical identifier.");
    $$ = PtHierName();
    YYERROR;
  }
  else {
    $$ = parser.new_HierName($1, $3->index_value(), $6);
  }
}
| hierarchical_identifier '.' IDENTIFIER
{
  $$ = $1;
  parser.add_HierName($1, $3);
}
| hierarchical_identifier '[' expression ']' '.' IDENTIFIER
{
  if ( !$3->is_index_expr() ) {
    // エラー: 添字に使える式ではなかった．
    MsgMgr::put_msg(__FILE__, __LINE__,
		    $3->file_region(),
		    MsgType::Error,
		    "PARS",
		    "illegal expression in hierarchical identifier.");
    $$ = PtHierName();
    YYERROR;
  }
  else {
    $$ = $1;
    parser.add_HierName($1, $3->index_value(), $6);
  }
}
;


//////////////////////////////////////////////////////////////////////
// A.9.4 Identifier branches
//////////////////////////////////////////////////////////////////////

// [SPEC] simple_hierarchical_branch ::=
//    simple_identifier ['[' unsigned_number ']']
//      [{'.' simple_identifier ['[' unsigned_number ']']
// [SPEC] escaped_hierarchical_branch ::=
//    escaped_identifier ['[' unsigned_number ']']
//      [{'.' escaped_identifier ['[' unsigned_number ']']


//////////////////////////////////////////////////////////////////////
// A.9.5 White space
//////////////////////////////////////////////////////////////////////

// [SPEC] white_space ::= space|tab|newline|eof



//////////////////////////////////////////////////////////////////////
%%

// yacc パーサーが内部で呼び出す関数
// トークンを一つ読み込みトークン番号を返す．
// 同時に意味値(semantic value)を yylval にセットする．
// また位置を yylloc にセットする．
int
yylex(
  YYSTYPE* lvalp,
  YYLTYPE* llocp,
  Parser& parser
)
{
  return parser.yylex(*lvalp, *llocp);
}

// yacc パーサーが内部で呼び出す関数
// エラーメッセージを出力する．
int
yyerror(
  YYLTYPE* llocp,
  Parser& parser,
  const char* s
)
{
  std::string s2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(s, "parse error", 11) ) {
    s2 ="syntax error";
    s2 += (s + 11);
  }
  else {
    s2 = s;
  }

  MsgMgr::put_msg(__FILE__, __LINE__,
		  *llocp,
		  MsgType::Error,
		  "PARS",
		  s2);

  return 1;
}

END_NAMESPACE_YM_VERILOG
