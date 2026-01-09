#! /usr/bin/env python3

"""CodeGen の実装ファイル

:file: codegen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import sys
import os.path
from datetime import datetime
from methoddef import MethodDef
from cgutils import Line, Text, Block


class CodeGen:
    """Python の拡張オブジェクト用のコードを生成するクラス"""
            
    def __init__(self):
        self.method_list = []
        self.year = f'{datetime.now().year}'
        
    def gen_h(self, *, fout=sys.stdout):
        """ヘッダファイルを出力する．

        :param fout: 出力先のファイルオブジェクト
        """
        head_text = """ifndef {CAP_PYCLASS}_H
#define {CAP_PYCLASS}_H

/// @file {PYCLASS}.h
/// @brief {PYCLASS} のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) {YEAR} Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

""".format(CAP_PYCLASS=self.py_class_name.upper(),
           PYCLASS=self.py_class_name,
           YEAR=self.year)
        fout.write(head_text)
        for filename in self.h_includes:
            fout.write(f'#include "{filename}"\n')
        fout.write('\n')
        fout.write('\n')
        block = self.namespace_block([self.h_class_block()])
        block.gen(fout, 0)
        tail_text= """
#endif // {CAP_PYCLASS}_H
""".format(CAP_PYCLASS=self.py_class_name.upper())
        fout.write(tail_text)

    def gen_cc(self, *, fout=sys.stdout):
        """ソースファイルを出力する．

        :param fout: 出力先のファイルオブジェクト
        """

        ns_body = [
            self.cc_localdef_block(),
            self.cc_init_block(),
            self.cc_frompy_block(),
            self.cc_topy_block(),
            self.cc_check_block(),
            self.cc_get_block(),
            self.cc_typeobject_block()
        ]

        self.cc_head().gen(fout, 0)
        self.cc_include_block().gen(fout, 0)
        block = self.namespace_block(ns_body)
        block.gen(fout, 0)

    def h_class_block(self):
        """ヘッダファイルのクラス定義を作る．"""
        list_body = []
        list_body.append(self.h_class_head())
        list_body.append(self.h_class_frompy())
        list_body.append(self.h_class_topy())
        list_body.append(self.h_class_check())
        list_body.append(self.h_class_get())
        list_body.append(self.h_class_tail())
        return Block.new_list(list_body)
                            
    def h_class_head(self):
        """ヘッダファイルのクラス定義の先頭を作る．"""
        head_text="""//////////////////////////////////////////////////////////////////////
/// @class {PYCLASS} {PYCLASS}.h "{PYCLASS}.h"
/// @brief Python 用の {CLASS} 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class {PYCLASS}
{{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @return 初期化が成功したら true を返す．
  static
  bool
  init(
    PyObject* m ///< [in] 親のモジュールを表す PyObject
  );
""".format(PYCLASS=self.py_class_name,
           CLASS=self.class_name)
        return Text(head_text)

    def h_class_frompy(self):
        """ヘッダファイルの FromPyObject() の宣言を作る．"""
        text = """
  /// @brief PyObject から {CLASS} を取り出す．
  /// @return 変換が成功したら true を返す．
  ///
  /// エラーの場合には Python 例外をセットする．
  static
  bool
  FromPyObject(
    PyObject* obj,            ///< [in] {CLASS} を表す PyObject
    {FROM_TYPE} val,     ///< [out] 変換された {CLASS} を格納する変数
    const char* msg = nullptr ///< [in] エラーメッセージ(省略時にはデフォルト値を使う)
  );
""".format(CLASS=self.class_name,
           FROM_TYPE=self.from_type)
        return Text(text)

    def h_class_topy(self):
        """ヘッダファイルの ToPyObject() の宣言を作る．"""
        text = """
  /// @brief {CLASS} を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    {TO_TYPE} val ///< [in] 値
  );
""".format(CLASS=self.class_name,
           TO_TYPE=self.to_type)
        return Text(text)

    def h_class_get(self):
        """ヘッダファイルの Get() の宣言を作る．"""
        text = """
  /// @brief PyObject から {CLASS} を取り出す．
  /// @return {CLASS} を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  {GET_TYPE}
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );
""".format(CLASS=self.class_name,
           GET_TYPE=self.get_type)
        return Text(text)

    def h_class_check(self):
        """ヘッダファイルの Check() の宣言を作る．"""
        text = """
  /// @brief PyObject が {CLASS} タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );
