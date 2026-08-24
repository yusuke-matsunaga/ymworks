#ifndef ASTDUMPER_H
#define ASTDUMPER_H

/// @file AstDumper.h
/// @brief AstDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/Ast.h"
#include "ym/FileLoc.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AstDumper AstDumper.h "parser/AstDumper.h"
/// @ingroup VlParser
/// @ingroup AstGroup
/// @brief Ast クラスの内容を出力するクラス
//////////////////////////////////////////////////////////////////////
class AstDumper
{
  friend class AstHeader;
public:

  /// @brief コンストラクタ
  AstDumper(
    std::ostream& s ///< [in] 出力先のストリーム
  );

  /// @brief デストラクタ
  ~AstDumper();


public:

  /// @brief UDP とモジュールの内部情報の表示
  void
  put(
    const std::vector<const AstUdp*>& udp_list,      ///< [in] UDP のリスト
    const std::vector<const AstModule*>& module_list ///< [in] モジュールのリスト
  );


  /// @brief UDP を表示する
  void
  put(
    const AstUdp* udp ///< [in] UDP
  );

  /// @brief UDP テーブルの値を表示する
  void
  put(
    const char* label,  ///< [in] ラベル
    const AstUdpValue* v ///< [in] テーブルの値
  );

  /// @brief module を表示する
  void
  put(
    const AstModule* m ///< [in] モジュール
  );

  /// @brief IO のヘッダを出力する．
  void
  put(
    const char* label, ///< [in] ラベル
    const AstIOHead* io ///< [in] IOヘッダ
  );

  /// @brief 宣言のヘッダを出力する．
  void
  put(
    const char* label,     ///< [in] ラベル
    const AstDeclHead* decl ///< [in] 宣言ヘッダ
  );

  /// @brief 要素の出力
  void
  put(
    const char* label, ///< [in] ラベル
    const AstItem* item ///< [in] 要素
  );

  /// @brief ステートメントの出力
  void
  put(
    const char* label, ///< [in] ラベル
    const AstStmt* stmt ///< [in] ステートメント
  );

  /// @brief 式の内容を出力する
  void
  put(
    const char* label, ///< [in] ラベル
    const AstExpr* expr ///< [in] 式
  );


public:

  /// @brief ファイル位置情報を出力するモードににする
  void
  enable_file_loc_mode();

  /// @brief ファイル位置情報の出力を抑制するモードにする
  void
  disable_file_loc_mode();

  /// @brief ファイル位置情報の出力モードの取得
  /// @return ファイル位置情報を出力するモードのとき true を返す．
  bool
  file_loc_mode() const;


private:

  /// @brief インクルード元のファイル情報の出力
  void
  put_parent_file(
    const FileLoc& file_loc ///< [in] ファイルの位置情報
  );

  /// @brief ポート結線式を出力する．
  void
  put_portref(
    const AstExpr* expr,
    VpiDir dir
  );

  /// @brief 宣言を出力する．
  void
  put_decls(
    const AstIOHeadVec& iohead_list,    ///< [in] IO宣言のリスト
    const AstDeclHeadVec& declhead_list ///< [in] 宣言のリスト
  );

  /// @brief 宣言と要素を出力する．
  void
  put_decl_item(
    const char* label,                  ///< [in] ラベル
    const AstDeclHeadVec& decl_list,    ///< [in] 宣言のリスト
    const AstItemVec& item_list         ///< [in] 要素のリスト
  );

  /// @brief 階層名の出力
  void
  put(
    const AstNameBranchList& nb_list
  );

