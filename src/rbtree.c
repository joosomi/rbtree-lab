#include "rbtree.h"

#include <stdlib.h>
  // TODO: initialize struct if needed
  //초기화
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p -> nil = (node_t *)calloc(1, sizeof(node_t));
  p->root = p->nil;
  p->nil->color = RBTREE_BLACK;

  return p;
}

/////////////////////
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

//RB Tree Left Rotate
void left_rotate(rbtree *t, node_t *x) {
  node_t *y;
  y = x->right; //y는 x의 오른쪽 서브트리 

  x->right = y->left; //y 왼쪽 서브트리를 x의 오른쪽 서브트리로 옮긴다. 

  //y의 왼쪽 자식이 있다면
  if (y->left != t->nil) {
    y->left->parent  = x; //y의 왼쪽 자식의 부모 = x => 서로 연결해준다.  
  }
  y->parent = x->parent; // x의 부모를 y로 연결한다.  x-y의 연결을 끊는다. 

  if (x->parent == t->nil) {
    t->root = y; 
    // x의 부모가 nil이면 (Root node이면)
    // Rb tree의 루트 노드를 y로 설정
  }
  else if( x== x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  // y의 왼쪽  자식 = x 
  // x의 부모 = y 
  // 서로 연결시킨다.
  y->left = x; 
  x->parent = y;
} 

//RB Tree Right Rotate
void right_rotate(rbtree *t, node_t *x) {
  node_t *y;
  y = x->left; // y는 x의 왼쪽 서브트리 
  x->left = y->right; // y의 오른쪽 자식을 x의 왼쪽 자식으로
  //y의 오른쪽 자식이 있다면 
  if (y->right != t->nil) {
    y->right->parent = x;  //y의 오른쪽 자식의 부모 = x 
  }
  y->parent = x->parent ; // x의 부모를 y의 부모로 연결 

  if (x->parent== t->nil) {
    t->root = y; // x가 루트 노드였다면, rb tree의 루트노드를 y로 
  }
  else if (x==x->parent->right) {
    x->parent->right = y; 
  } 
  else {
    x->parent->left = y;
  }
  //y의 오른쪽 자식을 x로,
  //x의 부모 노드를 y로, 서로 연결시켜준다. 
  y->right = x;
  x->parent = y;
}


//INSERT-FIXUP
// z: 새로 삽입될 노드
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  node_t *y;

  while (z->parent->color == RBTREE_RED) {
    // 삽입하려는 노드의 부모 노드가 조부모의 왼쪽 자식일 때 
      if (z->parent == z->parent->parent->left){
          y = z->parent->parent ->right;
          // y=> 삼촌 노드! 부모의 형제 노드 
          // y가 red => case 1, y 가 Black => case2, 3 으로 풀이한다.

          if (y->color == RBTREE_RED) {
            //CASE 1:
            //새로 삽입할 노드의 부모 색을 Black, 
            //삼촌 노드의 색을 Black,
            //조부모의 색을 Red로 변경하고 조부모에서 다시 시작
            z->parent->color = RBTREE_BLACK;
            y->color = RBTREE_BLACK;
            z->parent->parent->color = RBTREE_RED;
            z = z->parent->parent; 
          }
          else {
            //CASE 2 
            // 새로 삽입할 노드가 오른쪽 자식이면
            // 부모 노드를 기준으로 왼쪾 회전 하고 CASE 3 방식으로 해결
            if (z == z->parent->right){
              z = z->parent;
              left_rotate(t, z);
            }
            //CASE 3
            //새로 삽입할 노드가 왼쪽 자식이면
            //부모 노드의 색을 black으로,
            //조부모 노드의 색을 red로 변경하고
            //오른쪽 회전하여 double red를 해결한다. 
            z->parent->color = RBTREE_BLACK;
            z->parent->parent->color = RBTREE_RED;
            right_rotate(t, z->parent->parent);
          }
      }
      // (z-> parent == z->parent->parent->right)
      // 삽입하려는 노드의 부모 노드가 조부모의 오른쪽 자식일 때 
      else {
        //삼촌 노드 y
        y = z->parent->parent->left;
        //CASE 4. 삼촌 노드가 RED일 때
        if (y->color == RBTREE_RED) {
          z->parent->color = RBTREE_BLACK;
          y->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        }
        else {
            //CASE 5. 
            if (z== z->parent->left) {
              z = z->parent;
              right_rotate(t, z);
            }
            //CASE6.
            z->parent->color = RBTREE_BLACK;
            z->parent->parent->color = RBTREE_RED;
            left_rotate(t, z->parent->parent);
        }
     }
  }
  //함수 종료 전에 루트 노드의 색깔을 Black으로 바꿔주기
  t->root->color = RBTREE_BLACK;
}

