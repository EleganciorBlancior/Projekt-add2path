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

<<<<<<< HEAD
// odblokowanie funkcji linuxowych (readlink, realpath itp.) - musi być przed #include

#define _DEFAULT_SOURCE
#include <stdio.h>                                                              // printf, scanf, fopen, fclose, fprintf
#include <stdlib.h>                                                                      // getenv, system, free, remove
#include <string.h>                                                           // strcmp, strstr, strdup, strtok, strncpy
#include <unistd.h>                                                           // getcwd, access, execlp, readlink, sleep
#include <limits.h>                                                 // PATH_MAX <- maksymalna długość ścieżki w systemie

// kody kolorów ANSI, terminale interpretują \033[ jako początek kodu koloru

#define kolorZielony   "\033[32m"
#define kolorCzerwony  "\033[31m"
#define kolorNiebieski "\033[34m"
#define kolorReset     "\033[0m"                                                   // przywraca domyślny kolor terminala

// enum zamiast gołych liczb, łatwiej czytać powlokaBASH niż 0

=======
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
>>>>>>> origin/main
typedef enum {
    powlokaBASH,
    powlokaZSH,
    powlokaFISH,
    powlokaNIEZNANA
} TypPowloki;

<<<<<<< HEAD
// enum dla rodzaju operacji którą użytkownik chce zakolejkować

typedef enum {
    operacjaDODAJ,
    operacjaUSUN
} TypOperacji;

// struct grupuje powiązane dane, każda zmiana to operacja + ścieżka której dotyczy

typedef struct {
    TypOperacji operacja;
    char sciezka[PATH_MAX];
} Zmiana;

// globalna tablica zmian to bufor operacji zaplanowanych przez użytkownika
// zmiany są aplikowane dopiero przy opcji 7, dzięki czemu opcja 8 może je wszystkie porzucić

=======
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
>>>>>>> origin/main
#define maksZmian 64
Zmiana zmiany[maksZmian];
int liczbaZmian = 0;

<<<<<<< HEAD
// trzy funkcje do wypisywania komunikatów z kolorowym prefiksem (giga epic)

void wypiszOk(const char *msg) {
    printf(kolorZielony "  [OK] %s" kolorReset "\n", msg);
}

void wypiszBlad(const char *msg) {
    printf(kolorCzerwony "  [!] %s" kolorReset "\n", msg);
}

void wypiszInfo(const char *msg) {
    printf(kolorNiebieski "  >> %s" kolorReset "\n", msg);
}

// getenv pobiera wartość zmiennej środowiskowej $SHELL zawiera ścieżkę do aktywnej powłoki np. /usr/bin/fish
// strstr sprawdza czy jeden string zawiera się w drugim, zwraca NULL jeśli nie
=======
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
>>>>>>> origin/main

TypPowloki wykryjPowloke() {
    char *powloka = getenv("SHELL");
    if (powloka == NULL) return powlokaNIEZNANA;
    if (strstr(powloka, "bash") != NULL) return powlokaBASH;
    if (strstr(powloka, "zsh")  != NULL) return powlokaZSH;
    if (strstr(powloka, "fish") != NULL) return powlokaFISH;
    return powlokaNIEZNANA;
}

<<<<<<< HEAD
// zwraca nazwę powłoki jako tekst do wyświetlenia

=======
>>>>>>> origin/main
const char *nazwaPowloki(TypPowloki typ) {
    switch (typ) {
        case powlokaBASH: return "bash";
        case powlokaZSH:  return "zsh";
        case powlokaFISH: return "fish";
        default:          return "nieznana";
    }
}

<<<<<<< HEAD
// zwraca ścieżkę do pliku konfiguracyjnego danej powłoki względem katalogu domowego

const char *plikKonfiguracyjny(TypPowloki typ) {
    switch (typ) {
        case powlokaBASH: return "/.bashrc";
        case powlokaZSH:  return "/.zshrc";
        default:          return NULL;
    }
}

//funkcja do wypisywania epickiego logo epickiego projektu
//wypisuje naprawde epickie logo wygenerowane na stronie do tworzenia tekstu w stylu ASCII art

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
=======
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
>>>>>>> origin/main

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
<<<<<<< HEAD
    fflush(stdout);                               // bez fflush tekst może zostać w buforze i nie pojawić się na ekranie
}

// sprawdza czy ta sama operacja na tej samej ścieżce jest już w buforze
// strcmp zwraca 0 gdy stringi są identyczne
=======
    fflush(stdout);
}

// walidacja
>>>>>>> origin/main

