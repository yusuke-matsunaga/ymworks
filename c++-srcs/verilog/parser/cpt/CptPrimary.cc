
/// @file CptPrimary.cc
/// @brief CptPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"
#include "parser/PtFactory.h"
#include "parser/PtHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryBase::CptPrimaryBase(
  const char* name
) : mName{name}
{
}

// デストラクタ
CptPrimaryBase::~CptPrimaryBase()
{
}

// クラスの型を返す．
AstExpr::Type
CptPrimaryBase::type() const
{
  return Primary;
}

// 末尾の名前を取り出す．
const char*
CptPrimaryBase::name() const
{
  return mName;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryBase::is_simple() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimary
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimary::CptPrimary(
  const FileRegion& file_region,
  const char* name
) : CptPrimaryBase(name),
    mFileRegion{file_region}
{
}

// デストラクタ
CptPrimary::~CptPrimary()
{
}

// ファイル位置を返す．
FileRegion
CptPrimary::file_region() const
{
  return mFileRegion;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryI
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryI::CptPrimaryI(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& index_array
) : CptPrimaryBase(name),
    mFileRegion{file_region},
    mIndexArray{std::move(index_array)}
{
}

// デストラクタ
CptPrimaryI::~CptPrimaryI()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryI::file_region() const
{
  return mFileRegion;
}

// @brief インデックスリストのサイズの取得
SizeType
CptPrimaryI::index_num() const
{
  return mIndexArray.size();
}

// @brief インデックスの取得
const AstExpr*
CptPrimaryI::index(
  SizeType pos
) const
{
  return mIndexArray[pos];
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryI::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryCI
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryCI::CptPrimaryCI(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& index_array
) : CptPrimaryI(file_region, name, std::move(index_array))
{
}

// デストラクタ
CptPrimaryCI::~CptPrimaryCI()
{
}

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryCI::is_const_index() const
{
  // この関数こそがこのクラスの存在意義
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryR
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryR::CptPrimaryR(
  const FileRegion& file_region,
  const char* name,
  const AstPart* part
) : CptPrimaryBase(name),
    mFileRegion{file_region},
    mPart{part}
{
}

// デストラクタ
CptPrimaryR::~CptPrimaryR()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryR::file_region() const
{
  return mFileRegion;
}

// 範囲指定の取得
const AstPart*
CptPrimaryR::part() const
{
  return mPart;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryR::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryCR
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryCR::CptPrimaryCR(
  const FileRegion& file_region,
  const char* name,
  const AstPart* part
) : CptPrimaryR(file_region, name, part)
{
}

// デストラクタ
CptPrimaryCR::~CptPrimaryCR()
{
}

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryCR::is_const_index() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryIR
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryIR::CptPrimaryIR(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& index_array,
  const AstPart* part
) : CptPrimaryI(file_region, name, std::move(index_array)),
    mPart{part}
{
}

// デストラクタ
CptPrimaryIR::~CptPrimaryIR()
{
}

// 範囲指定を取り出す．
const AstPart*
CptPrimaryIR::part() const
{
  return mPart;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryH
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryH::CptPrimaryH(
  const FileRegion& file_region,
  PtNameBranchArray&& nb_array,
  const char* tail_name
) : CptPrimaryBase(tail_name),
    mFileRegion{file_region},
    mNbArray{std::move(nb_array)}
{
}

// デストラクタ
CptPrimaryH::~CptPrimaryH()
{
}

// ファイル位置を返す．
FileRegion
CptPrimaryH::file_region() const
{
  return mFileRegion;
}

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryH::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const AstNameBranch*
CptPrimaryH::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHI
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHI::CptPrimaryHI(
  const FileRegion& file_region,
  PtNameBranchArray&& nb_array,
  const char* tail_name,
  PtExprArray&& index_array
) : CptPrimaryI(file_region, tail_name, std::move(index_array)),
    mNbArray{std::move(nb_array)}
{
}

// デストラクタ
CptPrimaryHI::~CptPrimaryHI()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHI::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const AstNameBranch*
CptPrimaryHI::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHCI
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHCI::CptPrimaryHCI(
  const FileRegion& file_region,
  PtNameBranchArray&& nb_array,
  const char* tail_name,
  PtExprArray&& index_array
) : CptPrimaryHI(file_region, std::move(nb_array),
		 tail_name, std::move(index_array))
{
}

// デストラクタ
CptPrimaryHCI::~CptPrimaryHCI()
{
}

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryHCI::is_const_index() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHR
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHR::CptPrimaryHR(
  const FileRegion& file_region,
  PtNameBranchArray&& nb_array,
  const char* tail_name,
  const AstPart* part
) : CptPrimaryR(file_region, tail_name, part),
    mNbArray{std::move(nb_array)}
{
}

// デストラクタ
CptPrimaryHR::~CptPrimaryHR()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHR::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const AstNameBranch*
CptPrimaryHR::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHIR
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptPrimaryHIR::CptPrimaryHIR(
  const FileRegion& file_region,
  PtNameBranchArray&& nb_array,
  const char* tail_name,
  PtExprArray&& index_array,
  const AstPart* part
) : CptPrimaryIR(file_region, tail_name,
		 std::move(index_array),
		 part),
    mNbArray{std::move(nb_array)}
{
}

// デストラクタ
CptPrimaryHIR::~CptPrimaryHIR()
{
}

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHIR::namebranch_num() const
{
  return mNbArray.size();
}

// @brief 階層ブランチの取得
const AstNameBranch*
CptPrimaryHIR::namebranch(
  SizeType pos
) const
{
  return mNbArray[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimary));
  return new (p) CptPrimary(file_region, name);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& index_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryI));
  return new (p) CptPrimaryI(file_region, name, std::move(index_array));
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryR));
  return new (p) CptPrimaryR(file_region, name, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& index_array,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryIR));
  return new (p) CptPrimaryIR(file_region, name,
			      std::move(index_array),
			      part);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  PtHierName* hname
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryH));
  auto tail_name = hname->tail_name();
  return new (p) CptPrimaryH(file_region,
			     hname->name_branch_to_array(mAlloc),
			     tail_name);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  PtHierName* hname,
  PtExprArray&& index_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHI));
  auto tail_name = hname->tail_name();
  return new (p) CptPrimaryHI(file_region,
			      hname->name_branch_to_array(mAlloc),
			      tail_name,
			      std::move(index_array));
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  PtHierName* hname,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHR));
  auto tail_name = hname->tail_name();
  return new (p) CptPrimaryHR(file_region,
			      hname->name_branch_to_array(mAlloc),
			      tail_name, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  PtHierName* hname,
  PtExprArray&& index_array,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHIR));
  auto tail_name = hname->tail_name();
  return new (p) CptPrimaryHIR(file_region,
			       hname->name_branch_to_array(mAlloc),
			       tail_name,
			       std::move(index_array),
			       part);
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  PtExprArray&& index_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryCI));
  return new (p) CptPrimaryCI(file_region, name,
			      std::move(index_array));
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryCR));
  return new (p) CptPrimaryCR(file_region, name, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  PtHierName* hname,
  PtExprArray&& index_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHCI));
  auto tail_name = hname->tail_name();
  return new (p) CptPrimaryHCI(file_region,
			       hname->name_branch_to_array(mAlloc),
			       tail_name,
			       std::move(index_array));
}

END_NAMESPACE_YM_VERILOG
