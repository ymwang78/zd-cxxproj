#ifndef __wsse_h__
#define __wsse_h__

#include <utiny/utiny_config.h>

#define SOAP_WSSE_NONCELEN	(20)

const char* UBEDA_API wsse_calc_password(struct soap *psoap, const char *created, const char *nonce, int noncelen, const char *password);
int UBEDA_API soap_out_security(soap* psoap, char* username, char* password, char* nonce, char* created, int soap_type_string);

#endif
