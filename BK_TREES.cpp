// Approximate Query Searching using BK Trees

# include <iostream>
# include <vector>
# include <cstdlib>
# include <cmath>
# include <climits>

const int PI = 3.141592;
const int Size = 200;

using namespace std;

struct node{
    vector<double> tuple;
    node* childs[Size];

    node(vector<double> key){
        tuple = key;
        for(int i = 0; i < Size; i++){
            childs[i] = NULL;
        }
    }

    bool childs_empty(){
        for(int i = 0; i < Size; i++){
            if(childs[i] != NULL){
                return false;
            }
        }
        return true;
    }
};

bool compare(vector<double> tuple1, vector<double> tuple2){
    double epsilon = 0.0001;

    for(int i = 0; i < tuple1.size(); i++){
        if(abs(tuple1[i] - tuple2[i]) > epsilon){
            return false;
        }
    }
    return true;
}

vector<double> generateRandom(int range, int dim){
    vector<double> coordinates;

    for(int i = 0; i < dim; i++){
        double m = ((double)rand()/RAND_MAX);
        double theta = 2 * PI * m;
        double r = (range * m);

        coordinates.push_back(r*cos(theta));
    }

    return coordinates;
}

void printPoints(node* root){
    for(auto i : root->tuple){
        cout<<i<<" ";
    }
    cout<<endl;

    for(int i = 0; i < Size; i++){
        if((root->childs)[i] != NULL){
            printPoints((root->childs)[i]);
        }
    }
}

int EuclideanMetric(vector<double> tuple1, vector<double> tuple2){
    long double sum = 0;

    for(int i = 0; i < tuple1.size(); i++){
        long double diff = (tuple1[i] - tuple2[i]);
        sum += (diff * diff);
    }

    return sqrt(sum);
}

int ManhattanMetric(vector<double> tuple1, vector<double> tuple2){
    long double sum = 0;

    for(int i = 0; i < tuple1.size(); i++){
        long double diff = (tuple1[i] - tuple2[i]);
        sum += abs(diff);
    }

    return sum;
}

int pivots(node* root){
    int count = 0;

    for(int i = 0; i < Size; i++){
        if(root->childs[i] != NULL){
            count ++;
        }
    }

    return count;
}

vector<vector<double>> copy(node* &root){
    static vector<vector<double>> ans;

    if(root == NULL){
        return ans;
    }

    for(int i = 0; i < Size; i++){
        if((root->childs)[i] != NULL){
            ans.push_back((root->childs)[i]->tuple);
            copy((root->childs)[i]);

            delete((root->childs)[i]);
        }
    }

    return ans;
}

void Insertion(node* &root, vector<double> childTuple){
    int distance = EuclideanMetric(root->tuple, childTuple);

    if((root->childs)[distance] == NULL){
        (root->childs)[distance] = new node(childTuple);
        return;
    }

    Insertion((root->childs)[distance], childTuple);
}

void deletion(node* &root, vector<double> Node){
    int distance = EuclideanMetric(root->tuple, Node);
    vector<vector<double>> children;

    if((root->childs)[distance] == NULL){
        cout<<"Given point does not exist in the tree"<<endl;
        return;
    }

    if(compare((root->childs)[distance]->tuple, Node)){
        children = copy((root->childs)[distance]);
        (root->childs)[distance] = NULL;

        if(!children.empty()){
            for(auto i : children){
                Insertion(root, i);
            }
        }

        return;
    }

    deletion((root->childs)[distance], Node);
}

node* LookUp(node* root, vector<double> query){
    static int dist_best = INT_MAX;
    static node* best_vertex = NULL;   // tuple corresponing to dist_best

    int distance = EuclideanMetric(root->tuple, query);

    if(distance < dist_best){
        dist_best = distance;
        best_vertex = root;
    }

    if(root->childs_empty()){
        return best_vertex;
    }

    for(int i = 0; i < Size; i++){
        if((root->childs)[i] != NULL){
            if(abs(distance - i) < dist_best){
                best_vertex = LookUp((root->childs)[i], query);
            }
        }
    }

    return best_vertex;
}

void isNearest(vector<vector<double>> random, vector<double> query){
    vector<double> nearest;

    int minDist = INT_MAX;
    for(auto i : random){
        int dist = EuclideanMetric(i, query);

        if(dist < minDist){
            nearest = i;
            minDist = dist;
        }
    }

    cout<<"Nearest point from Brute-force approach : "<<endl;
    for(auto i : nearest){
        cout<<i<<" ";
    }
    cout<<endl;
}

int main()
{
    int range;
    cout<<"Enter range (For example, enter 10 if the range is [-10, 10]) : ";
    cin>>range;

    int dim;
    cout<<"Enter dimension of the space from which the points must be generated : ";
    cin>>dim;

    int N;
    cout<<"Enter number of points in space : ";
    cin>>N;

    vector<double> query(dim);
    cout<<"Enter a query point : ";
    for(int i = 0; i < dim; i++){
        cin>>query[i];
    }

    vector<vector<double>> random;
    for(int i = 0; i < N; i++){
        random.push_back(generateRandom(range, dim));
    }

    node* root = new node(random[0]);
    for(int i = 1; i < random.size(); i++){
        Insertion(root, random[i]);
    }

    // cout<<"Points are : "<<endl;
    // printPoints(root);

    cout<<endl;
    cout<<"Considering "<<pivots(root)<<" number of pivots"<<endl;

    node* approx = LookUp(root, query);
    cout<<"Nearest point to the given query point is "<<endl;
    for(int i = 0; i < (approx->tuple).size(); i++){
        cout<<(approx->tuple)[i]<<" ";
    }
    cout<<endl;

    isNearest(random, query);

    // cout<<"Deletion check : "<<endl;
    // vector<double> n = {-3.46431, -1.81324, -3.45956, 1.38276, -6.66846, -9.89239, -0.697006, 3.4743, -0.623936, -2.48123};
    // deletion(root, n);

    // cout<<"Pivots are : "<<endl;
    // printPoints(root);

    return 0;
}