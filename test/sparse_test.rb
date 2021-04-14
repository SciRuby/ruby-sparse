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

class RubySparse::CSR::CreationTest < Minitest::Test

  def setup
    @n = RubySparse::CSR.new [3, 3], [1, 2, 3], [0, 1, 2], [0, 1, 2]
    @m = RubySparse::CSR.new [4, 6], [1, 2, 3, 4, 5, 6, 7, 8], [0, 0, 1, 1, 2, 2, 2, 3], [0, 1, 1, 3, 2, 3, 4, 5]
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

  def test_indices
    assert_equal [0, 1, 1, 3, 2, 3, 4, 5], @m.indices
  end

  def test_indptr
    assert_equal [0, 2, 4, 7, 8], @m.indptr
  end

  def test_count
    assert_equal 3, @n.nzcount
  end

end

class RubySparse::CSC::CreationTest < Minitest::Test

  def setup
    @n = RubySparse::CSC.new [3, 3], [1, 2, 3], [0, 1, 2], [0, 1, 2]
    @m = RubySparse::CSC.new [4, 6], [1, 2, 3, 4, 5, 6, 7, 8], [0, 0, 1, 1, 2, 2, 2, 3], [0, 1, 1, 3, 2, 3, 4, 5]
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

  def test_indices
    assert_equal [0, 0, 1, 2, 1, 2, 2, 3], @m.indices
  end

  def test_indptr
    assert_equal [0, 1, 3, 4, 6, 7, 8], @m.indptr
  end

  def test_count
    assert_equal 3, @n.nzcount
  end

end