int czyWKolejce(const char *sciezka, TypOperacji operacja) {
    for (int i = 0; i < liczbaZmian; i++) {
        if (zmiany[i].operacja == operacja && strcmp(zmiany[i].sciezka, sciezka) == 0)
            return 1;
    }
    return 0;
}

<<<<<<< HEAD
// sprawdza czy ścieżka jest aktualnie w zmiennej PATH
// PATH to lista katalogów oddzielonych dwukropkiem np. /usr/bin:/usr/local/bin:/home/pawelb/.local/bin
// strtok "tnie" string na kawałki po podanym separatorze, w tym przypadku po ":"

int czyWPath(const char *sciezka) {
    char *zmiennaPath = getenv("PATH");
    if (zmiennaPath == NULL) return 0;
    char *kopia = strdup(zmiennaPath);                       // strdup robi kopię bo strtok modyfikuje oryginalny string
    char *token = strtok(kopia, ":");                                                         // pierwszy katalog z PATH
    int znaleziono = 0;
    while (token != NULL) {
        if (strcmp(token, sciezka) == 0) { znaleziono = 1; break; }
        token = strtok(NULL, ":");                                        // NULL = kontynuuj cięcie tego samego stringa
    }
    free(kopia);                              // zwalniamy pamięć zaalokowaną przez strdup, bez tego mamy wyciek pamięci
    return znaleziono;
}

// zapisuje operację dodania do bufora i nic jeszcze nie zmienia w systemie

void kolejkaDodaj(const char *sciezka) {

    // ścieżki bezwzględne zaczynają się od / - sprawdzamy czy katalog faktycznie istnieje na dysku
    // ścieżki względne pomijamy - access szukałby względem bieżącego katalogu co nie ma sensu w PATH

    if (sciezka[0] == '/') {
        wypiszInfo("Sprawdzanie czy katalog istnieje...");

        // access z F_OK sprawdza czy ścieżka istnieje na dysku
        // zapobiegawczo zeby czasem nie dodalo jakiegos pliku zamiast katalogu bo fish jest glupi

        if (access(sciezka, F_OK) != 0) {
            wypiszBlad("Katalog nie istnieje.");
            return;
        }
    } else {
        wypiszBlad("Podana ścieżka jest względna - może nie działać poprawnie w PATH.");
        printf("  Czy chcesz kontynuować? (t/n): ");
        fflush(stdout);
        char potwierdzenie;
        scanf(" %c", &potwierdzenie);
        if (potwierdzenie != 't' && potwierdzenie != 'T') {
            printf("  Anulowano.\n");
            return;
        }
    }
=======
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
>>>>>>> origin/main
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
<<<<<<< HEAD

    // strncpy zamiast strcpy bo kopiuje max N znaków, bezpieczniejsze

    strncpy(zmiany[liczbaZmian].sciezka, sciezka, PATH_MAX - 1);
    liczbaZmian++;

    //dodatkowe 40 znaków jako margines bezpieczeństwa, w przypadku ścieżki mającej dokładnie 4096 znaków (przeważnie max długość ścieżki w systemie)
    //mogłoby spowodować że dopisanie informacji np. Zakolejkowano dodanie przepełni bufora, co może spowodować nadpisanie innej zmiennej czy adresu powrotu funkcji

    char komunikat[PATH_MAX + 40];

    // snprintf działa jak printf ale zapisuje do stringa

=======
    strncpy(zmiany[liczbaZmian].sciezka, sciezka, PATH_MAX - 1);
    liczbaZmian++;

    char komunikat[PATH_MAX + 40];
>>>>>>> origin/main
    snprintf(komunikat, sizeof(komunikat), "Zakolejkowano dodanie: %s", sciezka);
    wypiszOk(komunikat);
}

<<<<<<< HEAD
// zapisuje operację usunięcia do bufora i nic jeszcze nie zmienia w systemie

=======
>>>>>>> origin/main
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

<<<<<<< HEAD
// usuwa element z tablicy pod danym indeksem przez przesunięcie wszystkich późniejszych elementów o 1 w lewo
// np. [A, B, C, D] po usunięciu B (indeks 1) -> [A, C, D]

void kolejkaUsunIndeks(int indeks) {
    for (int i = indeks; i < liczbaZmian - 1; i++)
        zmiany[i] = zmiany[i + 1];
    liczbaZmian--;
}

// tworzy tymczasowy skrypt fish, wykonuje go przez system() i usuwa
// fish_add_path --universal zapisuje ścieżkę trwale w zmiennych fish (plik fish_variables)
// string match -v zwraca wszystkie elementy listy POZA podanym czyli go usuwa

