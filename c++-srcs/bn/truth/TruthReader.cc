
/// @file TruthReader.cc
/// @brief TruthReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "TruthReader.h"
#include "ym/BnModel.h"
#include "ym/TvFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////


// @brief .truth 形式のファイルを読み込む．
// @return ネットワークを返す．
BnModel
BnModel::read_truth(
  const std::string& filename ///< [in] ファイル名
)
{
  std::ifstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": No such file";
    throw std::invalid_argument{buf.str()};
  }

  BnModel model;
  TruthReader reader;
  reader.read(s, model._model_impl());
  return model;
}


//////////////////////////////////////////////////////////////////////
// クラス TruthReader
//////////////////////////////////////////////////////////////////////

// @brief 真理値表形式のファイルを読み込む．
void
TruthReader::read(
  std::istream& s,
  ModelImpl& model
)
{
  std::vector<TvFunc> func_vect;
  {
    std::string line;
    while ( getline(s, line) ) {
      func_vect.push_back(TvFunc{line});
    }
  }

  auto no = func_vect.size();
  if ( no == 0 ) {
    return;
  }

  // 入力数が全て等しいかチェック
  auto ni = func_vect[0].input_num();
  for ( SizeType i = 1; i < no; ++ i ) {
    auto& func = func_vect[i];
    if ( func.input_num() != ni ) {
      std::cout << "ni = " << ni << ", func_vect["
		<< i << "].input_num() = "
		<< func.input_num()
		<< std::endl;
      throw std::invalid_argument{"the number of inputs should be the same for all outputs"};
    }
  }

  // 入力の生成
  for ( SizeType i = 0; i < ni; ++ i ) {
    model.new_input();
  }

  // 論理ノードの生成
  // 注意が必要なのは .truth フォーマットでは最上位の変数が
  // 最後の変数だということ．
  std::vector<SizeType> fanin_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    fanin_list[i] = ni - i - 1;
  }
  for ( auto& tvfunc: func_vect ) {
    auto func_id = model.reg_tvfunc(tvfunc);
    auto id = model.new_logic(func_id, fanin_list);
    model.new_output(id);
  }
  model.make_logic_list();
}

END_NAMESPACE_YM_BN
