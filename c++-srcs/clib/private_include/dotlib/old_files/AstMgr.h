#ifndef ASTMGR_H
#define ASTMGR_H

#error "don't include me"

/// @file AstMgr.h
/// @brief AstMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/dotlib_nsdef.h"
#include "dotlib/AttrType.h"
#include "dotlib/AstCPType.h"
#include "ym/ShString.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class AstMgr AstMgr.h "AstMgr.h"
/// @brief dotlib の AST ノードを管理するクラス
///
/// AST ノードは DotlibParser 内で生成されるが，DotlibParser が削除された
/// 後も存在している必要があるので AST ノードのオーナーとして AstMgr を
/// 用いる．
//////////////////////////////////////////////////////////////////////
class AstMgr
{
public:

  /// @brief コンストラクタ
  AstMgr();

  /// @brief デストラクタ
  ///
  /// ここで生成した全てのノードが削除される．
  ~AstMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  ///
  /// 以前，生成されたオブジェクトは破壊される．
  void
  clear();

  /// @brief 根のノードを返す．
  const AstLibrary*
  library_node() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief + 演算子を表す AstExpr を生成する．
  const AstExpr*
  new_plus(const AstExpr* opr1,  ///< [in] 第1オペランド
	   const AstExpr* opr2); ///< [in] 第2オペランド

  /// @brief - 演算子を表す AstExpr を生成する．
  const AstExpr*
  new_minus(const AstExpr* opr1,  ///< [in] 第1オペランド
	    const AstExpr* opr2); ///< [in] 第2オペランド

  /// @brief * 演算子を表す AstExpr を生成する．
  const AstExpr*
  new_mult(const AstExpr* opr1,  ///< [in] 第1オペランド
	   const AstExpr* opr2); ///< [in] 第2オペランド

  /// @brief / 演算子を表す AstExpr を生成する．
  const AstExpr*
  new_div(const AstExpr* opr1,  ///< [in] 第1オペランド
	  const AstExpr* opr2);	///< [in] 第2オペランド

  /// @brief NOT 演算子を表す AstExpr を生成する．
  const AstExpr*
  new_not(const FileRegion& loc, ///< [in] ファイル上の位置
	  const AstExpr* opr);   ///< [in] オペランド

  /// @brief AND 演算子を表す AstExpr を生成する．
  const AstExpr*
  new_and(const AstExpr* opr1,  ///< [in] 第1オペランド
	  const AstExpr* opr2);	///< [in] 第2オペランド

  /// @brief OR 演算子を表す AstExpr を生成する．
  const AstExpr*
  new_or(const AstExpr* opr1,  ///< [in] 第1オペランド
	 const AstExpr* opr2); ///< [in] 第2オペランド

  /// @brief XOR 演算子を表す AstExpr を生成する．
  const AstExpr*
  new_xor(const AstExpr* opr1,  ///< [in] 第1オペランド
	  const AstExpr* opr2);	///< [in] 第2オペランド

  /// @brief ブール値(0 or 1)を表す AstExpr を生成する．
  const AstExpr*
  new_bool_expr(const FileRegion& loc, ///< [in] ファイル上の位置
		bool val);             ///< [in] 値

  /// @brief 実数値を表す AstExpr を生成する．
  const AstExpr*
  new_float_expr(const FileRegion& loc, ///< [in] ファイル上の位置
		 double val);		///< [in] 値

  /// @brief VDDを表す AstExpr を生成する．
  const AstExpr*
  new_vdd_expr(const FileRegion& loc); ///< [in] ファイル上の位置

  /// @brief VSSを表す AstExpr を生成する．
  const AstExpr*
  new_vss_expr(const FileRegion& loc); ///< [in] ファイル上の位置

  /// @brief VCCを表す AstExpr を生成する．
  const AstExpr*
  new_vcc_expr(const FileRegion& loc); ///< [in] ファイル上の位置

