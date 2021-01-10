require_relative 'test_helper'

class RubySparse::COO::CreationTest < Minitest::Test

  def setup
    @n = RubySparse::COO.new [3, 3], [1, 2, 3], [0, 1, 2], [0, 1, 2]
  end

  def test_ndims
    assert_equal 2, @n.dim
  end

  def test_shape
    assert_equal [3, 3], @n.shape
  end

  def test_elements
    assert_equal [1, 2, 3], @n.elements
  end

  def test_coords
    assert_equal [[0, 1, 2], [0, 1, 2]], @n.coords
  end

  def test_count
    assert_equal 3, @n.nzcount
  end

end

class RubySparse::DIA::CreationTest < Minitest::Test

  def setup
    @n = RubySparse::DIA.new [3, 3], [1, 0, 3]
  end

  def test_ndims
    assert_equal 2, @n.dim
  end

  def test_shape
    assert_equal [3, 3], @n.shape
  end

  def test_elements
    assert_equal [1, 0, 3], @n.elements
  end

end