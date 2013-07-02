/****************************************************************** EXCEPT.H
 *                                                                         *
 *                            Exception Handler                            *
 *                                                                         *
 ***************************************************************************/

#ifndef EXCEPT_H
#define EXCEPT_H

extern ULONG APIENTRY ExceptionHandler
(
  PEXCEPTIONREPORTRECORD pExceptionReportRecord,
  PEXCEPTIONREGISTRATIONRECORD pExceptionRegistrationRecord,
  PCONTEXTRECORD pContextRecord,
  PVOID pDispatcherContext
) ;

#endif
