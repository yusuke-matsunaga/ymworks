#ifndef ATTRGEN_H
#define ATTRGEN_H

/// @file AttrGen.h
/// @brief AttrGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "parser/PtAttrInfo.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AttrGen AttrGen.h "AttrGen.h"
/// @brief attribute instance を生成するクラス
///
/// attribute instance は構文木の要素に対して設定されており，
/// 例えばモジュールを複数インスタンス化した時には当然複数回
/// この関数が呼ばれることになる．
/// 通常のエラボレーションと異なり attribute instance は毎回同じなので
/// おなじオブジェクトを使い回す．
///
/// 実際には attribute instance のリストであり，
/// さらに attribute instance 自体が attribute spec のリストなので
/// ややこしい
//////////////////////////////////////////////////////////////////////
class AttrGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  AttrGen(
    Elaborator& elab, ///< [in] 生成器
    ElbMgr& elb_mgr   ///, [in] Elbオブジェクトを管理するクラス
  );

  /// @brief デストラクタ
  ~AttrGen();


public:
  //////////////////////////////////////////////////////////////////////
  // AttrGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AstAttr から VlAttribute を生成する．
  ///
  /// 結果は mHash に登録される．
  void
  instantiate_attribute(
    const PtAttrInfo& attr_info ///< [in] 属性リストの情報
  );

  /// @brief 構文木要素に対応する属性リストを返す．
  const std::vector<const VlAttribute*>&
  attribute_list(
    const AstBase* ast_obj ///< [in] 元となる構文木要素
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  std::unordered_map<const AstBase*, std::vector<const VlAttribute*>> mHash;

  // 空のリスト
  std::vector<const VlAttribute*> mEmptyList{};

};

END_NAMESPACE_YM_VERILOG

#endif // ATTRGEN_H
