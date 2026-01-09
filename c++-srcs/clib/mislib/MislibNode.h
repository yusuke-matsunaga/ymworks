#ifndef MISLIBNODE_H
#define MISLIBNODE_H

/// @file MislibNode.h
/// @brief MislibNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "mislib_nsdef.h"
#include "ym/FileRegion.h"
#include "ym/ShString.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibNode MislibNode.h "MislibNode.h"
/// @brief mislib 形式のASTを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibNode
{
public:

  /// @brief コンストラクタ
  explicit
  MislibNode(
    const FileRegion& loc ///< [in] 位置情報
  ) : mLoc{loc}
  {
  }

  /// @brief デストラクタ
  ~MislibNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 共通な関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置情報を取り出す．
  FileRegion
  loc() const { return mLoc; }


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 位置を出力する．
  void
  dump_loc(
    std::ostream& s ///< [in] 出力ストリーム
  ) const
  {
    s << "<LOC>" << loc() << "</LOC>" << std::endl;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 位置情報
  FileRegion mLoc;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibStr MislibStr.h "MislibStr.h"
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibStr :
  public MislibNode
{
public:

  /// @brief コンストラクタ
  MislibStr(
    const FileRegion& loc, ///< [in] 位置情報
    const char* str        ///< [in] 文字列
  ) : MislibNode{loc},
      mStr{str}
  {
  }

  /// @brief デストラクタ
  ~MislibStr() = default;


public:

  /// @brief 文字列を取り出す
  ShString
  str() const { return mStr; }

  /// @brief 内容を出力する(デバッグ用)．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列
  ShString mStr;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibNum MislibNum.h "MislibNum.h"
/// @brief 数値を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibNum :
  public MislibNode
{
public:

  /// @brief コンストラクタ
  MislibNum(
    const FileRegion& loc, ///< [in] 位置情報
    double num             ///< [in] 数値
  ) : MislibNode{loc},
      mNum{num}
  {
  }

  /// @brief デストラクタ
  ~MislibNum() = default;


public:

  /// @brief 数値を取り出す
  double
  num() const { return mNum; }

  /// @brief 内容を出力する(デバッグ用)．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 数値
  double mNum;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibPhase MislibPhase.h "MislibPhase.h"
/// @brief 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibPhase :
  public MislibNode
{
  friend class MislibParser;
public:

  /// @breif 極性の種類
  enum Type {
    Noninv,
    Inv,
    Unknown,
  };


public:

  /// @brief コンストラクタ
  MislibPhase(
    const FileRegion& loc ///< [in] 位置情報
  ) : MislibNode{loc}
  {
  }

  /// @brief デストラクタ
  virtual
  ~MislibPhase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  virtual
  Type
  type() const = 0;

  /// @brief 内容を出力する(デバッグ用)．
  virtual
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibNoninv MislibPhase.h "MislibPhase.h"
/// @brief NONINV 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibNoninv :
  public MislibPhase
{
  friend class MislibParser;

public:

  /// @brief コンストラクタ
  MislibNoninv(
    const FileRegion& loc ///< [in] 位置情報
  ) : MislibPhase{loc}
  {
  }

  /// @brief デストラクタ
  ~MislibNoninv() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibInv MislibPhase.h "MislibPhase.h"
/// @brief INV 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibInv :
  public MislibPhase
{
public:

  /// @brief コンストラクタ
  MislibInv(
    const FileRegion& loc ///< [in] 位置情報
  ) : MislibPhase{loc}
  {
  }

  /// @brief デストラクタ
  ~MislibInv() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibUnknown MislibPhase.h "MislibPhase.h"
/// @brief UNKNOWN 極性を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibUnknown :
  public MislibPhase
{
public:

  /// @brief コンストラクタ
  MislibUnknown(
    const FileRegion& loc ///< [in] 位置情報
  ) : MislibPhase{loc}
  {
  }

  /// @brief デストラクタ
  ~MislibUnknown() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を取り出す．
  Type
  type() const override;

  /// @brief 内容を出力する．
  void
  dump(
    std::ostream& s ///< [in] 出力ストリーム
  ) const override;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBNODE_H
