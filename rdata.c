/*
 * rdata.c
 *
 * rdata implementation
 *
 * a Net::DNS like library for C
 *
 * (c) NLnet Labs, 2004
 *
 * See the file LICENSE for the license
 */

#include <config.h>

#include <ldns/rdata.h>
#include <ldns/error.h>
#include <ldns/str2host.h>

#include "util.h"

/*
 * Access functions 
 * do this as functions to get type checking
 */

/* read */
uint16_t
ldns_rdf_size(ldns_rdf *rd)
{
	return rd->_size;
}

ldns_rdf_type
ldns_rdf_get_type(ldns_rdf *rd)
{
	return rd->_type;
}

uint8_t *
ldns_rdf_data(ldns_rdf *rd)
{
	return rd->_data;
}

/* write */
void
ldns_rdf_set_size(ldns_rdf *rd, uint16_t s)
{
	rd->_size = s;
}

void
ldns_rdf_set_type(ldns_rdf *rd, ldns_rdf_type t)
{
	rd->_type = t;
}

void
ldns_rdf_set_data(ldns_rdf *rd, void *d)
{
	/* only copy the pointer */
	rd->_data = d;
}

/**
 * Allocate a new ldns_rdf structure 
 * fill it and return it
 */
ldns_rdf *
ldns_rdf_new(uint16_t s, ldns_rdf_type t, void *d)
{
	ldns_rdf *rd;
	rd = MALLOC(ldns_rdf);
	if (!rd) {
		return NULL;
	}

	ldns_rdf_set_size(rd, s);
	ldns_rdf_set_type(rd, t);
	ldns_rdf_set_data(rd, d);
	return rd;
}

void 
ldns_rdf_free(ldns_rdf *rd)
{
	FREE(rd);
}

/**
 * Create a new rdf from a string
 * \param[in] str string to use
 * \param[in] t   type to use
 * \return ldns_rdf*
 */
ldns_rdf *
ldns_rdf_new_frm_str(const char *str, ldns_rdf_type t)
{
	ldns_rdf *rd;
	ldns_status stat;
	
	switch(t) {
        	case LDNS_RDF_TYPE_NONE:
			break;
	        case LDNS_RDF_TYPE_DNAME:
			stat = ldns_str2rdf_dname(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_INT8:
			stat = ldns_str2rdf_int8(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_INT16:
			stat = ldns_str2rdf_int16(&rd, (const uint8_t*) str);
			break;
		case LDNS_RDF_TYPE_INT32:
			stat = ldns_str2rdf_int32(&rd, (const uint8_t*) str);
			break;
		case LDNS_RDF_TYPE_A:
			stat = ldns_str2rdf_a(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_AAAA:
			stat = ldns_str2rdf_aaaa(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_STR:
			stat = ldns_str2rdf_str(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_APL:
			stat = ldns_str2rdf_apl(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_B64:
			stat = ldns_str2rdf_b64(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_HEX:
			stat = ldns_str2rdf_hex(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_NSEC:
			stat = ldns_str2rdf_nsec(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_TYPE:
			stat = ldns_str2rdf_type(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_CLASS:
			stat = ldns_str2rdf_class(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_CERT:
			stat = ldns_str2rdf_cert(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_ALG:
			stat = ldns_str2rdf_alg(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_UNKNOWN:
			stat = ldns_str2rdf_unknown(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_TIME:
			stat = ldns_str2rdf_time(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_TSIGTIME:
			stat = ldns_str2rdf_tsigtime(&rd, (const uint8_t*) str);
			break;
       		case LDNS_RDF_TYPE_SERVICE:
			stat = ldns_str2rdf_service(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_LOC:
			stat = ldns_str2rdf_loc(&rd, (const uint8_t*) str);
			break;
        	case LDNS_RDF_TYPE_WKS:
			stat = ldns_str2rdf_wks(&rd, (const uint8_t*) str);
			break;
       	 	case LDNS_RDF_TYPE_NSAP:
			stat = ldns_str2rdf_nsap(&rd, (const uint8_t*) str);
			break;
		default:
			/* default default ??? */
			break;
	}
	if (LDNS_STATUS_OK != stat) {
		return NULL;
	} else {
		ldns_rdf_set_type(rd, t);
		return rd;
	}
}

/**
 * remove \\DDD, \\[space] and other escapes from the input
 * See RFC 1035, section 5.1
 * Return the length of the string or a negative error
 * code
 */
ldns_status
ldns_octet(char *word, size_t *length)
{
    char *s; char *p;
    *length = 0;

    for (s = p = word; *s != '\0'; s++,p++) {
        switch (*s) {
            case '.':
                if (s[1] == '.') {
                    fprintf(stderr,"Empty label");
		    return LDNS_STATUS_EMPTY_LABEL;
                }
                *p = *s;
                *length++;
                break;
            case '\\':
                if ('0' <= s[1] && s[1] <= '9' &&
                    '0' <= s[2] && s[2] <= '9' &&
                    '0' <= s[3] && s[3] <= '9')
                {
                    /* \DDD seen */
                    int val = ((s[1] - '0') * 100 +
                           (s[2] - '0') * 10 + (s[3] - '0'));

                    if (0 <= val && val <= 255) {
                        /* this also handles \0 */
                        s += 3;
                        *p = val;
                        *length++;
                    } else {
                        return LDNS_STATUS_DDD_OVERFLOW;
                    }
                } else {
                    /* an espaced character, like \<space> ? 
                    * remove the '\' keep the rest */
                    *p = *++s;
                    *length++;
                }
                break;
            case '\"':
                /* non quoted " Is either first or the last character in
                 * the string */

                *p = *++s; /* skip it */
                *length++;
		/* I'm not sure if this is needed in libdns... MG */
                if ( *s == '\0' ) {
                    /* ok, it was the last one */
                    *p  = '\0'; 
		    return LDNS_STATUS_OK;
                }
                break;
            default:
                *p = *s;
                *length++;
                break;
        }
    }
    *p = '\0';
    return LDNS_STATUS_OK;
}
