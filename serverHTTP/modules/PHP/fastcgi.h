/* Taken from FCGI specification and adapted */ 

#define FASTCGILENGTH 0xffff

#define FASTCGIMAXNVPAIR 0x7fff

/*
 * Value for version component of FCGI_Header
 */
#define FCGI_VERSION_1           1

typedef struct __attribute__((__packed__)) {
    unsigned char version;
    unsigned char type;
    unsigned short requestId;
    unsigned short contentLength;
    unsigned char paddingLength;
    unsigned char reserved;
    char contentData[FASTCGILENGTH]; 
} FCGI_Header;

#define FCGI_HEADER_SIZE           8

/*
 * Value for requestId component of FCGI_Header
 */
#define FCGI_NULL_REQUEST_ID     0

/*
 * Values for type component of FCGI_Header
 */
#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11
#define FCGI_MAXTYPE (FCGI_UNKNOWN_TYPE)



typedef struct __attribute__((__packed__)) {
            unsigned char type;    
            unsigned char reserved[7];
} FCGI_UnknownTypeBody;


typedef struct __attribute__((__packed__)) {
    unsigned short role;
    unsigned char flags;
    unsigned char unused[5];
}  FCGI_BeginRequestBody;


 typedef struct __attribute__((__packed__)) {
            unsigned int appStatus;
            unsigned char protocolStatus;
            unsigned char reserved[3];
} FCGI_EndRequestBody;




/*
 * Values for protocolStatus component of FCGI_EndRequestBody
 */
#define FCGI_REQUEST_COMPLETE 0
#define FCGI_CANT_MPX_CONN    1
#define FCGI_OVERLOADED       2
#define FCGI_UNKNOWN_ROLE     3

/*
 * Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
 */
#define FCGI_MAX_CONNS  "FCGI_MAX_CONNS"
#define FCGI_MAX_REQS   "FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS "FCGI_MPXS_CONNS"

/*
 * Mask for flags component of FCGI_BeginRequestBody
 */
#define FCGI_KEEP_CONN  1

#define FCGI_RESPONDER  1
#define FCGI_AUTHORIZER 2
#define FCGI_FILTER     3

