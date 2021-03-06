#include "clock.h"
:userdoc.
:title."Rellotge" Help
:body.

:h1 res=ID_MAIN.Introducci�
:i1.Introducci�
:artwork name='clock.bmp' align=center.
:p.
Aquest programa �s una revisi� substancial del programa Rellotge que es
pot trobar al llibre "Programming the OS/2 Presentation Manager" de
Charles Petzold. Mostra un rellotge anal�gic, actualitzat una vegada per
segon.
:p.
A petici� de l'escola br�ssol Frank, aquest programa s'ha modificat per
visualitzar tamb� el n�mero de tasques o la c�rrega del sistema a trav�s
del color del contorn de la finestra.
:p.
Aquesta ajuda �s interactiva, com ja us n'haureu adonat. L'acc�s a les
seg�ents funcions es pot fer a trav�s del men� de sistema de la
finestra&colon.
:sl compact.
:li.:hpt.Guardar configuraci�:ehpt.:hdref res=IDM_SAVE_APPLICATION.
:li.:hpt.Restaurar els valors per defecte:ehpt.:hdref res=IDM_RESET_DEFAULTS.
:li.:hpt.Amagar els controls:ehpt.:hdref res=IDM_HIDE_CONTROLS.
:li.:hpt.Configurar...:ehpt.:hdref res=IDM_CONFIGURE.
:li.:hpt.Informaci� del programa...:ehpt.:hdref res=IDM_ABOUT.
:li.:hpt.Close:ehpt.:hdref res=IDM_EXIT.
:esl.
:p.A m�s d'aquestes funcions descrites, el programa accepta ordres dels
controladors de fonts i de la paleta de colors de l'OS/2 2.0

:h1 res=IDM_SAVE_APPLICATION.Guardar configuraci� (Opci� del men�)
:i1.Guardar configuraci� (Opci� del men�)
Quan seleccioneu aquesta opci� del men�, el programa guarda la posici�
actual a la pantalla aix� com l'estat dels controls. La propera vegada
que s'executi el programa, ho far� a la mateixa posici� i amb els
controls amagats (o visibles), d'acord amb la informaci� gravada.
:note.La tecla acceleradora assignada a aquesta funci� �s F2.

:h1 res=IDM_RESET_DEFAULTS.Restaurar els valors per defecte (Opci� del men�)
:i1.Restaurar els valors per defecte (Opci� del men�)
Seleccionant aquesta opci� del men�, es restaura el tipus de lletra i
els atributs de color als seus valors per defecte.

:h1 res=IDM_HIDE_CONTROLS.Amagar els controls (Opci� del men�)
:i1.Amagar els controls (Opci� del men�)
Aquesta opci� del men�, quan est� seleccionada, fa que els controls de
la finestra del programa (el men� de sistema, la barra del t�tol i el
bot� de minimitzaci�) estiguin ocults. �s possible variar el valor
d'aquesta funci� si premeu dues vegades el bot� del mouse dintre de la
finestra. A m�s, com crec que ha de ser possible moure la finestra
mentres els controls estan amagats, �s possible fer aquesta acci� amb
qualsevol dels dos botons del mouse.
:note.La tecla acceleradora assignada a aquesta funci� �s ALT+H.

:h1 res=IDM_CONFIGURE.Configurar... (Opci� del men�)
:i1.Configurar... (Opci� del men�)
Quan seleccioneu aquesta opci� del men� es mostrar� la capsa de
di�leg de configuraci� del programa.
For more information, see :hpt.Configure (Dialog):ehpt.:hdref res=IDD_CONFIGURE..
:note.La combinaci� de tecles ALT+C realitza la mateixa acci�.

