#include <iostream>
#include<string>
#include<fstream>
#include<istream>
#include<vector>
#include<cstdlib>
#include<algorithm>
#include<iterator>
#define NUM 4

using namespace std;
vector<string> split(const string &s, const string &seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}



int main()
{
	ifstream infile("matrix");
	string temp;
	getline(infile, temp);
	vector<string> num = split(temp, " ");
	int row, col;

	row = atoi(num[0].c_str());
	col = atoi(num[1].c_str());
	vector<string> tmp, a;
	while (getline(infile, temp))
	{
		tmp = split(temp, " ");
		a.insert(a.end(), tmp.begin(), tmp.end());
	}
	vector<double> awa;
	transform(a.begin(), a.end(), back_inserter(awa), [](string &str) {return stod(str); });

	vector<double> awat;
	for (int i = 0; i < col; i++)
	{

		for (int j = 0; j < row; j++)
			awat.push_back(*(awa.begin() + i + j*col));
	}


	vector<double> b1, b2, v1, v2;
	vector<double> v;
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < col; j++)
		{
			v.push_back(0);
			if (i == j)
			{
				v.pop_back();
				v.push_back(1);
			}
		}
	}

	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i != j)
			{
				b1.insert(b1.end(), awat.begin() + i*row, awat.begin() + i*row + row);
				b2.insert(b2.end(), awat.begin() + j*row, awat.begin() + j*row + row);
				v1.insert(v1.end(), v.begin() + i*col, v.begin() + i*col + col);
				v2.insert(v2.end(), v.begin() + j*col, v.begin() + j*col + col);
			}
		}
	}
	cout << "b1's size:" << b1.size() << " " << "b2's size:" << b2.size() << "\n";
	for (vector<string>::size_type i = 0; i != b1.size(); ++i)
		cout << b1[i] << " ";
	cout << endl;
	for (vector<string>::size_type i = 0; i != b2.size(); ++i)
		cout << b2[i] << " ";
	cout << endl;
	cout << "b1's size:" << v1.size() << " " << "b2's size:" << v2.size() << "\n";
	for (vector<string>::size_type i = 0; i != v1.size(); ++i)
		cout << v1[i] << " ";
	cout << endl;
	for (vector<string>::size_type i = 0; i != v2.size(); ++i)
		cout << v2[i] << " ";
	cout << endl;

	char matrixFile[111] = "matrix_Xw_Y";
	matrixFile[7] = '0' + NUM;

	int aa, bb;
	aa = col / NUM; //除数
	bb = col%NUM;   //余数

	
	for (int k = 0; k < bb; k++)
	{
		matrixFile[10] = '1' + k;
		cout << matrixFile << "\n";

		ofstream outfile;
		outfile.open(matrixFile);
		outfile << aa + 1 << "\n" << (aa + 1)*(col - 1) << "\n";

		int taotao=0;
		int pp = 0, tt1 = 0;
		for (int mm = 0; mm < col; mm++)
		{
			int counter = 0;;
			if (mm%NUM == k)
			{				
				cout << counter<<endl;
				counter++;
				
				for (int i=( NUM*taotao+k)*(col-1);i<(NUM*taotao+1+k)*(col-1);i++)
				{						
					if (mm != tt1)
					{
						pp = 0;
					}					
					if (pp == mm)
					{
						pp++;
					}
					
					cout << mm <<" "<<pp<<" ";
					outfile << mm << " " << pp << " ";
					pp++;
					tt1 = mm;
					for (int j = 0; j < col - 1; j++)
					{
						cout << *(v1.begin() + i*col + j) << " ";
						outfile << *(v1.begin() + i*col + j) << " ";
					}
					cout << *(v1.begin() + i*col + col - 1);
					outfile << *(v1.begin() + i*col + col - 1);
					cout << " ";
					outfile << " ";
					for (int j = 0; j < row - 1; j++)
					{
						cout << *(b1.begin() + i*row + j) << " ";
						outfile << *(b1.begin() + i*row + j) << " ";
					}
					cout << *(b1.begin() + i*row + row - 1);
					outfile << *(b1.begin() + i*row + row - 1);
					cout << "\n";
					outfile << "\n";
				}
				taotao++;
			}
		}
		outfile.close();
	}

	
	for (int k = bb; k < NUM ; k++)
	{
		matrixFile[10] = '1' + k;
		cout << matrixFile << "\n";

		ofstream outfile;
		outfile.open(matrixFile);
		outfile << aa << "\n" << aa *(col - 1) << "\n";

		int taotao = 0;
		int pp = 0, tt1 = 0;
		for (int mm = 0; mm < col; mm++)
		{
			
			if (mm%NUM == k)
			{
				
				cout << "k:" << k << endl;
				for (int i = (NUM*taotao+k)*(col - 1); i<(NUM*taotao +k+ 1)*(col - 1); i++)
				{
					if (mm != tt1)
					{
						pp = 0;
					}
					if (pp == mm)
					{
						pp++;
					}
					cout << mm << " " << pp << " ";
					outfile << mm << " " << pp << " ";
					pp++;
					tt1 = mm;
					for (int j = 0; j < col - 1; j++)
					{
						cout << *(v1.begin() + i*col + j) << " ";
						outfile << *(v1.begin() + i*col + j) << " ";
					}
					cout << *(v1.begin() + i*col + col - 1);
					outfile << *(v1.begin() + i*col + col - 1);
					cout << " ";
					outfile << " ";
					for (int j = 0; j < row - 1; j++)
					{
						cout << *(b1.begin() + i*row + j) << " ";
						outfile << *(b1.begin() + i*row + j) << " ";
					}
					cout << *(b1.begin() + i*row + row - 1);
					outfile << *(b1.begin() + i*row + row - 1);
					cout << "\n";
					outfile << "\n";
				}
				taotao++;
			}
		}
		outfile.close();






		/*matrixFile[10] = '1' + bb + k;
		cout << matrixFile << "\n";

		ofstream outfile;
		outfile.open(matrixFile);
		outfile << aa << "\n" << aa *(col - 1) << "\n";

		for (int i = bb*(aa + 1)*(col - 1) + k*aa*(col - 1); i < bb*(aa + 1)*(col - 1) + (k + 1)*aa*(col - 1); i++)
		{
			int tt = i / (col - 1);
			if (tt != tt1)
			{
				pp = 0;
			}
			if (pp == tt)
			{
				pp++;
			}
			pp++;
			//cout << tt + 1 << " " << pp << " ";
			//outfile << tt + 1 << " " << pp << " ";
			cout << tt << " " << pp - 1 << " ";
			outfile << tt << " " << pp - 1 << " ";
			tt1 = tt;

			for (int j = 0; j < col - 1; j++)
			{
				cout << *(v1.begin() + i*col + j) << " ";
				outfile << *(v1.begin() + i*col + j) << " ";
			}
			cout << *(v1.begin() + i*col + col - 1);
			outfile << *(v1.begin() + i*col + col - 1);
			cout << " ";
			outfile << " ";
			for (int j = 0; j < row - 1; j++)
			{
				cout << *(b1.begin() + i*row + j) << " ";
				outfile << *(b1.begin() + i*row + j) << " ";
			}
			cout << *(b1.begin() + i*row + row - 1);
			outfile << *(b1.begin() + i*row + row - 1);


			cout << "\n";
			outfile << "\n";
		}
		outfile.close();*/
	}


	return 0;
}


