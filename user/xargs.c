#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUF_SIZE 512

// Hàm sao chép các đối số từ argv vào cmd, bỏ qua các tùy chọn '-n' và '1'
int copy_arguments(char *argv[], char *cmd[], int argc) {
    int n = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") != 0 && strcmp(argv[i], "1") != 0) {
            cmd[n++] = argv[i];
        }
    }
    cmd[n] = 0;  // Kết thúc chuỗi các đối số
    return n;    // Trả về số đối số đã sao chép
}

// Hàm đọc một dòng từ stdin và lưu vào buf
int read_line(char *buf, int size) {
    char *p = buf;
    memset(buf, 0, size);
    while (read(0, p, 1) == 1 && *p != '\n') {
        p++;
    }
    *p = 0; // Kết thúc chuỗi
    return (p != buf); // Trả về 1 nếu có dữ liệu, 0 nếu không
}

// Hàm thực thi lệnh với các đối số trong cmd
void execute_command(char *cmd[]) {
    if (fork() == 0) {
        exec(cmd[0], cmd);
        fprintf(2, "exec %s failed\n", cmd[0]);
        exit(1);
    }
    wait(0); // Chờ tiến trình con hoàn thành
}

int main(int argc, char *argv[]) {
    char buf[BUF_SIZE];
    char *cmd[MAXARG];

    // Kiểm tra đối số đầu vào và sao chép các đối số vào cmd
    if (argc < 2) {
        fprintf(2, "Usage: xargs [options] [command [initial-arguments]]\n");
        exit(1);
    }

    // Sao chép các đối số từ argv vào cmd
    int n = copy_arguments(argv, cmd, argc);

    // Vòng lặp đọc từng dòng từ stdin và thực thi lệnh
    while (read_line(buf, BUF_SIZE)) {
        cmd[n] = buf;
        cmd[n + 1] = 0;
        execute_command(cmd);  // Thực thi lệnh với đối số từ stdin
    }

    exit(0);
}
