#! /usr/bin/env python3

""" ファイルを生成するスクリプト

:file: mk_all.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

import os
import sys
import importlib
from argparse import ArgumentParser
from pathlib import Path
from mk_py_capi import root_dir
from mk_py_capi import ModuleGen, YmAddObjectLibraryTarget, AddLibraryTarget

# 引数の解釈
parser = ArgumentParser(prog='mk_all',
                        description="make 'ymworks' module")

parser.add_argument('-i', '--include_dir', type=str)
parser.add_argument('-s', '--source_dir', type=str)

args = parser.parse_args()

if args.include_dir is None:
    include_dir = os.path.abspath(os.path.join(root_dir, 'include', 'pym'))
else:
    include_dir = args.include_dir

if args.source_dir is None:
    source_dir = root_dir
else:
    source_dir = args.source_dir

subname_list = [
    'misc',
    'combopt',
    'logic',
    'clib',
    'bn',
    'aig',
    'sat',
    'fraig',
]
submodule_list = []
src_list = ['ymworks_module.cc']
for subname in subname_list:
    src_list.append(f'${{{subname}_SOURCES}}')
    submodule = importlib.import_module(subname)
    module_gen = ModuleGen(modulename=subname,
                           submodule=True,
                           pyclass_gen_list=submodule.gen_list,
                           namespace='YM')
    module_gen.make_all(include_dir=include_dir,
                        source_dir=source_dir)
    init_func = f'PyInit_{subname}'
    submodule_list.append((subname, init_func))

module_gen = ModuleGen(modulename="ymworks",
                       submodule_list=submodule_list,
                       namespace='YM',
                       target_list=[YmAddObjectLibraryTarget(objname='py_ymworks',
                                                             src_list=src_list),
                                    AddLibraryTarget(libname='ymworkslib',
                                                     shared=True,
                                                     src_list=['$<TARGET_OBJECTS:py_ymworks_obj>',
                                                               '${YM_SUBMODULE_OBJ_LIST}'],
                                                     link_list=['${YM_LIB_DEPENDS}',
                                                                '${Python3_LIBRARIES}'],
                                                    property_list=[('PREFIX', '""'),
                                                                   ('OUTPUT_NAME', '"ymworks"')])])
module_gen.make_all(include_dir=include_dir,
                    source_dir=source_dir)
