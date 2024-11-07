/*

Projekt 1:

Policz wartosci funkcji 𝑦 = 𝑓(𝑥) we wszystkich punktach podzialu na n czesci przedziału [a, b].
Funkcja f dana jest w postaci rozwiniecia w szereg potęgowy i w postaci wzoru analitycznego.
Obliczanie sumy szeregu wykonaj z dokladnoscia 𝜀. Algorytm obliczania sumy szeregu zapisz w
oddzielnej funkcji.
Uzupelnij funkcje obliczajaca sume szeregu tak, by sumowanych bylo co najwyżej M wyrazow
szeregu. Oznacza to, ze przerwanie sumowania moze nastapic rowniez wtedy, gdy nie zostala
osiagnieta zadana dokladnosc. Informacja o tym, czy zostala osiagnieta dokladnosc czy tez nie
winna byc znana w funkcji main().
Uzupelnij funkcje obliczajaca sume szeregu tak, by w funkcji main() znana była dodatkowo
liczba sumowanych wyrazow szeregu.

f(x) = (1+x)^(-1/2.0)

Kody wyjscia programu:
0 - program zostal wyegzekwowany poprawnie
1 - problem z funkcja scanf, nieprawidlowa liczba wprowadzonych elementow
2 - wprowadzono ilosc czesci przedzialu jako liczba mniejsza lub rowna zero
3 - nie udalo sie otworzyc pliku
4 - wartosci a & b nie zawieraja sie w przedziale od (-1;1)
5 - dokladnosc lub M sa mniejsze lub rowne zero
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// funkcja do obliczania sumy szeregu dla danego punktu
double suma_szeregu(double x, double dokladnosc, int M, int *warunek_stopu, int *liczba_wyrazow) {
    double suma = 1.0, wartosc = 1.0;
    *liczba_wyrazow = 1;
    *warunek_stopu = 0;
    /* 
    y - przyda nam sie do obliczania dalszych liczb szeregu
    petla wykonuje sie do momentu obliczenia maksymalnej ilosci liczb szeregu lub az osiagnie zadana dokladnosc
    */
    for (int y = 1; y<M; y++) {
        wartosc *= (2 * y - 1) * x / (2.0 * y);
        suma += (pow(-1,y) * wartosc);
        *liczba_wyrazow += 1;
        if(fabs(wartosc) <= dokladnosc) {
            *warunek_stopu = 1;
            break;
        }
    }

    return suma;
}
// czesc analityczna 
double fanalityczna(double x) {
    return pow(1+x, -1/2.0);
}

int main(void) {


    double a = 0.0, b = 0.0, dokladnosc = 0.0, krok = 0.0; 
    int n_czesci = 0, M = 0, warunek_stopu = 0, liczba_wyrazow = 0;

    
    printf("Podaj prosze przedzial [a,b] {a, b ∈ (-1,1)}, ilosc czesci przedzialu, dokladnosc epsilon oraz maksymalna ilosc sumowanych wyrazow: \n");

    if (scanf("%lf %lf %d %lf %d", &a, &b, &n_czesci, &dokladnosc, &M) != 5) {
        printf("Blad wprowadzenia danych");
        exit(1);
    }

    if (fabs(a) >= 1 || fabs(b) >= 1) {
        printf("Wartosci a i b musza nalezec do przedzialu (-1;1)");
        exit(4);
    }

    if(dokladnosc <= 0 || M <= 0) {
        printf("Dokladnosc oraz M nie moga byc ujemne lub rowne zero");
        exit(5);
    }

    if (n_czesci <= 0) {
        printf("Wprowadzono niepoprawna ilosc czesci przedzialu");
        exit(2);
    }
    else {
        krok = (b-a)/n_czesci;
    }

    FILE *plik = fopen("wyniki.txt", "w");
    if(!plik) {
        printf("Nie udalo sie otworzyc pliku");
        exit(3);
    }

    fprintf(plik, "\tx\tf_szereg(x)\tf_scisle(x)\tliczba_wyrazow\t\twarunek_stopu\n");
    //funkcja "kroczaca" po podanym przedziale
    for (int i = 0; i <= n_czesci; i++) {
        //okreslenie x i przesuwanie go o krok
        double x = a + i * krok;
        double wartosc_dokladna = fanalityczna(x);
        double wartosc_szeregu = suma_szeregu(x, dokladnosc, M, &warunek_stopu, &liczba_wyrazow);

        if(warunek_stopu && liczba_wyrazow == M) {
            fprintf(plik, " %2.5lf \t%2.5lf \t%2.5lf \t\t%d \t\t%s\n", x, wartosc_szeregu, wartosc_dokladna, liczba_wyrazow, "osiagnieto_dokladnosc_i_M");
        }
        else {c
            fprintf(plik, " %2.5lf \t%2.5lf \t%2.5lf \t\t%d \t\t%s\n", x, wartosc_szeregu, wartosc_dokladna, liczba_wyrazow, warunek_stopu ? "osiagnieto_dokladnosc" : "maksymalne_M");
        }

    }
    printf("Wyniki zapisano w pliku: wyniki.txt\n");
    fclose(plik);

    return 0;
}
