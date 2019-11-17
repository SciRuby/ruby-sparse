#include "ruby.h"
#include "stdio.h"

typedef struct SPARSE_STRUCT
{
  size_t ndims;
  size_t count;
  size_t* shape;
}sparse;

typedef struct COO_STRUCT
{
  size_t ndims;
  size_t count;     //count of non-zero elements;    
  size_t* shape;
  double* elements; //elements array
  size_t* ia;       //row index
  size_t* ja;       //col index
}coo_matrix;

VALUE RubySparse = Qnil;
VALUE SparseMatrix = Qnil;
VALUE COO = Qnil;

void Init_ruby_sparse();
VALUE coo_init(int argc, VALUE* argv, VALUE self);
VALUE coo_get_shape(VALUE self);
VALUE coo_get_elements(VALUE self);
VALUE coo_get_coords(VALUE self);
VALUE coo_get_count(VALUE self);
VALUE coo_get_ndims(VALUE self);
VALUE coo_alloc(VALUE klass);
void coo_free(coo_matrix* mat);

VALUE coo_add(VALUE self, VALUE another);

VALUE coo_from_nmatrix(VALUE self, VALUE nmat);


void Init_ruby_sparse() {
  RubySparse = rb_define_module("RubySparse");

  SparseMatrix = rb_define_class_under(RubySparse, "SparseMatrix", rb_cObject);
  COO          = rb_define_class_under(RubySparse, "COO", rb_cObject);

  rb_define_alloc_func(COO, coo_alloc);
  rb_define_method(COO, "initialize", coo_init, -1);
  rb_define_method(COO, "shape", coo_get_shape, 0);
  rb_define_method(COO, "elements", coo_get_elements, 0);
  rb_define_method(COO, "coords", coo_get_coords, 0);
  rb_define_method(COO, "nzcount", coo_get_count, 0);
  rb_define_method(COO, "dim", coo_get_ndims, 0);

  rb_define_method(COO, "+", coo_add, 1);

  //rb_define_singleton_method(COO, "from_nmatrix", coo_from_nmatrix, 1);
}


VALUE coo_init(int argc, VALUE* argv, VALUE self) {
  coo_matrix* mat;
  Data_Get_Struct(self, coo_matrix, mat);

  if(argc > 0){
    mat->ndims = 2;
    mat->count = (size_t)RARRAY_LEN(argv[1]);
    mat->shape = ALLOC_N(size_t, mat->ndims);
    for(size_t index = 0; index < mat->ndims; index++) {
      mat->shape[index] = (size_t)FIX2LONG(RARRAY_AREF(argv[0], index));
    }
    mat->elements = ALLOC_N(double, mat->count);
    for(size_t index = 0; index < mat->count; index++) {
      mat->elements[index] = (double)NUM2DBL(RARRAY_AREF(argv[1], index));
    }
    mat->ia = ALLOC_N(size_t, mat->count);
    for(size_t index = 0; index < mat->count; index++) {
      mat->ia[index] = (size_t)NUM2SIZET(RARRAY_AREF(argv[2], index));
    }
    mat->ja = ALLOC_N(size_t, mat->count);
    for(size_t index = 0; index < mat->count; index++) {
      mat->ja[index] = (size_t)NUM2SIZET(RARRAY_AREF(argv[3], index));
    }
  }

  return self;
}


VALUE coo_alloc(VALUE klass) {
  coo_matrix* mat = ALLOC(coo_matrix);

  return Data_Wrap_Struct(klass, NULL, coo_free, mat);
}


void coo_free(coo_matrix* mat) {
  xfree(mat);
}


VALUE coo_get_elements(VALUE self) {
  coo_matrix* input;

  Data_Get_Struct(self, coo_matrix, input);

  VALUE* array = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++){
    array[index] = DBL2NUM(input->elements[index]);
  }

  return rb_ary_new4(input->count, array);
}


VALUE coo_get_coords(VALUE self) {
  coo_matrix* input;

  Data_Get_Struct(self, coo_matrix, input);

  VALUE* array_ia = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++){
    array_ia[index] = SIZET2NUM(input->ia[index]);
  }

  VALUE* array_ja = ALLOC_N(VALUE, input->count);
  for (size_t index = 0; index < input->count; index++){
    array_ja[index] = SIZET2NUM(input->ja[index]);
  }

  VALUE ia = rb_ary_new4(input->count, array_ia);
  VALUE ja = rb_ary_new4(input->count, array_ja);

  return rb_ary_new3(2, ia, ja);
}

VALUE coo_get_count(VALUE self) {
  coo_matrix* input;

  Data_Get_Struct(self, coo_matrix, input);

  return SIZET2NUM(input->count);
}


VALUE coo_get_ndims(VALUE self) {
  coo_matrix* input;

  Data_Get_Struct(self, coo_matrix, input);

  return SIZET2NUM(input->ndims);
}


VALUE coo_get_shape(VALUE self) {
  coo_matrix* input;

  Data_Get_Struct(self, coo_matrix, input);

  VALUE* array = ALLOC_N(VALUE, input->ndims);
  for (size_t index = 0; index < input->ndims; index++){
    array[index] = SIZET2NUM(input->shape[index]);
  }

  return rb_ary_new4(input->ndims, array);
}


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

#include "interfaces/nmatrix.c"
