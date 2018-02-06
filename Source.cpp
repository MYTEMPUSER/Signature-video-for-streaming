
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


void reconfigure(char * file_name)
{
	ifstream file(file_name, ios::binary);
	fstream show("output.txt", ios::binary | ios::out);
	char level;
	char H[128];
	for (int i = 0; i < 128; i++)
		H[i] = 0;
	if (file.is_open())
	{
		while (!file.eof()) {
			file.read(&level, sizeof(level));
			s.push_back(level);
			if (s.size() == 128 || file.eof())
			{
				while (s.size() < 128)
					s.push_back(0);

				for (int i = 0; i < s.size(); i++)
				{
					show.write(&s[i], sizeof(char));
					H[i] = (s[i] | H[i]);
				}
				string HH = sw::sha512::calculate(H);
				for (int i = 0; i < 128; i++)
				{
					H[i] = HH[i];
					show.write(&H[i], sizeof(char));
				}
				s.clear();
			}
		}
		file.close();
		show.close();
	}
	else cout << "Unable to open file";
}

void read_and_check(char * file_name)
{
	ifstream file(file_name, ios::binary);
	fstream show("output.mp3", ios::binary | ios::out);
	char level;
	int cnt = 0;
	char H[128], HH[128];
	for (int i = 0; i < 128; i++)
	{
		HH[i] = 0;
		H[i] = 0;
	}
	if (file.is_open())
	{
		while (!file.eof()) {
			file.read(&level, sizeof(level));
			s.push_back(level);
			if (s.size() == 128 || file.eof())
			{
				cnt++;

				if (cnt % 2 == 1)
				{
					for (int i = 0; i < s.size(); i++)
					{
						show.write(&s[i], sizeof(char));
						H[i] = (s[i] | H[i]);
					}
					string HH = sw::sha512::calculate(H);
					for (int i = 0; i < 128; i++)
						H[i] = HH[i];
					s.clear();
				}
				else
				{
					string s2;
					for (int i = 0; i < 128; i++)
						s2.push_back(H[i]);
					if (s != s2)
					{
						cout << "EEEE";
						return;
					}
				}
				s.clear();
			}
		}
		file.close();
		show.close();
	}
	else cout << "Unable to open file";
}

int main()
{
	reconfigure("White_Power_-_Skinhed_(iPleer.fm).mp3");
	read_and_check("output.txt");
	return 0;
}