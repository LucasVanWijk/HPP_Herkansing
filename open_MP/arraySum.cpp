/* arraySum.cpp uses an array to sum the values in an input file,
 *  whose name is specified on the command-line.
 */
#include <iostream>
#include <fstream>
#include <omp.h> // OpenMP

using std::cout, std::endl;

void readArray(char *fileName, double **a, int *n);
double sumArray(double *a, int numValues);

int main(int argc, char *argv[])
{
    int howMany;
    double sum;
    double *a;
    double start = omp_get_wtime();
    if (argc != 2)
    {
        cout << endl << "*** Usage: arraySum <inputFile>" << endl << endl;
        exit(1);
    }

    readArray(argv[1], &a, &howMany);
    sum = sumArray(a, howMany);
    cout << "The sum of the values in the input file '" << argv[1] << "' is " << sum << endl;

    free(a);
    double end = omp_get_wtime();
    cout << "Elasped time = " << (end - start) << " sec" << endl;
    return 0;
}

/* readArray fills an array with values from a file.
 * Receive: fileName, a char*,
 *          a, the address of a pointer to an array,
 *          n, the address of an int.
 * PRE: fileName contains N, followed by N double values.
 * POST: a points to a dynamically allocated array
 *        containing the N values from fileName
 *        and n == N.
 */

void readArray(char *fileName, double **a, int *n)
{
    int count, howMany;
    double *tempA;
    std::ifstream fin(fileName);
    if (!fin.is_open())
    {
        cout << endl << "*** Unable to open input file ''" << fileName << "'" << endl << endl;
        exit(1);
    }

    fin >> howMany;
    tempA = (double *)calloc(howMany, sizeof(double));
    if (tempA == NULL)
    {
        cout << endl << "*** Unable to allocate " << howMany << "-length array" << endl << endl;
        exit(1);
    }

    for (count = 0; count < howMany; count++)
        fin >> tempA[count];

    fin.close();

    *n = howMany;
    *a = tempA;
}

/* sumArray sums the values in an array of doubles.
 * Receive: a, a pointer to the head of an array;
 *          numValues, the number of values in the array.
 * Return: the sum of the values in the array.
 */

double sumArray(double *a, int numValues)
{
    int i;
    double result = 0.0;

    for (i = 0; i < numValues; i++)
    {
        result += a[i];
    }

    return result;
}
