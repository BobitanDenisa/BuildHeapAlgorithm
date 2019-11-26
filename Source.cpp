/* Bobitan Denisa-Ioana

  Task:

  Implement correctly and efficiently 2 methods for building a heap (bottom-up, top-down).
  Compare the methods.

  Observations:

  In the average case, as the assignments graph shows, the bottom-up implementation is more efficient.
  As comparisons are concerned, the same result as above is shown in the diagram.
  For the total operations, also, the bottom-up approach is preffered.

  In the worst case, the conclusions are similar, the top-down approach being the most inefficient for assignment, comparisons and total operations.

  Conclusion:

  The bottom-up implementation would be the most efficient of the 2 approaches, for any operation (assigmnet or comparison).
*/


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Profiler.h"

#pragma warning(disable : 4996)


#define MAX_SIZE 2000

Profiler profiler("Average");

FILE *f = fopen("output.txt", "w");


//use global variables to count assignments and comparisons
int comparisonsBU = 0;
int assignmentsBU = 0;
int totalOpBU = 0;
int comparisonsTD = 0;
int assignmentsTD = 0;
int totalOpTD = 0;

void scanArray(int *a, int *n) //read length and array
{
	printf("Enter array length: ");
	scanf("%d", n);
	printf("Enter array: ");
	for (int i = 0; i < *n; i++)
		scanf("%d", &a[i]);
}

void printArray(int *a, int n) //print in the file the array given as parameter
//used in the test functions
{
	for (int i = 0; i < n; i++)
	{
		fprintf(f, "%d ", a[i]);
	}
	fprintf(f, "\n");
}

int parent(int i)
{
	return i / 2;
}

int left(int i)
{
	return 2 * i + 1;
}

int right(int i)
{
	return 2 * i + 2;
}

void swapElements(int *a, int i, int j) //swaps elements from position i and j in array a
{
	int aux = a[i];
	a[i] = a[j];
	a[j] = aux;
}

void maxHeapify(int *a, int i, int n)
{
	int l = left(i);
	int r = right(i);
	int max;
	comparisonsBU++;
	if ((l < n) && (a[l] > a[i]))
	{
		max = l;
	}
	else
		max = i;
	comparisonsBU++;
	if ((r < n) && (a[r] > a[max]))
	{
		max = r;
	}
	if (max != i)
	{
		swapElements(a, i, max);
		assignmentsBU += 3;
		maxHeapify(a, max, n);
	}
}

void buildMaxHeapBottomUp(int *a, int n) //bottom-up approach
{
	for (int i = n / 2 - 1; i >= 0; i--)
		maxHeapify(a, i, n);
}

void heapIncreaseKey(int *a, int i, int key)
{
	comparisonsTD++;
	if (key < a[i])
	{
		perror("Key smaller than curent key");
		exit(-1);
	}
	a[i] = key;
	assignmentsTD++;
	comparisonsTD++;
	while (i > 0 && a[parent(i)] < a[i])
	{
		comparisonsTD++;
		swapElements(a, i, parent(i));
		assignmentsTD += 3;
		i = parent(i);
	}
}

void maxHeapInsert(int *a, int n, int key)
{
	a[n] = INT_MIN;
	assignmentsTD++;
	heapIncreaseKey(a, n, key);
}

void buildMaxHeapTopDown(int *a, int n) //top-down approach
{
	int hsize = 0; //heap size
	for (int i = 1; i < n; i++)
	{
		hsize++;
		maxHeapInsert(a, hsize, a[i]);
	}
}

