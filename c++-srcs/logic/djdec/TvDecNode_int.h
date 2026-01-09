#ifndef TVDECNODE_INT_H
#define TVDECNODE_INT_H

/// @file TvDecNode_int.h
/// @brief TvDecNode_int のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TvDecNode.h"


BEGIN_NAMESPACE_YM_DJDEC

//////////////////////////////////////////////////////////////////////
/// @class TvDecNode_Base TvDecNode_int.h "TvDecNode_int.h"
/// @brief TvDecNode を実装する基底クラス
//////////////////////////////////////////////////////////////////////
class TvDecNode_Base :
  public TvDecNode
{
public:

  /// @brief コンストラクタ
  TvDecNode_Base(
    SizeType id,                         ///< [in] ID番号
    const std::vector<SizeType>& support ///< [in] サポートリスト
  );

  /// @brief デストラクタ
  ~TvDecNode_Base() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const override;

  /// @brief LITタイプの時に true を返す．
  bool
  is_lit() const override;

  /// @brief ORタイプの時に true を返す．
  bool
  is_or() const override;

  /// @brief XORタイプの時に true を返す．
  bool
  is_xor() const override;

  /// @brief CPLXタイプの時に true を返す．
  bool
  is_cplx() const override;

  /// @brief 先頭の変数を返す．
  SizeType
  top_var() const override;

  /// @brief サポートを返す．
  const std::vector<SizeType>&
  support() const override;

  /// @brief subfunction の数を得る．
  SizeType
  child_num() const override;

  /// @brief subfunction を表す枝を得る．
  TvDecEdge
  child(
    SizeType pos ///< [in] 位置 ( 0 <= pos < child_num() )
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief print の共通部分
  void
  print_base(
    std::ostream& s,        ///< [in] 出力ストリーム
    const std::string& type ///< [in] 種類を表す文字列
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

  // サポート
  std::vector<SizeType> mSupport;

};


//////////////////////////////////////////////////////////////////////
/// @class TvDecNode_Lit TvDecNode_int.h "TvDecNode_int.h"
/// @brief LITタイプの TvDecNode
//////////////////////////////////////////////////////////////////////
class TvDecNode_Lit :
  public TvDecNode_Base
{
public:

  /// @brief コンストラクタ
  TvDecNode_Lit(
    SizeType id, ///< [in] ID番号
    SizeType var ///< [in] 変数
  ) : TvDecNode_Base(id, {var})
  {
  }

  /// @brief デストラクタ
  ~TvDecNode_Lit() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief LITタイプの時に true を返す．
  bool
  is_lit() const override;

  /// @brief グローバル関数を返す．
  TvFunc
  global_func(
    SizeType input_num ///< [in] 入力数
  ) const override;

  /// @brief ローカル関数を求める．
  TvFunc
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class TvDecNode_Mid TvDecNode_int.h "TvDecNode_int.h"
/// @brief 子ノードを持つ共通の親クラス
//////////////////////////////////////////////////////////////////////
class TvDecNode_Mid :
  public TvDecNode_Base
{
public:

  /// @brief コンストラクタ
  TvDecNode_Mid(
    SizeType id,                             ///< [in] ID番号
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  );

  /// @brief デストラクタ
  ~TvDecNode_Mid() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief subfunction の数を得る．
  SizeType
  child_num() const override;

  /// @brief subfunction を表す枝を得る．
  TvDecEdge
  child(
    SizeType pos ///< [in] 位置 ( 0 <= pos < child_num() )
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief pint() の下請け処理を行う．
  void
  print_sub(
    std::ostream& s,        ///< [in] 出力ストリーム
    const std::string& type ///< [in] 種類を表す文字列
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子ノードの枝のリスト
  std::vector<TvDecEdge> mChildList;

};


//////////////////////////////////////////////////////////////////////
/// @class TvDecNode_Or TvDecNode_int.h "TvDecNode_int.h"
/// @brief ORノードを表すクラス
//////////////////////////////////////////////////////////////////////
class TvDecNode_Or :
  public TvDecNode_Mid
{
public:

  /// @brief コンストラクタ
  TvDecNode_Or(
    SizeType id,                             ///< [in] ID番号
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : TvDecNode_Mid(id, child_list)
  {
  }

  /// @brief デストラクタ
  ~TvDecNode_Or() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ORタイプの時に true を返す．
  bool
  is_or() const override;

  /// @brief グローバル関数を返す．
  TvFunc
  global_func(
    SizeType input_num ///< [in] 入力数
  ) const override;

  /// @brief ローカル関数を求める．
  TvFunc
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class TvDecNode_Xor TvDecNode_int.h "TvDecNode_int.h"
/// @brief XORノードを表すクラス
//////////////////////////////////////////////////////////////////////
class TvDecNode_Xor :
  public TvDecNode_Mid
{
public:

  /// @brief コンストラクタ
  TvDecNode_Xor(
    SizeType id,                             ///< [in] ID番号
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : TvDecNode_Mid(id, child_list)
  {
  }

  /// @brief デストラクタ
  ~TvDecNode_Xor() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief XORタイプの時に true を返す．
  bool
  is_xor() const override;

  /// @brief グローバル関数を返す．
  TvFunc
  global_func(
    SizeType input_num ///< [in] 入力数
  ) const override;

  /// @brief ローカル関数を求める．
  TvFunc
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class TvDecNode_Cplx TvDecNode_int.h "TvDecNode_int.h"
/// @brief CPLXノードを表すクラス
//////////////////////////////////////////////////////////////////////
class TvDecNode_Cplx :
  public TvDecNode_Mid
{
public:

  /// @brief コンストラクタ
  TvDecNode_Cplx(
    SizeType id,                             ///< [in] ID番号
    const TvFunc& local_func,                ///< [in] ローカル関数
    const std::vector<TvDecEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : TvDecNode_Mid(id, child_list),
      mLocalFunc{local_func}
  {
  }

  /// @brief デストラクタ
  ~TvDecNode_Cplx() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CPLXタイプの時に true を返す．
  bool
  is_cplx() const override;

  /// @brief グローバル関数を返す．
  TvFunc
  global_func(
    SizeType input_num ///< [in] 入力数
  ) const override;

  /// @brief ローカル関数を求める．
  TvFunc
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ローカル関数
  TvFunc mLocalFunc;

};

END_NAMESPACE_YM_DJDEC

#endif // TVDECNODE_INT_H