  /// @brief 文字列を表す AstExpr を生成する．
  const AstExpr*
  new_str_expr(const FileRegion& loc, ///< [in] ファイル上の位置
	       const ShString& str);  ///< [in] 値


public:
  //////////////////////////////////////////////////////////////////////
  // simple attribute の値を表す AstNode を作る関数
  //////////////////////////////////////////////////////////////////////

#if 0
  /// @brief ブール値を表す AstNode を生成する．
  const AstBool*
  new_bool(const FileRegion& loc, ///< [in] ファイル上の位置
	   bool value);		  ///< [in] 値

  /// @brief clear_preset_var の値を表す AstNode を生成する．
  const AstCPType*
  new_cptype(const FileRegion& loc,  ///< [in] ファイル上の位置
	     AstCPType::Type value); ///< [in] 値

  /// @brief 整数値を表す AstNode を生成する．
  const AstInt*
  new_int(const FileRegion& loc, ///< [in] ファイル上の位置
	  int value);		 ///< [in] 値

  /// @brief 実数値を表す AstNode を生成する．
  const AstFloat*
  new_float(const FileRegion& loc, ///< [in] ファイル上の位置
	    double value);	   ///< [in] 値

  /// @brief float のベクタ型の AstNode を生成する．
  const AstFloatVector*
  new_float_vector(const FileRegion& loc,        ///< [in] ファイル上の位置
		   const vector<double>& value); ///< [in] 値

  /// @brief 整数値のベクタを表す AstNode を生成する．
  const AstIntVector*
  new_int_vector(const FileRegion& loc,          ///< [in] ファイル上の位置
		 const vector<int>& value_list); ///< [in] 値

  /// @brief 文字列シンボルを表す AstNode を生成する．
  const AstString*
  new_string(const FileRegion& loc, ///< [in] ファイル上の位置
	     ShString value);	    ///< [in] 値

  /// @brief technology を表す AstNode を生成する．
  const AstTechnology*
  new_technology(const FileRegion& loc, ///< [in] ファイル上の位置
		 ClibTechnology value);	///< [in] 値

  /// @brief delay model を表す AstNode を生成する．
  const AstDelayModel*
  new_delay_model(const FileRegion& loc, ///< [in] ファイル上の位置
		  ClibDelayModel value); ///< [in] 値

  /// @brief direction を表す AstNode を生成する．
  const AstDirection*
  new_direction(const FileRegion& loc, ///< [in] ファイル上の位置
		ClibDirection value);  ///< [in] 値

  /// @brief timing_sense を表す AstNode を生成する．
  const AstTimingSense*
  new_timing_sense(const FileRegion& loc,  ///< [in] ファイル上の位置
		   ClibTimingSense value); ///< [in] 値

  /// @brief timing_type を表す AstNode を生成する．
  const AstTimingType*
  new_timing_type(const FileRegion& loc, ///< [in] ファイル上の位置
		  ClibTimingType value); ///< [in] 値

  /// @brief var_type を表す AstNode を生成する．
  const AstVarType*
  new_var_type(const FileRegion& loc, ///< [in] ファイル上の位置
	       ClibVarType value);    ///< [in] 値
#endif

public:
  //////////////////////////////////////////////////////////////////////
  // complex attribute の値を表す AstNode を作る関数
  //////////////////////////////////////////////////////////////////////

#if 0
  /// @brief ( float, float ) 型の AstNode を生成する．
  const AstFloat2*
  new_float2(const FloatFloatHandler& handler); ///< [in] ハンドラ

  /// @brief ( float, string ) 型の AstNode を生成する．
  const AstFloatStr*
  new_float_str(const FloatStrHandler& handler); ///< [in] ハンドラ

  /// @brief ( integer, float ) 型の AstNode を生成する．
  const AstIntFloat*
  new_int_float(const IntFloatHandler& handler); ///< [in] ハンドラ

  /// @brief ( integer, float_vector ) 型の AstNode を生成する．
  const AstIntFloatVector*
  new_int_float_vector(const IntFloatVectorHandler& handler); ///< [in] ハンドラ

  /// @brief ( string, float ) 型の AstNode を生成する．
  const AstStrFloat*
  new_str_float(const StrFloatHandler& handler); ///< [in] ハンドラ

