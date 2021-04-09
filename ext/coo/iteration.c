VALUE coo_each(VALUE self) {
  coo_matrix* input;

  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, input);

  for (size_t index = 0; index < input->count; index++) {
    rb_yield(DBL2NUM(input->elements[index]));
  }

  return self;
}

VALUE coo_each_with_indices(VALUE self) {
  coo_matrix* input;

  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, input);

  VALUE* result_array = ALLOC_N(VALUE, input->ndims + 1);
  for (size_t index = 0; index < input->count; index++) {
    result_array[0] = DBL2NUM(input->elements[index]);
    result_array[1] = INT2NUM(input->ia[index]);
    result_array[2] = INT2NUM(input->ja[index]);
    rb_yield(rb_ary_new4(input->ndims + 1, result_array));
  }

  return self;
}