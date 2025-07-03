#### lab2：

新增文件：

- `md5.h`
  - 新增文件`md5_SIMD.h`，包含了对部分函数的覆写
- `md5.cpp`
  - 新增文件`md5_SIMD.cpp`，包含原来的`md5`函数和SIMD处理后的`md5`函数

修改文件：

- `correctness.cpp`：修改了头文件，添加了`md5`和`md5_SIMD`函数执行结果的对比
- `main.cpp`：添加了`md5_SIMD`函数的调用



#### lab3：

新增文件：
- `PCFG.h`
  - 新增文件`PCFG_multi.h`，包含了`PriorityQueue_pthread`和`PriorityQueue_openmp`两个子类的定义
- `guessing.cpp`
  - 新增文件`guessing_multi.cpp`，包含了具体函数的实现

修改文件：

- `correctness_guessing.cpp`
- `PCFG.h`：将`Generate`函数类型改为`virtual`，这样在子类覆写时就能实现多态

`guessing_multi.cpp`中保留了原来函数的语法，并在子类中用openmp和pthread覆写了原来的`Generate`的函数，不会与父类互相冲突。

因此在测试时只要把`correctness_guessing.cpp`中20行左右`    PriorityQueue q;    `中`q`的类型改为`PriorityQueue_pthread`或`PriorityQueue_openmp`就行。