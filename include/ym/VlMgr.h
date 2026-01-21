#ifndef YM_VLMGR_H
#define YM_VLMGR_H

/// @file ym/VlMgr.h
/// @brief VlMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/pt/PtP.h"
#include "ym/vl/VlFwd.h"
#include "ym/ClibCellLibrary.h"
#include "ym/PathList.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtMgr;
class ElbMgr;

//////////////////////////////////////////////////////////////////////
/// @class VlMgr VlMgr.h "ym/VlMgr.h"
/// @brief Verilog-HDL の elaboration 結果を保持するクラス
//////////////////////////////////////////////////////////////////////
class VlMgr
{
public:

  /// @brief コンストラクタ
  VlMgr();

  /// @brief デストラクタ
  ~VlMgr();


public:

  /// @brief 内容をクリアする．
  void
  clear();


public:
  //////////////////////////////////////////////////////////////////////
  // parser 関係のメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを読み込む．
  /// @retval true 正常に終了した．
  /// @retval false エラーが起こった．
  /// @sa PathList
  /// @sa VlLineWatcher
  bool
  read_file(
    const std::string& filename,                   ///< [in] 読み込むファイル名
    const PathList& searchpath = {},               ///< [in] サーチパス
    const std::vector<VlLineWatcher*> watcher_list ///< [in] 行番号ウォッチャーのリスト
     = {}
  );

  /// @brief 登録されているモジュールのリストを返す．
  /// @return 登録されているモジュールのリスト
  /// @sa PtModule
  const std::vector<const PtModule*>&
  pt_module_list() const;

  /// @brief 登録されている UDP のリストを返す．
  /// @return 登録されている UDP のリスト
  /// @sa PtUdp
  const std::vector<const PtUdp*>&
  pt_udp_list() const;

  /// @brief attribute instance のリストを表す構文木要素を返す．
  /// @sa PtAttrInst, PtBase
  std::vector<const PtAttrInst*>
  pt_attr_list(
    const PtBase* pt_obj ///< [in] 対象の構文木要素
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // elaboration 関係のメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief エラボレーションを行う．
  /// @return エラー数を返す．
  /// @sa ClibCellLibrary
  SizeType
  elaborate(
    const ClibCellLibrary& cell_library ///< [in] セルライブラリ
    = ClibCellLibrary()
  );

  /// @brief UDP 定義のリストを返す．
  /// @sa VlUdpDefn
  const std::vector<const VlUdpDefn*>&
  udp_list() const;

  /// @brief 名前から UDP 定義を取出す．
  /// @return name という名の UDP を返す．
  /// @return なければ nullptr を返す．
  /// @sa VlUdpDefn
  const VlUdpDefn*
  find_udp(
    const char* name ///< [in] 名前
  ) const;

  /// @brief 名前から UDP 定義を取出す．
  /// @return name という名の UDP を返す．
  /// @return なければ nullptr を返す．
  /// @sa VlUdpDefn
  const VlUdpDefn*
  find_udp(
    const std::string& name ///< [in] 名前
  ) const;

  /// @brief topmodule のリストを返す．
  /// @sa VlModule
  const std::vector<const VlModule*>&
  topmodule_list() const;

  /// @brief 名前から UserSystf を取出す．
  /// @return name という名のユーザー定義関数を返す．
  /// @return なければ nullptr を返す．
  /// @sa VlUserSystf
  const VlUserSystf*
  find_user_systf(
    const char* name ///< [in] 名前
  ) const;

  /// @brief 名前から UserSystf を取出す．
  /// @return name という名のユーザー定義関数を返す．
  /// @return なければ nullptr を返す．
  /// @sa VlUserSystf
  const VlUserSystf*
  find_user_systf(
    const std::string& name ///< [in] 名前
  ) const;

  /// @brief スコープに属する internal scope のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlScope
  std::vector<const VlScope*>
  find_internalscope_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープとタグから宣言要素を取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlDecl, VlScope
  ///
  /// parent のスコープ内の tag というタグを持つ要素のリストを返す．
  std::vector<const VlDecl*>
  find_decl_list(
    const VlScope* parent, ///< [in] 検索対象のスコープ
    int tag                ///< [in] タグ
  ) const;

  /// @brief スコープとタグから宣言要素の配列を取り出す．
  /// @retrun 結果のリストを返す．
  /// @sa VlDeclArray, VlScope
  ///
  /// parent というスコープ内の tag というタグを持つ要素のリストを返す．
  std::vector<const VlDeclArray*>
  find_declarray_list(
    const VlScope* parent, ///< [in] 検索対象のスコープ
    int tag            	   ///< [in] タグ
  ) const;

  /// @brief スコープに属する defparam のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlDefParam, VlScope
  std::vector<const VlDefParam*>
  find_defparam_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属する param assign のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlParamAssign, VlScope
  std::vector<const VlParamAssign*>
  find_paramassign_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属する module のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlModule, VlScope
  std::vector<const VlModule*>
  find_module_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属する module arrayのリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlModuleArray, VlScope
  std::vector<const VlModuleArray*>
  find_modulearray_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属する primitive のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlPrimitive, VlScope
  std::vector<const VlPrimitive*>
  find_primitive_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属する primitive array のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlPrimArray, VlScope
  std::vector<const VlPrimArray*>
  find_primarray_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属するタスクのリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlTaskFunc, VlScope
  std::vector<const VlTaskFunc*>
  find_task_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属する関数のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlTaskFunc, VlScope
  std::vector<const VlTaskFunc*>
  find_function_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属する continuous assignment のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlContAssign, VlScope
  std::vector<const VlContAssign*>
  find_contassign_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief スコープに属する process のリストを取り出す．
  /// @return 結果のリストを返す．
  /// @sa VlProcess, VlScope
  std::vector<const VlProcess*>
  find_process_list(
    const VlScope* parent ///< [in] 検索対象のスコープ
  ) const;

  /// @brief 属性リストを得る．
  /// @sa VlAttribute, VlObj
  std::vector<const VlAttribute*>
  find_attr(
    const VlObj* obj ///< [in] 対象のオブジェクト
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Pt オブジェクトを管理するクラス
  std::unique_ptr<PtMgr> mPtMgr;

  // Elb オブジェクトを管理するクラス
  std::unique_ptr<ElbMgr> mElbMgr;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VLMGR_H
