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

    def pretty_print(q)
      self.inspect.pretty_print(q)
    end

    def inspect #:nodoc:
      original_inspect = super()
      original_inspect = original_inspect[0...original_inspect.size-1]
      original_inspect + "; " + inspect_helper.join("; ") + ">"
    end

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

    def _dump data
      [
        dim,
        nzcount,
        dtype,
        shape,
        elements,
        coords[0],
        coords[1],
      ].join ":"
    end

    def self._load args
      values = args.split(":")
      dim = values[0].to_i
      nzcount = values[1].to_i
      dtype = values[2].to_sym
      shape = values[3...(3+dim)]
      elements = values[(3+dim)...(3+dim+nzcount)]
      ia = values[(3+dim+nzcount)...(3+dim+(2*nzcount))]
      ja = values[(3+dim+(2*nzcount))...(3+dim+(3*nzcount))]

      (0...dim).each { |index| shape[index] = shape[index].to_i }
      (0...nzcount).each { |index| elements[index] = elements[index].to_f }
      (0...nzcount).each { |index| ia[index] = ia[index].to_i }
      (0...nzcount).each { |index| ja[index] = ja[index].to_i }

      self.new(shape, elements, ia, ja)
    end

    protected
  
    def inspect_helper #:nodoc:
      ary = []
      ary << "shape: [#{shape.join(',')}]" << "dtype: #{dtype}" << "nnz: #{nzcount}"
      ary
    end
    
  end

  class DIA < SparseArray

    # attr_reader :shape, :elements
  
    # def initialize shape, elements
    #   @shape = shape
    #   @elements = elements
    # end

    def pretty_print(q)
      self.inspect.pretty_print(q)
    end

    def inspect #:nodoc:
      original_inspect = super()
      original_inspect = original_inspect[0...original_inspect.size-1]
      original_inspect + "; " + inspect_helper.join("; ") + ">"
    end

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

    def _dump data
      [
        dim,
        dtype,
        shape,
        elements,
      ].join ":"
    end

    def self._load args
      values = args.split(":")
      dim = values[0].to_i
      dtype = values[1].to_sym
      shape = values[2...(2+dim)]
      elements = values[(2+dim)...(2+dim+shape[0].to_i)]

      (0...dim).each { |index| shape[index] = shape[index].to_i }
      (0...shape[0]).each { |index| elements[index] = elements[index].to_f }

      self.new(shape, elements)
    end

    protected
  
    def inspect_helper #:nodoc:
      ary = []
      ary << "shape: [#{shape.join(',')}]" << "dtype: #{dtype}" << "nnz: #{nzcount}"
      ary
    end

  end

  class CSR < SparseArray

    # attr_reader :shape, :elements
  
    # def initialize shape, elements
    #   @shape = shape
    #   @elements = elements
    # end

    def pretty_print(q)
      self.inspect.pretty_print(q)
    end

    def inspect #:nodoc:
      original_inspect = super()
      original_inspect = original_inspect[0...original_inspect.size-1]
      original_inspect + "; " + inspect_helper.join("; ") + ">"
    end

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

    def to_csc
      return self.to_coo.to_csc
    end

    def _dump data
      [
        dim,
        nzcount,
        dtype,
        shape,
        elements,
        indptr,
        indices,
      ].join ":"
    end

    def self._load args
      values = args.split(":")
      dim = values[0].to_i
      nzcount = values[1].to_i
      dtype = values[2].to_sym
      shape = values[3...(3+dim)]
      elements = values[(3+dim)...(3+dim+nzcount)]
      ip = values[(3+dim+nzcount)...(3+dim+nzcount+shape[0].to_i)]
      ja = values[(3+dim+nzcount+shape[0].to_i)...(3+dim+(2*nzcount)+shape[0].to_i)]

      (0...dim).each { |index| shape[index] = shape[index].to_i }
      (0...nzcount).each { |index| elements[index] = elements[index].to_f }
      (0...shape[0]).each { |index| ip[index] = ip[index].to_i }
      (0...nzcount).each { |index| ja[index] = ja[index].to_i }

      # convert ip to ia here and then pass ia below instead

      self.new(shape, elements, ip, ja)
    end

    protected
  
    def inspect_helper #:nodoc:
      ary = []
      ary << "shape: [#{shape.join(',')}]" << "dtype: #{dtype}" << "nnz: #{nzcount}"
      ary
    end

  end

  class CSC < SparseArray

    # attr_reader :shape, :elements
  
    # def initialize shape, elements
    #   @shape = shape
    #   @elements = elements
    # end

    def pretty_print(q)
      self.inspect.pretty_print(q)
    end

    def inspect #:nodoc:
      original_inspect = super()
      original_inspect = original_inspect[0...original_inspect.size-1]
      original_inspect + "; " + inspect_helper.join("; ") + ">"
    end

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
        for i in (jp[j]...jp[j + 1])
          nm_index = (self.shape[0] * j) + ia[i]
          nm_elements[nm_index] = self.elements[i]
        end
      end
      
      m = NMatrix.new self.shape, nm_elements
      return m
    end

    def to_csr
      return self.to_coo.to_csr
    end

    def _dump data
      [
        dim,
        nzcount,
        dtype,
        shape,
        elements,
        indices,
        indptr,
      ].join ":"
    end

    def self._load args
      values = args.split(":")
      dim = values[0].to_i
      nzcount = values[1].to_i
      dtype = values[2].to_sym
      shape = values[3...(3+dim)]
      elements = values[(3+dim)...(3+dim+nzcount)]
      ia = values[(3+dim+nzcount)...(3+dim+(2*nzcount))]
      jp = values[(3+dim+(2*nzcount))...(3+dim+(2*nzcount)+shape[1].to_i)]

      (0...dim).each { |index| shape[index] = shape[index].to_i }
      (0...nzcount).each { |index| elements[index] = elements[index].to_f }
      (0...nzcount).each { |index| ia[index] = ia[index].to_i }
      (0...shape[1]).each { |index| jp[index] = jp[index].to_i }

      # convert jp to ja here and then pass ja below instead

      self.new(shape, elements, ia, jp)
    end

    protected
  
    def inspect_helper #:nodoc:
      ary = []
      ary << "shape: [#{shape.join(',')}]" << "dtype: #{dtype}" << "nnz: #{nzcount}"
      ary
    end

  end
end