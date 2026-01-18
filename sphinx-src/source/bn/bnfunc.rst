BnFunc
~~~~~~~~~

`BnNode` の局所関数を表すためのクラス．
ユーザーがこのオブジェクトを明示的に作ることはできない．
`BnModel::new_primitive()` や `BnMode::new_cover()`
などを実行した結果， `BnModel` 内部で生成される．
そのため，このオブジェクトに関するリソースの管理は
親の `BnModel` に任されている．

.. doxygenclass:: BnFunc
   :members:
