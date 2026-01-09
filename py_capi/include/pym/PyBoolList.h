#ifndef PYBOOLLIST_H
#define PYBOOLLIST_H

/// @file PyBoolList.h
/// @brief PyBoolList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym_config.h"
#include "pym/PyBool.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyBoolList PyBoolList.h "PyBoolList.h"
/// @brief Python のリストに関する関数を集めたクラス
///
/// 意味的には PyList<bool, PyBool> と同様だが，
/// std::vector<bool> は通常の std::vector<T> とは実装が異なっている
/// ので別のクラスにしている．
//////////////////////////////////////////////////////////////////////
class PyBoolList
{
public:

  using ElemType = std::vector<bool>;

public:

  /// @brief 要素のリストを表す PyObject* を作るファンクタクラス
  struct Conv {
    PyObject*
    operator()(
      const ElemType& val_list
    )
    {
      typename PyBool::Conv conv;
      SizeType n = val_list.size();
      auto obj = PyList_New(n);
      for ( SizeType i = 0; i < n; ++ i ) {
	auto val = val_list[i];
	auto val_obj = conv(val);
	PyList_SET_ITEM(obj, i, val_obj);
      }
      return obj;
    }

  };


  /// @brief 要素のリストを取り出すファンクタクラス
  struct Deconv {
    bool
    operator()(
      PyObject* obj,
      ElemType& val_list
    )
    {
      typename PyBool::Deconv deconv;

      // 通常は T を表す PyObject のシーケンス
      if ( !PySequence_Check(obj) ) {
	return false;
      }
      auto n = PySequence_Size(obj);
      val_list.clear();
      val_list.reserve(n);
      for ( SizeType i = 0; i < n; ++ i ) {
	auto val_obj = PySequence_GetItem(obj, i);
	bool val;
	auto ans = deconv(val_obj, val);
	Py_DecRef(val_obj);
	if ( !ans ) {
	  return false;
	}
	val_list.push_back(val);
      }
      return true;
    }
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief vector<bool> を表す PyObject を作る．
  /// @return リストを表す Python のオブジェクト(PyBoolList)を返す．
  static
  PyObject*
  ToPyObject(
    const ElemType& val_list ///< [in] 値のリスト
  )
  {
    Conv conv;
    return conv(val_list);
  }

  /// @brief PyObject から vector<bool> を取り出す．
  /// @return 正しく変換できた時に true を返す．
  ///
  /// deconv は PyObject* から T を取り出すファンクタクラス
  static
  bool
  FromPyObject(
    PyObject* obj,     ///< [in] Python のオブジェクト
    ElemType& val_list ///< [out] 結果を格納するリスト
  )
  {
    Deconv deconv;
    return deconv(obj, val_list);
  }

  /// @brief PyObject がシーケンス型かどうか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の Python オブジェクト
  )
  {
    if ( !PySequence_Check(obj) ) {
      return false;
    }
    auto n = PySequence_Size(obj);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto obj1 = PySequence_GetItem(obj, i);
      auto ans = PyBool::Check(obj1);
      Py_DecRef(obj1);
      if ( !ans ) {
	return false;
      }
    }
    return true;
  }

  /// @brief PyObject から vector<bool> を取り出す．
  static
  ElemType
  Get(
    PyObject* obj ///< [in] 対象の Python オブジェクト
  )
  {
    ElemType val;
    if ( FromPyObject(obj, val) ) {
      return val;
    }
    PyErr_SetString(PyExc_TypeError, "not a sequence type");
    return {};
  }

};

END_NAMESPACE_YM

#endif // PYBOOLLIST_H
