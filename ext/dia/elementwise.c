double dia_perform_oper(double val_a, double val_b, char oper) {
  switch(oper) {
    case '+':
      return (val_a + val_b);
    case '-':
      return (val_a - val_b);
    case '*':
      return (val_a * val_b);
    default:  //unknown operator
      return 0.00;
  }
}


/*
  Takes two matrices and performs operator elementwise
*/
VALUE dia_elementwise_binary(VALUE self, VALUE another, char oper) {
  dia_matrix* left;
  dia_matrix* right;
  Data_Get_Struct(self, dia_matrix, left);
  Data_Get_Struct(another, dia_matrix, right);

  dia_matrix* result = ALLOC(dia_matrix);
  result->count = left->count;
  result->ndims = left->ndims;
  result->shape = ALLOC_N(size_t, result->ndims);

  for (size_t index = 0; index < result->ndims; index++) {
    result->shape[index] = left->shape[index];
  }

  result->elements = ALLOC_N(double, left->count);

  for(size_t result_index = 0; result_index < result->count; result_index++) {
    double result_val = dia_perform_oper(left->elements[result_index], right->elements[result_index], oper);
    result->elements[result_index] = result_val;
  }

  return Data_Wrap_Struct(DIA, NULL, dia_free, result);
}

VALUE dia_add(VALUE self, VALUE another) {
  return dia_elementwise_binary(self, another, '+');
}

VALUE dia_sub(VALUE self, VALUE another) {
  return dia_elementwise_binary(self, another, '-');
}

VALUE dia_mul(VALUE self, VALUE another) {
  return dia_elementwise_binary(self, another, '*');
}