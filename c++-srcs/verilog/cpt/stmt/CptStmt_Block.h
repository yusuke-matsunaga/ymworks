#ifndef CPTSTMT_BLOCK_H
#define CPTSTMT_BLOCK_H

/// @file CptStmt_Block.h
/// @brief CptStmt_Block のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptStmt_Block CptStmt_Block "CptStmt_Block.h"
/// @brief ブロック型ステートメントの基底クラス
//////////////////////////////////////////////////////////////////////
class CptStmt_Block :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmt_Block(
    const FileRegion& file_region,
    const PtStmt* stmt_top
  ) : CptStmt(file_region),
      mStmtTop{stmt_top}
  {
  }

  /// @brief デストラクタ
  ~CptStmt_Block() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントのリストの取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  const PtStmt*
  stmt_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ステートメントの先頭
  const PtStmt* mStmtTop;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_BlockN CptStmt_Block "CptStmt_Block.h"
/// @brief 名前と宣言要素を持つ StmtBlock
//////////////////////////////////////////////////////////////////////
class CptStmt_BlockN :
  public CptStmt_Block
{
public:

  /// @brief コンストラクタ
  CptStmt_BlockN(
    const FileRegion& file_region,
    const char* name,
    const PtDeclHead* declhead_top,
    const PtStmt* stmt_top
  ) : CptStmt_Block(file_region, stmt_top),
      mName{name},
      mDeclHeadTop{declhead_top}
  {
  }

  /// @brief デストラクタ
  ~CptStmt_BlockN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を取り出す．
  const char*
  name() const override;

  /// @brief 宣言ヘッダのリストの取得
  ///
  /// - type() != NamedParBlock|NamedSeqBlock の時 std::logic_error 例外を送出する．
  const PtDeclHead*
  declhead_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 宣言の先頭
  const PtDeclHead* mDeclHeadTop;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_ParBlock CptStmt_Block "CptStmt_Block.h"
/// @brief parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptStmt_ParBlock :
  public CptStmt_Block
{
public:

  /// @brief コンストラクタ
  CptStmt_ParBlock(
    const FileRegion& file_region,
    const PtStmt* stmt_top
  ) : CptStmt_Block(file_region, stmt_top)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_ParBlock() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  AstStmt::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_ParBlockN CptStmt_Block "CptStmt_Block.h"
/// @brief 名前と宣言要素を持つ parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptStmt_ParBlockN :
  public CptStmt_BlockN
{
public:

  /// @brief コンストラクタ
  CptStmt_ParBlockN(
    const FileRegion& file_region,
    const char* name,
    const PtDeclHead* declhead_top,
    const PtStmt* stmt_top
  ) : CptStmt_BlockN(file_region, name, declhead_top, stmt_top)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_ParBlockN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  AstStmt::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_SeqBlock CptStmt_Block "CptStmt_Block.h"
/// @brief sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptStmt_SeqBlock :
  public CptStmt_Block
{
public:

  /// @brief コンストラクタ
  CptStmt_SeqBlock(
    const FileRegion& file_region,
    const PtStmt* stmt_top
  ) : CptStmt_Block(file_region, stmt_top)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_SeqBlock() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  AstStmt::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptStmt_SeqBlockN :
  public CptStmt_BlockN
{
public:

  /// @brief コンストラクタ
  CptStmt_SeqBlockN(
    const FileRegion& file_region,
    const char* name,
    const PtDeclHead* declhead_top,
    const PtStmt* stmt_top
  ) : CptStmt_BlockN(file_region, name, declhead_top, stmt_top)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_SeqBlockN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  AstStmt::Type
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTSTMT_BLOCK_H
