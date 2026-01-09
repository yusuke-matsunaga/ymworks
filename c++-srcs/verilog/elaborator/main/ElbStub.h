#ifndef ELBSTUB_H
#define ELBSTUB_H

/// @file ElbStub.h
/// @brief ElbStub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbStub ElbStub.h "ElbStub.h"
/// @brief インスタンス化で用いる stub クラス
//////////////////////////////////////////////////////////////////////
class ElbStub
{
public:

  /// @brief 仮想デストラクタ
  virtual
  ~ElbStub() = default;

  /// @brief 処理を行う．
  virtual
  void
  eval() = 0;

};


//////////////////////////////////////////////////////////////////////
/// @name 個々のクラスごとの ElbStub クラスを作るためのテンプレートクラス
//////////////////////////////////////////////////////////////////////
//@{

/// @brief 引数が1つのタイプ
template<typename T,
	 typename A>
class ElbStubT1 :
  public ElbStub
{
public:

  /// @brief 引数が1つの ElbMgrImpl のメンバ関数
  using ElbFunc = void (T::*)(A);

public:

  /// @brief コンストラクタ
  ElbStubT1(
    T* obj,          ///< [in] 対象のオブジェクト
    ElbFunc memfunc, ///< [in] メンバ関数へのポインタ
    A a              ///< [in] memfunc を起動するときの引数
  ) : mObj{obj},
      mMemFunc{memfunc},
      mA{a}
  {
  }

  /// @brief デストラクタ
  ~ElbStubT1() = default;

  /// @brief 処理を行う．
  /// @note obj->memfunc(a) を実行する．
  void
  eval() override
  {
    (mObj->*(mMemFunc))(mA);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // T のオブジェクト
  T* mObj;

  // T のメンバ関数
  ElbFunc mMemFunc;

  // mMemFunc に渡される引数
  A mA;

};


/// @brief 引数が2つのタイプ
template<typename T,
	 typename A,
	 typename B>
class ElbStubT2 :
  public ElbStub
{
public:

  /// @brief 引数が2つの ElbMgrImpl のメンバ関数
  using ElbFunc = void(T::*)(A, B);

public:

  /// @brief コンストラクタ
  ElbStubT2(
    T* obj,          ///< [in] 対象のオブジェクト
    ElbFunc memfunc, ///< [in] メンバ関数へのポインタ
    A a,             ///< [in] memfunc を起動する時の引数1
    B b              ///< [in] memfunc を起動する時の引数2
  ) : mObj{obj},
      mMemFunc{memfunc},
      mA{a},
      mB{b}
  {
  }

  /// @brief デストラクタ
  ~ElbStubT2() = default;

  /// @brief 処理を行う．
  ///
  /// obj->memfunc(a, b) を実行する．
  void
  eval() override
  {
    (mObj->*(mMemFunc))(mA, mB);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;

  // mMemFunc に渡される引数
  A mA;
  B mB;

};


/// @brief 引数が3つのタイプ
template<typename T,
	 typename A,
	 typename B,
	 typename C>
class ElbStubT3 :
  public ElbStub
{
public:

  /// @brief 引数が3つの ElbMgrImpl のメンバ関数
  using ElbFunc = void (T::*)(A, B, C);

public:

  /// @brief コンストラクタ
  ElbStubT3(
    T* obj,           ///< [in] 対象のオブジェクト
    ElbFunc memfunc,  ///< [in] メンバ関数へのポインタ
    A a,	      ///< [in] memfunc を起動する時の引数1
    B b,	      ///< [in] memfunc を起動する時の引数2
    C c               ///< [in] memfunc を起動する時の引数3
  ) :  mObj{obj},
       mMemFunc{memfunc},
       mA{a},
       mB{b},
       mC{c}
  {
  }

  /// @brief デストラクタ
  ~ElbStubT3() = default;

  /// @brief 処理を行う．
  /// @note obj->memfunc(a, b, c) を実行する．
  void
  eval() override
  {
    (mObj->*(mMemFunc))(mA, mB, mC);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;

  // mMemFunc に渡される引数
  A mA;
  B mB;
  C mC;

};


/// @brief 引数が4つのタイプ
template<typename T,
	 typename A,
	 typename B,
	 typename C,
	 typename D>
class ElbStubT4 :
  public ElbStub
{
public:

  /// @brief 引数が4つの ElbMgrImpl のメンバ関数
  using ElbFunc = void (T::*)(A, B, C, D);

public:

  /// @brief コンストラクタ
  ElbStubT4(
    T* obj,          ///< [in] 対象のオブジェクト
    ElbFunc memfunc, ///< [in] メンバ関数へのポインタ
    A a,	     ///< [in] memfunc を起動する時の引数1
    B b,	     ///< [in] memfunc を起動する時の引数2
    C c,	     ///< [in] memfunc を起動する時の引数3
    D d              ///< [in] memfunc を起動する時の引数4
  ) : mObj{obj},
      mMemFunc{memfunc},
      mA{a},
      mB{b},
      mC{c},
      mD{d}
  {
  }

  /// @brief デストラクタ
  ~ElbStubT4() = default;

  /// @brief 処理を行う．
  /// @note obj->memfunc(a, b, c, d) を実行する．
  void
  eval() override
  {
    (mObj->*(mMemFunc))(mA, mB, mC, mD);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // T のオブジェクト
  T* mObj;

  // ElbMgrImpl のメンバ関数
  ElbFunc mMemFunc;

  // mMemFunc に渡される引数
  A mA;
  B mB;
  C mC;
  D mD;

};

//@}

END_NAMESPACE_YM_VERILOG

#endif // ELBSTUB_H
