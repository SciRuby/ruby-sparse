VALUE dia_each(VALUE self) {
  dia_matrix* input;

  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, input);

  for (size_t index = 0; index < input->count; index++) {
    rb_yield(DBL2NUM(input->elements[index]));
  }

  return self;
}