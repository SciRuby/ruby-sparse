require 'test_helper'

class COO::ElementWiseTest < Minitest::Test

  def setup
    @left = COO.new  [2,2],[1,   4.2, 3,   4.2]
    @right = COO.new [2,2],[1.9, 5.2, 3.1, 4]
  end

  def test_add
    result = COO.new  [2,2],[2.9, 9.4, 6.1,   8.2]
    answer = @left + @right
    assert_equal answer.elements, result.elements
  end

  def test_subtract
    result = COO.new  [2,2],[-0.9, -1, -0.1,   0.2]
    answer = @left - @right
    assert_equal answer.elements, result.elements
  end

end