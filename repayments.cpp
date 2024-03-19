#include <vector>
#include <algorithm>
#include "repayments.hpp"
#include <cmath>
using namespace std;

float FLOAT_ACCURACY = 0.001;

Matrix init_edge_matrix(const float debtors[], int d_len, const float creditors[], int c_len) {
    Matrix matrix(d_len + c_len + 1, d_len + c_len + 2); // there's no row for the sink because no edges come out of it
    for (int i=1; i<d_len+1; i++){
        for (int j=d_len+1; j<c_len+d_len+1; j++){
            matrix(i,j) = -debtors[i-1];
        }
    }
    for (int j=1; j<d_len+1; j++){
        matrix(0,j) = -debtors[j-1];
    }
    for (int i=d_len+1; i<c_len+d_len+1; i++){
        matrix(i,c_len+d_len+1) = creditors[i-d_len-1];
    }
    return matrix;
}

/*
Gets the edge matrix of the residual network.
If there's a path from the source to the sink, returns the pair {debtor index, creditor index} which represents the
path source > debtor > creditor > sink. 
The flow can be computed by min(edge_matrix[0][debtor], edge_matrix[debtor][creditor], edge_matrix[creditor][-1])
*/
pair<int, int> find_augmentation_path(const Matrix& edge_matrix, int d_len, int c_len){
    for (int i=1; i<d_len+1; i++) // iterate over all edges from the source
    {
        if (edge_matrix(0,i) > 0) // if an edge from the source to the first layer (the debtors) exists
        {
            for (int j=i; j<d_len + c_len + 1; j++) // iterate over all edges from this debtor
            {
                if (edge_matrix(i,j) > 0) // if an edge from the debtor to a creditor exists
                {
                    if (edge_matrix(j,c_len+d_len+1) > 0) // if and edge from the creditor to the sink exists
                    {
                        // the augmentation path is source > i > j > sink
                        // the flow is min(edge_matrix[0][i], edge_matrix[i][j], edge_matrix[j][c_len+d_len+1])
                        return {i,j};
                    }
                }
            }
        }
    }
    // no path
    return {-1,-1};
}

Matrix find_max_flow(const Matrix &graph, int d_len, int c_len, float* max_flow_result) {
    Matrix residual(graph);
    float max_flow = 0;
    auto path = find_augmentation_path(residual, d_len, c_len);
    while (path.first != -1)
    {
        float new_flow = std::min({residual(0,path.first), residual(path.first,path.second), residual(path.second, c_len + d_len + 1)});
        max_flow += new_flow;
        residual(0,path.first) -= new_flow;
        residual(path.first,path.second) -= new_flow;
        residual(path.second,c_len + d_len + 1) -= new_flow;
        path = find_augmentation_path(residual, d_len, c_len);
    }
    *max_flow_result = max_flow;
    return graph - residual;
}

int num_of_edges(const Matrix &graph, const int& d_len, const int& c_len){
    int num = 0;
    for (int i=1; i<d_len+1; i++){
        for (int j=d_len+1; j<c_len+d_len+1; j++){
            if (graph(i,j) > 0) ++num;
        }
    }
    return num;
}


// working, maybe there're ways to reduce runtime
void minimal_repayments_helper(Matrix &graph, int i, int j, const int& d_len, const int& c_len, const float& total_payment, int& minimal, Matrix* result) {
     
    if (i >= d_len+1) return;
    else if (j >= c_len+d_len+1) {
        minimal_repayments_helper(graph, i+1, d_len+1, d_len, c_len, total_payment, minimal, result);
        return;
    }
    float max_flow;
    float prev = graph(i,j);
    
    graph(i,j) = 0;
    auto flow_graph = find_max_flow(graph, d_len, c_len, &max_flow);
    if ((max_flow > total_payment) || (total_payment-max_flow < FLOAT_ACCURACY)) {
        int n = num_of_edges(graph, d_len, c_len);
        if (n <= minimal) {
            minimal = n;
            *result = flow_graph;
        }
        if (minimal == d_len) return;
        minimal_repayments_helper(graph, i, j+1, d_len, c_len, total_payment, minimal, result);
    }
    
    graph(i,j) = prev;
    flow_graph = find_max_flow(graph, d_len, c_len, &max_flow);
    if ((max_flow > total_payment) || (total_payment-max_flow < FLOAT_ACCURACY)) {
        int n = num_of_edges(graph, d_len, c_len);
        if (n <= minimal) {
            minimal = n;
            *result = flow_graph;
        }
        if (minimal == d_len) return;
        minimal_repayments_helper(graph, i, j+1, d_len, c_len, total_payment, minimal, result);
    }
}

Matrix repayment_graph(Matrix &graph, int d_len, int c_len) {
    Matrix result(d_len, c_len);
    for (int i=1; i<d_len+1; i++){
        for (int j=d_len+1; j<d_len + c_len +1; j++){
            result(i-1,j-d_len-1) = graph(i,j); 
        }
    }
    return result;
}

/*
Make a network flow graph from debtors to creditors. 
Each debtor has an edge directed to each one of the creditors, with a capacity of its debt.
We have d*c edges.
The max flow is obviously the sum of all debts.
In each iteration we remove an edge and check if the max flow stays the same.
If so, it's a legitimate graph.
We choose the graph with the minimum edges.
We'll need to compute all subset of edges, meaning 2^(d*c). Actually not exactly beacause there're some restraints.
Each iteration we need to compute the max flow which takes (d+c)*(d*c)^2.
All in all, O(n) = [2^(d*c)]*[(d+c)*(d*c)^2] which is exponantial but oh well.
*/
Matrix compute_minimal_repayments(const float debtors[], int d_len, const float creditors[], int c_len) {
    auto graph = init_edge_matrix(debtors, d_len, creditors, c_len);

    float total_payment = 0;
    for (int i = 0; i< c_len; i++) total_payment += creditors[i];

    int minimal = d_len * c_len;
    Matrix result(graph);
    minimal_repayments_helper(graph, 1, d_len+1, d_len, c_len, total_payment, minimal, &result);
    return repayment_graph(result, d_len, c_len); // the cell (i,j) represent the debtor i pays the creditor j the amount in the cell 
}

/*
A naive algorithm for computing the repayments:
Make a network flow graph from debtors to creditors. 
Each debtor has an edge directed to each one of the creditors, with a capacity of its debt.
We have d*c edges.
The max flow is obviously the sum of all debts.
We find the max flow network with Edmonds-Karp. It should take O(d*c*[d+c]^2) which is polynomial.
Returns a repayment matrix, not neccessarily the minimal one. 
*/
Matrix naive_repayments(const float debtors[], int d_len, const float creditors[], int c_len)
{
    Matrix graph = init_edge_matrix(debtors, d_len, creditors, c_len);
    float total_payment = 0;
    for (int i = 0; i< c_len; i++) total_payment += creditors[i];
    
    Matrix result = find_max_flow(graph, d_len, c_len, &total_payment);
    return repayment_graph(result, d_len, c_len);
}


