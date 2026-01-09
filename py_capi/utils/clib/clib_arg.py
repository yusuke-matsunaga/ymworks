#! /usr/bin/env python3

"""

:file: clib_arg.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from mk_py_capi import ObjConvArg, TypedObjConvArg


class ClibAreaArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibArea',
                         cvardefault=None,
                         pyclassname='PyClibArea')


class ClibTechnologyArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibTechnology',
                         cvardefault=None,
                         pyclassname='PyClibTechnology')


class ClibDelayModelArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibDelayModel',
                         cvardefault=None,
                         pyclassname='PyClibDelayModel')


class ClibDirectionArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibDirection',
                         cvardefault=None,
                         pyclassname='PyClibDirection')


class ClibTimingSenseArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibTimingSense',
                         cvardefault=None,
                         pyclassname='PyClibTimingSense')


class ClibTimingTypeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibTimingType',
                         cvardefault=None,
                         pyclassname='PyClibTimingType')


class ClibVarTypeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibVarType',
                         cvardefault=None,
                         pyclassname='PyClibVarType')


class ClibCellTypeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibCellType',
                         cvardefault=None,
                         pyclassname='PyClibCellType')


class ClibCPVArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibCPV',
                         cvardefault=None,
                         pyclassname='PyClibCPV')


class ClibPatTypeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibPatType',
                         cvardefault=None,
                         pyclassname='PyClibPatType')


class ClibBundleArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibBundle',
                         cvardefault=None,
                         pyclassname='PyClibBundle')


class ClibBusArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibBus',
                         cvardefault=None,
                         pyclassname='PyClibBus')


class ClibBusTypeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibBusType',
                         cvardefault=None,
                         pyclassname='PyClibBusType')


class ClibCapacitanceArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibCapacitance',
                         cvardefault=None,
                         pyclassname='PyClibCapacitance')


class ClibCellArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibCell',
                         cvardefault=None,
                         pyclassname='PyClibCell')


class ClibCellClassArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibCellClass',
                         cvardefault=None,
                         pyclassname='PyClibCellClass')


class ClibCellGroupArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibCellGroup',
                         cvardefault=None,
                         pyclassname='PyClibCellGroup')


class ClibCellLibraryArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibCellLibrary',
                         cvardefault=None,
                         pyclassname='PyClibCellLibrary')


class ClibIOMapArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibIOMap',
                         cvardefault=None,
                         pyclassname='PyClibIOMap')


class ClibPatGraphArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibPatGraph',
                         cvardefault=None,
                         pyclassname='PyClibPatGraph')


class ClibPinArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibPin',
                         cvardefault=None,
                         pyclassname='PyClibPin')


class ClibResistanceArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibResistance',
                         cvardefault=None,
                         pyclassname='PyClibResistance')


class ClibSeqAttrArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibSeqAttr',
                         cvardefault=None,
                         pyclassname='PyClibSeqAttr')


class ClibTimeArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibTime',
                         cvardefault=None,
                         pyclassname='PyClibTime')


class ClibTimingArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibTiming',
                         cvardefault=None,
                         pyclassname='PyClibTiming')


class ClibLutArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibLut',
                         cvardefault=None,
                         pyclassname='PyClibLut')


class ClibPinMapArg(TypedObjConvArg):

    def __init__(self, *,
                 name=None,
                 cvarname):
        super().__init__(name=name,
                         cvarname=cvarname,
                         cvartype='ClibPinMap',
                         cvardefault=None,
                         pyclassname='PyClibPinMap')
