#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// STRUCTURE TOOLSET
// -- better put all to a class --
//data structure of string
struct strData
{
	int dataVol;
	std::string stingData;	
};

//data structure to serialize
struct dataUnit
{	
	strData stringData;
	int intData;
	float floatData;
};

//data input methods
strData setString()
{
	std::cout << "Input string:\n";
	std::string str = "";
	std::getline(std::cin, str);
	int l = str.length();
	return {l,str};
}

int setInt()
{
	std::cout << "Input int:\n";
	std::string str = "";
	std::getline(std::cin, str);
	int a = atoi(str.c_str());
	return a;
}

float setFloat()
{
	std::cout << "Input float:\n";
	std::string str = "";
	std::getline(std::cin, str);
	float a = std::atof(str.c_str());
	return a;
}

//get structure
dataUnit setDataUnit()
{
	std::cout << "Input struct fields:\n";
	strData sd = setString();
	int id = setInt();
	float fd = setFloat();
	return {sd,id,fd};
}

//print structure
void printDataUnit(dataUnit dt)
{
	printf("String [%d]: \"%s\";\nInt: \"%d\";\nFloat: \"%f\".\n", 
		dt.stringData.dataVol,
		dt.stringData.stingData.c_str(),
		dt.intData,
		dt.floatData);
}

//save structure to a file
void saveDataUnit(dataUnit unt,std::string filename)
{
	std::ofstream dataWriter(filename);
	dataWriter << "strvol=" << unt.stringData.dataVol << ";\n";
	dataWriter << "str=" << unt.stringData.stingData << ";\n";
	dataWriter << "int=" << unt.intData << ";\n";
	dataWriter << "float=" << unt.floatData << ";\n";
	dataWriter.close();
}

//load data structure from file
dataUnit loadDataUnit(std::string filename, bool &success)
{
	dataUnit outp = { {0,""},0,0.0f };
	std::ifstream dataReader(filename);
	std::string rawData="";
	std::string currData = "";
	if (dataReader.is_open())
	{
		while (std::getline(dataReader, currData))
		{
			rawData += currData;
			currData = "";
		}
		dataReader.close();

		int l = rawData.length();
		char currChar = '\n';
		std::string currOp = "";

		for (int i = 0; i < l; i++)
		{
			if (rawData[i] != '\n')
			{
				currChar = rawData[i];
			}
			if (currChar != '\n')
			{
				if ((currChar != ';') && (currChar != '='))
				{
					currData += currChar;
				}
				else
				{
					if (currChar == ';')
					{

						if (currOp == "strvol")
						{
							outp.stringData.dataVol = atoi(currData.c_str());
						}
						if (currOp == "str")
						{
							outp.stringData.stingData = currData;
						}
						if (currOp == "int")
						{
							outp.intData = atoi(currData.c_str());
						}
						if (currOp == "float")
						{
							outp.floatData = atof(currData.c_str());
						}

						currData = "";

					}

					if (currChar == '=')
					{
						currOp = currData;
						currData = "";
					}
				}

			}
		}

		success = true;
		return outp;
	}
	else
	{
		success = false;
		return outp;
	}

}

std::string makeVecFilename(int id)
{
	return "vec_" + std::to_string(id) + ".du";
}

void vectorFileCleanup()
{
	int i = 0;
	int res = 0;
	do 
	{	
		std::string fn = makeVecFilename(i);
		res = remove(fn.c_str());
		i++;
	} while (res = 0);
}

void vectorClear(std::vector<dataUnit> &tgt)
{
	tgt.clear();
}

void vectorAdd(std::vector<dataUnit> &tgt, dataUnit dtu)
{
	tgt.push_back(dtu);
}

void vectorStore(std::vector<dataUnit> &tgt)
{
	vectorFileCleanup();
	int l = tgt.size();
	if (l > 0)
		for (int i = 0; i < l; i++)
		{
			std::string fn = makeVecFilename(i);
			saveDataUnit(tgt[i],fn.c_str());
		}
	std::cout << "Data stored\n";
}

void vectorLoad(std::vector<dataUnit> &tgt)
{
	vectorClear(tgt);
	bool follow=true;
	int i = 0;
	do
	{
		std::string fn = makeVecFilename(i);
		dataUnit temp = loadDataUnit(fn,follow);
		i++;
		if (follow) vectorAdd(tgt,temp);
	} while (follow);
	std::cout << "Data loaded\n";
}

void vectorPrint(std::vector<dataUnit> &tgt)
{
	int l = tgt.size();
	if (l > 0)
		for (int i = 0; i < l; i++)
		{
			std::cout << "++ U" << i << ": ";
			printDataUnit(tgt[i]);
		}
}

std::vector<dataUnit> dataVault = {};

int main()
{
	bool exitLoop = false;
	while (!exitLoop)
	{
		int op = 0;
		std::cout << "Select action: 1 - add, 2 - store, 3 - load, 4 - print, 5 - quit\n";
		std::string sk = "";
		getline(std::cin, sk);
		op = atoi(sk.c_str());
		switch (op)
		{
	 		default:
			case 1:
			{
				vectorAdd(dataVault, setDataUnit());
				break;
			}
			case 2:
			{
				vectorStore(dataVault);
				break;
			}
			case 3:
			{
				vectorLoad(dataVault);
				break;
			}
			case 4:
			{
				vectorPrint(dataVault);
				break;
			}
			case 5:
			{
				exitLoop = true;
				break;
			}			
		}
	}

	system("pause");
}