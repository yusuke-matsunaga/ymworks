#ifndef ELABORATOR_H
#define ELABORATOR_H

/// @file Elaborator.h
/// @brief Elaborator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/Ast.h"
#include "ym/vl/VlFwd.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"

#include "ObjDict.h"
#include "AttrDict.h"
#include "ElbStubList.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class AstMgr;

class ElbMgr;
class ElbError;

class UdpGen;
class ModuleGen;
class DeclGen;
class ItemGen;
class StmtGen;
class ExprGen;
class ExprEval;
class AttrGen;
class DefParamStub;

//////////////////////////////////////////////////////////////////////
/// @class Elaborator Elaborator.h "Elaborator.h"
/// @brief elaboration を行うクラス
/// 基本的には 内部に状態を持たないファンクタークラス
/// 実際には elaboration 途中でオブジェクトを保持しておくハッシュ表
/// などを持つ．最終結果は引数の ElbMgr に格納される．
//////////////////////////////////////////////////////////////////////
class Elaborator
{
  friend class ElbProxy;

public:

  /// @brief コンストラクタ
  Elaborator(
    ElbMgr& elb_mgr,                    ///< [in] Elbオブジェクトを管理するクラス
    const ClibCellLibrary& cell_library ///< [in] セルライブラリ
  );

  /// @brief デストラクタ
  ~Elaborator();


public:

  /// @brief エラボレーションを行う．
  /// @return エラー数を返す．
  ///
  /// この関数は一度しか呼べない．
  int
  operator()(
    const AstMgr& ast_mgr ///< [in] パース木を管理するクラス
  );


private:
  //////////////////////////////////////////////////////////////////////
  // elaboration で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 後で処理する defparam 文を登録する．
  void
  add_defparamstub(
    const VlModule* module, ///< [in] アイテムテンプレートのヘッダ (defparam を持つ)
    const AstItem& header   ///< [in] defparam 文のテンプレート
  );

  /// @brief phase1 で行う処理を登録する．
  void
  add_phase1stub(
    ElbStub* stub ///< [in] phase1 で行う処理を表すスタブ
  )
  {
    mPhase1StubList1.push_back(stub);
  }

  /// @brief phase2 で行う処理を登録する．
  void
  add_phase2stub(
    ElbStub* stub ///< [in] phase2 で行う処理を表すスタブ
  )
  {
    mPhase2StubList.push_back(stub);
  }

