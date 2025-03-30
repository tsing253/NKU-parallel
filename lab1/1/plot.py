import matplotlib.pyplot as plt
import numpy as np

# 配置中文显示
plt.rcParams['font.sans-serif'] = ['Microsoft YaHei']  # 参考
plt.rcParams['axes.unicode_minus'] = False

# 新数据转换
sizes = [1000, 2000, 4000, 8000, 16000]
data = {
    'Normal': [30, 132, 614, 2677, 13601],
    'Optimized': [10, 66, 246, 985, 3914]
}

# 创建画布
plt.figure(figsize=(12, 7), dpi=150)  # 参考

# 绘制折线
markers = ['o', 'D']  # 减少标记数量
colors = ['#1f77b4', '#ff7f0e']  # 标准颜色序列
for idx, (algorithm, times) in enumerate(data.items()):
    plt.plot(sizes, times, 
             marker=markers[idx], 
             color=colors[idx],
             linewidth=2.5,
             markersize=8,
             label=algorithm)

# 坐标轴设置
plt.xscale('log', base=2)  # X轴对数坐标
plt.xticks(sizes, [f'{s//1000}K' for s in sizes])  # 显示为1K,2K等
plt.xlabel('矩阵规模（对数坐标）', fontsize=12)
plt.ylabel('运行时间 (ms)', fontsize=12)
plt.title('矩阵运算算法性能对比', fontsize=15, pad=20)

# 网格线
plt.grid(True, linestyle='--', alpha=0.6)  # 参考

# 图例
plt.legend(loc='upper left', fontsize=10)

# 优化布局
plt.tight_layout()

# 保存并显示
plt.savefig('algorithm_comparison2.png', bbox_inches='tight', dpi=300)
plt.show()
