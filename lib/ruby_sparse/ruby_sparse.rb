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

    def self.from_nmatrix(nmat)
      nm_elements = nmat.elements
      nm_shape = nmat.shape

      if nmat.dim != 2
        raise StandardError.new "NMatrix must be of 2 dimensions."
      end

      dia_elements = []

      for i in (0...nmat.shape[0])
        nm_index = (nmat.shape[1] * i) + i
        nm_value = nm_elements[nm_index]
        dia_elements.append(nm_value)
      end

      dia_mat = self.new nm_shape, dia_elements
      return dia_mat
    end

    def to_nmatrix
      nm_elements = Array.new(self.shape[0]*self.shape[1], 0)
      for i in (0...self.shape[0])
        nm_index = (self.shape[1] * i) + i
        nm_elements[nm_index] = self.elements[i]
      end
      
      m = NMatrix.new self.shape, nm_elements
      return m
    end

  end

  class CSR < SparseArray

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

      csr_elements = []
      csr_ia = []
      csr_ja = []

      for i in (0...nmat.shape[0])
        for j in (0...nmat.shape[1])
          nm_index = (nmat.shape[1] * i) + j
          nm_value = nm_elements[nm_index]
          if nm_value == 0
            next
          end
          csr_elements.append(nm_value)
          csr_ia.append(i)
          csr_ja.append(j)
        end
      end

      csr_mat = self.new nm_shape, csr_elements, csr_ia, csr_ja
      return csr_mat
    end

    def to_nmatrix
      ip = self.indptr
      ja = self.indices

      nm_elements = Array.new(self.shape[0]*self.shape[1], 0)
      for i in (0...self.shape[0])
        for j in (ip[i]...ip[i + 1])
          nm_index = (self.shape[1] * i) + ja[j]
          nm_elements[nm_index] = self.elements[j]
        end
      end
      
      m = NMatrix.new self.shape, nm_elements
      return m
    end

  end

  class CSC < SparseArray

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

      csc_elements = []
      csc_ia = []
      csc_ja = []

      for i in (0...nmat.shape[0])
        for j in (0...nmat.shape[1])
          nm_index = (nmat.shape[1] * i) + j
          nm_value = nm_elements[nm_index]
          if nm_value == 0
            next
          end
          csc_elements.append(nm_value)
          csc_ia.append(i)
          csc_ja.append(j)
        end
      end

      csc_mat = self.new nm_shape, csc_elements, csc_ia, csc_ja
      return csc_mat
    end

    def to_nmatrix
      ia = self.indices
      jp = self.indptr

      nm_elements = Array.new(self.shape[0]*self.shape[1], 0)
      for j in (0...self.shape[1])
        for i in (jp[i]...jp[i + 1])
          nm_index = (self.shape[0] * i) + ia[j]
          nm_elements[nm_index] = self.elements[j]
        end
      end
      
      m = NMatrix.new self.shape, nm_elements
      return m
    end

  end
end