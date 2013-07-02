/******************************************************************** CLOCK.H
 *                                                                          *
 *                Analog Clock for Presentation Manager                     *
 *                                                                          *
 *                Original program by Charles Petzold                       *
 *                                                                          *
 ****************************************************************************/

#define PROGRAM_NAME "CLOCK"
#define CLASS_NAME   PROGRAM_NAME
#define LOGFILE      PROGRAM_NAME ".LOG"

extern HMODULE LibraryHandle ;

#define ID_MAIN 1
#define ID_TIMER 1

#define IDM_MENU                           10
#define IDM_SAVE_APPLICATION               11
#define IDM_RESET_DEFAULTS                 12
#define IDM_HIDE_CONTROLS                  13
#define IDM_CONFIGURE                      14
#define   IDD_CONFIGURE                    140
#define     IDD_CONFIG_ANALOG              140
#define     IDD_CONFIG_DIGITAL             141
#define     IDD_CONFIG_HOUR24              142
#define     IDD_CONFIG_HIDECONTROLS        143
#define     IDD_CONFIG_CHIME               144
#define     IDD_CONFIG_FLOAT               145
#define     IDD_CONFIG_ANIMATE             146
#define     IDD_CONFIG_SECONDS             147
#define     IDD_CONFIG_TASKCOUNT           148
#define     IDD_CONFIG_CPULOAD             149
#define     IDD_CONFIG_TASKCOUNTRED        150
#define     IDD_CONFIG_TASKCOUNTYELLOW     151
#define     IDD_CONFIG_CPULOADRED          152
#define     IDD_CONFIG_CPULOADYELLOW       153

#define IDM_ABOUT                          15
#define   IDD_ABOUT                        150

#define IDM_EXIT                           98
#define IDM_KEYS_HELP                      99

#define IDD_PROFILE_PATH                   9900
#define   IDD_PROF_ENTRY                   9900
#define   IDD_PROF_ERR                     9901

#define IDS_HELPTITLE                      0x0000
#define IDS_TITLE                          0x0001

#define IDS_SAVE_APPLICATION               0x0002
#define IDS_RESET_DEFAULTS                 0x0003
#define IDS_HIDE_CONTROLS                  0x0004
#define IDS_CONFIGURE                      0x0005

#define IDS_ABOUT                          0x0006
#define IDS_HELP                           0x0007

#define IDS_PARAMETERLIST                  0x0008
#define IDS_PARMS_RESET                    0x0009
#define IDS_PARMS_ANALOG                   0x000A
#define IDS_PARMS_DIGITAL                  0x000B

#define IDS_ERROR_INVALIDPARM              0x000C
#define IDS_ERROR_PRFOPENPROFILE           0x000D
#define IDS_ERROR_CREATEHELP               0x000E
#define IDS_ERROR_CREATEFRAME              0x000F
#define IDS_ERROR_WINREGISTERCLASS         0x0010
#define IDS_ERROR_CREATECLIENT             0x0011
#define IDS_ERROR_DOSGETCTRYINFO           0x0012

#define IDS_NOON                           0x0013
#define IDS_MIDNIGHT                       0x0014

#define IDS_EXCEPTION                      0x0015

#define IDS_HMERR                          0x8000
#define IDS_HMERR_EXTHELPUNDEFINED         0x8001
#define IDS_HMERR_NO_FRAME_WND_IN_CHAIN    0x8002
#define IDS_HMERR_INVALID_ASSOC_APP_WND    0x8003
#define IDS_HMERR_INVALID_ASSOC_HELP_IN    0x8004
#define IDS_HMERR_INVALID_DESTROY_HELP_    0x8005
#define IDS_HMERR_NO_HELP_INST_IN_CHAIN    0x8006
#define IDS_HMERR_INVALID_HELP_INSTANCE    0x8007
#define IDS_HMERR_INVALID_QUERY_APP_WND    0x8008
#define IDS_HMERR_HELP_INST_CALLED_INVA    0x8009
#define IDS_HMERR_HELPTABLE_UNDEFINE       0x800A
#define IDS_HMERR_HELP_INSTANCE_UNDEFIN    0x800B
#define IDS_HMERR_HELPITEM_NOT_FOUND       0x800C
#define IDS_HMERR_INVALID_HELPSUBITEM_S    0x800D
#define IDS_HMERR_HELPSUBITEM_NOT_FOUND    0x800E
#define IDS_HMERR_INDEX_NOT_FOUND          0x800F
#define IDS_HMERR_CONTENT_NOT_FOUND        0x8010
#define IDS_HMERR_OPEN_LIB_FILE            0x8011
#define IDS_HMERR_READ_LIB_FILE            0x8012
#define IDS_HMERR_CLOSE_LIB_FILE           0x8013
#define IDS_HMERR_INVALID_LIB_FILE         0x8014
#define IDS_HMERR_NO_MEMORY                0x8015
#define IDS_HMERR_ALLOCATE_SEGMENT         0x8016
#define IDS_HMERR_FREE_MEMORY              0x8017
#define IDS_HMERR_PANEL_NOT_FOUND          0x8018
#define IDS_HMERR_DATABASE_NOT_OPEN        0x8019
#define IDS_HMERR_UNKNOWN                  0x801A
#define IDS_HELPMODE_FRAME                 0x801B
#define IDS_HELPMODE_MENU                  0x801C
#define IDS_HELPMODE_WINDOW                0x801D
#define IDS_HELPMODE_UNKNOWN               0x801E
#define IDS_HELPSUBITEMNOTFOUND            0x801F
