VALUE csr_init(int argc, VALUE* argv, VALUE self) {
  csr_matrix* mat;
  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, mat);

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

  return TypedData_Wrap_Struct(klass, &csr_data_type, mat);
}


void csr_free(void* ptr) {
  csr_matrix *mat = (csr_matrix*)ptr;
  if (mat->shape) xfree(mat->shape);
  if (mat->elements) xfree(mat->elements);
  if (mat->ip) xfree(mat->ip);
  if (mat->ja) xfree(mat->ja);
  xfree(mat);
}

size_t csr_memsize(const void* ptr) {
  csr_matrix *mat = (csr_matrix*)ptr;
  size_t size = sizeof(mat);
  if (mat->shape) size += mat->ndims;
  if (mat->elements) size += mat->count;
  if (mat->ip) size += mat->shape[0]+1;
  if (mat->ja) size += mat->count;
  return size;
}


VALUE csr_get_elements(VALUE self) {
  csr_matrix* input;

  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, input);

  VALUE* array = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array[index] = DBL2NUM(input->elements[index]);
  }

  return rb_ary_new4(input->count, array);
}


VALUE csr_get_indices(VALUE self) {
  csr_matrix* input;

  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, input);

  VALUE* array_ja = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array_ja[index] = SIZET2NUM(input->ja[index]);
  }

  return rb_ary_new4(input->count, array_ja);
}


VALUE csr_get_indptr(VALUE self) {
  csr_matrix* input;

  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, input);

  VALUE* array_ip = ALLOC_N(VALUE, input->shape[0] + 1);
  for (size_t index = 0; index <= input->shape[0]; index++) {
    array_ip[index] = SIZET2NUM(input->ip[index]);
  }

  return rb_ary_new4(input->shape[0] + 1, array_ip);
}


VALUE csr_get_count(VALUE self) {
  csr_matrix* input;

  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, input);

  return SIZET2NUM(input->count);
}


VALUE csr_get_ndims(VALUE self) {
  csr_matrix* input;

  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, input);

  return SIZET2NUM(input->ndims);
}


VALUE csr_get_dtype(VALUE self) {
  csr_matrix* input;

  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, input);

  return ID2SYM(rb_intern(DTYPE_NAMES[input->dtype]));
}


VALUE csr_get_shape(VALUE self) {
  csr_matrix* input;

  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, input);

  VALUE* array = ALLOC_N(VALUE, input->ndims);
  for (size_t index = 0; index < input->ndims; index++) {
    array[index] = SIZET2NUM(input->shape[index]);
  }

  return rb_ary_new4(input->ndims, array);
}

VALUE csr_to_coo(VALUE self) {
  csr_matrix* left;
  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, left);

  coo_matrix* result = ALLOC(coo_matrix);
  result->dtype = left->dtype;
  result->count = left->count;
  result->ndims = left->ndims;
  result->shape = ALLOC_N(size_t, result->ndims);

  for (size_t index = 0; index < result->ndims; index++) {
    result->shape[index] = left->shape[index];
  }

  result->elements = ALLOC_N(double, left->count);
  result->ia       = ALLOC_N(size_t, left->count);
  result->ja       = ALLOC_N(size_t, left->count);

  for(size_t i = 0; i < left->shape[0]; i++) {
    for(size_t j = left->ip[i]; j < left->ip[i + 1]; j++) {
      // size_t index = (left->shape[1] * i) + left->ja[j];
      result->elements[j] = left->elements[j];
      result->ia[j] = i;
      result->ja[j] = left->ja[j];
    }
  }

  return TypedData_Wrap_Struct(COO, &coo_data_type, result);
}


#include "elementwise.c"