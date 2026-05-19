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

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define kolorZielony "\033[32m"
#define kolorCzerwony "\033[31m"
#define kolorNiebieski "\033[34m"
#define kolorReset "\033[0m"

typedef enum {
    powlokaBASH,
    powlokaZSH,
    powlokaFISH,
    powlokaNIEZNANA
} TypPowloki;

typedef enum {
    operacjaDODAJ,
    operacjaUSUN
} TypOperacji;

typedef struct {
    TypOperacji operacja;
    char sciezka[PATH_MAX];
} Zmiana;

#define maksZmian 64
Zmiana zmiany[maksZmian];
int liczbaZmian = 0;

// --- output ---

void wypiszOk(const char *msg) {
    printf(kolorZielony "  [OK] %s" kolorReset "\n", msg);
}

void wypiszBlad(const char *msg) {
    printf(kolorCzerwony "  [!] %s" kolorReset "\n", msg);
}

void wypiszInfo(const char *msg) {
    printf(kolorNiebieski "  >> %s" kolorReset "\n", msg);
}

// --- wykrywanie powłoki ---

TypPowloki wykryjPowloke() {
    char *powloka = getenv("SHELL");
    if (powloka == NULL) return powlokaNIEZNANA;
    if (strstr(powloka, "bash") != NULL) return powlokaBASH;
    if (strstr(powloka, "zsh")  != NULL) return powlokaZSH;
    if (strstr(powloka, "fish") != NULL) return powlokaFISH;
    return powlokaNIEZNANA;
}

const char *nazwaPowloki(TypPowloki typ) {
    switch (typ) {
        case powlokaBASH: return "bash";
        case powlokaZSH:  return "zsh";
        case powlokaFISH: return "fish";
        default:          return "nieznana";
    }
}

// --- UI ---

void wypiszEpickieLogo(TypPowloki powloka) {
    printf(kolorZielony);
    printf("    /$$$$$$        /$$       /$$  /$$$$$$  /$$$$$$$             /$$     /$$\n");
    printf("   /$$__  $$      | $$      | $$ /$$__  $$| $$__  $$           | $$    | $$\n");
    printf("  | $$  | $$  /$$$$$$$  /$$$$$$$|__/  | $$| $$  | $$ /$$$$$$  /$$$$$$  | $$$$$$$\n");
    printf("  | $$$$$$$$ /$$__  $$ /$$__  $$  /$$$$$$/| $$$$$$$/|____  $$|_  $$_/  | $$__  $$\n");
    printf("  | $$__  $$| $$  | $$| $$  | $$ /$$____/ | $$____/  /$$$$$$$  | $$    | $$  | $$\n");
    printf("  | $$  | $$| $$  | $$| $$  | $$| $$      | $$      /$$__  $$  | $$ /$$| $$  | $$\n");
    printf("  | $$  | $$|  $$$$$$$|  $$$$$$$| $$$$$$$$| $$     |  $$$$$$$  |  $$$$/| $$  | $$\n");
    printf("  |__/  |__/ |_______/ |_______/|________/|__/      |_______/   |___/  |__/  |__/\n");
    printf("                                                            By Paweł Borusk\n");
    printf(kolorReset "\n");
    printf("  Witaj w add2path! Wykryta powłoka: %s\n\n", nazwaPowloki(powloka));
}

void wypiszMenu() {
    printf("============================================\n");
    printf("                    MENU\n");
    printf("============================================\n");
    printf("  1. Dodaj bieżący katalog do PATH\n");
    printf("  2. Usuń bieżący katalog z PATH\n");
    printf("  3. Dodaj katalog podany jako argument\n");
    printf("  4. Usuń katalog podany jako argument\n");
    printf("  5. Wypisz zmiany z bieżącej sesji\n");
    printf("  6. Usuń operację z kolejki\n");
    printf("  7. Zapisz zmiany i zrestartuj powłokę\n");
    printf("  8. Wyjdź\n");
    printf("============================================\n");
    printf("  Wybór: ");
    fflush(stdout);
}

