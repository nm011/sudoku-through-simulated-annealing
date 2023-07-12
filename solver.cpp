#include<bits/stdc++.h>
using namespace std;

// get the grid
// randomly fill each 3x3 box such that it has all numbers from 1 to 9
// at each turn, keep swapping 2 random cells in a random box
// if cost increases within some iterations, randomly fill input grid again
// cost function would be sum of number of symbols missing all together 
// from all rows and columns


// https://www.researchgate.net/publication/220403361_Metaheuristics_can_solve_Sudoku_puzzles

int SEED = 42;                                                // seed for PRNG 
mt19937 rng(SEED);                                            

int randomInt(int low, int high) {
    uniform_int_distribution<> dist(low, high);
    return dist(rng);
}

double randomDouble() {
    uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng);
}

int getCost(vector<vector<int>>& grid) {
    int cost = 0;
    for (int i=0; i<9; i++) {
        set<int> rowSet, columnSet;
        for (int j=0; j<9; j++) {
            rowSet.insert(grid[i][j]);                         // distinct elements in each row
            columnSet.insert(grid[j][i]);                      // distinct elements in each column
        }
        cost += (18 - (rowSet.size() + columnSet.size()));
    }
    return cost;
}

void printGrid(vector<vector<int>>& grid) {
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            cout<<grid[i][j]<<" ";
        }
        cout<<"\n";
    }
    cout<<"\n";
}

void populateGrid(vector<vector<int>>& grid, vector<vector<bool>>& fixed) {
    for (int rowBox=0; rowBox<3; rowBox++) {
        for (int colBox=0; colBox<3; colBox++) {                // go over each 3x3 box

            set<int> boxSet;
            for (int i=1; i<10; i++) boxSet.insert(i);

            for (int i=3*rowBox; i<3*(rowBox+1); i++) {
                for (int j=3*colBox; j<3*(colBox+1); j++) {     // maintain unfixed elements in each box
                    if (fixed[i][j]) boxSet.erase(grid[i][j]);  
                }
            }

            vector<int> boxUnfixed; 
            for (int i:boxSet) boxUnfixed.push_back(i);
            shuffle(boxUnfixed.begin(), boxUnfixed.end(), rng); // shuffle unfixed elements

            int k = 0;
            for (int i=3*rowBox; i<3*(rowBox+1); i++) {
                for (int j=3*colBox; j<3*(colBox+1); j++) {
                    if (!fixed[i][j]) {
                        grid[i][j] = boxUnfixed[k];             // put in shuffled elements at empty places
                        k++;
                    }
                }
            }
            
        }
    }
}

void swapUnfixed(vector<vector<int>>& grid, vector<vector<bool>>& fixed) {

    int rowBox = randomInt(0,2);
    int colBox = randomInt(0,2);
    int i1=0, j1=0;                 
    while (1) {                                     // choose first cell
        i1 = 3*rowBox + randomInt(0,2);
        j1 = 3*colBox + randomInt(0,2);
        if (!fixed[i1][j1]) break;
    }
    int i2=0, j2=0;
    while (1) {                                     // choose second cell
        i2 = 3*rowBox + randomInt(0,2);
        j2 = 3*colBox + randomInt(0,2);
        if (!fixed[i2][j2]) break;
    }

    swap(grid[i1][j1], grid[i2][j2]);               // swap chosen cells

}

double stddev(vector<int>& costs) {
    int size = costs.size();
    double mean = 0;
    for (int i:costs) mean+=i;
    mean = mean/size;
    double var = 0;
    for (int i:costs) var += (i-mean)*(i-mean);
    return var/size;
}

int main() {

    freopen("input.txt", "r", stdin);
    vector<vector<int>> grid(9, vector<int>(9));    //stores grid
    vector<vector<bool>>fixed(9,vector<bool>(9));   //stores fixed places

    string s; cin>>s; 
    int pos = 0;
    for (int i=0; i<9; i++) {
        for (int j=0; j<9; j++) {
            grid[i][j] = s[pos]-'0'; pos++;
            if (grid[i][j]) fixed[i][j] = 1;
        }
    }

    vector<vector<int>> inputGrid = grid;
    cout<<"\nInput Grid : \n";
    printGrid(inputGrid);

    populateGrid(grid, fixed);

    cout<<"Populated Grid : \n";
    printGrid(grid);
    vector<vector<int>> tempGrid;                   // used to convey possible changes to main grid

    vector<int> costs;
    tempGrid = grid;
    for (int i=0; i<20; i++) {                      
        swapUnfixed(tempGrid, fixed);
        costs.push_back(getCost(tempGrid));         // small number of neighbourhood moves
    }                                               // to calculate appropriate initial temperature
    
    // set initial parameters
    int initialCost = getCost(grid);    
    double initialTemp = stddev(costs);
    vector<vector<int>> initialGrid = grid;
    double alpha = 0.99;

    int currCost = initialCost;
    double currTemp = initialTemp;

    vector<vector<int>> currGrid = grid;
    int reheatPeriod = 200;                         // sets reheat period
    queue<int> checkReheat;                         // checks if cost has increased within reheat period
                                                    // for a need to reheat 
    int iterations = 0;
    int maxIterations = 40000;

    while(1) {

        if (iterations > maxIterations) break;       // found a solution (cost = 0)

        if (!currCost) {
            cout<<"Perfect Solution!\n\n"; 
            break;
        }             

        if (iterations<reheatPeriod) 
            checkReheat.push(currCost);

        else {
            if (currCost > checkReheat.front()) {    // changed for the worse (probably) in past reheatPeriod
                currCost = initialCost;              // reset
                currTemp = initialTemp;
                currGrid = inputGrid;
                populateGrid(currGrid, fixed);       // repopulate initial grid

                cout<<"Reheated to get new Populated Grid: \n";
                printGrid(currGrid);

                checkReheat = queue<int>();
                iterations = 0;

            }
            else {
                checkReheat.pop();                    // maintain most recent costs with queue
                checkReheat.push(currCost);           
            }
        }

        tempGrid = currGrid;
        swapUnfixed(tempGrid, fixed);

        int tempCost = getCost(tempGrid);
        double delta = tempCost - currCost; 

        if ( (delta<0) ||  (exp(-delta/currTemp) > randomDouble()) ) {
            currGrid = tempGrid;
            currCost = tempCost;
        }
        
        currTemp *= alpha;
        iterations++;
    }
    
    cout<<"Final Grid :\n";
    printGrid(currGrid);
    cout<<"Cost = "<<currCost<<"\n";
    return 0;
}   