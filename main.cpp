#include <iostream>
#include <cstring>

struct node_t{
    node_t *left;
    node_t *right;
    node_t *parent;
};

struct binary_tree_t{
    node_t *root;
    size_t dataSize;
    size_t length;
    int (*compare)(void *, void *);
};

void TreeInit( binary_tree_t **tree, size_t dataSize, int (*compare)(void *, void *) ){
    (*tree) = static_cast<binary_tree_t*>(malloc( sizeof( binary_tree_t ) ) ) ;
    (*tree)->root = NULL;
    (*tree)->dataSize = dataSize;
    (*tree)->length = 0;
    (*tree)->compare = compare;
}

node_t *newNode( binary_tree_t **tree, void *data ){
    node_t *node = static_cast<node_t*>( malloc( sizeof(node_t) + (*tree)->dataSize ));
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    memcpy( reinterpret_cast<void*>(node + 1), data, (*tree)->dataSize );
    return node;
}



void TreeAdd( binary_tree_t **tree, void *data ){
    node_t *node = newNode( tree, data );

    if( (*tree)->root == NULL ){
        (*tree)->root = node;
    }else{

        node_t *currentNode = (*tree)->root;
        while(1){
            int cmp = (*tree)->compare( reinterpret_cast<void*>(currentNode+1), data );

            if( cmp > 0 ){

                if( currentNode->right != NULL ){
                    currentNode = currentNode->right;
                    continue;
                }else{
                    currentNode->right = node;
                    node->parent = currentNode;
                    break;
                }

            }else if( cmp < 0 ){

                if( currentNode->left != NULL ){
                    currentNode = currentNode->left;
                    continue;
                }else{
                    currentNode->left = node;
                    node->parent = currentNode;
                    break;
                }
            }else{
                return;
            }
        }
    }
    (*tree)->length++;
}

node_t* TreeFind (binary_tree_t **tree, void *data){
    node_t *currentNode = (*tree)->root;
    while(1){
        int cmp = (*tree)->compare( reinterpret_cast<void*>(currentNode+1), data );

        if( cmp > 0 ){
            if( currentNode->right != NULL ){
                currentNode = currentNode->right;
                continue;
            }else{
                return NULL;
            }

        }else if( cmp < 0 ){
            if( currentNode->left != NULL ){
                currentNode = currentNode->left;
                continue;
            }else{
                return NULL;
            }
        }else{
            return currentNode;
        }
    }
}

node_t* MaxOfLeft (binary_tree_t **tree) {
    //std::cout<<"zdecya ";
    node_t *currentNode = (*tree)->root->left;
    if (currentNode == NULL) {
        return NULL;
    } else {
        while (1) {
            if (currentNode->right != NULL) {
                currentNode = currentNode->right;
                continue;
            }
            return currentNode;
        }
    }
}

node_t* MaxOfLeft (node_t* root){
    if (root->left == NULL) {
        return NULL;
    }
    else{
        node_t *currentNode = root->left;
        while (1) {
            if (currentNode->right != NULL) {
                currentNode = currentNode->right;
                continue;
            }
            return currentNode;
        }
    }
}

node_t* MinOfRight (binary_tree_t **tree) {
    node_t *currentNode = (*tree)->root->right;
    if (currentNode == NULL) {
        return NULL;
    } else {
        while (1) {
            if (currentNode->left != NULL) {
                currentNode = currentNode->left;
                continue;
            }
            return currentNode;
        }
    }
}

node_t* MinOfRight (node_t* root){
    if (root->right == NULL) {
        return NULL;
    }
    else{
        node_t *currentNode = root->right;
        while (1) {
            if (currentNode->left != NULL) {
                currentNode = currentNode->left;
                continue;
            }
            return currentNode;
        }
    }
}

