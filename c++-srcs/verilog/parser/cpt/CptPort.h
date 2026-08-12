#ifndef CPTPORT_H
#define CPTPORT_H

/// @file CptPort.h
/// @brief CptPort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtPort.h"
#include "ym/FileRegion.h"
#include "parser/PtArray.h"


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
/// @brief 内部の式を持たないポート
//////////////////////////////////////////////////////////////////////
class CptPort0 :
  public CptPort
{
public:

  /// @brief コンストラクタ
  CptPort0(
    const FileRegion& file_region,
    const char* ext_name
  ) : CptPort(file_region, ext_name)
  {
  }

  /// @brief デストラクタ
  ~CptPort0() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内側のポート結線を表す式の取得
  const AstExpr*
  expr() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  const AstExpr*
  portref_elem(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;

  /// @brief 内部ポート結線の方向の取得
  VpiDir
  portref_dir(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  void
  set_portref_dir(
    SizeType pos, ///< [in] 位置番号 ( 0 <= pos < portref_num() )
    VpiDir dir    ///< [in] 方向
  ) override;

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
    const AstExpr* portref,
    const char* ext_name
  ) : CptPort(file_region, ext_name),
      mPortRef{portref}
  {
  }

  /// @brief デストラクタ
  ~CptPort1() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内側のポート結線を表す式の取得
  const AstExpr*
  expr() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  const AstExpr*
  portref_elem(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;

  ///@brief 内部ポート結線の方向の取得
  VpiDir
  portref_dir(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  void
  set_portref_dir(
    SizeType pos, ///< [in] 位置番号 ( 0 <= pos < portref_num() )
    VpiDir dir    ///< [in] 方向
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内部向きの接続を表す式
  const AstExpr* mPortRef;

  // 方向
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
    const AstExpr* expr,
    PtExprArray&& portref_array,
    const char* ext_name,
    void* q // mDirArray 用のメモリ領域
  ) : CptPort(file_region, ext_name),
      mExpr{expr},
      mPortRefArray{std::move(portref_array)},
      mDirArray{new (q) VpiDir[mPortRefArray.size()]}
  {
  }

  /// @brief デストラクタ
  ~CptPort2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtPort の継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内側のポート結線を表す式の取得
  const AstExpr*
  expr() const override;

  /// @brief 内部のポート結線リストのサイズの取得
  SizeType
  portref_size() const override;

  /// @brief 内部のポート結線リストの取得
  const AstExpr*
  portref_elem(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;

  ///@brief 内部ポート結線の方向の取得
  VpiDir
  portref_dir(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < portref_num() )
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief portref の方向を設定する．
  void
  set_portref_dir(
    SizeType pos, ///< [in] 位置番号 ( 0 <= pos < portref_num() )
    VpiDir dir    ///< [in] 方向
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全体を表す式
  const AstExpr* mExpr;

  // ポート参照式の配列
  PtExprArray mPortRefArray;

  // 方向の配列
  VpiDir* mDirArray;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTMODULE_H
