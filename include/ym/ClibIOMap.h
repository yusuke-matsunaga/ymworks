#ifndef CLIBIOMAP_H
#define CLIBIOMAP_H

/// @file ClibIOMap.h
/// @brief ClibIOMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibPinMap.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class ClibIOMap ClibIOMap.h "ClibIOMap.h"
/// @brief セルのピンの割当（変換）方法を表すクラス
/// @sa ClibPinMap
///
/// - 入力ピン，出力ピン，入出力ピンそれぞれの順序および極性の情報を持つ．
/// - ただし，入出力ピンに関しては入力と出力でおなじ順序，極性となる．
/// - 一つのピンに対する順序と極性の情報は ClibPinMap で表す．
/// - ClibPinMap は，変換後のピンに対するもとのピン番号と極性を表している．
/// - これは入力でも出力でも同じなので変換回路として考えると逆向きになる
///   ことに注意．実際にはこういう定義にしないと入出力ピンの変換を表せない．
/// - 具体的に input_map(0) の結果が {3, false} だった場合，変換後の
///   0 番目のピンはもとの 3 番目の入力ピンとなる．
///   output_map(0) の結果が {4, true} だった場合，変換後の 0 番目の
///   ピンはもとの 4 番目の出力ピンを反転させたものとなる．
/// - こうすることで，複数の変換に対して結合則が成り立つ．
//////////////////////////////////////////////////////////////////////
class ClibIOMap
{
public:

  /// @brief 空のコンストラクタ
  ClibIOMap() = default;

  /// @brief 1出力用のコンストラクタ
  ///
  /// - 出力ピンのマッピングは極性のみが意味を持つ．
  explicit
  ClibIOMap(
    const std::vector<ClibPinMap>& input_map, ///< [in] 入力ピンのマッピング
    bool oinv = false                         ///< [in] 出力ピンの反転属性
  ) : mInputMap{input_map},
      mOutputMap{ClibPinMap{0, oinv}}
  {
  }

  /// @brief 多出力用のコンストラクタ
  ClibIOMap(
    const std::vector<ClibPinMap>& input_map, ///< [in] 入力ピンのマッピング
    const std::vector<ClibPinMap>& output_map ///< [in] 出力ピンのマッピング
  ) : mInputMap{input_map},
      mOutputMap{output_map}
  {
  }

  /// @brief 入出力ピンを持つ場合のコンストラクタ
  ClibIOMap(
    const std::vector<ClibPinMap>& input_map,  ///< [in] 入力ピンのマッピング
    const std::vector<ClibPinMap>& output_map, ///< [in] 出力ピンのマッピング
    const std::vector<ClibPinMap>& inout_map   ///< [in] 入出力ピンのマッピング
  ) : mInputMap{input_map},
      mOutputMap{output_map},
      mInoutMap{inout_map}
  {
  }

  /// @brief 恒等変換を作るクラス関数
  static
  ClibIOMap
  identity(
    SizeType ni,     ///< [in] 入力数
    SizeType no = 1, ///< [in] 出力数
    SizeType nb = 0  ///< [in] 入出力数
  )
  {
    std::vector<ClibPinMap> input_map(ni);
    for ( SizeType i = 0; i < ni; ++ i ) {
      input_map[i] = ClibPinMap(i);
    }
    std::vector<ClibPinMap> output_map(no);
    for ( SizeType i = 0; i < no; ++ i ) {
      output_map[i] = ClibPinMap(i);
    }
    std::vector<ClibPinMap> inout_map(nb);
    for ( SizeType i = 0; i < nb; ++ i ) {
      inout_map[i] = ClibPinMap(i);
    }
    return ClibIOMap{input_map, output_map, inout_map};
  }

  /// @brief デストラクタ
  ~ClibIOMap() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  SizeType
  input_num() const
  {
    return mInputMap.size();
  }

  /// @brief 入力のマッピングを返す．
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPinMap
  ClibPinMap
  input_map(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < input_num() + inout_num() )
  ) const
  {
    if ( pos < 0 || input_num() + inout_num() <= pos ) {
      throw std::out_of_range("out of range");
    }
    if ( pos < input_num() ) {
      return mInputMap[pos];
    }
    else {
      return mInoutMap[pos - input_num()];
    }
  }

  /// @brief 入力のマッピングのリストを返す．
  /// @sa ClibPinMap
  const std::vector<ClibPinMap>&
  input_map_list() const
  {
    return mInputMap;
  }

  /// @brief 出力数を返す．
  SizeType
  output_num() const
  {
    return mOutputMap.size();
  }

  /// @brief 出力のマッピングを返す．
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPinMap
  ClibPinMap
  output_map(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < output_num() + inout_num() )
  ) const
  {
    if ( pos < 0 || output_num() + inout_num() <= pos ) {
      throw std::out_of_range("out of range");
    }
    if ( pos < output_num() ) {
      return mOutputMap[pos];
    }
    else {
      return mInoutMap[pos - output_num()];
    }
  }

  /// @brief 出力のマッピングのリストを返す．
  /// @sa ClibPinMap
  const std::vector<ClibPinMap>&
  output_map_list() const
  {
    return mOutputMap;
  }

  /// @brief 入出力数を返す．
  SizeType
  inout_num() const
  {
    return mInoutMap.size();
  }

  /// @brief 入出力のマッピングを返す．
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibPinMap
  ClibPinMap
  inout_map(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos && pos < inout_num() )
  ) const
  {
    if ( pos < 0 || inout_num() <= pos ) {
      throw std::out_of_range("out of range");
    }
    return mInoutMap[pos];
  }

  /// @brief 入出力のマッピングのリストを返す．
  /// @sa ClibPinMap
  const std::vector<ClibPinMap>&
  inout_map_list() const
  {
    return mInoutMap;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 演算の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 合成演算
  ClibIOMap
  operator*(
    const ClibIOMap& right
  ) const;

  /// @brief 合成演算(intern演算)
  ClibIOMap&
  operator*=(
    const ClibIOMap& right
  );

  /// @brief 逆写像演算
  ClibIOMap
  inverse() const;

  /// @brief 等価比較演算
  bool
  operator==(
    const ClibIOMap& right
  ) const;

  /// @brief 非等価比較演算
  bool
  operator!=(
    const ClibIOMap& right
  ) const
  {
    return !operator==(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリダンプを行う．
  /// @sa BinEnc
  void
  dump(
    BinEnc& bos ///< [in] 出力先のストリーム
  ) const;

  /// @brief バイナリダンプされた内容を読み込む．
  /// @sa BinDec
  void
  restore(
    BinDec& bis ///< [in] 入力元のストリーム
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピンのマッピング
  std::vector<ClibPinMap> mInputMap;

  // 出力ピンのマッピング
  std::vector<ClibPinMap> mOutputMap;

  // 入出力ピンのマッピング
  std::vector<ClibPinMap> mInoutMap;

};

/// @relates ClibIOMap
/// @brief ClibIOMap のストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,       ///< [in] 出力先のストリーム
  const ClibIOMap& iomap ///< [in] 入出力のマッピング
);

END_NAMESPACE_YM_CLIB

#endif // CLIBIOMAP_H
