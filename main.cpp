/*
 * g++ -o main main.cpp -I/usr/include/python3.4 -L/usr/lib/python3.4/config -lpython3.4m
 */
#include "Python.h"
#include <numpy/arrayobject.h>
#include <iostream>
#include <iomanip>
using namespace std;

void* init_numpy()
{
	import_array();
}


double*** create3DArray( int nx, int ny, int nz )
{
	double*** p = new double**[nx];
	for ( int i = 0; i < nx; i++ )
	{
		p[i] = new double*[ny];
		for ( int j = 0; j < ny; j++ )
		{
			p[i][j] = new double[nz];
			for ( int k = 0; k < nz; k++ )
			{
				p[i][j][k] = (100.0 * i + 10.0 * j + k);
				cout << p[i][j][k] << endl;
			}
		}
	}
	return(p);
}

void init3DPyArray(PyObject *object, double *** p3DArray)
{
	cout << " ======== C++ init3DPyArray : " << endl;
	if ( object == NULL )
	{
		cout << "init3DPyArray error: object is NULL ." << endl;
		return;
	}

	int index_i = 0, index_m = 0, index_n = 0;

	if ( PyArray_Check( object ) )
	{
		PyArrayObject *pAarray = (PyArrayObject *) object;
		/* ��ǰά�� */
		int nd = pAarray->nd;

		/* print shape */
		for ( int i = 0; i < nd; i++ )
		{
			int count = pAarray->dimensions[i];
			if ( i == 0 )
			{
				cout << "(";
				cout << count;
				if ( nd == 1 )
				{
					cout << " )" << endl;
				}else {
					cout << ", ";
				}
			}else if ( i == nd - 1 )
			{
				cout << count;
				cout << ")" << endl;
			}else {
				cout << count << ", ";
			}
		}

		/* ��3άnumpy����Ϊ����� */
		int	x	= pAarray->dimensions[0];
		int	y	= pAarray->dimensions[1];
		int	z	= pAarray->dimensions[2];

		/**
		 *	int nd��Numpy Array�����ά�ȡ�
		 *	int *dimensions ��Numpy Array ����ÿһά�����ݵĸ�����
		 *	int *strides��Numpy Array ����ÿһά�ȵĲ�����
		 *	char *data�� Numpy Array ��ָ�����ݵ�ͷָ�롣
		 */
		cout << endl << endl << "[";
		for ( int j = 0; j < x; j++ )
		{
			for ( int k = 0; k < y; k++ )
			{
				for ( int h = 0; h < z; h++ )
				{
					*(double *) (pAarray->data + j * pAarray->strides[0] +
										  k * pAarray->strides[1] + h * pAarray->strides[2]) = p3DArray[j][k][h];
				}			
			}			
		}		
	}else{
		cout << "Not a PyArrayObject" << endl;
	}
}


void free3DArray( double*** p, int nx, int ny )
{
	for ( int i = 0; i < nx; i++ )
	{
		for ( int j = 0; j < ny; j++ )
			delete[] p[i][j];
		delete[] p[i];
	}
	delete[] p;
}


/**
 * ���3άPyObject ����
 */
void printPyArray( PyObject *object )
{
	cout << " ======== C++ printPyArray : " << endl;
	if ( object == NULL )
	{
		cout << "printPyArray error: object is NULL ." << endl;
		return;
	}

	int index_i = 0, index_m = 0, index_n = 0;

	if ( PyArray_Check( object ) )
	{
		PyArrayObject *pAarray = (PyArrayObject *) object;
		/* ��ǰά�� */
		int nd = pAarray->nd;

		/* print shape */
		for ( int i = 0; i < nd; i++ )
		{
			int count = pAarray->dimensions[i];
			if ( i == 0 )
			{
				cout << "(";
				cout << count;
				if ( nd == 1 )
				{
					cout << " )" << endl;
				}else {
					cout << ", ";
				}
			}else if ( i == nd - 1 )
			{
				cout << count;
				cout << ")" << endl;
			}else {
				cout << count << ", ";
			}
		}

		/* ��3άnumpy����Ϊ����� */
		int	x	= pAarray->dimensions[0];
		int	y	= pAarray->dimensions[1];
		int	z	= pAarray->dimensions[2];


		/**
		 *	int nd��Numpy Array�����ά�ȡ�
		 *	int *dimensions ��Numpy Array ����ÿһά�����ݵĸ�����
		 *	int *strides��Numpy Array ����ÿһά�ȵĲ�����
		 *	char *data�� Numpy Array ��ָ�����ݵ�ͷָ�롣
		 */
		cout << endl << endl << "[";
		for ( int j = 0; j < x; j++ )
		{
			cout << "[";
			for ( int k = 0; k < y; k++ )
			{
				cout << "[";
				for ( int h = 0; h < z; h++ )
				{
					cout << setprecision( 8 ) << *(double *) (pAarray->data + j * pAarray->strides[0] +
										  k * pAarray->strides[1] + h * pAarray->strides[2]) << " ";
					if ( h != z - 1 )
					{
						cout << ", ";
					}
				}

				cout << "]";
				if ( k != y - 1 )
				{
					cout << endl;
				}
			}
			cout << "]";
			if ( j != x - 1 )
			{
				cout << endl << endl;
			}
		}
		cout << "] " << endl << endl;
	}else{
		cout << "Not a PyArrayObject" << endl;
	}
}


