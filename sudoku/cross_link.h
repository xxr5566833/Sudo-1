class cross_link {
#ifndef NULL
#define NULL 0
#endif // !NULL
public:
    typedef struct cross_node {
        cross_node *left;
        cross_node *right;
        cross_node *up;
        cross_node *down;
        bool ishead;
        int count;
        int row;
        int col;
    } *Cross;
    Cross head;
    Cross *rows;
    Cross *cols;
    cross_link(int i, int j) {
        rownum = i + 1;
        colnum = j + 1;
        head = new cross_node;
        rows = new Cross[rownum];
        cols = new Cross[colnum];
        build();
    }
    ~cross_link();
    // Cross getHead() { return head; }
    void insert(int i, int j);
    void delrow(int r);
    void delcol(int c);
    void recoverrow(int r);
    void recovercol(int c);

private:

    int rownum;
    int colnum;

    void build();
};