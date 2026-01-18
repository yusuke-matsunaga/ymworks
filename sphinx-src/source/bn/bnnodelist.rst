BnNodeList
~~~~~~~~~~~~

意味的には `std::vector<BnNode>` と同じ情報を表すがこちらは不変であり，
軽量である．
`BnNodeIter` は `BnNodeList` を範囲for文 ( `for ( auto x: node_list)` )
で使用可能にするための反復子であり通常は明示的に使うことはない．
`BnNodeIter2` は `BnNodeList` を Python から使用可能にするための実装用
に用意されているもので通常は使うことはない．

.. doxygenclass:: BnNodeList
   :members:

.. doxygenclass:: BnNodeIter
   :members:

.. doxygenclass:: BnNodeIter2
   :members:
