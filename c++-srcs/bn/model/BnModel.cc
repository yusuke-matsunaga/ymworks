
/// @file BnModel.cc
/// @brief BnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

BEGIN_NONAMESPACE

inline
std::vector<SizeType>
make_id_list(
  SizeType size
)
{
  std::vector<SizeType> id_list(size);
  for ( SizeType i = 0; i < size; ++ i ) {
    id_list[i] = i;
  }
  return id_list;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス BnModel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BnModel::BnModel(
) : BnModel(new ModelImpl)
{
}

// @brief 内容を指定したコンストラクタ
BnModel::BnModel(
  ModelImpl* impl
) : BnBase(std::shared_ptr<ModelImpl>{impl})
{
}

// @brief デストラクタ
BnModel::~BnModel()
{
}

// @brief '深い'コピーを作る．
BnModel
BnModel::copy() const
{
  auto impl = _model_impl().copy();
  return BnModel(impl);
}

// @brief DFF数を返す．
SizeType
BnModel::dff_num() const
{
  return _model_impl().dff_num();
}

// @brief DFFを返す．
BnDff
BnModel::dff(
  SizeType dff_id
) const
{
  return _id2dff(dff_id);
}

// @brief DFFのリストを返す．
BnDffList
BnModel::dff_list() const
{
  return _id2dff_list(make_id_list(dff_num()));
}

// @brief ノード数を返す．
SizeType
BnModel::node_num() const
{
  return _model_impl().node_num();
}

// @brief ノードを返す．
BnNode
BnModel::node(
  SizeType id
) const
{
  return _id2node(id);
}

// @brief 入力数を返す．
SizeType
BnModel::input_num() const
{
  return _model_impl().input_num();
}

// @brief 入力のノードを返す．
BnNode
BnModel::input(
  SizeType input_id
) const
{
  _check_input_id(input_id);
  auto id = _model_impl().input_id(input_id);
  return _id2node(id);
}

// @brief 入力のノードのリストを返す．
BnNodeList
BnModel::input_list() const
{
  return _id2node_list(_model_impl().input_id_list());
}

// @brief 出力数を返す．
SizeType
BnModel::output_num() const
{
  return _model_impl().output_num();
}

// @brief 入力のノードを返す．
BnNode
BnModel::output(
  SizeType output_id
) const
{
  _check_output_id(output_id);
  auto id = _model_impl().output_id(output_id);
  return _id2node(id);
}

// @brief 出力のノードのリストを返す．
BnNodeList
BnModel::output_list() const
{
  return _id2node_list(_model_impl().output_id_list());
}

// @brief 論理ノード数を返す．
SizeType
BnModel::logic_num() const
{
  return _model_impl().logic_num();
}

// @brief 論理ノードを返す．
BnNode
BnModel::logic(
  SizeType pos
) const
{
  _check_logic_id(pos);
  auto id = _model_impl().logic_id(pos);
  return _id2node(id);
}

// @brief 論理ノードのリストを返す．
BnNodeList
BnModel::logic_list() const
{
  return _id2node_list(_model_impl().logic_id_list());
}

// @brief 関数情報の数を返す．
SizeType
BnModel::func_num() const
{
  return _model_impl().func_num();
}

// @brief 関数情報を取り出す．
BnFunc
BnModel::func(
  SizeType func_id
) const
{
  if ( func_id < 0 || func_num() <= func_id ) {
    std::ostringstream buf;
    buf << "'func_id'(" << func_id << ") is out of range";
    throw std::out_of_range{buf.str()};
  }
  return _id2func(func_id);
}

// @brief オプション情報を表す JSON オブジェクトを返す．
JsonValue
BnModel::option() const
{
  return _model_impl().option();
}

// @brief 名前を返す．
std::string
BnModel::name() const
{
  return _model_impl().name();
}

// @brief コメントを返す．
const std::vector<std::string>&
BnModel::comment_list() const
{
  return _model_impl().comment_list();
}

// @brief 入力名を返す．
std::string
BnModel::input_name(
  SizeType input_id
) const
{
  _check_input_id(input_id);
  return _model_impl().input_name(input_id);
}

// @brief 出力名を返す．
std::string
BnModel::output_name(
  SizeType output_id
) const
{
  _check_output_id(output_id);
  return _model_impl().output_name(output_id);
}

// @brief DFF名を返す．
std::string
BnModel::dff_name(
  SizeType dff_id
) const
{
  _check_dff_id(dff_id);
  return _model_impl().dff_name(dff_id);
}

// @brief 内容を出力する．
void
BnModel::write(
  std::ostream& s
) const
{
  _model_impl().write(s);
}

// @brief ファイルの読み込みを行う．
BnModel
BnModel::read(
  const std::string& filename,
  const std::string& format
)
{
  auto format1 = format;
  if ( format1.empty() ) {
    std::filesystem::path path{filename};
    auto ext = path.extension();
    if ( ext == ".blif" ) {
      format1 = "blif";
    }
    else if ( ext == ".bench" ) {
      format1 = "iscas89";
    }
    else if ( ext == ".truth" ) {
      format1 = "truth";
    }
    else {
      std::ostringstream buf;
      buf << ext << ": unknown extensition for BnModel::read()";
      throw std::invalid_argument{buf.str()};
    }
  }

  if ( format1 == "blif" ) {
    return read_blif(filename);
  }
  if ( format1 == "iscas89" ) {
    return read_iscas89(filename);
  }
  if ( format1 == "truth" ) {
    return read_truth(filename);
  }

  std::ostringstream buf;
  buf << format1 << ": unknown format for BnModel::read()";
  throw std::invalid_argument{buf.str()};
}


//////////////////////////////////////////////////////////////////////
// クラス BnDff
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
BnDff::BnDff(
  const std::shared_ptr<ModelImpl>& model,
  SizeType id
) : BnBase(model),
    mId{id}
{
  if ( is_invalid() ) {
    mId = BAD_ID;
  }
}

// @brief デストラクタ
BnDff::~BnDff()
{
}

// @brief 名前を返す．
const std::string&
BnDff::name() const
{
  auto& dff = _dff_impl();
  return dff.name;
}

// @brief 出力ノードを返す．
BnNode
BnDff::output() const
{
  auto& dff = _dff_impl();
  return _id2node(dff.id);
}

// @brief 入力ノードを返す．
BnNode
BnDff::input() const
{
  auto& dff = _dff_impl();
  return _id2node(dff.src_id);
}

// @brief リセット値
char
BnDff::reset_val() const
{
  auto& dff = _dff_impl();
  return dff.reset_val;
}

// @brief DFFの実体を返す．
const DffImpl&
BnDff::_dff_impl() const
{
  if ( !is_valid() ) {
    throw std::logic_error{"BnDff: invalid data"};
  }
  return _model_impl().dff_impl(mId);
}

END_NAMESPACE_YM_BN
