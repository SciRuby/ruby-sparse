module RubySparse

  class SparseArray

    # attr_reader :shape, :elements
  
    # def initialize shape, elements
    #   @shape = shape
    #   @elements = elements
    # end
  
  end

  class COO < SparseArray

    # attr_reader :shape, :elements
  
    # def initialize shape, elements
    #   @shape = shape
    #   @elements = elements
    # end

    def self.from_nmatrix(nmat)
      nm_elements = nmat.elements
      nm_shape = nmat.shape

      if nmat.dim != 2
        raise StandardError.new "NMatrix must be of 2 dimensions."
      end

      coo_elements = []
      coo_ia = []
      coo_ja = []

      for i in (0...nmat.shape[0])
        for j in (0...nmat.shape[1])
          nm_index = (nmat.shape[1] * i) + j
          nm_value = nm_elements[nm_index]
          if nm_value == 0
            next
          end
          coo_elements.append(nm_value)
          coo_ia.append(i)
          coo_ja.append(j)
        end
      end

      coo_mat = self.new nm_shape, coo_elements, coo_ia, coo_ja
      return coo_mat
    end

    def to_nmatrix
      ia = self.coords[0]
      ja = self.coords[1]

      nm_elements = Array.new(self.shape[0]*self.shape[1], 0)
      for i in (0...self.nzcount)
        nm_index = (self.shape[1] * ia[i]) + ja[i]
        nm_elements[nm_index] = self.elements[i]
      end
      
      m = NMatrix.new self.shape, nm_elements
      return m
    end
    
  end

  class DIA < SparseArray

    # attr_reader :shape, :elements
  
    # def initialize shape, elements
    #   @shape = shape
    #   @elements = elements
    # end

  end
end