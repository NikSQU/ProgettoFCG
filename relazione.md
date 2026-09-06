# Relazione di Progetto: Mulino 3D Interattivo
**Autore:** Nicolò Vernetti
**Matricola:** S6382763
**Corso:** Fondamenti di Computer Grafica

# L'Idea

Il progetto consiste nell'esposizione di un mulino che gli utenti potranno guardare in libertà spostandosi in un ambiente 3D.
Si può inoltre manipolare il tempo dell'ambiente 3D velocizzandolo, rallentandolo o fermandolo.
Sarà presente anche un "sole" abbozzato che prova a simulare il fattore di passaggio del tempo ciclando intorno all'ambiente 3D.

Il progetto è una scena 3D interattiva renderizzata tramite **OpenGL 4.1** e **SFML 3.0.0**. 
L'obiettivo principale è stato sviluppare un codice modulare e orientato agli oggetti, superando il classico approccio a singolo file.


## Tappa 01: Setup Iniziale (Cmake, SFML e Suolo)


Come inizio del progetto ho preso dal laboratorio 7 del professor Rocca una base da cui partire in cui gli ambienti Cmake e SFML sono già stati implementati.

Sono presenti già i file utili .hh nella cartella include: **hotshaders.hh** , **matrices.hh** , **mesh.hh** , **rawmouse.hh** , **trackball.hh**

Sono anche già presenti il file **cube.off** che ci dà la geometria dei modelli iniziale (a forma di cubo).

Infine il primo cambiamento inedito è nel **main.cc** in cui viene disegnata nella funzione ***void draw()*** il suolo su cui baserà l'intero disegno virtuale. Usando la libreria Glad per avere delle *matrici 4x4* (**glm::mat4**), creiamo il pavimento (s) che viene poi traslato in basso rispetto al centro della scena. Dopo aver calcolato la matrice di modello finale, viene inviata allo shader e la matrice di vista-proiezione.

![alt text](images/image-1.png)


## Tappa 02: 

Inizia in questa tappa la costruzione del mulino

Sono stati aggiunti: un nuovo file oggetto **cylinder.off** per il modello base della torre del mulino; è stata poi aggiunta la definizione del disegno della torre del mulino nella funzione ***void draw()***, similare alla creazione precedente del suolo (tramite l'uso di *matrici 4x4*)

![alt text](images/image-2.png)


## Tappa 03:

Sono stati aggiunti: il perno di rotazione delle pale del mulino (mozzo), primo pezzo di animazione dentro l'ambiente 3D, disegnato sempre nella funzione ***void draw()***, similare alla creazione precedente del suolo.

Sono inoltre state aggiunte le prime pale intorno al rotore, rendendo la prima versione del mulino completa.

![alt text](images/image.png)


## Tappa 04:


Sono stati aggiunti: le vele delle pale del mulino, i colori della scena: torre del mulino, mozzo, suolo e cielo e anche l'illuminazione base per tutti questi elementi (tramite l'uso della classe Lights).


![alt text](images/image-3.png)



## Tappa 05:


In questa tappa vengono aggiunti i primi controlli interattivi con l'ambiente 3D con il controllo della velocità delle pale del mulino.
Sono stati aggiunti: comandi con tasto *Up* e *Down* per velocizzare e rallentare la velocità delle pale, tasto *Barra Spaziatrice* per fermare il movimento delle pale.


## Tappa 06:


In questa tappa è stato effettuato un full refactoring del progetto. 

Il cambiamento principale è la divisione delle classi del **main.cc** in diversi file:
**input.cc**: che gestisce le diverse handle di Sfml (*SFML Callbacks*)
Con il rispettivo file header **input.hh**
**scene.cc**: che gestisce la classe *Scene* (La princiaple classe dove viene raffigurata la scena)
Con il rispettivo file header **scene.hh**
**lights.hh**: che gestice la classe *Lights* (dove vengono gestiti i colori e luci della scena)
**camera.hh**: che gestice la classe *Camera* (che definisce il movimento della telecamera, che determina la vista della scena)
**gpumesh.hh**: che gestisce la classe *GPUmesh* (che a sua volta gestisce le mesh con i file da caricare per la gpu)
**glad.cc**: semplice file da richiamare per la gestione di glad per il progetto. (?)

