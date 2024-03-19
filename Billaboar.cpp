#include "parse.hpp"
#include "repayments.hpp"
#include <fstream>
using namespace std;


float* compute_debtors_percentage(float debtors[], int len, float percentage[]){
    float sum = 0;
    for (int i=0;i<len;i++){
        sum += debtors[i];
    }
    for (int i=0;i<len;i++){
        percentage[i] = (debtors[i] / sum) * 100;
    }
    return percentage;
}

void output(map<int, string> &d_dict, map<int, string> &c_dict, const vector<float>& debtors, const vector<float>& creditors, const Matrix& repayments_matrix, string filename){

	ofstream file(filename);
	file << "Debts:" << endl;
    for (int i=0; i<debtors.size(); i++){
        file << d_dict[i] << " owes " << debtors[i] << endl;
    }
    for (int i=0; i<creditors.size(); i++){
        file << c_dict[i] << " gets " << creditors[i] << endl;
    }
    file << endl;

	file << "Repayments:" << endl;
	for (int i=0; i<repayments_matrix.rows; i++){
        for (int j=0; j<repayments_matrix.columns; j++){
            if (repayments_matrix(i,j) > 0){
                file << d_dict[i] << " pays " << c_dict[j] << " " << repayments_matrix(i,j) << endl;
            }
        }
    }
	file.close();
}

char input(int size) {
	char method;
	cout << "Calculate minimal repayments?" << endl;
	cout << "Otherwise will use a naive method for arbitrary repayments." << endl;
	cout << "You have " << size << " different names ";
	if (size > 8) {
		cout << "so calculation might take a while." << endl;
	}
	else {
		cout << "which shouldn't take long." << endl;
	}
	cout << "[y\\n]" << endl;
	cin >> method;
	if (method != 'y' && method != 'n') {
		cerr << "Not a valid input";
		return -1;
	}
	return method;
}

int main(int argc, char* argv[])
{
	if (argc != 3) {
		cout << "Usage is [path to BreakBill.exe] [csv input file path] [output file]";
		return -1;
	}

	vector<pair<string,float>> data;
    read_file(argv[1], data);
	map<string,float> dict;
    group_bills(data, dict);
	map<string,float> d_and_c;
    compute_debts(dict, d_and_c);

    vector<float> debtors, creditors;
    map<int, string> d_dict, c_dict;
    debts_to_vectors(d_and_c, d_dict, c_dict, debtors, creditors);

	char method = input(d_and_c.size());

	Matrix repayments(debtors.size(), creditors.size());
	if (method == 'y') {
		
		repayments = compute_minimal_repayments(&debtors[0], debtors.size() , &creditors[0], creditors.size());
	}
	else if (method == 'n') {
		repayments = naive_repayments(&debtors[0], debtors.size() , &creditors[0], creditors.size());
	}

	output(d_dict, c_dict, debtors, creditors, repayments, argv[2]);
}