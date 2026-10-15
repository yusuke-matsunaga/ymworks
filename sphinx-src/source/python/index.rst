Python インターフェイス
========================

ymworks は `Python` からも利用するためのパッケージが
ビルドディレクトリの下の `ymworks/py_capi/ymworks.so`
に用意されている．
このファイルを `Python` インタプリタが見つけられるようにするためには
環境変数 `PYTHONPATH` にこのディレクトリを加えるか
`Python` 内のシステム変数 `sys.path` にこのディレクトリを加える必要が
ある．
どちらも少々複雑な操作が必要なので，予め `ymworks.so`
を組み込んだ `Python` インタプリタが
`ymworks/py_capi/pytest/py_ymworks` に用意されている．
なお，デバッグオプションを付けたコンパイルしたバージョンは
`ymworks/py_capi/pytest/py_ymworks_d` である．
`ymworks.so` のディレクトリを `PYTHONPATH` に加えて通常の
`Python` インタプリタを起動するのと
`py_ymworks` を起動するのはほぼ同じ動作をする．

`ymworks` パッケージを利用するためには他のパッケージ同様にインポートする
必要がある．

.. code-block:: python
   :caption: ymworks のインポート

   import ymworks

通常のパッケージ同様 `from` 文を使うこともできる．

.. code-block:: python
   :caption: from 文の使用例

   from ymworks.logic import Expr

   # Expr が使用可能になった．

`ymworks` パッケージには以下のサブモジュールが含まれる．

.. toctree::
   :maxdepth: 2

   aig
   bn
   clib
   combopt
   fraig
   logic
   misc
   sat
