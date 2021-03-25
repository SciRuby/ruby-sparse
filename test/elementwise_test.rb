require_relative 'test_helper'

class RubySparse::COO::ElementWiseTest < Minitest::Test

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

  def test_sub
    result = RubySparse::COO.new [3, 3], [1, -3, 2, -2], [0, 0, 1, 1], [0, 2, 1, 2]
    answer = @left - @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.coords, result.coords
  end

  def test_mul
    result = RubySparse::COO.new [3, 3], [9], [2], [2]
    answer = @left * @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.coords, result.coords
  end

end

class RubySparse::DIA::ElementWiseTest < Minitest::Test

  def setup
    @left = RubySparse::DIA.new [3, 3], [1, 0, 3]
    @right = RubySparse::DIA.new [3, 3], [3, 2, 3]
  end

  def test_add
    result = RubySparse::DIA.new [3, 3], [4, 2, 6]
    answer = @left + @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.elements, result.elements
  end

  def test_sub
    result = RubySparse::DIA.new [3, 3], [-2, -2, 0]
    answer = @left - @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.elements, result.elements
  end

  def test_mul
    result = RubySparse::DIA.new [3, 3], [3, 0, 9]
    answer = @left * @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.elements, result.elements
  end

end

class RubySparse::CSR::ElementWiseTest < Minitest::Test

  def setup
    @left = RubySparse::CSR.new [3, 3], [1, 2, 3], [0, 1, 2], [0, 1, 2]
    @right = RubySparse::CSR.new [3, 3], [3, 2, 3], [0, 1, 2], [2, 2, 2]
  end

  def test_add
    result = RubySparse::CSR.new [3, 3], [1, 3, 2, 2, 6], [0, 0, 1, 1, 2], [0, 2, 1, 2, 2]
    answer = @left + @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.indices, result.indices
    assert_equal answer.indptr, result.indptr
  end

  def test_sub
    result = RubySparse::CSR.new [3, 3], [1, -3, 2, -2], [0, 0, 1, 1], [0, 2, 1, 2]
    answer = @left - @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.indices, result.indices
    assert_equal answer.indptr, result.indptr
  end

  def test_mul
    result = RubySparse::CSR.new [3, 3], [9], [2], [2]
    answer = @left * @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.indices, result.indices
    assert_equal answer.indptr, result.indptr
  end

end

class RubySparse::CSC::ElementWiseTest < Minitest::Test

  def setup
    @left = RubySparse::CSC.new [3, 3], [1, 2, 3], [0, 1, 2], [0, 1, 2]
    @right = RubySparse::CSC.new [3, 3], [3, 2, 3], [0, 1, 2], [2, 2, 2]
  end

  def test_add
    result = RubySparse::CSC.new [3, 3], [1, 3, 2, 2, 6], [0, 0, 1, 1, 2], [0, 2, 1, 2, 2]
    answer = @left + @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.indices, result.indices
    assert_equal answer.indptr, result.indptr
  end

  def test_sub
    result = RubySparse::CSC.new [3, 3], [1, -3, 2, -2], [0, 0, 1, 1], [0, 2, 1, 2]
    answer = @left - @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.indices, result.indices
    assert_equal answer.indptr, result.indptr
  end

  def test_mul
    result = RubySparse::CSC.new [3, 3], [9], [2], [2]
    answer = @left * @right
    assert_equal answer.dim, result.dim
    assert_equal answer.shape, result.shape
    assert_equal answer.nzcount, result.nzcount
    assert_equal answer.elements, result.elements
    assert_equal answer.indices, result.indices
    assert_equal answer.indptr, result.indptr
  end

end
