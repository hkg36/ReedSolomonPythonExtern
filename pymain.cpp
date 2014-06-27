#include <Python.h>
#include <vector>
#include "src/ecc.h"
//void RSinit();
//std::vector<unsigned char> RSDecode ( const std::vector<unsigned char> &argCode, unsigned int errSize );
//std::vector<unsigned char> RSEncode ( const std::vector<unsigned char>& argMesg,unsigned int errSize );

static PyObject* PyRSEncode(PyObject* Self, PyObject* args)
{
  unsigned char * listObj=NULL;
  int size;
  if (! PyArg_ParseTuple( args, "s#", &listObj,&size)) return NULL;
  std::vector<unsigned char> result(size+NPAR);
  encode_data(listObj,size, result.data());
  return PyString_FromStringAndSize((const char*)result.data(),result.size());
}

static PyObject* PyRSDecode(PyObject* Self, PyObject* args)
{
  unsigned char * listObj=NULL;
  int size;
  if (! PyArg_ParseTuple( args, "s#", &listObj,&size)) return NULL;
  decode_data(listObj,size);
  if (check_syndrome () != 0) {
    if(0==correct_errors_erasures (listObj,size,0,NULL))
    {
      PyErr_SetString( PyExc_BufferError,"can not correct,too much error byte");
      return NULL;
    }
  }
  return PyString_FromStringAndSize((const char*)listObj,size-NPAR);
}

static PyMethodDef root_methods[] = {
  {"RSEncode",PyRSEncode,METH_VARARGS,"RSEncode"},
  {"RSDecode",PyRSDecode,METH_VARARGS,"RSEncode"},
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC initreedsolomon() {
  initialize_ecc ();
  //RSinit();
    PyObject *m;
    m = Py_InitModule3("reedsolomon", root_methods, "reedsolomon module.");
    PyObject *npar=PyInt_FromLong(NPAR);
    PyObject_SetAttrString(m,"NPAR",npar);
}