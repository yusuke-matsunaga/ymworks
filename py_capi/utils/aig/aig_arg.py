#! /usr/bin/env python3

""" AigLiteralArg の定義ファイル

:file: aig_arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ObjConvArg, TypedObjConvArg


class AigMgrArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='AigMgr',
                         cvardefault=None,
                         pyclassname='PyAigMgr')


class AigLiteralArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='AigLiteral',
                         cvardefault=None,
                         pyclassname='PyAigLiteral')


class AigLiteralListArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::vector<AigLiteral>',
                         cvardefault=None,
                         pyclassname='PyList<AigLiteral, PyAigLiteral>')
