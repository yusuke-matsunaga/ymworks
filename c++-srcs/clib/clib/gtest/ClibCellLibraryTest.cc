
/// @file ClibCellLibraryTest.cc
/// @brief ClibCellLibraryTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2022 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/ClibCellLibrary.h"
#include "ym/StreamMsgHandler.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_CLIB

class ClibCellLibraryTest :
  public ::testing::Test
{
public:

  void
  check_lines(
    std::istream& s1,
    std::istream& s2
  );

  void
  check_lut(
    ClibLut& lut,
    const std::vector<ClibVarType>& var_list,
    const std::vector<std::vector<double>>& index_list,
    const std::vector<double>& value_list
  );

};

void
ClibCellLibraryTest::check_lines(
  std::istream& s1,
  std::istream& s2
)
{
  std::string buff1;
  std::string buff2;
  while ( getline(s1, buff1) ) {
    getline(s2, buff2);
    EXPECT_TRUE( s2 );
    EXPECT_EQ( buff1, buff2 );
  }
  getline(s2, buff2);
  EXPECT_FALSE( s2 );
}

void
ClibCellLibraryTest::check_lut(
  ClibLut& lut,
  const std::vector<ClibVarType>& var_type_array,
  const std::vector<std::vector<double>>& index_list_array,
  const std::vector<double>& value_list
)
{
  ASSERT_EQ( var_type_array.size(), lut.dimension() );
  ASSERT_EQ( index_list_array.size(), lut.dimension() );

  for ( SizeType i = 0; i < lut.dimension(); ++ i ) {
    EXPECT_EQ( var_type_array[i], lut.variable_type(i) );
  }

  SizeType vsize = 1;
  for ( SizeType i = 0; i < lut.dimension(); ++ i ) {
    auto& index_list = index_list_array[i];
    ASSERT_EQ( index_list.size(), lut.index_num(i) );
    for ( SizeType j = 0; j < lut.index_num(i); ++ j ) {
      EXPECT_FLOAT_EQ( index_list[j], lut.index(i, j) );
    }
    vsize *= lut.index_num(i);
  }

  ASSERT_EQ( value_list.size(), vsize );
  std::vector<SizeType> pos_list(lut.dimension(), 0);
  for ( auto value: value_list ) {
    std::ostringstream buf;
    const char* comma = "";
    for ( SizeType i = 0; i < lut.dimension(); ++ i ) {
      buf << comma << pos_list[i];
      comma = ", ";
    }
    EXPECT_FLOAT_EQ( value, lut.grid_value(pos_list) ) << buf.str();
    for ( SizeType i = 0; i < lut.dimension(); ++ i ) {
      auto j = lut.dimension() - i - 1;
      ++ pos_list[j];
      if ( pos_list[j] < lut.index_num(j) ) {
	break;
      }
      pos_list[j] = 0;
    }
  }
}


