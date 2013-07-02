#include "clock.h"
:userdoc.
:title."Clock" Help
:body.

:h1 res=ID_MAIN.Introduzione
:i1.Introduzione
:artwork name='clock.bmp' align=center.
:p.
Questo programma  e'  sostanzialmente  una  revisione  del programma
Clock  di  Charles  Petzold,  che  si  puo'  trovare   nel   libro
"Programming  the  OS/2  Presentation  Manager".  Mostra un orologio
analogico, aggiornato una volta al secondo.
:p.
Per Frank's Nursery, ho  modificato  questo programma per monitorare
il numero di task attivi  nel  sistema  o  il  carico  della  CPU  e
cambiare il colore del bordo in funzione della situazione corrente.
:p.
Come  avrete  gia'  osservato  e'  funzionante  un  help,  e si puo'
accedere a tutti i  comandi  tramite  il  menu della finestra.  Sono
disponibili i seguenti comandi&colon.
:sl compact.
:li.:hpt.Salva configurazione:ehpt.:hdref res=IDM_SAVE_APPLICATION.
:li.:hpt.Reinizializza configurazione:ehpt.:hdref res=IDM_RESET_DEFAULTS.
:li.:hpt.Nascondi controlli:ehpt.:hdref res=IDM_HIDE_CONTROLS.
:li.:hpt.Configura...:ehpt.:hdref res=IDM_CONFIGURE.
:li.:hpt.Informationi sul prodotto...:ehpt.:hdref res=IDM_ABOUT.
:li.:hpt.Chiudi:ehpt.:hdref res=IDM_EXIT.
:esl.
:p.Oltre a queste funzioni gia' descritte  il  programma  accetta  i
comandi inviati dai programmi di Font e Colori dell'OS/2.

:h1 res=IDM_SAVE_APPLICATION.Salva configurazione (Optione di menu)
:i1.Salva configurazione (Optione di menu)
Quando  scegli  quest'opzione,  il  programma  salva  la sua attuale
posizione  e  lo  stato  dell'opzione  di  Nascondi  Controlli.   La
prossima volta che  il  programma  ripartira' utilizzera' nuovamente
queste informazioni.
:note.Il comando veloce e' F2.

:h1 res=IDM_RESET_DEFAULTS.Reinizializza configurazione (Opzione di menu)
:i1.Reinizializza configurazione (Opzione di menu)
Selezionando  quest'opzione  resetterai  le font del programma e gli
attributi del colore ai suoi valori di default.

:h1 res=IDM_HIDE_CONTROLS.Nascondi i controlli (opzione di menu)
:i1.Nascondi i controlli (opzione di menu)
Con questa opzione si  attiva  o  disattiva  la barra della finestra
(contenente il menu di systema, la barra del titolo ed il bottone di
miniminizzazione).   Questa   opzione   puo'   essere   attivata   o
disattivata  kliccando  con qualsiasi tasto del mouse.  Inoltre, per
migliorare la prestazione, la finestra puo' essere mossa anche senza
barra di menu.
:note.Il comando veloce e' Alt+H.

:h1 res=IDM_CONFIGURE.Configura (Opzione di menu)
:i1.Configura (Opzione di menu)
Quando azionata attiva la finestra di configurazione.
Per ulteriori informazioni, guarda :hpt.Configura (Finestra di dialogo):ehpt.:hdref res=IDD_CONFIGURE..
:note.Il comando veloce e' Alt+C.

