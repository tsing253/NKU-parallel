import matplotlib.pyplot as plt
import numpy as np

# 配置中文显示
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei']  # 参考
plt.rcParams['axes.unicode_minus'] = False

# 数据准备（根据用户提供数据整理）
sizes = [100000, 200000, 400000, 800000, 1600000]
data = {
    '平凡算法': [18, 35, 70, 141, 284],
    '多路链式': [14, 29, 61, 119, 248],
    '递归': [24, 47, 93, 185, 367],
    '二重循环': [20, 39, 80, 161, 324]
}

# 创建画布
plt.figure(figsize=(12, 7), dpi=150)  # 参考

# 绘制折线
markers = ['o', 's', '^', 'D']  # 不同标记区分算法
colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728']  # 标准颜色序列
for idx, (algorithm, times) in enumerate(data.items()):
    plt.plot(sizes, times, 
             marker=markers[idx], 
             color=colors[idx],
             linewidth=2.5,
             markersize=8,
             label=algorithm)

# 坐标轴设置
plt.xscale('log', base=2)  # X轴对数坐标
plt.xticks(sizes, [f'{s//1000}K' for s in sizes])  # 显示为100K,200K等
plt.xlabel('数据规模（对数坐标）', fontsize=12)
plt.ylabel('运行时间 (ms)', fontsize=12)
plt.title('不同算法时间复杂度对比', fontsize=15, pad=20)  # 参考

# 网格线
plt.grid(True, linestyle='--', alpha=0.6)  # 参考

# 图例
plt.legend(loc='upper left', fontsize=10)

# 优化布局
plt.tight_layout()

# 保存并显示
plt.savefig('algorithm_comparison.png', bbox_inches='tight', dpi=300)  # 高清输出
plt.show()