TEST_F(ClibCellLibraryTest, empty)
{
  ClibCellLibrary library;

  EXPECT_FALSE( library.is_valid() );
  EXPECT_TRUE( library.is_invalid() );
  EXPECT_THROW( library.name(),
		std::logic_error );
  EXPECT_THROW( library.technology(),
		std::logic_error );
  EXPECT_THROW( library.delay_model(),
		std::logic_error );
  EXPECT_THROW( library.piece_type(),
		std::logic_error );
  EXPECT_THROW( library.piece_define(),
		std::logic_error );
  EXPECT_THROW( library.bus_naming_style(),
		std::logic_error );
  EXPECT_THROW( library.date(),
		std::logic_error );
  EXPECT_THROW( library.revision(),
		std::logic_error );
  EXPECT_THROW( library.comment(),
		std::logic_error );
  EXPECT_THROW( library.time_unit(),
		std::logic_error );
  EXPECT_THROW( library.voltage_unit(),
		std::logic_error );
  EXPECT_THROW( library.current_unit(),
		std::logic_error );
  EXPECT_THROW( library.pulling_resistance_unit(),
		std::logic_error );
  EXPECT_THROW( library.capacitive_load_unit(),
		std::logic_error );
  EXPECT_THROW( library.leakage_power_unit(),
		std::logic_error );
  EXPECT_THROW( library.cell_num(),
		std::logic_error );
  EXPECT_THROW( library.cell(0),
		std::logic_error );
  EXPECT_THROW( library.cell("abc"),
		std::logic_error );
  EXPECT_THROW( library.cell_list(),
		std::logic_error );
  EXPECT_THROW( library.cell_group_num(),
		std::logic_error );
  EXPECT_THROW( library.cell_group(0),
		std::logic_error );
  EXPECT_THROW( library.cell_group_list(),
		std::logic_error );
  EXPECT_THROW( library.npn_class_num(),
		std::logic_error );
  EXPECT_THROW( library.npn_class(0),
		std::logic_error );
  EXPECT_THROW( library.npn_class_list(),
		std::logic_error );
  EXPECT_THROW( library.const0_func(),
		std::logic_error );
  EXPECT_THROW( library.const1_func(),
		std::logic_error );
  EXPECT_THROW( library.buf_func(),
		std::logic_error );
  EXPECT_THROW( library.inv_func(),
		std::logic_error );
  EXPECT_THROW( library.and_func(2),
		std::logic_error );
  EXPECT_THROW( library.nand_func(2),
		std::logic_error );
  EXPECT_THROW( library.or_func(2),
		std::logic_error );
  EXPECT_THROW( library.nor_func(2),
		std::logic_error );
  EXPECT_THROW( library.xor_func(2),
		std::logic_error );
  EXPECT_THROW( library.xnor_func(2),
		std::logic_error );
  EXPECT_THROW( library.mux2_func(),
		std::logic_error );
  EXPECT_THROW( library.mux4_func(),
		std::logic_error );
  EXPECT_THROW( library.find_ff_class(ClibSeqAttr()),
		std::logic_error );
  EXPECT_THROW( library.find_latch_class(ClibSeqAttr()),
		std::logic_error );
  EXPECT_THROW( library.pg_pat_num(),
		std::logic_error );
  EXPECT_THROW( library.pg_pat(0),
		std::logic_error );
  EXPECT_THROW( library.pg_max_input(),
		std::logic_error );
  EXPECT_THROW( library.pg_node_num(),
		std::logic_error );
  EXPECT_THROW( library.pg_node_type(0),
		std::logic_error );
  EXPECT_THROW( library.pg_input_id(0),
		std::logic_error );
  EXPECT_THROW( library.pg_input_node(0),
		std::logic_error );
  EXPECT_THROW( library.pg_edge_num(),
		std::logic_error );
  EXPECT_THROW( library.pg_edge_from(0),
		std::logic_error );
  EXPECT_THROW( library.pg_edge_to(0),
		std::logic_error );
  EXPECT_THROW( library.pg_edge_pos(0),
		std::logic_error );
  EXPECT_THROW( library.pg_edge_inv(0),
		std::logic_error );
}

TEST_F(ClibCellLibraryTest, read_mislib)
{
  try {
    auto data_dir = std::filesystem::path{DATA_DIR} / "clib";
    auto filename = data_dir / "lib2.genlib";
    auto library = ClibCellLibrary::read_mislib(filename);

    EXPECT_EQ( 29, library.cell_num() );

    std::ostringstream s;
    library.display(s);

    auto exp_filename = data_dir / "lib2.genlib.display";
    std::ifstream is1{exp_filename};
    ASSERT_TRUE( is1 );

    std::istringstream is2{s.str()};
    check_lines(is1, is2);
  }
  catch ( AssertError obj ) {
    std::cout << obj << std::endl;
    EXPECT_TRUE( false );
  }
}

