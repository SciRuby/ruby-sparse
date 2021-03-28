VALUE coo_init(int argc, VALUE* argv, VALUE self) {
  coo_matrix* mat;
  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, mat);

  if(argc > 0){
    mat->dtype = sp_float64;
    mat->ndims = 2;
    mat->count = (size_t)RARRAY_LEN(argv[1]);
    mat->shape = ALLOC_N(size_t, mat->ndims);
    for(size_t index = 0; index < mat->ndims; index++) {
      mat->shape[index] = (size_t)FIX2LONG(RARRAY_AREF(argv[0], index));
    }
    mat->elements = ALLOC_N(double, mat->count);
    for(size_t index = 0; index < mat->count; index++) {
      mat->elements[index] = (double)NUM2DBL(RARRAY_AREF(argv[1], index));
    }
    mat->ia = ALLOC_N(size_t, mat->count);
    for(size_t index = 0; index < mat->count; index++) {
      mat->ia[index] = (size_t)NUM2SIZET(RARRAY_AREF(argv[2], index));
    }
    mat->ja = ALLOC_N(size_t, mat->count);
    for(size_t index = 0; index < mat->count; index++) {
      mat->ja[index] = (size_t)NUM2SIZET(RARRAY_AREF(argv[3], index));
    }
  }

  return self;
}


VALUE coo_alloc(VALUE klass) {
  coo_matrix* mat = ALLOC(coo_matrix);

  return TypedData_Wrap_Struct(klass, &coo_data_type, mat);
}


void coo_free(void* ptr) {
  coo_matrix *mat = (coo_matrix*)ptr;
  if (mat->shape) xfree(mat->shape);
  if (mat->elements) xfree(mat->elements);
  if (mat->ia) xfree(mat->ia);
  if (mat->ja) xfree(mat->ja);
  xfree(mat);
}

size_t coo_memsize(const void* ptr) {
  coo_matrix *mat = (coo_matrix*)ptr;
  size_t size = sizeof(mat);
  if (mat->shape) size += mat->ndims;
  if (mat->elements) size += mat->count;
  if (mat->ia) size += mat->count;
  if (mat->ja) size += mat->count;
  return size;
}


VALUE coo_get_elements(VALUE self) {
  coo_matrix* input;

  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, input);

  VALUE* array = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array[index] = DBL2NUM(input->elements[index]);
  }

  return rb_ary_new4(input->count, array);
}


VALUE coo_get_coords(VALUE self) {
  coo_matrix* input;

  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, input);

  VALUE* array_ia = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array_ia[index] = SIZET2NUM(input->ia[index]);
  }

  VALUE* array_ja = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array_ja[index] = SIZET2NUM(input->ja[index]);
  }

  VALUE ia = rb_ary_new4(input->count, array_ia);
  VALUE ja = rb_ary_new4(input->count, array_ja);

  return rb_ary_new3(2, ia, ja);
}

VALUE coo_get_count(VALUE self) {
  coo_matrix* input;

  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, input);

  return SIZET2NUM(input->count);
}


VALUE coo_get_ndims(VALUE self) {
  coo_matrix* input;

  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, input);

  return SIZET2NUM(input->ndims);
}


VALUE coo_get_dtype(VALUE self) {
  coo_matrix* input;

  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, input);

  return ID2SYM(rb_intern(DTYPE_NAMES[input->dtype]));
}


VALUE coo_get_shape(VALUE self) {
  coo_matrix* input;

  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, input);

  VALUE* array = ALLOC_N(VALUE, input->ndims);
  for (size_t index = 0; index < input->ndims; index++) {
    array[index] = SIZET2NUM(input->shape[index]);
  }

  return rb_ary_new4(input->ndims, array);
}


#include "elementwise.c"