require_relative '../lib/sparse.so'

# COO example

a = RubySparse::COO.new [2, 3], [1, 2], [0, 1], [0, 2]
b = RubySparse::COO.new [2, 3], [10, 20, 30], [0, 0, 1], [0, 1, 2]

c = a + b

p c

p c.elements
p c.shape
p c.coords