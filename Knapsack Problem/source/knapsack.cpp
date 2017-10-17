#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;

struct Knapsack
{
    int num_items_;
    int capacity_;
    int *weight;
    int *value;
};

inline void getData( struct Knapsack &knapsack, const std::string &filename ) // Extrai os processos do arquivo. ( antigo )
{
    std::ifstream file(filename);

    file >> knapsack.num_items_;
    knapsack.weight = new int[knapsack.num_items_];
    knapsack.value = new int[knapsack.num_items_];
    file >> knapsack.capacity_;

    int i, j, k = 0;
    while ( file >> i >> j )
    {
        knapsack.weight[k] = i;
        knapsack.value[k] = j;
        k++;
    }
}

struct Cell{
    int objective_;
    bool x;
};

// Returns the maximum value that can be put in a knapsack of capacity W
int Knapsack( const Knapsack &knapsack )
{
    std::vector< std::vector<Cell> > table_vector( knapsack.capacity_ + 1, std::vector<Cell>( knapsack.num_items_ + 1 )  );
    std::vector<Cell> *K = table_vector.data();

    for ( int i = knapsack.num_items_; i >= 0 ; i-- )
    {
        for ( int w = 0; w <= knapsack.capacity_ ; w++)
        {
            if ( ( i == knapsack.num_items_ ) || w==0 )
                K[w][i].objective_ = 0;
            else if ( knapsack.weight[i] <= w )
            {
                if( K[w][i+1].objective_ > ( knapsack.value[i] + K[w - knapsack.weight[i]][i+1].objective_ ) )
                {
                    K[w][i].objective_ = K[w][i+1].objective_;
                    K[w][i].x = false;
                }
                else
                {
                    K[w][i].objective_ = knapsack.value[i] + K[w - knapsack.weight[i]][i+1].objective_;
                    K[w][i].x = true;
                }
            }
            else
            {
                K[w][i].objective_ = K[w][i+1].objective_;
                K[w][i].x = false;
            }
        }
    }

    int bestValue = K[knapsack.capacity_][0].objective_;
    int line = knapsack.capacity_;
    int column = 0;    

    while( bestValue > 0 )
    {
        if( !K[line][column].x )
        {
            column++;
        }
        else
        {
            cout << "Product " << column + 1 << " used\n";
            line = line - knapsack.weight[column];
            bestValue -= knapsack.value[column];
            column++;
        }
    }
    return K[knapsack.capacity_][0].objective_;
}

int main( int argc, char *argv[ ] )
{
    std::string filename;

    if ( argc == 1 )
    {
        filename = "../instancias_mochila/mochila02.txt";
    }
    else
    {
        filename = argv[1];
    }

    struct Knapsack knapsack;
    getData( knapsack, filename );

    clock_t begin = clock();
    cout << "Total profit: " << Knapsack( knapsack ) << endl;
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf( "Time spent: %f seconds\n", time_spent );
    return 0;
}