int zapiszZmianyFish() {
    char *home = getenv("HOME");                                                        // katalog domowy np. /home/user
    if (home == NULL) {
        wypiszBlad("Nie udało się ustalić HOME.");
        return 0;
    }

    char sciezkaSkryptu[PATH_MAX];
    snprintf(sciezkaSkryptu, sizeof(sciezkaSkryptu), "%s/.cache/add2path_apply.fish", home);

    FILE *skrypt = fopen(sciezkaSkryptu, "w");                        // w - otwórz do zapisu, utwórz jeśli nie istnieje
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

    fclose(skrypt);                           // fclose jest konieczne - bez niego dane mogą nie zostać zapisane na dysk

    char polecenie[PATH_MAX + 64];
    snprintf(polecenie, sizeof(polecenie), "fish '%s'", sciezkaSkryptu);
    system(polecenie);                                                            // wykonuje podaną komendę w terminalu
    remove(sciezkaSkryptu);                                                        // usuwa plik tymczasowy po wykonaniu
    return 1;
}

// tworzy tymczasowy skrypt sh, wykonuje go przez system() i usuwa
// dodawanie: dopisuje export PATH="$PATH:/sciezka" na końcu .bashrc/.zshrc przez >> (append, nie nadpisuje)
// usuwanie: grep -v wypisuje wszystkie linie OPRÓCZ tych zawierających daną ścieżkę, wynik leci do pliku tymczasowego
//           który potem zastępuje oryginał przez mv, nie można czytać i pisać do tego samego pliku jednocześnie

int zapiszZmianyPowloka(TypPowloki powloka) {
    char *home = getenv("HOME");
    if (home == NULL) {
        wypiszBlad("Nie udało się ustalić HOME.");
        return 0;
    }

    // składamy pełną ścieżkę do pliku konfiguracyjnego np. /home/pawelb/.bashrc

    char sciezkaKonfigu[PATH_MAX];
    snprintf(sciezkaKonfigu, sizeof(sciezkaKonfigu), "%s%s", home, plikKonfiguracyjny(powloka));

    char sciezkaSkryptu[PATH_MAX];
    snprintf(sciezkaSkryptu, sizeof(sciezkaSkryptu), "%s/.cache/add2path_apply.sh", home);

    FILE *skrypt = fopen(sciezkaSkryptu, "w");                        // w - otwórz do zapisu, utwórz jeśli nie istnieje
    if (skrypt == NULL) {
        wypiszBlad("Nie udało się utworzyć skryptu tymczasowego.");
        return 0;
    }

    fprintf(skrypt, "#!/bin/sh\n");

    for (int i = 0; i < liczbaZmian; i++) {
        if (zmiany[i].operacja == operacjaDODAJ) {
            fprintf(skrypt, "echo 'export PATH=\"$PATH:%s\"' >> '%s'\n",

            // >> dopisuje na koniec pliku zamiast go nadpisywać

                zmiany[i].sciezka, sciezkaKonfigu);
        } else {

            // grep -v zwraca wszystkie linie OPRÓCZ pasujących do wzorca - czyli usuwa wpis ze ścieżką
            // wynik zapisujemy do .tmp, potem mv zastępuje oryginał - nie można pisać i czytać tego samego pliku

            fprintf(skrypt, "grep -v 'PATH.*%s' '%s' > '%s.tmp' && mv '%s.tmp' '%s'\n",
                zmiany[i].sciezka, sciezkaKonfigu, sciezkaKonfigu, sciezkaKonfigu, sciezkaKonfigu);
        }
    }

    fclose(skrypt);                           // fclose jest konieczne - bez niego dane mogą nie zostać zapisane na dysk

    // nadajemy skryptowi uprawnienia do wykonania, bez tego system odmówi uruchomienia
    // chmod 700 = tylko właściciel może czytać, pisać i wykonywać

    char polecenie[PATH_MAX * 2 + 64];                     // dwa razy PATH_MAX bo sciezkaSkryptu pojawia się dwukrotnie
    snprintf(polecenie, sizeof(polecenie), "chmod 700 '%s' && sh '%s'", sciezkaSkryptu, sciezkaSkryptu);
    system(polecenie);                                                            // wykonuje podaną komendę w terminalu
    remove(sciezkaSkryptu);                                                        // usuwa plik tymczasowy po wykonaniu
    return 1;
}

// wywołuje odpowiednią funkcję zapisu zależnie od wykrytej powłoki

=======
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

