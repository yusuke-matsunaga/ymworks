#! /usr/bin/env python3

""" ファイルを生成するスクリプト

:file: mk_all.py
:author: Yusuke Matsunaga (松永 裕介)
:copyright: Copyright (C) 2025 Yusuke Matsunaga, All rights reserved.
"""

from .val3_gen import Val3Gen
from .primtype_gen import PrimTypeGen
from .literal_gen import LiteralGen
from .npnmap_gen import NpnMapGen
from .expr_gen import ExprGen
from .tvfunc_gen import TvFuncGen
from .sopcube_gen import SopCubeGen
from .sopcover_gen import SopCoverGen
from .bdd_gen import BddGen
from .bddvar_gen import BddVarGen
from .bddvarset_gen import BddVarSetGen
from .bddlit_gen import BddLitGen
from .bddcube_gen import BddCubeGen
from .bddmgr_gen import BddMgrGen
from .bddcompmap_gen import BddCompMapGen
from .bddvarmap_gen import BddVarMapGen
from .truthtable_gen import TruthTableGen
from .bcoptype_gen import BcOpTypeGen
from .bcop_gen import BcOpGen
from .bcedge_gen import BcEdgeGen
from .bcgraph_gen import BcGraphGen
from .djdec_gen import DjDecGen
from .logic_arg import Val3Arg
from .logic_arg import PrimTypeArg
from .logic_arg import LiteralArg, LiteralListArg
from .logic_arg import NpnMapArg
from .logic_arg import ExprArg, ExprListArg
from .logic_arg import TvFuncArg
from .logic_arg import SopCubeArg, SopCubeListArg
from .logic_arg import SopCoverArg, SopCoverListArg
from .logic_arg import BddMgrArg, BddArg, BddListArg
from .logic_arg import BddVarArg, BddVarListArg, BddVarSetArg
from .logic_arg import BddLitArg, BddLitListArg, BddCubeArg
from .logic_arg import BddCompMapArg, BddVarMapArg
from .logic_arg import TruthTableArg
from .logic_arg import BcOpTypeArg
from .logic_arg import BcEdgeArg


gen_list = [
    Val3Gen(),
    PrimTypeGen(),
    LiteralGen(),
    NpnMapGen(),
    ExprGen(),
    TvFuncGen(),
    SopCubeGen(),
    SopCoverGen(),
    BddGen(),
    BddVarGen(),
    BddLitGen(),
    BddVarSetGen(),
    BddCubeGen(),
    BddMgrGen(),
    BddCompMapGen(),
    BddVarMapGen(),
    TruthTableGen(),
    BcOpTypeGen(),
    BcOpGen(),
    BcEdgeGen(),
    BcGraphGen(),
    DjDecGen(),
]
