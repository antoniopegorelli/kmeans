#include <iostream>
#include <math.h>
#include <ctime>
#include <time.h> 
#include <Eigen/Eigenvalues>	// Library to calculate eigenvalues and eigenvectors
#include <fstream>

using namespace std;
using namespace Eigen;

//------------------------------------------------- Matrix Class -----------------------------------------------------------

// Matrix class declaration
class matrix
{
private:
	int matrixRows;
	int matrixCols;
	double matrixData[2000][10];
public:
	matrix(int, int);
	~matrix();
	void addData(double, int, int);
	double getData(int, int);
	int getRows(void);
	int getCols(void);
	double getMaxInCol(int col);
	int getMaxInRow(int row);
	int getMinInCol(int col);
	int getMinInRow(int row);
	int countClass(int col, int classVal);
	matrix* exportRow(int row);
	matrix* exportCol(int col);
	void print(string name);
	void printToFile(string name);

};

// Matrix class contructor
matrix::matrix(int rows, int cols)
{
	matrixRows = rows;
	matrixCols = cols;
	matrixData[0][0] = 0;
}

// Matrix class destructor
matrix::~matrix()
{

}

// Matrix class to add data
void matrix::addData(double data, int row, int col)
{
	matrixData[row][col] = data;
}

// Matrix class to get data
double matrix::getData(int row, int col)
{
	return matrixData[row][col];
}

// Matrix class to get row size
int matrix::getRows(void)
{
	return matrixRows;
}

// Matrix class to get column size
int matrix::getCols(void)
{
	return matrixCols;
}

// Matrix class to get maximum value
double matrix::getMaxInCol(int col)
{
	double max = numeric_limits<int>::min();

	for (int i = 0; i < matrixRows; i++)
	{
		if (matrixData[i][col] > max) { max = matrixData[i][col]; }
	}

	return max;
}

// Matrix class to get maximum value
int matrix::getMaxInRow(int row)
{
	int max = numeric_limits<int>::min();

	for (int i = 0; i < matrixCols; i++)
	{
		if (matrixData[row][i] > max) { max = matrixData[row][i]; }
	}

	return max;
}

// Matrix class to get minimum value
int matrix::getMinInCol(int col)
{
	int min = numeric_limits<int>::max();

	for (int i = 0; i < matrixRows; i++)
	{
		if (matrixData[i][col] < min) { min = matrixData[i][col]; }
	}

	return min;
}

// Matrix class to get minimum value
int matrix::getMinInRow(int row)
{
	int min = numeric_limits<int>::max();

	for (int i = 0; i < matrixCols; i++)
	{
		if (matrixData[row][i] < min) { min = matrixData[row][i]; }
	}

	return min;
}

// Matrix class to count class quantity
int matrix::countClass(int col, int classVal)
{
	int count = 0;

	for (int i = 0; i < matrixRows; i++)
	{
		if (matrixData[i][col] == classVal) { count++; }
	}

	return count;
}

// Export one row of data to a new matrix
matrix* matrix::exportRow(int row)
{
	matrix* result = new matrix(1, this->getCols());

	for (int j = 0; j < this->getCols(); j++)
	{
		result->addData(matrixData[row][j], 0, j);
	}
	return result;
}

// Export one column of data to a new matrix
matrix* matrix::exportCol(int col)
{
	matrix* result = new matrix(this->getRows(), 1);

	for (int i = 0; i < this->getRows(); i++)
	{
		result->addData(matrixData[i][col], i, 0);
	}
	return result;
}

// Matrix class to print it on screen
void matrix::print(string name)
{
	cout << name << ": " << endl << matrixRows << " x " << matrixCols << " matrix" << endl << endl;

	for (int i = 0; i < this->getRows(); i++)
	{
		for (int j = 0; j < this->getCols(); j++)
		{
			cout << this->getData(i, j) << "    ";
		}
		cout << endl << endl;
	}
	cout << endl;
}

