#ifndef YM_VL_ASTBASE_H
#define YM_VL_ASTBASE_H

/// @file ym/vl/AstBase.h
/// @brief Ast の基底クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

// in AstBase.h
class AstBase;
class AstNamedBase;
class AstHierNamedBase;

// in AstUdp.h
class AstUdp;
class AstUdpEntry;
//using AstUdpEntryList = std::vector<const AstUdpEntry*>;
class AstUdpValue;
//using AsgUdpValueList = std::vector<const AstUdpValue*>;

// in AstModule.h
class AstModule;

// in AstPort.h
class AstPort;
//using AstPortList = std::vector<const AstPort*>;

// in AstDecl.h
class AstIOHead;
//using AstIOHeadList = std::vector<const AstIOHead*>;

class AstIOItem;

class AstDeclHead;
//using AstDeclHeadList = std::vector<const AstDeclHead*>;

class AstDeclItem;
class AstRange;
//using AstRangeList = std::vector<const AstRange*>;
class AstPart;

// in AstItem.h
class AstItem;
//using AstItemList = std::vector<const AstItem*>;
class AstDefParam;
//using AstDefparamList = std::vector<const AstDefparam*>;
class AstContAssign;
//using AstContAssignList = std::vector<const AstContAssign*>;
class AstInst;
//using AstInstList = std::vector<const AstInst*>;
class AstGenCaseItem;
//using AstGenCaseItemList = std::vector<const AstGenCaseItem*>;
class AstPathDecl;
class AstPathDelay;

// in AstStmt.h
class AstStmt;
//using AstStmtList = std::vector<const AstStmt*>;
class AstCaseItem;
//using AstCaseItemList = std::vector<const AstCaseItem*>;

// in AstExpr.h
class AstExpr;
//using AstExprList = std::vector<const AstExpr*>;

// in AstMisc.h
class AstStrength;
class AstDelay;
class AstControl;
class AstConnection;
//using AstConnectionList = std::vector<const AstConnection*>;
class AstNameBranch;
//using AstNameBranchList = std::vector<const AstNameBranch*>;
class AstAttrInst;
class AstAttrSpec;


//////////////////////////////////////////////////////////////////////
/// @class AstBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 全ての Ast クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class AstBase
{
public:

  /// @brief デストラクタ
  virtual
  ~AstBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // AstBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置の取得
  /// @return ファイル位置
  virtual
  FileRegion
  file_region() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstNamedBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 名前を持つ AstBase
//////////////////////////////////////////////////////////////////////
class AstNamedBase :
  public AstBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstNamedBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  /// @return 名前
  virtual
  const char*
  name() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class AstHierNamedBase AstBase.h "ym/vl/AstBase.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief 階層付き名前を持つ AstBase
//////////////////////////////////////////////////////////////////////
class AstHierNamedBase :
  public AstNamedBase
{
public:
  //////////////////////////////////////////////////////////////////////
  // AstHierNamedBase の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 階層ブランチの要素数の取得
  virtual
  SizeType
  namebranch_num() const = 0;

  /// @brief 階層ブランチの取得
  ///
  /// - pos >= namebranch_num() の時 std::out_of_range 例外を送出する．
  virtual
  const AstNameBranch*
  namebranch(
    SizeType pos ///< [in] 位置 ( 0 <= pos < namebranch_num() )
  ) const = 0;

  /// @brief 階層名を展開する．
  std::string
  fullname() const;

  /// @brief 階層ブランチのリストを返す．
  std::vector<const AstNameBranch*>
  namebranch_list() const
  {
    auto n = namebranch_num();
    std::vector<const AstNameBranch*> vec;
    vec.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto nb = namebranch(i);
      vec.push_back(nb);
    }
    return vec;
  }

};

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_STD

// AstModule へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::nsVerilog::AstModule*>
{
  SizeType
  operator()(YM_NAMESPACE::nsVerilog::AstModule* module) const
  {
    auto tmp = reinterpret_cast<PtrIntType>(module)/sizeof(void*);
    return static_cast<SizeType>(tmp);
  }
};

// const AstModule へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<const YM_NAMESPACE::nsVerilog::AstModule*>
{
  SizeType
  operator()(const YM_NAMESPACE::nsVerilog::AstModule* module) const
  {
    auto tmp = reinterpret_cast<PtrIntType>(module)/sizeof(void*);
    return static_cast<SizeType>(tmp);
  }
};

END_NAMESPACE_STD

#endif // YM_VL_ASTBASE_H