  /// @brief phase3 で行う処理を登録する．
  void
  add_phase3stub(
    ElbStub* stub ///< [in] phase3 で行う処理を表すスタブ
  )
  {
    mPhase3StubList.push_back(stub);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 要素を検索する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前からモジュール定義を取り出す．
  /// @return name という名のモジュール定義
  /// @return なければ nullptr を返す．
  AstModule
  find_moduledef(
    const std::string& name ///< [in] 名前
  ) const
  {
    if ( mModuleDict.count(name) == 0 ) {
      return AstModule();
    }
    return mModuleDict.at(name);
  }

  /// @brief 関数定義を探す．
  AstItem
  find_funcdef(
    const VlModule* module, ///< [in] 親のモジュール
    const std::string& name ///< [in] 関数名
  ) const;

  /// @brief constant function を取り出す．
  /// @return parent というスコープ内の name という関数を返す．
  /// @return なければ nullptr を返す．
  const VlTaskFunc*
  find_constant_function(
    const VlScope* parent,  ///< [in] 検索対象のスコープ
    const std::string& name ///< [in] 名前
  ) const;

  /// @brief オブジェクトを探す．
  ///
  /// 見つからなかった時は nullptr を返す．
  ObjHandle*
  find_obj(
    const VlScope* parent,  ///< [in] 検索対象のスコープ
    const std::string& name ///< [in] 名前
  ) const;

  /// @brief セルの探索
  /// @return name という名のセルを返す．
  ///
  /// なければ不正値を返す．
  ClibCell
  find_cell(
    const std::string& name ///< [in] セル名
  ) const
  {
    return mCellLibrary.cell(name);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 要素を生成・登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルのスコープを生成する
  const VlScope*
  new_Toplevel();

  /// @brief constant function を登録する．
  void
  reg_constant_function(
    const VlTaskFunc* func ///< [in] 関数
  )
  {
    mCfDict.add(func);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // エラー出力関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 同名のモジュール定義がある．
  void
  error_module_redefined(
    const char* file,                  ///< [in] ファイル名
    int line,                          ///< [in] 行番号
    const AstModule& ast_module,       ///< [in] モジュール定義
    const FileRegion& prev_file_region ///< [in] 前の定義位置
  );

  /// @brief 同名の関数定義がある．
  void
  error_function_redefined(
    const char* file,                  ///< [in] ファイル名
    int line,                          ///< [in] 行番号
    const AstItem& ast_funcdef,        ///< [in] 関数定義
    const FileRegion& prev_file_region ///< [in] 前の定義位置
  );

  /// @brief 未解決の defparam 文がある．
  void
  error_defparam_unresolved(
    const char* file_name,          ///< [in] ファイル名
    int line,                       ///< [in] 行番号
    const AstDefParam& ast_defparam ///< [in] defparam 定義
  );


public:
  //////////////////////////////////////////////////////////////////////
  // メッセージ出力関数
  //////////////////////////////////////////////////////////////////////


  /// @brief エラーメッセージを出力する．
  void
  put_error(
    const ElbError& error ///< [in] エラー情報
  );

  /// @brief 警告メッセージを出力する．
  void
  put_warning(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 警告箇所
    const char* label,     ///< [in] ラベル
    const std::string& msg ///< [in] メッセージ
  );

  /// @brief 情報メッセージを出力する．
  void
  put_info(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 対象の箇所
    const char* label,     ///< [in] ラベル
    const std::string& msg ///< [in] メッセージ
  );

  /// @brief デバッグメッセージを出力する．
  void
  put_debug(
    const char* file,      ///< [in] ソースファイル名
    int line,              ///< [in] ソースファイル上の行番号
    const FileRegion& loc, ///< [in] 対象の箇所
    const char* label,     ///< [in] ラベル
    const std::string& msg ///< [in] メッセージ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // モジュールのインスタンス化に関する操作
  //////////////////////////////////////////////////////////////////////

  /// @brief インスタンス化の印を付ける．
  void
  set_instance_mark(
    const AstModule& ast_module
  );

  /// @brief インスタンス化の印を消す．
  void
  clear_instance_mark(
    const AstModule& ast_module
  );

  /// @brief インスタンス化の印を調べる．
  bool
  check_instance_mark(
    const AstModule& ast_module
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // エラボレーションを行ったことを示すフラグ
  bool mDone;

  // 生成したオブジェクトを管理するクラス
  ElbMgr& mMgr;

  // セルライブラリ
  ClibCellLibrary mCellLibrary;

  // UDP 生成用のオブジェクト
  std::unique_ptr<UdpGen> mUdpGen;

  // モジュール生成用のオブジェクト
  std::unique_ptr<ModuleGen> mModuleGen;

  // 宣言要素生成用のオブジェクト
  std::unique_ptr<DeclGen> mDeclGen;

  // 構成要素生成用のオブジェクト
  std::unique_ptr<ItemGen> mItemGen;

  // ステートメント生成用のオブジェクト
  std::unique_ptr<StmtGen> mStmtGen;

  // 式生成用のオブジェクト
  std::unique_ptr<ExprGen> mExprGen;

  // 定数式評価用のオブジェクト
  std::unique_ptr<ExprEval> mExprEval;

  // attribute instance 生成用のオブジェクト
  std::unique_ptr<AttrGen> mAttrGen;

  // インスタンス展開中のフラグ
  std::unordered_set<PtrIntType> mModuleMark;

  // 関数定義の辞書
  std::unordered_map<std::string, AstItem> mFuncDict;

  // constant function の辞書
  ObjDict mCfDict;

  // モジュールテンプレートの辞書
  std::unordered_map<std::string, AstModule> mModuleDict;

  // attribute instance の辞書
  AttrDict mAttrDict;

  // defparam 文の元のリスト
  std::vector<DefParamStub> mDefParamStubList;

  // phase1 で生成するオブジェクトを追加するリスト
  ElbStubList mPhase1StubList1;

  // phase1 で生成するオブジェクトを読み出すリスト
  ElbStubList mPhase1StubList2;

  // phase2 生成するオブジェクトを入れたリスト
  ElbStubList mPhase2StubList;

  // phase3 で link するオブジェクトを入れたリスト
  ElbStubList mPhase3StubList;


protected:
  //////////////////////////////////////////////////////////////////////
  // コンパイルオプション
  //////////////////////////////////////////////////////////////////////

  // IOに範囲がなく宣言のみに範囲を持つ場合を許すとき true
  bool mAllowEmptyIORange;

};

END_NAMESPACE_YM_VERILOG

#endif // ELABORATOR_H
