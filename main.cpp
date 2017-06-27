/*
* g++ -o main  main.cpp  -I/usr/include/python3.4 -L/usr/lib/python3.4/config  -lpython3.4m
*/
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "Python.h"
#include <numpy/arrayobject.h>
#include <iostream>
using namespace std;

int init_numpy(){
    import_array();
}

int main(int argc, char* argv[])
{
    Py_Initialize();
	
	if (!Py_IsInitialized())
	{
		return -1;
	}
	init_numpy();
	
	PyRun_SimpleString("import sys");      
	PyRun_SimpleString("sys.path.append('./')"); 
	
	PyObject *pModule = NULL;
	pModule = PyImport_ImportModule("func");  //µº»Îpyfun.py 
	if (!pModule)
	{
		printf("not found .py file\n");
		return 0;
	}

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;
	PyObject *pArgs=NULL;
	//PyObject *pList=NULL;
  	
	cout<< " call : "<<endl;
	pFunc = PyObject_GetAttrString(pModule, "func"); 
	pArgs=PyTuple_New(3);

	//PyTuple_SetItem(pArgs, 0, Py_BuildValue("[f,f]", 1.0,3.0)); 
	//PyTuple_SetItem(pArgs, 1, Py_BuildValue("[f,f]", 2.0, 4.0));
	//PyTuple_SetItem(pArgs, 2, Py_BuildValue("[f,f,f]", 1.0, 5.0, 7.0)); 
	
	double aArrays[5] = {1.0,2.0,3.0,4.0,5.0};
	
	double bArrays[3][5] = {{1.0, 1.0, 1.0,1.0,1.0}, 
							{2.0, 2.0, 2.0,2.0,2.0},
							{3.0, 3.0, 3.0,3.0,3.0}};
							
	double cArrays[4][3] = {{1.0, 1.0, 1.0}, 
							{2.0, 2.0, 2.0},
							{3.0, 3.0, 3.0},
							{1.0, 1.0, 1.0}};
							
    double dArrays[3][5][2] = {{{-3., -1}, {-3., -1.}, {-3., -1.}, {-3., -1}, {-3., -1}},
							   {{-2., -2}, {-2., -2.}, {-2., -2.}, {-2., -2}, {-2., -2}},
							   {{-1., -3}, {-1., -3.}, {-1., -3.}, {-1., -3}, {-1., -3}}		
							  };						
	
	npy_intp  aDims[1] = {5};
	npy_intp  bDims[2] = {3, 5};
	//npy_intp  cDims[2] = {4, 3};
	npy_intp  dDims[3] = {3, 5, 2};
	
	PyObject *pa  = PyArray_SimpleNewFromData(1, aDims, NPY_DOUBLE, aArrays);
	PyObject *pb  = PyArray_SimpleNewFromData(2, bDims, NPY_DOUBLE, bArrays);
	//PyObject *pc  = PyArray_SimpleNewFromData(2, cDims, NPY_DOUBLE, cArrays);
	PyObject *pc  = PyArray_SimpleNewFromData(3, dDims, NPY_DOUBLE, dArrays);
	
	PyTuple_SetItem(pArgs, 0, pa);
	PyTuple_SetItem(pArgs, 1, pb);
	PyTuple_SetItem(pArgs, 2, pc);
	
	pValue = PyObject_CallObject(pFunc, pArgs);
	cout<< " \n ====================\n result : "<<endl;
	
	//if(PyObject_IsInstance(pValue)){
	//	cout<< " list : "<<endl;
	//}
	
	//cout << PyLong_AsLong(pValue) << endl;
	/*Py_DECREF(pModule);
	Py_DECREF(pFunc);
	Py_DECREF(pArgs);
	Py_DECREF(pValue);
	Py_DECREF(pa);
	Py_DECREF(pb);
	Py_DECREF(pc);*/

	Py_Finalize();

	return 0;
}