// Matrix class to print it on file
void matrix::printToFile(string name)
{
	name += ".csv";

	ofstream outputFile;
	outputFile.open(name);

	//outputFile << name << ": " << matrixRows << " x " << matrixCols << " matrix\n";

	for (int i = 0; i < this->getCols(); i++)
	{
		outputFile << name << " " << i << ";";
	}
	outputFile << "\n";

	for (int i = 0; i < this->getRows(); i++)
	{
		for (int j = 0; j < this->getCols(); j++)
		{
			outputFile << this->getData(i, j) << ";";
		}
		outputFile << "\n";
	}
	outputFile << "\n";

	outputFile.close();

	cout << "Successfully written to file: " << name << endl << endl;
}

//----------------------------------------------------- Datasets ---------------------------------------------------------------

// Import CSV file to matrix
matrix* dataImportCSV(string filename, char delimiter)
{
	filename += ".csv";
	ifstream inputFile(filename);
	string line;

	int i = 0;
	int j = 0;
	bool firstLine = true;

	if (inputFile.good())
		cout << filename << " opened successfully!" << endl;

	while (getline(inputFile, line))
	{
		if (line.find(delimiter) == string::npos)
			break;

		istringstream lineString(line);
		string singleData;

		bool stringErr = false;

		while (getline(lineString, singleData, delimiter))
		{
			if (firstLine) j++;

			singleData.erase(remove_if(singleData.begin(), singleData.end(), [](char c) { return !(c >= 32 && c < 167); }), singleData.end());

			try
			{
				stod(singleData);
			}
			catch (exception & e)
			{
				stringErr = true;
			}
		}

		if (stringErr)
		{
			i--;
		}

		firstLine = false;
		i++;
	}

	cout << "i = " << i << endl << "j = " << j << endl;

	matrix* result = new matrix(i, j);

	inputFile.clear();
	inputFile.seekg(0, ios::beg);

	i = 0;
	j = 0;

	while (getline(inputFile, line))
	{
		if (line.find(delimiter) == string::npos)
			break;

		istringstream lineString(line);
		string singleData;

		//cout << endl << line << endl;

		bool stringErr = false;

		while (getline(lineString, singleData, delimiter))
		{

			//cout << "|" << singleData << "|";
			singleData.erase(remove_if(singleData.begin(), singleData.end(), [](char c) { return !(c >= 32 && c < 167); }), singleData.end());
			//cout << "|" << singleData << "|" << endl;

			try
			{
				result->addData(stod(singleData), i, j);
			}
			catch (exception & e)
			{
				//cout << "Could not convert string to double" << endl;
				stringErr = true;
			}
			j++;
		}
		if (!stringErr)
		{
			i++;
		}
		j = 0;
	}

	if (inputFile.bad())
		cout << "error while reading file" << endl;

	inputFile.close();

	return result;
}

// Data from class presentation
double aula[17][2] = {
	{ 1.9, 7.3 },
	{ 3.4, 7.5 },
	{ 2.5, 6.8 },
	{ 1.5, 6.5 },
	{ 3.5, 6.4 },
	{ 2.2, 5.8 },
	{ 3.4, 5.2 },
	{ 3.6, 4 },
	{ 5, 3.2 },
	{ 4.5, 2.4 },
	{ 6, 2.6 },
	{ 1.9, 3 },
	{ 1, 2.7 },
	{ 1.9,2.4 },
	{ 0.8,2 },
	{ 1.6,1.8 },
	{1, 1 }
};

// Prepare class presentation database on matrix class
matrix* classPresentationData(void)
{
	matrix* result = new matrix(17, 2);

	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			result->addData(aula[i][j], i, j);
		}
	}

	return result;
}

