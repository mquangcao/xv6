#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf(" [ERROR] => [Usage: find <path> <filename>]\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}


void find(char *path, char *filename) {
    char buf[512]; // path thu muc
    char *p;
    int fd; // file descriptor để mở thư mục và đọc nội dung thư mục
    struct dirent de; // 1 muc trong thu muc
    struct stat st; // thong tin ve tep hoac thu muc

    // Mở thư mục theo đường dẫn path ( 0 => readonly)
    if ((fd = open(path, 0)) < 0) {
        printf("find: cannot open %s\n", path);
        return;
    }

    // Lấy thông tin của thư mục path lưu vào st
    if (fstat(fd, &st) < 0) {
        printf("find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // Kiểm tra nếu không phải là thư mục thì dừng
    if (st.type != T_DIR) {
        printf("find: %s is not a directory\n", path);
        close(fd);
        return;
    }

    // Sao chép đường dẫn hiện tại vào buf
    strcpy(buf, path);
    p = buf + strlen(buf);
    // Them / vao cuoi
    *p++ = '/';

    // Duyệt qua từng mục trong thư mục
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        // Bỏ qua các thư mục đặc biệt "." và ".."
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        // Sao chép tên mục vào buf để tạo đường dẫn đầy đủ
        memmove(p, de.name, strlen(de.name));
        p[strlen(de.name)] = '\0';

        // Lấy thông tin của mục hiện tại
        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        // Kiểm tra nếu tên mục trùng với filename thì in ra
        if (strcmp(de.name, filename) == 0) {
            printf("%s\n", buf);
        }

        // Nếu là thư mục, đệ quy gọi hàm find
        if (st.type == T_DIR) {
            find(buf, filename);
        }
    }

    close(fd);
}

