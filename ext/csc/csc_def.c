VALUE csc_init(int argc, VALUE* argv, VALUE self) {
  csc_matrix* mat;
  Data_Get_Struct(self, csc_matrix, mat);

  if(argc > 0){
    mat->dtype = sp_float64;
    mat->ndims = 2;
    mat->count = (size_t)RARRAY_LEN(argv[1]);
    mat->shape = ALLOC_N(size_t, mat->ndims);
    for(size_t index = 0; index < mat->ndims; index++) {
      mat->shape[index] = (size_t)FIX2LONG(RARRAY_AREF(argv[0], index));
    }
    mat->elements = ALLOC_N(double, mat->count);
    mat->ia = ALLOC_N(size_t, mat->count);
    mat->jp = ALLOC_N(size_t, mat->shape[1] + 1);
    for(size_t index = 0; index <= mat->shape[1]; index++) mat->jp[index] = 0;
    for(size_t index = 0; index < mat->count; index++) {
      size_t col_index_val = (size_t)NUM2SIZET(RARRAY_AREF(argv[3], index));
      mat->jp[col_index_val] += 1;
    }
    for(size_t index = 0, cumsum = 0; index < mat->shape[1]; index++) {
      size_t temp = mat->jp[index];
      mat->jp[index] = cumsum;
      cumsum += temp;
    }
    mat->jp[mat->shape[1]] = mat->count;
    for(size_t index = 0; index < mat->count; index++) {
      size_t col = (size_t)NUM2SIZET(RARRAY_AREF(argv[3], index));
      size_t dest = mat->jp[col];

      mat->ia[dest] = (size_t)NUM2SIZET(RARRAY_AREF(argv[2], index));
      mat->elements[dest] = (double)NUM2DBL(RARRAY_AREF(argv[1], index));
      
      mat->jp[col] += 1;
    }
    
    for(size_t index = 0, last = 0; index <= mat->shape[1]; index++) {
      size_t temp = mat->jp[index];
      mat->jp[index] = last;
      last = temp;
    }
  }

  return self;
}


VALUE csc_alloc(VALUE klass) {
  csc_matrix* mat = ALLOC(csc_matrix);

  return Data_Wrap_Struct(klass, NULL, csc_free, mat);
}


void csc_free(csc_matrix* mat) {
  xfree(mat);
}


VALUE csc_get_elements(VALUE self) {
  csc_matrix* input;

  Data_Get_Struct(self, csc_matrix, input);

  VALUE* array = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array[index] = DBL2NUM(input->elements[index]);
  }

  return rb_ary_new4(input->count, array);
}


VALUE csc_get_indices(VALUE self) {
  csc_matrix* input;

  Data_Get_Struct(self, csc_matrix, input);

  VALUE* array_ia = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array_ia[index] = SIZET2NUM(input->ia[index]);
  }

  return rb_ary_new4(input->count, array_ia);
}


VALUE csc_get_indptr(VALUE self) {
  csc_matrix* input;

  Data_Get_Struct(self, csc_matrix, input);

  VALUE* array_jp = ALLOC_N(VALUE, input->shape[1] + 1);
  for (size_t index = 0; index <= input->shape[1]; index++) {
    array_jp[index] = SIZET2NUM(input->jp[index]);
  }

  return rb_ary_new4(input->shape[1] + 1, array_jp);
}


VALUE csc_get_count(VALUE self) {
  csc_matrix* input;

  Data_Get_Struct(self, csc_matrix, input);

  return SIZET2NUM(input->count);
}


VALUE csc_get_ndims(VALUE self) {
  csc_matrix* input;

  Data_Get_Struct(self, csc_matrix, input);

  return SIZET2NUM(input->ndims);
}


VALUE csc_get_dtype(VALUE self) {
  csc_matrix* input;

  Data_Get_Struct(self, csc_matrix, input);

  return ID2SYM(rb_intern(DTYPE_NAMES[input->dtype]));
}


VALUE csc_get_shape(VALUE self) {
  csc_matrix* input;

  Data_Get_Struct(self, csc_matrix, input);

  VALUE* array = ALLOC_N(VALUE, input->ndims);
  for (size_t index = 0; index < input->ndims; index++) {
    array[index] = SIZET2NUM(input->shape[index]);
  }

  return rb_ary_new4(input->ndims, array);
}


#include "elementwise.c"