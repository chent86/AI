#include <iostream>
#include <vector>
#include "limits.h"
using namespace std;

void print(string s) {
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      cout << s[i*3+j] << " ";
    }
    cout << endl;
  }
  cout << "---------" << endl;
}

void input(string& cur) {
  int a = -1;
  cin >> a; 
  while(a < 0 || a > 8) {
    cout << "Please enter 0-8" << endl;
    cin >> a;    
  }
  if(cur[a] == ' ')
    cur[a] = 'X';
}

struct node{
  bool type; // true:极大层  false:极小层
  bool update;
  int a;
  int b;
  int height;
  string val;
  node* parent;
  vector<node*> children;
  int choose;
  node(string val, bool type, int height, node* parent) {
    this->update = false;
    this->val = val;
    this->type = type;
    this->height = height;
    this->parent = parent;
  }
};

int three_num(string& s, char c) {
  int count = 0;
  if(s[0] == c && s[1] == c && s[2] == c)
    count++;
  if(s[3] == c && s[4] == c && s[5] == c)
    count++;
  if(s[6] == c && s[7] == c && s[8] == c)
    count++;
  if(s[0] == c && s[3] == c && s[6] == c)
    count++;
  if(s[1] == c && s[4] == c && s[7] == c)
    count++;
  if(s[2] == c && s[5] == c && s[8] == c)
    count++;
  if(s[0] == c && s[4] == c && s[8] == c)
    count++;
  if(s[2] == c && s[4] == c && s[6] == c)
    count++;
  return count;
}

int evaluation(string s) {
  if(three_num(s, 'O') != 0)
    return 1000;
  if(three_num(s, 'X') != 0)
    return -1000;
  string all_O = s;
  string all_X = s;
  for(int i = 0; i < 9; i++) {
    if(all_O[i] == ' ')
      all_O[i] = 'O';
    if(all_X[i] == ' ')
      all_X[i] = 'X';
  }
  return three_num(all_O,'O')-three_num(all_X, 'X');
}

void init(node* root, int height) {
  if(height == -1)
    return;
  if(three_num(root->val, 'X') != 0 || three_num(root->val, 'O') != 0)
    return;
  for(int i = 0; i < 9; i++) {
    if(root->val[i] == ' ') {
      string new_val = root->val;
      if(root->type)
        new_val[i] = 'O';
      else
        new_val[i] = 'X';
      node* child = new node(new_val, !root->type, height, root);
      root->children.push_back(child);
      init(child, height-1);
    }
  }
}

void process(node* p) {
  if(p->children.size() == 0)
    return;
  int cur = 0;
  if(p->type) // 当前是极大层
    cur = INT_MIN;
  else        // 当前是极小层
    cur = INT_MAX;
  for(int i = 0; i < p->children.size(); i++) {
    if(p->children[i]->children.size() == 0) {
      int value = evaluation(p->children[i]->val);
      if(p->type) { // 极大层
        if(value > cur) {
          cur = value;
          p->choose = i;
        }
        if(p->parent != nullptr && p->parent->update == true && cur >= p->parent->b) {
          p->a = cur;
          return; // b剪枝
        }
      } else { // 极小层
        if(value < cur) {
          cur = value;
          p->choose = i;
        }
        if(p->parent != nullptr && p->parent->update == true && p->parent->a >= cur) {
          p->b = cur;
          return; // a减枝
        }
      }
    } else {
      process(p->children[i]);
      if(p->type) { // 极大层
        if(p->children[i]->b > cur) {
          cur = p->children[i]->b;
          p->choose = i;
        }
        if(p->parent != nullptr && p->parent->update == true && cur >= p->parent->b) {
          p->a = cur;
          return; // b剪枝
        }
      } else { // 极小层
        if(p->children[i]->a < cur) {
          cur = p->children[i]->a;
          p->choose = i;
        }
        if(p->parent != nullptr && p->parent->update == true && p->parent->a >= cur) {
          p->b = cur;
          return; // a剪枝
        }
      }
    }
  }
  if(p->parent != nullptr) {
    if(p->parent->update == false) {
      p->parent->update = true;
      if(p->parent->type) {
        p->parent->a = cur;
      } else {
        p->parent->b = cur;
      }
    } else {
      if(p->parent->type) {
        if(p->parent->a < cur)
          p->parent->a = cur;
      } else {
        if(p->parent->b > cur)
          p->parent->b = cur;
      }      
    }
  }
  if(p->type) {
    p->a = cur;
  } else {
    p->b = cur;
  }
}

string a_b(string cur) {
  int height = 2;
  node* root = new node(cur, true, height, nullptr);
  init(root, height-1);
  process(root);
  string result = root->children[root->choose]->val;
  return result;
}

void win(string cur) {
  if(three_num(cur, 'X') != 0) {
    cout << "You win!" << endl;
    exit(0);
  }
  if(three_num(cur, 'O') != 0) {
    cout << "You lose!" << endl;
    exit(0); 
  }
  for(int i = 0; i < 9; i++) {
    if(cur[i] == ' ')
      return;
  }
  cout << "tie!" << endl;
  exit(0);
}

int main() {
  string hand;
  while(hand != "Y" && hand != "N") {
    cout << "First hand or not ? Enter Y or N" << endl;
    cin >> hand;
  }
  string cur = "         ";
  if(hand == "Y") {
    while(true) {
      input(cur);
      print(cur);
      win(cur);

      cur = a_b(cur);
      print(cur);
      win(cur);
    }
  } else {
    while(true) {
      cur = a_b(cur);
      print(cur);
      win(cur);
      
      input(cur);
      print(cur);
      win(cur);
    }    
  }
}