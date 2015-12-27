#pragma once

#include "FHvector.h"
#include "FHlist.h"





//MatNode is used to store any type of variable in the member variable data
//It also stores column number in int col. 
//Its use is for the SparseMat
template <class Object>
class MatNode
{
protected:
    int col;

public:
    Object data;
    // we need a default constructor for lists
    MatNode(int cl = 0, Object dt = Object()) : col(cl), data(dt) {}
    int getCol() const { return col; }

    const Object & operator=(const Object &x) { return (data = x); }
};






//Sparse matrix implementation using FHvector<FHlist<MatNode<Object>>>
//The vector is considered the rows and the list stores nodes holding 
//corresponding column numbers
//The class functions protect from bad client input via exceptions and true/false returns
template <class Object>
class SparseMat
{
protected:
    typedef FHlist< MatNode<Object> > MatRow;
    typedef FHvector<MatRow> MasterCol;

    MasterCol rows;
    Object defaultVal;
    int rowSize, colSize;

public:
    SparseMat(int r, int c, const Object & defaultVal);
    const Object & get(int r, int c) const;
    bool set(int r, int c, const Object &x);
    void clear();
    void showSubSquare(int start, int size);

    //throws error if bounds are violated
    class ArrayBoundException {};
};







//Precondition: r and c must be greater than 0.
//              defaultVal must have a default constructor.
//Postcondition: rowSize, colSize, and the defaultVal are set.
//               The vector allocates empty sublists of the rowSize aka r.

template <class Object>
SparseMat<Object>::SparseMat(int r, int c, const Object & defaultVal)
{
    //sanity check, set defaults if bad values are passed
    if (r < 1 || c < 1)
        rowSize = colSize = 1;
    else
    {
        rowSize = r;
        colSize = c;
    }

    //set defaultVal and create the vector with a set size
    this->defaultVal = defaultVal;
    rows = MasterCol(rowSize);
}







//Precondition: r and c must be 0 or greater.
//Postcondition: Returns the value at that (r, c) in the sparce matrix
//               If it doesn't exist a default value is returned.

template <class Object>
const Object & SparseMat<Object>::get(int r, int c) const
{
    //sanity check, throw user defined exception if breached
    if (r < 0 || c < 0 || r >= rowSize || c >= colSize)
        throw ArrayBoundException();


    MatRow::const_iterator rIter;

    //iterator through the FHsublist of the specific row r, then check column for match

    for (rIter = rows[r].begin(); rIter != rows[r].end(); ++rIter)
        if ((*rIter).getCol() == c)
            return (*rIter).data;

    //return the sparse matrix's default value if the client's 
    //node is not found
    return defaultVal;
}






//Precondition: r, c must be valid rows/columns.
//              defaultVal must have a default constructor.
//              defaultVal must have comparison operators overloaded
//Postcondition: Creates a MatNode at that (r, c) in the sparse matrix.
//               If the MatNode already exists, it is reallocated.
//        If x equals the default value, erase the MatNode at that location.
//        (NOTE: insert is done in order)

template <class Object>
bool SparseMat<Object>::set(int r, int c, const Object &x)
{

    //sanity check, return false to client if breached
    if (r < 0 || c < 0 || r >= rowSize || c >= colSize)
        return false;


    MatRow::iterator rIter;

    //check beforehand if the sublist is empty, save time/resources
    if (rows[r].size() == 0)
    {
        rows[r].push_back(MatNode<Object>(c, x));
        return true;
    }

    //This is to find if a pre-existing node of the same column exists!
    //iterator through the FHsublist of the specific row r
    for (rIter = rows[r].begin(); rIter != rows[r].end(); ++rIter)
    {
        //check if the column matches
        if ((*rIter).getCol() == c)
        {
            //defaultVal? Erase the node.
            if (x == defaultVal)
            {
                rows[r].erase(rIter);
                return false;
            }
            //otherwise set the data variable and return true
            (*rIter).data = x;
            return true;
        }
    }

    //If a pre-existing node in that (row,column) is not found, we create a new one
    //This loop ensures we insert it into the right position
    for (rIter = rows[r].begin(); rIter != rows[r].end(); ++rIter)
        if ((*rIter).getCol() > c)
            break;

    rows[r].insert(rIter, MatNode<Object>(c, x));

    return true;
}






//Postcondition: Every sublist is cleared of all nodes.
//               Important: The vector holding all sublists is kept(not cleared),
//                          this is to keep the SparseMat usable if more nodes
//                          want to be added at a later time!
template <class Object>
void SparseMat<Object>::clear()
{
    //goes through every sublist and calls .clear() from FHlist class
    for (int k = 0; k < rowSize; k++)
        rows[k].clear();
}







//Precondition: start and size must be valid bounds of the sparse matrix
//Postcondition: Print out that portion of the sparse matrix in a grid format
template <class Object>
void SparseMat<Object>::showSubSquare(int start, int size)
{
    int end = start + size - 1;
    int k, j;

    //makes sure the parameters passed are within range
    //could replace with exception in future depending on client needs
    if (end >= rowSize || end >= colSize)
    {
        cout << "showSubSquare has been passed invalid parameters\n";
        return;
    }

    //iterate through vector indices
    for (k = start; k <= end; k++)
    {
        //iterate through list of nodes
        //use get() to print out the node
        for (j = start; j <= end; j++)
            cout << this->get(k, j) << " ";
        cout << endl;
    }
    cout << endl;
}





