#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <string>
#include <time.h>
#include <vector>

using namespace std;
class Cell
{
public:
    Cell()
    {
        up = true;
        down = true;
        right = true;
        left = true;

        border_up = false;
        border_down = false;
        border_right = false;
        border_left = false;
    }

    //0 = up
    //1 = down
    //2 = left
    //3 = right

    void remove_wall(int wall)
    {
        switch (wall)
        {
        case 0:
            up = false;
            return;

        case 1:
            down = false;
            return;

        case 2:
            right = false;
            return;

        case 3:
            left = false;
            return;
        }
    } // 0 = north, 1 = south, 2 = east, 3 = west
    bool wall_exists(int wall)
    {
        switch (wall)
        {
        case 0:
            return up;

        case 1:
            return down;

        case 2:
            return right;

        case 3:
            return left;
        }
    }

    bool border_exists(int wall)
    {
        switch (wall)
        {
        case 0:
            return border_up;

        case 1:
            return border_down;

        case 2:
            return border_right;

        case 3:
            return border_left;
        }
    } // 0 = north, 1 = south, 2 = east, 3 = west
    void set_border(int wall)
    {
        switch (wall)
        {
        case 0:
            border_up = true;
            break;

        case 1:
            border_down = true;
            break;

        case 2:
            border_right = true;
            break;

        case 3:
            border_left = true;
            break;
        }
    }
    void display_walls()
    {
        if (up)
        {
            cout << " North ";
        }
        if (down)
        {
            cout << " South ";
        }
        if (right)
        {
            cout << " East ";
        }
        if (left)
        {
            cout << " West ";
        }
        cout << endl;
    }

    void set_index(int index)
    {
        index = index;
    }
    int get_index()
    {
        return index;
    }

private:
    int index;

    bool border_up;
    bool up;
    bool border_down;
    bool down;
    bool border_right;
    bool right;
    bool border_left;
    bool left;
};
class DisjointSet
{
public:
    vector<int> set;
    int elements_in_set;
    int sets;

    DisjointSet();
    explicit DisjointSet(int n)
    { // creates a vector of length n.
        // initializes each cell to contain the value -1.
        for (int i = 0; i < n; i++)
        {
            set.push_back(-1);
        }
        elements_in_set = n;
        sets = n;
    }

    int find(int x)
    {
        // Performs a find with path compression

        if (set[x] < 0)
            return x;
        else
            return set[x] = find(set[x]);
    }
    void union_sets(int root1, int root2)
    {
        // Union of two disjoint sets using the weighted union rule.
        int i = find(root1);
        int j = find(root2);

        if (set[j] < set[i])
        { //if j is deeper than i
            --set[i];
            set[j] = i;
        }
        else
        {
            --set[j];
            set[i] = j;
        }

        sets--; // A union will result in one less disjointed set
    }
    int return_sets()
    {
        return sets;
    }
};

int main()
{
    srand(time(NULL));
    int rows = 55;
    int collumns = 55;
    int total_cells = rows * collumns;
    DisjointSet cells(total_cells);

    Cell maze[rows][collumns];
    int index = 0;
    //init all cells in the maze
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < collumns; j++)
        {
            maze[i][j].set_index(index);
            index++;
        }
    }

    //set border walls
    for (int i = 0; i < collumns; i++)
    {
        maze[0][1].set_border(0);
    }
    for (int i = 0; i < collumns; i++)
    {
        maze[rows - 1][i].set_border(1);
    }
    for (int i = 0; i < rows; i++)
    {
        maze[i][collumns - 1].set_border(2);
    }
    for (int i = 0; i < rows; i++)
    {
        maze[i][0].set_border(3);
    }

    while (cells.return_sets() > 1)
    {
        int row = (rand() % rows);
        int collumn = (rand() % collumns);
        int wall = (rand() % 4);

        int random_wall = maze[row][collumn].get_index();

        switch (wall)
        {
        case 0:
            if (!maze[row][collumn].border_exists(0))
            {
                if (cells.find(random_wall) != cells.find(random_wall - collumns))
                {
                    cells.union_sets(random_wall, (random_wall - collumns));
                    maze[row][collumn].remove_wall(0);
                    maze[row - 1][collumn].remove_wall(1);
                }
            }
            break;
        case 1:
            if (!maze[row][collumn].border_exists(1))
            {
                if (cells.find(random_wall) != cells.find(random_wall + collumns))
                {
                    cells.union_sets(random_wall, (random_wall + collumns));
                    maze[row][collumn].remove_wall(1);
                    maze[row + 1][collumn].remove_wall(0);
                }
            }
            break;
        case 2:
            if (!maze[row][collumn].border_exists(2))
            {
                if (cells.find(random_wall) != cells.find(random_wall + 1))
                {
                    cells.union_sets(random_wall, (random_wall + 1));
                    maze[row][collumn].remove_wall(2);
                    maze[row][collumn + 1].remove_wall(3);
                }
            }
            break;
        case 3:
            if (!maze[row][collumn].border_exists(3))
            {
                if (cells.find(random_wall) != cells.find(random_wall - 1))
                {
                    cells.union_sets(random_wall, (random_wall - 1));
                    maze[row][collumn].remove_wall(3);
                    maze[row][collumn - 1].remove_wall(2);
                }
            }
            break;
        }
    }

    int start = rand() % rows;
    int end = rand() % collumns;
    maze[start][0].remove_wall(3);
    maze[end][collumns - 1].remove_wall(2);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < collumns; j++)
        {
            cout << "Cell " << setw(2) << right << maze[i][j].get_index() + 1 << ":";
            maze[i][j].display_walls();
        }
    }

    cout << "Done" << endl;
    return 0;
}