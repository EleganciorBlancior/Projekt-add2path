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

// kolory
#define kolorZielony "\033[32m"
#define kolorCzerwony "\033[31m"
#define kolorReset "\033[0m"

// typy obsługiwanych powłok
typedef enum {
    powlokaBASH,
    powlokaZSH,
    powlokaFISH,
    powlokaNIEZNANA
} TypPowloki;

// typ operacji w buforze zmian
typedef enum {
    operacjaDODAJ,
    operacjaUSUN
} TypOperacji;

// pojedyncza zmiana w sesji
typedef struct {
    TypOperacji operacja;
    char sciezka[PATH_MAX];
} Zmiana;

// bufor zmian sesji
#define maksZmian 64
Zmiana zmiany[maksZmian];
int liczbaZmian = 0;

// kolorowe indykatory

void wypiszOk(const char *wiadomosc) {
    printf(kolorZielony "  [OK] %s" kolorReset "\n", wiadomosc);
}

void wypiszBlad(const char *wiadomosc) {
    printf(kolorCzerwony "  [!] %s" kolorReset "\n", wiadomosc);
}

void wypiszInfo(const char *wiadomosc) {
    printf("  >> %s\n", wiadomosc);
}

// wykrywanie powłoki

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

// ekran powitalny

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

// menu

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

// walidacja

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

// operacje na buforze

void kolejkaDodaj(const char *sciezka) {
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
    // przesuń wszystkie elementy po usuniętym o jedno miejsce w lewo
    for (int i = indeks; i < liczbaZmian - 1; i++) {
        zmiany[i] = zmiany[i + 1];
    }
    liczbaZmian--;
}

// zapisywanie zmian

int zapiszZmianyFish() {
    char *katalogDomowy = getenv("HOME");
    if (katalogDomowy == NULL) {
        wypiszBlad("Nie udało się ustalić katalogu domowego.");
        return 0;
    }

    char sciezkaKonfigu[PATH_MAX];
    snprintf(sciezkaKonfigu, sizeof(sciezkaKonfigu), "%s/.config/fish/config.fish", katalogDomowy);

    wypiszInfo("Otwieranie pliku konfiguracyjnego fish...");
    FILE *plik = fopen(sciezkaKonfigu, "a");
    if (plik == NULL) {
        wypiszBlad("Nie udało się otworzyć pliku config.fish.");
        return 0;
    }

    fprintf(plik, "\n# add2path\n");
    for (int i = 0; i < liczbaZmian; i++) {
        if (zmiany[i].operacja == operacjaDODAJ) {
            wypiszInfo("Zapisywanie wpisu fish_add_path...");
            fprintf(plik, "fish_add_path %s\n", zmiany[i].sciezka);
        } else {
            wypiszInfo("Zapisywanie wpisu usunięcia z PATH...");
            fprintf(plik, "set -e fish_user_paths[(contains -i %s $fish_user_paths)]\n", zmiany[i].sciezka);
        }
    }

    fclose(plik);
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

// opcje menu

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

// main

int main(int argc, char *argv[]) {
    char sciezkaAplikacji[PATH_MAX];
    realpath(argv[0], sciezkaAplikacji);

    TypPowloki powloka = wykryjPowloke();
    wypiszEpickieLogo(powloka);

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
            case 1:
                dodajBiezacy();
                break;

            case 2:
                usunBiezacy();
                break;

            case 3:
                dodajArgument();
                break;

            case 4:
                usunArgument();
                break;

            case 5:
                wypiszZmiany();
                break;

            case 6:
                usunZKolejki();
                break;

            case 7: {
                printf("  Czy na pewno chcesz zapisać zmiany i zrestartować powłokę? (t/n): ");
                fflush(stdout);
                char potwierdzenie;
                scanf(" %c", &potwierdzenie);
                printf("\n");
                if (potwierdzenie == 't' || potwierdzenie == 'T') {
                    wypiszInfo("Zapisywanie zmian do pliku konfiguracyjnego...");
                    if (zapiszZmiany(powloka)) {
                        wypiszOk("Zmiany zapisane pomyślnie.");
                        wypiszInfo("Restartowanie powłoki...");
                        char polecenie[PATH_MAX + 64];
                        snprintf(polecenie, sizeof(polecenie),
                            "source ~/.config/fish/config.fish && %s", sciezkaAplikacji);
                        execl("/usr/bin/fish", "fish", "-c", polecenie, NULL);
                        wypiszBlad("Nie udało się zrestartować powłoki.");
                    }
                } else {
                    printf("  Anulowano.\n");
                }
                break;
            }

            case 8: {
                if (liczbaZmian > 0) {
                    printf("  Masz niezapisane zmiany! Czy na pewno chcesz wyjść? Zmiany zostaną utracone. (t/n): ");
                } else {
                    printf("  Czy na pewno chcesz wyjść? (t/n): ");
                }
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