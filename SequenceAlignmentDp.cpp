#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<fstream>
#include<sys/time.h>
#include<sys/resource.h>

using namespace std;

//string generator
string strGenerator(const string& baseStr, const vector<int>& insertionIdx){
	string currStr = baseStr;
	for(auto& idx: insertionIdx){
		string temp = currStr.substr(0, idx + 1);
		temp += currStr;
		temp += currStr.substr(idx + 1);
		currStr = temp;
	}
	return currStr;
}

//initialize dp matrix, alpha table, delta value
void initialize(const string& str1, const string& str2, vector<vector<int>>& dp, unordered_map<char, unordered_map<char, int>>& alpha, int& delta){
	dp = vector<vector<int>>(str1.size() + 1, vector<int>(str2.size() + 1));
	alpha['A']['A'] = 0, alpha['A']['C'] = 110, alpha['A']['G'] = 48, alpha['A']['T'] = 94;
	alpha['C']['A'] = 110, alpha['C']['C'] = 0, alpha['C']['G'] = 118, alpha['C']['T'] = 48;
	alpha['G']['A'] = 48, alpha['G']['C'] = 118, alpha['G']['G'] = 0, alpha['G']['T'] = 110;
	alpha['T']['A'] = 94, alpha['T']['C'] = 48, alpha['T']['G'] = 110, alpha['T']['T'] = 0;
	delta = 30;
}

//dp calculating min cost
//OPT(i, j) = min(OPT(i - 1, j - 1) + alpha(str1[i], str2[j]), OPT(i - 1, j) + delta, OPT(i, j - 1) + delta)
//base cases: i == 0, j == 0
int align(const string& str1, const string& str2, vector<vector<int>>& dp, unordered_map<char, unordered_map<char, int>>& alpha, const int& delta){
	dp[0][0] = 0;
	for(int i = 1; i < dp.size(); ++i){
		dp[i][0] = i * delta;
	}
	for(int j = 1; j < dp[0].size(); ++j){
		dp[0][j] = j * delta;
	}
	for(int i = 1; i < dp.size(); ++i){
		for(int j = 1; j < dp[0].size(); ++j){
			dp[i][j] = min({dp[i - 1][j - 1] + alpha[str1[i - 1]][str2[j - 1]], dp[i - 1][j] + delta, dp[i][j - 1] + delta});
		}
	}
	return dp.back().back();
}

//backtracking sequence: [first 50 of first string, last 50 of first string, first 50 of second string, last 50 of second string]
vector<string> backtrack(const string& str1, const string& str2, const vector<vector<int>>& dp, unordered_map<char, unordered_map<char, int>>& alpha, const int& delta){
	int i = dp.size() - 1, j = dp[0].size() - 1;
	string seq1 = "", seq2 = "";
	while(i > 0 || j > 0){
		if(i - 1 >= 0 && j - 1 >= 0 && dp[i][j] == dp[i - 1][j - 1] + alpha[str1[i - 1]][str2[j - 1]]){
			seq1 += str1[i - 1];
			seq2 += str2[j - 1];
			--i, --j;
		}
		else if(j - 1 >= 0 && dp[i][j] == dp[i][j - 1] + delta){
			seq1 += '_';
			seq2 += str2[j - 1];
			--j;
		}
		else{
			seq1 += str1[i - 1];
			seq2 += '_';
			--i;
		}
	}
	vector<string> subSequences;
	int seq1Size = seq1.size(), seq2Size = seq2.size();
	subSequences.push_back((seq1Size > 50) ? seq1.substr(seq1Size - 50) : seq1), subSequences.push_back((seq1Size > 50) ? seq1.substr(0, 50) : seq1);
	subSequences.push_back((seq2Size > 50) ? seq2.substr(seq2Size - 50) : seq2), subSequences.push_back((seq2Size > 50) ? seq2.substr(0, 50) : seq2);
	for(auto& subSeq: subSequences){
		reverse(subSeq.begin(), subSeq.end());
	}
	return subSequences;
}

//input handling
void inputHandler(const string& filename, vector<int>& insertionIdx1, vector<int>& insertionIdx2, string& baseStr1, string& baseStr2){
	ifstream inputFile(filename);
	string inputStr;
	getline(inputFile, baseStr1);
	if(baseStr1.back() == '\r'){
		baseStr1.pop_back();
	}
	while(getline(inputFile, inputStr) && inputStr[0] >= '0' && inputStr[0] <= '9'){
		insertionIdx1.push_back(stoi(inputStr));
	}
	baseStr2 = (inputStr.size() == 0 || isdigit(inputStr[0])) ? "" : inputStr;
	if(baseStr2.back() == '\r'){
		baseStr2.pop_back();
	}
	while(getline(inputFile, inputStr)){
		insertionIdx2.push_back(stoi(inputStr));
	}
	inputFile.close();
}

//output handling
void outputHandler(ofstream& outputFile, vector<string>& subSequences, int& minCost){
	outputFile << subSequences[0] << " " << subSequences[1] << endl;
	outputFile << subSequences[2] << " " << subSequences[3] << endl;
	outputFile << minCost << endl;
}

int main(int argc, char* argv[]){
	string filename(argv[1]);
	vector<int> insertionIdx1, insertionIdx2;
	string baseStr1, baseStr2;
	inputHandler(filename, insertionIdx1, insertionIdx2, baseStr1, baseStr2);
	vector<vector<int>> dp;
	unordered_map<char, unordered_map<char, int>> alpha;
	int delta;
	string str1 = strGenerator(baseStr1, insertionIdx1), str2 = strGenerator(baseStr2, insertionIdx2);
	initialize(str1, str2, dp, alpha, delta);
	int minCost = align(str1, str2, dp, alpha, delta);
	vector<string> subSequences = backtrack(str1, str2, dp, alpha, delta);
	ofstream outputFile("output.txt");
	outputHandler(outputFile, subSequences, minCost);
	int who = RUSAGE_SELF;
	struct rusage usage;
	getrusage(who, &usage);
	outputFile << (double) (usage.ru_utime.tv_sec) + (usage.ru_utime.tv_usec / 1000000.0) << endl;
	outputFile << usage.ru_maxrss << endl;
	outputFile.close();
	return 0;
}
