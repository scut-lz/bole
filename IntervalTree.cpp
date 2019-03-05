#include<iostream>
#include<vector>
#define maxn 10000
using namespace std;

struct io_ctx{
    off_t offset;
    size_t len;
    uint8_t op;
    void *data;
}ioctx[maxn];


struct Interval{
    off_t low;
    size_t high;
};

struct node{
    Interval *i;
    size_t max;
    node *left,*right;
};

node *newNode(Interval i){
    node *tmp = new node();
    //Interval *ii = NULL;
    //ii -> low = i.offset;
    //ii -> high = i.offset + i.len;
    tmp -> i = new Interval(i);
    tmp -> max = i.high;
    tmp -> left = tmp ->right = NULL;
    //delete ii;
};

node *insert(node *root, Interval i){
    if(root == NULL){
        return newNode(i);
    }

    long int l = root -> i -> low;

    if(i.low < l)
        root -> left = insert(root -> left,i);
    else
        root -> right = insert(root -> right,i);

    if(root -> max < i.high)
        root -> max = i.high;

    return root;
}

bool overlap(Interval i1,Interval i2){
    if(i1.low > i2.high || i2.low >i1.high)//逆向判断
        return false;
    return true;
}


bool overlapSearch(node *root,Interval i){
    if(root == NULL)
        return false;
    if(overlap(*(root -> i),i))
        return true;

    if(root -> left != NULL && root -> left -> max >= i.low){
        return overlapSearch(root -> left,i);

    }

    return overlapSearch(root -> right, i);

}


int main(){
    int ints[6][2] = {{15,20},{10,30},{17,19},{5,20},{12,15},{30,40}};
    int num = sizeof(ints)/sizeof(ints[0]);
    for(int i = 0; i < num;i++){
        ioctx[i].offset = (long int)ints[i][0];
        ioctx[i].len = (long int)ints[i][1];
    }

    io_ctx *iop = new io_ctx;
    iop -> offset = 1;
    iop -> len = 15;
    node *root = NULL;
    for(int i = 0; i < num;i++){
        Interval t = {ioctx[i].offset,ioctx[i].offset+ioctx[i].len};
        root = insert(root,t);
        //delete t;
    }

    Interval target = {iop -> offset,iop -> len + iop ->offset};

    bool result = overlapSearch(root,target);
    if(!result)
        cout<<"False";
    else
        cout<<"True";
    return 0;

}

