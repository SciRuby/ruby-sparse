VALUE dia_init(int argc, VALUE* argv, VALUE self) {
  dia_matrix* mat;
  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, mat);

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
  }

  return self;
}


VALUE dia_alloc(VALUE klass) {
  dia_matrix* mat = ALLOC(dia_matrix);

  return TypedData_Wrap_Struct(klass, &dia_data_type, mat);
}


void dia_free(void* ptr) {
  dia_matrix *mat = (dia_matrix*)ptr;
  if (mat->shape) xfree(mat->shape);
  if (mat->elements) xfree(mat->elements);
  xfree(mat);
}

size_t dia_memsize(const void* ptr) {
  dia_matrix *mat = (dia_matrix*)ptr;
  size_t size = sizeof(mat);
  if (mat->shape) size += mat->ndims;
  if (mat->elements) size += mat->count;
  return size;
}


VALUE dia_get_elements(VALUE self) {
  dia_matrix* input;

  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, input);

  VALUE* array = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++) {
    array[index] = DBL2NUM(input->elements[index]);
  }

  return rb_ary_new4(input->count, array);
}


VALUE dia_get_count(VALUE self) {
  dia_matrix* input;

  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, input);

  return SIZET2NUM(input->count);
}


VALUE dia_get_ndims(VALUE self) {
  dia_matrix* input;

  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, input);

  return SIZET2NUM(input->ndims);
}


VALUE dia_get_dtype(VALUE self) {
  dia_matrix* input;

  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, input);

  return ID2SYM(rb_intern(DTYPE_NAMES[input->dtype]));
}


VALUE dia_get_shape(VALUE self) {
  dia_matrix* input;

  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, input);

  VALUE* array = ALLOC_N(VALUE, input->ndims);
  for (size_t index = 0; index < input->ndims; index++) {
    array[index] = SIZET2NUM(input->shape[index]);
  }

  return rb_ary_new4(input->ndims, array);
}


#include "elementwise.c"