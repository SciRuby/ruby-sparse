#include "ruby.h"
#include "stdio.h"

# define SP_NUM_DTYPES 6


// data types
typedef enum sp_dtype{
  sp_bool,
  sp_int,
  sp_float32,
  sp_float64,
  sp_complex32,
  sp_complex64
}sp_dtype;

const char* const DTYPE_NAMES[SP_NUM_DTYPES] = {
  "sp_bool",
  "sp_int",
  "sp_float32",
  "sp_float64",
  "sp_complex32",
  "sp_complex64"
};

typedef struct SPARSE_STRUCT
{
  size_t ndims;
  size_t count;
  size_t* shape;
}sparse;

typedef struct COO_STRUCT
{
  sp_dtype dtype;
  size_t ndims;
  size_t count;     //count of non-zero elements
  size_t* shape;
  double* elements; //elements array
  size_t* ia;       //row index
  size_t* ja;       //col index
}coo_matrix;

typedef struct CSR_STRUCT
{
  sp_dtype dtype;
  size_t ndims;
  size_t count;     //count of non-zero elements
  size_t* shape;
  double* elements; //elements array
  size_t* ip;       //row pointer vals
  size_t* ja;       //col index
}csr_matrix;

typedef struct CSC_STRUCT
{
  sp_dtype dtype;
  size_t ndims;
  size_t count;     //count of non-zero elements
  size_t* shape;
  double* elements; //elements array
  size_t* ia;       //row index
  size_t* jp;       //col pointer vals
}csc_matrix;

typedef struct DIA_STRUCT
{
  sp_dtype dtype;
  size_t ndims;
  size_t count;     //count of diagonal elements
  size_t* shape;
  double* elements; //elements array
}dia_matrix;

// typedef struct GCXS_STRUCT
// {
//   sp_dtype dtype;
//   size_t ndims;
//   size_t count;     //count of non-zero elements;
//   size_t* shape;
//   double* elements; //elements array
//   size_t* ia;       //row index
//   size_t* ja;       //col index
// }coo_matrix;

VALUE RubySparse = Qnil;
VALUE SparseArray = Qnil;
VALUE COO = Qnil;
VALUE CSR = Qnil;
VALUE CSC = Qnil;
VALUE DIA = Qnil;
// VALUE GCXS = Qnil;

void Init_ruby_sparse();

// coo methods declaration
VALUE coo_init(int argc, VALUE* argv, VALUE self);
VALUE coo_get_dtype(VALUE self);
VALUE coo_get_shape(VALUE self);
VALUE coo_get_elements(VALUE self);
VALUE coo_get_coords(VALUE self);
VALUE coo_get_count(VALUE self);
VALUE coo_get_ndims(VALUE self);
VALUE coo_alloc(VALUE klass);
void coo_free(coo_matrix* mat);

VALUE coo_add(VALUE self, VALUE another);
VALUE coo_sub(VALUE self, VALUE another);
VALUE coo_mul(VALUE self, VALUE another);

VALUE coo_from_nmatrix(VALUE self, VALUE nmat);

// csr methods declaration
VALUE csr_init(int argc, VALUE* argv, VALUE self);
VALUE csr_get_dtype(VALUE self);
VALUE csr_get_shape(VALUE self);
VALUE csr_get_elements(VALUE self);
VALUE csr_get_indices(VALUE self);
VALUE csr_get_indptr(VALUE self);
VALUE csr_get_count(VALUE self);
VALUE csr_get_ndims(VALUE self);
VALUE csr_alloc(VALUE klass);
void csr_free(csr_matrix* mat);

VALUE csr_add(VALUE self, VALUE another);
VALUE csr_sub(VALUE self, VALUE another);
VALUE csr_mul(VALUE self, VALUE another);

VALUE csr_from_nmatrix(VALUE self, VALUE nmat);

// csc methods declaration
VALUE csc_init(int argc, VALUE* argv, VALUE self);
VALUE csc_get_dtype(VALUE self);
VALUE csc_get_shape(VALUE self);
VALUE csc_get_elements(VALUE self);
VALUE csc_get_indices(VALUE self);
VALUE csc_get_indptr(VALUE self);
VALUE csc_get_count(VALUE self);
VALUE csc_get_ndims(VALUE self);
VALUE csc_alloc(VALUE klass);
void csc_free(csc_matrix* mat);

VALUE csc_add(VALUE self, VALUE another);
VALUE csc_sub(VALUE self, VALUE another);
VALUE csc_mul(VALUE self, VALUE another);

VALUE csc_from_nmatrix(VALUE self, VALUE nmat);