  /// @brief ( string, int ) 型の AstNode を生成する．
  const AstStrInt*
  new_str_int(const StrIntHandler& handler); ///< [in] ハンドラ

  /// @brief ( string, string, ... ) 型の AstNode を生成する．
  const AstStrList*
  new_str_list(const StrListHandler& handler); ///< [in] ハンドラ

  /// @brief ( string, string ) 型の AstNode を生成する．
  const AstStr2*
  new_str2(const StrStrHandler& handler); ///< [in] ハンドラ

  /// @brief ( string, string, string ) 型の AstNode を生成する．
  const AstStr3*
  new_str3(const StrStrStrHandler& handler); ///< [in] ハンドラ
#endif

public:
  //////////////////////////////////////////////////////////////////////
  // group statement の値を表す AstNode を作る関数
  //////////////////////////////////////////////////////////////////////

#if 0
  /// @brief ライブラリを表す AstNode を生成する．
  const AstLibrary*
  new_library(const FileRegion& attr_loc,   ///< [in] 属性のファイル上の位置
	      const StrHandler& header,     ///< [in] ヘッダを読み込んだハンドラ
	      const LibraryHandler& group); ///< [in] グループ本体を読み込んだハンドラ

  /// @brief セルを表す AstNode を生成する．
  const AstCell*
  new_cell(const FileRegion& attr_loc, ///< [in] 属性のファイル上の位置
	   const StrHandler& header,   ///< [in] ヘッダを読み込んだハンドラ
	   const CellHandler& group);  ///< [in] グループ本体を読み込んだハンドラ

  /// @brief ピンを表す AstNode を生成する．
  const AstPin*
  new_pin(const FileRegion& attr_loc,   ///< [in] 属性のファイル上の位置
	  const StrListHandler& header,	///< [in] ヘッダを読み込んだハンドラ
	  const PinHandler& group);	///< [in] グループ本体を読み込んだハンドラ

  /// @brief cell_degradation を表す AstNode を生成する．
  /// @param[in] header ヘッダを読み込んだハンドラ
  /// @param[in] group グループ本体を読み込んだハンドラ
  const AstCellDegradation*
  new_cell_degradation(const FileRegion& loc,
		       const AstString* name,
		       const AstString* calc_mode,
		       const AstFloatVector* index_1,
		       const AstFloatVector* values,
		       const AstFloatVector* coefs,
		       const AstIntVector* orders,
		       const AstVariableRange* variable_1_range,
		       const AstDomain* domain);

  /// @brief ff を表す AstNode を生成する．
  const AstFF*
  new_ff(const FileRegion& attr_loc,  ///< [in] 属性のファイル上の位置
	 const StrStrHandler& header, ///< [in] ヘッダを読み込んだハンドラ
	 const FFHandler& group);     ///< [in] グループ本体を読み込んだハンドラ

  /// @brief ff_bank を表す AstNode を生成する．
  const AstFFBank*
  new_ff_bank(const FileRegion& attr_loc,     ///< [in] 属性のファイル上の位置
	      const StrStrIntHandler& header, ///< [in] ヘッダを読み込んだハンドラ
	      const FFHandler& group);	      ///< [in] グループ本体を読み込んだハンドラ

  /// @brief latch を表す AstNode を生成する．
  const AstLatch*
  new_latch(const FileRegion& attr_loc,  ///< [in] 属性のファイル上の位置
	    const StrStrHandler& header, ///< [in] ヘッダを読み込んだハンドラ
	    const LatchHandler& group);	 ///< [in] グループ本体を読み込んだハンドラ

  /// @brief latch_bank を表す AstNode を生成する．
  const AstLatchBank*
  new_latch_bank(const FileRegion& attr_loc,     ///< [in] 属性のファイル上の位置
		 const StrStrIntHandler& header, ///< [in] ヘッダを読み込んだハンドラ
		 const LatchHandler& group);	 ///< [in] グループ本体を読み込んだハンドラ