>>>>>>> origin/main
int zapiszZmiany(TypPowloki powloka) {
    if (liczbaZmian == 0) {
        wypiszBlad("Brak zmian do zapisania.");
        return 0;
    }
    switch (powloka) {
        case powlokaFISH: return zapiszZmianyFish();
<<<<<<< HEAD
        case powlokaBASH: return zapiszZmianyPowloka(powlokaBASH);
        case powlokaZSH:  return zapiszZmianyPowloka(powlokaZSH);
=======
>>>>>>> origin/main
        default:
            wypiszBlad("Obsługa tej powłoki jeszcze niezaimplementowana.");
            return 0;
    }
}

<<<<<<< HEAD
// getcwd odpowiednik komendy pwd - zwraca katalog w którym aktualnie jesteśmy
=======
// opcje menu
>>>>>>> origin/main

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

<<<<<<< HEAD
// ścieżki bezwzględne zaczynają się od / np. /home/pawelb/projekty
// ścieżki względne od czegoś innego np. ./projekty - w PATH powinny być tylko bezwzględne ale add2path wspiera obydwa typy

=======
>>>>>>> origin/main
void dodajArgument() {
    char sciezka[PATH_MAX];
    printf("  Podaj ścieżkę do dodania: ");
    fflush(stdout);
    if (scanf(" %s", sciezka) != 1) {
        wypiszBlad("Nie udało się odczytać ścieżki.");
        return;
    }
<<<<<<< HEAD
=======
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
>>>>>>> origin/main
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

<<<<<<< HEAD
// operator trójkowy: warunek ? wartość_gdy_prawda : wartość_gdy_fałsz
// użyty do wyboru koloru znacznika [+] lub [-] zależnie od typu operacji

=======
>>>>>>> origin/main
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
<<<<<<< HEAD
=======

>>>>>>> origin/main
    wypiszZmiany();
    printf("\n  Podaj numer operacji do usunięcia (0 = anuluj): ");
    fflush(stdout);

    int numer;
    if (scanf("%d", &numer) != 1) {
        int znak;
<<<<<<< HEAD
        while ((znak = getchar()) != '\n' && znak != EOF);                   // czyścimy bufor wejścia po błędnym wpisie
=======
        while ((znak = getchar()) != '\n' && znak != EOF);
>>>>>>> origin/main
        wypiszBlad("Nieprawidłowy numer.");
        return;
    }

<<<<<<< HEAD
    if (numer == 0) { printf("  Anulowano.\n"); return; }
=======
    if (numer == 0) {
        printf("  Anulowano.\n");
        return;
    }
>>>>>>> origin/main

    if (numer < 1 || numer > liczbaZmian) {
        wypiszBlad("Numer poza zakresem.");
        return;
    }

    char komunikat[PATH_MAX + 40];
    snprintf(komunikat, sizeof(komunikat), "Usunięto z kolejki: %s", zmiany[numer - 1].sciezka);
<<<<<<< HEAD
    kolejkaUsunIndeks(numer - 1);                          // -1 bo tablica jest indeksowana od 0, a menu wyświetla od 1
    wypiszOk(komunikat);
}

// argc = liczba argumentów z linii poleceń, argv = tablica tych argumentów
// argv[0] to zawsze ścieżka do programu np. "./add2path"

