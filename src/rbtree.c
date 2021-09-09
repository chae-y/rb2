#include "rbtree.h"

#include <malloc.h>

node_t nill = {RBTREE_BLACK, 5434, NULL, NULL, NULL};
node_t *nil = &nill;


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(sizeof(rbtree), 1);
  return p;
}

int left_rotation(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;
  if(y->left != NULL){
    y->left->parent = x;
  }
  y->parent = x->parent;//추가
  if(x->parent == NULL){//루트일 떄
    t->root = y;
  }else {
    if(x->parent->left == x){
      x->parent->left = y;
    }else{
      x->parent->right= y;
    }
  }
  y->left = x;
  x->parent = y;
  return 0;
}

int right_rotation(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right;
  if(y->right != NULL){//n
    y->right->parent = x;
  }
  y->parent = x->parent;//추가
  if(x->parent == NULL){//루트일 떄
    t->root = y;
  }else{
    if(x->parent->left == x){
      x->parent->left = y;
    }else{
      x->parent->right= y;
    }
  }
  y->right = x;
  x->parent = y;
  return 0;
}
void delete_node(node_t *n){
  if(n==NULL){
    return;
  }
  delete_node(n->right);
  delete_node(n->left);
  free(n);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t->root);
  free(t);
}

void RB_insert_fixup(rbtree *t, node_t *x){
  while(x != t->root && x->parent->color == RBTREE_RED){
    if(x->parent == x->parent->parent->left){
      node_t *uncle = x->parent->parent->right;
      if(uncle != NULL && uncle->color ==RBTREE_RED){
        x->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        if(x->parent != t->root){
          x->parent->parent->color = RBTREE_RED;
          x = x->parent->parent;
        }
      }
      else{
        if(x==x->parent->right){
          x->color = RBTREE_BLACK;
          x->parent->parent->color = RBTREE_RED;
          node_t *g = x->parent->parent;
          left_rotation(t, x->parent);
          right_rotation(t, g);
        }
        else{
          x->parent->color = RBTREE_BLACK;
          x->parent->parent->color = RBTREE_RED;
          right_rotation(t, x->parent->parent);
        }
      }
    }
    else{
      node_t *uncle = x->parent->parent->left;
      if(uncle != NULL && uncle->color == RBTREE_RED){
        x->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        if(x->parent != t->root){
          x->parent->parent->color = RBTREE_RED;
          x = x->parent->parent;
        }
      }
      else{
        if(x==x->parent->left){
          x->color = RBTREE_BLACK;
          x->parent->parent->color = RBTREE_RED;
          node_t *g = x->parent->parent;
          right_rotation(t, x->parent);
          left_rotation(t, g);
        }
        else{
          x->parent->color = RBTREE_BLACK;
          x->parent->parent->color = RBTREE_RED;
          left_rotation(t, x->parent->parent);
        }
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *temp = (node_t *)calloc(sizeof (node_t), 1);
  temp->color = RBTREE_RED;
  temp->key = key;
  // temp->left = Nil;//n
  // temp->right = Nil;//n

  // 1) 트리에 아무것도 없을 때
  if(t->root == NULL){
    t->root = temp;
    t->root->color = RBTREE_BLACK;
    return t->root;
  }
  node_t *n = t->root;
  while(n != NULL){
    if(key < n->key){
      if(n->left == NULL){
        n->left = temp;
        temp->parent = n;
        break;
      }
      n = n->left;
    }
    else{
      if(n->right == NULL){
        n->right = temp;
        temp->parent = n;
        break;
      }
      n = n->right;
    }
  }
  
  RB_insert_fixup(t, temp);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *n = t->root;
  while(1){
    if(n == NULL){
      return NULL;
    }
    else if(key == n->key){
      return n;
    }
    else if(key < n->key){
      n = n->left;
    }
    else{
      n = n->right;
    }
  }
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *n = t->root;
  while(1){
    if(n->left == NULL){
      return n;
    }
    n = n->left;
  }
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *n = t->root;
  while(1){
    if(n->right == NULL){
      return n;
    }
    n = n->right;
  }
  return t->root;
}

int RB_transplant(rbtree *t, node_t *u, node_t *v){
  // if(v==NULL){
  //   v = nil;
  // }//없으면 v가 루트가 되는거고 있으면지금처럼하면되는거고
  if(u->parent == NULL){
    t->root = v;
    v->parent = NULL;
    return 0;
  }
  if(u->parent->left){
    u->parent->left = v;
  }else{
    u->parent->right = v;
  }
  v->parent = u->parent;
  return 0;
}

int RB_erase_fixed(rbtree *t, node_t *x){
  while(x != t->root && x->color == RBTREE_BLACK){
    node_t *w;
    if(x == x->parent->left){
      w = x->parent->right;
      if(w->color == RBTREE_RED){//1번
        w->color  = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t,x->parent);
        w = x->parent->right;
      }
      node_t *w_left = w->left;
      node_t *w_right = w->right;
      if (w_left == NULL) {
        w_left = nil;
      }
      if (w_right == NULL) {
        w_right =nil;
      }

      if(w_left->color == RBTREE_BLACK && w_right->color == RBTREE_BLACK){//2번
        w->color = RBTREE_RED;
        x = x->parent;///?
      }
      else {
        if(w_right->color == RBTREE_BLACK){//3번
          w_left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation(t, w);
          w = x->parent->right;
        }
        //4번
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w_right->color = RBTREE_BLACK;
        left_rotation(t,x->parent);
        x = t->root;
      }
    }
    else{
      w = x->parent->left;
      if(w->color == RBTREE_RED){//1번
        w->color  = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }
      node_t *w_left = w->left;
      node_t *w_right = w->right;
      if (w_left == NULL) {
        w_left = nil;
      }
      if (w_right == NULL) {
        w_right =nil;
      }
      if(w_left->color == RBTREE_BLACK && w_right->color == RBTREE_BLACK){//2번
        w->color = RBTREE_RED;
        x = x->parent;///?
      }
      else {
        if(w_left->color == RBTREE_BLACK){//3번
          w_right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }
        //4번
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w_right->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
  return 0;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p; //삭제될 것
  node_t *x = p; //원래 위치
  color_t origin_col = p->color;
  // if(t->root == p && p->right == NULL && p->left == NULL){
  //   t->root = NULL;
  //   free(p);
  //   return 0;
  // }
  if(p->left == NULL){
    x = p->right;
    if (x == NULL) x = nil;
    RB_transplant(t, p, x);
  }else if(p->right == NULL){
    x = y->left;
    RB_transplant(t, p, p->left);
  }else{
    y = p->right;
    while(1){
      if(y->left == NULL){
        break;
      }
      y = y->left;
    }//최소값찾기
    origin_col = y->color;
    x = y->right;
    if (x == NULL) x = nil;
    if(y->parent == p){
      x->parent = y;//x가 null일때를 위해서 그러는듯
    }
    else{
      RB_transplant(t, y, x);
      y->right = p->right;
      y->right->parent = y;//y를 z자리에 올려놨음
    }
    RB_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }//이 윗부분은 그냥 바이너리트리의 삭제다.
  if(origin_col == RBTREE_BLACK){
    RB_erase_fixed(t,x);
  }
  if(t->root == nil){
    t->root = NULL;
  }
  else if(nil->parent != NULL){
    if(nil->parent->left == nil){
      nil->parent->left = NULL;
    }
    else{
      nil->parent->right = NULL;
    }
  }
  nil->parent = NULL;
  free(p);
  return 0;
}

key_t arr1[10000000];
int count = 0;
void dfs(node_t *n){
  if(n != NULL){
    dfs(n->left);
    arr1[count]=n->key;
    count++;
    dfs(n->right);
  }
  return;
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  count = 0;
  dfs(t->root);
  for(int i=0; i<n; i++){
    printf("%d\n",arr1[i]);
  }
  return 0;
}
