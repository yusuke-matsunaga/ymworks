#ifndef XFORMINFO_H
#define XFORMINFO_H

/// @file XformInfo.h
/// @brief XformInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/TruthTable.h"


BEGIN_NAMESPACE_YM_EXSYN

//////////////////////////////////////////////////////////////////////
/// @class XformInfo XformInfo.h "XformInfo.h"
/// @brief TruthTable の正規化情報を表すクラス
///
/// 正規化は以下の手順で行う．
/// - 定数0・定数１の出力を求める．
///   ドントケアを含む場合もあるので判断基準は定数0から1を含まないこと．
/// - リテラル関数を求める．
///   i 番目の入力と一致するかどうかを調べる．
///   定数の場合と同じでドントケアを含んでもよい．
/// - 支配されている出力を求める．
///   出力 i の値が出力 j の値と等しいか，ドントケアであるとき，
///   出力 i は出力 j に支配されると定義する．
///   明らかに支配されている出力は支配している出力と同一の演算で実現できる．
/// - 上記の処理で見つかった出力は残りの処理対象から外す．
/// - サポートとなっている入力を求める．
///   具体的にはi番目の入力を反転させた時に出力が異なる場合に
///   サポートとなる．
///   サポートは各出力ごとに定義されるが，ここでは個々のサポートの
///   ユニオンを求める．
/// - 以降の処理ではサポートでない入力は除外する．
/// - ピボットを求める．
///   ピボットとはドントケアとなる出力の個数が最も少ない入力のこと．
///   複数ある場合は（どれを選んでもよいが）2進数に変換した時もっとも
///   小さな数になるものを用いる．
///   多くの場合は (0, 0, ..., 0) となる．
/// - ピボットの入力に対する出力が0になるように出力の反転属性を決める．
///   ドントケアの場合には決められないので双極性フラグを立てておく．
/// - ピボットの入力が(0, 0, ..., 0)になるように入力の反転属性を決める．
///   具体的にはピボットの入力のうち，1の入力を反転すればよい．
/// - もとの関数から定数・リテラル関数の出力を取り除き，サポートでない入力
///   を取り除いたものに対して，入力と出力の反転を行った関数を生成する．
/// - Exact Synthesis はこの関数に対して処理を行う．
/// - Exact Synthesis の結果から得られた回路に対して上記の変換の逆変換
///   を施す．
//////////////////////////////////////////////////////////////////////
class XformInfo
{
public:

  // 入力に関する情報を表す列挙型
  enum InputInfo {
    NOINV, ///< [in] 通常の入力
    INV,   ///< [in] 反転入力
    UNUSED ///< [in] サポートでない入力
  };

  // 出力に関する情報を表す列挙型
  enum OutputInfo {
    ZERO,      ///< [in] 定数0
    ONE,       ///< [in] 定数1
    POSI_LIT,  ///< [in] 通常の入力
    NEGA_LIT,  ///< [in] 反転した入力
    NOINV_OP,  ///< [in] 通常の演算ノード
    INV_OP,    ///< [in] 反転した演算ノード
    NOINV_DOM, ///< [in] 支配されている出力
    INV_DOM    ///< [in] 反転支配されている出力
  };


public:

  /// @brief コンストラクタ
  XformInfo() = default;

  /// @brief TruthTable を指定したコンストラクタ
  XformInfo(
    const TruthTable& tt ///< [in] 元の関数
  );

