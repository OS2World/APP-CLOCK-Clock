#include "clock.h"
:userdoc.
:title."Horloge" Help
:body.

:h1 res=ID_MAIN.Introduction
:i1.Introduction
:artwork name='clock.bmp' align=center.
:p.
Ce programme est une adaptation importante du programme d&apos.horloge
de Charles Petzold, qui peut �tre trouv� dans le livre �Programming the
OS/2 Presentation Manager�. Il affiche une horloge analogique, mise �
jour une fois par seconde.
:p.
Pour information, ce programme a �t� modifi� pour observer le nombre de
t�ches actives du syst�me ou la charge du processeur, et changer la
couleur de la bordure en fonction du niveau de charge.
:p.
Comme vous avez pu le constater, l&apos.aide en ligne fonctionne, et les
commandes du programme peuvent �tre atteintes via le menu syst�me de la
fen�tre. Les commandes suivantes sont disponibles�&colon.
:sl compact.
:li.:hpt.Sauvegarder la configuration:ehpt.:hdref res=IDM_SAVE_APPLICATION.
:li.:hpt.R�initialiser l&apos.environnement:ehpt.:hdref res=IDM_RESET_DEFAULTS.
:li.:hpt.Cacher la barre de titre:ehpt.:hdref res=IDM_HIDE_CONTROLS.
:li.:hpt.Configurer...:ehpt.:hdref res=IDM_CONFIGURE.
:li.:hpt.A propos...:ehpt.:hdref res=IDM_ABOUT.
:li.:hpt.Fermer:ehpt.:hdref res=IDM_EXIT.
:esl.
:p.En plus des fonctionnalit�s d�j� d�crites, le programme accepte les
commandes provenant des gestionnaires de polices et de couleurs d'OS/2
2.x.

:h1 res=IDM_SAVE_APPLICATION.Sauvegarder la configuration (Option de menu)
:i1.Sauvegarder la configuration (Option de menu)
Quand cette option de menu est s�lectionn�e, le programme enregistre sa
position sur l&apos.�cran et l&apos.�tat de l&apos.option :hp4.Cacher la
barre de titre:ehp4.. La prochaine fois que le programme est lanc�, la
configuration actuelle sera r�utilis�e.
:note. La touche de raccourci pour cette fonction est :hp4.F2:ehp4..

:h1 res=IDM_RESET_DEFAULTS.R�initialiser l&apos.environnement (Option de menu)
:i1.R�initialiser l&apos.environnement (Option de menu)
Cette option de menu entra�ne la r�initialisation des attributs de
police et de couleur du programme � leurs valeurs par d�faut.

:h1 res=IDM_HIDE_CONTROLS.Cacher la barre de titre (Option de menu)
:i1.Cacher la barre de titre (Option de menu)
La s�lection de cette option de menu entra�ne la disparition de la barre
de titre de la fen�tre, et des contr�les qui y sont associ�s (le menu
syst�me, la plaquette de r�duction). Cette option peut �tre invers�e en
cliquant deux fois l&apos.un des boutons de la souris. De plus, comme il
m&apos.a sembl� tr�s utile de pouvoir d�placer la fen�tre quand la barre
de titre est absente, la fen�tre est configur�e de telle sorte que vous
pouvez la faire glisser en pressant l&apos.un des boutons de la souris.
:note.La touche de raccourci pour cette fonction est :hp4.Alt+H:ehp4..

:h1 res=IDM_CONFIGURE.Configurer (Option de menu)
:i1.Configurer (Option de menu)
La s�lection de cette option de menu entra�ne l'apparition de la bo�te
de dialogue de configuration du programme.
Pour plus d'informations, voir
:hpt.Configurer (Dialogue):ehpt.:hdref res=IDD_CONFIGURE..
:note.Le raccourci clavier pour cette commande est :hp4.Alt+C:ehp4..