  /// @brief lut template を表す AstNode を生成する．
  const AstTemplate*
  new_template(const FileRegion& attr_loc,    ///< [in] 属性のファイル上の位置
	       const StrHandler& header,      ///< [in] ヘッダを読み込んだハンドラ
	       const TemplateHandler& group); ///< [in] グループ本体を読み込んだハンドラ

  /// @brief lut を表す AstNode を生成する．
  const AstLut*
  new_lut(const FileRegion& attr_loc,  ///< [in] 属性のファイル上の位置
	  const StrHandler& header,    ///< [in] ヘッダを読み込んだハンドラ
	  const TableHandler& group);  ///< [in] グループ本体を読み込んだハンドラ

  /// @brief input voltage を表す AstNode を生成する．
  const AstInputVoltage*
  new_input_voltage(const FileRegion& attr_loc,        ///< [in] 属性のファイル上の位置
		    const StrHandler& header,	       ///< [in] ヘッダを読み込んだハンドラ
		    const InputVoltageHandler& group); ///< [in] グループ本体を読み込んだハンドラ

  /// @brief output voltage を表す AstNode を生成する．
  const AstOutputVoltage*
  new_output_voltage(const FileRegion& attr_loc,         ///< [in] 属性のファイル上の位置
		     const StrHandler& header,		 ///< [in] ヘッダを読み込んだハンドラ
		     const OutputVoltageHandler& group); ///< [in] グループ本体を読み込んだハンドラ

  /// @brief domain を表す AstNode を生成する．
  const AstDomain*
  new_domain(const FileRegion& attr_loc,  ///< [in] 属性のファイル上の位置
	     const StrHandler& header,	  ///< [in] ヘッダを読み込んだハンドラ
	     const DomainHandler& group); ///< [in] グループ本体を読み込んだハンドラ

  /// @brief timing を表す AstNode を生成する．
  const AstTiming*
  new_timing(const FileRegion& attr_loc,   ///< [in] 属性のファイル上の位置
	     const StrListHandler& header, ///< [in] ヘッダを読み込んだハンドラ
	     const TimingHandler& group);  ///< [in] グループ本体を読み込んだハンドラ

  /// @brief 使用メモリ量の一覧を出力する．
  void
  show_stats(ostream& s) const; ///< [in] 出力先のストリーム
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  AstLibrary* mLibraryNode;

  // 全てのノードのリスト
  vector<AstNode*> mNodeList;

  // 個々の要素の使用数
  SizeType mBoolNum;
  SizeType mBoolExprNum;
  SizeType mDelayModelNum;
  SizeType mDirectionNum;
  SizeType mFloatNum;
  SizeType mFloat2Num;
  SizeType mFloatExprNum;
  SizeType mFloatStrNum;
  SizeType mFloatVectNum;
  SizeType mFloatVectElemSize;
  SizeType mIntNum;
  SizeType mIntFloatNum;
  SizeType mIntFloatVectorNum;
  SizeType mIntVectNum;
  SizeType mIntVectElemSize;
  SizeType mStrNum;
  SizeType mStr2Num;
  SizeType mStr3Num;
  SizeType mStrFloatNum;
  SizeType mStrIntNum;
  SizeType mStrVectNum;
  SizeType mStrVectElemSize;
  SizeType mOprNum;
  SizeType mNotNum;
  SizeType mDomainNum;
  SizeType mSymbolExprNum;
  SizeType mStrExprNum;
  SizeType mListNum;
  SizeType mListElemSize;
  SizeType mTemplateNum;
  SizeType mLutNum;
  SizeType mInputVolNum;
  SizeType mOutputVolNum;
  SizeType mPieceWiseNum;
  SizeType mTechnologyNum;
  SizeType mTimingSenseNum;
  SizeType mTimingTypeNum;
  SizeType mVarTypeNum;
  SizeType mVarRangeNum;
  SizeType mFFNum;
  SizeType mFFBankNum;
  SizeType mLatchNum;
  SizeType mLatchBankNum;
  SizeType mPinNum;
  SizeType mAttrNum;

};

END_NAMESPACE_YM_DOTLIB

#endif // ASTMGR_H
