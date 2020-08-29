//
// Created by binli on 2020/8/27.
//

#include <stdio.h>
#include "struct_type.h"

/**
 * 打印 Book 的详细内容
 */
void print_book(const struct Book *book) {
    // *book 为结构体指针
    printf("book %s{author: %s, subject: %s, id: %d}\n", book->name, book->author, book->subject, book->id);
}

void print_admin(const Admin *admin) {
    printf("admin %s, age %d\n", admin->name, admin->age);
}

void about_struct() {
    struct Book book = {"c/c++", "no one", "program language", 191};
    print_book(&book);
    Admin admin = {"binlee", 26};
    print_admin(&admin);

    printf("sizeof(struct Book) %lu\n", sizeof(struct Book));
    printf("sizeof(struct Admin) %lu\n", sizeof(Admin));
}
