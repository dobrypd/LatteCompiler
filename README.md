Latte language compiler
-----------------------

Author Piotr Dobrowolski

pd291528@students.mimuw.edu.pl

Uwaga:
------

Nie byłem pewien gdzie `Runtime.class` ma się znajdować podczas uruchomienia programu.
(Albo inaczej gdzie program będzie uruchamiany, aby ustawić classpath).
Więc przyjąłem, że `Runtime.class` powinien być w `./lib/` oraz przy uruchamianiu
w tym samym katalogu co program `*.class`.
Jest to zakodowane w programie w pliku `./src/kernel/JVMGenerator.cpp:26`,
zmienna `JVMGenerator::runtime_lib`.

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
  
