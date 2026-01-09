#ifndef CGSIGREP_H
#define CGSIGREP_H

/// @file CgSigRep.h
/// @brief CgSigRep のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"
#include "ym/ClibIOMap.h"
#include "ym/Expr.h"
#include "ym/NpnMap.h"
#include "ym/ClibSeqAttr.h"
#include "CgPolInfo.h"


BEGIN_NAMESPACE_YM_CLIB

struct CgPinGroup;
struct CgSymInfo;

//////////////////////////////////////////////////////////////////////
/// @class CgSigRep CgSigRep.h "CgSigRep.h"
/// @brief CgSignature の中身を表すクラス
//////////////////////////////////////////////////////////////////////
class CgSigRep
{
public:

  /// @brief コンストラクタ(1出力の組み合わせ論理セル)
  CgSigRep(
    SizeType ni,                         ///< [in] 入力数
    const Expr& expr                     ///< [in] 論理式
  );

  /// @brief コンストラクタ(汎用の組み合わせ論理セル)
  CgSigRep(
    SizeType ni,                              ///< [in] 入力数
    SizeType no,                              ///< [in] 出力数
    SizeType nb,                              ///< [in] 入出力数
    const std::vector<TvFunc>& func_list,     ///< [in] 対象の論理関数のリスト
    const std::vector<TvFunc>& tristate_list  ///< [in] tristate条件のリスト
  );

  /// @brief コンストラクタ(順序セル)
  CgSigRep(
    ClibCellType cell_type,                   ///< [in] セルの種類
    ClibSeqAttr seq_attr,                     ///< [in] 順序セルの属性
    SizeType ni,                              ///< [in] 入力数
    SizeType no,                              ///< [in] 出力数
    SizeType nb,                              ///< [in] 入出力数
    const std::vector<TvFunc>& func_list,     ///< [in] 対象の論理関数のリスト
    const std::vector<TvFunc>& tristate_list, ///< [in] tristate条件のリスト
    const Expr& expr = Expr::invalid()        ///< [in] 論理式
  );

  /// @brief デストラクタ
  ~CgSigRep() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief シグネチャ文字列を返す．
  std::string
  str() const;

  /// @brief セルの種類を返す．
  ClibCellType
  cell_type() const;

  /// @brief 順序セルの属性を返す．
  ClibSeqAttr
  seq_attr() const;

  /// @brief 単一の論理式を持つ場合，その式を返す．
  Expr
  expr() const;

  /// @brief 変換を施した後のシグネチャを返す．
  std::unique_ptr<const CgSigRep>
  xform(
    const ClibIOMap& iomap ///< [in] 変換マップ
  ) const;

  /// @brief 代表シグネチャに対する変換を求める．
  ClibIOMap
  rep_map() const;

  /// @brief 同位体変換のリストを求める．
  std::vector<ClibIOMap>
  idmap_list() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正規形への変換を求める．
  std::vector<ClibIOMap>
  gen_cannonical_map() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Walsh_0 を用いて出力のグループ分けを行う．
  void
  w0_refine(
    const std::vector<SizeType>& pos_list, ///< [in] 出力番号のリスト
    std::vector<CgPinGroup>& og_list,      ///< [out] 出力のグループのリスト
    std::vector<CgPolInfo>& opol_list      ///< [out] 出力の反転属性のリスト
  ) const;

  /// @brief 対称グループを作る．
  std::vector<SizeType>
  gen_symgroup(
    std::vector<CgSymInfo>& syminfo_list, ///< [out] 対称グループのリスト
    std::vector<bool>& syminv_list        ///< [out] 反転属性のリスト
  ) const;

  /// @brief Walsh_1_sum を用いて入力グループの細分化を行う．
  void
  w1sum_refine(
    const std::vector<SizeType>& src_list,
    const std::vector<CgPolInfo>& opol_list,
    std::vector<CgPinGroup>& ig_list,
    std::vector<CgPolInfo>& ipol_list
  ) const;

  /// @brief 入力の対称性を調べる．
  bool
  check_sym(
    SizeType i1,
    SizeType i2,
    bool inv
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルの種類
  ClibCellType mCellType{ClibCellType::Logic};

  // 順序セルの属性
  ClibSeqAttr mSeqAttr;

  // 入力数
  SizeType mNi;

  // 出力数
  SizeType mNo;

  // 入出力数
  SizeType mNb;

  // 論理関数のリスト
  std::vector<TvFunc> mFuncList;

  // tristate 条件のリスト
  std::vector<TvFunc> mTristateList;

  // 単純な論理セルの場合の論理式
  Expr mExpr;

};

END_NAMESPACE_YM_CLIB

#endif // CGSIGREP_H
