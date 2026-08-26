#ifndef CPTBLOCKSTMT_H
#define CPTBLOCKSTMT_H

/// @file CptStmt.h
/// @brief CptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief StmtBlock の基底クラス
//////////////////////////////////////////////////////////////////////
class CptStmtBlock :
  public CptStmt
{
protected:

  /// @brief コンストラクタ
  CptStmtBlock(
    const FileRegion& file_region,
    PtStmt* stmt_top
  ) : CptStmt(file_region),
      mStmtTop{stmt_top}
  {
  }

  /// @brief デストラクタ
  ~CptStmtBlock() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmtBlock の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のステートメントのリストの取得
  ///
  /// - type() != ParBlock|SeqBlock|NamedParBlock|NamedSeqBlock の時
  ///   std::logic_error 例外を送出する．
  AstStmtList
  stmt_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ステートメントの先頭
  const AstStmt* mStmtTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ StmtBlock
//////////////////////////////////////////////////////////////////////
class CptStmtBlockN :
  public CptStmtBlock
{
public:

  /// @brief コンストラクタ
  CptStmtBlockN(
    const FileRegion& file_region,
    const char* name,
    PtDeclHead* declhead_top,
    PtStmt* stmt_top
  ) : CptStmtBlock(file_region, stmt_top),
      mName{name},
      mDeclHeadTop{declhead_top}
  {
  }

  /// @brief デストラクタ
  ~CptStmtBlockN() {}


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
  AstDeclHeadList
  declhead_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 宣言の先頭
  PtDeclHead* mDeclHeadTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptParBlock :
  public CptStmtBlock
{
public:

  /// @brief コンストラクタ
  CptParBlock(
    const FileRegion& file_region,
    PtStmt* stmt_top
  ) : CptStmtBlock(file_region, stmt_top)
  {
  }

  /// @brief デストラクタ
  ~CptParBlock() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ parallel block を表すノード
//////////////////////////////////////////////////////////////////////
class CptParBlockN :
  public CptStmtBlockN
{
public:

  /// @brief コンストラクタ
  CptParBlockN(
    const FileRegion& file_region,
    const char* name,
    PtDeclHead* declhead_top,
    PtStmt* stmt_top
  ) : CptStmtBlockN(file_region, name, declhead_top, stmt_top)
  {
  }

  /// @brief デストラクタ
  ~CptParBlockN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptSeqBlock :
  public CptStmtBlock
{
public:

  /// @brief コンストラクタ
  CptSeqBlock(
    const FileRegion& file_region,
    PtStmt* stmt_top
  ) : CptStmtBlock(file_region, stmt_top)
  {
  }

  /// @brief デストラクタ
  ~CptSeqBlock() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前と宣言要素を持つ sequential block を表すノード
//////////////////////////////////////////////////////////////////////
class CptSeqBlockN :
  public CptStmtBlockN
{
public:

  /// @brief コンストラクタ
  CptSeqBlockN(
    const FileRegion& file_region,
    const char* name,
    PtDeclHead* declhead_top,
    PtStmt* stmt_top
  ) : CptStmtBlockN(file_region, name, declhead_top, stmt_top)
  {
  }

  /// @brief デストラクタ
  ~CptSeqBlockN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTBLOCKSTMT_H
