
/// @file read.cc
/// @brief AigMgr::read() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "ym/BnModel.h"


BEGIN_NAMESPACE_YM_AIG

// @brief 任意の形式のファイルを読み込む．
AigMgr
AigMgr::read(
  const std::string& filename,
  const std::string& format
)
{
  std::string format1 = format;
  if ( format1.empty() ) {
    std::filesystem::path p = filename;
    auto ext = p.extension();
    if ( ext == ".aag" ) {
      format1 = "aag";
    }
    else if ( ext == ".aig" ) {
      format1 = "aig";
    }
    else if ( ext == ".blif" ) {
      format1 = "blif";
    }
    else if ( ext == ".bench" ) {
      format1 = "iscas89";
    }
    else {
      std::ostringstream buf;
      buf << ext << ": unknown extension for AigMgr::read()";
      throw std::invalid_argument{buf.str()};
    }
  }

  if ( format1 == "aag" ) {
    return read_aag(filename);
  }
  if ( format1 == "aig" ) {
    return read_aig(filename);
  }
  if ( format1 == "blif" ) {
    return read_blif(filename);
  }
  if ( format1 == "iscas89" ||
       format1 == "bench" ) {
    return read_iscas89(filename);
  }

  std::ostringstream buf;
  buf << format1 << ": unknown format for AigMgr::read()";
  throw std::invalid_argument{buf.str()};
}

// @brief blif ファイルの読み込みを行う．
AigMgr
AigMgr::read_blif(
  const std::string& filename
)
{
  // 読み込んだ BLIF モデル．
  auto blif_model = BnModel::read_blif(filename);

  return from_bn(blif_model);
}

// @brief iscs89 ファイルの読み込みを行う．
AigMgr
AigMgr::read_iscas89(
  const std::string& filename
)
{
  // 読み込んだ ISCAS89 モデル．
  auto iscas89_model = BnModel::read_iscas89(filename);

  return from_bn(iscas89_model);
}

END_NAMESPACE_YM_AIG