// Iris flower database
double iris[150][5] = { {5.1, 3.5, 1.4, 0.2, 1},
{4.9,3.0,1.4,0.2,1},
{4.7,3.2,1.3,0.2,1},
{4.6,3.1,1.5,0.2,1},
{5.0,3.6,1.4,0.3,1},
{5.4,3.9,1.7,0.4,1},
{4.6,3.4,1.4,0.3,1},
{5.0,3.4,1.5,0.2,1},
{4.4,2.9,1.4,0.2,1},
{4.9,3.1,1.5,0.1,1},
{5.4,3.7,1.5,0.2,1},
{4.8,3.4,1.6,0.2,1},
{4.8,3.0,1.4,0.1,1},
{4.3,3.0,1.1,0.1,1},
{5.8,4.0,1.2,0.2,1},
{5.7,4.4,1.5,0.4,1},
{5.4,3.9,1.3,0.4,1},
{5.1,3.5,1.4,0.3,1},
{5.7,3.8,1.7,0.3,1},
{5.1,3.8,1.5,0.3,1},
{5.4,3.4,1.7,0.2,1},
{5.1,3.7,1.5,0.4,1},
{4.6,3.6,1.0,0.2,1},
{5.1,3.3,1.7,0.5,1},
{4.8,3.4,1.9,0.2,1},
{5.0,3.0,1.6,0.2,1},
{5.0,3.4,1.6,0.4,1},
{5.2,3.5,1.5,0.2,1},
{5.2,3.4,1.4,0.2,1},
{4.7,3.2,1.6,0.2,1},
{4.8,3.1,1.6,0.2,1},
{5.4,3.4,1.5,0.4,1},
{5.2,4.1,1.5,0.1,1},
{5.5,4.2,1.4,0.2,1},
{4.9,3.1,1.5,0.2,1},
{5.0,3.2,1.2,0.2,1},
{5.5,3.5,1.3,0.2,1},
{4.9,3.6,1.4,0.1,1},
{4.4,3.0,1.3,0.2,1},
{5.1,3.4,1.5,0.2,1},
{5.0,3.5,1.3,0.3,1},
{4.5,2.3,1.3,0.3,1},
{4.4,3.2,1.3,0.2,1},
{5.0,3.5,1.6,0.6,1},
{5.1,3.8,1.9,0.4,1},
{4.8,3.0,1.4,0.3,1},
{5.1,3.8,1.6,0.2,1},
{4.6,3.2,1.4,0.2,1},
{5.3,3.7,1.5,0.2,1},
{5.0,3.3,1.4,0.2,1},
{7.0,3.2,4.7,1.4,2},
{6.4,3.2,4.5,1.5,2},
{6.9,3.1,4.9,1.5,2},
{5.5,2.3,4.0,1.3,2},
{6.5,2.8,4.6,1.5,2},
{5.7,2.8,4.5,1.3,2},
{6.3,3.3,4.7,1.6,2},
{4.9,2.4,3.3,1.0,2},
{6.6,2.9,4.6,1.3,2},
{5.2,2.7,3.9,1.4,2},
{5.0,2.0,3.5,1.0,2},
{5.9,3.0,4.2,1.5,2},
{6.0,2.2,4.0,1.0,2},
{6.1,2.9,4.7,1.4,2},
{5.6,2.9,3.6,1.3,2},
{6.7,3.1,4.4,1.4,2},
{5.6,3.0,4.5,1.5,2},
{5.8,2.7,4.1,1.0,2},
{6.2,2.2,4.5,1.5,2},
{5.6,2.5,3.9,1.1,2},
{5.9,3.2,4.8,1.8,2},
{6.1,2.8,4.0,1.3,2},
{6.3,2.5,4.9,1.5,2},
{6.1,2.8,4.7,1.2,2},
{6.4,2.9,4.3,1.3,2},
{6.6,3.0,4.4,1.4,2},
{6.8,2.8,4.8,1.4,2},
{6.7,3.0,5.0,1.7,2},
{6.0,2.9,4.5,1.5,2},
{5.7,2.6,3.5,1.0,2},
{5.5,2.4,3.8,1.1,2},
{5.5,2.4,3.7,1.0,2},
{5.8,2.7,3.9,1.2,2},
{6.0,2.7,5.1,1.6,2},
{5.4,3.0,4.5,1.5,2},
{6.0,3.4,4.5,1.6,2},
{6.7,3.1,4.7,1.5,2},
{6.3,2.3,4.4,1.3,2},
{5.6,3.0,4.1,1.3,2},
{5.5,2.5,4.0,1.3,2},
{5.5,2.6,4.4,1.2,2},
{6.1,3.0,4.6,1.4,2},
{5.8,2.6,4.0,1.2,2},
{5.0,2.3,3.3,1.0,2},
{5.6,2.7,4.2,1.3,2},
{5.7,3.0,4.2,1.2,2},
{5.7,2.9,4.2,1.3,2},
{6.2,2.9,4.3,1.3,2},
{5.1,2.5,3.0,1.1,2},
{5.7,2.8,4.1,1.3,2},
{6.3,3.3,6.0,2.5,3},
{5.8,2.7,5.1,1.9,3},
{7.1,3.0,5.9,2.1,3},
{6.3,2.9,5.6,1.8,3},
{6.5,3.0,5.8,2.2,3},
{7.6,3.0,6.6,2.1,3},
{4.9,2.5,4.5,1.7,3},
{7.3,2.9,6.3,1.8,3},
{6.7,2.5,5.8,1.8,3},
{7.2,3.6,6.1,2.5,3},
{6.5,3.2,5.1,2.0,3},
{6.4,2.7,5.3,1.9,3},
{6.8,3.0,5.5,2.1,3},
{5.7,2.5,5.0,2.0,3},
{5.8,2.8,5.1,2.4,3},
{6.4,3.2,5.3,2.3,3},
{6.5,3.0,5.5,1.8,3},
{7.7,3.8,6.7,2.2,3},
{7.7,2.6,6.9,2.3,3},
{6.0,2.2,5.0,1.5,3},
{6.9,3.2,5.7,2.3,3},
{5.6,2.8,4.9,2.0,3},
{7.7,2.8,6.7,2.0,3},
{6.3,2.7,4.9,1.8,3},
{6.7,3.3,5.7,2.1,3},
{7.2,3.2,6.0,1.8,3},
{6.2,2.8,4.8,1.8,3},
{6.1,3.0,4.9,1.8,3},
{6.4,2.8,5.6,2.1,3},
{7.2,3.0,5.8,1.6,3},
{7.4,2.8,6.1,1.9,3},
{7.9,3.8,6.4,2.0,3},
{6.4,2.8,5.6,2.2,3},
{6.3,2.8,5.1,1.5,3},
{6.1,2.6,5.6,1.4,3},
{7.7,3.0,6.1,2.3,3},
{6.3,3.4,5.6,2.4,3},
{6.4,3.1,5.5,1.8,3},
{6.0,3.0,4.8,1.8,3},
{6.9,3.1,5.4,2.1,3},
{6.7,3.1,5.6,2.4,3},
{6.9,3.1,5.1,2.3,3},
{5.8,2.7,5.1,1.9,3},
{6.8,3.2,5.9,2.3,3},
{6.7,3.3,5.7,2.5,3},
{6.7,3.0,5.2,2.3,3},
{6.3,2.5,5.0,1.9,3},
{6.5,3.0,5.2,2.0,3},
{6.2,3.4,5.4,2.3,3},
{ 5.9,3.0,5.1,1.8,3} };

