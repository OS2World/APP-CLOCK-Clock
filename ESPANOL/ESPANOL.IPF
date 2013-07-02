#include "clock.h"
:userdoc.
:title."Reloj" Help
:body.

:h1 res=ID_MAIN.Introducci¢n
:i1.Introducci¢n
:artwork name='clock.bmp' align=center.
:p.
Este programa es una revisi¢n sustancial del programa Reloj que se puede
encontrar en el libro "Programming the OS/2 Presentation Manager" de
Charles Petzold. Muestra un reloj anal¢gico, actualizado una vez por
segundo.
:p.
A petici¢n de la guarder¡a Frank, este programa se ha modificado para
visualizar tambi‚n el n£mero de tareas o la carga del sistema a trav‚s
del color del contorno de la ventana.
:p.
Esta ayuda es sensible al contexto, como ya se habr  dado cuenta. El
acceso a las siguientes funciones se realiza a trav‚s del men£ de
sistema de la ventana&colon.
:sl compact.
:li.:hpt.Grabar configuraci¢n:ehpt.:hdref res=IDM_SAVE_APPLICATION.
:li.:hpt.Restaurar los valores por defecto:ehpt.:hdref res=IDM_RESET_DEFAULTS.
:li.:hpt.Esconder los controles:ehpt.:hdref res=IDM_HIDE_CONTROLS.
:li.:hpt.Configurar...:ehpt.:hdref res=IDM_CONFIGURE.
:li.:hpt.Informaci¢n del programa...:ehpt.:hdref res=IDM_ABOUT.
:li.:hpt.Cerrar:ehpt.:hdref res=IDM_EXIT.
:esl.
:p.Aparte de estas funciones, el programa accepta ordenes de los
controladores de fuentes y de la paleta de colores del OS/2 2.0

:h1 res=IDM_SAVE_APPLICATION.Grabar configuraci¢n (Opci¢n del men£)
:i1.Grabar configuraci¢n (Opci¢n del men£)
Cuando se selecciona esta opci¢n del men£, el programa guarda la posici¢n
actual en la pantalla as¡ como el estado de los controles. La pr¢xima
vez que se ejecute el programa, lo har  en la misma posici¢n y con los
controles ocultos (o visibles), de acuerdo con la informaci¢n grabada.
:note.La tecla aceleradora asignada a esta funci¢n es F2.

:h1 res=IDM_RESET_DEFAULTS.Restaurar los valores por defecto (Opci¢n del men£)
:i1.Restaurar los valores por defecto (Opci¢n del men£)
Seleccionado esta opci¢n del men£, se restaura el tipo de letra y los
atributos de color a sus valores por defecto.

:h1 res=IDM_HIDE_CONTROLS.Esconder los controles (Opci¢n del men£)
:i1.Esconder los controles (Opci¢n del men£)
Esta opci¢n del men£, cuando est  seleccionada, obliga a esconder los
controles de la ventana del programa (el men£ de sistema, la barra de
t¡tulo y el bot¢n de minimizaci¢n). Es posible variar el valor de esta
funci¢n si pulsamos dos veces el bot¢n del mouse dentro de la ventana.
Adem s, como creo que ha de ser posible mover la ventana mientras los
controles estan escondidos, esta acci¢n puede realizarse con cualquiera
de los dos botones del mouse.
:note.La tecla aceleradora asignada a esta funci¢n es ALT+H.

:h1 res=IDM_CONFIGURE.Configurar... (Opci¢n del men£)
:i1.Configurar... (Opci¢n del men£)
Cuando se seleccione esta opci¢n del men£ se mostrar  la caja de di logo
de configuraci¢n del programa.
Para mas informaci¢n, v‚ase :hpt.Configurar (Caja de di logo):ehpt.:hdref res=IDD_CONFIGURE..
:note.La combinaci¢n de teclas ALT+C realiza la misma acci¢n.

