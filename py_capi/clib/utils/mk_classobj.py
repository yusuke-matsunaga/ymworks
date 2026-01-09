#! /usr/bin/env python3

""" 拡張オブジェクトを作るスクリプト

:file: mk_classobj.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2024 Yusuke Matsunaga, All rights reserved.
"""

import os.path
from datetime import datetime


def mk_classobj(class_name, impl_name, namespace, src_h_file, module_name):
    basedir = os.path.dirname(__file__)
    template_h_file = os.path.join(basedir, "PyClassTempl.h")
    template_cc_file = os.path.join(basedir, "PyClassTempl.cc")
    template_py_file = os.path.join(basedir, "py_templ_test.py")

    h_dir = os.path.join(basedir, "..", "..", "include", "pym")
    cc_dir = os.path.join(basedir, "..")
    py_dir = os.path.join(basedir, "..", "..", "tests", "pytest")
    dist_h_file = os.path.join(h_dir, f"Py{class_name}.h")
    dist_cc_file = os.path.join(cc_dir, f"Py{class_name}.cc")
    dist_py_file = os.path.join(py_dir, f"py_{class_name}_test.py")
    
    cap_class_name = class_name.upper();
    year = f'{datetime.now().year}'

    with open(template_h_file, "rt") as fin:
        body = fin.read()

        body = body.replace("%%YEAR%%", year)
        body = body.replace('%%NAMESPACE%%', namespace)
        body = body.replace('%%HEADER_FILE%%', src_h_file)
        body = body.replace('%%CAP_CLASS%%', cap_class_name)
        body = body.replace('%%CLASS%%', class_name)
        body = body.replace('%%IMPL%%', impl_name)
        
        with open(dist_h_file, "wt") as fout:
            fout.write(body)

    with open(template_cc_file, "rt") as fin:
        body = fin.read()

        body = body.replace('%%YEAR%%', year)
        body = body.replace('%%NAMESPACE%%', namespace)
        body = body.replace('%%CLASS%%', class_name)
        body = body.replace('%%IMPL%%', impl_name)

        with open(dist_cc_file, "wt") as fout:
            fout.write(body)

    with open(template_py_file, "rt") as fin:
        body = fin.read()
        filename = os.path.basename(dist_py_file)
        body = body.replace("%%FILENAME%%", filename)
        body = body.replace("%%YEAR%%", year)
        body = body.replace('%%CLASS%%', class_name)
        body = body.replace('%%MODULE%%', module_name)
        
        with open(dist_py_file, "wt") as fout:
            fout.write(body)

        
if __name__ == '__main__':
    
    mk_classobj("ClibPin", "YM_CLIB", "ymcell",
                "ym/clib.h", "foo.h", "foo.cc")
    
