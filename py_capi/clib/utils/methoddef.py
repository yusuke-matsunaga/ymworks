#! /usr/bin/env python3

""" MethodDef の定義ファイル

:file: methoddef.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import cgutils


class MethodDef:
    """Python のメソッド定義"""

    def __init__(self, name, *,
                 func_name=None,
                 no_args=False,
                 keywords=False,
                 static_method=False,
                 class_method=False):
        self.name = name
        if func_name is None:
            func_name = name
        self.func_name = func_name
        self.no_args = no_args
        self.keywords = keywords
        self.static_method = static_method
        self.class_method = class_method

    def gen_defbody(self):
        """定義本体"""
        return cgutils.Block.new_list([
            self.head(),
            self.body(),
            self.tail()
        ])

    def gen_entry(self):
        """テーブル用のエントリ"""
        list_obj = []
        list_obj.append(cgutils.Text('  {'))
        list_obj.append(cgutils.Text(f'"{self.name}", '))
        if self.keywords:
            list_obj.append(cgutils.Text(f'reinterpret_cast<PyCFunction>({self.func_name}),\n'))
        else:
            list_obj.append(cgutils.Text(f'{self.func_name},\n'))
        if self.no_args:
            flag_str = 'METH_NOARGS'
        else:
            flag_str = 'METH_VARARGS'
        if self.keywords:
            flag_str += ' | METH_KEYWORDS'
        if self.static_method:
            flag_str += ' | METH_STATIC'
        if self.class_method:
            flag_str += ' | METH_CLASS'
        list_obj.append(cgutils.Text(f'   {flag_str},\n'))
        list_obj.append(cgutils.Text(f'   PyDoc_STR("{self.doc_str}")}},\n'))
        return cgutils.Block.new_list(list_obj)
    
    def head(self):
        """先頭部分"""
        list_obj = [
            cgutils.Text('PyObject*\n'),
            cgutils.Text(f'{self.func_name}(\n')
        ]
        if self.static_method or self.class_method:
            list_obj.append(cgutils.Text('  PyObject* Py_UNUSED(self),\n'))
        else:
            list_obj.append(cgutils.Text('  PyObject* self,\n'))
        if self.no_args:
            list_obj.append(cgutils.Text('  PyObject* Py_UNUSED(args)'))
        else:
            list_obj.append(cgutils.Text('  PyObject* args'))
        if self.keywords:
            list_obj.append(cgutils.Text(',\n  PyObject* kwds\n'))
        else:
            list_obj.append(cgutils.Text('\n'))
        list_obj.append(cgutils.Text(')\n'))
        list_obj.append(cgutils.Text('{\n'))
        return cgutils.Block.new_list(list_obj)

    def body(self):
        return None
    
    def parse(self):
        """引数の解析部分"""
        assert self.no_args
        
    def tail(self):
        """末尾部分"""
        return cgutils.Text('}\n\n')

    @property
    def doc_str(self):
        return f'{self.name}'

    @staticmethod
    def return_sizetype(self, varname):
        return cgutils.Text(f'  return Py_BuildValue("k", varname);\n')

    @staticmethod
    def return_bool(self, varname):
        return cgutils.Text(f'  return PyBool_FromLong(varname);\n')

    @staticmethod
    def return_string(self, varname):
        return cgutils.Text(f'  return Py_BuildValue("s", varname);\n')

    @staticmethod
    def return_topyobject(self, typename, varname):
        return cgutils.Text(f'  return {typename}::ToPyObject({varname});\n')
    
    @staticmethod
    def return_none(self):
        return cgutils.Text('  Py_RETURN_NONE;\n')
    

if __name__ == '__main__':
    import sys
    
    class Meth1(MethodDef):

        def __init__(self):
            super().__init__('is_valid',
                             no_args=True)

        @property
        def doc_str(self):
            return "check if having a valid data"

    meth = Meth1()

    meth.gen_defbody().gen(fout=sys.stdout)
