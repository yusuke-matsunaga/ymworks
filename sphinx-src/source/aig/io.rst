ファイル入出力
~~~~~~~~~~~~~~~~~~

現時点では以下のファイル形式からの読み込みに対応している．

- 'aag': ASCII AIGER 形式
- 'aig': AIGER 形式
- 'blif': blif 形式
- 'iscas89(.bench)': iscas89 形式

`blif` と `iscas89` 形式に関しては一旦 BnModel として読み込んで
から AigMgr に変換している．
その際に2入力ノードへ分解を行っている．

ファイル出力に関しては以下の形式に対応している．

- 'aag': ASCII AIGER 形式
- 'aig': AIGER 形式

aag 形式と aig 形式の詳細は :doc:`aig_format` を参照．
