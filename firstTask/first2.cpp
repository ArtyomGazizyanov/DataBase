// first2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include<iostream>
#include <vector>
#include <map>

#include <string>

/*вектор мап
в мап ищем нужные вхождения*/

using namespace std;
map<string, int> GetColumnName(fstream & db)
{
	map<string, int> columnNames;
	string tempLine;
	string delim(",");
	size_t prev = 0;
	size_t next;
	size_t delta = delim.length();
	getline(db, tempLine);
	int column = 0;
	while ((next = tempLine.find(delim, prev)) != string::npos)
	{
		string foundString = tempLine.substr(prev, next - prev);
		prev = next + delta; 
		columnNames.insert(pair<string, int>(foundString, column));
		column++;
	}
	return columnNames;
}

string MakeValidName(string name)
{
	string delim("\""); 
	if (!name.find(delim))
	{
		//erase(name.begin());
		name.erase(name.begin());
		name.erase(name.end()-1); 
	}
		
	return name;
} 

void PrintSearchResult(vector<vector<string>> finded)
{
	for (auto i : finded)
	{
		cout << i[0] << endl;
	}
}

void FindAndInsertIntoMap(fstream & db, map<string, vector<vector<string>>> & dbMap, size_t MaxBDbufferSize, map<string, int> columnNames, string InputUsername)
{

	size_t BDbuffer = 0;
	while (!db.eof() && BDbuffer < MaxBDbufferSize)
	{
		string tempLine;
		string delim(",");
		size_t prev = 0;
		size_t next;
		size_t delta = delim.length();
		vector<string> oneStringArray;
		getline(db, tempLine);
		int column = 0;
		string authorName;
		bool isRightName = false;
		while ((next = tempLine.find(delim, prev)) != string::npos)
		{
			string foundSubStr = tempLine.substr(prev, next - prev);
			if (columnNames["author"] == column)
			{
				authorName = MakeValidName(foundSubStr);
				if (authorName == InputUsername)
				{					
					isRightName = true;
				}
			}
			else
			{
				oneStringArray.push_back(foundSubStr);
			}
			prev = next + delta;
			column++;
		}
		if (isRightName)
		{
			if (dbMap.count(authorName))
			{
				dbMap[authorName].push_back(oneStringArray);
			}
			else
			{
				vector<vector<string>> vectorString;
				vectorString.push_back(oneStringArray);
				dbMap.insert(pair<string, vector<vector<string>>>(authorName, vectorString));
			}
			cout << oneStringArray[0] <<endl;
		}
		BDbuffer++;
	}
}

string GetUsername()
{
	string getstr;
	getline(cin, getstr);
		
	return getstr;
}

int main()
{
	fstream db("db.csv"); 
	size_t MaxBDbufferSize = 100;

	map<string, int> columnNames = GetColumnName(db);
	map<string, vector<vector<string>>> dbMap;
	size_t lastOutputContro = 0;
	string username;

	cout << "Username: "; 
	username = GetUsername();

	cout << "ID`s: " << endl;
	do
	{
		FindAndInsertIntoMap(db, dbMap, MaxBDbufferSize, columnNames, username);
	} while (!db.eof());

	vector<vector<string>> searchResult = dbMap[username];
	if (searchResult.empty())
	{
		cout << "There`s no this user" << endl;
	}
	//PrintSearchResult(searchResult);
    return 0;
}

		