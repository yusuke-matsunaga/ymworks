#ifndef PARSER_ASTMGR_H
#define PARSER_ASTMGR_H

/// @file AstMgr.h
/// @brief AstMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"
#include "alloc/Alloc.h"
#include "parser/PtList.h"
#include "PtAttrInfo.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstMgr AstMgr.h "parser/AstMgr.h"
/// @ingroup AstMgr
/// @brief Verilog のパース木を管理するクラス
//////////////////////////////////////////////////////////////////////
class AstMgr
{
  friend class Parser;

public:

  /// @brief コンストラクタ
  AstMgr();

  /// @brief デストラクタ
  ~AstMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 登録されているモジュールのリストを返す．
  /// @return 登録されているモジュールのリスト
  const std::vector<const AstModule*>&
  module_list() const;

  /// @brief 登録されている UDP のリストを返す．
  /// @return 登録されている UDP のリスト
  const std::vector<const AstUdp*>&
  udp_list() const;

  /// @brief インスタンス記述で用いられている名前かどうか調べる．
  /// @return 用いられていたら true を返す．
  bool
  check_def_name(
    const std::string& name ///< [in] 調べる名前
  ) const;

  /// @brief 関数を探す．
  ///
  /// なければ nullptr を返す．
  const AstItem*
  find_function(
    const AstModule* module, ///< [in] 親のモジュール
    const std::string& name ///< [in] 関数名
  ) const;

  /// @brief attribute instance を取り出す．
  /// @return AstAttrInst のリスト
  ///
  /// 空の場合もある．
  AstAttrInstVec
  find_attr_list(
    const AstBase* ast_obj ///< [in] 対象の構文木の要素
  ) const;

  /// @brief 全ての属性リストのリストを返す．
  std::vector<PtAttrInfo>
  all_attr_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // データをセットする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 今までに生成したインスタンスをすべて破壊する．
  void
  clear();

  /// @brief UDP 定義を追加する．
  ///
  /// 内部で reg_ast() を呼んでいる．
  void
  reg_udp(
    const AstUdp* udp
  );

  /// @brief モジュール定義を追加する．
  ///
  /// 内部で reg_ast() を呼んでいる．
  void
  reg_module(
    const AstModule* module
  );

  /// @brief インスタンス定義名を追加する．
  void
  reg_defname(
    const std::string& name
  );

  /// @brief attribute instance を登録する．
  void
  reg_attrinst(
    const AstBase* ast_obj,
    const AstAttrInstList* ai_list,
    bool def = false
  );

  /// @brief 文字列領域を確保する．
  /// @return 文字列を返す．
  ///
  /// 同一の文字列は共有する．
  const char*
  save_string(
    const std::string& str ///, [in] 登録する文字列
  );


public:
  //////////////////////////////////////////////////////////////////////
  // メンバにアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メモリアロケーターを返す．
  Alloc&
  alloc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケーター
  std::unique_ptr<Alloc> mAlloc;

  // UDP 定義のリスト
  std::vector<const AstUdp*> mUdpList;

  // モジュール定義のリスト
  std::vector<const AstModule*> mModuleList;

  // インスタンス記述で用いられている名前
  // たぶんモジュール名か UDP名のはず
  std::unordered_set<std::string> mDefNames;

  // 文字列の辞書
  std::unordered_set<std::string> mStringPool;

  // ハッシュ関数
  struct AttrHash
  {
    SizeType
    operator()(
      const PtAttrInfo& attr_info
    ) const
    {
      auto tmp = reinterpret_cast<SizeType>(attr_info.obj());
      return (tmp * tmp) >> 16;
    }
  };

  // 等価比較関数
  struct AttrEq
  {
    bool
    operator()(
      const PtAttrInfo& attr_info1,
      const PtAttrInfo& attr_info2
    ) const
    {
      return attr_info1.obj() == attr_info2.obj();
    }
  };

  // 属性リストの辞書
  // PtAttrInfo をキーにする．
  std::unordered_set<PtAttrInfo, AttrHash, AttrEq> mAttrDict;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSER_ASTMGR_H
