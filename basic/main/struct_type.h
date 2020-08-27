//
// Created by binli on 2020/8/27.
//

// 方式一：声明的时候需要使用 struct 关键字，如：struct Book book = {...};
struct Book { // 总长度为 32
    char *name; // 长度为8
    char *author; // 长度为8
    char *subject; // 长度为8
    int id; // 长度为 4 + 空闲 4(与 8 对齐)
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
