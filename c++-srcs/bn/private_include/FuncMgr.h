#ifndef FUNCMGR_H
#define FUNCMGR_H

/// @file FuncMgr.h
/// @brief FuncMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/bn.h"
#include "ym/logic.h"
#include "ym/BddMgr.h"
#include "FuncImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
/// @class FuncMgr FuncMgr.h "FuncMgr.h"
/// @brief 関数情報(FuncImpl)を管理するクラス
//////////////////////////////////////////////////////////////////////
class FuncMgr
{
  /// @brief FuncImpl をキーとして関数番号を格納するハッシュ表
  using FuncMap = std::unordered_map<const FuncImpl*, SizeType,
				     FuncHash, FuncEq>;

public:

  /// @brief コンストラクタ
  FuncMgr() = default;

  /// @brief コピーコンストラクタもどき
  FuncMgr(
    const FuncMgr& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~FuncMgr() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief プリミティブ型を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_primitive(
    SizeType input_num,     ///< [in] 入力数
    PrimType primitive_type ///< [in] プリミティブの種類
  );

  /// @brief カバー型を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_cover(
    const SopCover& input_cover, ///< [in] 入力カバー
    bool output_inv              ///< [in] 出力の反転構造
  );

  /// @brief 論理式型を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_expr(
    const Expr& expr ///< [in] 論理式
  );

  /// @brief 真理値表型を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_tvfunc(
    const TvFunc& tvfunc ///< [in] 真理値表
  );

  /// @brief BDD型を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_bdd(
    const Bdd& bdd ///< [in] BDD
  );

  /// @brief 登録されている関数情報の数を返す．
  SizeType
  func_num() const
  {
    return mFuncArray.size();
  }

  /// @brief 関数情報を取り出す．
  const FuncImpl&
  func(
    SizeType func_id ///< [in] 関数番号 ( 0 <= func_id < func_num() );
  ) const
  {
    if ( func_id >= func_num() ) {
      throw std::out_of_range{"func_id is out of range"};
    }
    return *mFuncArray[func_id];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数情報を登録する．
  /// @return 関数番号を返す．
  SizeType
  reg_func(
    FuncImpl* func
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDDマネージャ
  BddMgr mBddMgr;

  // FuncImpl の配列
  std::vector<std::unique_ptr<FuncImpl>> mFuncArray;

  // FuncImpl* をキーとして関数番号を格納する辞書
  FuncMap mFuncMap;

};

END_NAMESPACE_YM_BN

#endif // FUNCMGR_H
