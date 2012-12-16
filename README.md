Latte language compiler
-----------------------

Author Piotr Dobrowolski

pd291528@students.mimuw.edu.pl

Kompilacja:
-----------

W katalogu głównym wywołanie:

`$ make`

Jeżeli razem z debugowym outputem:

`$ make debug=1`

Uruchamianie:
-------------

W katalogu głównym:

`$ ./latec nazwa_pliku`

Uruchom:

`$ ./latec -h` żeby uzyskać więcej informacji.

Używane narzędzia i biblioteki:
-------------------------------

Do budowania parsera korzystam z narzędzia `bnfc`.
Oprócz standardowych bibliotek (stl) korzystam z boost'a (shared pointery).

Do budowania kodu jvm korzystam z `jasmina`, (który zgodnie ze specyfikacją
powinien znajdować się w katalogu `lib`).

Ponadto predefiniowane funkcje powinny znajdować się w takim katalogu
aby mogły zostać znalezione przez classloader. Najlepiej w tym samym co
wykonanie. Predefiniowane funkcje znajdują się w katalogu `lib` w pliku
`Runtime.class`. 

Zaimplementowane rozszerzenia:
------------------------------

W obecnej wersji nie zostały zaimplementowane rozszerzenia.

Struktura karalogów:
--------------------

 * `/src/` źródła,
 * `/src/grammar` gramatyka bnfc,
 * `/src/kernel/` pliki cpp,
 * `/src/includes/` nagłówki,
 * `/bin/` elementy zbudowane,
 * `/scripts/` skrypty, np. do testowania,
 * `/testy/` testy,
  
