/**
 *
 * 测试题一：
用c++语言，使用 abcdefghi......xyz 26个字母组合成 aaa aab aac ...... zzz 这样的三位字母表，
每位均是a-z总共26^3=17576个。分别用vector/list/map/unordered_map/queue/dequeue/set/unordered_set来进行如下操作：
1、增操作，把所有的元素存入容器
2、查操作及改操作，找到所有包含xy的元素，把它们中的xy修改成数字89。
3、删操作，先把包含ab的元素统统从容器中删除，再把剩余所有的元素中不包括ac的元素全部从容器中删除，最后再把剩余元素全部从容器中删除。

统计所有容器的三步操作中分别耗时多少并打印在屏幕上。
推荐使用C++11新特性，注意代码的可读性。

提示：
可以参考的书籍：《C++标准模板库》、《C++ Primer》、《STL源码剖析》
可以参考的网站：http://zh.cppreference.com   https://msdn.microsoft.com/zh-cn/library/a7tkse1h.aspx
    本题仅要求会用STL，不要求深层次探索STL的实现原理。
 */

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <functional>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <deque>
#include <algorithm>
using namespace std;
vector<string> generate() {
  vector<string> result;
  for (char i = 'a'; i <= 'z'; i++) {
    for (char j = 'a'; j <= 'z'; j++) {
      for (char k = 'a'; k <= 'z'; k++) {
        string s;
        s.push_back(i);
        s.push_back(j);
        s.push_back(k);
        result.push_back(s);
      }
    }
  }
  return result;
}

template<class T>
using F = std::function<void(const vector<string> &data, T &)>;

template<class T>
void work(vector<string> &data, F<T> insert, F<T> mod, F<T> del, const string &msg) {
  auto start = std::chrono::system_clock::now();
  T t;
  insert(data, t);
  mod(data, t);
  del(data, t);
  auto end = std::chrono::system_clock::now();
  std::cout << msg << " : " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << std::endl;
}

string modify(const string &s) {
  auto x = s.find('x');
  auto y = s.find('y');
  if (x != string::npos && y != string::npos) {
    string tmp = s;
    tmp[x] = '8';
    tmp[y] = '9';
    return tmp;
  }
  return "";
}
bool checkAB(const string &s) {
  return s.find('a') != string::npos && s.find('b') != string::npos;
}

bool checkNotAC(const string &s) {
  return !(s.find('a') != string::npos && s.find('c') != string::npos);
}

