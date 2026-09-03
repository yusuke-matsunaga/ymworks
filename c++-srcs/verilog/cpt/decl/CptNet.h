#ifndef CPTNET_H
#define CPTNET_H

/// @file CptNet.h
/// @brief CptNet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptDeclHead.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetH :
  public CptDeclHead
{
public:

  /// @brief コンストラクタ
  CptNetH(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,           ///< [in] net の型
    bool sign,                     ///< [in] 符号付きのとき true となるフラグ
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptDeclHead(file_region, item_top)
  {
    mFlags = (static_cast<std::uint32_t>(net_type) << 1) |
      static_cast<std::uint32_t>(sign);
  }

  /// @brief デストラクタ
  ~CptNetH() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスを識別するための型を返す．
  /// @return 宣言要素の型
  AstDeclHead::Type
  type() const override;

  /// @brief net type を返す．(vpiWire など)
  /// @return net 型
  VpiNetType
  net_type() const override;

  /// @brief 符号の有無の取得
  bool
  is_signed() const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // net type + sign (+ vstype)
  std::uint32_t mFlags;

};


//////////////////////////////////////////////////////////////////////
/// strength つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHS :
  public CptNetH
{
public:

  /// @brief コンストラクタ
  CptNetHS(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    bool sign,                     ///< [in] 符号付きのとき true となるフラグ
    const PtStrength* strength,    ///< [in] 信号強度
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptNetH(file_region, net_type, sign, item_top),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptNetHS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  const PtStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  /// strength
  const PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHD :
  public CptNetH
{
public:

  /// @brief コンストラクタ
  CptNetHD(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtDelay* delay,          ///< [in] 遅延
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptNetH(file_region, net_type, sign, item_top),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptNetHD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  /// @return delay
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// strength と delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHSD :
  public CptNetH
{
public:

  /// @brief コンストラクタ
  CptNetHSD(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtStrength* strength,    ///< [in] 信号強度
    const PtDelay* delay,          ///< [in] 遅延
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptNetH(file_region, net_type, sign, item_top),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptNetHSD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  /// @return delay
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// ビットベクタ型の net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////
class CptNetHV :
  public CptNetH
{
public:

  /// @brief コンストラクタ
  CptNetHV(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    VpiVsType vstype,		   ///< [in] vectored/scalared 属性を表す値
    bool sign,                     ///< [in] 符号付きのとき true となるフラグ
    const PtRange* range,          ///< [in] パース木の範囲定義
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptNetH(file_region, net_type, sign, item_top),
      mRange{range}
  {
    mFlags |= (static_cast<std::uint32_t>(vstype) << 5);
  }

  /// @brief デストラクタ
  ~CptNetHV() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief vectored/scalared 属性を返す．
  /// @retval kVpiVsNone vectored|scalared 指定なし
  /// @retval kVpiVectored vectored 指定あり
  /// @retval kVpiScalared scalared 指定あり
  VpiVsType
  vs_type() const override;

  /// @brief 範囲を取り出す．
  /// @return 範囲
  const PtRange*
  range() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲
  const PtRange* mRange;

};


//////////////////////////////////////////////////////////////////////
/// strength つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVS :
  public CptNetHV
{
public:

  /// @brief コンストラクタ
  CptNetHVS(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    VpiVsType vstype,		   ///< [in] vectored/scalared 属性を表す値
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtRange* range,          ///< [in] パース木の範囲定義
    const PtStrength* strength,    ///< [in] 信号強度
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptNetHV(file_region, net_type, vstype, sign, range, item_top),
      mStrength{strength}
  {
  }

  /// @brief デストラクタ
  ~CptNetHVS() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  const PtStrength*
  strength() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

};


//////////////////////////////////////////////////////////////////////
/// delay つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVD :
  public CptNetHV
{
public:

  /// @brief コンストラクタ
  CptNetHVD(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    VpiVsType vstype,		   ///< [in] vectored/scalared 属性を表す値
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtRange* range,          ///< [in] パース木の範囲定義
    const PtDelay* delay,          ///< [in] 遅延
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptNetHV(file_region, net_type,
	       vstype, sign, range,
	       item_top),
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptNetHVD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief delay を返す．
  /// @return delay
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // delay
  const PtDelay* mDelay;

};


//////////////////////////////////////////////////////////////////////
/// strength と delay つきの PtNetHV
//////////////////////////////////////////////////////////////////////
class CptNetHVSD :
  public CptNetHV
{
public:

  /// @brief コンストラクタ
  CptNetHVSD(
    const FileRegion& file_region, ///< [in] ファイル位置の情報
    VpiNetType net_type,	   ///< [in] net の型
    VpiVsType vstype,		   ///< [in] vectored/scalared 属性を表す値
    bool sign,			   ///< [in] 符号付きのとき true となるフラグ
    const PtRange* range,          ///< [in] パース木の範囲定義
    const PtStrength* strength,    ///< [in] 信号強度
    const PtDelay* delay,          ///< [in] 遅延
    const PtDeclItem* item_top     ///< [in] 要素の先頭
  ) : CptNetHV(file_region, net_type,
	       vstype, sign, range,
	       item_top),
      mStrength{strength},
      mDelay{delay}
  {
  }

  /// @brief デストラクタ
  ~CptNetHVSD() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtDecl の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief strength を返す．
  /// @return strength
  const PtStrength*
  strength() const override;

  /// @brief delay を返す．
  /// @return delay
  const PtDelay*
  delay() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // strength
  const PtStrength* mStrength;

  // delay
  const PtDelay* mDelay;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTNET_H
