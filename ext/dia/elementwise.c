#include "string.h"
#include "math.h"


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
  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, left);
  TypedData_Get_Struct(another, dia_matrix, &dia_data_type, right);

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

  return TypedData_Wrap_Struct(DIA, &dia_data_type, result);
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

double dia_unary_oper(double val, const char* oper) {
  if (strcmp(oper, "sin") == 0)
    return sin(val);
  else if (strcmp(oper, "cos") == 0)
    return cos(val);
  else if (strcmp(oper, "tan") == 0)
    return tan(val);
  else
    return 0.00;
}

/*
  Takes the matrix and performs unary operator elementwise
*/
VALUE dia_elementwise_unary(VALUE self, const char* oper) {
  dia_matrix* left;
  TypedData_Get_Struct(self, dia_matrix, &dia_data_type, left);

  dia_matrix* result = ALLOC(dia_matrix);
  result->dtype = left->dtype;
  result->count = left->count;
  result->ndims = left->ndims;
  result->shape = ALLOC_N(size_t, result->ndims);

  for (size_t index = 0; index < result->ndims; index++) {
    result->shape[index] = left->shape[index];
  }

  result->elements = ALLOC_N(double, left->count);

  for(size_t index = 0; index < result->count; index++) {
    result->elements[index] = dia_unary_oper(left->elements[index], oper);
  }

  return TypedData_Wrap_Struct(DIA, &dia_data_type, result);
}

VALUE dia_sin(VALUE self) {
  return dia_elementwise_unary(self, "sin");
}

VALUE dia_cos(VALUE self) {
  return dia_elementwise_unary(self, "cos");
}

VALUE dia_tan(VALUE self) {
  return dia_elementwise_unary(self, "tan");
}
