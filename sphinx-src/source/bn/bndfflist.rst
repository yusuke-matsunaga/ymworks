BnDffList
~~~~~~~~~~~~

意味的には `std::vector<BnDff>` と同じ情報を表すがこちらは不変であり，
軽量である．
`BnDffIter` は `BnDffList` を範囲for文 ( `for ( auto x: dff_list)` )
で使用可能にするための反復子であり通常は明示的に使うことはない．
`BnDffIter2` は `BnDffList` を Python から使用可能にするための実装用
に用意されているもので通常は使うことはない．

.. doxygenclass:: BnDffList
   :members:

.. doxygenclass:: BnDffIter
   :members:

.. doxygenclass:: BnDffIter2
   :members:
