
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
    mOutputNameList{src.mOutputNameList},
    mNameDict{src.mNameDict},
    mFuncMgr{src.mFuncMgr}
{
  // NodeImpl は単純にはコピーできない．
  mNodeArray.reserve(src.mNodeArray.size());
  mNodeList.reserve(src.mNodeList.size());
  mInputList.reserve(src.mInputList.size());
  mOutputList.reserve(src.mOutputList.size());
  mDffArray.reserve(src.mDffArray.size());
  mDffList.reserve(src.mDffList.size());
  mLogicList.reserve(src.mLogicList.size());
  // mNodeArray, mNodeList のコピー
  for ( auto src_node: src.mNodeList ) {
    mNodeArray.push_back(src_node->copy());
    mNodeList.push_back(mNodeArray.back().get());
  }
  // mInputList のコピー
  for ( auto src_node: src.mInputList ) {
    auto node = mNodeList[src_node->id()];
    mInputList.push_back(node);
  }
  // mOutputList のコピー
  for ( auto src_node: src.mOutputList ) {
    auto node = mNodeList[src_node->id()];
    mOutputList.push_back(node);
  }
  // mDffArray, mDffList のコピー
  for ( auto src_dff: src.mDffList ) {
    auto output = mNodeList[src_dff->output()->id()];
    auto src = mNodeList[src_dff->src()->id()];
    auto new_dff = new DffImpl(this, src_dff->id(), src_dff->name(),
			       output, src, src_dff->reset_val());
    mDffArray.push_back(std::unique_ptr<DffImpl>{new_dff});
    mDffList.push_back(mDffArray.back().get());
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
      auto name = dff_impl(i)->name();
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

// @brief 新しい入力ノードを作る．
const NodeImpl*
ModelImpl::new_input(
  const std::string& name
)
{
  auto node = new_node(
    [&](SizeType id) {
      auto iid = mInputList.size();
      return NodeImpl::new_primary_input(this, id, iid);
    });
  mInputList.push_back(node);
  if ( name != "" ) {
    mNameDict.emplace(node->id(), name);
  }
  return node;
}

// @brief 新しいDFF出力ノードを作る．
const NodeImpl*
ModelImpl::new_dff_output(
  SizeType dff_id
)
{
  _check_dff_id(dff_id, "set_dff_output");
  auto node = new_node(
    [&](SizeType id) {
      return NodeImpl::new_dff_output(this, id, dff_id);
    });
  mDffArray[dff_id]->mOutput = node;
  return node;
}

// @brief 新しい出力ノードを作る．
SizeType
ModelImpl::new_output(
  const NodeImpl* src,
  const std::string& name
)
{
  auto oid = mOutputList.size();
  mOutputList.push_back(src);
  mOutputNameList.push_back(name);
  return oid;
}

// @brief 新しい論理ノードを作る．
const NodeImpl*
ModelImpl::new_logic(
  const FuncImpl* func,
  const std::vector<const NodeImpl*>& fanin_list
)
{
  auto node = new_node(
    [&](SizeType id) {
      return NodeImpl::new_logic(this, id, func, fanin_list);
    });
  mLogicList.push_back(node);
  return node;
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
    auto node = input(i);
    s << "I#" << i;
    auto name = input_name(i);
    if ( !name.empty() ) {
      s << "[" << name << "]";
    }
    s << ": "
      << node_name(node)
      << std::endl;
  }
  for ( SizeType i = 0; i < output_num(); ++ i ) {
    auto node = output(i);
    s << "O#" << i;
    auto name = output_name(i);
    if ( !name.empty() ) {
      s << "[" << name << "]";
    }
    s << ": "
      << node_name(node)
      << std::endl;
  }
  for ( SizeType i = 0; i < dff_num(); ++ i ) {
    auto dff = dff_impl(i);
    auto node = dff->output();
    auto src = dff->src();
    s << "Q#" << i;
    auto name = dff->name();
    if ( !name.empty() ) {
      s << "[" << name << "]";
    }
    s << ": output = " << node_name(node);
    if ( src != nullptr ) {
      s << ", src = N#" << src->id();
    }
    s << std::endl;
  }
  for ( auto node: logic_list() ) {
    s << node_name(node)
      << " = "
      << "F#" << node->func()->id()
      << "(";
    const char* comma = "";
    for ( auto inode: node->fanin_list() ) {
      s << comma << "N#" << inode->id();
      comma = ", ";
    }
    s << ")"
      << std::endl;
  }
  if ( func_num() > 0 ) {
    for ( SizeType id = 0; id < func_num(); ++ id ) {
      s << "F#" << id << ": ";
      auto func = func_impl(id);
      func->print(s);
    }
  }
}

// @brief print() 中でノード名を出力する関数
std::string
ModelImpl::node_name(
  const NodeImpl* node
) const
{
  std::ostringstream buf;
  auto id = node->id();
  buf << "N#" << id;
  if ( mNameDict.count(id) > 0 ) {
    buf << "[" << mNameDict.at(id) << "]";
  }
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス ImplBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ImplBase::ImplBase(
  const ModelImpl* model
) : mModel{model}
{
}

// @brief 参照を増やす
void
ImplBase::inc_ref() const
{
  if ( _model() != nullptr ) {
    _model()->inc_ref();
  }
}

// @brief 参照を減らす
void
ImplBase::dec_ref() const
{
  if ( _model() != nullptr ) {
    _model()->dec_ref();
  }
}

END_NAMESPACE_YM_BN