:h2 res=IDD_CONFIGURE.Configurer (Dialogue)
Cette bo�te de dialogue vous permet de modifier tous les aspects
configurables du comportement de ce programme. Pour quitter ce dialogue
sans effectuer de changement, pressez la touche :hp4.�chap:ehp4. ou
s�lectez la plaquette :hp4.Annuler:ehp4.. Pour valider les changements
que vous avez effectu�s, pressez la touche :hp4.Entr�e:ehp4. ou s�lectez
la plaquette :hp4.Ok:ehp4..
.br
Les �l�ments configurables sont les suivants�:
:p.
:hp2.Type d'horloge:ehp2.
L'�cran de l'horloge peut �tre soit analogique (avec des aiguilles),
soit num�rique (� chiffres). Quand l'horloge est r�duite, le cadran est
toujours analogique.
:p.
:hp2.24 heures:ehp2.
L'horloge num�rique peut afficher l'heure de 0 � 23 heures ou de 0 � 12
heures.
:p.
:hp2.Cacher la barre de titre:ehp2.
Quand l'horloge n'est pas r�duite, elle peut �tre affich�e avec ou sans
la barre de titre, le menu syst�me et la plaquette de r�duction. Si vous
choisissez de cacher ces �l�ments de contr�le, les bords de
redimentionnement sont conserv�s. La fen�tre peut �tre d�plac�e en la
faisant glisser�; elle peut �tre redimentionn�e en faisant glisser l'un
des c�t�s ou un coin.
:p.
:hp2.Carillon:ehp2.
Cette case active la fonctionne de carillon horaire. Quand cette case
est coch�e, l'horloge �met deux tops sonores toutes les heures.
:p.
:hp2.Toujours visible:ehp2.
Quand cette case est coch�e, l'horloge reste visible en permanence.
:p.
:hp2.Animer:ehp2.
Cette case active l'animation lors de l'ouverture et de la fermeture de
la fen�tre. Quand cette case est coch�e, la fen�tre du programme s'ouvre
et se ferme de la m�me fa�on que les dossiers�: avec un accompagnement
sonore et une animation. De m�me que pour les dossiers, cette
fonctionnalit� doit �tre autoris�e dans la configuration du syst�me.
:p.
:hp2.Afficher les secondes:ehp2.
Quand cette case est coch�e, les secondes sont affich�es quand l'horloge
est en mode num�rique. L'aiguille des secondes est toujours affich�e
quand l'horloge est en mode analogique et que la taille de la fen�tre
est suffisante.
:p.
:hp2.Type d&apos.alerte:ehp2.
La couleur du bord de la fen�tre change en fonction de la charge du
processeur ou du nombre de t�ches actives. Vous pouvez s�lectionner
l&apos.indicateur qui sera observ�.
:p.
:hp2.Paliers d&apos.alerte:ehp2.
Vous pouvez ajuster le nombre de t�ches ou la charge du processeur �
partir desquels le cadre de la fen�tre passe du vert au jaune et du
jaune au rouge. Utilisez les fl�ches vers le haut et le bas pour
modifier les valeurs, ou utilisez la souris pour s�lecter les plaquettes
haut et bas situ�es � droite des valeurs.

:h1 res=IDM_ABOUT.A propos (Option de menu)
:i1.A propos (Option de menu)
Cette option fait appara�tre la fen�tre d&apos.informations sur le
programme.
Pour plus d'informations, voir
:hpt.A propos (Dialogue):ehpt.:hdref res=IDD_ABOUT..

:h2 res=IDD_ABOUT.A propos (Dialogue)
Cette bo�te de dialogue affiche le nom du programme, l'ic�ne et les
informations de Copyright. Pour quitter le dialogue, pressez la touche
:hp4.Entr�e:ehp4., la barre d'espacement ou la touche :hp4.�chap:ehp4..

:h1 res=IDM_EXIT.Fermer (Option de menu)
:i1.Fermer (Option de menu)
Le choix de cette option de menu entraine l'arr�t du programme.

:h1 res=IDM_KEYS_HELP.Aide sur les touches
:i1.Aide sur les touches
Les touches de fonction suivantes sont disponibles dans ce programme�&colon.
:sl compact.
:li.F1 - Aide
:li.F2 - Sauvegarder la configuration
:li.F3 - Sortie
:li.Alt+H - Cacher la barre de titre
:li.Alt+C - Configurer
:esl.

:h1 res=IDD_PROFILE_PATH.R�pertoire configuration
:i1.R�pertoire configuration
Cette bo�te de dialogue est affich�e quand le programme ne trouve pas
son fichier de configuration (.INI), et demande o� ce fichier se trouve
ou l&apos.endroit o� il doit �tre cr��.
:p.
Seul un nom de r�pertoire existant sera accept�. Une fois le nom entr�,
pressez la touche :hp4.Entr�e:ehp4. ou s�lectez la plaquette
:hp4.Ok:ehp4. pour que le programme continue son ex�cution.
:p.
Si vous voulez interrompre l'initialisation du programme, pressez la
touche :hp4.�chap:ehp4. ou s�lectez la plaquette :hp4.Annuler:ehp4..

:euserdoc.
