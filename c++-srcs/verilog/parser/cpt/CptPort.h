#ifndef CPTPORT_H
#define CPTPORT_H

/// @file CptPort.h
/// @brief CptPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtPort.h"
#include "parser/PtArray.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief ポートの基底クラス
//////////////////////////////////////////////////////////////////////
class CptPort :
  public PtPort
{
public:

  /// @brief コンストラクタ
  CptPort(
    const FileRegion& file_region,
    const char* ext_name
  ) : mFileRegion{file_region},
      mExtName{ext_name}
  {
  }

  /// @brief デストラクタ
  ~CptPort() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  FileRegion
  file_region() const override;

  /// @brief 外向の名前の取得
  /// @return 外向の名前(本当のポート名)
  /// @return 無い場合は nullptr を返す
  const char*
  ext_name() const override;

  /// @brief 内側のポート結線を表す式の取得
  const AstExpr*
  expr() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線式の取得
  const AstExpr*
  portref(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const override;

  /// @brief 内部のポート結線のリストの取得
  ///
  /// portef_size() <= 1 の時は nullptr を返す．
  AstExprVec
  portref_list() const override;

  /// @brief 内部のポート結線の向きの取得
  VpiDir
  portref_dir(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線の向きを設定する．
  void
  set_portref_dir(
    SizeType index,
    VpiDir dir
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 外部向きの名前
  const char* mExtName;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス (portref が1つ)
//////////////////////////////////////////////////////////////////////
class CptPort1 :
  public CptPort
{
public:

  /// @brief コンストラクタ
  CptPort1(
    const FileRegion& file_region,
    const char* ext_name,
    const AstExpr* expr
  ) : CptPort(file_region, ext_name),
      mExpr{expr}
  {
  }

  /// @brief デストラクタ
  ~CptPort1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内側のポート結線を表す式の取得
  const AstExpr*
  expr() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線の取得
  const AstExpr*
  portref(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const override;

  /// @brief 内部のポート結線のリストの取得
  ///
  /// portef_size() <= 1 の時は nullptr を返す．
  AstExprVec
  portref_list() const override;

  /// @brief 内部のポート結線の向きの取得
  VpiDir
  portref_dir(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線の向きを設定する．
  void
  set_portref_dir(
    SizeType index,
    VpiDir dir
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief index をチェックする．
  void
  _check_index(
    SizeType index
  ) const
  {
    if ( index >= 1 ) {
      throw std::out_of_range{"index is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 唯一の要素
  const AstExpr* mExpr;

  // 向き
  VpiDir mDir;

};


//////////////////////////////////////////////////////////////////////
/// @brief port を表すクラス (portref リスト付き)
//////////////////////////////////////////////////////////////////////
class CptPort2 :
  public CptPort
{
public:

  /// @brief コンストラクタ
  CptPort2(
    const FileRegion& file_region,
    const char* ext_name,
    const AstExpr* expr,
    PtExprArray&& portref_list,
    VpiDir* dir_array
  ) : CptPort(file_region, ext_name),
      mExpr{expr},
      mPortRefList{std::move(portref_list)},
      mDirArray{dir_array}
  {
  }

  /// @brief デストラクタ
  ~CptPort2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内側のポート結線を表す式の取得
  const AstExpr*
  expr() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線の取得
  const AstExpr*
  portref(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const override;

  /// @brief 内部のポート結線のリストの取得
  ///
  /// portef_size() <= 1 の時は nullptr を返す．
  AstExprVec
  portref_list() const override;

  /// @brief 内部のポート結線の向きの取得
  VpiDir
  portref_dir(
    SizeType index ///< [in] インデックス ( 0 <= index < portref_size() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のポート結線の向きを設定する．
  void
  set_portref_dir(
    SizeType index,
    VpiDir dir
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// @brief index をチェックする．
  void
  _check_index(
    SizeType index
  ) const
  {
    if ( index >= portref_size() ) {
      throw std::out_of_range{"index is out of range"};
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全体を表す式
  const AstExpr* mExpr;

  // ポート参照式のリスト
  PtExprArray mPortRefList;

  // 向きの配列
  VpiDir* mDirArray;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTMODULE_H
