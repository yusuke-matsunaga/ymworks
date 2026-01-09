#ifndef SATLOGGER_H
#define SATLOGGER_H

/// @file SatLogger.h
/// @brief SatLogger のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/sat.h"
#include "ym/json.h"
#include "ym/SatBool3.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SatLogger SatLogger.h "SatLogger.h"
/// @brief SatSolver の動作を記録するクラス
//////////////////////////////////////////////////////////////////////
class SatLogger
{
public:

  /// @brief 新しいインスタンスを生成するクラスメソッド
  static
  std::unique_ptr<SatLogger>
  new_impl(
    const JsonValue& js_obj ///< [in] 初期化パラメータ
  );

  /// @brief コンストラクタ
  SatLogger() = default;

  /// @brief デストラクタ
  virtual
  ~SatLogger() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常な状態の時 true を返す．
  virtual
  bool
  valid();

  /// @brief 変数を追加する．
  void
  new_variable(
    SatLiteral lit, ///< [in] 変数を表すリテラル
    bool decision   ///< [in] 決定変数の時 true にするフラグ
  );

  /// @brief 節を追加する．
  void
  add_clause(
    const std::vector<SatLiteral>& lits ///< [in] リテラルのベクタ
  );

  /// @brief assumption 付きの SAT 問題を解く．
  void
  solve(
    const std::vector<SatLiteral>& assumptions ///< [in] あらかじめ仮定する変数の値割り当てリスト
  );

  /// @brief solve() の結果を出力する．
  void
  solve_result(
    SatBool3 res ///< [in] 結果
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行文の出力を行う．
  virtual
  void
  put_line(
    const std::string& str ///< [in] 出力する文字列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラルを出力する．
  void
  put_lit(
    std::ostream& s,
    SatLiteral lit
  );

};


//////////////////////////////////////////////////////////////////////
/// @class SatLogger_file SatLogger.h "SatLogger.h"
/// @brief SatSolver の動作をファイルに記録するクラス
//////////////////////////////////////////////////////////////////////
class SatLogger_file :
  public SatLogger
{
public:

  /// @brief コンストラクタ
  SatLogger_file(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief デストラクタ
  ~SatLogger_file() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常な状態の時 true を返す．
  bool
  valid() override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行文の出力を行う．
  void
  put_line(
    const std::string& str ///< [in] 出力する文字列
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  std::ofstream mS;

};


//////////////////////////////////////////////////////////////////////
/// @class SatLogger_cout SatLogger.h "SatLogger.h"
/// @brief SatSolver の動作を標準出力に記録するクラス
//////////////////////////////////////////////////////////////////////
class SatLogger_cout :
  public SatLogger
{
public:

  /// @brief コンストラクタ
  SatLogger_cout() = default;

  /// @brief デストラクタ
  ~SatLogger_cout() = default;



protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行文の出力を行う．
  void
  put_line(
    const std::string& str ///< [in] 出力する文字列
  ) override;

};


//////////////////////////////////////////////////////////////////////
/// @class SatLogger_cerr SatLogger.h "SatLogger.h"
/// @brief SatSolver の動作を標準出力に記録するクラス
//////////////////////////////////////////////////////////////////////
class SatLogger_cerr :
  public SatLogger
{
public:

  /// @brief コンストラクタ
  SatLogger_cerr() = default;

  /// @brief デストラクタ
  ~SatLogger_cerr() = default;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一行文の出力を行う．
  void
  put_line(
    const std::string& str ///< [in] 出力する文字列
  ) override;

};

END_NAMESPACE_YM_SAT

#endif // SATLOGGER_H