int main( int argc, char* argv[] )
{
	Py_Initialize();

	if ( !Py_IsInitialized() )
	{
		return(-1);
	}
	init_numpy();

	PyRun_SimpleString( "import sys" );
	PyRun_SimpleString( "sys.path.append('./')" );

	PyObject *pModule = NULL;
	pModule = PyImport_ImportModule( "func" ); /* ����pyfun.py */
	if ( !pModule )
	{
		printf( "not found .py file\n" );
		return(0);
	}

	PyObject	*pFunc	= NULL;
	PyObject	*pValue = NULL;
	PyObject	*pArgs	= NULL;
	/* PyObject *pList=NULL; */

	cout << " call : " << endl;
	pFunc = PyObject_GetAttrString( pModule, "func" );
	/* Python ���������ĸ���3 */
	pArgs = PyTuple_New( 3 );

	double aArrays[5] = { 1.0, 2.0, 3.0, 4.0, 5.0 };

	double bArrays[3][5] = { { 1.0, 1.0, 1.0, 1.0, 1.0 },
				 { 2.0, 2.0, 2.0, 2.0, 2.0 },
				 { 3.0, 3.0, 3.0, 3.0, 3.0 } };

	double cArrays[3][5][2] = {
		{ { -3., -1 }, { -3., -1. }, { -3., -1. }, { -3., -1 }, { -3., -1 } },
		{ { -2., -2 }, { -2., -2. }, { -2., -2. }, { -2., -2 }, { -2., -2 } },
		{ { -1., -3 }, { -1., -3. }, { -1., -3. }, { -1., -3 }, { -1., -3 } }
	};
	
	/* ������һ��һά����Ϊ5�Ķ��� */
	npy_intp aDims[1] = { 5 };
	/* ������һ��2ά��������һά����3����ά����5 */
	npy_intp bDims[2] = { 3, 5 };
	/* ������һ��3ά��������һά����3����ά����5����ά����Ϊ2 */
	npy_intp cDims[3] = { 3, 5, 2 };
	
	double*** dArrays = create3DArray(3, 5, 2);  
	// ����һ��pyarray 3ά����
	PyObject* pd = PyArray_SimpleNew(3, cDims, NPY_DOUBLE);
	// ��C++ 3ά���飬��ʼ�� PyArray����
	init3DPyArray(pd, dArrays);

	/* ��C++ �������npy���󣬵�һ����������ά�� */
	PyObject	*pa	= PyArray_SimpleNewFromData( 1, aDims, NPY_DOUBLE, aArrays );
	PyObject	*pb	= PyArray_SimpleNewFromData( 2, bDims, NPY_DOUBLE, bArrays );
	
	PyObject	*pc	= PyArray_SimpleNewFromData( 3, cDims, NPY_DOUBLE, cArrays );

	PyTuple_SetItem( pArgs, 0, pa );
	PyTuple_SetItem( pArgs, 1, pb );
	PyTuple_SetItem( pArgs, 2, pd );

	/* ��ʼ����python ���� */
	pValue = PyObject_CallObject( pFunc, pArgs );

	printPyArray( pValue );

	Py_Finalize();

	return(0);
}


