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
#include "dia/dia_def.c"

#include "interfaces/nmatrix.c"
