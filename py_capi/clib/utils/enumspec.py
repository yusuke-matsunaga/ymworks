#! /usr/bin/env python3

""" enum 型の拡張オブジェクト用ソースファイルを生成するスクリプト

:file: enumspec.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

class EnumSpec:
    """列挙型の仕様を表すクラス"""

    def __init__(self, class_name, keywords):
        self.class_name = class_name
        self.keywords = keywords

        self.has_none = False
        self.max_length = 0
        for kwd in self.keywords:
            l = len(kwd)
            self.max_length = max(self.max_length, l)
            if kwd == 'none':
                self.has_none = True

    def const_str(self):
        """定数を表す文字列名の定義文を作る．"""
        buf = ""
        for kwd in self.keywords:
            if kwd == 'none':
                continue
            l = len(kwd)
            nsp = self.max_length - l
            sp = ' ' * nsp
            cap_kwd = kwd.upper()
            buf += f'const char* {cap_kwd}_STR{sp} = "{kwd}";'
            buf += '\n'
        return buf.rstrip()

    def const_def(self):
        """定数オブジェクトの定義文を作る．"""
        buf = ""
        for kwd in self.keywords:
            if kwd == 'none':
                continue
            l = len(kwd)
            nsp = self.max_length - l
            sp = ' ' * nsp
            cap_kwd = kwd.upper()
            buf += f'PyObject* {self.class_name}_{cap_kwd}{sp} = nullptr;'
            buf += '\n'
        return buf.rstrip()
    
    def strcmp(self):
        """文字列比較用のコードを作る．"""
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
        return buf.rstrip()

    def const_repr(self):
        """定数を表す文字列の生成文を作る．"""
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
        return buf.rstrip()
    
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
    
    keywords = [ "cmos", "fpga", "none" ]
    enum_spec = EnumSpec("ClibTechnology", "ym/clib.h", keywords)

    if len(sys.argv) != 2:
        print('USAGE: enumspec <CMD>')
        exit(1)

    if sys.argv[1] == 'const_str':
        ans = enum_spec.const_str()
    elif sys.argv[1] == 'const_def':
        ans = enum_spec.const_def()
    elif sys.argv[1] == 'strcmp':
        ans = enum_spec.strcmp()
    elif sys.argv[1] == 'const_repr':
        ans = enum_spec.const_repr()
    elif sys.argv[1] == 'new_obj':
        ans = enum_spec.new_obj()
    elif sys.argv[1] == 'reg_obj':
        ans = enum_spec.reg_obj()
    elif sys.argv[1] == 'xdecref':
        ans = enum_spec.xdecref()
    elif sys.argv[1] == 'topyobject':
        ans = enum_spec.topyobject()
    else:
        print(f'{sys.argv[1]}: unknown cmd')
        exit(2)

    print(ans, end='')
    