Il **main.cc** non è stato eliminato, rimane ancora in utilizzo per il principale main loop per costruire la scena chiamando le funzioni delle altre classi ed è anche rimasta nel main la classe *Setup* che si occupa del gestire il setup iniziale della finestra e di OpenGL.



## Tappa 07:


In questa tappa inizia lo sviluppo della camera indipendente in *first person view*. 
Sono stati aggiunti: movimento con il mouse libero con drag della visuale tenendo tasto sinistro del mouse, eliminata la trackball come metodo di vista.
Sono state attuate modifiche nel file **input.cc** e nel file **camera.hh** principalmente con l'implementazione di ***void process_mouse()***. 


![alt text](images/image-4.png)



## Tappa 08:


In questa tappa viene invece fixato il metodo per la gestione del mouse con "*rawmouse*" per riuscire ad avere le migliori prestazioni per il controllo del mouse(e anche altri fix minori), ma principalmente viene rimosso l'obbligo di drag con il mouse sinistro premuto per il movimento della camera.
Viene inoltre cambiato il modo di visione della finestra in full screen.

I principali cambiamenti avvengono in **input.cc** con il cambio della handle che gestisce il movimento del mouse.


## Tappa 09:


In questa tappa viene aggiunto il movimento tridimensionale con i comandi *W* *A* *S* *D* per muoversi nella quattro direzioni orizzontali, e con *SHIFT* e la *Barra Spaziatrice* per salire e scendere di quota.

I cambiamenti principali avvengono in **camera.hh** dove viene creata ***void move()*** e ***void process_movement()*** e poi sostituita da ***void handle_realtime_input()*** in **input.cc** che gestisce il movimento tramite comandi tastiera.


![alt text](images/image-5.png)



## Tappa 10:


In questa tappa è avvenuto un secondo refactoring minore dove viene effettivamente usata la classe *Rawmouse*, per risolvere i problemi di gestione del mouse.
Viene aggiunto un file **rawmouse.cc** e il suo header **rawmouse.hh** (Successivamente levato perchè non mi ero accorto fosse già presente in *include/*) e vengono modificati i diversi tipi di MouseMoved in Rawmouse per le handle di **input.cc**. 

Oltre ad altri fix minori, viene inoltre viene cambiato il **.gitignore** per la creazione *build-win/* e aggiunto il **windows-toolchain.cmake** per avere una compilazione più veloce e efficiente e viene risettato anche OpenGL. (?)



## Tappa 11:


In questa tappa vengono aggiunte le collisioni per il prato della scena. Collisioni prima per il pavimento a forma quadrata, cambiata poi in una collisione circolare, poi per il "sotto-pavimento" (soffitto) è stato aggiunto un controllo continuo sull'asse Y che impedisce compenetrazioni e cadute accidentali quando ci si trova sotto (e anche sopra) il prato.

Quindi è stato utilizzato il concetto di *Continuous Collision Detection* per questa tappa.



## Tappa 12:


In questa tappa vengono aggiunti l'illuminazione ambientale globale e il colore del cielo (tramite *glClearColor*) che sono calcolati proceduralmente e viene aggiunto anche un sole fisico.
Usando funzioni trigonometriche, il sole compie un'orbita nel cielo, e lo scorrere del tempo è stato vincolato all'accelerazione meccanica delle pale del mulino, permettendo la manipolazione del tempo della scena con i comandi *M* per fermare e *Up*/*Down* per velocizzare e rallentare il tempo.

I cambiamenti maggiori avvengono in **scene.cc** dove viene disegnato il sole fisico nella funzione ***void draw()*** e vengono aggiunte le luci distinte a seconda del tempo della scena nella funzione ***void update_all()***


![alt text](images/image-7.png)


![alt text](images/image-6.png)








-Eventuali problemi riscontrati e le soluzioni tecniche adottate. 


-L'indicazione di eventuali risorse esterne utilizzate (con le relative fonti).