// Prepare iris flower database on matrix class
matrix* irisData(void)
{
	//matrix* result = new matrix(150, 4);
	//for (int i = 0; i < 150; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		result->addData(iris[i][j], i, j);
	//	}
	//}

	matrix* result = new matrix(150, 2);
	for (int i = 0; i < 150; i++)
	{
		
		result->addData(iris[i][0], i, 0);
		result->addData(iris[i][3], i, 1);
	
	}

	return result;
}

// Prepare survey data on matrix class
matrix* happinessSurveyData(void)
{
	matrix* data = dataImportCSV("SomervilleHappinessSurvey2015", ';');
	matrix* result = new matrix(data->getRows(), 2);

	for (int i = 0; i < data->getRows(); i++)
	{
		result->addData(data->getData(i, 2), i, 0);
		result->addData(data->getData(i, 5), i, 1);
	}
	return result;
}

// Prepare car data on matrix class
matrix* carData(void)
{
	matrix* data = dataImportCSV("car", ';');
	matrix* result = new matrix(data->getRows(), 2);

	for (int i = 0; i < data->getRows(); i++)
	{
		result->addData(data->getData(i, 1), i, 0);
		result->addData(data->getData(i, 5), i, 1);
	}
	return result;
}

// Prepare auto-mpg data on matrix class
matrix* autoMPGData(void)
{
	matrix* data = dataImportCSV("auto-mpg", ';');
	matrix* result = new matrix(data->getRows(), 3);

	for (int i = 0; i < data->getRows(); i++)
	{
		result->addData(data->getData(i, 0), i, 0);
		result->addData(data->getData(i, 2), i, 1);
		result->addData(data->getData(i, 4), i, 2);
	}
	return result;
}

//------------------------------------------------- Support Functions -----------------------------------------------------------

