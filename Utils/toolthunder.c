#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>

void show_help() {
    printf("────────────────────────────────────────────────\n");
    printf("\033[1;34m# Welcome to ToolThunder -- Multi Call Binary:\033[0m\n");
    printf("────────────────────────────────────────────────\n");
    printf("\033[1mUsage:\033[0m\n");
    printf("  \033[1;32mtoolthunder clear\033[0m          → Clear screen\n");
    printf("  \033[1;32mtoolthunder help\033[0m           → Show help\n");
    printf("  \033[1;32mtoolthunder author\033[0m         → Developer info\n");
    printf("  \033[1;32mtoolthunder info\033[0m           → Module info\n");
    printf("  \033[1;32mtoolthunder --version\033[0m       → Show version\n");
    printf("  \033[1;32mtoolthunder --install -s <dir>\033[0m    → Symlink install\n");
    printf("  \033[1;32mtoolthunder --remove -s <dir>\033[0m     → Remove symlinks\n");
    printf("  \033[1;32mtoolthunder --status --link <dir>\033[0m → Check symlink status\n");
    printf("────────────────────────────────────────────────\n");
}

void show_author() {
    puts("\033[1;33m👨‍💻 Pengembang: Sahrul\033[0m");
    puts("\033[1m🔧 Thunder Linux Power Edition\033[0m");
}

void show_info() {
    puts("\033[1;36m📦 toolthunder v2.2\033[0m");
    puts("Modular • Symlink-aware • CLI Toolkit");
}

void show_version() {
    puts("\033[1;36m🔧 toolthunder version 2.2\033[0m");
    puts("Created by Sahrul • Thunder Linux");
}

void clear_screen() {
    printf("\033c");
}

void install_symlinks(const char *mode, const char *dir) {
    const char *names[] = {"clear", "help", "author", "info"};
    char command[256];
    for (int i = 0; i < 4; ++i) {
        snprintf(command, sizeof(command),
                 "ln %s toolthunder %s/%s", mode, dir, names[i]);
        int res = system(command);
        if (res == 0)
            printf("\033[1;32m✅ Linked:\033[0m %s/%s\n", dir, names[i]);
        else
            fprintf(stderr, "\033[1;31m❌ Gagal:\033[0m %s/%s\n", dir, names[i]);
    }
}

void remove_symlinks(const char *mode, const char *dir) {
    const char *names[] = {"clear", "help", "author", "info"};
    char command[256];
    for (int i = 0; i < 4; ++i) {
        snprintf(command, sizeof(command),
                 (strcmp(mode, "-sf") == 0) ? "rm -f %s/%s" : "rm %s/%s",
                 dir, names[i]);
        int res = system(command);
        if (res == 0)
            printf("\033[1;33m🗑️ Removed:\033[0m %s/%s\n", dir, names[i]);
        else
            fprintf(stderr, "\033[1;31m❌ Gagal Hapus:\033[0m %s/%s\n", dir, names[i]);
    }
}

void check_symlinks(const char *dir) {
    const char *names[] = {"clear", "help", "author", "info"};
    char path[256], resolved[256];
    struct stat st;
    for (int i = 0; i < 4; ++i) {
        snprintf(path, sizeof(path), "%s/%s", dir, names[i]);
        if (lstat(path, &st) == 0 && S_ISLNK(st.st_mode)) {
            ssize_t len = readlink(path, resolved, sizeof(resolved) - 1);
            resolved[len] = '\0';
            if (access(resolved, F_OK) == 0)
                printf("\033[1;32m✔ Symlink OK:\033[0m %s → %s\n", path, resolved);
            else
                printf("\033[1;31m⚠ Broken Link:\033[0m %s → %s\n", path, resolved);
        } else {
            printf("\033[1;31m✘ Tidak Ada atau bukan symlink:\033[0m %s\n", path);
        }
    }
}

void dispatch_command(const char *cmd) {
    if (strcmp(cmd, "clear") == 0) clear_screen();
    else if (strcmp(cmd, "help") == 0) show_help();
    else if (strcmp(cmd, "author") == 0) show_author();
    else if (strcmp(cmd, "info") == 0) show_info();
    else {
        fprintf(stderr, "\033[1;31m❓ Unknown command:\033[0m %s\n", cmd);
        show_help();
        exit(1);
    }
}

int main(int argc, char **argv) {
    const char *alias = basename(argv[0]);

    if (argc >= 2 && strcmp(alias, "toolthunder") == 0) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "help") == 0) return show_help(), 0;
        if (strcmp(argv[1], "--author") == 0 || strcmp(argv[1], "author") == 0) return show_author(), 0;
        if (strcmp(argv[1], "--info") == 0 || strcmp(argv[1], "info") == 0) return show_info(), 0;
        if (strcmp(argv[1], "clear") == 0) return clear_screen(), 0;
        if (strcmp(argv[1], "--version") == 0) return show_version(), 0;

        // --install / --link
        if ((strcmp(argv[1], "--install") == 0 || strcmp(argv[1], "--link") == 0) && argc >= 4) {
            const char *mode = argv[2], *dir = argv[3];
            if (strcmp(mode, "-s") == 0 || strcmp(mode, "-sf") == 0)
                return install_symlinks(mode, dir), 0;
            else
                fprintf(stderr, "\033[1;31m⚠️ Invalid mode for symlink: %s\033[0m\n", mode);
            return 1;
        }

        // --remove
        if (strcmp(argv[1], "--remove") == 0 && argc >= 4) {
            const char *mode = argv[2], *dir = argv[3];
            if (strcmp(mode, "-s") == 0 || strcmp(mode, "-sf") == 0)
                return remove_symlinks(mode, dir), 0;
            else
                fprintf(stderr, "\033[1;31m⚠️ Invalid mode for remove: %s\033[0m\n", mode);
            return 1;
        }

        // --status --link
        if (strcmp(argv[1], "--status") == 0 && argc >= 4 && strcmp(argv[2], "--link") == 0) {
            const char *dir = argv[3];
            check_symlinks(dir);
            return 0;
        }

        fprintf(stderr, "\033[1;31m❌ Unknown subcommand:\033[0m %s\n", argv[1]);
        show_help();
        return 1;
    }

    dispatch_command(alias);
    return 0;
}
