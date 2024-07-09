#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<queue>
#include<ctime>
#include<cmath>
using namespace std;

#define MP make_pair
#define fi first
#define se second
typedef pair<int, int> pii;


int n, m;                  // n为节点个数，m为边的个数
vector<map<int, int>> G;   // 无向无权图G，map实现，G[u][v]存储sup值
vector<vector<int>> nb;    // 邻居的（排序后的）编号，且deg[v] > deg[u]
vector<int> itou;          // 排序后的编号
vector<int> deg;           // 节点度数
map<int, vector<pii>> k_class;


// @brief: 更新边的support
// @param: u, v, c
// @ret: None
inline void update_edge_sup(int u, int v, int c){
	G[u][v] += c;
	G[v][u] += c;
}


// @brief: 删除边(u,v)，时间复杂度 O(logn)
// @param: u, v
// @ret: None
inline void remove_edge(int u, int v){
	G[u].erase(v);
	G[v].erase(u);
}


// @brief: G中的边是否全部被去除
// @param: None
// @ret: bool，false表示存在边没有去除，true表示所有边都被去除
bool all_edges_removed(){
	for(int u = 0; u < n; u ++ )
		if(G[u].size()) return false;
	return true;
}


// @brief: 求u和v的邻居的交集
// @param: u, v
// @ret: vector<int>
vector<int> get_join_nodes(int u, int v){
	vector<int> res;
	for(auto &[w, _] : G[u])
		if(G[v].count(w))
			res.emplace_back(w);
	return res;
}


// @brief: 获取当前support小于k-2的边
// @param: k
// @ret: None, 直接入队
void get_sup_lower_edges(int k, queue<pii> &q){
	for(int u = 0; u < n; u ++ )
		for(auto &[v, sup] : G[u]){
			if(sup < k - 2) q.emplace(MP(u, v));
		}
}


// @brief: (u,v)使得新的边满足 sup < k-2，进行入队操作
// @param: u, v, k, q
// @ret: None
inline void push_edge(int u, int v, int k, queue<pii> &q){
	if(G[u].count(v) && G[u][v] < k - 2){
		q.emplace(MP(u, v));
	}
}


// @brief: 输出k-class所有的边
// @param: None
// @ret: None，前cnt行输出k-class中的每条边，最后一行输出边条数
void count_support(){
	for(int u = 0; u < n; u ++ )
		for(auto &[v, _] : G[u])
			G[u][v] = (int)get_join_nodes(u, v).size();
}


// @brief: 输出k-class所有的边
// @param: None
// @ret: None
void output_k_truss(int k){
	int cnt = 0;
	cout << k << "-truss: \n";
	for(int u = 0; u < n; u ++ ){
		for(auto &[v, sup] : G[u]){
			if(u > v) continue;
			cnt ++ ;
		}
	}
	cout << "total edges: " << cnt << "\n";
	cout << "------------\n\n";
}


// @brief: Truss Decomposition（Algorithm 1）
// @param: None
// @ret: None
// @complexity: O(m+n) memory, O(\sum_{v\in V_G}(deg(v))^2) time
void TrussDecomposition(){
	count_support();

	output_k_truss(2);  // k=2，即G本身

	int k = 3;

	while(true){  // G中存在边未被去除
		queue<pii> q;   // 队列，存储 sup < k - 2 的边的编号
		get_sup_lower_edges(k, q);  // 当前 sup < k - 2 的边
		
		while(q.size()){  // G中存在sup < k-2的边
			auto [u, v] = q.front(); 
			q.pop();

			if(!G[u].count(v)) continue;
			remove_edge(u, v);

			vector<int> W = get_join_nodes(u, v);  // nb(u) \cap nb(v)

			for(auto &w : W){
				update_edge_sup(u, w, -1);  // 更新sup_{uw}和sup_{vw}
				update_edge_sup(v, w, -1);
				push_edge(u, w, k, q);  // 判断(u,v)是否使得新的边满足 sup < k-2
				push_edge(v, w, k, q);
			}
		}

		if(all_edges_removed()){  // 如果所有边都被去除了，退出循环
			cout << "Algorithm Completed.\n";
			break;  
		}

		output_k_truss(k);  // 输出k-truss的边
		k ++ ;              // k += 1，继续求解下一个k-truss
	}
}


// @brief: 建图
// @param: None
// @ret: None
void BuildGraph(){
	cin >> n >> m;
	
	int mx = 0;
	vector<pii> edges;
	for(int i = 1, u, v; i <= m; i ++ ){
		cin >> u >> v;
		if(u == v) continue;
		edges.emplace_back(u, v);
		mx = max({mx, u, v});  // 统计最大的点编号
	}

	n = mx + 1;
	G.resize(n);
	deg.resize(n);
	for(auto &[u, v] : edges){
		if(u == v) continue;    // 去除自环
		if(!G[u].count(v)){
			G[u][v] = 0;
			G[v][u] = 0;
			deg[u] ++ , deg[v] ++ ;
		}
	}
}


int main(){
	clock_t start_time, end_time;
	long double duration;

	start_time = clock();   // 开始计时

	// 数据文件路径
	string path("../data/Skitter");  
	string file_in = path + ".txt";
	// string file_out = path + "-out-1.txt";

	// 打开文件读入数据，并写入到指定输出文件
	freopen(file_in.c_str(), "r", stdin);    
	// freopen(file_out.c_str(), "w", stdout);

	BuildGraph();  // 建图

	TrussDecomposition();  // Truss Decomposition 算法

	// 重新定向到控制台
	// freopen("CON", "r", stdin);
    // freopen("CON", "w", stdout);

	end_time = clock();     // 结束计时
	duration = (long double)(end_time - start_time) / CLK_TCK;
	cout << "Spend Time = " << duration << "s\n";

	return 0;
}


// P2P: 0.122s
// HEP: 0.267s
// Amazon: 37.166s
// Wiki: 1865.03s
// Skitter: 3514.55s