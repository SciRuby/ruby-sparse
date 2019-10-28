// Not used right now, but might be handy later
VALUE coo_from_nmatrix(VALUE self, VALUE nmat) {
  VALUE r_elements = rb_funcall(nmat, rb_intern("elements"), 0);
  VALUE r_shape = rb_funcall(nmat, rb_intern("shape"), 0);

  if((size_t)RARRAY_LEN(r_shape) != 2) {
    rb_raise(rb_eStandardError, "NMatrix must be of 2 dimensions.");
  }
  
  coo_matrix* mat;

  mat->ndims = 2;
  mat->count = 0;
  mat->shape = ALLOC_N(size_t, mat->ndims);
  for(size_t index = 0; index < mat->ndims; index++) {
    mat->shape[index] = (size_t)NUM2SIZET(RARRAY_AREF(r_shape, index));
  }

  size_t dense_elements_count = (mat->shape[0]) * (mat->shape[1]);
  size_t sparse_elements_count = 0;
  for(size_t index = 0; index < dense_elements_count; ++index) {
    double value = (double)NUM2DBL(RARRAY_AREF(r_elements, index));
      if(fabs(value - 0.0) < 1e-6)
        continue;
      sparse_elements_count++;
  }

  mat->count = sparse_elements_count;
  mat->elements = ALLOC_N(double, mat->count);
  mat->ia = ALLOC_N(size_t, mat->count);
  mat->ja = ALLOC_N(size_t, mat->count);

  size_t element_index = 0, index = 0;
  for(size_t i = 0; i < mat->shape[0]; ++i) {
    for(size_t j = 0; j < mat->shape[1]; ++j) {
      double value = (double)NUM2DBL(RARRAY_AREF(r_elements, element_index++));
      if(fabs(value - 0.0) < 1e-6)
        continue;
      mat->elements[index] = value;
      mat->ia[index] = i;
      mat->ja[index] = j;
      index++;
    }
  }

  return Data_Wrap_Struct(COO, NULL, coo_free, mat);
}