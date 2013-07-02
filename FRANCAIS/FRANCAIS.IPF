#include "clock.h"
:userdoc.
:title."Horloge" Help
:body.

:h1 res=ID_MAIN.Introduction
:i1.Introduction
:artwork name='clock.bmp' align=center.
:p.
Ce programme est une adaptation importante du programme d&apos.horloge
de Charles Petzold, qui peut àtre trouvÇ dans le livre ÆProgramming the
OS/2 Presentation ManagerØ. Il affiche une horloge analogique, mise Ö
jour une fois par seconde.
:p.
Pour information, ce programme a ÇtÇ modifiÇ pour observer le nombre de
tÉches actives du systäme ou la charge du processeur, et changer la
couleur de la bordure en fonction du niveau de charge.
:p.
Comme vous avez pu le constater, l&apos.aide en ligne fonctionne, et les
commandes du programme peuvent àtre atteintes via le menu systäme de la
fenàtre. Les commandes suivantes sont disponiblesˇ&colon.
:sl compact.
:li.:hpt.Sauvegarder la configuration:ehpt.:hdref res=IDM_SAVE_APPLICATION.
:li.:hpt.RÇinitialiser l&apos.environnement:ehpt.:hdref res=IDM_RESET_DEFAULTS.
:li.:hpt.Cacher la barre de titre:ehpt.:hdref res=IDM_HIDE_CONTROLS.
:li.:hpt.Configurer...:ehpt.:hdref res=IDM_CONFIGURE.
:li.:hpt.A propos...:ehpt.:hdref res=IDM_ABOUT.
:li.:hpt.Fermer:ehpt.:hdref res=IDM_EXIT.
:esl.
:p.En plus des fonctionnalitÇs dÇjÖ dÇcrites, le programme accepte les
commandes provenant des gestionnaires de polices et de couleurs d'OS/2
2.x.

:h1 res=IDM_SAVE_APPLICATION.Sauvegarder la configuration (Option de menu)
:i1.Sauvegarder la configuration (Option de menu)
Quand cette option de menu est sÇlectionnÇe, le programme enregistre sa
position sur l&apos.Çcran et l&apos.Çtat de l&apos.option :hp4.Cacher la
barre de titre:ehp4.. La prochaine fois que le programme est lancÇ, la
configuration actuelle sera rÇutilisÇe.
:note. La touche de raccourci pour cette fonction est :hp4.F2:ehp4..

:h1 res=IDM_RESET_DEFAULTS.RÇinitialiser l&apos.environnement (Option de menu)
:i1.RÇinitialiser l&apos.environnement (Option de menu)
Cette option de menu entraåne la rÇinitialisation des attributs de
police et de couleur du programme Ö leurs valeurs par dÇfaut.

:h1 res=IDM_HIDE_CONTROLS.Cacher la barre de titre (Option de menu)
:i1.Cacher la barre de titre (Option de menu)
La sÇlection de cette option de menu entraåne la disparition de la barre
de titre de la fenàtre, et des contrìles qui y sont associÇs (le menu
systäme, la plaquette de rÇduction). Cette option peut àtre inversÇe en
cliquant deux fois l&apos.un des boutons de la souris. De plus, comme il
m&apos.a semblÇ träs utile de pouvoir dÇplacer la fenàtre quand la barre
de titre est absente, la fenàtre est configurÇe de telle sorte que vous
pouvez la faire glisser en pressant l&apos.un des boutons de la souris.
:note.La touche de raccourci pour cette fonction est :hp4.Alt+H:ehp4..

:h1 res=IDM_CONFIGURE.Configurer (Option de menu)
:i1.Configurer (Option de menu)
La sÇlection de cette option de menu entraåne l'apparition de la boåte
de dialogue de configuration du programme.
Pour plus d'informations, voir
:hpt.Configurer (Dialogue):ehpt.:hdref res=IDD_CONFIGURE..
:note.Le raccourci clavier pour cette commande est :hp4.Alt+C:ehp4..

