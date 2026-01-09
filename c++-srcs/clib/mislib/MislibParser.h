#ifndef MISLIBPARSER_H
#define MISLIBPARSER_H

/// @file MislibParser.h
/// @brief MislibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "mislib_nsdef.h"
#include "ym/FileRegion.h"
#include "ym/ShString.h"
#include "MislibToken.h"


BEGIN_NAMESPACE_YM_MISLIB

class MislibScanner;

//////////////////////////////////////////////////////////////////////
/// @class MislibParser MislibParser.h "MislibParser.h"
/// @brief mislib(genlib)のパーサークラス
//////////////////////////////////////////////////////////////////////
class MislibParser
{
public:

  /// @brief コンストラクタ
  MislibParser() = default;

  /// @brief デストラクタ
  ~MislibParser() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パーズする．
  /// @return ゲートの AST のリストを返す．
  ///
  /// エラーが起きたら std::invalid_argument 例外を創出する．
  std::vector<MislibGatePtr>
  parse(
    const std::string& filename ///< [in] ファイル名
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み込んだゲートリストが正しいかチェックする．
  void
  check_gate_list(
    const std::vector<MislibGatePtr>& gate_list ///< [in] ゲートのASTを格納するリスト
  );

  /// @brief 文字列を読み込む．
  /// @return 文字列を表す AST のノードを返す．
  ///
  /// エラーが起きたら std::invalid_argument 例外を創出する．
  MislibStrPtr
  read_str();

  /// @brief 数値を読み込む．
  /// @return 数値を表す AST のノードを返す．
  ///
  /// エラーが起きたら std::invalid_argument 例外を創出する．
  MislibNumPtr
  read_num();

  /// @brief 式を読み込む．
  /// @return 式を表す AST のノードを返す．
  ///
  /// エラーが起きたら std::invalid_argument 例外を創出する．
  MislibExprPtr
  read_expr(
    MislibToken::Type end_type ///< [in] 終端の種類
  );

  /// @brief 積項を読み込む．
  /// @return 式を表す AST のノードを返す．
  ///
  /// エラーが起きたら std::invalid_argument 例外を創出する．
  MislibExprPtr
  read_product();

  /// @brief リテラルを読み込む．
  /// @return 式を表す AST のノードを返す．
  ///
  /// エラーが起きたら std::invalid_argument 例外を創出する．
  MislibExprPtr
  read_literal();

  /// @brief ピンリスト記述を読み込む．
  /// @return ピンリストを返す．
  ///
  /// - エラーが起きたら std::invalid_argument 例外を創出する．
  /// - ピン名の代わりに * の場合があるので注意
  std::vector<MislibPinPtr>
  read_pin_list();


private:
  //////////////////////////////////////////////////////////////////////
  // 低レベル関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次のトークンを盗み見る．
  MislibToken
  peek();

  /// @brief peek() で読んだトークンを捨てる．
  void
  skip_token();

  /// @brief 字句解析を行う．
  /// @return トークンを返す．
  MislibToken
  scan();

  /// @brief 次のトークンを予測する．
  ///
  /// 予測どおりのトークンでなければ std::invalid_argument 例外を創出する．
  void
  expect_token(
    MislibToken::Type exp_type ///< [in] 予測されるトークンの種類
  );

  /// @brief GATE ノードを生成する．
  MislibGatePtr
  new_gate(
    const FileRegion& loc,
    MislibStrPtr&& name,
    MislibNumPtr&& area,
    MislibStrPtr&& oname,
    MislibExprPtr&& expr,
    std::vector<MislibPinPtr>&& ipin_list
  );

  /// @brief PIN ノードを生成する．
  MislibPinPtr
  new_pin(
    const FileRegion& loc,
    MislibStrPtr&& name,
    MislibPhasePtr&& phase,
    MislibNumPtr&& input_load,
    MislibNumPtr&& max_load,
    MislibNumPtr&& rise_block_delay,
    MislibNumPtr&& rise_fanout_delay,
    MislibNumPtr&& fall_block_delay,
    MislibNumPtr&& fall_fanout_delay
  );

  /// @brief NOT ノードを生成する．
  MislibExprPtr
  new_not(
    const FileRegion& loc,
    MislibExprPtr&& opr1
  );

  /// @brief AND ノードを生成する．
  MislibExprPtr
  new_and(
    const FileRegion& loc,
    MislibExprPtr&& opr1,
    MislibExprPtr&& opr2
  );

  /// @brief OR ノードを生成する．
  MislibExprPtr
  new_or(
    const FileRegion& loc,
    MislibExprPtr&& opr1,
    MislibExprPtr&& opr2
  );

  /// @brief XOR ノードを生成する．
  MislibExprPtr
  new_xor(
    const FileRegion& loc,
    MislibExprPtr&& opr1,
    MislibExprPtr&& opr2
  );

  /// @brief 定数0ノードを生成する．
  MislibExprPtr
  new_const0(
    const FileRegion& loc
  );

  /// @brief 定数1ノードを生成する．
  MislibExprPtr
  new_const1(
    const FileRegion& loc
  );

  /// @brief 変数ノードを生成する．
  MislibExprPtr
  new_varname(
    const FileRegion& loc,
    const char* str
  );

  /// @brief NONINV ノードを生成する．
  MislibPhasePtr
  new_noninv(
    const FileRegion& loc
  );

  /// @brief INV ノードを生成する．
  MislibPhasePtr
  new_inv(
    const FileRegion& loc
  );

  /// @brief UNKNOWN ノードを生成する．
  MislibPhasePtr
  new_unknown(
    const FileRegion& loc
  );

  /// @brief 直前のトークンが文字列の場合に文字列ノードを返す．
  MislibStrPtr
  new_str(
    const FileRegion& loc
  );

  /// @brief 直前のトークンが数字の場合に数字ノードを返す．
  MislibNumPtr
  new_num(
    const FileRegion& loc
  );

  /// @brief エラーメッセージを出力する．
  void
  syntax_error(
    const FileRegion& loc,
    const std::string& msg
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  MislibScanner* mScanner{nullptr};

  // 戻したトークン
  MislibToken mUngetToken;

  // 生成したノードのリスト
  std::vector<std::unique_ptr<MislibNode>> mNodeList;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBPARSER_H
