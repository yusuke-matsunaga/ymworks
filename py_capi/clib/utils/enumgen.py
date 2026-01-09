#! /usr/bin/env python3

""" enum 型の拡張オブジェクト用ソースファイルを生成するスクリプト

:file: enumgen.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

from codegen import CodeGen
from cgutils import Line, Text, Block


class EnumGen(CodeGen):
    """列挙型の拡張オブジェクト用ソースファイルを生成するクラス"""

    def __init__(self, keywords):
        super().__init__()
        self.keywords = keywords

        self.has_none = False
        self.max_length = 0
        for kwd in self.keywords:
            l = len(kwd)
            self.max_length = max(self.max_length, l)
            if kwd == 'none':
                self.has_none = True

    def gen_const_str_def(self):
        """定数を表す文字列名の定義文を作る．"""
        list_obj = []
        for kwd in self.keywords:
            if kwd == 'none':
                # 'none' は無視
                continue
            l = len(kwd)
            nsp = self.max_length - l
            sp = ' ' * nsp
            cap_kwd = kwd.upper()
            list_obj.append(Text(f'const char* {cap_kwd}_STR{sp} = "{kwd}";'))
            list_obj.append(Text('\n'))
        return Block.new_list(list_obj)

    def gen_const_def(self):
        """定数オブジェクトの定義文を作る．"""
        list_obj = []
        for kwd in self.keywords:
            if kwd == 'none':
                # 'none' は無視
                continue
            l = len(kwd)
            nsp = self.max_length - l
            sp = ' ' * nsp
            cap_kwd = kwd.upper()
            list_obj.append(Text(f'PyObject* {self.class_name}_{cap_kwd}{sp} = nullptr;'))
            list_obj.append(Text('\n'))
        return Block.new_list(list_obj)

    def new_block(self):
        """new_func()関数を創る．"""
        head_text = """
