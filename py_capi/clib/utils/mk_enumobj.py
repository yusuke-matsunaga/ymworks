#! /usr/bin/env python3

""" 列挙型の拡張オブジェクトを作るスクリプト

:file: mk_enumobj.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import os.path
from datetime import datetime
from enumspec import EnumSpec


def mk_enumobj(class_name, keywords, namespace, module_name,
               src_h_file, dist_h_file, dist_cc_file, dist_py_file):
    basedir = os.path.dirname(__file__)
    template_h_file = os.path.join(basedir, "PyEnumTempl.h")
    template_cc_file = os.path.join(basedir, "PyEnumTempl.cc")
    template_py_file = os.path.join(basedir, "py_enum_test.py")
    enum_spec = EnumSpec(class_name, keywords)
    
    year = f'{datetime.now().year}'

    with open(template_h_file, "rt") as fin:
        body = fin.read()

        body = body.replace("%%YEAR%%", year)
        body = body.replace('%%NAMESPACE%%', namespace)
        body = body.replace('%%HEADER_FILE%%', src_h_file)
        body = body.replace('%%CAP_CLASS%%', enum_spec.class_name.upper())
        body = body.replace('%%CLASS%%', enum_spec.class_name)

        with open(dist_h_file, "wt") as fout:
            fout.write(body)

    with open(template_cc_file, "rt") as fin:
        body = fin.read()

        body = body.replace('%%YEAR%%', year)
        body = body.replace('%%NAMESPACE%%', namespace)
        body = body.replace('%%CLASS%%', enum_spec.class_name)

        body = body.replace('%%CONST_STR%%', enum_spec.const_str())
        body = body.replace('%%CONST_DEF%%', enum_spec.const_def())
        body = body.replace('%%STRCMP%%', enum_spec.strcmp())
        body = body.replace('%%CONST_REPR%%', enum_spec.const_repr())
        body = body.replace('%%NEW_OBJ%%', enum_spec.new_obj())
        body = body.replace('%%REG_OBJ%%', enum_spec.reg_obj())
        body = body.replace('%%XDECREF%%', enum_spec.xdecref())
        body = body.replace('%%NONEOBJ%%', enum_spec.noneobj())
        body = body.replace('%%TOPYOBJECT%%', enum_spec.topyobject())

        with open(dist_cc_file, "wt") as fout:
            fout.write(body)

    with open(template_py_file, "rt") as fin:
        body = fin.read()
        filename = os.path.basename(dist_py_file)
        body = body.replace("%%FILENAME%%", filename)
        body = body.replace("%%YEAR%%", year)
        body = body.replace('%%CLASS%%', enum_spec.class_name)
        body = body.replace('%%MAKE_TEST%%', enum_spec.make_test())
        body = body.replace('%%MODULE%%', module_name)
        
        with open(dist_py_file, "wt") as fout:
            fout.write(body)

        
if __name__ == '__main__':
    
    keywords = [ "cmos", "fpga", "none" ]
    mk_enumobj("ClibTechnology", keywords, "YM", "ymcell",
               "ym/clib.h", "foo.h", "foo.cc")
    
