//
// Created by binli on 2020/8/27.
//

// 方式一：声明的时候需要使用 struct 关键字，如：struct Book book = {...};
struct Book { // 总长度为 32
  char *name; // 长度为8
  char *author; // 长度为8
  char *subject; // 长度为8
  int id; // 长度为 4 + 空闲 4(与 8 对齐)
  // 打印 Book 的详细内容
  char *(*toString)(struct Book book);
};

// 方式二: 声明的时候不需要使用 struct 关键字，如: Admin admin = {...};
typedef struct { // 总长度为 16
  char *name; // 长度为8
  int age; // 长度为 4 + 空闲 4(与 8 对齐)
} Admin;

/**
 * 结构体: struct
 */
void about_struct();


// 共用一块内存，每次只能使用一个属性
// 总大小取决于属性所占用的最大空间
union person {
  char age; // 2 * 4 = 8
  long time; // 8 * 4 = 32
};

/**
 * 共用体：union
 */
void about_union();
