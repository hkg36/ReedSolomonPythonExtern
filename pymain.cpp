#include <Python.h>
#include <vector>
#include "src/ecc.h"
//void RSinit();
//std::vector<unsigned char> RSDecode ( const std::vector<unsigned char> &argCode, unsigned int errSize );
//std::vector<unsigned char> RSEncode ( const std::vector<unsigned char>& argMesg,unsigned int errSize );

static PyObject* PyRSEncode(PyObject* Self, PyObject* args)
{
  PyObject * listObj=NULL;
  if (! PyArg_ParseTuple( args, "O!", &PyList_Type, &listObj)) return NULL;
  size_t lsize= PyList_Size(listObj);
  std::vector<unsigned char> srcdata;
  for(size_t i=0;i<lsize;i++)
  {
    PyObject* cell = PyList_GetItem(listObj, i);
    long int one=PyInt_AsLong(cell);
    if (one == -1 && PyErr_Occurred())
      return NULL;
    srcdata.push_back(one);
  }
  std::vector<unsigned char> result(srcdata.size()+NPAR);
  encode_data(srcdata.data(), srcdata.size(), result.data());
  //=RSEncode(srcdata,temp_int);
  PyObject* list_result= PyList_New(result.size());
  for(size_t i=0;i<result.size();i++)
    PyList_SetItem(list_result,i,PyInt_FromLong(result[i]));
  return list_result;
}

static PyObject* PyRSDecode(PyObject* Self, PyObject* args)
{
  PyObject * listObj=NULL;
  if (! PyArg_ParseTuple( args, "O!", &PyList_Type, &listObj)) return NULL;
  size_t lsize= PyList_Size(listObj);
  std::vector<unsigned char> srcdata;
  for(size_t i=0;i<lsize;i++)
  {
    PyObject* cell = PyList_GetItem(listObj, i);
    long int one=PyInt_AsLong(cell);
    if (one == -1 && PyErr_Occurred())
      return NULL;
    srcdata.push_back(one);
  }
  decode_data(srcdata.data(),srcdata.size());
  if (check_syndrome () != 0) {
    if(0==correct_errors_erasures (srcdata.data(),srcdata.size(),0,NULL))
    {
      PyErr_SetString( PyExc_BufferError,"can not correct,too much error byte");
      return NULL;
    }
  }
  PyObject* list_result= PyList_New(srcdata.size()-NPAR);
  for(size_t i=0,count=srcdata.size()-NPAR;i<count;i++)
    PyList_SetItem(list_result,i,PyInt_FromLong(srcdata[i]));
  return list_result;
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
}