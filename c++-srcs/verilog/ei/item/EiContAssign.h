#ifndef EICONTASSIGN_H
#define EICONTASSIGN_H

/// @file EiContAssign.h
/// @brief EiContAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/VlContAssign.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiCaBase EiContAssign.h "ei/EiContAssign.h"
/// @brief 継続的代入文の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class EiCaBase :
  public VlContAssign
{
protected:

  /// @brief コンストラクタ
  EiCaBase(
    const FileRegion& loc, ///< [in] ファイル位置
    const VlExpr* lhs,     ///< [in] 左辺式
    const VlExpr* rhs      ///< [in] 右辺式
  ) : mFileRegion{loc},
      mLhs{lhs},
      mRhs{rhs}
  {
  }

  /// @brief デストラクタ
  ~EiCaBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置の取得
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式を返す．
  const VlExpr*
  lhs() const override;

  /// @brief 右辺を返す．
  const VlExpr*
  rhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 左辺式
  const VlExpr* mLhs;

  // 右辺式
  const VlExpr* mRhs;

};


//////////////////////////////////////////////////////////////////////
/// @class EiContAssign EiContAssign.h "ei/EiContAssign.h"
/// @brief ElbCaHead を持つクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class EiContAssign :
  public EiCaBase
{
public:

  /// @brief コンストラクタ
  EiContAssign(
    ElbCaHead* head,       ///< [in] ヘッダ
    const FileRegion& loc, ///< [in] ファイル位置
    const VlExpr* lhs,     ///< [in] 左辺式
    const VlExpr* rhs      ///< [in] 右辺式
  ) : EiCaBase(loc, lhs, rhs),
      mHead{head}
  {
  }

  /// @brief デストラクタ
  ~EiContAssign() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  module() const override;

  /// @brief 0の強さを返す．
  VpiStrength
  drive0() const override;

  /// @brief 1の強さを返す．
  VpiStrength
  drive1() const override;

  /// @brief 遅延を表す式を返す．
  const VlDelay*
  delay() const override;

  /// @brief ネット宣言中の assignment の時に true を返す．
  bool
  has_net_decl_assign() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ヘッダ
  const ElbCaHead* mHead;

};


//////////////////////////////////////////////////////////////////////
/// @class EiCa_Net EiContAssign.h "ei/EiContAssign.h"
/// @brief net 宣言中の継続的代入文
//////////////////////////////////////////////////////////////////////
class EiCa_Net :
  public EiCaBase
{
public:

  /// @brief コンストラクタ
  EiCa_Net(
    const VlModule* module, ///< [in] 親のモジュール
    const FileRegion& loc,  ///< [in] ファイル位置
    const VlExpr* lhs,      ///< [in] 左辺式
    const VlExpr* rhs       ///< [in] 右辺式
  ) : EiCaBase(loc, lhs, rhs),
      mModule{module}
  {
  }

  /// @brief デストラクタ
  ~EiCa_Net() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlContAssign の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のモジュールを返す．
  const VlModule*
  module() const override;

  /// @brief 0の強さを返す．
  VpiStrength
  drive0() const override;

  /// @brief 1の強さを返す．
  VpiStrength
  drive1() const override;

  /// @brief 遅延を表す式を返す．
  const VlDelay*
  delay() const override;

  /// @brief ネット宣言中の assignment の時に true を返す．
  bool
  has_net_decl_assign() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のモジュール
  const VlModule* mModule;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONTASSIGN_H
