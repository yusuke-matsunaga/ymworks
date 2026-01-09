#ifndef AIGPARSER_H
#define AIGPARSER_H

/// @file AigParser.h
/// @brief AigParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/aig.h"
#include "AigEdge.h"
#include "ym/FileInfo.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigParser AigParser.h "AigParser.h"
/// @brief AIG 形式のファイルを読むためのクラス
//////////////////////////////////////////////////////////////////////
class AigParser
{
public:

  /// @brief コンストラクタ
  AigParser(
    AigMgrImpl* mgr ///< [in] 読み込んだ結果を格納するマネージャ
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~AigParser() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ファイル入出力
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief Ascii AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aag(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief AIG フォーマットを読み込む．
  /// @return 読み込みが成功したら true を返す．
  bool
  read_aig(
    const std::string& filename ///< [in] ファイル名
  );

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  bool
  open(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief aag ヘッダ行を読み込む．
  bool
  read_aag_header(
    SizeType& M, ///< [out] 総ノード数を格納する変数
    SizeType& I, ///< [out] 入力数を格納する変数
    SizeType& L, ///< [out] ラッチ数を格納する変数
    SizeType& O, ///< [out] 出力数を格納する変数
    SizeType& A  ///< [out] ANDノード数を格納する変数
  );

  /// @brief aig ヘッダ行を読み込む．
  bool
  read_aig_header(
    SizeType& M, ///< [out] 総ノード数を格納する変数
    SizeType& I, ///< [out] 入力数を格納する変数
    SizeType& L, ///< [out] ラッチ数を格納する変数
    SizeType& O, ///< [out] 出力数を格納する変数
    SizeType& A	 ///< [out] ANDノード数を格納する変数
  );

  /// @brief 初期化する．
  void
  initialize(
    SizeType M, ///< [in] 総ノード数(正確には最大のノード番号+1)
    SizeType I, ///< [in] 入力数
    SizeType O, ///< [in] 出力数
    SizeType L  ///< [in] ラッチ(DFF)数
  );

  /// @brief aag の入力行の読み込み
  bool
  read_input(
    SizeType input_id ///< [in] 入力番号
  );

  /// @brief aag のラッチ行の読み込み
  bool
  read_latch(
    SizeType latch_id ///< [in] ラッチ番号
  );

  /// @brief aag の出力行/aig のラッチ行の読み込み
  bool
  read_src(
    SizeType& src ///< [out] 結果のソースリテラルを格納するオブジェクト
  );

  /// @brief aag のAND行の読み込み
  bool
  read_and(
    SizeType and_id
  );

  /// @brief aig の数字の読み込み
  SizeType
  read_number();

  /// @brief シンボルテーブルとコメントを読み込む．
  void
  read_symbols();

  /// @brief 1行読み出す．
  bool
  read_line(
    std::string& buf  ///< [out] 結果を格納するオブジェクト
  );

  /// @brief リテラルを定義済みにする．
  /// @return エラーの場合にfalseを返す．
  ///
  /// エラーとなるのは以下の場合
  /// - 定数のリテラル
  /// - 負のリテラル(奇数)
  /// - すでに定義済み
  bool
  set_defined(
    SizeType lit, ///< [in] リテラル
    SizeType& id  ///< [out] ID番号を格納する変数
  );

  /// @brief DFF の設定を行う．
  void
  set_dff(
    SizeType dff_id, ///< [in] ラッチ番号
    SizeType oid,    ///< [in] 出力のノード番号
    SizeType src_lit ///< [in] 入力のリテラル
  );

  /// @brief リテラルが定義済みか調べる．
  bool
  check_defined(
    SizeType lit,          ///< [in] リテラル
    const std::string& ref ///< [in] このリテラルを参照しているオブジェクト名
  );

  /// @brief リテラルを枝に変換する
  AigEdge
  conv_to_edge(
    SizeType lit
  );

  /// @brief リテラルからID番号を取り出す．
  static
  SizeType
  lit_to_id(
    SizeType lit
  )
  {
    return (lit / 2) - 1;
  }

  /// @brief リテラルから反転属性を取り出す．
  static
  bool
  lit_to_inv(
    SizeType lit
  )
  {
    return (lit % 2) == 1;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // aag で読み込んだ情報を表す構造体
  struct NodeInfo {
    bool defined; // 定義済みの印
    int src0;     // ファンイン０
    int src1;     // ファンイン１
    AigEdge edge; // 結果の枝
  };

  // 入力ストリーム
  std::istream* mS{nullptr};

  // ファイルの情報
  FileInfo mFileInfo;

  // 現在の行番号
  int mLineNo;

  // 右端のコラム
  int mLastCol;

  // 対象のマネージャ
  AigMgrImpl* mMgr{nullptr};

  // ノード番号をキーにして定義済みの印を格納する配列
  std::vector<bool> mDefined;

};

END_NAMESPACE_YM_AIG

#endif // AIGPARSER_H