  /// @brief デストラクタ
  ~XformInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputInfoArray.size();
  }

  /// @brief 入力の情報を返す．
  InputInfo
  input_info(
    SizeType i_id ///< [in] 入力番号 ( 0 <= i_id < input_num() )
  ) const
  {
    if ( i_id >= input_num() ) {
      throw std::out_of_range{"i_id is out of range"};
    }
    return mInputInfoArray[i_id];
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputInfoArray.size();
  }

  /// @brief 出力の情報を返す．
  OutputInfo
  output_info(
    SizeType o_id ///< [in] 出力番号 ( 0 <= o_id < output_num() )
  ) const
  {
    if ( o_id >= output_num() ) {
      throw std::out_of_range{"o_id is out of range"};
    }
    return mOutputInfoArray[o_id];
  }

  /// @brief 出力がリテラル関数の時入力番号を返す．
  ///
  /// リテラル関数でない時は例外を送出する．
  SizeType
  output_literal(
    SizeType o_id ///< [in] 出力番号 ( 0 <= o_id < output_num() )
  ) const
  {
    if ( o_id >= output_num() ) {
      throw std::out_of_range{"o_id is out of range"};
    }
    auto o_info = mOutputInfoArray[o_id];
    if ( o_info != POSI_LIT && o_info != NEGA_LIT ) {
      std::ostringstream buf;
      buf << "O#" << o_id << " is not a literal function";
      throw std::invalid_argument{buf.str()};
    }
    return mLiteralArray[o_id];
  }

  /// @brief 正規化された関数を返す．
  const TruthTable&
  normal_func() const
  {
    return mNormalFunc;
  }

  /// @brief 正規化された関数の入力番号を元の入力番号に変換する．
  SizeType
  input_map(
    SizeType i_id
  ) const
  {
    if ( i_id >= mInputList.size() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return mInputList[i_id];
  }

  /// @brief 元の関数の出力番号を正規化された関数の出力番号に変換する．
  SizeType
  output_map(
    SizeType o_id
  ) const
  {
    if ( o_id >= mOutputMap.size() ) {
      throw std::out_of_range{"o_id is out of range"};
    }
    auto o_info = mOutputInfoArray[o_id];
    if ( o_info != NOINV_OP && o_info != INV_OP ) {
      std::ostringstream buf;
      buf << "O#" << o_id << " is not an OP output";
      throw std::invalid_argument{buf.str()};
    }
    return mOutputMap[o_id];
  }

  /// @brief BcGraph を作る．
  ///
  /// normal_func() が空の時のみ有効
  BcGraph
  make_graph() const
  {
    return denormalize(BcGraph());
  }

  /// @brief normal_func 用の BcGraph を元に戻す．
  BcGraph
  denormalize(
    const BcGraph& normal_graph
  ) const;

  /// @brief 内容を表す文字列を返す．
  std::string
  to_string() const
  {
    std::ostringstream buf;

    // 入力の情報
    for ( auto iinfo: mInputInfoArray ) {
      switch ( iinfo ) {
      case NOINV:  buf << '0'; break;
      case INV:    buf << '1'; break;
      case UNUSED: buf << '-'; break;
      }
    }
    // 出力の情報
    buf << "|";
    SizeType no = output_num();
    for ( SizeType o = 0; o < no; ++ o ) {
      switch ( mOutputInfoArray[o] ) {
      case ZERO:
	buf << "C0";
	break;
      case ONE:
	buf << "C1";
	break;
      case POSI_LIT:
	buf << 'I' << mLiteralArray[o];
	break;
      case NEGA_LIT:
	buf << 'i' << mLiteralArray[o];
	break;
      case NOINV_OP:
	buf << "+X";
	break;
      case INV_OP:
	buf << "-X";
	break;
      case NOINV_DOM:
	buf << "D" << mDomArray[o];
	break;
      case INV_DOM:
	buf << "d" << mDomArray[o];
	break;
      }
    }
    return buf.str();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変換情報を参照して枝を作る．
  BcEdge
  make_edge(
    BcEdge src_edge
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力の情報を持つ配列
  std::vector<InputInfo> mInputInfoArray;

  // 出力の情報を持つ配列
  std::vector<OutputInfo> mOutputInfoArray;

  // リテラル関数になっている出力の入力番号の値を持つ．
  // リテラル関数ではない時の値は意味を持たない．
  std::vector<SizeType> mLiteralArray;

  // 支配している出力番号を格納する配列
  std::vector<SizeType> mDomArray;

  // 正規化された関数の出力番号を持つ配列
  // 正規化された関数に現れない出力の値は意味を持たない．
  std::vector<SizeType> mOutputMap;

  // サポートとなっている入力番号のリスト
  std::vector<SizeType> mInputList;

  // 正規化された関数
  TruthTable mNormalFunc;

};

inline
std::ostream&
operator<<(
  std::ostream& s,
  const XformInfo& xf
)
{
  s << xf.to_string();
  return s;
}

inline
std::ostream&
operator<<(
  std::ostream& s,
  XformInfo::InputInfo iinfo
)
{
  switch ( iinfo ) {
  case XformInfo::NOINV:  s << "NOINV"; break;
  case XformInfo::INV:    s << "INV"; break;
  case XformInfo::UNUSED: s << "UNUSED"; break;
  }
  return s;
}

inline
std::ostream&
operator<<(
  std::ostream& s,
  XformInfo::OutputInfo oinfo
)
{
  switch ( oinfo ) {
  case XformInfo::ZERO:      s << "ZERO"; break;
  case XformInfo::ONE:       s << "ONE"; break;
  case XformInfo::POSI_LIT:  s << "POSI_LIT"; break;
  case XformInfo::NEGA_LIT:  s << "NEGA_LIT"; break;
  case XformInfo::NOINV_OP:  s << "NOINV_OP"; break;
  case XformInfo::INV_OP:    s << "INV_OP"; break;
  case XformInfo::NOINV_DOM: s << "NOINV_DOM"; break;
  case XformInfo::INV_DOM:   s << "INV_DOM"; break;
  }
  return s;
}

END_NAMESPACE_YM_EXSYN

#endif // XFORMINFO_H
