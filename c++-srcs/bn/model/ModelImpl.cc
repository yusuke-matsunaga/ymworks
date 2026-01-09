
/// @file ModelImpl.cc
/// @brief ModelImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ModelImpl.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_BN

// @brief コンストラクタ
ModelImpl::ModelImpl()
{
}

// @brief コピーコンストラクタ
ModelImpl::ModelImpl(
  const ModelImpl& src
) : mName{src.mName},
    mCommentList{src.mCommentList},
    mNodeArray(src.mNodeArray.size()),
    mInputList{src.mInputList},
    mOutputList{src.mOutputList},
    mDffList{src.mDffList},
    mLogicList{src.mLogicList},
    mNameDict{src.mNameDict},
    mFuncMgr{src.mFuncMgr}
{
  for ( SizeType i = 0; i < src.mNodeArray.size(); ++ i ) {
    mNodeArray[i] = src.mNodeArray[i]->copy();
  }
}

// @brief デストラクタ
ModelImpl::~ModelImpl()
{
}

// @brief オプション情報を返す．
JsonValue
ModelImpl::option() const
{
  std::unordered_map<std::string, JsonValue> src_dict;
  if ( !name().empty() ) {
    src_dict.emplace("name", JsonValue{name()});
  }
  if ( !mCommentList.empty() ) {
    std::vector<JsonValue> json_list;
    json_list.reserve(mCommentList.size());
    for ( auto& comment: mCommentList ) {
      json_list.push_back(JsonValue{comment});
    }
    src_dict.emplace("comment", JsonValue{json_list});
  }
  {
    std::unordered_map<std::string, JsonValue> symbol_dict;
    for ( SizeType i = 0; i < input_num(); ++ i ) {
      auto name = input_name(i);
      if ( !name.empty() ) {
	std::ostringstream buf;
	buf << "i" << i;
	auto key = buf.str();
	symbol_dict.emplace(key, JsonValue(name));
      }
    }
    for ( SizeType i = 0; i < output_num(); ++ i ) {
      auto name = output_name(i);
      if ( !name.empty() ) {
	std::ostringstream buf;
	buf << "o" << i;
	auto key = buf.str();
	symbol_dict.emplace(key, JsonValue(name));
      }
    }
    for ( SizeType i = 0; i < dff_num(); ++ i ) {
      auto name = mDffList[i].name;
      if ( !name.empty() ) {
	std::ostringstream buf;
	buf << "q" << i;
	auto key = buf.str();
	symbol_dict.emplace(key, JsonValue(name));
      }
    }
    if ( !symbol_dict.empty() ) {
      src_dict.emplace("symbol_dict",
		       JsonValue(symbol_dict));
    }
  }
  return JsonValue(src_dict);
}

// @brief 内容をクリアする．
void
ModelImpl::clear()
{
  mName = std::string{};
  mCommentList.clear();
  mNodeArray.clear();
  mInputList.clear();
  mOutputList.clear();
  mOutputNameList.clear();
  mLogicList.clear();
  mNameDict.clear();
  mFuncMgr.clear();
}

BEGIN_NONAMESPACE

// symbol_dict のキーをデコードする．
inline
SizeType
decode(
  const std::string& key
)
{
  auto num_str = key.substr(1);
  SizeType num;
  std::stoi(num_str, &num);
  return num;
}

END_NONAMESPACE

// @brief オプション情報をセットする．
void
ModelImpl::set_option(
  const JsonValue& option
)
{
  if ( option.has_key("name") ) {
    set_name(option.at("name").get_string());
  }
  if ( option.has_key("comment") ) {
    auto js_comment = option.at("comment");
    if ( !js_comment.is_array() ) {
      throw std::logic_error{"something wrong"};
    }
    auto n = js_comment.size();
    for ( SizeType i = 0; i < n; ++ i ) {
      auto comment = js_comment[i].get_string();
      add_comment(comment);
    }
  }
  if ( option.has_key("symbol_dict") ) {
    for ( auto& p: option.item_list() ) {
      auto key = p.first;
      auto value = p.second.get_string();
      auto num = decode(key);
      if ( key[0] == 'i' ) {
	set_input_name(num, value);
      }
      else if ( key[0] == 'o' ) {
	set_output_name(num, value);
      }
      else if ( key[0] == 'q' ) {
	set_dff_name(num, value);
      }
      else {
	throw std::invalid_argument{"symbol_dict is broken"};
      }
    }
  }
}

// @brief 対応するID番号に入力用の印を付ける．
void
ModelImpl::set_input(
  SizeType id,
  const std::string& name
)
{
  if ( mNodeArray[id].get() != nullptr ) {
    throw std::invalid_argument{"id has already been used"};
  }
  auto iid = mInputList.size();
  auto node = NodeImpl::new_primary_input(iid);
  mNodeArray[id] = std::unique_ptr<NodeImpl>{node};
  mInputList.push_back(id);
  if ( name != "" ) {
    mNameDict.emplace(id, name);
  }
}


