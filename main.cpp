/*
* g++ -o main  main.cpp  -I/usr/include/python3.4 -L/usr/lib/python3.4/config  -lpython3.4m
*/
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "Python.h"
#include <numpy/arrayobject.h>
#include <iostream>
using namespace std;

int init_numpy(){
	//初始化 numpy //执行环境，主要是导入包，python2.7用void返回类型，python3.0以上用int返回类型
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
	pModule = PyImport_ImportModule("pyfun");  //导入hello.py 
	if (!pModule)
	{
		printf("not found .py file\n");
	}

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;
	PyObject *pArgs=NULL;
	PyObject *pList=NULL;
  	

	cout<< " call : "<<endl;
	pFunc = PyObject_GetAttrString(pModule, "func"); 
	pArgs=PyTuple_New(3);

	PyTuple_SetItem(pArgs, 0, Py_BuildValue("[f,f]", 1.0,3.0)); 
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("[f,f]", 2.0, 4.0));
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("[f,f,f]", 1.0, 5.0, 7.0)); 
	
	// 传递 numpy 参数
	double CArrays[3][3] = {{1.3, 2.4, 5.6}, {4.5, 7.8, 8.9}, {1.7, 0.4, 0.8}};
	npy_intp Dims[2] = {3, 3};
	
	pValue = PyObject_CallObject(pFunc, pArgs);
	cout<< " \n ====================\n result : "<<endl;
	
	//if(PyObject_IsInstance(pValue)){
	//	cout<< " list : "<<endl;
	//}
	
	//cout << PyLong_AsLong(pValue) << endl;

	Py_Finalize(); /* 结束Python解释器，释放资源 */

	return 0;
}