:h2 res=IDD_CONFIGURE.Configurar... (Caja de di logo)
Esta caja de di logo muestra todas las opciones del programa que son
configurables. Para abortar sin grabar cualquier cambio que se hubiera
realizado pulsar la tecla ESC o bien hacer un click en el bot¢n
"Cancelar".
Para grabar los cambios que se hayan realizado, pulsar la tecla ENTER o
bien hacer un click en el bot¢n "OK".
.br
Estos son los apartados del programa que son configurables:
:p.
:hp2.Tipo de reloj:ehp2.
Este reloj se puede presentar en forma anal¢gica (con manillas) o bien
digital. Cuando el reloj est  en forma de icono, siempre es anal¢gico.
:p.
:hp2.24 horas:ehp2.
El reloj digital puede mostrar la hora en formato 24 horas (desde las
00&colon.00 hasta las 23&colon.59) o bien en formato 12 horas,
a¤adiendo las indicaciones 'am' (antes del mediod¡a) y 'pm' (despu‚s del
mediod¡a).
:p.
:hp2.Esconder los controles:ehp2.
Cuando el reloj est  en forma de icono, puede mostrar o no los controles
de la ventana: la barra de t¡tulo, el men£ de sistema y los botones de
minimizar/maximizar. Si se escoge la opci¢n de esconder estos controles,
se mantiene el contorno de la ventana, de forma que el tama¤o de esta
puede ser modificado arrastrando cualquiera de las cuatro esquinas de la
ventana.
:p.
:hp2.Campana:ehp2.
Este bot¢n activa la funci¢n de campana (alarma) horaria. Cuando este
bot¢n est  marcadi, el reloj emitir  una doble se¤al cada hora.
:p.
:hp2.Quedar en primer plano:ehp2.
Este bot¢n activa la funci¢n que hace que la ventana est‚ siempre en
primer plano. Cuando este bot¢n est‚ marcado, el reloj siempre estar 
arriba de todas las ventanas, quedando siempre visible.
:p.
:hp2.Animaci¢n:ehp2.
Este bot¢n activa la animaci¢n de la ventana en el momento de
abrirla y cerrarla. Cuando est  activa, la ventana del programa
se abrir  de la misma forma que lo hacen las carpetas (folders):
con sonido y movimiento. Como con los folders, la animaci¢n s¢lo
se realizar  si est  activada a todo el sistema.
:p.
:hp2.Mostrar Segundos:ehp2.
Este bot¢n causa que los segundos se vean en el reloj cuando
est‚ de forma digital.  La segundera siempre se ve cuando
el reloj est‚ de forma anal¢gica y mas grande que un cierto
tama¤o de todos modos.
:p.
:hp2.Tipo de aviso:ehp2.
El contorno del reloj cambia de color de acuerdo con la carga del
sistema o del n£mero de tareas activas. El usuario puede escoger qu‚
tipo de acci¢n se ha de medir.
:p.
:hp2.Niveles de aviso:ehp2.
Se puede ajustar el n£mero de tareas activas o el nivel de ocupaci¢n de
la CPU que har  cambiar el color del contorno de la la ventana de verde
al amarillo y del amarillo al rojo. Utilizar las  teclas de cursor para
ajustar los valores, o bien con el mouse (pulsando los botones que hay a
la derecha de los valores).

:h1 res=IDM_ABOUT.Informaci¢n del prorgama (Opci¢n del men£)
:i1.Informaci¢n del programa (Opci¢n del men£)
Cuando se selecciona esta opci¢n del men£ se muestra informaci¢n sobre
el programa.
Para mas informaci¢n, v‚ase :hpt.Informaci¢n del programa (Caja de di logo):ehpt.:hdref res=IDD_ABOUT..

:h2 res=IDD_ABOUT.Informaci¢n del programa (Caja de di logo)
Esta caja de di logo muestra informaci¢n sobre el nombre del programa,
su icono e informaci¢n sobre los derechos de autor. Para salir de esta
caja de di logo, pulsar la tecla ENTER, la barra espaciadora o bien la
tecla ESCAPE.

:h1 res=IDM_EXIT.Cerrar (Opci¢n del men£)
:i1.Cerrar (Opci¢n del men£)
Cuando se selecciona esta opci¢n del men£ se terminar  el programa.

:h1 res=IDM_KEYS_HELP.Uso del teclado
:i1.Uso del telcado
Este programa tiene definidas las siguientes teclas de funci¢n&colon.
:sl compact.
:li.F1 - Ayuda
:li.F2 - Grabar configuraci¢n
:li.F3 - Salida
:li.Alt+H - Esconder los controles
:li.Alt+C - Configurar
:esl.

:h1 res=IDD_PROFILE_PATH.Indicar el directorio del fichero .INI
:i1.Indicar el directorio del fichero .INI
Esta caja de di logo se muestra cuando el programa no puede encontrar su
fichero .INI y pregunta d¢nde se encuentra este fichero, o d¢nde debe
crearlo.
:p.
S¢lo se aceptan nombres v lidos de directorios ya existentes. Una vez
que hayais escrito el nombre, pulsando la tecla RETURN o bien haciendo
click en el bot¢n "OK", el programa continuar  su secuencia normal de
inicializaci¢n.
:p.
Si, por el contrario, se desea cortar esta secuencia de inicializaci¢n,
pulsar la tecla ESC o bien hacer click en el bot¢n de anulaci¢n.

:euserdoc.
