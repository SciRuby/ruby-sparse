#include "string.h"
#include "math.h"


double coo_perform_oper(double val_a, double val_b, char oper) {
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
VALUE coo_elementwise_binary(VALUE self, VALUE another, char oper) {
  coo_matrix* left;
  coo_matrix* right;
  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, left);
  TypedData_Get_Struct(another, coo_matrix, &coo_data_type, right);

  coo_matrix* result = ALLOC(coo_matrix);
  result->dtype = left->dtype;
  result->count = 0;
  result->ndims = left->ndims;
  result->shape = ALLOC_N(size_t, result->ndims);

  for (size_t index = 0; index < result->ndims; index++) {
    result->shape[index] = left->shape[index];
  }

  result->elements = ALLOC_N(double, 2 * left->count);
  result->ia       = ALLOC_N(size_t, 2 * left->count);
  result->ja       = ALLOC_N(size_t, 2 * left->count);

  size_t left_index = 0, right_index = 0, result_index = 0;
  while (left_index < left->count && right_index < right->count) {
    if (left->ia[left_index] == right->ia[right_index] //left and right indices equal
    && left->ja[left_index] == right->ja[right_index]) {
      double result_val = coo_perform_oper(left->elements[left_index], right->elements[right_index], oper);
      if(fabs(result_val) < 1e-6) {  //near to zero
        left_index++, right_index++;
        continue;  //skip current result value
      }
      result->elements[result_index] = result_val;
      result->ia[result_index]       = left->ia[left_index];
      result->ja[result_index]       = left->ja[left_index];

      left_index++, right_index++, result_index++;
    }
    else if ((left->ia[left_index] < right->ia[right_index]) //left index smaller
    || (left->ia[left_index] == right->ia[right_index] && left->ja[left_index] < right->ja[right_index])) {
      double result_val = coo_perform_oper(left->elements[left_index], 0.0, oper);
      if(fabs(result_val) < 1e-6) {  //near to zero
        left_index++;
        continue;  //skip current result value
      }
      result->elements[result_index] = result_val;
      result->ia[result_index]       = left->ia[left_index];
      result->ja[result_index]       = left->ja[left_index];

      left_index++, result_index++;
    }
    else {  //right index smaller
      double result_val = coo_perform_oper(0.0, right->elements[right_index], oper);
      if(fabs(result_val) < 1e-6) {  //near to zero
        right_index++;
        continue;  //skip current result value
      }
      result->elements[result_index] = result_val;
      result->ia[result_index]       = right->ia[right_index];
      result->ja[result_index]       = right->ja[right_index];

      right_index++, result_index++;
    }

    result->count++;
  }

  while (left_index < left->count) {
    double result_val = coo_perform_oper(left->elements[left_index], 0.0, oper);
    if(fabs(result_val) < 1e-6) {  //near to zero
      left_index++;
      continue;  //skip current result value
    }
    result->elements[result_index] = result_val;
    result->ia[result_index]       = left->ia[left_index];
    result->ja[result_index]       = left->ja[left_index];

    left_index++, result_index++;
    result->count++;
  }

  while (right_index < right->count) {
    double result_val = coo_perform_oper(0.0, right->elements[right_index], oper);
    if(fabs(result_val) < 1e-6) {  //near to zero
      right_index++;
      continue;  //skip current result value
    }
    result->elements[result_index] = result_val;
    result->ia[result_index]       = right->ia[right_index];
    result->ja[result_index]       = right->ja[right_index];

    right_index++, result_index++;
    result->count++;
  }

  return TypedData_Wrap_Struct(COO, &coo_data_type, result);
}

VALUE coo_add(VALUE self, VALUE another) {
  return coo_elementwise_binary(self, another, '+');
}

VALUE coo_sub(VALUE self, VALUE another) {
  return coo_elementwise_binary(self, another, '-');
}

VALUE coo_mul(VALUE self, VALUE another) {
  return coo_elementwise_binary(self, another, '*');
}

double coo_unary_oper(double val, const char* oper) {
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
VALUE coo_elementwise_unary(VALUE self, const char* oper) {
  coo_matrix* left;
  TypedData_Get_Struct(self, coo_matrix, &coo_data_type, left);

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

  for(size_t index = 0; index < result->count; index++) {
    result->elements[index] = coo_unary_oper(left->elements[index], oper);
    result->ia[index] = left->ia[index];
    result->ja[index] = left->ja[index];
  }

  return TypedData_Wrap_Struct(COO, &coo_data_type, result);
}

VALUE coo_sin(VALUE self) {
  return coo_elementwise_unary(self, "sin");
}

VALUE coo_cos(VALUE self) {
  return coo_elementwise_unary(self, "cos");
}

VALUE coo_tan(VALUE self) {
  return coo_elementwise_unary(self, "tan");
}
