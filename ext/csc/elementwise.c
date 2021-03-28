double csc_perform_oper(double val_a, double val_b, char oper) {
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
VALUE csc_elementwise_binary(VALUE self, VALUE another, char oper) {
  csc_matrix* left;
  csc_matrix* right;
  TypedData_Get_Struct(self, csc_matrix, &csc_data_type, left);
  TypedData_Get_Struct(another, csc_matrix, &csc_data_type, right);

  csc_matrix* result = ALLOC(csc_matrix);
  result->count = 0;
  result->ndims = left->ndims;
  result->shape = ALLOC_N(size_t, result->ndims);

  for (size_t index = 0; index < result->ndims; index++) {
    result->shape[index] = left->shape[index];
  }

  result->elements = ALLOC_N(double, 2 * left->count);
  result->ia       = ALLOC_N(size_t, 2 * left->count);
  result->jp       = ALLOC_N(size_t, left->shape[1] + 1);

  for(size_t index = 0; index <= result->shape[1]; index++) result->jp[index] = 0;

  size_t left_index = 0, right_index = 0, result_index = 0;
  for(size_t index = 1; index <= result->shape[1]; index++) {
    size_t left_col_ele_count = left->jp[index];
    size_t right_col_ele_count = right->jp[index];

    while (left_index < left_col_ele_count && right_index < right_col_ele_count) {
      if (left->ia[left_index] == right->ia[right_index]) { //left and right indices equal
        double result_val = csc_perform_oper(left->elements[left_index], right->elements[right_index], oper);
        if(fabs(result_val) < 1e-6) {  //near to zero
          left_index++, right_index++;
          continue;  //skip current result value
        }
        result->elements[result_index] = result_val;
        // result->ia[result_index]    = left->ia[left_index];
        result->ia[result_index]       = left->ia[left_index];

        left_index++, right_index++, result_index++;
      }
      else if (left->ia[left_index] < right->ia[right_index]) { //left index smaller
        double result_val = csc_perform_oper(left->elements[left_index], 0.0, oper);
        if(fabs(result_val) < 1e-6) {  //near to zero
          left_index++;
          continue;  //skip current result value
        }
        result->elements[result_index] = result_val;
        // result->ia[result_index]    = left->ia[left_index];
        result->ia[result_index]       = left->ia[left_index];

        left_index++, result_index++;
      }
      else {  //right index smaller
        double result_val = csc_perform_oper(0.0, right->elements[right_index], oper);
        if(fabs(result_val) < 1e-6) {  //near to zero
          right_index++;
          continue;  //skip current result value
        }
        result->elements[result_index] = result_val;
        // result->ia[result_index]    = right->ia[right_index];
        result->ia[result_index]       = right->ia[right_index];

        right_index++, result_index++;
      }

      result->count++;
      result->jp[index] += 1;
    }

    while (left_index < left_col_ele_count) {
      double result_val = csc_perform_oper(left->elements[left_index], 0.0, oper);
      if(fabs(result_val) < 1e-6) {  //near to zero
        left_index++;
        continue;  //skip current result value
      }
      result->elements[result_index] = result_val;
      // result->ia[result_index]    = left->ia[left_index];
      result->ia[result_index]       = left->ia[left_index];

      left_index++, result_index++;
      result->count++;
      result->jp[index] += 1;
    }

    while (right_index < right_col_ele_count) {
      double result_val = csc_perform_oper(0.0, right->elements[right_index], oper);
      if(fabs(result_val) < 1e-6) {  //near to zero
        right_index++;
        continue;  //skip current result value
      }
      result->elements[result_index] = result_val;
      // result->ia[result_index]    = right->ia[right_index];
      result->ia[result_index]       = right->ia[right_index];

      right_index++, result_index++;
      result->count++;
      result->jp[index] += 1;
    }

    result->jp[index] += result->jp[index - 1];
  }

  return TypedData_Wrap_Struct(CSC, &csc_data_type, result);
}

VALUE csc_add(VALUE self, VALUE another) {
  return csc_elementwise_binary(self, another, '+');
}

VALUE csc_sub(VALUE self, VALUE another) {
  return csc_elementwise_binary(self, another, '-');
}

VALUE csc_mul(VALUE self, VALUE another) {
  return csc_elementwise_binary(self, another, '*');
}