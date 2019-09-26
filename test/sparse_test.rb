require 'test_helper'

class COO::CreationTest < Minitest::Test

  def setup
    @i = COO.new [2,2],[1, 4.2, 3,4]
  end

  def test_dims
    assert_equal [2,2], @i.shape
  end

  def test_elements
    assert_equal [1, 4.2, 3, 4], @i.elements
  end

end