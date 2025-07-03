import matplotlib.pyplot as plt
import numpy as np

# 示例数据 (替换为你自己的数据)
data = {
    "tasks": [2e7, 4e7, 6e7, 8e7, 1e8],  # 任务数列表
    "serial_time": [5.16, 11.43, 15.40, 21.95, 27.59],  # 原版串行时间（秒）
    "parallel_time": [4.84, 10.71, 14.63, 19.42, 26.95],      # 并行时间（秒）
    "speedup": [8.0, 8.0, 8.0, 8.0, 8.0]           # 加速比（可注释掉后用下方公式自动计算）
}

# 自动计算加速比（如果未提供）
data["speedup"] = [s/p for s,p in zip(data["serial_time"], data["parallel_time"])]

# 创建画布与子图
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), gridspec_kw={'height_ratios': [2, 1]})
plt.subplots_adjust(hspace=0.3)

# ----------------------
# 上半部分：时间对比图
# ----------------------
x = np.arange(len(data["tasks"]))  # x轴位置
width = 0.4  # 柱状图宽度

# 绘制柱状图
rects1 = ax1.bar(x - width/2, data["serial_time"], width, label='Serial Time', color='#2ca02c', alpha=0.8)
rects2 = ax1.bar(x + width/2, data["parallel_time"], width, label='Parallel Time', color='#1f77b4', alpha=0.8)

# 坐标轴与标签
ax1.set_ylabel('Time (seconds)', fontsize=12)
ax1.set_title('Serial vs Parallel Performance', fontsize=14, pad=20)
ax1.set_xticks(x)
ax1.set_xticklabels([f"{t:.1e}" for t in data["tasks"]], fontsize=10)  # 科学计数法显示任务数
ax1.grid(axis='y', linestyle='--', alpha=0.7)
ax1.legend(fontsize=10)

# 在柱子上方添加数值
def add_labels(rects):
    for rect in rects:
        height = rect.get_height()
        ax1.annotate(f'{height:.2f}',
                     xy=(rect.get_x() + rect.get_width()/2, height),
                     xytext=(0, 3),  # 3点垂直偏移
                     textcoords="offset points",
                     ha='center', va='bottom', fontsize=9)
add_labels(rects1)
add_labels(rects2)

# ----------------------
# 下半部分：加速比折线图
# ----------------------
ax2.plot(x, data["speedup"], marker='o', linestyle='--', color='#ff7f0e', markersize=8, linewidth=2)
ax2.axhline(1, color='gray', linestyle='--', alpha=0.5)  # 基线

# 坐标轴与标签
ax2.set_xlabel('Number of Tasks', fontsize=12)
ax2.set_ylabel('Speedup Ratio', fontsize=12)
ax2.set_xticks(x)
ax2.set_xticklabels([f"{t:.1e}" for t in data["tasks"]], fontsize=10)
ax2.set_ylim(0, max(data["speedup"])*1.1)
ax2.grid(axis='y', linestyle='--', alpha=0.7)

# 在折线点上方添加数值
for i, speedup in enumerate(data["speedup"]):
    ax2.text(x[i], speedup + 0.2, f'{speedup:.1f}x', ha='center', va='bottom', fontsize=9)

# ----------------------
# 保存或显示图像
# ----------------------
plt.savefig('speedup_plot.png', dpi=300, bbox_inches='tight')  # 保存为高清图片
plt.show()