int main() {
  auto data = generate();
  //vector
  work<vector<string>>(data,
                       [](const vector<string> &data, vector<string> &t) {
                         std::copy(data.begin(), data.end(), back_inserter(t));
                       },
                       [](const vector<string> &data, vector<string> &t) {
                         for (auto &item : t) {
                           auto ret = modify(item);
                           if (!ret.empty()) {
                             item = ret;
                           }
                         }
                       },
                       [](const vector<string> &data, vector<string> &t) {
                         t.erase(remove_if(t.begin(), t.end(), checkAB), t.end());
                         t.erase(remove_if(t.begin(), t.end(), checkNotAC), t.end());
                         t.clear();
                       },
                       "vector"
  );
  //list
  work<list<string>>(data,
                     [](const vector<string> &data, list<string> &t) {
                       std::copy(data.begin(), data.end(), back_inserter(t));
                     },
                     [](const vector<string> &data, list<string> &t) {
                       for (auto &item : t) {
                         auto ret = modify(item);
                         if (!ret.empty()) {
                           item = ret;
                         }
                       }
                     },
                     [](const vector<string> &data, list<string> &t) {
                       t.remove_if(checkAB);
                       t.remove_if(checkNotAC);
                       t.clear();
                     },
                     "list"
  );
  //map
  work<map<string, int>>(data,
                         [](const vector<string> &data, map<string, int> &t) {
                           std::transform(data.begin(), data.end(), std::inserter(t, t.end()), [](const string &s) {
                             return std::make_pair(s, 1);
                           });
                         },
                         [](const vector<string> &data, map<string, int> &t) {
                           vector<string> insert;
                           for (auto iter = t.begin(); iter != t.end();) {
                             auto ret = modify(iter->first);
                             if (ret.empty()) {
                               iter++;
                             } else {
                               insert.push_back(ret);
                               t.erase(iter++);
                             }
                           }
                           transform(insert.begin(), insert.end(), std::inserter(t, t.end()), [](const string &s) {
                             return make_pair(s, 1);
                           });
                         },
                         [](const vector<string> &data, map<string, int> &t) {
                           for (auto iter = t.begin(); iter != t.end();) {
                             if (checkAB(iter->first)) {
                               t.erase(iter++);
                             } else {
                               iter++;
                             }
                           }
                           for (auto iter = t.begin(); iter != t.end();) {
                             if (checkNotAC(iter->first)) {
                               t.erase(iter++);
                             } else {
                               iter++;
                             }
                           }
                           t.clear();
                         }, "map"
  );
  //unordered_map
  work<unordered_map<string, int>>(data,
                                   [](const vector<string> &data, unordered_map<string, int> &t) {
                                     std::transform(data.begin(),
                                                    data.end(),
                                                    std::inserter(t, t.end()),
                                                    [](const string &s) {
                                                      return std::make_pair(s, 1);
                                                    });
                                   },
                                   [](const vector<string> &data, unordered_map<string, int> &t) {
                                     vector<string> insert;
                                     for (auto iter = t.begin(); iter != t.end();) {
                                       auto ret = modify(iter->first);
                                       if (ret.empty()) {
                                         iter++;
                                       } else {
                                         insert.push_back(ret);
                                         t.erase(iter++);
                                       }
                                     }
                                     transform(insert.begin(),
                                               insert.end(),
                                               std::inserter(t, t.end()),
                                               [](const string &s) {
                                                 return make_pair(s, 1);
                                               });
                                   },
                                   [](const vector<string> &data, unordered_map<string, int> &t) {
                                     for (auto iter = t.begin(); iter != t.end();) {
                                       if (checkAB(iter->first)) {
                                         t.erase(iter++);
                                       }else{
                                         iter++;
                                       }
                                     }
                                     for (auto iter = t.begin(); iter != t.end();) {
                                       if (checkNotAC(iter->first)) {
                                         t.erase(iter++);
                                       }else{
                                         iter++;
                                       }
                                     }
                                     t.clear();
                                   }, "unordered_map"
  );
  //queue
  work<queue<string>>(data,
                      [](const vector<string> &data, queue<string> &t) {
                        for (auto &item : data) {
                          t.push(item);
                        }
                      },
                      [](const vector<string> &data, queue<string> &t) {
                        queue<string> q;
                        while (!t.empty()) {
                          auto item = t.front();
                          t.pop();
                          auto ret = modify(item);
                          if (!ret.empty()) {
                            q.push(ret);
                          } else {
                            q.push(item);
                          }
                        }
                        swap(q, t);
                      },
                      [](const vector<string> &data, queue<string> &t) {
                        queue<string> q;
                        while (!t.empty()) {
                          auto item = t.front();
                          t.pop();
                          if (!checkAB(item)) {
                            q.push(item);
                          }
                        }
                        swap(q, t);
                        q = queue<string>();
                        while (!t.empty()) {
                          auto item = t.front();
                          t.pop();
                          if (!checkNotAC(item)) {
                            q.push(item);
                          }
                        }
                        swap(q, t);
                        t = queue<string>();
                      },
                      "queue"
  );
  //dequeue
  work<deque<string>>(data,
                      [](const vector<string> &data, deque<string> &t) {
                        std::copy(data.begin(), data.end(), back_inserter(t));
                      },
                      [](const vector<string> &data, deque<string> &t) {
                        for (auto &item : t) {
                          auto ret = modify(item);
                          if (!ret.empty()) {
                            item = ret;
                          }
                        }
                      },
                      [](const vector<string> &data, deque<string> &t) {
                        t.erase(remove_if(t.begin(), t.end(), checkAB), t.end());
                        t.erase(remove_if(t.begin(), t.end(), checkNotAC), t.end());
                        t.clear();
                      },
                      "dequeu"
  );
  //set
  work<set<string>>(data,
                    [](const vector<string> &data, set<string> &t) {
                      std::copy(data.begin(), data.end(), inserter(t, t.end()));
                    },
                    [](const vector<string> &data, set<string> &t) {
                      vector<string> insert;
                      for (auto iter = t.begin(); iter != t.end();) {
                        auto ret = modify(*iter);
                        if (!ret.empty()) {
                          insert.push_back(ret);
                          t.erase(iter++);
                        } else {
                          iter++;
                        }
                      }
                      copy(insert.begin(), insert.end(), inserter(t, t.end()));
                    },
                    [](const vector<string> &data, set<string> &t) {
                      for (auto iter = t.begin(); iter != t.end();) {
                        if (checkAB(*iter)) {
                          t.erase(iter++);
                        } else {
                          iter++;
                        }
                      }

                      for (auto iter = t.begin(); iter != t.end();) {
                        if (checkNotAC(*iter)) {
                          t.erase(iter++);
                        } else {
                          iter++;
                        }
                      }

                      t.clear();
                    },
                    "set"
  );
  //unordered_set
  work<unordered_set<string>>(data,
                              [](const vector<string> &data, unordered_set<string> &t) {
                                std::copy(data.begin(), data.end(), inserter(t, t.end()));
                              },
                              [](const vector<string> &data, unordered_set<string> &t) {
                                vector<string> insert;
                                for (auto iter = t.begin(); iter != t.end();) {
                                  auto ret = modify(*iter);
                                  if (!ret.empty()) {
                                    insert.push_back(ret);
                                    t.erase(iter++);
                                  } else {
                                    iter++;
                                  }
                                }
                                copy(insert.begin(), insert.end(), inserter(t, t.end()));
                              },
                              [](const vector<string> &data, unordered_set<string> &t) {
                                for (auto iter = t.begin(); iter != t.end();) {
                                  if (checkAB(*iter)) {
                                    t.erase(iter++);
                                  } else {
                                    iter++;
                                  }
                                }

                                for (auto iter = t.begin(); iter != t.end();) {
                                  if (checkNotAC(*iter)) {
                                    t.erase(iter++);
                                  } else {
                                    iter++;
                                  }
                                }

                                t.clear();
                              },
                              "unordered_set"
  );

  return 0;
}