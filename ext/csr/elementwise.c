#include "string.h"
#include "math.h"


double csr_perform_oper(double val_a, double val_b, char oper) {
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
VALUE csr_elementwise_binary(VALUE self, VALUE another, char oper) {
  csr_matrix* left;
  csr_matrix* right;
  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, left);
  TypedData_Get_Struct(another, csr_matrix, &csr_data_type, right);

  csr_matrix* result = ALLOC(csr_matrix);
  result->count = 0;
  result->ndims = left->ndims;
  result->shape = ALLOC_N(size_t, result->ndims);

  for (size_t index = 0; index < result->ndims; index++) {
    result->shape[index] = left->shape[index];
  }

  result->elements = ALLOC_N(double, 2 * left->count);
  result->ip       = ALLOC_N(size_t, left->shape[0] + 1);
  result->ja       = ALLOC_N(size_t, 2 * left->count);

  for(size_t index = 0; index <= result->shape[0]; index++) result->ip[index] = 0;

  size_t left_index = 0, right_index = 0, result_index = 0;
  for(size_t index = 1; index <= result->shape[0]; index++) {
    size_t left_row_ele_count = left->ip[index];
    size_t right_row_ele_count = right->ip[index];

    while (left_index < left_row_ele_count && right_index < right_row_ele_count) {
      if (left->ja[left_index] == right->ja[right_index]) { //left and right indices equal
        double result_val = csr_perform_oper(left->elements[left_index], right->elements[right_index], oper);
        if(fabs(result_val) < 1e-6) {  //near to zero
          left_index++, right_index++;
          continue;  //skip current result value
        }
        result->elements[result_index] = result_val;
        // result->ia[result_index]    = left->ia[left_index];
        result->ja[result_index]       = left->ja[left_index];

        left_index++, right_index++, result_index++;
      }
      else if (left->ja[left_index] < right->ja[right_index]) { //left index smaller
        double result_val = csr_perform_oper(left->elements[left_index], 0.0, oper);
        if(fabs(result_val) < 1e-6) {  //near to zero
          left_index++;
          continue;  //skip current result value
        }
        result->elements[result_index] = result_val;
        // result->ia[result_index]    = left->ia[left_index];
        result->ja[result_index]       = left->ja[left_index];

        left_index++, result_index++;
      }
      else {  //right index smaller
        double result_val = csr_perform_oper(0.0, right->elements[right_index], oper);
        if(fabs(result_val) < 1e-6) {  //near to zero
          right_index++;
          continue;  //skip current result value
        }
        result->elements[result_index] = result_val;
        // result->ia[result_index]    = right->ia[right_index];
        result->ja[result_index]       = right->ja[right_index];

        right_index++, result_index++;
      }

      result->count++;
      result->ip[index] += 1;
    }

    while (left_index < left_row_ele_count) {
      double result_val = csr_perform_oper(left->elements[left_index], 0.0, oper);
      if(fabs(result_val) < 1e-6) {  //near to zero
        left_index++;
        continue;  //skip current result value
      }
      result->elements[result_index] = result_val;
      // result->ia[result_index]    = left->ia[left_index];
      result->ja[result_index]       = left->ja[left_index];

      left_index++, result_index++;
      result->count++;
      result->ip[index] += 1;
    }

    while (right_index < right_row_ele_count) {
      double result_val = csr_perform_oper(0.0, right->elements[right_index], oper);
      if(fabs(result_val) < 1e-6) {  //near to zero
        right_index++;
        continue;  //skip current result value
      }
      result->elements[result_index] = result_val;
      // result->ia[result_index]    = right->ia[right_index];
      result->ja[result_index]       = right->ja[right_index];

      right_index++, result_index++;
      result->count++;
      result->ip[index] += 1;
    }

    result->ip[index] += result->ip[index - 1];
  }

  return TypedData_Wrap_Struct(CSR, &csr_data_type, result);
}

VALUE csr_add(VALUE self, VALUE another) {
  return csr_elementwise_binary(self, another, '+');
}

VALUE csr_sub(VALUE self, VALUE another) {
  return csr_elementwise_binary(self, another, '-');
}

VALUE csr_mul(VALUE self, VALUE another) {
  return csr_elementwise_binary(self, another, '*');
}

double csr_unary_oper(double val, const char* oper) {
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
VALUE csr_elementwise_unary(VALUE self, const char* oper) {
  csr_matrix* left;
  TypedData_Get_Struct(self, csr_matrix, &csr_data_type, left);

  csr_matrix* result = ALLOC(csr_matrix);
  result->dtype = left->dtype;
  result->count = left->count;
  result->ndims = left->ndims;
  result->shape = ALLOC_N(size_t, result->ndims);

  for (size_t index = 0; index < result->ndims; index++) {
    result->shape[index] = left->shape[index];
  }

  result->elements = ALLOC_N(double, left->count);
  result->ip       = ALLOC_N(size_t, left->shape[0] + 1);
  result->ja       = ALLOC_N(size_t, left->count);

  for(size_t index = 0; index < result->count; index++) {
    result->elements[index] = csr_unary_oper(left->elements[index], oper);
    result->ja[index] = left->ja[index];
  }

  for(size_t index = 0; index <= left->shape[0]; index++) {
    result->ip[index] = left->ip[index];
  }

  return TypedData_Wrap_Struct(CSR, &csr_data_type, result);
}

VALUE csr_sin(VALUE self) {
  return csr_elementwise_unary(self, "sin");
}

VALUE csr_cos(VALUE self) {
  return csr_elementwise_unary(self, "cos");
}

VALUE csr_tan(VALUE self) {
  return csr_elementwise_unary(self, "tan");
}
