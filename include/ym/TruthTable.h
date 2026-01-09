#ifndef TRUTHTABLE_H
#define TRUTHTABLE_H

/// @file TruthTable.h
/// @brief TruthTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/Val3.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TruthTable TruthTable.h "ym/TruthTable.h"
/// @brief 真理値表を表すクラス
///
/// 多入力多出力の不完全指定論理関数を表す．
/// 具体的には 2^input_num() * output_num() 個の論理値(Val3: 0, 1, X)の
/// ベクタを用いる．
/// 真理値表は0 番目の入力が左端に来るので左端をMSB，右端をLSBとする．
/// そのためインデックスは Sum_2^(ni - i - 1) の形で表される．
///
/// 内部では Val3 を2ビットに符号化して表す．
//////////////////////////////////////////////////////////////////////
class TruthTable
{
public:

  /// @brief 空のコンストラクタ
  TruthTable() = default;

  /// @brief サイズを指定したコンストラクタ
  ///
  /// 全てドントケアで初期化される．
  TruthTable(
    SizeType input_num, ///< [in] 入力数
    SizeType output_num ///< [in] 出力数
  ) : mInputNum{input_num},
      mOutputNum{output_num},
      mValue0((1 << input_num) * output_num, true),
      mValue1((1 << input_num) * output_num, true)
  {
  }

  /// @brief ファイルを読み込む．
  /// @return 結果の TruthTable を返す．
  ///
  /// ファイルの形式は IWLS2023 のベンチマークの形式
  /// エラーの場合は std::invalid_argument を返す．
  static
  TruthTable
  read_file(
    const std::string& filename ///< [in] ファイル名
  );

  /// @brief BIN 形式の文字列から変換する．
  /// @return 結果の TruthTable を返す．
  ///
  /// 文字列は <ni>':'<no>':'<bin_str> の形式を仮定する．
  /// <bin_str> の長さは (2^<ni>) * <no>)
  /// ただし，一つの出力ごとに区切って2進文字列に
  /// 変換する．
  /// 使用可能な文字は '0', '1', '-', '*'
  /// エラーの場合は std::invalid_argument を返す．
  static
  TruthTable
  from_bin(
    const std::string& str ///< [in] 文字列
  );

  /// @brief HEX 形式の文字列から変換する．
  /// @return 結果の TruthTable を返す．
  ///
  /// 文字列は <ni>':'<no>':'<hex_str> の形式を仮定する．
  /// <hex_str> の長さは ((2^<ni>) * <no>) / 4
  /// ただし，一つの出力ごとに区切って16進文字列に
  /// 変換する．
  /// なので1入力以下の関数は表せない．
  /// エラーの場合は std::invalid_argument を返す．
  static
  TruthTable
  from_hex(
    const std::string& str ///< [in] 文字列
  );

  /// @brief デストラクタ
  ~TruthTable() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputNum;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputNum;
  }

  /// @brief 入力インデックスを作る．
  SizeType
  encode_index(
    const std::vector<bool> input_vals ///< [in] 入力値のベクタ
  ) const;

  /// @brief 入力インデックスから入力値ベクタを作る．
  std::vector<bool>
  decode_index(
    SizeType i_index ///< [in] 入力インデックス
  ) const;

  /// @brief 値を読み出す．
  /// @return 出力数分のベクタを返す．
  Val3
  get_value(
    SizeType i_index, ///< [in] 入力インデックス ( 0 <= i_index < 2^input_num() )
    SizeType o_index  ///< [in] 出力インデックス ( 0 <= o_index < output_num() )
  ) const
  {
    auto offset = i_index * output_num();
    auto v0 = mValue0[offset + o_index];
    auto v1 = mValue1[offset + o_index];
    if ( v0 == false ) {
      return Val3::ONE;
    }
    if ( v1 == false ) {
      return Val3::ZERO;
    }
    return Val3::X;
  }

  /// @brief 値をセットする．
  void
  set_value(
    SizeType i_index, ///< [in] 入力インデックス ( 0 <= i_index < 2^input_num() )
    SizeType o_index, ///< [in] 出力インデックス ( 0 <= o_index < output_num() )
    Val3 val          ///< [in] 値
  )
  {
    auto offset = i_index * output_num() + o_index;
    switch ( val ) {
    case Val3::ZERO:
      mValue0[offset] = true;
      mValue1[offset] = false;
      break;
    case Val3::ONE:
      mValue0[offset] = false;
      mValue1[offset] = true;
      break;
    case Val3::X:
      mValue0[offset] = true;
      mValue1[offset] = true;
      break;
    }
  }

  /// @brief サポートを得る．
  /// @return サポートかどうかのビットベクタを返す．
  std::vector<bool>
  get_support(
    SizeType o_index ///< [in] 出力番号 ( 0 <= o_index < output_num() )
  ) const;

  /// @brief 内容を出力する．
  void
  print(
    std::ostream& s ///< [in] 出力ストリーム
  ) const;

  /// @brief 内容を表すBIN文字列を返す．
  ///
  /// from_bin() で用いる文字列と同一の形式
  std::string
  bin_str() const;

  /// @brief 内容を表すHEX文字列を返す．
  ///
  /// from_hex() で用いる文字列と同一の形式
  std::string
  hex_str() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  SizeType mInputNum{0};

  // 出力数
  SizeType mOutputNum{0};

  // 0 のビットベクタ
  // サイズは 2^mInputNum * mOutputNum ビット
  std::vector<bool> mValue0;

  // 1 のビットベクタ
  // サイズは 2^mInputNum * mOutputNum ビット
  std::vector<bool> mValue1;

};

END_NAMESPACE_YM

#endif // TRUTHTABLE_H
