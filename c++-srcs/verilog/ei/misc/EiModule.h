#ifndef EIMODULE_H
#define EIMODULE_H

/// @file EiModule.h
/// @brief EiModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiModuleBase.h"
#include "EiModuleHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiModule EiModule.h "EiModule.h"
/// @brief 単独要素の EiModule
//////////////////////////////////////////////////////////////////////
class EiModule :
  public EiModuleBase
{
public:

  /// @brief コンストラクタ
  EiModule(
    const VlScope* parent,       ///< [in] 親のスコープ
    const AstModule& ast_module, ///< [in] モジュールテンプレート
    const AstItem& ast_head,     ///< [in] パース木のヘッダ定義
    const AstInst& ast_inst      ///< [in] パース木のインスタンス定義
  ) : mHead{parent, ast_module, ast_head, ast_inst}
  {
    auto port_num = ast_module.port_list().size();
    auto io_num = ast_module.iohead_list().size();
    init(port_num, io_num);
  }

  /// @brief デストラクタ
  ~EiModule() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlNamedObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  std::string
  name() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlModule の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列要素の時 true を返す．
  bool
  is_array() const override;

  /// @brief インデックスの値を返す．
  int
  index() const override;

  /// @brief 親の配列を返す．
  const VlModuleArray*
  module_array() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // EiModule の派生クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダ情報を返す．
  const EiModuleHead&
  head() const override;

  /// @brief ヘッダ情報を返す．
  EiModuleHead&
  head() override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ情報
  EiModuleHead mHead;

};

END_NAMESPACE_YM_VERILOG

#endif // EIMODULE_H
