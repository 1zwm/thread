#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include <algorithm>
#include <queue>

using namespace std;

typedef struct node {
        int ddl, value;
    }Node;

class solution {



public:
    int finalday = 0;
    int ans = 0;

    unordered_map<int, vector<int>> map;  //地图信息
    unordered_map<int, int> minDis; //存储最短路径
    priority_queue<int, vector<int>, less<int> > mvalue;

    static bool cmp_ddl(const Node left, const Node right) { // 比较函数
        return left.ddl < right.ddl;
    }

    void Dijkstra(int n) {  //求最短路径, n代表有多少地点
        bool judge[n + 1] = { false };
        for(int i = 1; i <= n; i++) { // 1、创建距离表（设置初值为最大
            minDis[i] = INT32_MAX;
        }
        for(int i = 0; i < map[0].size(); i += 2) { // 2、遍历起点0，找到邻接顶点，并刷新距离表
            int temp = map[0][i];
            int range = map[0][i+1];
            minDis[temp] = min(minDis[temp], range); 
        }

        judge[0] = true;    // 先遍历0点，标记0点已经遍历过

        int count = 1;   // 记录已经遍历的点的数量

        while(count < n) {

            unordered_map<int,int>::iterator a;

            for(auto i = minDis.begin(); i != minDis.end(); i++) { // 3、遍历找到距离0最近且未遍历的点
                if(!judge[i->first]) {
                    a = i;
                    break;
                }
            }

            for(auto i = minDis.begin(); i != minDis.end(); i++) {
                if(!judge[i->first]) {
                    a = i->second < a->second ? i : a;
                }
            }

        
            for(int i = 0; i < map[a->first].size(); i += 2) { // 4、遍历找到的a点，刷新距离表
                int temp = map[a->first][i];
                if(!judge[temp]) {
                    int range = map[a->first][i+1];
                    minDis[temp] = min(minDis[temp], range + a->second);
                }   
            }

            judge[a->first] = true; // 标记已经遍历的点。
            count++;    

        }

    }

    int read() {
        int n, m;
        int maxlocate = 0;
        cin >> n >> m;
        int num1, num2, num3;
        for(int i = 0; i < m; i++) {            //读取地图信息
            cin >> num1 >> num2 >> num3;
            map[num1].emplace_back(num2);
            map[num1].emplace_back(num3);
            
            map[num2].emplace_back(num1);
            map[num2].emplace_back(num3);
            maxlocate = max(num1, num2);
        } 
        
        Dijkstra(maxlocate); // 求出最短路径.


        Node mail[n];

        for(int j = 1; j <= n; j++) {           //读取截止日期 及 截止日期内对应的最短路径（盈亏）
            cin >> num1;
            mail[j-1].ddl = num1;
            mail[j-1].value = minDis[j];
            finalday = max(finalday, num1);
        }

        sort(mail, mail+n, cmp_ddl);

        int sum = 0;  //丢弃的包裹价值
        int cnt = 0;  //快递数量
        // while(end <= finalday) {
        //     while(mail[i].ddl <= end && cnt < end) {
        //         mvalue.push(mail[i].value);
        //         cnt++;
        //         i++;
        //     }
        //     while(mail[i].ddl <= end) {
        //         if(mail[i].value > mvalue.top()) {
        //             sum += mvalue.top();
        //             mvalue.pop();
        //             mvalue.push(mail[i].value);
        //         } else {
        //             sum += mail[i].value;
        //         }
        //         i++;
        //     }
        //     end++;
        // }

        for(int i = 0; i < n; i++) {
            if(cnt < mail[i].ddl) {
                mvalue.push(mail[i].value);
                cnt++;
            } else {
                if(mail[i].value > mvalue.top()){
                    sum += mvalue.top();
                    mvalue.pop();
                    mvalue.push(mail[i].value);
                } else {
                    sum += mail[i].value;
                }
            }
        }

        while(!mvalue.empty()) {
            ans += mvalue.top();
            mvalue.pop();
        }

        ans -= sum;
        return ans;

    }
    

};


int main() {
    int m_ans;
    solution s;
    cout << " 请输入数据 ： " << endl;
    m_ans = s.read();
    cout << endl << "答案：" << m_ans << endl;
    //cout << ans << endl;
    return 1;

}