//RB tree INSERT
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y; 
  node_t *x;
  
  y = t->nil;
  x = t->root;

  // node_t *z;
  // z: 새로 삽입하는 노드 
  node_t *z = (node_t*)calloc(1, sizeof(node_t));
  //동적 메모리 할당-> 노드의 키 값들도 초기화 해준다.
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

//  새로운 노드를 삽입할 위치 탐색
  while (x != t-> nil) {
    y = x;

    //새로 삽입하려는 노드의 키 값이 현재 노드의 키 값보다 더 작다면 Left 이동
    if ( key < x->key) {
      x = x->left;
    }
    //새로 삽입하려는 노드의 키 값이 현재 노드의 키 값보다 더 크다면 Right 이동
    else {
      x = x->right;
    }
  }
  // x 는 다음 레벨의 자식 노드를 가리키고, y는 x의 부모 노드를 가리키게 됨. 

  //y: 새로 삽입될 노드의 부모 노드가 됨
  z->parent = y;
  
  //부모 노드가 NIL(빈 트리)라면 루트 노드는 새로 삽입하는 노드가 된다.
  if (y == t->nil) {
    t->root = z;
  }
  // 새로 삽입하려는 노드의 키 값이 부모 노드의 키 값보다 작으면 
  // 새로 삽입하려는 노드는 왼쪽 자식이 된다.
  else if (key < y->key) {
    y->left = z;
  }
  // 크다면
  //새로 삽입하려는 노드는 오른쪽 자식이 된다.
  else {
    y->right = z;
  }

  //새로 삽입되는 노드 z의 left, right 은 NIL노드, 색깔은 항상 RED
  z->left = t-> nil;
  z->right = t->nil; 
  z->color = RBTREE_RED; //삽입하는 노드의 색은 항상 RED로 고정한다.
  //삽입할 때 레드 블랙 트리의 속성을 깨지지 않게 하기 위해 rbtree_isnert_fixup
  rbtree_insert_fixup(t, z); 

  return t->root;
}

/////////////////////////////////////////////////////////
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *target;
  target = t->root;

   while(target != t->nil){
    if(target->key == key){
      return target;
    }
    else if(target->key < key){
      target = target ->right;
    }
    else {
      target = target->left;
    }
   }
  return NULL;
}

/////////////////////////////////////////////////////////
/// tree-minimum
// 트리에서 가장 작은값을 가지는 노드 Return - successor 구할 때 사용
node_t *tree_minimum(rbtree *t, node_t *sub_root) {
  node_t *r;
  r = sub_root;
  //빈 트리라면
  if (r == t->nil) {
    return r;
  }

  while  (r->left  != t->nil) {
     r = r->left;
  }
  return r; 
}

/// tree-maximum
node_t *tree_maximum(rbtree *t, node_t *sub_root) {
  node_t *r;
  r = sub_root;
  //빈 트리라면
  if (r== t->nil) {
    return r;
  }

  while (r->right != t->nil) {
      r = r->right;
  }
  return r;
}

//전체 트리에서 최소값을 가지는 노드
node_t *rbtree_min(const rbtree *t) {
  node_t *p;
  p = t->root;

  while(p->left != t->nil){
    p = p->left;
  }
  return p;
}

//전체 트리에서 최댓값을 가지는 노드
node_t *rbtree_max(const rbtree *t) {
  node_t *p;
  p = t->root;

  while(p->right != t->nil){
    p = p->right;
  }
  return p;
}

/////////////////////////////////////////////////////////
///////////////RB TREE 삭제 연산///////
/////////////////////////////////////////////////////////

