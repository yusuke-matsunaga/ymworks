クラス一覧
~~~~~~~~~~~~~

多くのクラスがあるので大まかに以下のように分類している．

- 基本的なデータ型: `Val3`, `Literal`
- 論理式: `Expr`, `ExprWriter`
- 積和形論理式: `SopCube`, `SopCover`
- 真理値表: `TvFunc`, `TruthTable`
- NPN同値類: `NpnMap`, `NpnMapM`, `NpnVmap`
- Decision Diagrams: `Bdd`, `BddMgr`, `Zdd`, `ZddMgr`, ...
- Boolean Chain: `BcGraph`, `BcOp`

.. toctree::
   :maxdepth: 2

   common_grp
   expr_grp
   sop_grp
   tv_grp
   npn_grp
   dd_grp
   bc_grp
