/*
 * error.h
 *
 * error reporting function and definition
 *
 * a Net::DNS like library for C
 *
 * (c) NLnet Labs, 2004
 *
 * See the file LICENSE for the license
 */

#ifndef _ERROR_H
#define _ERROR_H

enum ldns_enum_status 
{
	LDNS_STATUS_OK,	
	LDNS_STATUS_EMPTY_LABEL,
	LDNS_STATUS_LABEL_OVERFLOW,
	LDNS_STATUS_DOMAINNAME_OVERFLOW,
	LDNS_STATUS_DOMAINNAME_UNDERFLOW,
	LDNS_STATUS_DDD_OVERFLOW,
	LDNS_STATUS_PACKET_OVERFLOW,
	LDNS_STATUS_INVALID_POINTER,
	LDNS_STATUS_MEM_ERR,
	LDNS_STATUS_INTERNAL_ERR,
	LDNS_STATUS_ERR,
	LDNS_STATUS_INT_EXP,
	LDNS_STATUS_INVALID_IP4,
	LDNS_STATUS_INVALID_IP6,
	LDNS_STATUS_INVALID_STR,
	LDNS_STATUS_INVALID_B64,
	LDNS_STATUS_INVALID_HEX,
	LDNS_STATUS_NETWORK_ERROR,
	LDNS_STATUS_ADDRESS_ERROR,
	LDNS_STATUS_UNKNOWN_INET,
	LDNS_STATUS_NOT_IMPL
};
typedef enum ldns_enum_status ldns_status;

#endif /* _ERROR_H */