//RB Tree Transplant
// rb tree에 이식할 노드 u : 삭제할 노드 v: 대체할 노드 
void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  //삭제할 노드 u가 루트 노드라면
  //v가 루트노드를 대체해야 한다. 
  if (u->parent == t->nil) {
    t->root = v;
  }
  //삭제할 노드 u가 부모 노드의 왼쪽 자식이라면
  //v는 왼쪽을 대체해야 한다.
  //v와 u->parent->left 연결
  else if (u == u->parent->left) {
    u->parent->left = v;
  } 
  else {
    //삭제할 노드가 오른쪽 자식이라면
    //v는 오른쪽을 대체해야 한다. v와 u의 자리를 연결시킨다.
    u->parent->right = v; 
  }
  //대체할 노드의 Parent => 삭제한 노드의 parent 연결
  v ->parent = u->parent ;
}



//RB tree-delete-fixup(t,x)
//x: 삭제된 z를 대체하는 노드 x
void rbtree_delete_fixup(rbtree *t, node_t *x) {
    
  while (x != t->root && x->color == RBTREE_BLACK){

    //왼쪽에 Dbouly black이 생긴  상황
    if (x== x->parent->left){
      node_t *uncle = x->parent->right;
      //CASE 1 => doubly node의 형제가 red 빨간색
      //삼촌의 색을 black, 부모의 색을 Red로한 후 왼쪽 회전하고
      //이제 Uncle은 새로운 노드가 될테니 다시 선언
      if (uncle->color == RBTREE_RED){
        uncle ->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        uncle = x->parent->right;
      }
      ////doubly node의 형제가 검은색인 경우
      //CASE 2 : 삼촌의 자식 노드 둘다 검은색이면
      //삼촌의 색을 Red로 변경하고 x 부모 노드에게 Doubly black을 넘기니까 
      if (uncle->left->color == RBTREE_BLACK && uncle->right->color == RBTREE_BLACK){
        uncle->color = RBTREE_RED;
        x = x->parent;
      }
      else{
      // CASE3: 삼촌의 오른쪽 자식이 black, 삼촌의 왼쪽 자식이 red
        //삼촌의 오른쪽 자식이 black인 경우
        //
        //삼촌의 왼쪽 자식의 색깔을 black, uncle 색을 Red로 변경 후 오른쪽 회전
        //하면 doubly black의 Uncle 새로 선언
        if (uncle->right->color == RBTREE_BLACK) {
          uncle->left->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          right_rotate(t, uncle);
          uncle = x->parent->right;
        }
        //CASE 4: 삼촌의 오른쪽 자식의 색이 red, 왼쪽 자식의 색이 black,
        // 삼촌의 색을 부모의 색으로 바꾸고
        //부모와 삼촌의 오른쪽 자식의 색을 black으로 바꾼다. 
        //그리고 부모 기준으로 left rotate
        uncle->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        uncle->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x= t->root;
      }
    }
    //대칭된 경우
    // x == x->parent->right
    // doubly black이 오른쪽에 있는 경우
    else {
      node_t *uncle = x->parent->left;
      //CASE1 
      //삼촌의 색이 RED인 경우
      if (uncle->color == RBTREE_RED){
        uncle->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        uncle = x->parent->left;
      }
      //CASE 2
      //삼촌의 두 자식이 모두 BLACK인 경우
      if (uncle->left->color == RBTREE_BLACK && uncle->right->color == RBTREE_BLACK){
        uncle->color = RBTREE_RED;
        x = x->parent;
      }
      else {
      //CASE 3
      //  삼촌의 왼쪽 자식이 Black이고 
        if (uncle->left->color == RBTREE_BLACK){
          uncle->right->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          left_rotate(t, uncle);
          uncle = x->parent->left;
        }
        //CASE 4
        uncle->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        uncle->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x= t->root;
      }
    }
  }
  //루트 노드의 색은 black으로 변경
  x->color = RBTREE_BLACK;
}


//RB tree-delete
//z: 삭제하려는 노드
//노드 y: 트리에서 삭제된 노드 또는 트리에서 이동한 노드

