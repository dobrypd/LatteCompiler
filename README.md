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

w katalogu głównym:

`$ ./bin/latec`

Używane narzędzia i biblioteki:
-------------------------------

Do budowania parsera korzystam z narzędzia `bnfc`.
Oprócz standardosy bibliotek (stl) korzystam z boost'a (shared pointery).

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
  
