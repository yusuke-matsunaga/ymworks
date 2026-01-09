#! /usr/bin/env python3

""" AigLiteralArg の定義ファイル

:file: aig_arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ObjConvArg, TypedObjConvArg


class FraigMgrArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='FraigMgr',
                         cvardefault=None,
                         pyclassname='PyFraigMgr')


class FraigLiteralArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='FraigLiteral',
                         cvardefault=None,
                         pyclassname='PyFraigLiteral')


class FraigLiteralListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<FraigLiteral>',
                         cvardefault=None,
                         pyclassname='PyList<FraigLiteral, PyFraigLiteral>')
