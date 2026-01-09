#! /usr/bin/env python3

"""

:file: misc_arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ObjConvArg, TypedObjConvArg, TypedObjRefArg


class JsonValueArg(ObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='JsonValue',
                         cvardefault=None,
                         pyclassname='PyJsonValue')


class Mt19937Arg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='std::mt19937',
                         cvardefault=None,
                         pyclassname='PyMt19937')


class OutStreamArg(TypedObjRefArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         pyclassname='PyOutStream')


class InStreamArg(TypedObjRefArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         pyclassname='PyInStream')
