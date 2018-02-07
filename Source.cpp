
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <functional>
#include <set>
#include <queue>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <list>
#include "sha512.hh"


using namespace std;

string s;
const int block_size = 128;

void reconfigure(char * file_name)
{
	ifstream in_file(file_name, ios::binary);
	fstream out_file("output.txt", ios::binary | ios::out);

	string H;
	if (in_file.is_open())
	{
		while (!in_file.eof()) {
			char byte;
			in_file.read(&byte, sizeof(byte));
			s.push_back(byte);
			if (s.size() == block_size || in_file.eof())
			{
				while (s.size() < block_size)
					s.push_back(0);
				for (int i = 0; i < s.size(); i++)
					out_file.write(&s[i], sizeof(char));
				s += H;
				H = sw::sha512::calculate(s);
				for (int i = 0; i < block_size; i++)
					out_file.write(&H[i], sizeof(char));
				s.clear();
			}
		}
		in_file.close();
		out_file.close();
	}
	else cout << "Unable to open file";
}

void check_and_recover(char * file_name)
{
	ifstream in_file(file_name, ios::binary);
	fstream out_file("output.mp3", ios::binary | ios::out);

	string H;
	int cnt = 0;
	if (in_file.is_open())
	{
		while (!in_file.eof()) {
			char byte;
			in_file.read(&byte, sizeof(byte));
			s.push_back(byte);
			if (s.size() == block_size || in_file.eof())
			{
				cnt++;
				while (s.size() < block_size)
					s.push_back(0);
				if (cnt % 2 == 1)
				{
					for (int i = 0; i < s.size(); i++)
						out_file.write(&s[i], sizeof(char));
					s += H;
					H = sw::sha512::calculate(s);
				}
				else
					if (s != H)
					{
						throw "Bad data";
						return;
					}
				s.clear();
			}
		}
		in_file.close();
		out_file.close();
	}
	else cout << "Unable to open file";
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Bad number of arguments";
	}
	else
	{
		if (!strcmp(argv[1], "-c"))
			reconfigure(argv[2]);
		if (!strcmp(argv[1], "-uc"))
			check_and_recover(argv[2]);
	}
	return 0;
}