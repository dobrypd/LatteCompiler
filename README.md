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

`$ ./latc_x86 nazwa_pliku`

Uruchom:

`$ ./latc_x86 -h` żeby uzyskać więcej informacji.

Używane narzędzia i biblioteki:
-------------------------------

Do budowania parsera korzystam z narzędzia `bnfc`.
Oprócz standardowych bibliotek (stl) korzystam z boost'a (shared pointery).

Do zamiany kodu asemblera na program wykonywalny używam `gcc`.
Do linkowania `ld`.

Predefiniowane funkcje znajdują się w katalogu `lib` w pliku
`runtime.c`, skompilowane `runtime.o`. 

Zaimplementowane rozszerzenia:
------------------------------

Zaimplementowałem tablice oraz obiekty (wraz z metodami wirtualnymi).

Struktura karalogów:
--------------------

 * `/src/` źródła,
 * `/src/grammar` gramatyka bnfc,
 * `/src/kernel/` pliki cpp,
 * `/src/includes/` nagłówki,
 * `/bin/` elementy zbudowane,
 * `/scripts/` skrypty, np. do testowania,
 * `/testy/` testy,
  