void profilerReportAverage() //function for creating the report for the average case
{
	int a[MAX_SIZE], b[MAX_SIZE];
	for (int i = 100; i < MAX_SIZE; i += 100) //array lengths
	{
		//initialize global variables
		assignmentsBU = comparisonsBU = totalOpBU = assignmentsTD = comparisonsTD = totalOpTD = 0;

		for (int j = 0; j < 5; j++) //5 different arrays randomly generated
		{
			FillRandomArray(a, i, 10, 50000, true, 0);

			CopyArray(b, a, i); //copy the array to use it for both bottom-up and top-down
			printf("%d bottom-up\n", i);
			buildMaxHeapBottomUp(b, i);

			CopyArray(b, a, i);
			printf("%d top-down\n", i);
			buildMaxHeapTopDown(b, i);
		}

		assignmentsBU /= 5;
		comparisonsBU /= 5;
		totalOpBU = assignmentsBU + comparisonsBU;

		assignmentsTD /= 5;
		comparisonsTD /= 5;
		totalOpTD = assignmentsTD + comparisonsTD;

		profiler.countOperation("asigBU", i, assignmentsBU);
		profiler.countOperation("compBU", i, comparisonsBU);
		profiler.countOperation("totBU", i, totalOpBU);

		profiler.countOperation("asigTD", i, assignmentsTD);
		profiler.countOperation("compTD", i, comparisonsTD);
		profiler.countOperation("totTD", i, totalOpTD);

		profiler.createGroup("Assignments", "asigBU", "asigTD");
		profiler.createGroup("AssignmentsBU", "asigBU");
		profiler.createGroup("AssignmentsTD", "asigTD");

		profiler.createGroup("Comparisons", "compBU", "compTD");
		profiler.createGroup("ComparisonsBU", "compBU");
		profiler.createGroup("ComparisonsTD", "compTD");

		profiler.createGroup("TotalOperations", "totBU", "totTD");
		profiler.createGroup("TotalOperationsBU", "totBU");
		profiler.createGroup("TotalOperationsTD", "totTD");
	}
}

void profilerReportWorst() //function for creating the report for the worst case
{
	profiler.reset("Worst");

	int a[MAX_SIZE], b[MAX_SIZE];
	FillRandomArray(a, MAX_SIZE, 10, 50000, true, 1); //the arrays are sorted in ascending order
	for (int i = 100; i < MAX_SIZE; i += 100)
	{
		//initialize global variables
		assignmentsBU = comparisonsBU = totalOpBU = assignmentsTD = comparisonsTD = totalOpTD = 0;

		CopyArray(b, a, i);
		buildMaxHeapBottomUp(b, i);

		CopyArray(b, a, i);
		buildMaxHeapTopDown(b, i);

		totalOpBU = assignmentsBU + comparisonsBU;
		totalOpTD = assignmentsTD + comparisonsTD;

		profiler.countOperation("asigBU", i, assignmentsBU);
		profiler.countOperation("compBU", i, comparisonsBU);
		profiler.countOperation("totBU", i, totalOpBU);

		profiler.countOperation("asigTD", i, assignmentsTD);
		profiler.countOperation("compTD", i, comparisonsTD);
		profiler.countOperation("totTD", i, totalOpTD);

		profiler.createGroup("Assignments", "asigBU", "asigTD");
		profiler.createGroup("AssignmentsBU", "asigBU");
		profiler.createGroup("AssignmentsTD", "asigTD");

		profiler.createGroup("Comparisons", "compBU", "compTD");
		profiler.createGroup("ComparisonsBU", "compBU");
		profiler.createGroup("ComparisonsTD", "compTD");

		profiler.createGroup("TotalOperations", "totBU", "totTD");
		profiler.createGroup("TotalOperationsBU", "totBU");
		profiler.createGroup("TotalOperationsTD", "totTD");
	}
}

//test functions to show the methods work on given arrays

void testBottomUp()
{
	int n, a[100];
	scanArray(a, &n);
	buildMaxHeapBottomUp(a, n);
	printArray(a, n);
}

void testTopDown()
{
	int n, a[100];
	scanArray(a, &n);
	buildMaxHeapTopDown(a, n);
	printArray(a, n);
}

int main()
{

	//testBottomUp();
	//testTopDown();
	//fclose(f);

	profilerReportAverage();
	profilerReportWorst();
	profiler.showReport();

	return 0;
}