""".format(CLASS=self.class_name)
        return Text(text)
        
    def h_class_tail(self):
        """ヘッダファイルのクラス定義の末尾を作る．"""
        text = """
  /// @brief {CLASS} を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

}};

""".format(CLASS=self.class_name)
        return Text(text)

    def cc_head(self):
        """ソースファイルの先頭ブロック"""
        text = """
/// @file {PYCLASS}.cc
/// @brief {PYCLASS} の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) {YEAR} Yusuke Matsunaga
/// All rights reserved.

""".format(PYCLASS=self.py_class_name,
           YEAR=self.year)
        return Text(text)

    def cc_include_block(self):
        """ソースファイルのインクルードブロック"""
        list_body = []
        for filename in self.cc_includes:
            list_body.append(Line(f'#include "{filename}"'))
        list_body.append(Line(''))
        list_body.append(Line(''))
        return Block.new_list(list_body)

    def cc_localdef_block(self):
        """ソースファイルのローカルな定義ブロック"""
        list_body = []
        list_body.append(self.obj_block())
        list_body.append(self.type_block())
        list_body.append(self.new_block())
        list_body.append(self.dealloc_block())
        list_body.append(self.repr_block())
        list_body.append(self.method_block())
        list_body.append(self.getset_block())
        list_body.append(self.richcmp_block())
        list_body.append(self.as_number_block())
        list_body.append(self.as_sequence_block())
        list_body.append(self.as_mapping_block())
        list_body.append(self.hash_block())
        list_body.append(self.call_block())
        list_body.append(self.str_block())
        body = Block.new_list(list_body)
        return Block.new_nonamespace(list_body)

    def obj_block(self):
        """オブジェクト定義ブロック"""
        return Block.new_list([
            self.obj_head(),
            self.obj_body(),
            self.obj_tail()
        ])

    def obj_head(self):
        """オブジェクト定義の先頭"""
        text="""
