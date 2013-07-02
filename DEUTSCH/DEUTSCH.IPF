#include "clock.h"
:userdoc.
:title."Uhr" Hilfe
:body.

:h1 res=ID_MAIN.Einfhrung
:i1.Einfhrung
:artwork name='clock.bmp' align=center.
:p.
Dieses Programm stellt eine grundlegende Revision des Uhrenprogramms
von Charles Petzold aus seinem Buch "Programming the OS/2 Presentation
Manager" dar.  Es zeigt eine analoge Uhr an, die einmal pro Sekunde
angepaát wird.
:p.
Fr Frank's Nursery wurde das Programm modifiziert, so daá es die
Task-Zahl des Systems oder die Systemlast berwacht und die Farbe
seines Rahmens entsprechend der augenblicklichen Last ver„ndert.
:p.
Wie Sie schon gesehen haben, ist die Hilfeeinrichtung aktiv und die
existierenden Kommandos k”nnen ber das System-Men des Fensters
angesprochen werden.  Die folgenden Kommandos sind verfgbar&colon.
:sl compact.
:li.:hpt.Werte speichern:ehpt.:hdref res=IDM_SAVE_APPLICATION.
:li.:hpt.Standardwerte:ehpt.:hdref res=IDM_RESET_DEFAULTS.
:li.:hpt.Ohne Rahmen:ehpt.:hdref res=IDM_HIDE_CONTROLS.
:li.:hpt.Konfiguration...:ehpt.:hdref res=IDM_CONFIGURE.
:li.:hpt.Produktinformation:ehpt.:hdref res=IDM_ABOUT.
:li.:hpt.Schlieáen:ehpt.:hdref res=IDM_EXIT.
:esl.
:p.Auáer den schon beschriebenen Vorrichtungen akzeptiert das Programm
auch Kommandos von den OS/2 2.0-Programmen zur Farb- und Schriftartpalette.

:h1 res=IDM_SAVE_APPLICATION.Werte speichern (Men-Option)
:i1.Werte speichern (Men-Option)
Bei Selektion dieser Men-Option speichert das Programm seine
augenblickliche Position und den Zustand des Rahmens.
Beim n„chsten Programmstart wird das Fenster an der gleichen Position
und entweder mit oder ohne Rahmen erscheinen, entsprechend dem
gespeicherten Zustand.
:note.Dieses Kommando kann ber F2 direkt ausgefhrt werden.

:h1 res=IDM_RESET_DEFAULTS.Standardwerte (Men-Option)
:i1.Standardwerte (Men-Option)
Diese Men-Option setzt die Schriftart- und Farbattribute des
Programms auf ihre Standardwerte zurck.

:h1 res=IDM_HIDE_CONTROLS.Ohne Rahmen (Men-Option)
:i1.Ohne Rahmen (Men-Option)
Diese Men-Option verbirgt den Rahmen des Programmfensters, d.h. also
das System-Men, den Titelbalken und den Knopf fr Symbolgr”áe.  Mit
einem Doppelklick auf irgendeinen Mausknopf kann diese Option
umgeschaltet werden.  Da ich es fr ntzlich hielt, das Fenster
bewegen zu k”nnen, auch wenn der Rahmen verborgen ist, wurde das
Fenster so eingerichtet, daá es mit jedem Mausknopf verschoben werden
kann.
:note.Dieses Kommando kann ber ALT+H direkt ausgefhrt werden.

:h1 res=IDM_CONFIGURE.Konfiguration... (Men-Option)
:i1.Konfiguration... (Men-Option)
Bei Selektion dieser Men-Option wird der
Konfigurations-Dialog des Programms angezeigt.
Fr weitere Informationen, siehe :hpt.Konfiguration (Dialog):ehpt.:hdref res=IDD_CONFIGURE..
:note.Die Tastenkombination Alt+C fhrt dieselbe Funktion aus.