bool TreeNodeDelete( binary_tree_t **tree, void *data) {
    node_t *node = TreeFind(tree, data);
    if (node == NULL) {      // ell yok
        return false;
    }
    else if (node->right == NULL && node->left == NULL) { // list
        std::cout<<"list"<<std::endl;
        if (node->parent == NULL){                      // list - root
            std::cout<<"list & root"<<std::endl;
            (*tree)->root = NULL;
            delete node;
            return true;
        }
        else if (node->parent->right == node) {
            node->parent->right = NULL;
            delete node;
            return true;
        }
        else if (node->parent->left == node) {
            node->parent->left = NULL;
            delete node;
            return true;
        }

    }
    else if( node->right == NULL){               // node with only left subtree
        std::cout<<"right free"<<std::endl;
        if (node->parent == NULL){
            node->left->parent = NULL;
            (*tree)->root = node->left;
            delete node;
            return true;

        }
        else if (node->parent->right == node){
            node->parent->right = node->left;
            node->left->parent = node->parent;
            delete node;
            return true;
        }
        else if (node->parent->left == node){
            node->parent->left = node -> left;
            node->left->parent = node->parent;
            delete node;
            return true;

        }
    }
    else if ( node->left == NULL ){             // node with only right subtree
        std::cout<<"left free"<<std::endl;
        if (node->parent == NULL){
            node->right->parent = NULL;
            (*tree)->root = node->right;
            delete node;
            return true;

        }
        if (node->parent->left == node){
            node->parent->left = node->right;
            node->right->parent=node->parent;
            delete node;
            return true;
        }
        else if(node->parent->right == node){
            node->parent->right = node -> right;
            node->right->parent = node->parent;
            delete node;
            return true;
        }
    }

    else if (node->left!=NULL && node->right!=NULL){
        std::cout<<"right & left"<<std::endl;
        if (node->parent != NULL) {
            std::cout << "node->parent: " << *reinterpret_cast<int *>( node->parent + 1 ) << std::endl;
        }
        if (node->parent==NULL){                        // root
            std::cout<<"root"<<std::endl;
            node_t* new_node = MaxOfLeft(tree);    // ell for switch
            std::cout<<" new: "<<*reinterpret_cast<int*>( new_node + 1 )<<std::endl;
            if (node->left == new_node){  // ell for switch —  root->left
                std::cout<<"new is root->left"<<std::endl;
                new_node->right = node->right;
                node->right->parent = new_node;
                new_node->parent = node->parent;
                new_node->left = node->left->left;
                (*tree)->root = new_node;

                delete node;
                return true;


            }
            else if ( new_node->left == NULL && new_node->right == NULL ){  // new is list
                std::cout<<"new is list"<<std::endl;
                new_node->left = node->left;
                new_node->right = node->right;

                node->left->parent = new_node;
                node->right->parent = new_node;

                new_node->parent->right = NULL;
                new_node->parent = NULL;
                (*tree)->root = new_node;
                delete node;
                return true;
            }
            else if (new_node->left != NULL){                      // not a list
                std::cout<<"not a list"<<std::endl;

                new_node->parent->right = new_node->left;
                new_node->left->parent = new_node->parent;

                new_node->left = node->left;
                new_node->right = node->right;

                new_node->parent = NULL;

                node->right->parent = new_node;
                node->left->parent = new_node;

                (*tree)->root = new_node;
                delete node;
                return true;
            }
        }
        else if (node->parent->left == node) {     // left subtree
                std::cout << "left subtree!" << std::endl;

                node_t *new_node = MaxOfLeft(node);
                if (node->left == new_node) {  // ell for switch —  node->left
                    std::cout << "new is node->left" << std::endl;
                    new_node->right = node->right;
                    node->right->parent = new_node;
                    new_node->parent = node->parent;
                    node->parent->left = new_node;

                    delete node;
                    return true;
                }
                else if (new_node->left == NULL && new_node->right == NULL) {  // new is list
                    std::cout << "new is list" << std::endl;
                    new_node->left = node->left;
                    new_node->right = node->right;
                    new_node->parent->right = NULL;
                    new_node->parent = node->parent;
                    node->parent->left = new_node;
                    node->left->parent = new_node;
                    node->right->parent = new_node;
                    delete node;
                    return true;
                }
                else if (new_node->left != NULL){                      // not a list
                    std::cout << "not a list" << std::endl;

                    new_node->parent->right = new_node->left;
                    new_node->left->parent = new_node->parent;

                    new_node->left = node->left;
                    new_node->right = node->right;

                    node->right->parent = new_node;
                    node->left->parent = new_node;

                    node->parent->left = new_node;
                    new_node->parent = node->parent;


                    delete node;
                    return true;
                }


            }
        else if (node->parent->right == node) {     // right subtree
            std::cout << "right subtree!" << std::endl;

            node_t *new_node = MinOfRight(node);
            if (node->right == new_node) {  // ell for switch —  node->right
                std::cout << "new is node->right" << std::endl;
                new_node->left = node->left;
                node->left->parent = new_node;
                new_node->parent = node->parent;
                node->parent->right = new_node;

                delete node;
                return true;
            }
            else if (new_node->left == NULL && new_node->right == NULL) {  // new is list
                std::cout << "new is list" << std::endl;
                new_node->left = node->left;
                new_node->right = node->right;
                new_node->parent->left = NULL;
                new_node->parent = node->parent;
                node->parent->right = new_node;
                node->left->parent = new_node;
                node->right->parent = new_node;
                delete node;
                return true;
            }

            else if (new_node->right != NULL){                      // not a list
                std::cout << "not a list" << std::endl;

                new_node->parent->left = new_node->right;
                new_node->right->parent = new_node->parent;

                new_node->left = node->left;
                new_node->right = node->right;

                node->right->parent = new_node;
                node->left->parent = new_node;

                node->parent->right = new_node;
                new_node->parent = node->parent;


                delete node;
                return true;
            }


        }
        }

    return true;
}

void integerPreOrder( node_t *root ){
    if( root == NULL ) {
        //std::cout<<'\n';
        return;
    }

    std::cout << *reinterpret_cast<int*>( root + 1 ) <<' ';
    integerPreOrder( root->left );
    integerPreOrder( root->right );
}


int integerCompare( void* first, void* second ){
    return *static_cast<int*>(second) - *static_cast<int*>(first);
}



int main() {
    binary_tree_t *bin_tree;
    int value;
    TreeInit(&bin_tree, sizeof(int), integerCompare);

        int values[] = {14, 22, 3, 18, 9, 15, 39, 25, 36, 12, 31, 30, 33, 38, 7, 10, 17, 26, 6, 24
        };
    int values_to_delete[] = {22, 7, 17, 24, 10, 15, 38, 18, 36, 6, 31, 14, 9, 25, 26, 30, 3, 12, 33, 39
    };


    for (int i: values) {
        TreeAdd(&bin_tree, &i);

    }

    integerPreOrder(bin_tree->root);
    std::cout << '\n';



    for (int i: values_to_delete) {
        std::cout<<"value: "<<i<<std::endl;
        if(TreeNodeDelete(&bin_tree, &i)){
            integerPreOrder(bin_tree->root);
            std::cout << '\n';
            if (bin_tree->root != NULL) {
                std::cout << "root: " << *reinterpret_cast<int *>( bin_tree->root + 1 ) << std::endl;
            }
        }
        else{
            std::cout<<"no such element in tree"<<std::endl;

        }
    }
    if (bin_tree->root != NULL) {
        std::cout << "root: " << *reinterpret_cast<int *>( bin_tree->root + 1 ) << std::endl;
    }
    else{
        std::cout<<"bin_tree is free"<<std::endl;
    }
}