// --- walidacja ---

int czyWKolejce(const char *sciezka, TypOperacji operacja) {
    for (int i = 0; i < liczbaZmian; i++) {
        if (zmiany[i].operacja == operacja && strcmp(zmiany[i].sciezka, sciezka) == 0)
            return 1;
    }
    return 0;
}

int czyWPath(const char *sciezka) {
    char *zmiennaPath = getenv("PATH");
    if (zmiennaPath == NULL) return 0;
    char *kopia = strdup(zmiennaPath);
    char *token = strtok(kopia, ":");
    int znaleziono = 0;
    while (token != NULL) {
        if (strcmp(token, sciezka) == 0) { znaleziono = 1; break; }
        token = strtok(NULL, ":");
    }
    free(kopia);
    return znaleziono;
}

// --- bufor zmian ---

void kolejkaDodaj(const char *sciezka) {
    wypiszInfo("Sprawdzanie czy katalog istnieje...");
    if (access(sciezka, F_OK) != 0) {
        wypiszBlad("Katalog nie istnieje.");
        return;
    }

    wypiszInfo("Sprawdzanie czy katalog jest już w PATH...");
    if (czyWPath(sciezka)) {
        wypiszBlad("Katalog jest już w PATH.");
        return;
    }
    wypiszInfo("Sprawdzanie czy operacja jest już zakolejkowana...");
    if (czyWKolejce(sciezka, operacjaDODAJ)) {
        wypiszBlad("Dodanie tego katalogu jest już zakolejkowane.");
        return;
    }
    if (liczbaZmian >= maksZmian) {
        wypiszBlad("Osiągnięto limit zmian w sesji (64).");
        return;
    }
    wypiszInfo("Dodawanie do kolejki zmian...");
    zmiany[liczbaZmian].operacja = operacjaDODAJ;
    strncpy(zmiany[liczbaZmian].sciezka, sciezka, PATH_MAX - 1);
    liczbaZmian++;

    char komunikat[PATH_MAX + 40];
    snprintf(komunikat, sizeof(komunikat), "Zakolejkowano dodanie: %s", sciezka);
    wypiszOk(komunikat);
}

void kolejkaUsun(const char *sciezka) {
    wypiszInfo("Sprawdzanie czy katalog jest w PATH...");
    if (!czyWPath(sciezka)) {
        wypiszBlad("Katalogu nie ma w PATH.");
        return;
    }
    wypiszInfo("Sprawdzanie czy operacja jest już zakolejkowana...");
    if (czyWKolejce(sciezka, operacjaUSUN)) {
        wypiszBlad("Usunięcie tego katalogu jest już zakolejkowane.");
        return;
    }
    if (liczbaZmian >= maksZmian) {
        wypiszBlad("Osiągnięto limit zmian w sesji (64).");
        return;
    }
    wypiszInfo("Dodawanie do kolejki zmian...");
    zmiany[liczbaZmian].operacja = operacjaUSUN;
    strncpy(zmiany[liczbaZmian].sciezka, sciezka, PATH_MAX - 1);
    liczbaZmian++;

    char komunikat[PATH_MAX + 40];
    snprintf(komunikat, sizeof(komunikat), "Zakolejkowano usunięcie: %s", sciezka);
    wypiszOk(komunikat);
}

void kolejkaUsunIndeks(int indeks) {
    for (int i = indeks; i < liczbaZmian - 1; i++)
        zmiany[i] = zmiany[i + 1];
    liczbaZmian--;
}

// --- zapis ---

