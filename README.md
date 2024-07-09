## **项目介绍**

本项目主要复现了论文 [Truss Decomposition in Massive Networks](https://arxiv.org/abs/1205.6693) 中的 <font face='Times'>Algorithm 1</font> 和 <font face='Times'>Algorithm 2</font>。包含两种算法的 C++ 代码，并用了 [SNAP](https://snap.stanford.edu/data/) 的一些数据进行实验，验证了两种算法效率的不同。

## **环境依赖**

编译环境为 `g++ std=c++17`，建议 GCC 版本 `9.2.0` 及以上。

## **目录结构描述**
```
Truss Decomposition
├─code                  // C++ 代码
|  ├─Algorithm_1.cpp
|  └─Algorithm_2.cpp
├─data                  // 数据集
|  ├─Amazon.txt
|  ├─HEP.txt
|  ├─P2P.txt
|  ├─Skitter.txt
|  └─Wiki.txt
├─res                   // 实验结果总结
|  ├─实验结果总结.md
|  └─实验结果总结.pdf
├─paper                 // 参考论文
|  ├─0310049 (marked).pdf
|  ├─1205.6693 (marked).pdf
|  └─Latapy-2008-triangles (marked).pdf
├─notes                 // 阅读笔记markdown
|  ├─论文阅读笔记.md
|  └─论文阅读笔记.pdf
├─README.md             // 说明文档
└─README.pdf
```

## **数据集**

| Name | $V_G$  | $E_G$ | Number of $\bigtriangleup$ | $k_{max}$ |
|:--------|:---------:|:--------:|:--------:|:--------:|
| [P2P](https://snap.stanford.edu/data/p2p-Gnutella08.html) | 6,301 | 41,554 | 2383 | 5 |
| [HEP](https://snap.stanford.edu/data/ca-HepTh.html) | 9,877 | 25,998 | 28339 | 32 |
| [Amazon](https://snap.stanford.edu/data/amazon0601.html) | 403,394 | 3,387,388 | 3,986,507 | 11 | 
| [Wiki](https://snap.stanford.edu/data/wiki-Talk.html) | 2,394,385 | 5,021,410 | 9,203,519 | 53 | 
| [Skitter](https://snap.stanford.edu/data/as-Skitter.html) | 1,696,415 | 11,095,298 | 28,769,868 | 68 | 

<br/>

## **运行时间**

| Name | Algorithm 1  | Algorithm 2 |
|:--------|:---------:|:--------:|
| [P2P](https://snap.stanford.edu/data/p2p-Gnutella08.html)  | 0.122s | **0.061s** |
| [HEP](https://snap.stanford.edu/data/ca-HepTh.html) | 0.267s | **0.177s** |
| [Amazon](https://snap.stanford.edu/data/amazon0601.html) | 37.166s | **21.777s** |
| [Wiki](https://snap.stanford.edu/data/wiki-Talk.html) | 1865.03s | **60.765s** |
| [Skitter](https://snap.stanford.edu/data/as-Skitter.html) | 3514.55s | **163.465s** |
