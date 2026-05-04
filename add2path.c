/*
  /$$$$$$        /$$       /$$  /$$$$$$  /$$$$$$$             /$$     /$$
 /$$__  $$      | $$      | $$ /$$__  $$| $$__  $$           | $$    | $$
| $$  \ $$  /$$$$$$$  /$$$$$$$|__/  \ $$| $$  \ $$ /$$$$$$  /$$$$$$  | $$$$$$$
| $$$$$$$$ /$$__  $$ /$$__  $$  /$$$$$$/| $$$$$$$/|____  $$|_  $$_/  | $$__  $$
| $$__  $$| $$  | $$| $$  | $$ /$$____/ | $$____/  /$$$$$$$  | $$    | $$  \ $$
| $$  | $$| $$  | $$| $$  | $$| $$      | $$      /$$__  $$  | $$ /$$| $$  | $$
| $$  | $$|  $$$$$$$|  $$$$$$$| $$$$$$$$| $$     |  $$$$$$$  |  $$$$/| $$  | $$
|__/  |__/ \_______/ \_______/|________/|__/      \_______/   \___/  |__/  |__/
                                                            By Paweł Borusk
 */

#define _BSD_SOURCE // clion sie pruje o PATH_MAX itp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

// typy obsługiwanych powłok
typedef enum {
    SHELL_BASH,
    SHELL_ZSH,
    SHELL_FISH,
    SHELL_UNKNOWN
} ShellType;

//wykrywanie powłoki
ShellType detect_shell() {
    char *shell = getenv("SHELL");
    if (shell == NULL) return SHELL_UNKNOWN;

    if (strstr(shell, "bash") != NULL) return SHELL_BASH;
    if (strstr(shell, "zsh")  != NULL) return SHELL_ZSH;
    if (strstr(shell, "fish") != NULL) return SHELL_FISH;

    return SHELL_UNKNOWN;
}

const char *shell_name(ShellType type) {
    switch (type) {
        case SHELL_BASH:    return "bash";
        case SHELL_ZSH:     return "zsh";
        case SHELL_FISH:    return "fish";
        default:            return "nieznana";
    }
}

//ekran powitalny (początkowo miał być efekt linijka po linijce, ale średnio wyglądało)
void print_welcome(ShellType shell) {
    const char *lines[] = {
    "    /$$$$$$        /$$       /$$  /$$$$$$  /$$$$$$$             /$$     /$$",
    "   /$$__  $$      | $$      | $$ /$$__  $$| $$__  $$           | $$    | $$",
    "  | $$  | $$  /$$$$$$$  /$$$$$$$|__/  | $$| $$  | $$ /$$$$$$  /$$$$$$  | $$$$$$$",
    "  | $$$$$$$$ /$$__  $$ /$$__  $$  /$$$$$$/| $$$$$$$/|____  $$|_  $$_/  | $$__  $$",
    "  | $$__  $$| $$  | $$| $$  | $$ /$$____/ | $$____/  /$$$$$$$  | $$    | $$  | $$",
    "  | $$  | $$| $$  | $$| $$  | $$| $$      | $$      /$$__  $$  | $$ /$$| $$  | $$",
    "  | $$  | $$|  $$$$$$$|  $$$$$$$| $$$$$$$$| $$     |  $$$$$$$  |  $$$$/| $$  | $$",
    "  |__/  |__/ |_______/ |_______/|________/|__/      |_______/   |___/  |__/  |__/",
    "                                                              By Paweł Borusk",
    };

    int n = sizeof(lines) / sizeof(lines[0]);
    for (int i = 0; i < n; i++) {
        printf("%s\n", lines[i]);
    }

    printf("\n");
    printf("  Witaj w add2path! Wykryta powloka: %s\n", shell_name(shell));
    printf("\n");
}

/* ── menu ────────────────────────────────────────────────────────────────── */
void print_menu() {
    printf("============================================\n");
    printf("                    MENU\n");
    printf("============================================\n");
    printf("  1. Dodaj biezacy katalog do PATH\n");
    printf("  2. Usun biezacy katalog z PATH\n");
    printf("  3. Dodaj katalog podany jako argument\n");
    printf("  4. Usun katalog podany jako argument\n");
    printf("  5. Wypisz zmiany z biezacej sesji\n");
    printf("  6. Zapisz zmiany i wyjdz\n");
    printf("  7. Wyjdz bez zapisywania\n");
    printf("============================================\n");
    printf("  Wybor: ");
    fflush(stdout);
}

//main
int main(int argc, char *argv[]) {
    //zapamietywanie sciezki w razie restartu
    char app_path[PATH_MAX];
    realpath(argv[0], app_path);

    ShellType shell = detect_shell();

    print_welcome(shell);

    int running = 1;
    while (running) {
        print_menu();

        int choice;
        if (scanf("%d", &choice) != 1) {
            //czyszczenie bufora przy złym wejściu
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("\n  [!] Podaj liczbe od 1 do 7.\n\n");
            continue;
        }

        printf("\n");
        //obsługa opcji
        switch (choice) {
            case 1:
                printf("  [TODO] Dodawanie biezacego katalogu...\n");
                break;

            case 2:
                printf("  [TODO] Usuwanie biezacego katalogu...\n");
                break;

            case 3:
                printf("  [TODO] Dodawanie katalogu z argumentu...\n");
                break;

            case 4:
                printf("  [TODO] Usuwanie katalogu z argumentu...\n");
                break;

            case 5:
                printf("  [TODO] Wypisywanie zmian z sesji...\n");
                break;

            case 6: {
                printf("  Czy na pewno chcesz zapisac zmiany i wyjsc? (t/n): ");
                fflush(stdout);
                char confirm;
                scanf(" %c", &confirm);
                if (confirm == 't' || confirm == 'T') {
                    printf("\n  [TODO] Zapisywanie zmian...\n");
                    printf("  Zmiany zapisane. Do zobaczenia!\n\n");
                    running = 0;
                } else {
                    printf("  Anulowano.\n");
                }
                break;
            }

            case 7: {
                printf("  Czy na pewno chcesz wyjsc BEZ zapisywania? Wszystkie zmiany z sesji zostana cofniete. (t/n): ");
                fflush(stdout);
                char confirm;
                scanf(" %c", &confirm);
                if (confirm == 't' || confirm == 'T') {
                    printf("\n  Wychodzenie bez zapisywania. Do zobaczenia!\n\n");
                    running = 0;
                } else {
                    printf("  Anulowano.\n");
                }
                break;
            }

            default:
                printf("  [!] Nieprawidlowy wybor. Podaj liczbe od 1 do 7.\n");
                break;
        }

        printf("\n");
    }

    return 0;
}