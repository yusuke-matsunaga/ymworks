/// @file Parser_port.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtPort.h"
#include "parser/PtExpr.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtiPort の生成
//////////////////////////////////////////////////////////////////////

// @brief 空のポートの生成
void
Parser::new_Port()
{
  auto port = mFactory.new_Port(FileRegion());
  mPortList.push_back(port);
}

// @brief ポートの生成 (内側の式のみ指定するタイプ)
void
Parser::new_Port1(
  const FileRegion& file_region
)
{
  // 明示的に外の名前がついていなくても内側の名前が1つで
  // 範囲指定が無いときには内側の名前を外側の名前とする．
  if ( mPortRefList.size() == 1 ) {
    auto portref = mPortRefList.front();
    const char* name = nullptr;
    if ( portref->index_num() == 0 && portref->part() == nullptr ) {
      name = portref->name();
    }
    mPortRefList.clear();
    auto port = mFactory.new_Port(file_region, portref, name);
    mPortList.push_back(port);
  }
  else {
    auto port = mFactory.new_Port(file_region,
				  PtExprArray(mAlloc, mPortRefList),
				  nullptr);
    mPortList.push_back(port);
  }
}

// @brief ポートの生成 (外側の名前のみ指定するタイプ)
void
Parser::new_Port2(
  const FileRegion& file_region,
  const char* name
)
{
  auto port = mFactory.new_Port(file_region, name);
  mPortList.push_back(port);
}

// @brief ポートの生成 (外側の名前と内側の式を指定するタイプ)
void
Parser::new_Port3(
  const FileRegion& file_region,
  const char* name
)
{
  if ( mPortRefList.size() == 1 ) {
    auto port = mFactory.new_Port(file_region, mPortRefList.front(), name);
    mPortList.push_back(port);
    mPortRefList.clear();
  }
  else {
    auto port = mFactory.new_Port(file_region,
				  PtExprArray(mAlloc, mPortRefList),
				  name);
    mPortList.push_back(port);
  }
}


//////////////////////////////////////////////////////////////////////
// PtiPortArray の生成
//////////////////////////////////////////////////////////////////////

// @brief 入出力宣言中の重複チェックを行う．
bool
Parser::check_PortArray(
  const std::vector<PtIOHead*>& iohead_array
)
{
  std::unordered_set<std::string> portref_dic;
  for ( auto head: iohead_array ) {
    for ( auto elem: head->item_list() ) {
      auto name = elem->name();
      if ( portref_dic.count(name) > 0 ) {
	std::ostringstream buf;
	buf << "\"" << name << "\" is redefined.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			elem->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
	return false;
      }
      portref_dic.insert(name);
    }
  }
  return true;
}

// @brief 入出力宣言からポートを作る．
std::vector<PtPort*>
Parser::new_PortArray(
  const std::vector<PtIOHead*>& iohead_array
)
{
  SizeType num = 0;
  for ( auto head: iohead_array ) {
    num += head->item_num();
  }

  // ポートを生成し vec に格納する．
  std::vector<PtPort*> vec;
  vec.reserve(num);
  for ( auto head: iohead_array ) {
    for ( auto elem: head->item_list() ) {
      auto name = elem->name();
      auto portref = mFactory.new_Primary(elem->file_region(), name);
      auto port = mFactory.new_Port(elem->file_region(), portref, name);
      auto dir = head->direction();
      port->set_portref_dir(0, dir);
      vec.push_back(port);
    }
  }
  return vec;
}


//////////////////////////////////////////////////////////////////////
// portref の生成
//////////////////////////////////////////////////////////////////////

// @brief ポート参照式の生成
void
Parser::new_PortRef(
  const FileRegion& fr,
  const char* name
)
{
  auto primary = mFactory.new_Primary(fr, name);
  mPortRefList.push_back(primary);
}

// @brief ビット指定つきポート参照式の生成
void
Parser::new_PortRef(
  const FileRegion& fr,
  const char* name,
  const AstExpr* index
)
{
  auto primary = mFactory.new_Primary(fr, name,
				      PtExprArray(mAlloc, index));
  mPortRefList.push_back(primary);
}

// @brief 範囲指定付きポート参照式の生成
void
Parser::new_PortRef(
  const FileRegion& fr,
  const char* name,
  const AstPart* part
)
{
  auto primary = mFactory.new_Primary(fr, name, part);
  mPortRefList.push_back(primary);
}

// @brief ポート参照リストを初期化する．
void
Parser::init_portref_list()
{
  mPortRefList.clear();
}

END_NAMESPACE_YM_VERILOG
