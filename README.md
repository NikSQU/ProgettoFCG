# Mulino 3D Interattivo

## Requisiti di Sistema
* Compilatore C++17 compatibile (GCC/Clang per Linux/macOS, MSVC per Windows).
* CMake (versione 3.16 o superiore).

# Istruzioni di Compilazione (Ultima Versione)

Per compilare la versione finale del progetto (Tappa 12), apri il terminale nella directory principale del progetto e lancia i seguenti comandi:

```bash
# 1. Configura il progetto e genera la cartella di build
cmake -B build -S . -DCMAKE_POLICY_VERSION_MINIMUM=3.5

# 2. Compila il codice sorgente
cmake --build build

# 3. Esegui
./build/Mulino3D cube.off
# oppure da tappa 02:
./build/Mulino3D cylinder.off
```

# Istruzioni di Compilazione Completa (Tutte le tappe)

```bash
./stages.sh all
```


# Comandi di Gioco


## Movimento (Telecamera FPS)

Fino a tappa 06:

**Visuale Trackball**: Ruota con la scena tendendo premuto il tasto sinistro del mouse. 


Da tappa 07 in poi:

**Mouse (Movimento)**: Ruota liberamente la visuale a 360 gradi.


Da tappa 09 in poi:

**W / A / S / D**: Cammina (o vola) in avanti, a sinistra, indietro e a destra.

**Barra Spaziatrice**: Sali di quota (vola verso l'alto).

**Shift Sinistro**: Scendi di quota (vola verso il basso).


## Manipolazione dell'Universo

Dalla tappa 03 fino alla tappa 11:

**Freccia Su**: Accelera le pale del mulino.

**Freccia Giù**: Rallenta le pale del mulino.

**Barra Spaziatrice**: Ferma il mulino.

Dalla tappa 12 il tempo atmosferico e astronomico è fisicamente vincolato all'accelerazione meccanica del mulino a vento.

**Freccia Su**: Accelera il tempo.

**Freccia Giù**: Rallenta il tempo.

**M**: Ferma/riprendi a far scorrere il tempo.