  /// @brief ブール型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    bool b             ///< [in] ブール値
  );

  /// @brief 整数型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    int d              ///< [in] 整数値
  );

  /// @brief 符号なし整数型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    std::uint32_t d         ///< [in] 整数値
  );

  /// @brief ビットベクタ型データの出力
  void
  put(
    const char* label,  ///< [in] ラベル
    const BitVector& bv ///< [in] 整数値
  );

  /// @brief 実数型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    double r           ///< [in] 実数値
  );

  /// @brief 文字列型データの出力
  void
  put(
    const char* label, ///< [in] ラベル
    const char* str    ///< [in] 文字列
  );

  /// @brief 文字列型データの出力
  void
  put(
    const char* label,     ///< [in] ラベル
    const std::string& str ///< [in] 文字列
  );

  /// @brief 補助的なデータ型の出力
  void
  put(
    const char* label,  ///< [in] ラベル
    VpiAuxType aux_type ///< [in] 補助的なデータ型
  );

  /// @brief ネット型の出力
  void
  put(
    const char* label,  ///< [in] ラベル
    VpiNetType net_type ///< [in] ネット型
  );

  /// @brief 変数型の出力
  void
  put(
    const char* label,  ///< [in] ラベル
    VpiVarType var_type ///< [in] 変数型
  );

  /// @brief 方向の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiDir direction   ///< [in] 方向
  );

  /// @brief unconnected drive の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiUnconnDrive ud  ///< [in] unconnected drive
  );

  /// @brief default delay mode の出力
  void
  put(
    const char* label,         ///< [in] ラベル
    VpiDefDelayMode delay_mode ///< [in] default delay mode
  );

  /// @brief primitive型の出力
  void
  put(
    const char* label,    ///< [in] ラベル
    VpiPrimType prim_type ///< [in] primitive 型
  );

  /// @brief 演算子型の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiOpType op_type  ///< [in] 演算子型
  );

  /// @brief 定数型の出力
  void
  put(
    const char* label,      ///< [in] ラベル
    VpiConstType const_type ///< [in] 定数型
  );

  /// @brief strength の出力
  void
  put(
    const char* label,   ///< [in] ラベル
    VpiStrength strength ///< [in] strength
  );

  /// @brief vectored/scalared 属性の出力
  void
  put(
    const char* label, ///< [in] ラベル
    VpiVsType vs_type  ///< [in] vectored/scalared 属性
  );

  /// @brief 範囲指定モードの出力
  void
  put(
    const char* label,      ///< [in] ラベル
    VpiRangeMode range_mode ///< [in] 範囲指定モード
  );

  /// @brief FileLoc の情報を出力する．
  void
  put(
    const char* label,      ///< [in] ラベル
    const FileLoc& file_loc ///< [in] ファイルの位置情報
  );

  /// @brief FileRegion の情報を出力する．
  void
  put(
    const char* label,         ///< [in] ラベル
    const FileRegion& file_loc ///< [in] ファイル領域の情報
  );

  /// @brief 属性インスタンスを出力する．
  void
  put(
    const char* label,         ///< [in] ラベル
    const AstAttrInst* attr_top ///< [in] 属性インスタンスの先頭
  );

  /// @brief コントロールの内容を表示する．
  void
  put(
    const char* label,    ///< [in] ラベル
    const AstControl* ctrl ///< [in] コントロール
  );

  /// @brief strength の内容を表示する．
  void
  put(
    const char* label,    ///< [in] ラベル
    const AstStrength* str ///< [in] 信号強度
  );

  /// @brief 遅延情報を出力する．
  void
  put(
    const char* label,   ///< [in] ラベル
    const AstDelay* delay ///< [in] 遅延
  );

  /// @brief 接続情報を表示する．
  void
  put(
    const char* label,      ///< [in] ラベル
    const AstConnection* con ///< [in] 接続情報
  );

  /// @brief 階層ブランチの情報を出力する
  void
  put(
    const char* label,     ///< [in] ラベル
    const AstNameBranch* nb ///< [in] 階層ブランチ
  );

  /// @brief 開始マーカを出力する．
  void
  put_begin(
    const char* label,   ///< [in] マーカのラベル
    const char* type,    ///< [in] タイプ
    bool need_cr = true  ///< [in] true の時はマーカ出力後に改行する．
  );

  /// @brief 終了マーカを出力する．
  void
  put_end(
    const char* label ///< [in] マーカのラベル
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  std::ostream& mStream;

  // マーカ出力後に改行したことを示すフラグ
  std::vector<bool> mDoCR;

  // インデント量
  int mIndent;

  // ファイル位置情報の出力フラグ
  bool mFileLocMode;

};

END_NAMESPACE_YM_VERILOG

#endif // ASTDUMPER_H