// Distance calculation from centers
double distanceCalc(matrix* testData, matrix* centers, int row, int classNum)
{
	double result = 0;

	for (int j = 0; j < testData->getCols(); j++)
	{
		result += pow(testData->getData(row, j) - centers->getData(classNum, j), 2);
	}
	
	return sqrt(result);
}

// Random integer generator
int randomIntCalc(int init, int max)
{
	return init + static_cast<int>(rand()) / (static_cast<int>(RAND_MAX / (max - init)));
}

// Random double generator
double randomDoubleCalc(double init, double max)
{
	return init + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max - init)));
}

//------------------------------------------------- Kmeans Calculation -----------------------------------------------------------

// Main Kmeans function
matrix* kMeans(matrix* testData, int classCount, bool random)
{
	matrix* centers = new matrix(classCount, testData->getCols() + 1);
	matrix* oldClasses = new matrix(testData->getRows(), 1);
	matrix* currentClasses = new matrix(testData->getRows(), 1);

	bool finished = true;

	testData->print("TestData");

	if (random)
	{
		for (int i = 0; i < classCount; i++)
		{
			for (int j = 0; j < testData->getCols(); j++)
			{
				centers->addData(randomDoubleCalc(testData->getMinInCol(j), testData->getMaxInCol(j)), i, j);
			}
			centers->addData(0, i, testData->getCols() + 1);
		}
	}
	else
	{
		for (int i = 0; i < classCount; i++)
		{
			int row = randomIntCalc(0, testData->getRows() - 1);
			for (int j = 0; j < testData->getCols(); j++)
			{
				centers->addData(testData->getData(row, j), i, j);
			}
			centers->addData(0, i, testData->getCols() + 1);
		}
	}

	centers->print("Initial");

	int iteration = 0;

	// Claculating distances to centers
	do
	{
		finished = true;

		for (int i = 0; i < testData->getRows(); i++)
		{
			matrix* compare = new matrix(classCount, 1);

			for (int c = 0; c < classCount; c++)
			{
				compare->addData(distanceCalc(testData, centers, i, c), c, 0);
			}

			double lowest = numeric_limits<double>::max();

			for (int c = 0; c < classCount; c++)
			{
				if (compare->getData(c, 0) < lowest)
				{
					lowest = compare->getData(c, 0);
					currentClasses->addData(c, i, 0);
				}
			}
		}

		for (int i = 0; i < classCount; i++)
		{
			for (int j = 0; j < centers->getCols(); j++)
			{
				centers->addData(0, i, j);
			}
		}

		for (int i = 0; i < testData->getRows(); i++)
		{
			if (oldClasses->getData(i, 0) != currentClasses->getData(i, 0))
			{
				finished = false;
			}
			
			oldClasses->addData(currentClasses->getData(i, 0), i, 0);

			for (int j = 0; j < testData->getCols(); j++)
			{
				centers->addData(centers->getData(currentClasses->getData(i, 0), j) + testData->getData(i, j), currentClasses->getData(i, 0), j);
			}
			centers->addData(centers->getData(currentClasses->getData(i, 0), centers->getCols() - 1) + 1, currentClasses->getData(i, 0), centers->getCols() - 1);
		}

		for (int i = 0; i < classCount; i++)
		{
			for (int j = 0; j < centers->getCols() - 1; j++)
			{
				centers->addData(centers->getData(i, j) / centers->getData(i, centers->getCols() - 1), i, j);
			}
		}

		centers->print("Center iteration " + to_string(iteration));

		iteration++;

	} while (!finished);

	cout << "Total iterations: " << to_string(iteration) << endl << endl;

	centers->print("Centers");
	
	return centers;
}

int main()
{
	srand(time(NULL));
	
	int classCount = 3;

	matrix* classCenters = kMeans(classPresentationData(), classCount, false);
	classCenters->printToFile("Class Centers");

	matrix* irisCenters = kMeans(irisData(), classCount, true);
	irisCenters->printToFile("Iris Centers");

	matrix* happinessSurvey = kMeans(happinessSurveyData(), classCount, true);
	happinessSurvey->printToFile("Happiness Survey Centers");

	matrix* autoMPGCenters = kMeans(autoMPGData(), classCount, false);
	autoMPGCenters->printToFile("auto-mpg Centers 3d");

	cin.get();
}