// Python 用のオブジェクト定義
struct {OBJ_CLASS}
{{
  PyObject_HEAD
""".format(OBJ_CLASS=self.py_obj_name)
        return Text(text)

    def obj_body(self):
        """オブジェクト定義の本体"""
        return Text(f'  {self.class_name}* mVal;\n')
    
    def obj_tail(self):
        """オブジェクト定義の末尾"""
        return Text('};\n')
        
    def type_block(self):
        """タイプ定義ブロック"""
        text="""
// Python 用のタイプ定義
PyTypeObject {TYPE_CLASS} = {{
  PyVarObject_HEAD_INIT(nullptr, 0)
}};
""".format(TYPE_CLASS=self.py_type_name)
        return Text(text)

    def new_block(self):
        return None

    def dealloc_block(self):
        """dealloc() 関数の定義"""
        text="""
// 終了関数
void
dealloc(
  PyObject* self
)
{{
  auto val_obj = reinterpret_cast<{OBJ_CLASS}*>(self);
  // 必要なら val_obj->mVal の終了処理を行う．
  Py_TYPE(self)->tp_free(self);
}}

""".format(OBJ_CLASS=self.py_obj_name)
        return Text(text)

    def repr_block(self):
        return None

    def method_block(self):
        if len(self.method_list) == 0:
            return None
        list_obj = []
        for meth in self.method_list:
            block = meth.gen_defbody()
            list_obj.append(block)
        list_obj.append(self.method_table())
        return Block.new_list(list_obj)

    def method_table(self):
        list_obj = []
        list_obj.append(self.meth_table_head())
        for meth in self.method_list:
            entry = meth.gen_entry()
            list_obj.append(entry)
        list_obj.append(self.meth_table_tail())
        return Block.new_list(list_obj)

    def meth_table_head(self):
        text = """// メソッド定義
PyMethDef methods[] = {
"""
        return Text(text)
    
    def meth_table_tail(self):
        text = """  {nullptr, nullptr, 0, nullptr}
};
"""
        return Text(text)
            
    def getset_block(self):
        return None

    def richcmp_block(self):
        return None

    def as_number_block(self):
        return None

    def as_sequence_block(self):
        return None

    def as_mapping_block(self):
        return None

    def hash_block(self):
        return None

    def call_block(self):
        return None

    def str_block(self):
        return None

    def cc_init_block(self):
        """オブジェクトの初期化ブロック"""
        list_obj = []
        list_obj.append(self.cc_init_head())
        if self.richcmp_block() is not None:
            text = f"  {self.py_type_name}.tp_richcompare = richcmpfunc;"
            list_obj.append(Line(text))
        if self.method_block() is not None:
            text = f'  {self.py_type_name}.tp_methods = methods;'
            list_obj.append(Line(text))
        if self.getset_block() is not None:
            block = Text(f'  {self.py_type_name}.tp_getset = getsetters;\n')
            list_obj.append(block)
        if self.new_block() is not None:
            block = Text(f'  {self.py_type_name}.tp_new = new_func;\n')
            list_obj.append(block)
        if self.repr_block() is not None:
            block = Text(f'  {self.py_type_name}.tp_repr = repr_func;\n')
            list_obj.append(block)
        if self.str_block() is not None:
            block = Text(f'  {self.py_type_name}.tp_str = str_func;\n')
            list_obj.append(block)
        if self.as_number_block() is not None:
            block = Text(f'  {self.py_type_name}.tp_as_number = number;\n')
            list_obj.append(block)
        if self.as_sequence_block() is not None:
            block = Text(f'  {self.py_type_name}.tp_as_sequence = sequence;\n')
            list_obj.append(block)
        if self.as_mapping_block() is not None:
            block = Text(f'  {self.py_type_name}.tp_as_mapping = mapping;\n')
            list_obj.append(block)
        list_obj.append(self.cc_init_tail())
        return Block.new_list(list_obj)

    def cc_init_head(self):
        """init_block の先頭"""
        text="""
// @brief 初期化する．
bool
{PYCLASS}::init(
  PyObject* m
)
{{
  {TYPE_CLASS}.tp_name = "{CLASS}";
  {TYPE_CLASS}.tp_basicsize = sizeof({OBJ_CLASS});
  {TYPE_CLASS}.tp_itemsize = 0;
  {TYPE_CLASS}.tp_dealloc = dealloc;
  {TYPE_CLASS}.tp_flags = Py_TPFLAGS_DEFAULT;
  {TYPE_CLASS}.tp_doc = PyDoc_STR("{DOC_STR}");
""".format(PYCLASS=self.py_class_name,
           TYPE_CLASS=self.py_type_name,
           CLASS=self.class_name,
           OBJ_CLASS=self.py_obj_name,
           DOC_STR=self.doc_str)
        return Text(text)

    def cc_init_tail(self):
        """init_block の末尾"""
        text="""  if ( PyType_Ready(&{TYPE_CLASS}) < 0 ) {{
    return false;
  }}

  return true;
}}
""".format(TYPE_CLASS=self.py_type_name)
        return Text(text)

    def cc_frompy_block(self):
        """FromPyObject()関数の定義"""
        text="""
// @brief PyObject から {CLASS} を取り出す．
bool
{PYCLASS}::FromPyObject(
  PyObject* obj,
  {FROM_TYPE} val,
  const char* msg
)
{{
  if ( !Check(obj) ) {{
    if ( msg == nullptr ) {{
      msg = "object should be a {CLASS} type";
    }}
    PyErr_SetString(PyExc_TypeError, msg);
    return false;
  }}
  val = Get(obj);
  return true;
}}
""".format(PYCLASS=self.py_class_name,
           CLASS=self.class_name,
           FROM_TYPE=self.from_type)
        return Text(text)

    def cc_topy_block(self):
        """ToPyObject()関数の定義"""
        text="""
// @brief {CLASS} を表す PyObject を作る．
PyObject*
{PYCLASS}::ToPyObject(
  {TO_TYPE} val
)
{{
  PyObject* obj = {TYPE_CLASS}.tp_alloc(&{TYPE_CLASS}, 0);
  auto val_obj = reinterpret_cast<{OBJ_CLASS}*>(obj);
  //val_obj->mVal の設定
  Py_INCREF(obj);
  return obj;
}}
""".format(PYCLASS=self.py_class_name,
           CLASS=self.class_name,
           TO_TYPE=self.to_type,
           TYPE_CLASS=self.py_type_name,
           OBJ_CLASS=self.py_obj_name)
        return Text(text)

    def cc_get_block(self):
        """Get()関数の定義"""
        text="""
// @brief PyObject から {CLASS} を取り出す．
{GET_TYPE}
{PYCLASS}::Get(
  PyObject* obj
)
{{
  auto val_obj = reinterpret_cast<{OBJ_CLASS}*>(obj);
  return val_obj->mVal;
}}
""".format(PYCLASS=self.py_class_name,
           GET_TYPE=self.get_type,
           OBJ_CLASS=self.py_obj_name,
           CLASS=self.class_name)
        return Text(text)

    def cc_check_block(self):
        """Check()関数の定義"""
        text="""
// @brief PyObject が {CLASS} タイプか調べる．
bool
{PYCLASS}::Check(
  PyObject* obj
)
{{
  return Py_IS_TYPE(obj, _typeobject());
}}
""".format(PYCLASS=self.py_class_name,
           CLASS=self.class_name)
        return Text(text)

    def cc_typeobject_block(self):
        """_typeobject()関数の定義"""
        text="""
// @brief {CLASS} を表すオブジェクトの型定義を返す．
PyTypeObject*
{PYCLASS}::_typeobject()
{{
  return &{TYPE_CLASS};
}}
""".format(PYCLASS=self.py_class_name,
           TYPE_CLASS=self.py_type_name,
           CLASS=self.class_name)
        return Text(text)
        
    def namespace_block(self, body):
        """名前空間で囲まれたブロックを作る．"""
        return Block.new_namespace(self.namespace, body=body)

    @property
    def class_name(self):
        assert False
        
    @property
    def h_includes(self):
        return []

    @property
    def cc_includes(self):
        return []
    
    @property
    def namespace(self):
        return "YM"
    
    @property
    def py_class_name(self):
        """Python API 用のクラス名を返す．"""
        return f'Py{self.class_name}'

    @property
    def py_obj_name(self):
        """Python オブジェクト 用のクラス名を返す．"""
        return f'{self.class_name}_Object'

    @property
    def py_type_name(self):
        """Python タイプオブジェクト 用のクラス名を返す．"""
        return f'{self.class_name}_Type'

    @property
    def doc_str(self):
        """Doc Stringを返す．"""
        return f'{self.class_name} object'
    
    @property
    def from_type(self):
        """FromPyObject() の引数の型を返す．"""
        return f'{self.class_name}&'

    @property
    def to_type(self):
        """ToPyObject() の引数の型を返す．"""
        return f'const {self.class_name}&'
    
    @property
    def get_type(self):
        """Get() の返り値の型を返す．"""
        return f'{self.class_name}'
    

if __name__ == '__main__':

    class CellLibraryGen(CodeGen):

        def __init__(self):
            super().__init__()

        @property
        def class_name(self):
            return 'ClibCellLibrary'
        
        @property
        def h_includes(self):
            return ['ym/clib.h']

        @property
        def cc_includes(self):
            return ['pym/PyClibCellLibrary.h',
                    'pym/PyModule.h']
    
    class Meth1(MethodDef):

        def __init__(self):
            super().__init__('is_valid',
                             no_args=True)

        @property
        def doc_str(self):
            return "check if having a valid data"

    meth = Meth1()
        
    gen = CellLibraryGen()
    gen.method_list.append(meth)

    gen.gen_h()
    #gen.gen_cc()