// @brief DFFの情報をセットする．
void
ModelImpl::set_dff_output(
  SizeType id,
  SizeType dff_id
)
{
  if ( mNodeArray[id].get() != nullptr ) {
    throw std::invalid_argument{"id has already been used"};
  }
  _check_dff_id(dff_id, "set_dff_output");
  auto node = NodeImpl::new_dff_output(dff_id);
  mNodeArray[id] = std::unique_ptr<NodeImpl>{node};
  mDffList[dff_id].id = id;
}

// @brief 論理ノードの情報をセットする．
void
ModelImpl::set_logic(
  SizeType id,
  SizeType func_id,
  const std::vector<SizeType>& fanin_list
)
{
  if ( mNodeArray[id].get() != nullptr ) {
    throw std::invalid_argument{"id has already been used"};
  }
  auto node = NodeImpl::new_logic(func_id, fanin_list);
  mNodeArray[id] = std::unique_ptr<NodeImpl>{node};
  // mLogicList には追加しない．
}

// @brief ノード名をセットする．
void
ModelImpl::set_node_name(
  SizeType id,
  const std::string& name
)
{
  mNameDict.emplace(id, name);
}

// @brief 論理ノードのリストを作る．
void
ModelImpl::make_logic_list()
{
  std::unordered_set<SizeType> mark;

  // 入力ノードに印をつける．
  for ( auto id: mInputList ) {
    mark.emplace(id);
  }

  // DFFの出力に印を作る．
  for ( auto& dff: mDffList ) {
    mark.emplace(dff.id);
  }

  // 出力ノードからファンインをたどり
  // post-order で番号をつける．
  // 結果としてノードは入力からのトポロジカル順
  // に整列される．
  for ( auto id: mOutputList ) {
    order_node(id, mark);
  }

  // DFFのファンインに番号をつける．
  for ( auto& dff: mDffList ) {
    auto src_id = dff.src_id;
    order_node(src_id, mark);
  }
}

// @brief トポロジカルソートを行い mLogicList にセットする．
void
ModelImpl::order_node(
  SizeType id,
  std::unordered_set<SizeType>& mark
)
{
  if ( mark.count(id) > 0 ) {
    return;
  }
  auto node = mNodeArray[id].get();
  if ( !node->is_logic() ) {
    throw std::logic_error{"node->is_logic() == false"};
  }
  for ( auto iid: node->fanin_id_list() ) {
    order_node(iid, mark);
  }
  mLogicList.push_back(id);
  mark.emplace(id);
}

// @brief 内容を出力する．
void
ModelImpl::write(
  std::ostream& s
) const
{
  if ( !name().empty() ) {
    s << "Name: " << name()
      << std::endl;
  }
  for ( auto& comment: comment_list() ) {
    s << "Comment: " << comment
      << std::endl;
  }
  for ( SizeType i = 0;i < input_num(); ++ i ) {
    auto id = input_id(i);
    s << "I#" << i;
    auto name = input_name(i);
    if ( !name.empty() ) {
      s << "[" << name << "]";
    }
    s << ": "
      << node_name(id)
      << std::endl;
  }
  for ( SizeType i = 0; i < output_num(); ++ i ) {
    auto id = output_id(i);
    s << "O#" << i;
    auto name = output_name(i);
    if ( !name.empty() ) {
      s << "[" << name << "]";
    }
    s << ": "
      << node_name(id)
      << std::endl;
  }
  for ( SizeType i = 0; i < dff_num(); ++ i ) {
    auto& dff = dff_impl(i);
    auto id = dff.id;
    auto src_id = dff.src_id;
    s << "Q#" << i;
    auto name = dff.name;
    if ( !name.empty() ) {
      s << "[" << name << "]";
    }
    s << ": output = " << node_name(id)
      << ", src = N#" << src_id
      << std::endl;
  }
  for ( auto id: logic_id_list() ) {
    auto& node = node_impl(id);
    s << node_name(id)
      << " = "
      << "F#" << node.func_id()
      << "(";
    const char* comma = "";
    for ( auto iid: node.fanin_id_list() ) {
      s << comma << "N#" << iid;
      comma = ", ";
    }
    s << ")"
      << std::endl;
  }
  if ( func_num() > 0 ) {
    for ( SizeType id = 0; id < func_num(); ++ id ) {
      s << "F#" << id << ": ";
      auto& func = func_impl(id);
      func.print(s);
    }
  }
}

// @brief print() 中でノード名を出力する関数
std::string
ModelImpl::node_name(
  SizeType id
) const
{
  std::ostringstream buf;
  buf << "N#" << id;
  if ( mNameDict.count(id) > 0 ) {
    buf << "[" << mNameDict.at(id) << "]";
  }
  return buf.str();
}

END_NAMESPACE_YM_BN
