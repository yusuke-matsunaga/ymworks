
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
//
// [SPEC] udp_port_list ::= output_port_identifier ','
//             input_port_identifier {',' input_port_identifier }
// [SPEC] output_port_identifier ::= identifier
// [SPEC] input_port_identifier ::= identifier
//
// [SPEC*] nzlist_of_uport_decl ::=
//            udp_port_declaration { udp_port_declaration }
//
// [SPEC] udp_port_declaration ::=
//             udp_output_declaration ';'
//            |udp_input_declaration ';'
//            |udp_reg_declaration ';'
//
// [SPEC] udp_output_declaration ::=
//             { attribute_instance } "output"       port_identifier
//            |{ attribute_instance } "output" "reg" port_identifier
//                 [ '=' constant_expression]
//
// [SPEC] udp_input_declaration ::=
//             { attribute_instance } "input" list_of_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
//
// [SPEC] udp_reg_declaration ::=
//             { attribute_instance } "reg" variable_identifier
//
// [SPEC] udp_declaration_port_list ::= udp_output_declaration ','
//             udp_input_declaration {',' udp_input_declaration}
// [SPEC] udp_input_declaration ::=
//             { attribute_instance } "input" list_of_port_identifiers
// [SPEC] list_of_port_identifiers ::= port_identifier {',' port_identifier}
//
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
//
// [SPEC] combinational_entry ::= level_input_list ':' output_symbol ';'
// [SPEC] sequential_entry ::=
//             seq_input_list ':' current_state ':' next_state ';'
// [SPEC] seq_input_list ::= level_input_list | edge_input_list
//
// [SPEC] seq_input_list ::= level_input_list | edge_input_list
// [SPEC] level_input_list ::= level_symbol { level_symbol }
// [SPEC] edge_input_list ::= { level_symbol } edge_indicator { level_symbol }
//
// [SPEC] output_symbol ::= '0' | '1' | 'x' | 'X'
//
// [SPEC] current_state ::= level_symbol
//
// [SPEC] level_symbol ::= '0' | '1' | 'x' | 'X' | '?' | 'b' | 'B'
//
// [SPEC] edge_symbol ::= 'r' | 'R' | 'f' | 'F' | 'p' | 'P' | 'n' | 'N' | '*'
primitive udp_comb1995_1 ( output1, input1 );
   output output1;
   input input1;
   table
      0 : 1;
      1 : 0;
   endtable
endprimitive

primitive udp_comb1995_2 ( output1, input1, input2 );
   output output1;
   input input1;
   input input2;
   table
      00 : 1;
      01 : 1;
      10 : 1;
      11 : 0;
   endtable
endprimitive

primitive udp_comb2001_1 ( output output1, input input1 );
   table
      0 : 1;
      1 : 0;
   endtable
endprimitive

primitive udp_comb2001_2 ( output output1, input input1, input2 );
   table
      00 : 1;
      01 : 1;
      10 : 1;
      11 : 0;
   endtable
endprimitive
