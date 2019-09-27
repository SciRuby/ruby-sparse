require 'test_helper'

class COO::ElementWiseTest < Minitest::Test

  def setup
    @left = RubySparse::COO.new [3, 3], [1, 2, 3], [0, 1, 2], [0, 1, 2]
    @right = RubySparse::COO.new [3, 3], [3, 2, 3], [0, 1, 2], [2, 2, 2]
  end

  def test_add
    result = RubySparse::COO.new [3, 3], [1, 3, 2, 2, 6], [0, 0, 1, 1, 2], [0, 2, 1, 2, 2]
    answer = @left + @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.coords, result.coords
  end

end