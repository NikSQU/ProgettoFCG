# Mulino 3D Interattivo

## Requisiti di Sistema
* Compilatore C++17 compatibile (GCC/Clang per Linux/macOS, MSVC per Windows).
* CMake (versione 3.16 o superiore).

# Istruzioni di Compilazione (Ultima Versione)

Per compilare la versione finale del progetto (Tappa 12), apri il terminale nella directory principale del progetto e lancia i seguenti comandi:

```bash
# 1. Configura il progetto e genera la cartella di build e build-win
cmake -B build-win -S . -DCMAKE_TOOLCHAIN_FILE='windows-toolchain.cmake' -DCMAKE_POLICY_VERSION_MINIMUM=3.5

# 2. Compila il codice sorgente
cmake --build build
o
cmake --build build-win -j $(nproc) 

# 3. Esegui
./build-win/Mulino3D.exe cylinder.off

```


# Comandi di Gioco

## Movimento (Telecamera FPS)

**Mouse (Movimento)**: Ruota liberamente la visuale a 360 gradi.

**W / A / S / D**: Cammina (o vola) in avanti, a sinistra, indietro e a destra.

**Barra Spaziatrice**: Sali di quota (vola verso l'alto).

**Shift Sinistro**: Scendi di quota (vola verso il basso).


## Manipolazione dell'Universo

Il tempo atmosferico e astronomico è fisicamente vincolato all'accelerazione meccanica del mulino a vento.

**Freccia Su**: Accelera il tempo.

**Freccia Giù**: Rallenta lo scorrere del tempo.

**M**: Pausa/Riprendi (Congela istantaneamente l'animazione delle pale, il sole nel cielo e il colore dell'illuminazione ambientale).