int zapiszZmianyFish() {
    char *home = getenv("HOME");
    if (home == NULL) {
        wypiszBlad("Nie udało się ustalić HOME.");
        return 0;
    }

    // zapisz skrypt do pliku tymczasowego
    char sciezkaSkryptu[PATH_MAX];
    snprintf(sciezkaSkryptu, sizeof(sciezkaSkryptu), "%s/.cache/add2path_apply.fish", home);

    FILE *skrypt = fopen(sciezkaSkryptu, "w");
    if (skrypt == NULL) {
        wypiszBlad("Nie udało się utworzyć skryptu tymczasowego.");
        return 0;
    }

    for (int i = 0; i < liczbaZmian; i++) {
        if (zmiany[i].operacja == operacjaDODAJ) {
            fprintf(skrypt, "fish_add_path --universal '%s'\n", zmiany[i].sciezka);
        } else {
            fprintf(skrypt, "set -U fish_user_paths (string match -v '%s' $fish_user_paths)\n", zmiany[i].sciezka);
        }
    }

    fclose(skrypt);

    // wykonaj skrypt
    char polecenie[PATH_MAX + 64];
    snprintf(polecenie, sizeof(polecenie), "fish '%s'", sciezkaSkryptu);
    system(polecenie);
    remove(sciezkaSkryptu);
    return 1;
}

int zapiszZmiany(TypPowloki powloka) {
    if (liczbaZmian == 0) {
        wypiszBlad("Brak zmian do zapisania.");
        return 0;
    }
    switch (powloka) {
        case powlokaFISH: return zapiszZmianyFish();
        default:
            wypiszBlad("Obsługa tej powłoki jeszcze niezaimplementowana.");
            return 0;
    }
}

// --- opcje menu ---

void dodajBiezacy() {
    wypiszInfo("Pobieranie bieżącego katalogu...");
    char biezacyKatalog[PATH_MAX];
    if (getcwd(biezacyKatalog, sizeof(biezacyKatalog)) == NULL) {
        wypiszBlad("Nie udało się pobrać bieżącego katalogu.");
        return;
    }
    kolejkaDodaj(biezacyKatalog);
}

void usunBiezacy() {
    wypiszInfo("Pobieranie bieżącego katalogu...");
    char biezacyKatalog[PATH_MAX];
    if (getcwd(biezacyKatalog, sizeof(biezacyKatalog)) == NULL) {
        wypiszBlad("Nie udało się pobrać bieżącego katalogu.");
        return;
    }
    kolejkaUsun(biezacyKatalog);
}

void dodajArgument() {
    char sciezka[PATH_MAX];
    printf("  Podaj ścieżkę do dodania: ");
    fflush(stdout);
    if (scanf(" %s", sciezka) != 1) {
        wypiszBlad("Nie udało się odczytać ścieżki.");
        return;
    }
    if (sciezka[0] != '/') {
        wypiszBlad("Podana ścieżka jest względna.");
        printf("  Dodanie ścieżki względnej może nie działać poprawnie.\n");
        printf("  Czy chcesz kontynuować? (t/n): ");
        fflush(stdout);
        char potwierdzenie;
        scanf(" %c", &potwierdzenie);
        if (potwierdzenie != 't' && potwierdzenie != 'T') {
            printf("  Anulowano.\n");
            return;
        }
    }
    kolejkaDodaj(sciezka);
}

void usunArgument() {
    char sciezka[PATH_MAX];
    printf("  Podaj ścieżkę do usunięcia: ");
    fflush(stdout);
    if (scanf(" %s", sciezka) != 1) {
        wypiszBlad("Nie udało się odczytać ścieżki.");
        return;
    }
    kolejkaUsun(sciezka);
}

void wypiszZmiany() {
    if (liczbaZmian == 0) {
        printf("  Brak zmian w bieżącej sesji.\n");
        return;
    }
    printf("  Zmiany zakolejkowane w tej sesji:\n");
    printf("  ----------------------------------\n");
    for (int i = 0; i < liczbaZmian; i++) {
        const char *znacznik = zmiany[i].operacja == operacjaDODAJ
            ? kolorZielony "[+]" kolorReset
            : kolorCzerwony "[-]" kolorReset;
        printf("  %d. %s %s\n", i + 1, znacznik, zmiany[i].sciezka);
    }
    printf("  ----------------------------------\n");
    printf("  Łącznie: %d zmian\n", liczbaZmian);
}

