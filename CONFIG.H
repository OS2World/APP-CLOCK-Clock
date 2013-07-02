/******************************************************************* CONFIG.H
 *                                                                          *
 *                     Configure Dialog definitions                         *
 *                                                                          *
 ****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
  int    Id ;
  BOOL   Analog ;
  BOOL   Hour24 ;
  BOOL   HideControls ;
  BOOL   Chime ;
  BOOL   Float ;
  BOOL   Animate ;
  BOOL   Seconds ;
  USHORT AlertType ;
  USHORT AlertLevels [2] [2] ;
} CONFIG_PARMS, *PCONFIG_PARMS ;

extern FNWP ConfigureProcessor ;

#endif
