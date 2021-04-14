VALUE csc_each(VALUE self) {
  csc_matrix* input;

  TypedData_Get_Struct(self, csc_matrix, &csc_data_type, input);

  for (size_t index = 0; index < input->count; index++) {
    rb_yield(DBL2NUM(input->elements[index]));
  }

  return self;
}

VALUE csc_each_column(VALUE self) {
  csc_matrix* input;

  TypedData_Get_Struct(self, csc_matrix, &csc_data_type, input);

  for (size_t j = 0; j < input->shape[1]; j++) {
    size_t col_len = input->jp[j + 1] - input->jp[j];
    VALUE* col_array = ALLOC_N(VALUE, col_len);

    for(size_t i = input->jp[j]; i < input->jp[j + 1]; i++) {
      col_array[i - input->jp[j]] = DBL2NUM(input->elements[i]);
    }

    rb_yield(rb_ary_new4(col_len, col_array));
  }

  return self;
}

VALUE csc_each_with_indices(VALUE self) {
  csc_matrix* input;

  TypedData_Get_Struct(self, csc_matrix, &csc_data_type, input);

  VALUE* result_array = ALLOC_N(VALUE, input->ndims + 1);

  for (size_t j = 0; j < input->shape[1]; j++) {
    size_t col_index = j;
    for(size_t i = input->jp[j]; i < input->jp[j + 1]; i++) {
      size_t row_index = input->ia[i];
      size_t index = i;
      result_array[0] = DBL2NUM(input->elements[index]);
      result_array[1] = INT2NUM(row_index);
      result_array[2] = INT2NUM(col_index);
      rb_yield(rb_ary_new4(input->ndims + 1, result_array));

    }
  }

  return self;
}