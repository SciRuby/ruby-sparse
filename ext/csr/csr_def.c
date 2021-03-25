VALUE csr_init(int argc, VALUE* argv, VALUE self) {
  csr_matrix* mat;
  Data_Get_Struct(self, csr_matrix, mat);

  if(argc > 0){
    mat->dtype = sp_float64;
    mat->ndims = 2;
    mat->count = (size_t)RARRAY_LEN(argv[1]);
    mat->shape = ALLOC_N(size_t, mat->ndims);
    for(size_t index = 0; index < mat->ndims; index++) {
      mat->shape[index] = (size_t)FIX2LONG(RARRAY_AREF(argv[0], index));
    }
    mat->elements = ALLOC_N(double, mat->count);
    mat->ip = ALLOC_N(size_t, mat->shape[0] + 1);
    mat->ja = ALLOC_N(size_t, mat->count);
    for(size_t index = 0; index <= mat->shape[0]; index++) mat->ip[index] = 0;
    for(size_t index = 0; index < mat->count; index++) {
      size_t row_index_val = (size_t)NUM2SIZET(RARRAY_AREF(argv[2], index));
      mat->ip[row_index_val] += 1;
    }
    for(size_t index = 0, cumsum = 0; index < mat->shape[0]; index++) {
      size_t temp = mat->ip[index];
      mat->ip[index] = cumsum;
      cumsum += temp;
    }
    mat->ip[mat->shape[0]] = mat->count;
    for(size_t index = 0; index < mat->count; index++) {
      size_t row = (size_t)NUM2SIZET(RARRAY_AREF(argv[2], index));
      size_t dest = mat->ip[row];

      mat->ja[dest] = (size_t)NUM2SIZET(RARRAY_AREF(argv[3], index));
      mat->elements[dest] = (double)NUM2DBL(RARRAY_AREF(argv[1], index));
      
      mat->ip[row] += 1;
    }
    
    for(size_t index = 0, last = 0; index <= mat->shape[0]; index++) {
      size_t temp = mat->ip[index];
      mat->ip[index] = last;
      last = temp;
    }
  }

  return self;
}


VALUE csr_alloc(VALUE klass) {
  csr_matrix* mat = ALLOC(csr_matrix);

  return Data_Wrap_Struct(klass, NULL, csr_free, mat);
}


void csr_free(csr_matrix* mat) {
  xfree(mat);
}


VALUE csr_get_elements(VALUE self) {
  csr_matrix* input;

  Data_Get_Struct(self, csr_matrix, input);

  VALUE* array = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array[index] = DBL2NUM(input->elements[index]);
  }

  return rb_ary_new4(input->count, array);
}


VALUE csr_get_indices(VALUE self) {
  csr_matrix* input;

  Data_Get_Struct(self, csr_matrix, input);

  VALUE* array_ja = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array_ja[index] = SIZET2NUM(input->ja[index]);
  }

  return rb_ary_new4(input->count, array_ja);
}


VALUE csr_get_indptr(VALUE self) {
  csr_matrix* input;

  Data_Get_Struct(self, csr_matrix, input);

  VALUE* array_ip = ALLOC_N(VALUE, input->shape[0] + 1);
  for (size_t index = 0; index <= input->shape[0]; index++) {
    array_ip[index] = SIZET2NUM(input->ip[index]);
  }

  return rb_ary_new4(input->shape[0] + 1, array_ip);
}


VALUE csr_get_count(VALUE self) {
  csr_matrix* input;

  Data_Get_Struct(self, csr_matrix, input);

  return SIZET2NUM(input->count);
}


VALUE csr_get_ndims(VALUE self) {
  csr_matrix* input;

  Data_Get_Struct(self, csr_matrix, input);

  return SIZET2NUM(input->ndims);
}


VALUE csr_get_dtype(VALUE self) {
  csr_matrix* input;

  Data_Get_Struct(self, csr_matrix, input);

  return ID2SYM(rb_intern(DTYPE_NAMES[input->dtype]));
}


VALUE csr_get_shape(VALUE self) {
  csr_matrix* input;

  Data_Get_Struct(self, csr_matrix, input);

  VALUE* array = ALLOC_N(VALUE, input->ndims);
  for (size_t index = 0; index < input->ndims; index++) {
    array[index] = SIZET2NUM(input->shape[index]);
  }

  return rb_ary_new4(input->ndims, array);
}


#include "elementwise.c"