:h2 res=IDD_CONFIGURE.Configurer (Dialogue)
Cette boåte de dialogue vous permet de modifier tous les aspects
configurables du comportement de ce programme. Pour quitter ce dialogue
sans effectuer de changement, pressez la touche :hp4.êchap:ehp4. ou
sÇlectez la plaquette :hp4.Annuler:ehp4.. Pour valider les changements
que vous avez effectuÇs, pressez la touche :hp4.EntrÇe:ehp4. ou sÇlectez
la plaquette :hp4.Ok:ehp4..
.br
Les ÇlÇments configurables sont les suivantsˇ:
:p.
:hp2.Type d'horloge:ehp2.
L'Çcran de l'horloge peut àtre soit analogique (avec des aiguilles),
soit numÇrique (Ö chiffres). Quand l'horloge est rÇduite, le cadran est
toujours analogique.
:p.
:hp2.24 heures:ehp2.
L'horloge numÇrique peut afficher l'heure de 0 Ö 23 heures ou de 0 Ö 12
heures.
:p.
:hp2.Cacher la barre de titre:ehp2.
Quand l'horloge n'est pas rÇduite, elle peut àtre affichÇe avec ou sans
la barre de titre, le menu systäme et la plaquette de rÇduction. Si vous
choisissez de cacher ces ÇlÇments de contrìle, les bords de
redimentionnement sont conservÇs. La fenàtre peut àtre dÇplacÇe en la
faisant glisserˇ; elle peut àtre redimentionnÇe en faisant glisser l'un
des cìtÇs ou un coin.
:p.
:hp2.Carillon:ehp2.
Cette case active la fonctionne de carillon horaire. Quand cette case
est cochÇe, l'horloge Çmet deux tops sonores toutes les heures.
:p.
:hp2.Toujours visible:ehp2.
Quand cette case est cochÇe, l'horloge reste visible en permanence.
:p.
:hp2.Animer:ehp2.
Cette case active l'animation lors de l'ouverture et de la fermeture de
la fenàtre. Quand cette case est cochÇe, la fenàtre du programme s'ouvre
et se ferme de la màme faáon que les dossiersˇ: avec un accompagnement
sonore et une animation. De màme que pour les dossiers, cette
fonctionnalitÇ doit àtre autorisÇe dans la configuration du systäme.
:p.
:hp2.Afficher les secondes:ehp2.
Quand cette case est cochÇe, les secondes sont affichÇes quand l'horloge
est en mode numÇrique. L'aiguille des secondes est toujours affichÇe
quand l'horloge est en mode analogique et que la taille de la fenàtre
est suffisante.
:p.
:hp2.Type d&apos.alerte:ehp2.
La couleur du bord de la fenàtre change en fonction de la charge du
processeur ou du nombre de tÉches actives. Vous pouvez sÇlectionner
l&apos.indicateur qui sera observÇ.
:p.
:hp2.Paliers d&apos.alerte:ehp2.
Vous pouvez ajuster le nombre de tÉches ou la charge du processeur Ö
partir desquels le cadre de la fenàtre passe du vert au jaune et du
jaune au rouge. Utilisez les flàches vers le haut et le bas pour
modifier les valeurs, ou utilisez la souris pour sÇlecter les plaquettes
haut et bas situÇes Ö droite des valeurs.

:h1 res=IDM_ABOUT.A propos (Option de menu)
:i1.A propos (Option de menu)
Cette option fait apparaåtre la fenàtre d&apos.informations sur le
programme.
Pour plus d'informations, voir
:hpt.A propos (Dialogue):ehpt.:hdref res=IDD_ABOUT..

:h2 res=IDD_ABOUT.A propos (Dialogue)
Cette boåte de dialogue affiche le nom du programme, l'icìne et les
informations de Copyright. Pour quitter le dialogue, pressez la touche
:hp4.EntrÇe:ehp4., la barre d'espacement ou la touche :hp4.êchap:ehp4..

:h1 res=IDM_EXIT.Fermer (Option de menu)
:i1.Fermer (Option de menu)
Le choix de cette option de menu entraine l'arràt du programme.

:h1 res=IDM_KEYS_HELP.Aide sur les touches
:i1.Aide sur les touches
Les touches de fonction suivantes sont disponibles dans ce programmeˇ&colon.
:sl compact.
:li.F1 - Aide
:li.F2 - Sauvegarder la configuration
:li.F3 - Sortie
:li.Alt+H - Cacher la barre de titre
:li.Alt+C - Configurer
:esl.

:h1 res=IDD_PROFILE_PATH.RÇpertoire configuration
:i1.RÇpertoire configuration
Cette boåte de dialogue est affichÇe quand le programme ne trouve pas
son fichier de configuration (.INI), et demande oó ce fichier se trouve
ou l&apos.endroit oó il doit àtre crÇÇ.
:p.
Seul un nom de rÇpertoire existant sera acceptÇ. Une fois le nom entrÇ,
pressez la touche :hp4.EntrÇe:ehp4. ou sÇlectez la plaquette
:hp4.Ok:ehp4. pour que le programme continue son exÇcution.
:p.
Si vous voulez interrompre l'initialisation du programme, pressez la
touche :hp4.êchap:ehp4. ou sÇlectez la plaquette :hp4.Annuler:ehp4..

:euserdoc.