//노드 y: 
//노드 x: y가 제거되거나 이동하기 전의 색깔을 저장하며 y의 원래 위치로 이동하는 노드 x
//x -> 더블리 노드가 생기는 위치
int rbtree_erase(rbtree *t, node_t *z){
  node_t *x;
  node_t *y;
  //z: 삭제하려는 노드가 1개 이하의 자식을 갖는 경우
  // 자식이 없을 때
  //삭제하려는 노드를 우선 y에 저장
  y = z;

  color_t y_original_color;
  y_original_color = y ->color;
  //오른쪽 자식만 있을 때
  if (z->left == t->nil){
    x = z->right; //오른쪽 자식을 x에 담아둔다.
    //z의 오른쪽 자식을 z의 위치에 Transplant
    rb_transplant(t, z, z->right);
  }
  //왼쪽 자식만 있을 때
  else if (z->right == t->nil) {
    x= z->left; // 왼쪽 자식을 x에 담아둔다.
    // z의 왼쪽 자식을 z의 위치에 Transplant
    rb_transplant(t, z, z->left);
  }

  //z : 삭제하려는 노드가 2개 이상의 자식을 갖고 있을 경우
  //z의 Successor가 삭제된 자리를 대체한다.
  // y는 z->right의 successor로 설정한다.
  else {
    y = tree_minimum(t, z->right);
    y_original_color = y->color; // 실제로 삭제되는 successor의 색 저장
    
    //y는 successor니까 y->left는 어짜피 NIL 노드
    // y의 오른쪽 자식을 x에 저장 
    // y자리의 대체자 x -> extra black 부여하게 되는 노드
    x = y->right; 

    //y의 부모노드가 z라면 (edge가 1개)
    //x의 부모 노드를 y로 연결 
    // Successor의 오른쪽 자식의 부모를 y로 
    if (y->parent == z){
      x->parent = y;
    } 

    //y의 부모노드가 z가 아니라면
    //y위치에  y->right 이식 
    //서로 연결 시켜줌
    //후계자 노드의 오른쪽 서브트리의 부모를 후계자 노드로 설정
    else {
      rb_transplant(t, y, y->right); 
      y->right = z->right;
      y->right->parent = y;
      //y의 자식과 그 자식의 부모를 y로 향하게 설정
    }

  //z위치에 y 이식 
  //z의 기존 왼쪽 서브트리와 y 연결하고
  //y의 색은 기존의 z노드의 색깔으로 (y의 색깔이 삭제됨 - y_original_color를 저장하는 이유임)
  // y는 z의 속성을 물려받게 된다. 
    rb_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  //삭제되는 색이 BLACk이면 rbtree_delete_fixup 
  if (y_original_color == RBTREE_BLACK){
    rbtree_delete_fixup(t, x);
    //대체하는 노드 y의 자식인 x가 y의 자리로 올라오면서 
    //이 x에 extra black 부여
  }
  return 0;
}
/////////////////////////////////////////////////////////////////////////////
// int make_array(node_t *node, key_t *arr, int i ) {
//   if (node == NULL) {
//     return i;
//   }

//   arr[i] = node->key;
//   i++;

//   if (node->left != NULL) {
//     i = make_array(node->left, arr, i);
//   }


// }

//중위순회하면서 트리의 노드들을 배열에 저장하는 함수 
//왼쪽 서브트리 -> 루트 노드 -> 오른쪽 서브트리
void inorder(const rbtree *t, node_t *node, key_t *arr, int *i, size_t n){ 
  //현재 노드가 t->nil이면 
  if (node == t->nil) {
    return ;
  }
  //왼쪽 서브트리 순회
  inorder(t, node ->left, arr, i, n);
  if (*i < n){
      arr[(*i)++] = node ->key;
  }
  //오른쪽 서브트리 순회
  inorder(t, node->right, arr, i, n );
  //업데이트 된 인덱스 반환
}

//RBTREE to ARRAY
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root == t->nil) {
    return -1 ;
  }
  //배열의 인덱스 관리 
  int idx = 0;

  //루트노드부터 중위 순회
  inorder(t, t->root, arr, &idx, n );
  return 0;
}




// // 트리를 출력하는 함수
// void print_rbtree(rbtree *t, node_t *node, int space)
// {
//   if (node == t->nil)
//     return;

//   space += 10;
//   print_rbtree(t, node->right, space);

//   printf("\n");
//   for (int i = 10; i < space; i++)
//     printf(" ");
//   printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

//   print_rbtree(t, node->left, space);
// }

// int main()
// {
//   rbtree *t = new_rbtree(); // 레드-블랙 트리 생성 함수
//   int key;

//   printf("노드를 삽입하려면 키 값을 입력하세요 (음수를 입력하면 종료):\n");
//   while (scanf("%d", &key) && key >= 0)
//   {
//     rbtree_insert(t, key);
//     print_rbtree(t, t->root, 0);
//   }

//   // 트리 메모리 해제
//   delete_rbtree(t);

//   return 0;
// }