TEST_F(ClibCellLibraryTest, read_liberty)
{
  try {
    auto data_dir = std::filesystem::path{DATA_DIR} / "clib";
    StreamMsgHandler handler(std::cout);
    MsgMgr::attach_handler(&handler);
    auto filename = data_dir / "HIT018.typ.snp";
    auto library = ClibCellLibrary::read_liberty(filename);

    ASSERT_TRUE( library.is_valid() );
    EXPECT_FALSE( library.is_invalid() );
    EXPECT_EQ( "HIT018", library.name() );
    EXPECT_EQ( ClibTechnology::cmos, library.technology() );
    EXPECT_EQ( ClibDelayModel::table_lookup, library.delay_model() );
    EXPECT_EQ( ClibVarType::none, library.piece_type() );
    EXPECT_EQ( std::vector<double>{}, library.piece_define() );
    EXPECT_EQ( "", library.bus_naming_style() );
    EXPECT_EQ( "", library.date() );
    EXPECT_EQ( "", library.revision() );
    EXPECT_EQ( "", library.comment() );
    EXPECT_EQ( ClibTimeUnit("1ns"), library.time_unit() );
    EXPECT_EQ( ClibVoltageUnit("1V"), library.voltage_unit() );
    EXPECT_EQ( ClibCurrentUnit("1mA"), library.current_unit() );
    EXPECT_EQ( ClibResistanceUnit(), library.pulling_resistance_unit() );
    EXPECT_EQ( ClibCapacitanceUnit(1.0, "pf"), library.capacitive_load_unit() );
    EXPECT_EQ( ClibPowerUnit(), library.leakage_power_unit() );
    EXPECT_EQ( 310, library.cell_num() );
    EXPECT_THROW( library.cell(310),
		  std::out_of_range );
    EXPECT_THROW( library.cell("bad_name"),
		  std::out_of_range );
    { // Cell#0 のテスト
      // 期待値をハードコーディングしている．
      auto cell0 = library.cell(0);
      ASSERT_TRUE( cell0.is_valid() );
      EXPECT_FALSE( cell0.is_invalid() );
      EXPECT_EQ( library, cell0.library() );
      EXPECT_EQ( "HIT18AND2P005", cell0.name() );
      auto cell1 = library.cell(cell0.name());
      EXPECT_EQ( cell0, cell1 );
      EXPECT_EQ( ClibArea(38.4), cell0.area() );
      EXPECT_EQ( 3, cell0.pin_num() );
      EXPECT_EQ( 2, cell0.input_num() );
      EXPECT_EQ( 1, cell0.output_num() );
      EXPECT_EQ( 0, cell0.inout_num() );
      EXPECT_EQ( 2, cell0.input2_num() );
      EXPECT_EQ( 1, cell0.output2_num() );
      EXPECT_EQ( 0, cell0.internal_num() );
      EXPECT_THROW( cell0.pin(3),
		    std::out_of_range );
      EXPECT_THROW( cell0.input(2),
		    std::out_of_range );
      EXPECT_THROW( cell0.output(1),
		    std::out_of_range );
      EXPECT_THROW( cell0.inout(0),
		    std::out_of_range );
      EXPECT_THROW( cell0.internal(0),
		    std::out_of_range );
      EXPECT_THROW( cell0.pin("Z"),
		    std::out_of_range );
      { // Cell#0.Pin#0 のテスト
	auto pin0 = cell0.pin(0);
	EXPECT_EQ( 0, pin0.pin_id() );
	ASSERT_TRUE( pin0.is_valid() );
	EXPECT_FALSE( pin0.is_invalid() );
	EXPECT_EQ( "A", pin0.name() );
	auto pin1 = cell0.pin(pin0.name());
	EXPECT_EQ( pin0, pin1 );
	EXPECT_EQ( ClibDirection::input, pin0.direction() );
	EXPECT_TRUE( pin0.is_input() );
	EXPECT_FALSE( pin0.is_output() );
	EXPECT_FALSE( pin0.is_inout() );
	EXPECT_FALSE( pin0.is_internal() );

	auto pin2 = cell0.input(pin0.input_id());
	EXPECT_EQ( pin0, pin2 );
	EXPECT_EQ( ClibCapacitance(0.003052), pin0.capacitance() );
	EXPECT_EQ( ClibCapacitance(0.003052), pin0.rise_capacitance() );
	EXPECT_EQ( ClibCapacitance(0.003052), pin0.fall_capacitance() );

	EXPECT_THROW( pin0.output_id(),
		      std::logic_error );
	EXPECT_THROW( pin0.max_fanout(),
		      std::logic_error );
	EXPECT_THROW( pin0.min_fanout(),
		      std::logic_error );
	EXPECT_THROW( pin0.max_capacitance(),
		      std::logic_error );
	EXPECT_THROW( pin0.min_capacitance(),
		      std::logic_error );
	EXPECT_THROW( pin0.max_transition(),
		      std::logic_error );
	EXPECT_THROW( pin0.min_transition(),
		      std::logic_error );
	EXPECT_THROW( pin0.function(),
		      std::logic_error );
	EXPECT_THROW( pin0.tristate(),
		      std::logic_error );

	EXPECT_THROW( pin0.internal_id(),
		      std::logic_error );
      }
      { // Cell#0.Pin#1 のテスト
	auto pin0 = cell0.pin(1);
	EXPECT_EQ( 1, pin0.pin_id() );
	ASSERT_TRUE( pin0.is_valid() );
	EXPECT_FALSE( pin0.is_invalid() );
	EXPECT_EQ( "B", pin0.name() );
	auto pin1 = cell0.pin(pin0.name());
	EXPECT_EQ( pin0, pin1 );
	EXPECT_EQ( ClibDirection::input, pin0.direction() );
	EXPECT_TRUE( pin0.is_input() );
	EXPECT_FALSE( pin0.is_output() );
	EXPECT_FALSE( pin0.is_inout() );
	EXPECT_FALSE( pin0.is_internal() );

	auto pin2 = cell0.input(pin0.input_id());
	EXPECT_EQ( pin0, pin2 );
	EXPECT_EQ( ClibCapacitance(0.004028), pin0.capacitance() );
	EXPECT_EQ( ClibCapacitance(0.004028), pin0.rise_capacitance() );
	EXPECT_EQ( ClibCapacitance(0.004028), pin0.fall_capacitance() );

	EXPECT_THROW( pin0.output_id(),
		      std::logic_error );
	EXPECT_THROW( pin0.max_fanout(),
		      std::logic_error );
	EXPECT_THROW( pin0.min_fanout(),
		      std::logic_error );
	EXPECT_THROW( pin0.max_capacitance(),
		      std::logic_error );
	EXPECT_THROW( pin0.min_capacitance(),
		      std::logic_error );
	EXPECT_THROW( pin0.max_transition(),
		      std::logic_error );
	EXPECT_THROW( pin0.min_transition(),
		      std::logic_error );
	EXPECT_THROW( pin0.function(),
		      std::logic_error );
	EXPECT_THROW( pin0.tristate(),
		      std::logic_error );

	EXPECT_THROW( pin0.internal_id(),
		      std::logic_error );
      }
      { // Cell#0.Pin#2 のテスト
	auto pin0 = cell0.pin(2);
	EXPECT_EQ( 2, pin0.pin_id() );
	ASSERT_TRUE( pin0.is_valid() );
	EXPECT_FALSE( pin0.is_invalid() );
	EXPECT_EQ( "Y", pin0.name() );
	auto pin1 = cell0.pin(pin0.name());
	EXPECT_EQ( pin0, pin1 );
	EXPECT_EQ( ClibDirection::output, pin0.direction() );
	EXPECT_FALSE( pin0.is_input() );
	EXPECT_TRUE( pin0.is_output() );
	EXPECT_FALSE( pin0.is_inout() );
	EXPECT_FALSE( pin0.is_internal() );

	EXPECT_THROW( pin0.capacitance(),
		      std::logic_error );
	EXPECT_THROW( pin0.rise_capacitance(),
		      std::logic_error );
	EXPECT_THROW( pin0.fall_capacitance(),
		      std::logic_error );

	auto pin2 = cell0.output(pin0.output_id());
	EXPECT_EQ( pin0, pin2 );
	EXPECT_EQ( ClibCapacitance(0.962646), pin0.max_fanout() );
	EXPECT_EQ( ClibCapacitance(0.0), pin0.min_fanout() );
	EXPECT_EQ( ClibCapacitance(0.096265), pin0.max_capacitance() );
	EXPECT_EQ( ClibCapacitance(0.0), pin0.min_capacitance() );
	EXPECT_EQ( ClibTime(std::numeric_limits<double>::infinity()),
		   pin0.max_transition() );
	EXPECT_EQ( ClibTime(0.0), pin0.min_transition() );
	auto v0 = Expr::positive_literal(0);
	auto v1 = Expr::positive_literal(1);
	EXPECT_EQ( v0 & v1, pin0.function() );
	EXPECT_EQ( Expr::invalid(), pin0.tristate() );

	EXPECT_THROW( pin0.internal_id(),
		      std::logic_error );
      }

      EXPECT_EQ( 0, cell0.bus_num() );
      EXPECT_THROW( cell0.bus(0),
		    std::out_of_range );

      EXPECT_EQ( 0, cell0.bundle_num() );
      EXPECT_THROW( cell0.bundle(0),
		    std::out_of_range );

      { // タイミングのテスト
	auto tlist0 = cell0.timing_list(0, 0, ClibTimingSense::positive_unate);
	ASSERT_EQ( 1, tlist0.size() );
	auto timing0 = tlist0[0];
	EXPECT_EQ( ClibTimingType::combinational, timing0.type() );
	EXPECT_EQ( Expr::one(), timing0.timing_cond() );

	EXPECT_THROW( timing0.intrinsic_rise(),
		      std::logic_error );
	EXPECT_THROW( timing0.intrinsic_fall(),
		      std::logic_error );
	EXPECT_THROW( timing0.slope_rise(),
		      std::logic_error );
	EXPECT_THROW( timing0.slope_fall(),
		      std::logic_error );

	EXPECT_THROW( timing0.rise_resistance(),
		      std::logic_error );
	EXPECT_THROW( timing0.fall_resistance(),
		      std::logic_error );

	EXPECT_THROW( timing0.rise_pin_resistance(0),
		      std::logic_error );
	EXPECT_THROW( timing0.fall_pin_resistance(0),
		      std::logic_error );
	EXPECT_THROW( timing0.rise_delay_intercept(0),
		      std::logic_error );
	EXPECT_THROW( timing0.fall_delay_intercept(0),
		      std::logic_error );

	auto rt = timing0.rise_transition();
	check_lut(rt,
		  {ClibVarType::total_output_net_capacitance,
		   ClibVarType::input_net_transition},
		  {{0.005, 0.05, 0.14, 0.3},
		   {0.04, 0.92, 2}},
		  {0.104, 0.132, 0.16,
		   0.471, 0.477, 0.488,
		   1.255, 1.256, 1.26,
		   2.654, 2.654, 2.657});

	auto ft = timing0.fall_transition();
	check_lut(ft,
		  {ClibVarType::total_output_net_capacitance,
		   ClibVarType::input_net_transition},
		  {{0.005, 0.05, 0.14, 0.3},
		   {0.04, 0.92, 2}},
		  {0.0849, 0.119, 0.132,
		   0.249, 0.265, 0.286,
		   0.615, 0.62, 0.629,
		   1.284, 1.286, 1.291});

	auto rp = timing0.rise_propagation();
	EXPECT_FALSE( rp.is_valid() );

	auto fp = timing0.fall_propagation();
	EXPECT_FALSE( fp.is_valid() );

	auto cr = timing0.cell_rise();
	check_lut(cr,
		  {ClibVarType::total_output_net_capacitance,
		   ClibVarType::input_net_transition},
		  {{0.005, 0.05, 0.14, 0.3},
		   {0.04, 0.92, 2}},
		  {0.116371, 0.207, 0.253,
		   0.273441, 0.3695, 0.424,
		   0.586702, 0.681321, 0.735,
		   1.14, 1.234582, 1.289372});

	auto cf = timing0.cell_fall();
	check_lut(cf,
		  {ClibVarType::total_output_net_capacitance,
		   ClibVarType::input_net_transition},
		  {{0.005, 0.05, 0.14, 0.3},
		   {0.04, 0.92, 2}},
		  {0.110102, 0.2175, 0.291,
		   0.205085, 0.3285, 0.413,
		   0.377483, 0.502, 0.5875,
		   0.679562, 0.804, 0.8895});
      }
      {
	auto tlist0 = cell0.timing_list(0, 0, ClibTimingSense::negative_unate);
	EXPECT_EQ( 0, tlist0.size() );
      }
      EXPECT_THROW( cell0.timing_list(2, 0, ClibTimingSense::positive_unate),
		    std::out_of_range );
      EXPECT_THROW( cell0.timing_list(0, 1, ClibTimingSense::positive_unate),
		    std::out_of_range );
    }
    std::ostringstream s;
    library.display(s);

    auto exp_filename = data_dir / "HIT018.typ.snp.display";
    std::ifstream is1{exp_filename};
    ASSERT_TRUE( is1 );

    std::istringstream is2{s.str()};
    check_lines(is1, is2);
  }
  catch ( AssertError obj ) {
    std::cout << obj << std::endl;
    EXPECT_TRUE( false );
  }
}

TEST_F(ClibCellLibraryTest, dump_restore)
{
  try {
    auto data_dir = std::filesystem::path{DATA_DIR} / "clib";
    auto filename = data_dir / "HIT018.typ.snp";
    auto library = ClibCellLibrary::read_liberty(filename);

    EXPECT_EQ( 310, library.cell_num() );

    std::string dump_buff;
    {
      std::ostringstream s;
      library.dump(s);
      dump_buff = s.str();
    }

    ClibCellLibrary library2;
    {
      std::istringstream s{dump_buff};
      library2 = ClibCellLibrary::restore(s);
    }

    EXPECT_EQ( library.cell_num(), library2.cell_num() );
    // 内容が等しいか調べる．
    std::ostringstream buf1;
    library.display(buf1);
    std::ostringstream buf2;
    library2.display(buf2);
    EXPECT_EQ( buf1.str(), buf2.str() );
  }
  catch ( AssertError obj ) {
    std::cerr << obj << std::endl;
    EXPECT_TRUE( false );
  }
}

END_NAMESPACE_YM_CLIB
