#ifndef YM_BN_H
#define YM_BN_H

/// @file ym/bn.h
/// @brief ym-bn パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


/// @brief bn 用の名前空間名
#define BN_NSNAME nsBn

/// @brief bn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BN \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(BN_NSNAME)

/// @brief bn 用の名前空間の終了
#define END_NAMESPACE_YM_BN \
END_NAMESPACE(BN_NSNAME) \
END_NAMESPACE_YM

/// @brief bn 用の名前空間
#define BN_NAMESPACE YM_NAMESPACE::BN_NSNAME


BEGIN_NAMESPACE_YM_BN

/// @brief 不正なノード番号
const SizeType BAD_ID = -1;

//////////////////////////////////////////////////////////////////////
// クラスの先行宣言
//////////////////////////////////////////////////////////////////////

class BnModel;
class BnDff;
class BnDffList;
class BnDffIter2;
class BnNode;
class BnNodeList;
class BnNodeIter2;
class BnFunc;

END_NAMESPACE_YM_BN

BEGIN_NAMESPACE_YM

using BN_NAMESPACE::BnModel;
using BN_NAMESPACE::BnDff;
using BN_NAMESPACE::BnDffList;
using BN_NAMESPACE::BnDffIter2;
using BN_NAMESPACE::BnNode;
using BN_NAMESPACE::BnNodeList;
using BN_NAMESPACE::BnNodeIter2;
using BN_NAMESPACE::BnFunc;

END_NAMESPACE_YM

#endif // YM_BN_H