int main(int argc, char *argv[]) {
    TypPowloki powloka = wykryjPowloke();
    wypiszEpickieLogo(powloka);

    // /proc/self/exe to specjalny plik linuxa, konkretniej dowiązanie symboliczne do aktualnie wykonywanego programu
    // readlink odczytuje dokąd wskazuje dowiązanie i daje nam pewną absolutną ścieżkę do własnej binarki
    // potrzebujemy jej żeby po restarcie powłoki program uruchomił się ponownie z właściwego miejsca

    char sciezkaAplikacji[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", sciezkaAplikacji, sizeof(sciezkaAplikacji) - 1);
    if (len != -1) sciezkaAplikacji[len] = '\0';           // readlink nie dodaje \0 na końcu stringa, robimy to ręcznie
    else realpath(argv[0], sciezkaAplikacji);                                        // fallback gdyby readlink zawiodło

    int dziala = 1;                                                           // flaga pętli głównej, 0 kończy działanie
    while (dziala) {
        wypiszMenu();

        int wybor;
        if (scanf("%d", &wybor) != 1) {
            int znak;
            while ((znak = getchar()) != '\n' && znak != EOF);                       // czyścimy bufor po błędnym wpisie
=======
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
>>>>>>> origin/main
            wypiszBlad("Podaj liczbę od 1 do 8.");
            printf("\n");
            continue;
        }

        printf("\n");
<<<<<<< HEAD

        switch (wybor) {
            case 1: dodajBiezacy();  break;
            case 2: usunBiezacy();   break;
            case 3: dodajArgument(); break;
            case 4: usunArgument();  break;
            case 5: wypiszZmiany();  break;
            case 6: usunZKolejki();  break;

=======

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

>>>>>>> origin/main
            case 7: {
                printf("  Czy na pewno chcesz zapisać zmiany i zrestartować powłokę? (t/n): ");
                fflush(stdout);
                char potwierdzenie;
                scanf(" %c", &potwierdzenie);
                printf("\n");
                if (potwierdzenie == 't' || potwierdzenie == 'T') {
<<<<<<< HEAD
                    wypiszInfo("Zapisywanie zmian...");
                    if (zapiszZmiany(powloka)) {
                        wypiszOk("Zmiany zapisane pomyślnie.");
                        wypiszInfo("Restartowanie powłoki...");

                        // czekamy sekundę żeby fish zdążył zapisać fish_variables przed restartem
                        // o dziwo naprawiło to 50% problemów

                        sleep(1);
                        char *powlokaExec = getenv("SHELL");
                        char poleceniePonownego[PATH_MAX * 2 + 128];

                        // dla fish czyścimy PATH żeby wczytał go na nowo z fish_variables
                        // dla bash/zsh resetujemy PATH do minimalnego systemowego przed startem nowej sesji
                        // dzięki temu .bashrc wczyta ścieżki od zera bez duplikatów z poprzedniej sesji

                        if (powloka == powlokaFISH) {
                            snprintf(poleceniePonownego, sizeof(poleceniePonownego),
                                "set -e PATH; '%s'; exec fish -l", sciezkaAplikacji);
                        } else if (powloka == powlokaBASH) {
                            snprintf(poleceniePonownego, sizeof(poleceniePonownego),
                                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin /bin/bash -lc '\"%s\"; exec /bin/bash -l'", sciezkaAplikacji);
                        } else if (powloka == powlokaZSH) {
                            snprintf(poleceniePonownego, sizeof(poleceniePonownego),
                                "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin /bin/zsh -lc '\"%s\"; exec /bin/zsh -l'", sciezkaAplikacji);
                        }
                        execlp(powlokaExec, powlokaExec, "-c", poleceniePonownego, NULL);

                        // execlp zastępuje bieżący proces nowym a stary przestaje istnieć
                        // uruchamiamy powłokę która najpierw startuje naszą aplikację,
                        // a po jej zamknięciu odpala interaktywną sesję fish z nowym $PATH
                        // jeśli execlp się powiedzie, kod poniżej nigdy się nie wykona

                        wypiszBlad("Nie udało się zrestartować powłoki.");           // dotarcie tutaj = execlp zawiodło
=======
                    wypiszInfo("Zapisywanie zmian do pliku konfiguracyjnego...");
                    if (zapiszZmiany(powloka)) {
                        wypiszOk("Zmiany zapisane pomyślnie.");
                        wypiszInfo("Restartowanie powłoki...");
                        char polecenie[PATH_MAX + 64];
                        snprintf(polecenie, sizeof(polecenie),
                            "source ~/.config/fish/config.fish && %s", sciezkaAplikacji);
                        execl("/usr/bin/fish", "fish", "-c", polecenie, NULL);
                        wypiszBlad("Nie udało się zrestartować powłoki.");
>>>>>>> origin/main
                    }
                } else {
                    printf("  Anulowano.\n");
                }
                break;
            }

            case 8: {
<<<<<<< HEAD
                if (liczbaZmian > 0)
                    printf("  Masz niezapisane zmiany! Czy na pewno chcesz wyjść? (t/n): ");
                else
                    printf("  Czy na pewno chcesz wyjść? (t/n): ");
=======
                if (liczbaZmian > 0) {
                    printf("  Masz niezapisane zmiany! Czy na pewno chcesz wyjść? Zmiany zostaną utracone. (t/n): ");
                } else {
                    printf("  Czy na pewno chcesz wyjść? (t/n): ");
                }
>>>>>>> origin/main
                fflush(stdout);
                char potwierdzenie;
                scanf(" %c", &potwierdzenie);
                printf("\n");
                if (potwierdzenie == 't' || potwierdzenie == 'T') {
                    printf("  Do zobaczenia!\n");
<<<<<<< HEAD
                    dziala = 0;                                     // wyjście z pętli głównej, program kończy działanie
=======
                    dziala = 0;
>>>>>>> origin/main
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