// dia methods declaration
VALUE dia_init(int argc, VALUE* argv, VALUE self);
VALUE dia_get_dtype(VALUE self);
VALUE dia_get_shape(VALUE self);
VALUE dia_get_elements(VALUE self);
VALUE dia_get_count(VALUE self);
VALUE dia_get_ndims(VALUE self);
VALUE dia_alloc(VALUE klass);
void dia_free(dia_matrix* mat);

VALUE dia_add(VALUE self, VALUE another);
VALUE dia_sub(VALUE self, VALUE another);
VALUE dia_mul(VALUE self, VALUE another);

VALUE dia_from_nmatrix(VALUE self, VALUE nmat);


void Init_ruby_sparse() {
  RubySparse = rb_define_module("RubySparse");

  SparseArray = rb_define_class_under(RubySparse, "SparseArray", rb_cObject);
  COO         = rb_define_class_under(RubySparse, "COO", SparseArray);
  CSR         = rb_define_class_under(RubySparse, "CSR", SparseArray);
  CSC         = rb_define_class_under(RubySparse, "CSC", SparseArray);
  DIA         = rb_define_class_under(RubySparse, "DIA", SparseArray);
  // GCXS        = rb_define_class_under(RubySparse, "GCXS", SparseArray);

  rb_define_alloc_func(COO, coo_alloc);
  rb_define_method(COO, "initialize", coo_init, -1);
  rb_define_method(COO, "dtype", coo_get_dtype, 0);
  rb_define_method(COO, "shape", coo_get_shape, 0);
  rb_define_method(COO, "elements", coo_get_elements, 0);
  rb_define_method(COO, "coords", coo_get_coords, 0);
  rb_define_method(COO, "nzcount", coo_get_count, 0);
  rb_define_method(COO, "dim", coo_get_ndims, 0);

  rb_define_method(COO, "+", coo_add, 1);
  rb_define_method(COO, "-", coo_sub, 1);
  rb_define_method(COO, "*", coo_mul, 1);

  //rb_define_singleton_method(COO, "from_nmatrix", coo_from_nmatrix, 1);


  rb_define_alloc_func(CSR, csr_alloc);
  rb_define_method(CSR, "initialize", csr_init, -1);
  rb_define_method(CSR, "dtype", csr_get_dtype, 0);
  rb_define_method(CSR, "shape", csr_get_shape, 0);
  rb_define_method(CSR, "elements", csr_get_elements, 0);
  rb_define_method(CSR, "indices", csr_get_indices, 0);
  rb_define_method(CSR, "indptr", csr_get_indptr, 0);
  rb_define_method(CSR, "nzcount", csr_get_count, 0);
  rb_define_method(CSR, "dim", csr_get_ndims, 0);

  rb_define_method(CSR, "+", csr_add, 1);
  rb_define_method(CSR, "-", csr_sub, 1);
  rb_define_method(CSR, "*", csr_mul, 1);

  //rb_define_singleton_method(CSR, "from_nmatrix", csr_from_nmatrix, 1);


  rb_define_alloc_func(CSC, csc_alloc);
  rb_define_method(CSC, "initialize", csc_init, -1);
  rb_define_method(CSC, "dtype", csc_get_dtype, 0);
  rb_define_method(CSC, "shape", csc_get_shape, 0);
  rb_define_method(CSC, "elements", csc_get_elements, 0);
  rb_define_method(CSC, "indices", csc_get_indices, 0);
  rb_define_method(CSC, "indptr", csc_get_indptr, 0);
  rb_define_method(CSC, "nzcount", csc_get_count, 0);
  rb_define_method(CSC, "dim", csc_get_ndims, 0);

  rb_define_method(CSC, "+", csc_add, 1);
  rb_define_method(CSC, "-", csc_sub, 1);
  rb_define_method(CSC, "*", csc_mul, 1);

  //rb_define_singleton_method(CSC, "from_nmatrix", csc_from_nmatrix, 1);


  rb_define_alloc_func(DIA, dia_alloc);
  rb_define_method(DIA, "initialize", dia_init, -1);
  rb_define_method(DIA, "dtype", dia_get_dtype, 0);
  rb_define_method(DIA, "shape", dia_get_shape, 0);
  rb_define_method(DIA, "elements", dia_get_elements, 0);
  rb_define_method(DIA, "dim", dia_get_ndims, 0);

  rb_define_method(DIA, "+", dia_add, 1);
  rb_define_method(DIA, "-", dia_sub, 1);
  rb_define_method(DIA, "*", dia_mul, 1);

  //rb_define_singleton_method(DIA, "from_nmatrix", dia_from_nmatrix, 1);
}

#include "coo/coo_def.c"
#include "csr/csr_def.c"
#include "csc/csc_def.c"
#include "dia/dia_def.c"

#include "interfaces/nmatrix.c"
