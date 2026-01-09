#! /usr/bin/env python3

""" clib サブモジュール生成用初期化スクリプト

:file: __init__.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from .clibarea_gen import ClibAreaGen
from .clibenum_gen import ClibTechnologyGen
from .clibenum_gen import ClibDelayModelGen
from .clibenum_gen import ClibDirectionGen
from .clibenum_gen import ClibTimingSenseGen
from .clibenum_gen import ClibTimingTypeGen
from .clibenum_gen import ClibVarTypeGen
from .clibenum_gen import ClibCellTypeGen
from .clibenum_gen import ClibCPVGen
from .clibenum_gen import ClibPatTypeGen
from .clibbundle_gen import ClibBundleGen
from .clibbus_gen import ClibBusGen
from .clibbustype_gen import ClibBusTypeGen
from .clibcapacitance_gen import ClibCapacitanceGen
from .clibcell_gen import ClibCellGen
from .clibcellclass_gen import ClibCellClassGen
from .clibcellgroup_gen import ClibCellGroupGen
from .clibcelllibrary_gen import ClibCellLibraryGen
from .clibiomap_gen import ClibIOMapGen
from .cliblut_gen import ClibLutGen
from .clibpatgraph_gen import ClibPatGraphGen
from .clibpin_gen import ClibPinGen
from .clibpinmap_gen import ClibPinMapGen
from .clibresistance_gen import ClibResistanceGen
from .clibseqattr_gen import ClibSeqAttrGen
from .clibtime_gen import ClibTimeGen
from .clibtiming_gen import ClibTimingGen


gen_list = [
    ClibAreaGen(),
    ClibTechnologyGen(),
    ClibDelayModelGen(),
    ClibDirectionGen(),
    ClibTimingSenseGen(),
    ClibTimingTypeGen(),
    ClibVarTypeGen(),
    ClibCellTypeGen(),
    ClibCPVGen(),
    ClibPatTypeGen(),
    ClibBundleGen(),
    ClibBusGen(),
    ClibBusTypeGen(),
    ClibCapacitanceGen(),
    ClibCellGen(),
    ClibCellClassGen(),
    ClibCellGroupGen(),
    ClibCellLibraryGen(),
    ClibLutGen(),
    ClibIOMapGen(),
    ClibPatGraphGen(),
    ClibPinGen(),
    ClibPinMapGen(),
    ClibResistanceGen(),
    ClibSeqAttrGen(),
    ClibTimeGen(),
    ClibTimingGen(),
]
