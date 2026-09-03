#ifndef YM_VL_ASTATTRINST_H
#define YM_VL_ASTATTRINST_H

/// @file ym/vl/AstAttrInst.h
/// @brief AstAttrInst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"


BEGIN_NAMESPACE_YM_VERILOG

class PtAttrInst;

//////////////////////////////////////////////////////////////////////
/// @class AstAttrInst AstAttrInst.h "ym/vl/AstAttrInst.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief attribute_instance を表すクラス
///
/// 実体は AstAttrSpec のリスト
/// ただしファイル領域としては先頭と末尾の '(*' と '*)' を含んだ領域となる．
//////////////////////////////////////////////////////////////////////
class AstAttrInst :
  public AstBaseWithPtr<const PtAttrInst>
{
public:

  /// @brief コンストラクタ
  explicit
  AstAttrInst(
    const PtAttrInst* ptr = nullptr ///< [in] 実体のポインタ
  ) : AstBaseWithPtr<const PtAttrInst>(ptr)
  {
  }

  /// @brief デストラクタ
  ~AstAttrInst() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素のリストの取得
  AstAttrSpecList
  attrspec_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 適切な値を持っている時 true を返す．
  bool
  is_valid() const override;

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 比較用のユニークなキーを返す．
  PtrIntType
  key() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstList<> の要素のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を返す．
  AstAttrInst
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief json_obj() の下請け関数
  void
  json_sub(
    JsonValue& jobj ///< [in] 対象の JSON オブジェクト
  ) const override;

};

END_NAMESPACE_YM_VERILOG

#endif // YM_VL_ASTATTRINST_H