:h2 res=IDD_CONFIGURE.Konfiguration... (Dialog)
Dieses Dialogfenster erlaubt die nderung aller konfigurierbaren
Aspekte des Programms.
Nehmen Sie alle gewnschten nderungen vor und drcken dann
die ENTER-Taste oder klicken auf den OK-Knopf, um die
nderungen wirksam werden zu lassen.
:p.
Um den Dialog abzubrechen ohne irgendeine der vorgenommenen
nderungen zu speichern, drcken Sie die ESC-Taste oder
klicken auf den `Abbruch'-Knopf.
.br
Konfigurierbar sind u.a.:
:p.
:hp2.Uhrtyp:ehp2.
Der Uhrtyp kann entweder analog (mit Zeigern) oder digital sein.  Wenn
die Uhr auf Symbolgr”áe reduziert wird, ist sie immer analog.
:p.
:hp2.24-Stunden-Uhr:ehp2.
Die Digitaluhr kann die Zeit entweder im 24-Stunden-Format von
00&colon.00 bis 23&colon.59 oder im 12-Stunden-Format amerikanischer
Machart anzeigen, wobei 'am' bzw. 'pm' an die Zeit angeh„ngt wird.
:p.
:hp2.Ohne Rahmen:ehp2.
Wenn die Uhr nicht auf Symbolgr”áe reduziert ist, kann sie mit oder
ohne den blichen Titelbalken mit System-Men und
Verkleinerungs/Vergr”áerungskn”pfen angezeigt werden.  Wenn Sie diese
Teile des Rahmens verbergen wollen, bleibt der Rand zum Ver„ndern der
Gr”áe trotzdem erhalten.  Das Fenster kann positioniert werden, indem
es mit Mauszeiger in seinem inneren Bereich verschoben wird.  Seine
Gr”áe kann ver„ndert werden, indem irgendwelche seiner vier R„nder
bzw. Ecken mit der Maus verschoben werden.
:p.
:hp2.Stundensignal:ehp2.
Wenn dieser Knopf markiert ist, wird die Uhr zu Beginn jeder vollen
Stunde zwei kurze akustische Signale geben.
:p.
:hp2.Immer oberstes Fenster:ehp2.
Wenn dieser Knopf markiert ist, wird die Uhr immer auf der Spitze des
Fensterstapels bleiben, so daá sie immer sichtbar bleibt.
:p.
:hp2.Animation:ehp2.
Dieser Knopf aktiviert die Animationsfunktion beim ™ffnen und
Schlieáen eines Fensters.  Wenn diese aktiv ist, ”ffnet und schlieát
sich das Programmfenster in der gleichen Art wie Ordner-Objekte,
n„mlich mit Klang und Bewegung.  Wie bei den Ordnern erfolgt die
Animation jedoch nur, wenn sie auf der System-Ebene eingeschaltet
wurde.
:p.
:hp2.Sekunden anzeigen:ehp2.
Dieser Knopf sorgt dafr, daá die Sekunden bei der Uhr angezeigt
werden, wenn sie in digitaler Form ist.  Der Sekundenzeiger wird
sowieso immer
dargestellt, wenn die Uhr in analoger Darstelllung ist und eine
bestimmte Gr”áe berschreitet.
:p.
:hp2.Alarmtyp:ehp2.
Der Rahmen der Uhr ver„ndert seine Farbe entweder entsprechend der
Systembelastung oder der Anzahl aktiver Tasks.  Hier k”nnen Sie
ausw„hlen, welche dieser Gr”áen berwacht wird.
:p.
:hp2.Alarmstufen:ehp2.
Hier k”nnen Sie die Task-Anzahlen oder CPU-Lastfaktoren einstellen,
bei denen der Rahmen der Uhr sich von grn zu gelb und von gelb zu rot
verf„rbt.  Bentzen Sie die Pfeiltasten nach oben oder unten, um die
Werte anzupassen oder bentzen Sie die Maustasten, um die Aufw„rts-
bzw. Abw„rtskn”pfe rechts neben den Werten zu drcken.

:h1 res=IDM_ABOUT.Produktinformation (Men-Option)
:i1.Produktinformation (Men-Option)
Selektion dieser Option bewirkt die Anzeige der Produktinformation.
Fr weitere Informationen, siehe :hpt.Produktinformation (Dialog):ehpt.:hdref res=IDD_ABOUT..

:h2 res=IDD_ABOUT.Produktinformation (Dialog)
Das Dialogfenster zeigt den Namen des Programms, das zugeh”rige Symbol
und die Copyright-Information an.  Das Fenster kann verlassen werden,
indem man die Eingabetaste, die Leertaste oder ESC drckt oder auf den
OK-Knopf klickt.

:h1 res=IDM_EXIT.Schlieáen (Men-Option)
:i1.Schlieáen (Men-Option)
Bei Selektion dieser Men-Option wird das Programm beendet.

:h1 res=IDM_KEYS_HELP.Hilfe fr Tasten
:i1.Hilfe fr Tasten
Die folgenden Funktionstasten wurden fr dieses Programm definiert&colon.
:sl compact.
:li.F1 - Hilfe
:li.F2 - Werte speichern
:li.F3 - Ende
:li.Alt+H - Ohne Rahmen
:li.Alt+C - Konfiguration
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Initialisierungspfad setzen (Dialog)
:i1.Initialisierungspfad setzen (Dialog)
Dieser Dialog wird angezeigt, wenn das Programm seine
Initialisierungsdatei (INI) nicht finden kann.  Er fragt danach, wo
die Datei ist oder wo sie angelegt werden soll.
:p.
Nur ein gltiger
Verzeichnisname wird akzeptiert.  Wenn Sie den Namen eingegeben haben,
drcken Sie die ENTER-Taste oder klicken Sie auf den OK-Knopf, damit
das Programm fortf„hrt.
:p.
Wenn Sie die Initialisierungssequenz
abbrechen wollen, drcken Sie die ESC-Taste oder klicken auf den
`Abbruch'-Knopf.

:euserdoc.

