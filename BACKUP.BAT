@Echo Off
Verify On
PKZIP -f -o CATALA
PKZIP -f -o DEUTSCH
PKZIP -f -o ENGLISH
PKZIP -f -o ESPANOL
PKZIP -f -o FRANCAIS
PKZIP -f -o ITALIANO
PKZIP -f -o -x*.zip SOURCE
PKZIP -o SOURCE *.zip
PKZIP -b -f -o -xsource.zip a:CLOCK32
PKZIP -b -o a:CLOCK32 source.zip
