//
// Created by binli on 2020/8/27.
//

#include <stdio.h>
#include "struct_type.h"

void print_admin(const Admin *admin) {
    printf("admin %s, age %d\n", admin->name, admin->age);
}

void *to_string(const struct Book *book) {
    // *book 为结构体指针
    const char *format = "book %s{author: %s, subject: %s, id: %d}";
    char temp[100];
    char *desc = temp;
    snprintf(desc, 100, format, book->name, book->author, book->subject, book->id);
    return (void *) desc;
}

void about_struct() {
    struct Book book = {
            "c/c++", "no one", "program language", 191,
            .toString = to_string(&book)
    };
    printf("book to string: %s\n", (char *) book.toString);
    Admin admin = {"binlee", 26};
    print_admin(&admin);

    printf("sizeof(struct Book) %lu\n", sizeof(struct Book));
    printf("sizeof(struct Admin) %lu\n", sizeof(Admin));
}