:h1 res=IDD_CONFIGURE.Configura (Finestra di dialogo)
:i1.Configura (Finestra di dialogo)
Questa finestra ti permette di modificare tutti  gli  aspetti  delle
operazioni  eseguibili  sul  programma.  Per uscire senza modificare
nulla premi  o  seleziona  il  tasto  di  ESCAPE.   Per  salvare  le
modifiche premi ENTER o seleziona il pulsante di OK.
.br
Gli elementi configurabili sono i seguenti:
:p.
:hp2.Tipo di orologio:ehp2.
L'orologio puo' essere sia analogico (con le lancette) che digitale.
Quando l'orologio e' ridotto  ad  icona  il  suo aspetto e' comunque
sempre analogico.
:p.
:hp2.24 ore:ehp2.
L'orologio puo' visualizzare  il  tempo  nello  stile  militare,  da
00&colon.00  a  23&colon.59,  o nel modo a 12 ore, con l'aggiunta di
'am' e 'pm'.
:p.
:hp2.Nascondi i controlli":ehp2.
Quando l'orologio non e' ridotto  ad icona, puo' essere mostrato con
o senza la barra della finestra, il menu di sistema ed i pulsanti di
dimensionamento.   Se  scegli  di nascondere questi controlli, resta
solo il bordo  di  dimensionamento.La  finestra puo' essere spostata
trascinandola nell'area centrale.   Puo'  essere  dimensionata  come
tutte le altra finestre.
:p.
:hp2.Suono orario:ehp2.
Questo  comando  attiva  la  funzione  di allarme oraria: l'orologio
emettera' un breve suono ad ogni cambio d'ora.
:p.
:hp2.Sempre visibile:ehp2.
Attiva l'opzione di 'galleggiamento'.
Con quest'opzione la  finestra  dell'orologio  restera' sempre sopra
qualsiasi altra, restando visibile tutto il tempo.
:p.
:hp2.Animazione:ehp2.
Attiva  l'opzione  di  animazione  nell'apertura  e  chiusura  della
finestra; cosi' la finestra del clock si comportera' come  tutte  le
finstre  della scrivania, inclusi i suoni.  Comunque se l'animazione
e' stata impostata a livello di sistema questa opzione e' superflua.
:p.
:hp2.Mostra i secondi:ehp2.
Causa  la  visualizzazione  dei  secondi  quando  l'orologio  e'  in
modalita'  digitale.   La   lancetta   dei   secondi,  in  modalita'
analogica,   e'   sempre   mostrata,   dimensioni   della   finestra
permettendolo.
:p.
:hp2.Tipo di allarme:ehp2.
Il bordo dell'orologio cambia di colore a  seconda  del  carico  del
sistema  o  del  numero  di task attivi.  Puoi decidere quale evento
monitorera'.
:p.
:hp2.Livello di allerta:ehp2.
Puoi modificare la soglia del numero di task o del carico di CPU che
mutera' il colore del bordo da  verde  a giallo e da giallo a rosso.
Hai a disposizione i pulsanti freccia o la digitazione diretta.

:h1 res=IDM_ABOUT.Informazioni sul prodotto... (Opzione di menu)
:i1.Informazioni sul prodotto... (Opzione di menu)
Visualizza una finestra di informazioni.
Per ulteriori informazioni, guarda :hpt.Informazioni sul prodotto... (Finestra di dialogo):ehpt.:hdref res=IDD_ABOUT..

:h1 res=IDD_ABOUT.Informazioni sul prodotto... (Finestra di dialogo)
:i1.Informazioni sul prodotto... (Finestra di dialogo)
Questa finestra mostra il nome del programma, l'icona e informazioni
di copyright.
Per uscire, batti ENTER, la barra di SPAZIO o il tasto di ESCAPE.

:h1 res=IDM_EXIT.Chiudi (Opzione di menu)
:i1.Chiudi (Opzione di menu)
Selezionando quest'opzione di menu si pone termine all'esecuzione del programma.

:h1 res=IDM_KEYS_HELP.Tasti di Help
:i1.Tasti di Help
Sono stati definiti i seguenti tasti funzione per questo programma&colon.
:sl compact.
:li.F1 - Help
:li.F2 - Salva configurazione
:li.F3 - Esci
:li.Alt+H - Nascondi controlli
:li.Alt+C - Configura
:esl.

:h1 res=IDD_PROFILE_PATH.Seleziona percorso di configurazione
:i1.Seleziona percorso di configurazione
Questa finestra appare quando il  programma  non riesce a trovare il
file di configurazione (INI), e chiede dove sia o dove possa  essere
creato.
:p.
E' accettata solo una directory esistente.
Una  volta  scritto il percorso premi ENTER o clikka sul pulsante di
OK per continuare.
:p.
Altrimenti per interrompere la  sequenza di inzializzazione premi il
tasto ESC o il bottone di escape.

:euserdoc.
