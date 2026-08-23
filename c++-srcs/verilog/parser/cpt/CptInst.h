#ifndef CPTINST_H
#define CPTINST_H

/// @file CptInst.h
/// @brief CptInst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief gate header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptGateH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptGateH(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    PtInstArray&& inst_list
  ) : mFileRegion{file_region},
      mPrimType{prim_type},
      mInstList{std::move(inst_list)}
  {
  }

  /// @brief デストラクタ
  ~CptGateH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief プリミティブタイプを返す．
  VpiPrimType
  prim_type() const override;

  /// @brief module/UDP/gate instance の要素数の取得
  SizeType
  inst_num() const override;

  /// @brief module/UDP/gate instance リストの取得
  const AstInst*
  inst(
    SizeType index ///< [in] インデックス ( 0 <= index < inst_num() )
  ) const override;

  /// @brief module/UDP/gate instance リストの取得
  AstInstVec
  inst_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // プリミティブタイプ
  VpiPrimType mPrimType;

  // 要素の配列
  PtInstArray mInstList;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGateHS :
  public CptGateH
{
public:

  /// @brief コンストラクタ
  CptGateHS(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const AstStrength* strength,
    PtInstArray&& inst_list
  ) : CptGateH(file_region, prim_type, std::move(inst_list)),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptGateHS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtGateH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const AstStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const AstStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGateHD :
  public CptGateH
{
public:

  /// @brief コンストラクタ
  CptGateHD(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const AstDelay* delay,
    PtInstArray&& inst_list
  ) : CptGateH(file_region, prim_type, std::move(inst_list)),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptGateHD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtGateH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  const AstDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const AstDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength と delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGateHSD :
  public CptGateH
{
public:

  /// @brief コンストラクタ
  CptGateHSD(
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const AstStrength* strength,
    const AstDelay* delay,
    PtInstArray&& inst_list
  ) : CptGateH(file_region, prim_type, std::move(inst_list)),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptGateHSD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtGateH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const AstStrength*
  strength() const override;

  /// @brief delay を返す．
  const AstDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const AstStrength* mStrength;

  // delay
  const AstDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief module/udp header のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptMuH :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptMuH(
    const FileRegion& file_region,
    const char* def_name,
    PtInstArray&& inst_list
  ) : mFileRegion{file_region},
      mName{def_name},
      mInstList{std::move(inst_list)}
  {
  }

  /// @brief デストラクタ
  ~CptMuH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief 定義名を返す．
  const char*
  name() const override;

  /// @brief module/UDP/gate instance の要素数の取得
  SizeType
  inst_num() const override;

  /// @brief module/UDP/gate instance リストの取得
  const AstInst*
  inst(
    SizeType index ///< [in] インデックス ( 0 <= index < inst_num() )
  ) const override;

  /// @brief module/UDP/gate instance リストの取得
  AstInstVec
  inst_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 定義名
  const char* mName;

  // 要素の配列
  PtInstArray mInstList;

};


//////////////////////////////////////////////////////////////////////
/// @brief パラメータ割り当てつきの CptMuH
//////////////////////////////////////////////////////////////////////
class CptMuHP :
  public CptMuH
{
public:

  /// @brief コンストラクタ
  CptMuHP(
    const FileRegion& file_region,
    const char* def_name,
    PtConnectionArray&& con_list,
    PtInstArray&& inst_list
  ) : CptMuH(file_region, def_name, std::move(inst_list)),
      mParamAssignList{std::move(con_list)}
  {
  }

  /// @brief デストラクタ
  ~CptMuHP() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ割り当ての要素数の取得
  SizeType
  paramassign_num() const override;

  /// @brief パラメータ割り当ての取得
  const AstConnection*
  paramassign(
    SizeType index ///< [in] インデックス ( 0 <= index < paramassign_num() )
  ) const override;

  /// @brief パラメータ割り当てのリストの取得
  AstConnectionVec
  paramassign_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ割り当てのリスト
  PtConnectionArray mParamAssignList;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptMuHS :
  public CptMuH
{
public:

  /// @brief コンストラクタ
  CptMuHS(
    const FileRegion& file_region,
    const char* def_name,
    const AstStrength* strength,
    PtInstArray&& inst_list
  ) : CptMuH(file_region, def_name, std::move(inst_list)),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptMuHS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const AstStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const AstStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// @brief delay を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptMuHD :
  public CptMuH
{
public:

  /// @brief コンストラクタ
  CptMuHD(
    const FileRegion& file_region,
    const char* def_name,
    const AstDelay* delay,
    PtInstArray&& inst_list
  ) : CptMuH(file_region, def_name, std::move(inst_list)),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptMuHD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  const AstDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const AstDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief strength と delay を持つ MuH
//////////////////////////////////////////////////////////////////////
class CptMuHSD :
  public CptMuH
{
public:

  /// @brief コンストラクタ
  CptMuHSD(
    const FileRegion& file_region,
    const char* def_name,
    const AstStrength* strength,
    const AstDelay* delay,
    PtInstArray&& inst_list
  ) : CptMuH(file_region, def_name, std::move(inst_list)),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptMuHSD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuH の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  const AstStrength*
  strength() const override;

  /// @brief delay を返す．
  const AstDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const AstStrength* mStrength;

  // delay
  const AstDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// @brief module instance/UDP/gate instance のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptInst :
  public PtInst
{
public:

  /// @brief コンストラクタ
  CptInst(
    const FileRegion& file_region,
    PtConnectionArray&& con_list
  ) : mFileRegion{file_region},
      mPortList{std::move(con_list)}
  {
  }

  /// @brief デストラクタ
  ~CptInst() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtMuInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 名前の取得
  /// @return 名前
  const char*
  name() const override;

  /// @brief 範囲の取得
  /// @return 範囲
  const AstRange*
  range() const override;

  /// @brief ポートの要素数の取得
  SizeType
  port_num() const override;

  /// @brief ポート接続の取得
  const AstConnection*
  port(
    SizeType index ///< [in] インデックス ( 0 <= index < port_num() )
  ) const override;

  /// @brief ポートリストの取得
  AstConnectionVec
  port_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ポート割り当ての配列
  PtConnectionArray mPortList;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前を持つ CptInst
//////////////////////////////////////////////////////////////////////
class CptInstN :
  public CptInst
{
public:

  /// @brief コンストラクタ
  CptInstN(
    const FileRegion& file_region,
    const char* name,
    PtConnectionArray&& con_list
  ) : CptInst(file_region, std::move(con_list)),
      mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptInstN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と範囲指定を持つ CptInst
//////////////////////////////////////////////////////////////////////
class CptInstR :
  public CptInstN
{
public:

  /// @brief コンストラクタ
  CptInstR(
    const FileRegion& file_region,
    const char* name,
    const AstRange* range,
    PtConnectionArray&& con_list
  ) : CptInstN(file_region, name, std::move(con_list)),
      mRange{range}
  {
  }

  /// @brief デストラクタ
  ~CptInstR() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtInst の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲を取出す．
  const AstRange*
  range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const AstRange* mRange;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTINST_H