void usunZKolejki() {
    if (liczbaZmian == 0) {
        wypiszBlad("Kolejka jest pusta, nie ma czego usuwać.");
        return;
    }

    wypiszZmiany();
    printf("\n  Podaj numer operacji do usunięcia (0 = anuluj): ");
    fflush(stdout);

    int numer;
    if (scanf("%d", &numer) != 1) {
        int znak;
        while ((znak = getchar()) != '\n' && znak != EOF);
        wypiszBlad("Nieprawidłowy numer.");
        return;
    }

    if (numer == 0) {
        printf("  Anulowano.\n");
        return;
    }

    if (numer < 1 || numer > liczbaZmian) {
        wypiszBlad("Numer poza zakresem.");
        return;
    }

    char komunikat[PATH_MAX + 40];
    snprintf(komunikat, sizeof(komunikat), "Usunięto z kolejki: %s", zmiany[numer - 1].sciezka);
    kolejkaUsunIndeks(numer - 1);
    wypiszOk(komunikat);
}

// --- main ---

int main(int argc, char *argv[]) {
    TypPowloki powloka = wykryjPowloke();
    wypiszEpickieLogo(powloka);
    char sciezkaAplikacji[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", sciezkaAplikacji, sizeof(sciezkaAplikacji) - 1);
    if (len != -1) sciezkaAplikacji[len] = '\0';
    else realpath(argv[0], sciezkaAplikacji); // fallback

    int dziala = 1;
    while (dziala) {
        wypiszMenu();

        int wybor;
        if (scanf("%d", &wybor) != 1) {
            int znak;
            while ((znak = getchar()) != '\n' && znak != EOF);
            wypiszBlad("Podaj liczbę od 1 do 8.");
            printf("\n");
            continue;
        }

        printf("\n");

        switch (wybor) {
            case 1: dodajBiezacy();  break;
            case 2: usunBiezacy();   break;
            case 3: dodajArgument(); break;
            case 4: usunArgument();  break;
            case 5: wypiszZmiany();  break;
            case 6: usunZKolejki();  break;

            case 7: {
                printf("  Czy na pewno chcesz zapisać zmiany i zrestartować powłokę? (t/n): ");
                fflush(stdout);
                char potwierdzenie;
                scanf(" %c", &potwierdzenie);
                printf("\n");
                if (potwierdzenie == 't' || potwierdzenie == 'T') {
                    wypiszInfo("Zapisywanie zmian...");
                    if (zapiszZmiany(powloka)) {
                        wypiszOk("Zmiany zapisane pomyślnie.");
                        wypiszInfo("Restartowanie powłoki...");
                        sleep(1); // daj fish czas na zapis do fish_variables
                        char *powlokaExec = getenv("SHELL");
                        system("cat ~/.config/fish/fish_variables | grep fish_user_paths");
                        char poleceniePonownego[PATH_MAX + 64];
                        snprintf(poleceniePonownego, sizeof(poleceniePonownego),
                            "set -e PATH; '%s'; exec fish -l", sciezkaAplikacji);
                        execlp(powlokaExec, powlokaExec, "-c", poleceniePonownego, NULL);
                        wypiszBlad("Nie udało się zrestartować powłoki.");
                    }
                } else {
                    printf("  Anulowano.\n");
                }
                break;
            }

            case 8: {
                if (liczbaZmian > 0)
                    printf("  Masz niezapisane zmiany! Czy na pewno chcesz wyjść? (t/n): ");
                else
                    printf("  Czy na pewno chcesz wyjść? (t/n): ");
                fflush(stdout);
                char potwierdzenie;
                scanf(" %c", &potwierdzenie);
                printf("\n");
                if (potwierdzenie == 't' || potwierdzenie == 'T') {
                    printf("  Do zobaczenia!\n");
                    dziala = 0;
                } else {
                    printf("  Anulowano.\n");
                }
                break;
            }

            default:
                wypiszBlad("Nieprawidłowy wybór. Podaj liczbę od 1 do 8.");
                break;
        }

        printf("\n");
    }

    return 0;
}