:h2 res=IDD_CONFIGURE.Configurar... (Capsa de di�leg)
Aquesta capsa de di�leg mostra totes les opcions del programa que s�n
configurables. Per avortar sense grabar qualsevol canvi que hagiu fet,
premeu la tecla ESC o b� feu un click al bot� "Cancel�lar".
Per grabar els cavis que hagiu fet, premeu la tecla ENTER o b� feu un
click al bot� "OK".
.br
Aquests s�n els apartats del programa que s�n configurables:
:p.
:hp2.Tipus de rellotge:ehp2.
Aquest rellotge pot presentar-se en forma anal�gica (amb manetes) o b�
digital. Quan el rellotge est� en forma d'icona, sempre �s anal�gic.
:p.
:hp2.24 hores:ehp2.
El rellotge digital pot mostrar l'hora en format 24 hores (des de les
00&colon.00 fins a les 23&colon.59, o b� en format 12 hores, afegint-hi
les indicacions 'am' (abans del migdia) i 'pm' (despr�s del migdia).
:p.
:hp2.Amagar els controls:ehp2.
Quan el rellotge est� en forma d'icona, pot mostrar o no els controls de
la finestra: la barra de t�tol, el men� de sistema i els botons de
minimitzar/maximitzar. Si escolliu l'opci� d'amagar aquests controls, es
mant� el contorn de la finestra, de forma que el tamany d'aquesta pugui
�sser modificat arrossegant qualsevol de les quatre cantonades de la
finestra.
:p.
:hp2.Campana:ehp2.
Aquest bot� activa la funci� de campana (alarma) hor�ria. Quan aquest
bot� est� marcat, el rellotge emetr� un doble senyal cada hora.
:p.
:hp2.Restar en primer pla:ehp2.
Aquest bot� activa la funci� que fa que la finestra estigui sempre en
primer pla. Quan aquest bot� est� marcat, el rellotge sempre estar� al
capdemunt de totes les finestres, restant sempre visible.
:p.
:hp2.Animaci�:ehp2.
Aquest bot� activa l'animaci� de la finestra a l'obrir-la i
tancar-la. Quan est� activa, la finestra del programa s'obrir�
de la mateixa forma que ho fan les carpetes (folders): amb so i
moviment. Com amb els folders, l'animaci� nom�s es far� si est�
activada a tot el sistema.
:p.
:hp2.Display Seconds:ehp2.
This button causes the seconds to be displayed in the clock
when the clock is in digital form.  The second hand is always
displayed when the clock is in analog form and over a certain
size anyway.
:p.
:hp2.Tipus d'av�s:ehp2.
El contorn del rellotge canvia de color d'acord amb la c�rrega del
sistema o del n�mero de tasques actives. L'usuari pot escollir quin
tipus d'acci� s'ha de mesurar.
:p.
:hp2.Nivells d'av�s:ehp2.
Es pot ajustar el n�mero de tasques actives o el nivell d'ocupaci� de la
CPU que far� canviar el color del contorn de la finestra de verd al groc
i del groc al vermell. Feu servir les tecles de cursor per ajustar els
valors, o b� amb el mouse (prement els botons que hi ha a la dreta dels
valors).

:h1 res=IDM_ABOUT.Informaci� del prorgama (Opci� del men�)
:i1.Informaci� del programa (Opci� del men�)
Quan es selecciona aquesta opci� del men� es mostra informaci� sobre
el programa.
For more information, see :hpt.About (Dialog):ehpt.:hdref res=IDD_ABOUT..

:h2 res=IDD_ABOUT.Informaci� del programa (Capsa de di�leg)
Aquesta capsa de di�leg mostra informaci� sobre el nom del programa, la
seva icona i informaci� sobre els drets d'autor. Per sortir d'aquesta
capsa de di�leg, premeu la tecla ENTER, la Barra d'espais o b� la tecla
ESCAPE.

:h1 res=IDM_EXIT.Close (Menu Option)
:i1.Close (Menu Option)
This menu option, when selected, will cause the program to terminate.

:h1 res=IDM_KEYS_HELP.Us del teclat
:i1.Us del telcat
Aquest programa ha definit les seg�ents tecles de funci�&colon.
:sl compact.
:li.F1 - Ajuda
:li.F2 - Guardar configuraci�
:li.F3 - Sortida
:li.Alt+H - Amagar els controls
:li.Alt+C - Configurar
:esl.

:h1 res=IDD_PROFILE_PATH.Indicar el directori del fitxer .INI
:i1.Indicar el directori del fitxer .INI
Aquesta capsa de di�leg es mostra quan el programa no pot trobar el seu
fitxer .INI, i pregunta on es troba aquest fitxer o a on s'ha de crear.
:p.
Nom�s s'accepten noms v�lids de directoris ja existents. Una vegada que
hagiu escrit el nom, prement la tecla RETURN o b� fent un click al bot�
"OK" fa que el programa continu� la seva seq��ncia d'inicialitzaci�.
:p.
Si, per contra, voleu tallar la inicialitzaci� del programa, premeu
la tecla ESC o b� feu un click al bot� d'anul�laci�.

:euserdoc.