// 生成関数
PyObject*
new_func(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{{
  if ( type != &{TYPE_CLASS} ) {{
    PyErr_SetString(PyExc_TypeError, "{CLASS} cannot be overloaded");
    return nullptr;
  }}

  // キーワード引数
  static const char* kwlist[] = {{
    "name",
    nullptr
  }};
  const char* name_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &name_str) ) {{
    return nullptr;
  }}

  {CLASS} val;
        
""".format(CLASS=self.class_name,
           TYPE_CLASS=self.py_type_name)
        head = Text(head_text)

        # 文字列比較用のコードを作る．
        buf = ""
        first = True
        for kwd in self.keywords:
            cap_kwd = kwd.upper();
            buf += '  '
            if first:
                first = False
            else:
                buf += 'else '
            if kwd == 'none':
                buf += 'if ( strcasecmp(name_str, "none") == 0 ) {'
                buf += '\n'
                buf += f'    val = {self.class_name}::none;'
                buf += '\n'
            else:
                buf += f'if ( strcasecmp(name_str, {cap_kwd}_STR) == 0 ) {{'
                buf += '\n'
                buf += f'    val = {self.class_name}::{kwd};'
                buf += '\n'
            buf += f'  }}'
            buf += '\n'
        buf += '  else {'
        buf += '\n'
        buf += '    // エラー'
        buf += '\n'
        buf += '    ostringstream buf;'
        buf += '\n'
        buf += '    buf << "argument 1 must be one of \\""'
        buf += '\n'
        first = True
        for kwd in self.keywords:
            if first:
                first = False
            else:
                buf += '        << "\\", \\""'
                buf += '\n'
            if kwd == 'none':
                buf += '        << "none"'
            else:
                cap_kwd = kwd.upper();
                buf += f'        << {cap_kwd}_STR'
            buf += '\n'
        buf += '        << "\\"";'
        buf += '\n'
        buf += '    PyErr_SetString(PyExc_ValueError, buf.str().c_str());'
        buf += '\n'
        buf += '    return nullptr;'
        buf += '\n'
        buf += '  }'
        buf += '\n'
        body = Text(buf)
        tail = Text(f'  return {self.py_class_name}::ToPyObject(val);\n}}\n')
        return Block.new_list([head, body, tail])

    def repr_block(self):
        """repr_func()を生成する．"""
        head_text = """
// repr() 関数
PyObject*
{CLASS}_repr(
  PyObject* self
)
{{
  auto val = {PY_CLASS}::Get(self);
  const char* tmp_str = nullptr;
  switch ( val ) {{
""".format(CLASS=self.class_name,
           PY_CLASS=self.py_class_name)
        tail_text = """
  }
  return Py_BuildValue("s", tmp_str);
}
"""
        head = Text(head_text)
        tail = Text(tail_text)
        buf = ""
        for kwd in self.keywords:
            l = len(kwd)
            nsp = self.max_length - l
            sp = ' ' * nsp
            buf += f'  case {self.class_name}::{kwd}:{sp} '
            if kwd == 'none':
                buf += 'tmp_str = ""; break;'
            else:
                cap_kwd = kwd.upper();
                buf += f'tmp_str = {cap_kwd}_STR; break;'
            buf += '\n'
        body = Text(buf)
        return Block.new_list([head, body, tail])
    
    def new_obj(self):
        """定数オブジェクトの生成文を作る．"""
        buf = ""
        for kwd in self.keywords:
            if kwd == 'none':
                continue
            l = len(kwd)
            nsp = self.max_length - l
            sp = ' ' * nsp
            cap_kwd = kwd.upper();
            buf += f'  {self.class_name}_{cap_kwd}{sp} = '
            buf += f'new_obj({self.class_name}::{kwd});'
            buf += '\n'
        return buf.rstrip()
    
    def reg_obj(self):
        """定数オブジェクトを登録する文を作る．"""
        buf = ""
        for kwd in self.keywords:
            if kwd == 'none':
                continue
            l = len(kwd)
            nsp = self.max_length - l
            sp = ' ' * nsp
            cap_kwd = kwd.upper();
            buf += f'  if ( !reg_obj({cap_kwd}_STR, {self.class_name}_{cap_kwd}) ) {{'
            buf += '\n'
            buf += f'    goto error;'
            buf += '\n'
            buf += '  }'
            buf += '\n'
        return buf.rstrip()
    
    def xdecref(self):
        """定数オブジェクトの開放用のコードを作る．"""
        buf = ""
        for kwd in self.keywords:
            if kwd == 'none':
                continue
            cap_kwd = kwd.upper();
            buf += f'  Py_XDECREF({self.class_name}_{cap_kwd});'
            buf += '\n'
        return buf.rstrip()

    def noneobj(self):
        """None 対応のコードを生成する．"""
        buf = ""
        if self.has_none:
            buf += '  if ( obj == Py_None ) {'
            buf += '\n'
            buf += f'    // 特例: None は {self.class_name}::none に変換する．'
            buf += '\n'
            buf += f'    val = {self.class_name}::none;'
            buf += '\n'
            buf += '    return true;'
            buf += '\n'
            buf += '  }'
            buf += '\n'
        return buf

    def topyobject(self):
        """PyObject への変換コードを生成する．"""
        buf = ""
        for kwd in self.keywords:
            l = len(kwd)
            nsp = self.max_length - l
            sp = ' ' * nsp
            cap_kwd = kwd.upper();
            buf += f'  case {self.class_name}::{kwd}:{sp} '
            if kwd == 'none':
                buf += 'Py_RETURN_NONE;'
            else:
                buf += f'obj = {self.class_name}_{cap_kwd}; break;'
            buf += '\n'
        return buf.rstrip()

    def make_test(self):
        buf = ""
        for kwd in self.keywords:
            l_class = self.class_name.lower()
            if kwd == 'none':
                buf += f'def test_{l_class}_none():'
                buf += '\n'
                buf += f'    val1 = %%MODULE%%.{self.class_name}("{kwd}")'
                buf += '\n'
                buf += '    assert val1 is None'
                buf += '\n'
            else:
                buf += f'def test_{l_class}_{kwd}():'
                buf += '\n'
                buf += f'    val1 = %%MODULE%%.{self.class_name}("{kwd}")'
                buf += '\n'
                buf += f'    assert val1 == %%MODULE%%.{self.class_name}.{kwd}'
                buf += '\n'
                buf += f'    assert val1.__repr__() == "{kwd}"'
                buf += '\n'
            buf += '\n'
        return buf.rstrip()
    
        
if __name__ == '__main__':
    import sys

    class ClibTechnologyGen(EnumGen):
        def __init__(self):
            keywords = [ "cmos", "fpga", "none" ]
            super().__init__(keywords)

        @property
        def class_name(self):
            return "ClibTechnology"

        @property
        def h_includes(self):
            return ["ym/clib.h"]

        @property
        def cc_incudes(self):
            return ["ym/clib.h"]

    gen = ClibTechnologyGen()
    
    #gen.gen_h()
    gen.gen_cc()
