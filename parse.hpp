#include <map>
#include <vector>
#include <string>

void read_file(const std::string filename, std::vector<std::pair<std::string, float>>& result);
void group_bills(const std::vector<std::pair<std::string, float>> &data, std::map<std::string, float>& dict);
void compute_debts(const std::map<std::string,float>& dict, std::map<std::string, float>& result);
void debts_to_vectors(const std::map<std::string,float>& dict, std::map<int,std::string>& d_dict, std::map<int,std::string>& c_dict, std::vector<float>& debtors, std::vector<float>& creditors);