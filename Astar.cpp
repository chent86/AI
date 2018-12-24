#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

// 当前八数码的所有移动可能
vector<string> get_children(string node) {
  int pos = node.find('0');
  vector<string> result;
  if(pos/3 != 0) {
    string tmp = node;
    tmp[pos] = node[pos-3];
    tmp[pos-3] = node[pos];
    result.push_back(tmp);
  }
  if(pos/3 != 2) {
    string tmp = node;
    tmp[pos] = node[pos+3];
    tmp[pos+3] = node[pos];
    result.push_back(tmp);    
  }
  if(pos%3 != 0) {
    string tmp = node;
    tmp[pos] = node[pos-1];
    tmp[pos-1] = node[pos];
    result.push_back(tmp);      
  }
  if(pos%3 != 2) {
    string tmp = node;
    tmp[pos] = node[pos+1];
    tmp[pos+1] = node[pos];
    result.push_back(tmp);      
  }
  return result;
}

struct node{
  set<string> ancestors;
  node* parent;
  vector<node*> children;
  string data;
  int depth;
  int eval;
  node(string data, node* parent, vector<node*> children, set<string> ancestors, int depth, int eval) {
    this->data = data;              //　当前八数码
    this->parent = parent;          //  父亲结点
    this->children = children;      //  所有孩子结点
    this->ancestors = ancestors;    //  所有祖先结点
    this->depth = depth;            //  结点所在深度
    this->eval = eval;              //  结点的评估
  }
};

bool comp(node* a, node* b) {
  return (a->depth+a->eval) < (b->depth+b->eval);
}

string final = "123804765";
int type = 2;
int evaluation(string s) {
  if(type == 1) {
    int count = 0;
    for(int i = 0; i < 9; i++) {
      if(s[i] != final[i] && s[i] != '0')
        count++;
    }
    return count;
  }
  int count = 0;
  for(char i = '1'; i < '9'; i++) {
    int start = s.find(i);
    int end = final.find(i);
    count += abs(start/3-end/3)+abs(start%3-end%3);  //　后者在复杂情况下会快很多
  }
  return count;
  // 九数码
  // if(type == 1) {
  //   int count = 0;
  //   for(int i = 0; i < 9; i++) {
  //     if(s[i] != final[i])
  //       count++;
  //   }
  //   return count;
  // }
  // int count = 0;
  // for(char i = '0'; i < '9'; i++) {
  //   int start = s.find(i);
  //   int end = final.find(i);
  //   count += abs(start/3-end/3)+abs(start%3-end%3);  //　后者在复杂情况下会快很多
  // }
  // return count;
}

void update(node* current, int depth) {
  if(current->children.size() == 0)
    return;
  for(auto& i: current->children) {   // 更新所有子节点的费用
    i->depth = depth+1;
    update(i, i->depth);
  }
}

void format(string s) {
	for(int i = 0; i < 9; i++) {
		cout << s[i] << " ";
		if(i%3 == 2)                       // 输出为矩阵形式
			cout << endl; 
	}
  cout << "评估函数值: " << evaluation(s) << endl;
}


string generator() {
	string origin = "123456780";
    srand(time(0));
	for(int i = 0; i < 10; i++) {        // 生成随机初始化的矩阵
		int a=rand() % 9;
		int b= rand() % 9;
		char c = origin[a];
		origin[a] = origin[b];
		origin[b] = c;
	}
	return origin;
}

bool check(string s) {
  int count_a = 0, count_b = 0;
  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < i; j++) {        // 判断八数码问题是否有解
      if(s[j] != '0') {
        if(s[j] < s[i])
          count_a++;
      }
      if(final[j] != '0') {
        if(final[j] < final[i])
          count_b++;
      }
    }
  }
  if((count_a+count_b)%2==1) {
    // cout << "no solution" << endl;
    return false;
  }
  return true;
}

int main() {
  cout << "Choose a kind of evaluation: 1 or 2" << endl;
  cin >> type;
  if(type != 1 && type != 2)
    return 0;
  string init = "507814326";
  // string init = generator();
  while(!check(init))
    init = generator();
  vector<node*> open;
  map<string, node*> G_table;                                        // G_table 存储所有的节点

  node* init_node = new node(init, nullptr, {}, {}, 0, evaluation(init));
  open.push_back(init_node);
  G_table.insert(make_pair(init, init_node));                        // 初始化添加第一个节点

  bool reach = false;
  // set<string> test;

  int total = 1; // 总扩展的节点数
  while(!open.empty()) {
    sort(open.begin(), open.end(), comp);

    node* top = open[0];                                             // 选取f(n)值最小的节点

    cout << " 当前open表的节点数: " << open.size();
    cout << " 评估函数值最小的节点: " << top->data; 
    cout << " 评估函数的最小值: " << top->depth+top->eval;
    cout << " 总扩展的节点数: " << total;
    printf("\r\033[k"); 

    if(top->depth+top->eval > 27) {
      cout << "error" << endl;
      exit(0);
    }

    open.erase(open.begin());
    
    // if(test.find(top->data) != test.end())
    //   break;
    // test.insert(top->data);

    if(top->data == final) {
      cout << endl << "f*(S0)=" << top->depth;
      reach = true;
      break;
    }

    vector<string> children = get_children(top->data);               // 产生n的一切后继
    set<string> ancestors = top->ancestors;                       // 后继的祖先为n的祖先加上n
    ancestors.insert(top->data);

    for(int i = 0; i < children.size(); i++) {
      if(top->ancestors.find(children[i]) != top->ancestors.end())   // 不能是它的前驱节点
        continue;
      // if(G_table.find(children[i]) != G_table.end())
      //   continue;
      if(G_table.find(children[i]) == G_table.end()) {               // 如果P不属于G
        node* child = new node(children[i], top, {}, {}, top->depth+1, evaluation(children[i]));
        top->children.push_back(child);
        open.push_back(child);
        total++;
        G_table.insert(make_pair(children[i], child));
      } else {
        if(top->depth+1 < G_table[children[i]]->depth) {
          G_table[children[i]]->depth = top->depth+1;
          G_table[children[i]]->parent = top;                        // 修改P到n的指针
          G_table[children[i]]->ancestors = ancestors;  
                                                                     
          update(G_table[children[i]], top->depth+1);                // 更改P的子节点n的指针和费用(指针不用修改)
        }
      }
    }
  }
  if(reach) {
    cout << endl;
  	vector<string> path;
    node* p = G_table[final];
    while(p->parent) {
       path.push_back(p->data);
      p = p->parent;
    }
    path.push_back(p->data);
    for(int i = 0; i < path.size()-1; i++)
      if(evaluation(path[i]) > 1+evaluation(path[i+1])) {
        cout << "error" << endl;
        exit(0);
      }
    for(auto i = path.rbegin(); i != path.rend(); i++) {
      cout << *i << " 评估值" << evaluation(*i) << endl;
    	// format(*i);
    }
  } else {
    cout << "cannot reach" << endl;
  }
}

