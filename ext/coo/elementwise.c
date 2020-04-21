VALUE coo_add(VALUE self, VALUE another) {
  coo_matrix* left;
  coo_matrix* right;
  Data_Get_Struct(self, coo_matrix, left);
  Data_Get_Struct(another, coo_matrix, right);

  coo_matrix* result = ALLOC(coo_matrix);
  result->count = 0;
  result->ndims = left->ndims;
  result->shape = ALLOC_N(size_t, result->ndims);

  for(size_t index = 0; index < result->ndims; index++){
    result->shape[index] = left->shape[index];
  }

  result->elements = ALLOC_N(double, 2 * left->count);
  result->ia = ALLOC_N(size_t, 2 * left->count);
  result->ja = ALLOC_N(size_t, 2 * left->count);

  size_t left_index = 0, right_index = 0, result_index = 0;
  while(left_index < left->count || right_index < right->count) {
    if(left->ia[left_index] == right->ia[right_index] //left and right indices equal
    && left->ja[left_index] == right->ja[right_index] 
    && left_index < left->count && right_index < right->count) {
      result->elements[result_index] = left->elements[left_index] + right->elements[right_index];
      result->ia[result_index] = left->ia[left_index];
      result->ja[result_index] = left->ja[left_index];

      left_index++, right_index++, result_index++;
    }
    else if((left->ia[left_index] < right->ia[right_index]) //left index smaller
    || (left->ia[left_index] == right->ia[right_index] && left->ja[left_index] < right->ja[right_index])
    && left_index < left->count) {
      result->elements[result_index] = left->elements[left_index];
      result->ia[result_index] = left->ia[left_index];
      result->ja[result_index] = left->ja[left_index];

      left_index++, result_index++;
    }
    else {  //right index smaller
      result->elements[result_index] = right->elements[right_index];
      result->ia[result_index] = right->ia[right_index];
      result->ja[result_index] = right->ja[right_index];

      right_index++, result_index++;
    }

    result->count++;
  }

  return Data_Wrap_Struct(COO, NULL, coo_free, result);
}