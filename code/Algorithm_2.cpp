#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<queue>
#include<unordered_set>
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

int bin_cnt;               		// 桶个数
vector<int> bin;           		// 桶的起始位置
vector<pii> sorted_edges;  		// 根据度数排序的边有序数组
vector<map<int, int>> edge_pos; // 边的位置


// @brief: 根据度数排序
// @param: u, v
// @ret: None
inline bool cmp(int u, int v){
	if(deg[u] != deg[v]) return deg[u] < deg[v];
	return u < v;
}


// @brief: 根据度数交换节点顺序
// @param: u, v
// @ret: None
inline void order_pair(int &u, int &v){
	if(!cmp(u, v)) swap(u, v);
}


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


// @brief: 根据度数排序
// @param: None
// @ret: None
void reorder(){
	itou.resize(n);
	for(int i = 0; i < n; i ++ ) itou[i] = i;
	sort(begin(itou), end(itou), cmp);
}


// @brief: 求u和v的邻居的交集
// @param: nb_u, nb_v
// @ret: nb_join
vector<int> intersect(vector<int> nb_u, vector<int> nb_v){
	vector<int> nb_join;
	int i = 0;
	for(auto &w : nb_u){
		while(i < (int)nb_v.size() && nb_v[i] > w) i ++ ;
		if(i >= (int)nb_v.size()) break;
		if(nb_v[i] == w) nb_join.push_back(w);
	}
	return nb_join;
}


// @brief: triangle listing
// @param: None
// @ret: None
void count_triangles() {
	nb.resize(n);
	int cnt = 0;   // 统计三角形个数
	for(int idx = n - 1; ~idx; idx -- ){
		int v = itou[idx];
		for (auto [u, sup] : G[v]) {
			if(!cmp(u, v)) continue;
			vector<int> nb_join = intersect(nb[u], nb[v]);
			for(int i = 0; i < (int)nb_join.size(); i ++ ){
				int w = itou[nb_join[i]];
				update_edge_sup(u, v, 1);  // 更新三条边的sup值
				update_edge_sup(v, w, 1);
				update_edge_sup(w, u, 1);
				cnt ++ ;
			}
			nb[u].push_back(idx);
		}
	}
	cout << cnt << " triangles found.\n";
}


// @brief: 桶排序，根据sup进行排序，并求得每个桶的起始位置、边在序列中国的位置
// @param: None
// @ret: None
void bin_sort(){
	bin.resize(n);
	int max_sup = 0;   // 记录最大的sup
	for(int u = 0; u < n; u ++ ){
		for(auto &[v, sup] : G[u]){
			if(!cmp(u, v)) continue;
			bin[sup] ++ ;   // 累计每个桶的边数
			max_sup = max(max_sup, sup);
		}
	}

	bin_cnt = max_sup + 1;  // 最大的sup+1即为桶的个数
	int pre_bin_sum = 0;    // 统计前缀桶大小之和
	for(int i = 0; i < bin_cnt; i ++ ){
		int sz = bin[i];
		bin[i] = pre_bin_sum;
		pre_bin_sum += sz;
	}

	sorted_edges.resize(m);
	edge_pos.resize(n);
	for(int u = 0; u < n; u ++ ){
		for(auto &[v, sup] : G[u]){
			if(!cmp(u, v)) continue;
			int &p = bin[sup];
			sorted_edges[p] = MP(u, v);  // 记录边的位置
			edge_pos[u][v] = p ++ ;
		}
	}
	// 转换为每个桶的起始位置
	for(int i = bin_cnt; i; i -- ) bin[i] = bin[i - 1];  
	bin[0] = 0;
}


// @brief: 更新边的位置以及所在桶的起始位置
// @param: u, v
// @ret: None
void update_edge(int u, int v, int min_sup){
	order_pair(u, v);
	int sup = G[u][v];

	if(sup <= min_sup) return;   

	int p_bin = bin[sup];         // 这条边(u,v)的sup值对应的桶的起始位置
	int p_edge = edge_pos[u][v];  // 边(u,v)当前在有序序列中的位置
	
	if(p_edge != p_bin){
		pii tmp_edge = sorted_edges[p_bin];
		pii edge = MP(u, v);

		// 将这条边(u,v)换到桶的起始位置
		edge_pos[u][v] = p_bin;     
		sorted_edges[p_bin] = edge;

		// 将桶起始位置的边换到(u,v)的位置
		edge_pos[tmp_edge.fi][tmp_edge.se] = p_edge;
		sorted_edges[p_edge] = tmp_edge;
	}

	bin[sup] ++ ;   // 对应的桶起始位置往后移
	update_edge_sup(u, v, -1);  // 更新(u,v)的sup
}


// @brief: 输出k-class所有的边
// @param: None
// @ret: None
void output_k_class(){
	for(auto &[k, edges] : k_class){
		cout << k << "-class: \n";
		// for(auto &[u, v] : edges){
		// 	cout << "(" << u << ", " << v << ")" << "\n";
		// }
		cout << "total edges: " << (int)edges.size() << "\n";
		cout << "------------\n\n";
	}
}


// @brief: Improved Truss Decomposition（Algorithm 2）
// @param: None
// @ret: None
// @complexity: O(m+n) memory, O(m\sqrt(m)) time
void ImprovedTrussDecomposition(){
	reorder();
	count_triangles();
	bin_sort();

	int p = 0;  // 指针，用于记录下一个删除的边的位置，位置 <p 的边都被视为已经删除
	while(p < m){
		auto [u, v] = sorted_edges[p];
		order_pair(u, v);
		int sup_uv = G[u][v];

		k_class[sup_uv + 2].emplace_back(u, v);

		int cnt = 0;
		for(auto &[w, sup] : G[u]){
			if(cnt == sup_uv) break;
			if(w == v) continue;
			if(G[v].count(w)){
				cnt ++ ;
				update_edge(u, w, sup_uv);
				update_edge(v, w, sup_uv);
			}
		}
		remove_edge(u, v);
		p ++ ;
	}

	output_k_class();
	cout << "Algorithm Completed.\n";
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
	string path("../data/P2P");  
	string file_in = path + ".txt";
	// string file_out = path + "-out-2.txt";

	// 打开文件读入数据，并写入到指定输出文件
	freopen(file_in.c_str(), "r", stdin);    
	// freopen(file_out.c_str(), "w", stdout);

	BuildGraph();  // 建图

	ImprovedTrussDecomposition();  // Improved Truss Decomposition 算法

	// 重新定向到控制台
	// freopen("CON", "r", stdin);
    // freopen("CON", "w", stdout);

	end_time = clock();     // 结束计时
	duration = (long double)(end_time - start_time) / CLOCKS_PER_SEC;
	cout << "Spend Time = " << duration << "s\n";

	return 0;
}


// P2P: 0.061s
// HEP: 0.177s
// Amazon: 21.777s
// Wiki: 60.765s
// Skitter: 163.465s